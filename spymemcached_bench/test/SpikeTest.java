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
import java.util.concurrent.Callable;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.ExecutorService;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Arrays;
import java.util.Random;
import java.io.PrintWriter;
import java.io.File;
import javax.swing.Timer;

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

class SpikeTest {
	private static final int NUM_THREADS = 1;
	private static final long CACHE_SIZE = 16L*1024L*1024L; //2L*1024L*1024L*1024L;//2GB
	private static final double PERCENT_FULL = 0.8;
	private static final int SMALL_SIZE_THRESH = 5*1024;
	private static final int MEDIUM_SIZE_THRESH = 20*1024;
	private static final double SMALL_BLEND = 0.0; //240 byte objs/480 byte objs
	private static final double MEDIUM_BLEND = 0.0; //Small objs/ large objs
	private static final double LARGE_BLEND = 1.0; //10k byte objs/100k byte objs
	private static final double T_STEP = 10.0;
	private static final String loadFile = new String("server_a_load.dat");
	private static final double[] modPercent = new double[] {0.0,0.1, 0.3, 0.5, 0.7, 0.9, 1.0};
	private static final int TIMESTEP = 15;

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

		if(args.length < 2) {
			System.out.println("Usage: java Test <host> <port#>");
			System.exit(-1);
		}
	
		cacheURI = args[0] + ":" + args[1];
/*		int[] objNums = initCache(NUM_THREADS);

		if(objNums == null) {
		System.out.println("Could not initialize cache");
		System.exit(-1);
		}

		System.out.println(String.format("Cache initialization success:\n\tSmall240: %d\n\tSmall480: %d\n\tLarge10k: %d\n\tLarge100k: %d", objNums[0], objNums[1], objNums[2], objNums[3]));
*/
		System.out.println("M TIMESTEP NUM_MODS NUM_GETS");
		for(int p = 0; p < modPercent.length; p++) {
			//for each time interval, set up the number of request types

			int[] objNums = initCache(1);
			
			if(objNums == null) {
				System.out.println("Could not initialize cache");
				System.exit(-1);
			}

			for(int iter = 0; iter < 60; iter++) {		
				int num_mods = randInt(1000, 1500);
				double mod = modPercent[p];
				double static_mod = 0.5;
				if(iter%6 < 3) {
					num_mods = -10;
					mod = 0.5;
					static_mod = modPercent[p];
				}
				ExecutorService executor = Executors.newSingleThreadExecutor();
				Future<TaskOutput> future = executor.submit(new Task(num_mods, mod, static_mod, 0.8, objNums[2], cacheURI));
				TaskOutput res = new TaskOutput();
				try {
					res = future.get(TIMESTEP, TimeUnit.SECONDS);
				} catch (Exception e) {
					System.out.println("Timeout error! " + e);
					System.exit(-1);
				}
		
				executor.shutdownNow();
				System.out.println(String.format("%d %d %d %d", p, iter, res.num_mods, res.num_gets));
			}
		}
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
		String k10k = ":RecordingListProto10k:1";
		String k100k = ":RecordingListProto100k";
		int r240Size = r240.toByteArray().length;
		int r480Size = r480.toByteArray().length;
		int r10kSize = r10k.toByteArray().length;
		int r100kSize = r100k.toByteArray().length;

		int count240 = 0;
		int count480 = 0;
		int count10k = 0;
		int count100k = 0;

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
		cli.shutdown();
		return new int[] {(int)count240, (int)count480, (int)count10k, (int)count100k};
	}

	private static int randInt(int min, int max) {
		Random random = new Random();
		return random.nextInt((max-min) + 1) + min;
	}
}

class TaskOutput {
	int num_mods;
	int num_gets;
}

class Task implements Callable<TaskOutput> {

	TaskOutput out;
	private int num_mods;
	private int num_objs;
	private double mod_ratio;
	private double static_ratio;
	private double get_ratio;
	private String cacheURI;
	private RecordingList r10k;

	public Task(int num_mods, double ratio, double static_ratio, double get_ratio, int num_objs, String cacheURI) {
		out = new TaskOutput();
		this.num_mods = num_mods;
		this.mod_ratio = ratio;
		this.static_ratio = static_ratio;
		this.get_ratio = get_ratio;
		this.num_objs = num_objs;
		this.cacheURI = cacheURI;
		r10k = proto10K().build();
	}

	@Override
	public TaskOutput call() throws Exception {
		int num_invoke = (int)(num_mods * mod_ratio); 
		int num_gets = num_mods - num_invoke;
		int total_mods = 0;
		
		MyMemcachedClient cli = null;
		try {
			cli = new MyMemcachedClient(
				new MyBinaryConnectionFactory(), AddrUtil.getAddresses(cacheURI));
		}
		catch(Exception e) {
			System.out.println("Exception on connecting: " + e);
			return null;
		}
		boolean no_set = false;
		boolean no_reply = false;
	
		//stop compression
		SerializingTranscoder ncTranscoder = new SerializingTranscoder();
		ncTranscoder.setCompressionThreshold(1000*1024*1024);
		registerFcn(cli, "setDelete", "./protolib.so:recordingListSetDelete");

		String k10k = ":RecordingListProto10k:1";
		int total_gets = 0;
		long start = System.nanoTime();
		while(!Thread.interrupted() && num_mods != 0 && System.nanoTime()-start < 10000000000L) {
			double rand = Math.random();	
			if(get_ratio > rand) {
				total_gets++;
				int rInd = randInt(0, num_objs-1);
				String k = String.format("%02d:%06d"+k10k, 0, rInd);

				Object obj = cli.get(k, ncTranscoder);
				
				RecordingList.Builder r10kb = RecordingList.newBuilder();

				try {
					r10kb.mergeFrom((byte[])obj);
				} catch(Exception e) {
					System.err.println("[10k REG]Could not parse Recording: " + e);
					continue;
				}
			}

			if(rand < 1-static_ratio && num_gets != 0 && mod_ratio != 1) {
				num_gets--;
				num_mods--;
				total_mods++;
	
				int rInd = randInt(0, num_objs-1);
				String k = String.format("%02d:%06d"+k10k, 0, rInd);

				Object obj = cli.get(k, ncTranscoder);
				
				RecordingList.Builder r10kb = RecordingList.newBuilder();

				try {
					r10kb.mergeFrom((byte[])obj);
				} catch(Exception e) {
					System.err.println("[10k REG]Could not parse Recording: " + e);
					continue;
				}
			
				Recording.Builder r480b = r10kb.getRecordingsBuilder(2);

				r480b.setStatus(RecordingStatus.Deleted);


				Future<Boolean> f = cli.set(k, 0, r10kb.build().toByteArray(), ncTranscoder);
				try {
					f.get();
				} catch (Exception e) {
					System.err.println("couldn't get set state");
					continue;
				}
			}
			else if(num_invoke != 0 && mod_ratio != 0){
				num_invoke--;
				num_mods--;
				total_mods++;

				int rInd = randInt(0, num_objs-1);
				String k = String.format("%02d:%06d"+k10k, 0, rInd);

				Object obj = cli.invoke(k, "setDelete", "22", no_set, no_reply); //set object and reply

				RecordingList r10k = null; 
				try {
					r10k = RecordingList.parseFrom((byte[])obj); //make sure we can read this object
				} catch(Exception e) {
					System.err.println("[10k MOD]Could not parse Recording: " + e);
					continue;
				}

			}
		}
		cli.shutdown();

		out.num_mods = total_mods;
		out.num_gets = total_gets;
		return out;
	}

	private Recording.Builder proto240() {
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


	private Recording.Builder proto480() {
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
	private RecordingList.Builder protoX(int num_bytes) {
		return recordingProto(num_bytes);
	}

	private RecordingList.Builder proto10K() {
		return recordingProto(10*1024);
	}

	private RecordingList.Builder proto100K() {
		return recordingProto(100*1024);
	}

	private RecordingList.Builder recordingProto(int size) {
		//RecordingList is just repeated recording objects
		RecordingList.Builder r = RecordingList.newBuilder();
		while(r.build().toByteArray().length < size) {
			r.addRecordings(proto240());
		}
		return r;
	}
	
	private int randInt(int min, int max) {
		Random random = new Random();
		return random.nextInt((max-min) + 1) + min;
	}
	
	private static void registerFcn(MyMemcachedClient cli, String fcnKey, String fcnPath) {
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
	
}
