import net.spy.memcached.*;
import net.spy.memcached.transcoders.SerializingTranscoder;
import net.spy.memcached.MyMemcachedClient;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.MyBinaryConnectionFactory;
import net.spy.memcached.AddrUtil;
import java.net.InetSocketAddress;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.CountDownLatch;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Arrays;
import java.util.Random;
import java.io.PrintWriter;
import java.io.File;

//comcast protobuf import
import com.comcast.data.protobuf.message.scheduler.RecordingProto.Recording;
import com.comcast.data.protobuf.message.scheduler.cache.RecordingListProto.RecordingList;
import com.comcast.data.protobuf.message.scheduler.RecordingSegmentProto.RecordingSegment;
import com.comcast.data.protobuf.message.scheduler.ListingDataProto.ListingData;
import com.hercules.serverdata.data.NotRecordReasonProto.NotRecordReason;
import com.hercules.serverdata.data.RecordingStatusProto.RecordingStatus;
import com.hercules.serverdata.data.RecordingTypeProto.RecordingType;
import com.hercules.serverdata.data.DeletePriorityProto.DeletePriority;
import com.hercules.serverdata.data.NotRecordReasonProto;

class Test {
	private static final int NUM_THREADS = 1;
	private static final long CACHE_SIZE = 48L*1024L*1024L; //2L*1024L*1024L*1024L;//2GB
	private static final double PERCENT_FULL = 0.8;
	private static final int SMALL_SIZE_THRESH = 5*1024;
	private static final int MEDIUM_SIZE_THRESH = 20*1024;
	private static final double SMALL_BLEND = 0.85; //240 byte objs/480 byte objs
	private static final double MEDIUM_BLEND = 0.5; //Small objs/ large objs
	private static final double LARGE_BLEND = 0.75; //10k byte objs/100k byte objs
	private static final double T_STEP = 10.0;
	private static final String loadFile = new String("server_a_load.dat");
	private static final ArrayList<Stats> serverReqs = new ArrayList<Stats>();
	private static final double[] modPercent = new double[] {0.0, 0.1, 0.3, 0.5, 0.7, 0.9, 1.0};

	private static class Stats {
		static final int tStep = 10; //time step in seconds

		public Stats() {
		}

		public enum StatID {
			step,
			req_per_sec,
			get,
			set,
			delete,
			incr,
			decr,
			cas,
			bad_cas,
			touch,
			small_obj,
			small_get,
			small_set,
			small_delete,
			medium_obj,
			medium_get,
			medium_set,
			medium_delete,
			large_obj,
			large_get,
			large_set,
			large_delete
		};

		void setStat(int ind, double value) {
			switch(ind) {
				case 0: interval = value;
				case 1: reqs = value;
				case 2: gets = value;
				case 3: sets = value;
				case 4: deletes = value;
				case 10: small_reqs = value;
				case 11: small_gets = value;
				case 12: small_sets = value;
				case 13: small_deletes = value;
				case 14: medium_reqs = value;
				case 15: medium_gets = value;
				case 16: medium_sets = value;
				case 17: medium_deletes = value;
				case 18: large_reqs = value;
				case 19: large_gets = value;
				case 20: large_sets = value;
				case 21: large_deletes = value;
			}
		}

		public double interval;
		public double reqs;
		public double gets;
		public double sets;
		public double deletes;
		public double small_reqs;
		public double small_gets;
		public double small_sets;
		public double small_deletes;
		public double medium_reqs;
		public double medium_gets;
		public double medium_sets;
		public double medium_deletes;
		public double large_reqs;
		public double large_gets;
		public double large_sets;
		public double large_deletes;
	}

	private static class BenchThread implements Runnable {
		private class Requests {
			public Requests() {}
			public double time; //the max time to complete these operations
			public int num_gets;
			public int num_small_gets;
			public int num_medium_gets;
			public int num_large_gets;
			public int num_sets;
			public int num_small_sets;
			public int num_medium_sets;
			public int num_large_sets;
			public int num_mods; //either get/set pair or register/invoke
			public int num_small_mods;
			public int num_medium_mods;
			public int num_large_mods;
			public int total_ops;

			public void setTotalOps() {
				total_ops = 0;
				total_ops += num_small_gets;
				total_ops += num_medium_gets;
				total_ops += num_large_gets;
				total_ops += num_small_sets;
				total_ops += num_medium_sets;
				total_ops += num_large_sets;
				total_ops += num_small_mods;
				total_ops += num_medium_mods;
				total_ops += num_large_mods;
			}
			
			public String toString() {
				return "Requests --\n" +
					"\tsmall_sets: " + num_small_sets +
					"\n\tmedium_sets: " + num_medium_sets +
					"\n\tlarge_sets: " + num_large_sets;
			}
		}
		public Requests reqs;
		private boolean useInvoke;
		private int thread_num;
		private int num240;
		private int num480;
		private int num10k;
		private int num100k;
		private final String k240 = ":RecordingProto240";
		private final String k480 = ":RecordingProto480";
		private final String k10k = ":RecordingListProto10k";
		private final String k100k = ":RecordingListProto100k";
		private Recording r240;
		private Recording r480;
		private RecordingList r10k;
		private RecordingList r100k; 
		public MyMemcachedClient cli;
		public SerializingTranscoder ncTranscoder;
		public CountDownLatch latch;
		public BenchThread(int thread_num, CountDownLatch latch, double t, int gets, int s_gets, int m_gets, int l_gets, int sets, int s_sets, int m_sets, int l_sets, int mods, int s_mods, int m_mods, int l_mods, int num240, int num480, int num10k, int num100k) {
			reqs = new Requests();
			reqs.time = t;
			reqs.num_gets = gets;
			reqs.num_small_gets = s_gets;
			reqs.num_medium_gets = m_gets;
			reqs.num_large_gets = l_gets;
			reqs.num_sets = sets;
			reqs.num_small_sets = s_sets;
			reqs.num_medium_sets = m_sets;
			reqs.num_large_sets = l_sets;
			reqs.num_mods = mods;
			reqs.num_small_mods = s_mods;
			reqs.num_medium_mods = m_mods;
			reqs.num_large_mods = l_mods;
			reqs.setTotalOps();
			this.thread_num = thread_num;
			this.latch = latch;
			this.num240 = num240;
			this.num480 = num480;
			this.num10k = num10k;
			this.num100k = num100k;
			r240 = proto240().build();
			r480 = proto480().build();
			r100k = proto100K().build();
			r10k = proto10K().build();
			ncTranscoder = new SerializingTranscoder();
			ncTranscoder.setCompressionThreshold(1000*1024*1024);
		}

		//use reqs object to run benchmark
		public void run() {
			System.out.println("Running benchmark with " + reqs.num_mods + " mods.");
            
			cli = null;
			try {
				cli = new MyMemcachedClient(
					new MyBinaryConnectionFactory(), AddrUtil.getAddresses(cacheURI));
			}
			catch(Exception e) {
				System.out.println("Exception on connecting: " + e);
				return;
			}

			//debug_bench();
			bench();
			latch.countDown();

			cli.shutdown();
		}


		public void debug_bench() {
			//start by registering all functions
			registerFcn("listDelete", "./protolib.so:recordingListSetDelete");
			registerFcn("delete", "./protolib.so:setRecordDelete");
			registerFcn("failure", "./protolib.so:setFailureReason");
			registerFcn("lowCost", "./protolib.so:lowCost");

			java.util.Scanner sin = new java.util.Scanner(System.in);
			String in = "";
			while(in.equals("")) {
/*				//small set 240
					int rInd = randInt(0, num240);
					String k = String.format("%02d:%06d"+k240, this.thread_num, rInd);
					
					Future<Boolean> f = cli.set(k, 0, r240.toByteArray(), ncTranscoder);
					try {
						if(!f.get()) System.out.println("Set failed");
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}
				//small get 240
					Object obj = cli.get(k, ncTranscoder);
					if(obj == null) System.out.println("Get failed");
				//small mod 240
					Object o = cli.invoke(k, "failure", false);
					if(o == null) System.out.println("Invoke failed");

				//large set
*/					int rInd = randInt(0, num100k);
					String k = String.format("%02d:%06d"+k100k, this.thread_num, rInd);
					System.out.println("Key is " + k);
					Future<Boolean> f = cli.set(k, 0, r100k.toByteArray(), ncTranscoder);
					try {
						if(!f.get()) System.out.println("Set failed");
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}

				//large get
					Object obj = cli.get(k, ncTranscoder);
					if(obj == null) System.out.println("Get failed");
				
				//large mod	
					Object o = cli.invoke(k, "listDelete", "hahahahaha", true, false);
					if(o == null) System.out.println("100k invoke failed");



				System.out.println("Enter to quit");
				in = sin.nextLine();	
			}

		}

		//Invoke
		public void bench() {
			//start by registering all functions
			//registerFcn("nop", "./protolib.so:nop");
			registerFcn("listDelete", "./protolib.so:recordingListSetDelete");
			registerFcn("delete", "./protolib.so:setRecordDelete");
			registerFcn("failure", "./protolib.so:setFailureReason");
			registerFcn("lowCost", "./protolib.so:lowCost");

			boolean no_set = false;
			boolean no_reply = false;

			int num_240_gets = (int)(reqs.num_small_gets*SMALL_BLEND);
			int num_480_gets = reqs.num_small_gets - num_240_gets;
			int num_240_sets = (int)(reqs.num_small_sets*SMALL_BLEND);
			int num_480_sets = reqs.num_small_sets - num_240_sets;
			int num_240_mods = (int)(reqs.num_small_mods*SMALL_BLEND);
			int num_480_mods = reqs.num_small_mods - num_240_mods;
			//perform mix of operations on keys in cache
			//reqs.num_medium_mods += reqs.num_large_mods;
			//reqs.num_large_mods = 0;
			int total_ops = reqs.total_ops;
			while(total_ops > 0) {
				//small set 240
				if(num_240_sets > 0) {
					int rInd = randInt(0, num240);
					String k = String.format("%02d:%06d"+k240, this.thread_num, rInd);
					
					Future<Boolean> f = cli.set(k, 0, r240.toByteArray(), ncTranscoder);
					try {
						f.get();
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}
					num_240_sets--;
					total_ops--;
				}
				//small set 480
				if(num_480_sets > 0) {
					int rInd = randInt(0, num480);
					String k = String.format("%02d:%06d"+k480, this.thread_num, rInd);
					
					Future<Boolean> f = cli.set(k, 0, r480.toByteArray(), ncTranscoder);
					try {
						f.get();
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}
					num_480_sets--;
					total_ops--;
				}
				//medium set
				if(reqs.num_medium_sets > 0) {
					int rInd = randInt(0, num10k);
					String k = String.format("%02d:%06d"+k10k, this.thread_num, rInd);
					
					Future<Boolean> f = cli.set(k, 0, r10k.toByteArray(), ncTranscoder);
					try {
						f.get();
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}
					reqs.num_medium_sets--;
					total_ops--;
				}
				//large set
				if(reqs.num_large_sets > 0) {
					int rInd = randInt(0, num100k);
					String k = String.format("%02d:%06d"+k100k, this.thread_num, rInd);
					
					Future<Boolean> f = cli.set(k, 0, r100k.toByteArray(), ncTranscoder);
					try {
						f.get();
					} catch (Exception e) {
						System.out.println("couldn't get set state");
					}
					reqs.num_large_sets--;
					total_ops--;
				}
				//small get 240
				if(num_240_gets > 0) {
					int rInd = randInt(0, num240);
					String k = String.format("%02d:%06d"+k240, this.thread_num, rInd);	
					Object obj = cli.get(k, ncTranscoder);
					num_240_gets--;
					total_ops--;
				}
				//small get 480
				if(num_480_gets > 0) {
					int rInd = randInt(0, num480);
					String k = String.format("%02d:%06d"+k480, this.thread_num, rInd);	
					Object obj = cli.get(k, ncTranscoder);
					num_480_gets--;
					total_ops--;
				}
				//medium get
				if(reqs.num_medium_gets > 0) {
					int rInd = randInt(0, num10k);
					String k = String.format("%02d:%06d"+k10k, this.thread_num, rInd);
					Object obj = cli.get(k, ncTranscoder);
					reqs.num_medium_gets--;
					total_ops--;
				}
				//large get
				if(reqs.num_large_gets > 0) {
					int rInd = randInt(0, num100k);
					String k = String.format("%02d:%06d"+k100k, this.thread_num, rInd);
					Object obj = cli.get(k, ncTranscoder);
					reqs.num_large_gets--;
					total_ops--;
				}
				//small mod 240
				if(num_240_mods > 0) {
					int rInd = randInt(0, num240);
					String k = String.format("%02d:%06d"+k240, this.thread_num, rInd);	
					Object o = cli.invoke(k, "lowCost", "lolololol", no_set, no_reply);
					if(o == null) {
						System.out.println(String.format("Invoke 240 failed: %s", k));
					}

					num_240_mods--;
					total_ops--;
				}
				//small mod 480
				if(num_480_mods > 0) {
					int rInd = randInt(0, num480);
					String k = String.format("%02d:%06d"+k480, this.thread_num, rInd);	
					Object o = cli.invoke(k, "lowCost", "hahahahaha", no_set, no_reply);
					if(o == null) System.out.println(String.format("Invoke 480 failed: %s", k));
					num_480_mods--;
					total_ops--;
				}
				//medium mod
				if(reqs.num_medium_mods > 0) {
					int rInd = randInt(0, num10k);
					String k = String.format("%02d:%06d"+k10k, this.thread_num, rInd);
					Object o = cli.invoke(k, "lowCost", "roflrofl", no_set, no_reply);
					if(o == null) System.out.println("10k invoke failed");
					reqs.num_medium_mods--;
					total_ops--;
				}
				//large mod
				if(reqs.num_large_mods > 0) {
					int rInd = randInt(0, num100k);
					String k = String.format("%02d:%06d"+k100k, this.thread_num, rInd);
					Object o = cli.invoke(k, "lowCost", "coptercopter", no_set, no_reply);
					if(o == null) System.out.println("100k invoke failed");
					reqs.num_large_mods--;
					total_ops--;
				}
			}
		}

		public int randInt(int min, int max) {
			Random random = new Random();
			return random.nextInt((max-min) + 1) + min;
		}
		
		//Each thread just needs one of each size. Accessing the same object
		//multiple times should still reflect the same CPU usage.
		public void setupCache() {
		/*		Object o = cli.invoke(k240, "failure", false);
				Object o2 = cli.invoke(k240, "delete", true);

				if(o != null) System.out.println("Failure set worked!");
				if(o2 != null) {
					System.out.println("Delete worked!");
					byte[] b = ((String)o2).getBytes();
					System.out.println(b);
					System.out.println(((String)o2).length());
					try {
						Recording r = Recording.parseFrom(b);
						System.out.println(r);
					} catch(Exception e) {
						System.out.println("Wasn't a good format...");
					}
				}

				cli.shutdown();*/
		}

		public void registerFcn(String fcnKey, String fcnPath) {
			System.out.printf("Registering %s:%s -> ", fcnKey, fcnPath);
			Future<Boolean> r = cli.register(fcnKey, fcnPath);

			try {
				if(r.get()) System.out.println("Registered!");
				else {
					System.out.println("Failed to load " + fcnPath);
					System.exit(-1);
				}
			} catch (Exception e) {
				System.out.println("Operation timed out.");
			}
		}

	}

	/**
	* Either proto240 or proto480 recordings can be used for the simple "change
	* recording status" use case.
	*/
	public static Recording.Builder proto240() {
	    Recording.Builder r =
		Recording.newBuilder()
		.setId("RP")
		.setAccountGuid("A")
		.setDeviceGuid("B")
		.setSourceGuid("C")
		.setEntityId("D")
		.setStartTime(90000)
		.setEndTime(100000)
		.setStationGuid("E")
		.setChannelNumber(99)
		.setDeletePriority(DeletePriority.P0)
		.setStatus(RecordingStatus.PendingNoConflict)
		.setRecordingType(RecordingType.OneTimeRecording)
		.setFailureReason("Because I'm right, and you're wrong, and I'm smart, and you're not")
		.setTitle("My Show: Prequel")
		.setDescription("It's me before I was me.")
		.setWatched(false)
		.setPayPerView(false)
		.setEntitlementId(24601)
		.setUndeleteStatus(RecordingStatus.PendingNoConflict)
		.setTimeDeleted(0)
		.addListingDataList(
			ListingData.newBuilder()
			.setAudioType("Mono")
			.setCaptionType("Closed")
			.setIsAdult(true)
			.setSeriesGuid("123456")
			.setIsSAP(true)
			.setIsSubTitled(true)
			.setIsThreeD(false)
			.setProgramStart(0)
			.setProgramEnd(-1)
			.setSeasonNumber(1)
			.setEpisodeNumber(1)
			.setEpisodeTitle("Adam Gets a Life"));

	    return r;
	}

	public static Recording.Builder proto480() {
	    Recording.Builder r =
		Recording.newBuilder()
		.setId("RP")
		.setAccountGuid("ABCDEFGHIJK")
		.setDeviceGuid("BCDEFGIJKLM")
		.setSourceGuid("CDEFGHKLMNQ")
		.setEntityId("DEFGHIMNOPQ")
		.setStartTime(90000)
		.setEndTime(100000)
		.setStationGuid("EFGHIJOPQRS")
		.setChannelNumber(99)
		.setDeletePriority(DeletePriority.P0)
		.setStatus(RecordingStatus.PendingNoConflict)
		.setRecordingType(RecordingType.OneTimeRecording)
		.setFailureReason("Because I'm right, and you're wrong, and I'm smart, and you're not")
		.setTitle("My Show: The Prequel The Sequel")
		.setDescription("It's me before I was me. Except now I am already me and dealing with the consequences of having become me.")
		.setWatched(false)
		.setPayPerView(false)
		.setEntitlementId(24601)
		.setUndeleteStatus(RecordingStatus.PendingNoConflict)
		.setTimeDeleted(0)
		.addListingDataList(
			ListingData.newBuilder()
			.setAudioType("Stereo Surround Sound")
			.setCaptionType("Closed")
			.setIsAdult(true)
			.setSeriesGuid("123456789")
			.setIsSAP(true)
			.setIsSubTitled(true)
			.setIsThreeD(false)
			.setProgramStart(0)
			.setProgramEnd(-1)
			.setSeasonNumber(1)
			.setEpisodeNumber(1)
			.setEpisodeTitle("Adam Loses the Life He Just Got, but Not Like Death or Anything. Just Metaphorically"));

	    return r;
	}

	/**
	* Either proto10K or proto100K recordings can be used for the more complex "add/delete
	* a recording" use case.
	*/
	public static RecordingList.Builder proto10K() {
		return recordingProto(10*1024);
	}

	public static RecordingList.Builder proto100K() {
		return recordingProto(100*1024);
	}

	public static RecordingList.Builder recordingProto(int size) {
		//RecordingList is just repeated recording objects
		RecordingList.Builder r = RecordingList.newBuilder();
		while(r.build().toByteArray().length < size) {
			r.addRecordings(proto480());
		}
		return r;
	}

	public static String host;
	public static String port;
	public static String cacheURI;
	public static void main(String[] args) throws InterruptedException {
		readServerData(serverReqs, loadFile);

		if(args.length < 2) {
			System.out.println("Usage: java Test <host> <port#>");
			System.exit(-1);
		}
	
		cacheURI = args[0] + ":" + args[1];

		for(int num_threads = NUM_THREADS; num_threads <= NUM_THREADS; num_threads++) {
			for(int p = 0; p < modPercent.length; p++) {
				//for each time interval, set up the number of request types
	
				int[] objNums = initCache(num_threads);
				
				if(objNums == null) {
					System.out.println("Could not initialize cache");
					System.exit(-1);
				}

				System.out.println(String.format("Cache initialization success:\n\tSmall240: %d\n\tSmall480: %d\n\tLarge10k: %d\n\tLarge100k: %d", objNums[0], objNums[1], objNums[2], objNums[3]));
				System.out.printf("%f mods\n", modPercent[p]);
				for(int i = 0; i < serverReqs.size(); i++) {
					Stats s = serverReqs.get(i); //we can actually ignore deletes

					int mods = (int)(s.sets*modPercent[p])/num_threads;
					int s_mods = (int)(s.small_sets*modPercent[p])/num_threads;
					int m_mods = (int)(s.medium_sets*modPercent[p])/num_threads;
					int l_mods = (int)(s.large_sets*modPercent[p])/num_threads;

					//take percentage of maximum allowable pairs
					if(s.sets < s.gets) mods = (int)(s.sets*modPercent[p])/num_threads;
					else mods = (int)(s.gets*modPercent[p])/num_threads;

					if(s.small_sets < s.small_gets) s_mods = (int)(s.small_sets*modPercent[p])/num_threads;
					else s_mods = (int)(s.small_gets*modPercent[p])/num_threads;

					if(s.medium_sets < s.medium_gets) m_mods = (int)(s.medium_sets*modPercent[p])/num_threads;
					else m_mods = (int)(s.medium_gets*modPercent[p])/num_threads;

					if(s.large_sets < s.large_gets) l_mods = (int)(s.large_sets*modPercent[p])/num_threads;
					else l_mods = (int)(s.large_gets*modPercent[p])/num_threads;

					int gets = (int)s.gets/num_threads - mods;
					int sets = (int)s.sets/num_threads - mods;
					int s_gets = (int)s.small_gets/num_threads - s_mods;
					int m_gets = (int)s.medium_gets/num_threads - m_mods;
					int l_gets = (int)s.large_gets/num_threads - l_mods;
					int s_sets = (int)s.small_sets/num_threads - s_mods;
					int m_sets = (int)s.medium_sets/num_threads - m_mods;
					int l_sets = (int)s.large_sets/num_threads - l_mods;
					//identical threads for reach operation
					CountDownLatch latch = new CountDownLatch(num_threads);
					for(int thread = 0; thread < num_threads; thread++) {
						BenchThread b = new BenchThread(thread, latch, T_STEP, gets, s_gets, m_gets, l_gets, sets, s_sets, m_sets, l_sets, mods, s_mods, m_mods, l_mods, objNums[0]/num_threads, objNums[1]/num_threads, objNums[2]/num_threads, objNums[3]/num_threads);
						Thread t = new Thread(b);
						t.start();
					}
					System.out.println("Waiting for latch");
					latch.await();
					System.out.println("Latch passed");
				}
			}
			//runBench(args);

		} 	
	}

	public static boolean readServerData(ArrayList<Stats> reqs, String file) {
		Scanner fin = null;
		try {
			fin = new Scanner(new File(file));
		} catch(Exception e) {
			System.out.println("Error: " + e);
			return false;
		}

		String headerText = fin.nextLine();
		ArrayList<String> header = new ArrayList<String>(Arrays.asList(headerText.split(",")));
		while(fin.hasNext()) {
			Stats s = new Stats();
			String[] line = fin.nextLine().split(",");
			for(int i = 0; i < line.length; i++) {
				s.setStat(i, Double.parseDouble(line[i]));
			}
			reqs.add(s);
		}

		return true;
	}

	public static int[] initCache(int num_thread) {
		MyMemcachedClient cli = null;
		try {
			cli = new MyMemcachedClient(
				new MyBinaryConnectionFactory(), AddrUtil.getAddresses(cacheURI));
		}
		catch(Exception e) {
			System.out.println("Exception on connecting: " + e);
			return null;
		}
		//stop compression
		SerializingTranscoder ncTranscoder = new SerializingTranscoder();
		ncTranscoder.setCompressionThreshold(1000*1024*1024);

		//want to populate a 2GB cache with information for our thread count threads
		long mem_per_thread = (long)((CACHE_SIZE/num_thread)*PERCENT_FULL);
		System.out.println("Cache size = " + (long)CACHE_SIZE + ".   Mem/thread = " + mem_per_thread);
		//SMALL_BLEND gives 240/480 ratio
		//LARGE_BLEND gives 10k/100k ratio
		//MEDIUM_BLEND gives SMALL/LARGE ratio

		long numSmall = (long)(mem_per_thread*MEDIUM_BLEND);
		long num240 = (long)(numSmall*SMALL_BLEND);
		long num480 = numSmall - num240;
		long numLarge = mem_per_thread - numSmall;
		long num10k = (long)(numLarge*LARGE_BLEND);
		long num100k = numLarge - num10k;

		Recording r240 = proto240().build();
		Recording r480 = proto480().build();
		RecordingList r100k = proto100K().build();
		RecordingList r10k = proto10K().build();
		String k240 = ":RecordingProto240";
		String k480 = ":RecordingProto480";
		String k10k = ":RecordingListProto10k";
		String k100k = ":RecordingListProto100k";
		int r240Size = r240.toByteArray().length;
		int r480Size = r480.toByteArray().length;
		int r10kSize = r10k.toByteArray().length;
		int r100kSize = r100k.toByteArray().length;

		try {
			PrintWriter fout = new PrintWriter("Proto240.txt", "UTF-8");
			fout.println(new String(r240.toByteArray()));
			fout.close();

			fout = new PrintWriter("Proto480.txt", "UTF-8");
			fout.println(new String(r480.toByteArray()));
			fout.close();

			fout = new PrintWriter("Proto10k.txt", "UTF-8");
			fout.println(new String(r10k.toByteArray()));
			fout.close();
	
			fout = new PrintWriter("Proto100k.txt", "UTF-8");
			fout.println(new String(r100k.toByteArray()));
			fout.close();
		} catch(Exception e) {
			System.out.println("Something didn't work");
		}

		int count240 = 0;
		int count480 = 0;
		int count10k = 0;
		int count100k = 0;

		System.out.println("Starting cache init");
		for(int i = 0; i < num_thread; i++) {
			//store a bunch of objects of each size;
			long currSize = num240;
			int objNum = 0;
			Future<Boolean> f;
			while(currSize > 0) {
				String k = String.format("%02d:%06d"+k240, i, objNum);
				f = cli.set(k, 0, r240.toByteArray(), ncTranscoder);	
				try {
					f.get();
				} catch (Exception e) {
					System.out.println("Couldn't get future state");
				}
				currSize -= r240Size;
				count240++;
				objNum++;
			}

			currSize = num480;
			objNum = 0;
			while(currSize > 0) {
				String k = String.format("%02d:%06d"+k480, i, objNum);
				f = cli.set(k, 0, r480.toByteArray(), ncTranscoder);	
				try {
					f.get();
				} catch (Exception e) {
					System.out.println("Couldn't get future state");
				}
				currSize -= r480Size;
				count480++;
				objNum++;
			}

			currSize = num10k;
			objNum = 0;
			while(currSize > 0) {
				String k = String.format("%02d:%06d"+k10k, i, objNum);
				f = cli.set(k, 0, r10k.toByteArray(), ncTranscoder);	
				try {
					f.get();
				} catch (Exception e) {
					System.out.println("Couldn't get future state");
				}
				currSize -= r10kSize;
				count10k++;
				objNum++;
			}

			currSize = num100k;
			objNum = 0;
			while(currSize > 0) {
				String k = String.format("%02d:%06d"+k100k, i, objNum);
				f = cli.set(k, 0, r100k.toByteArray(), ncTranscoder);	
				try {
					f.get();
				} catch (Exception e) {
					System.out.println("Couldn't get future state");
				}
				currSize -= r100kSize;
				count100k++;
				objNum++;
			}
		}
		System.out.println(String.format("Finish cache init. %d sets", count240+count480+count10k+count100k));
		cli.shutdown();
		return new int[] {(int)count240, (int)count480, (int)count10k, (int)count100k};
	}

	public static Recording makeRecording() {
		return null;
	}

	public static RecordingList makeRecordingList() {
		return null;
	}

}
