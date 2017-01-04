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
import java.util.List;
import java.util.Random;
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

class PerfTest {
	private static final int NUM_THREADS = 1;//12;
	private static final int NUM_ITERS = 100000;
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
	}

	private boolean useInvoke;
	private static long num240;
	private static long num480;
	private static long num10k;
	private static long num100k;
	private static final String k240 = ":RecordingProto240";
	private static final String k480 = ":RecordingProto480";
	private static final String k10k = ":RecordingListProto10k";
	private static final String k100k = ":RecordingListProto100k";
	private static Recording r240;
	private static Recording r480;
	private static RecordingList r10k;
	private static RecordingList r100k; 
	public static MyMemcachedClient cli;
	public static SerializingTranscoder ncTranscoder;
/*
	//use reqs object to run benchmark
	public void run() {
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
*/
/*
	//Invoke
	public void bench() {
		//start by registering all functions
		registerFcn("nop", "./protolib.so:nop");
		registerFcn("listDelete", "./protolib.so:recordingListSetDelete");
		registerFcn("delete", "./protolib.so:setRecordDelete");
		registerFcn("failure", "./protolib.so:setFailureReason");
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
		//for gets, check correctness?
		//small get 240
		if(num_240_gets > 0) {
			int rInd = randInt(0, num240);
			String k = String.format("%02d:%06d"+k240, this.thread_num, rInd);	
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
			Object o = cli.invoke(k, "failure", "lolololol", false, false);
			if(o == null) System.out.println("Invoke failed");
			num_240_mods--;
			total_ops--;
		}
		//small mod 480
		if(num_480_mods > 0) {
			int rInd = randInt(0, num480);
			String k = String.format("%02d:%06d"+k480, this.thread_num, rInd);	
			Object o = cli.invoke(k, "delete", "hahahahaha", false, false);
			num_480_mods--;
			total_ops--;
		}
		//medium mod
		if(reqs.num_medium_mods > 0) {
			int rInd = randInt(0, num10k);
			String k = String.format("%02d:%06d"+k10k, this.thread_num, rInd);
			Object o = cli.invoke(k, "listDelete", "roflrofl", true, false);
			if(o == null) System.out.println("10k invoke failed");
			reqs.num_medium_mods--;
			total_ops--;
		}
		//large mod
		if(reqs.num_large_mods > 0) {
			int rInd = randInt(0, num100k);
			String k = String.format("%02d:%06d"+k100k, this.thread_num, rInd);
			Object o = cli.invoke(k, "listDelete", "coptercopter", true, false);
			if(o == null) System.out.println("100k invoke failed");
			reqs.num_large_mods--;
			total_ops--;
		}
	}*/

	public static int randInt(int min, int max) {
		Random random = new Random();
		return random.nextInt((max-min)) + min;
	}

	public static void registerFcn(String fcnKey, String fcnPath) {
		Future<Boolean> r = cli.register(fcnKey, fcnPath);

		try {
			if(!r.get()) {
				System.out.println("Failed to load " + fcnPath);
				System.exit(-1);
			}
		} catch (Exception e) {
			System.out.println("Operation timed out.");
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
		r240 = proto240().build();
		r480 = proto480().build();
		r100k = proto100K().build();
		
		//for each object, run 1000 random get/set modifications and 1000 random register/invoke modificationsr10k = proto10K().build();
		ncTranscoder = new SerializingTranscoder();
		ncTranscoder.setCompressionThreshold(1000*1024*1024);
	
		if(args.length < 2) {
			System.out.println("Usage: java Test <host> <port#>");
			System.exit(-1);
		}
	
		cacheURI = args[0] + ":" + args[1];

		//number of 240, 480, 10k, and 100k objects
		int[] objNums = initCache(NUM_THREADS);
			
		if(objNums == null) {
			System.out.println("Could not initialize cache");
			System.exit(-1);
		}

		try {
			cli = new MyMemcachedClient(
				new MyBinaryConnectionFactory(), AddrUtil.getAddresses(cacheURI));
		}
		catch(Exception e) {
			System.out.println("Exception on connecting: " + e);
			System.exit(-1);
		}
		//for each object, run 1000 random get/set modifications and 1000 random register/invoke modifications
		boolean no_set = false;
		boolean no_reply = false;
		//240 Objects
		long time240r = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[0]);
			String k = String.format("%02d:%06d"+k240, 0, rInd);

			Object obj = cli.get(k, ncTranscoder);

			Recording.Builder r240b = Recording.newBuilder();

			try {
				r240b.mergeFrom((byte[])obj);
			} catch(Exception e) {
				System.out.println("[240 REG]Could not parse Recording: " + e);
				break;
			}

			r240b.setFailureReason("**Because I'm right, and you're wrong, and I'm smart, and you're not");

			Future<Boolean> f = cli.set(k, 0, r240b.build().toByteArray(), ncTranscoder);
			try {
				f.get();
			} catch (Exception e) {
				System.out.println("couldn't get set state");
			}
		}
		System.out.println("Get/Set 240 finished");
		time240r = System.nanoTime() - time240r;
		long time240m = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[0]);
			String k = String.format("%02d:%06d"+k240, 0, rInd);
			registerFcn("failure", "./protolib.so:setFailureReason");

			Object obj = cli.invoke(k, "failure", "**Because I'm right, and you're wrong, and I'm smart, and you're not", no_set, no_reply); //set object and reply
			try {
				Recording.Builder r240 = Recording.newBuilder().mergeFrom((byte[])obj);
			} catch(Exception e) {
				System.out.println("[240 MOD]Could not parse Recording: " + e);
				break;
			}
		}
		System.out.println("Register/Invoke 240 finished");
		time240m = System.nanoTime() - time240m;

		//480 Objects
		long time480r = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[1]);
			String k = String.format("%02d:%06d"+k480, 0, rInd);

			Object obj = cli.get(k, ncTranscoder);

			Recording.Builder r480b = Recording.newBuilder();

			try {
				r480b.mergeFrom((byte[])obj);
			} catch(Exception e) {
				System.out.println("[480 REG]Could not parse Recording: " + e);
				break;
			}

			r480b.setFailureReason("**Because I'm right, and you're wrong, and I'm smart, and you're not");

			Future<Boolean> f = cli.set(k, 0, r480b.build().toByteArray(), ncTranscoder);
			try {
				f.get();
			} catch (Exception e) {
				System.out.println("couldn't get set state");
			}
		}
		System.out.println("Get/Set 480 finished");
		time480r = System.nanoTime() - time480r;

		long time480m = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[1]);
			String k = String.format("%02d:%06d"+k480, 0, rInd);
			registerFcn("failure", "./protolib.so:setFailureReason");

			Object obj = cli.invoke(k, "failure", "**Because I'm right, and you're wrong, and I'm smart, and you're not", no_set, no_reply); //set object and reply
			
			Recording r480 = null; 
			try {
				r480 = Recording.parseFrom((byte[])obj); //make sure we can read this object
			} catch(Exception e) {
				System.out.println("[480 MOD]Could not parse Recording: " + e);
				break;
			}
		}
		System.out.println("Register/Invoke 480 finished");
		time480m = System.nanoTime() - time480m;

		long time10kr = System.nanoTime();
		//10k Objects
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[2]);
			String k = String.format("%02d:%06d"+k10k, 0, rInd);

			Object obj = cli.get(k, ncTranscoder);
			
			RecordingList.Builder r10kb = RecordingList.newBuilder();

			try {
				r10kb.mergeFrom((byte[])obj);
			} catch(Exception e) {
				System.out.println("[10k REG]Could not parse Recording: " + e);
				break;
			}

			Recording.Builder r480b = r10kb.getRecordingsBuilder(2);
			r480b.setStatus(RecordingStatus.Deleted);


			Future<Boolean> f = cli.set(k, 0, r10kb.build().toByteArray(), ncTranscoder);
			try {
				f.get();
			} catch (Exception e) {
				System.out.println("couldn't get set state");
			}
		}
		System.out.println("Get/Set 10k finished");
		time10kr = System.nanoTime() - time10kr;
		
		long time10km = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[2]);
			String k = String.format("%02d:%06d"+k10k, 0, rInd);
			registerFcn("setDelete", "./protolib.so:nop");

			Object obj = cli.invoke(k, "setDelete", "22", no_set, no_reply); //set object and reply

			RecordingList r10k = null; 
			try {
				r10k = RecordingList.parseFrom((byte[])obj); //make sure we can read this object
			} catch(Exception e) {
				System.out.println("[10k MOD]Could not parse Recording: " + e);
				break;
			}
		}
		System.out.println("Register/Invoke 10k finished");
		time10km = System.nanoTime() - time10km;
		
		long time100kr = System.nanoTime();
		//100k Objects
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[3]);
			String k = String.format("%02d:%06d"+k100k, 0, rInd);

			Object obj = cli.get(k, ncTranscoder);
			
			RecordingList.Builder r100kb = RecordingList.newBuilder();

			try {
				r100kb.mergeFrom((byte[])obj);
			} catch(Exception e) {
				System.out.println("[100k REG]Could not parse Recording: " + e);
				break;
			}

			Recording.Builder r480b = r100kb.getRecordingsBuilder(2);
			r480b.setStatus(RecordingStatus.Deleted);


			Future<Boolean> f = cli.set(k, 0, r100kb.build().toByteArray(), ncTranscoder);
			try {
				f.get();
			} catch (Exception e) {
				System.out.println("couldn't get set state");
			}
		}
		System.out.println("Get/Set 100k finished");
		time100kr = System.nanoTime() - time100kr;

		long time100km = System.nanoTime();
		for(int i = 0; i < NUM_ITERS; i++) {
			int rInd = randInt(0, objNums[3]);
			String k = String.format("%02d:%06d"+k100k, 0, rInd);
			registerFcn("setDelete", "./protolib.so:nop");

			Object obj = cli.invoke(k, "setDelete", "22", no_set, no_reply); //set object and reply

			RecordingList r100k = null; 
			try {
				r100k = RecordingList.parseFrom((byte[])obj); //make sure we can read this object
			} catch(Exception e) {
				System.out.println("[100k MOD]Could not parse Recording: " + e);
				break;
			}
		}
		System.out.println("Register/Invoke 100k finished");
		time100km = System.nanoTime() - time100km;

		System.out.println(String.format("%d,%d\n%d,%d\n%d,%d\n%d,%d\n", time240r, time240m, time480r, time480m, time10kr, time10km, time100kr, time100km));

		cli.shutdown();
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
		int r240Size = r240.toByteArray().length;
		int r480Size = r480.toByteArray().length;
		int r10kSize = r10k.toByteArray().length;
		int r100kSize = r100k.toByteArray().length;

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
		System.out.println("Finish cache init");
		cli.shutdown();
		return new int[] {(int)count240, (int)count480, (int)count10k, (int)count100k};
	}
}
