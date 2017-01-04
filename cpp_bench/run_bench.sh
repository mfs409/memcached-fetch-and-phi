# now specify the number of experiments
#OBJ_SIZES="256 4096 66536"
OBJ_SIZES="256"
#OBJ_SIZES="4096"
#OBJ_SIZES="66536"
SERVERS="localhost 6pac1"
TRIALS=5 
THREADS=12
PORT=11211
RESULTS_DIR="results/"
EXEC="./driver"
THREAD_FLAG="-t"
OBJ_SIZE_FLAG="-k"
SERVER_FLAG="-s"
PORT_FLAG="-p"

#make sure we have the LD library for our dist of libmemcached
export LD_LIBRARY_PATH=/home/ams314/cloud_migration/libmemcached-1.0.18/lib

for server in $SERVERS
  do
  for obj_size in $OBJ_SIZES
    do
    for thread in `seq $THREADS`
      do
      for trial in `seq $TRIALS`
        do
        echo genearting $server:$obj_size.$thread.$trial
	$EXEC $OBJ_SIZE_FLAG $obj_size $SERVER_FLAG $server $PORT_FLAG $PORT $THREAD_FLAG $thread > tmp
	mv tmp $RESULTS_DIR$server.$obj_size.$thread.$trial
	echo done
	done
      done
    done
  done
  
