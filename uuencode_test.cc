#include <iostream>
#include <string>
#include <cstdlib>
#include "uuencode.h"

using namespace std;

int main(int argc, char* argv[]) {

	char* res;
	size_t res_size;

	res_size = uuencode(&res, "Hello world! \0My name \0is Adam Schaub.", 38);

	cout << "Size: " << res_size << endl;
	cout << res << endl;

	char* deres;
	res_size = uudecode(&deres, res, res_size);	

	cout << "Size: " << res_size << endl;

	char* ptr = deres;
	for(int i = 0; i < res_size; ++i)
		cout << *ptr++;
	cout << endl;
		
	cout.flush();
	free(res);
	free(deres);
	return 0;
}
