// Douglas Keller

#include "rational.hpp"
#include <iostream>

#include <time.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

using namespace std;

int main () {
	vector<Rational> rationals;
	srand(time(nullptr));

	cout << "============================================================\n";
	cout << "=              = Douglas Keller - Rationals =              =\n";
	cout << "============================================================\n\n";

	// Create four random Rationals
	for(int i = 1; i <= 4; i++) {
		long num = rand() % 21 - 10; // Random even numerator between -10 and 10
		long den = rand() % 41 - 20; // Random even non-zero denominator between -20 and 20

		int fac = rand() % 5 + 1; // Multiply num and den by a randon number to ensure at least one common factor
		num *= fac;
		den *= fac;

		if(!den) den = 1;
		Rational r(num, den);

		cout << "Rational" << i << " = " << num << "/" << den << " = " << r << "\n";
		rationals.push_back(r);
	}

	cout << "\n========================= Sorting ==========================\n\n";

	//Display unsorted and sorted vectors
	cout << "Unsorted: ";
	for(vector<Rational>::iterator it = rationals.begin(); it != rationals.end(); it++) {
		cout << *it << "    ";
	}
	sort(rationals.begin(), rationals.end());
	cout << "\nSorted:   ";
	for(vector<Rational>::iterator it = rationals.begin(); it != rationals.end(); it++) {
		cout << *it << "    ";
	}
	cout << "\ntoDouble(): ";
	for(vector<Rational>::iterator it = rationals.begin(); it != rationals.end(); it++) {
		cout << it->toDouble() << "    ";
	}

	// This Rational is used to avoid dividing by a Rational with value 0/1
	Rational r(2,3);

	cout << "\n\n=================== Arithmetic Operators ===================\n";

	// Display random calculations using the generated values
	cout << "\nAddition\n";
	cout << "\t" << rationals[0] << " + " << rationals[1] << " = " << (rationals[0] + rationals[1]) << "\n";
	cout << "\t" << rationals[2] << " + 3 = " << (rationals[2] + 3) << "\n";

	cout << "\nSubtraction\n";
	cout << "\t" << rationals[2] << " - " << rationals[3] << " = " << (rationals[2] - rationals[3]) << "\n";
	cout << "\t5 - " << rationals[0] << " = " << (5 - rationals[0]) << "\n";

	cout << "\nMultiplication\n";
	cout << "\t" << rationals[0] << " * " << rationals[3] << " = " << (rationals[0] * rationals[3]) << "\n";
	cout << "\t" << rationals[2] << " * 2 = " << (rationals[2] * 2) << "\n";

	cout << "\nDivision\n";
	cout << "\t" << rationals[0] << " ÷ " << r << " = " << (rationals[0] / r) << "\n";
	cout << "\t" << rationals[2] << " ÷ 3 = " << (rationals[2] / 3) << "\n";

	cout << "\nDouble Arithmetic\n";
	cout << "\t" << rationals[1] << " + 3.14159 = " << (rationals[1] + 3.14159) << "\n";
	cout << "\t" << rationals[1] << " * 867.5309 = " << (rationals[1] * 867.5309) << "\n";

	cout << "\n======================= Comparisons ========================\n\n";

	// Display random comparisons and their results
	cout << "\t" << rationals[1] << " == " << rationals[0] << " ? " << ((rationals[1] == rationals[0]) ? "true" : "false") << "\n";
	cout << "\t" << rationals[2] << " <= 7.5 ? " << ((rationals[2] <= 7.5) ? "true" : "false") << "\n"; 
	cout << "\t" << "10 > " << rationals[1] << " ? " << ((10 > rationals[1]) ? "true" : "false") << "\n";

	cout << "\n============================================================\n";
}
