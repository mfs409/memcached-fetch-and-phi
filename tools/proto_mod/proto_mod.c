#include <stdio.h>
#include <sstream>
#include <time.h>
#include "proto_src/Recording.pb.h"
#include "proto_src/cache/RecordingList.pb.h"
#include "proto_src/DeletePriority.pb.h"
#include "proto_src/ListingData.pb.h"
#include "proto_src/RecordingSegment.pb.h"
#include "proto_src/RecordingType.pb.h"
#include "proto_src/RecordingOptions.pb.h"
#include "proto_src/RecordingStatus.pb.h"
#include "proto_src/NotRecordReason.pb.h"
#include "protostream.h"

#ifdef __cplusplus
extern "C" {
#endif
	int setRecordDelete(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		protostream pin(buf, buf+n);
		scheduler::Recording r;
		if(r.ParseFromIstream(&pin)) {
			scheduler::RecordingStatus s;
			RecordingStatus_Parse("Deleted", &s);	
			r.set_status(s);
			std::ostringstream out;
			int len = r.ByteSize();
			out.rdbuf()->pubsetbuf(buf, len);
			if(!r.SerializeToOstream(&out)) return -1;
			return len; 
		}
		return n;
	} 
	int setFailureReason(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		protostream pin(buf, buf+n);
		scheduler::Recording r;
		if(r.ParseFromIstream(&pin)) {
			std::string* tmp = r.mutable_failurereason();
			*tmp = std::string(param);
			std::ostringstream out;
			int len = r.ByteSize();
			out.rdbuf()->pubsetbuf(buf, len);
			if(!r.SerializeToOstream(&out)) return -1;
			return len; 
		}
		return n;
	}

	int recordingListSetDelete(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		protostream pin(buf, buf+n);
		scheduler::cache::RecordingList rl;
		if(rl.ParseFromIstream(&pin)) {
			int size = rl.recordings_size();
			if(size > 0) {
				scheduler::Recording *r = rl.mutable_recordings(0);
				scheduler::RecordingStatus s = r->status();
				if(RecordingStatus_Name(s) != "Deleted") {
					scheduler::RecordingStatus nS;
					RecordingStatus_Parse("Deleted", &nS);
					r->set_status(nS);
				}
				std::ostringstream out;
				int len = rl.ByteSize();
				out.rdbuf()->pubsetbuf(buf, len);
				if(!rl.SerializeToOstream(&out)) return -1;
				return len; 
			}
		}
		return n; 
	}

	int getFirstField(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		char* ptr = buf;
		while(*ptr != '') {
			ptr++;
		}
		ptr++;
		int len = 0;
		while(*ptr >= 'A' && *ptr <= 'Z') {
			*buf = *ptr;
			buf++;
			ptr++;
			len++;
		}
		return len;
	}

	int getRecordingFailureReason(char* buf, int n, int MAX_LEN, char* param, int p_len) {
	/*	int ind = atoi(param);
		std::string fr = "No failure reason set.";
		strcpy(buf, fr.c_str());
		return fr.length();
	*/
		int ind = atoi(param);
	
		protostream pin(buf, buf+n);
		scheduler::cache::RecordingList rl;
		if(rl.ParseFromIstream(&pin)) {
			scheduler::Recording *r = rl.mutable_recordings(ind);
		
			std::string fr = "No failure reason set.";
			if(r->has_failurereason()) {
				fr = r->failurereason();
			}
			
			strcpy(buf, fr.c_str());
			return fr.length();
		}
		return n; 
	}

	int getListPart(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		/*int ind = atoi(param);
		std::string fr = "No failure reason set.";
		std::string tmp = "";
		int i = 0;
		for(i = 0; i < 100; i++) {
			tmp += fr;
		}
		strcpy(buf, tmp.c_str());
		return (int)tmp.length();
*/
		int ind = atoi(param);
		protostream pin(buf, buf+n);
		scheduler::cache::RecordingList rl;
		if(rl.ParseFromIstream(&pin)) {
			scheduler::Recording *r = rl.mutable_recordings(ind);
			std::ostringstream out;
			int len = r->ByteSize();
			out.rdbuf()->pubsetbuf(buf, len);
			if(!r->SerializeToOstream(&out)) return -1;
			return len; 
		}
		return n; 
	}

	int lowCost(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		int i = n;
		double q = 0;
		while(i-- > 0) {
			q = q*0.989;
		}
		return n;
	}

	int nop(char* buf, int n, int MAX_LEN, char* param, int p_len) {
		return n;
	}
#ifdef __cplusplus
}
#endif
