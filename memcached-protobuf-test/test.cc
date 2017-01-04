//play with memcached and protobuf together

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <libmemcached/memcached.h>
#include "test.pb.h"
#include "uuencode.h"

using namespace std;

const char* conf_str = "--SERVER=localhost:11211";

void addEntry(test::Person* person);
void displayBook(test::AddressBook& addr);
void displayEntry(test::Person& p);
void storeEntry(test::AddressBook& addr, memcached_st *memc);
void getEntry(test::AddressBook& addr, memcached_st *memc);
char* entryKey(const test::Person& p);


int main(int argc, char* argv[]) {
	
	//Check compatibility between the version of protobuf used to
	//compile the messages and the version linked to this program
	GOOGLE_PROTOBUF_VERIFY_VERSION;	
	
	//connect to memcached
	memcached_st *memc = memcached(conf_str, strlen(conf_str));

	if(!memc) {
		cerr << "Failed to connect to memcached using option " << conf_str << endl;
		return -1;
	}


	//input arguments
	char ch;
	char* messageLoc = NULL;
	while((ch = getopt(argc, argv, "m:")) != -1) {
		switch(ch) {
			case 'm': cout << "Using message file: " << optarg << endl;
				  messageLoc = new char[strlen(optarg) + 1];
				  strcpy(messageLoc, optarg);
				  break;
			case '?': cout << "unknown option" << endl;
		}
	}
	if(!messageLoc) {
		messageLoc = "addressbook_default";
	}
	cout << messageLoc << endl;

	test::AddressBook addr;

	fstream input(messageLoc, ios::in | ios::binary);

	if(!input) {
		cout << "File " << messageLoc << " not found. New file created." << endl;
		addr.set_id(1);
		
		test::Person* addrOwner = addr.mutable_owner();
		string name;
		cout << "What is your name?" << endl;
		getline(cin, name);

		addrOwner->set_id(1);
		addrOwner->set_name(name);
	}
	else if(!addr.ParseFromIstream(&input)) {
		cerr << "Unable to parse file " << messageLoc << endl;
		return -1;
	}
	input.close();

	int choice = 1;
	while(choice != 6) {
		cout << "Choose an action: " << endl
		     << "\t1: Display Address Book" << endl
		     << "\t2: Show String Serialization" << endl
		     << "\t3: Add Entry" << endl
		     << "\t4: Store Entry" << endl
		     << "\t5: Get Entry" << endl
		     << "\t6: Quit" << endl;
		
		cin >> choice;
	
		if(choice == 1) {
			displayBook(addr);
		}
		else if(choice ==2) {
			string out;
			addr.SerializeToString(&out);
			cout << out << endl << endl;
		}
		else if(choice == 3) {
			addEntry(addr.add_entry());
		}
		else if(choice == 4) {
			storeEntry(addr, memc);
		}
		else if(choice == 5) {
			getEntry(addr, memc);
		}
	}

	//output everything to the appropriate file
	fstream output(messageLoc, ios::out | ios::trunc | ios::binary);
	if(!addr.SerializeToOstream(&output)) {
		cerr << "Failed to write file " << messageLoc << endl;
		return -1;
	}
	output.close();

	//close everything down
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}

void addEntry(test::Person* person) {
	int id;
	cout << "Enter entry ID: ";
	cin >> id;
	person->set_id(id);
	cin.ignore(256, '\n');

	string name;
	cout << "Enter name: ";
	getline(cin, name);
	
	if(!name.empty()) {
		person->set_name(name);
	}
	else {
		person->set_name("Joe Placeholder");
	}		

	string email;
	cout << "Enter email: ";
	getline(cin, email);

	if(!email.empty()) {
		person->set_email(email);
	}
	else {
		person->set_email("joe.placeholder@placeholder.com");
	}

	while(true) {
		string number;
		cout << "Enter phonenumber (or leave blank to stop): ";
		getline(cin, number);

		if(number.empty()) {
			cout << "Stopping." << endl;
			break;
		}

		test::Person::PhoneNumber* nPhone = person->add_phone();
		nPhone->set_number(number);

		string pType;
		cout << "Enter phone type [mobile, work, home, private]: ";
		getline(cin, pType);
		
		if(pType == "mobile") {
			nPhone->set_type(test::Person::MOBILE);
		}
		else if(pType == "work") {
			nPhone->set_type(test::Person::WORK);
		}
		else if(pType == "home") {
			nPhone->set_type(test::Person::HOME);
		}
		else if(pType == "private") {
			nPhone->set_type(test::Person::PRIVATE);
		}
		else {
			cout << "Unrecognized type " << pType << ". Setting default." << endl;
		}
	}
}

void displayBook(test::AddressBook& addr) {
	cout << "========================================" << endl
	     << "Address Book for " << addr.owner().name() << endl
	     << "========================================" << endl;

	for(int i = 0, len = addr.entry_size(); i < len; ++i) {
		const test::Person& p = addr.entry(i);
		cout << "Entry " << i+1 << ": " << p.name() << endl;
		cout << "\tid: " << p.id() << endl;
		cout << "\temail: " << p.email() << endl;
		cout << "\tphone numbers: " << endl;

		for(int j = 0, len = p.phone_size(); j < len; ++j) {
			const test::Person::PhoneNumber pn = p.phone(j);
			cout << "\tPhone #" << j+1 << ": " << endl;
			cout << "\t\tNumber: " << pn.number() << endl;
			cout << "\t\tPhoneType: " << pn.type() << endl;	
		}
	}
	cout << endl;	
}

void displayEntry(test::Person& p) {
	cout << "Entry for: " << p.name() << endl;
	cout << "\tid: " << p.id() << endl;
	cout << "\temail: " << p.email() << endl;
	cout << "\tphone numbers: " << endl;

	for(int j = 0, len = p.phone_size(); j < len; ++j) {
		const test::Person::PhoneNumber pn = p.phone(j);
		cout << "\tPhone #" << j+1 << ": " << endl;
		cout << "\t\tNumber: " << pn.number() << endl;
		cout << "\t\tPhoneType: " << pn.type() << endl;	
	}
	cout << endl;	
}

void storeEntry(test::AddressBook& addr, memcached_st *memc) {
	displayBook(addr);

	cout << "Choose the entry number you wish to store: " << endl;
	int num;
	cin >> num;

	if(num > addr.entry_size() || num < 1) {
		cout << "Invalid entry selection." << endl;
		return;
	}
	
	string val;
	addr.entry(num-1).SerializeToString(&val);
	char* key = entryKey(addr.entry(num-1));
	time_t exp = 0; //expiration of this record
	uint32_t flags = 0;

	//store value as uuencoded string first
	char* val_enc;
	int val_length = uuencode(&val_enc, val.c_str(), val.length());

	memcached_return_t rc;
	rc = memcached_set(memc, key, strlen(key), val_enc, val_length, exp, flags); 

	cout << "Inserting with key " << key << endl;

	if(rc != MEMCACHED_SUCCESS) {
		cout << "Store value failed." << endl;
		cout << "Error: " << memcached_strerror(memc, rc) << endl;	
	}
		
	free(key);
}

void getEntry(test::AddressBook& addr, memcached_st *memc) {
	displayBook(addr);

	cout << "Choose the entry number you wish to find: " << endl;
	int num;
	cin >> num;

	if(num > addr.entry_size() || num < 1) {
		cout << "Invalid entry selection." << endl;
		return;
	}

	char* key = entryKey(addr.entry(num-1));
	size_t val_size;
	uint32_t flags;
	memcached_return_t rc;
	char* res = memcached_get(memc, key, strlen(key), &val_size, &flags, &rc);	
	
	if(rc != MEMCACHED_SUCCESS) {
		cout << "Did not find entry for " << key << endl;
		cout << "Error: " << memcached_strerror(memc, rc) << endl;
	}
	else {
		test::Person p;
		char* res_dec;
		int res_len = uudecode(&res_dec, res, val_size);	
		string result(res_dec, res_len);
		p.ParseFromString(result);
		displayEntry(p);
	}

	free(key);
	free(res);
}

//note to self, no white space in keys
char* entryKey(const test::Person& p) {
	string temp = p.email();
	char* key = new char[temp.length() + 1];
	strcpy(key, temp.c_str());
	return key;
}
