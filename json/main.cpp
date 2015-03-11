// Douglas Keller

#include "json.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace json;

int main(int argc, char** argv) {
	if(argc > 1) {
		cout << "Filtering Json Document for the following key values:\n\t";
		for(int i = 1; i < argc; ++i) 
			cout << argv[i] << "    ";
		cout << '\n';

		// Create a vector of all arguments excluding the command
		vector<string> args(argv + 1, argv + argc);

		Document j(cin);
		Document f = j.filter(args);

		cout << f << endl;
		
	} else {
		Document j(cin);
		cout << j << endl;
	}
}