PerfTest:
	Collect performance statistics from the server. Send a number of get/set/
	register/invoke operations to the server for each object size.

SpikeTest:
	Test spiking data requests. Every 20 seconds, there is a 10 second spike of requests.
	During spikes, as many requests are sent as possible. Between spikes, a semi-constant
	number of get/set operations are performed.

FetchTest:
	Test the affect of network latency with large objects. Includes time to get/set a large (100kB)
	object, the time to register and invoke a complex function on a large object, and the time
	to register and invoke a simple function on a large object. 
