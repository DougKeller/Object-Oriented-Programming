// Douglas Keller

#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <ostream>

// Invariants: _denominator > 0
//             _numerator/_denominator is always in simplest form
class Rational
{
private:
	long _numerator, _denominator;

	/* 	I made the member variables of type long, to guarantee a
		range of values from at least -2^32 to 2^32-1, rather 
		than an int's guaranteed range of -2^16 to 2^16-1
	*/

	void _simplify ();
	long _gcf () const;

	/*  _gcf() is used in the _simplify() method to improve 
		readability and clarity
	*/

public:
	Rational ();
	Rational (long);
	Rational (long, long);
	Rational (Rational const&);

	/*	In my implementation of this datastructure, I decided to
		omit the constructor Rational(double) due to the difficulty
		in accurately casting a floating-point value into a p/q
		representation. For example, since sqrt(2) is an irrational
		number, it has no rational representation, hence it is a
		narrowing conversion and loses precision
	*/

	// Accessors
	long numerator   () const;
	long denominator () const;
	double toDouble  () const;

	/*	I introduced the toDouble () method, which makes double 
		representation and comparisons to other double values a
		little better without the risk of losing precision from
		attempting to turn a double into a rational. This makes
		comparing doubles to Rationals as precise as comparing
		doubles to doubles
	*/

	// Overloaded member operators for =,+=,-=,*=, and /=
	Rational& operator=  (Rational const&);
	Rational& operator=  (long);
	Rational& operator=  (int);
	Rational& operator+= (Rational const&);
	Rational& operator+= (long);
	Rational& operator+= (int);
	Rational& operator-= (Rational const&);
	Rational& operator-= (long);
	Rational& operator-= (int);
	Rational& operator*= (Rational const&);
	Rational& operator*= (long);
	Rational& operator*= (int);
	Rational& operator/= (Rational const&);
	Rational& operator/= (long);
	Rational& operator/= (int);

	/* 	I didn't overload modifying operators between Rationals and doubles
		since it's not possible to accurately cast a double into a Rational
		and give the correct result (e.g. sqrt(2) is not a rational number)
	*/

};

// Overloaded global operators for <<, basic arithmetic and comparisons
std::ostream& operator<< (std::ostream&, Rational const&);

/* 	Since turning a double into a Rational would be a narrowing
	conversion, I decided to make the arithmetic operators between
	doubles and Rationals return a double value, since a rational
	number can be represented more accurately as a double than a
	double can be represented as a Rational
*/

Rational operator+ (Rational const&, Rational const&);
Rational operator+ (long, Rational const&);
Rational operator+ (Rational const&, long);
Rational operator+ (int, Rational const&);
Rational operator+ (Rational const&, int);
double   operator+ (double, Rational const&);
double   operator+ (Rational const&, double);

Rational operator- (Rational const&, Rational const&);
Rational operator- (long, Rational const&);
Rational operator- (Rational const&, long);
Rational operator- (int, Rational const&);
Rational operator- (Rational const&, int);
double   operator- (double, Rational const&);
double   operator- (Rational const&, double);

Rational operator* (Rational const&, Rational const&);
Rational operator* (long, Rational const&);
Rational operator* (Rational const&, long);
Rational operator* (int, Rational const&);
Rational operator* (Rational const&, int);
double   operator* (double, Rational const&);
double   operator* (Rational const&, double);

Rational operator/ (Rational const&, Rational const&);
Rational operator/ (long, Rational const&);
Rational operator/ (Rational const&, long);
Rational operator/ (int, Rational const&);
Rational operator/ (Rational const&, int);
double   operator/ (double, Rational const&);
double   operator/ (Rational const&, double);

// Comparison overloading. Comparisons to doubles utilize the toDouble() function
//     Note: comparisons to ints were implemented to avoid ambiguity between longs and doubles
bool operator== (Rational const&, Rational const&);
bool operator== (int, Rational const&);
bool operator== (Rational const&, int);
bool operator== (long, Rational const&);
bool operator== (Rational const&, long);
bool operator== (double, Rational const&);
bool operator== (Rational const&, double);

bool operator!= (Rational const&, Rational const&);
bool operator!= (int, Rational const&);
bool operator!= (Rational const&, int);
bool operator!= (long, Rational const&);
bool operator!= (Rational const&, long);
bool operator!= (double, Rational const&);
bool operator!= (Rational const&, double);

bool operator>  (Rational const&, Rational const&);
bool operator>  (int, Rational const&);
bool operator>  (Rational const&, int);
bool operator>  (long, Rational const&);
bool operator>  (Rational const&, long);
bool operator>  (double, Rational const&);
bool operator>  (Rational const&, double);

bool operator<  (Rational const&, Rational const&);
bool operator<  (int, Rational const&);
bool operator<  (Rational const&, int);
bool operator<  (long, Rational const&);
bool operator<  (Rational const&, long);
bool operator<  (double, Rational const&);
bool operator<  (Rational const&, double);

bool operator>= (Rational const&, Rational const&);
bool operator>= (int, Rational const&);
bool operator>= (Rational const&, int);
bool operator>= (long, Rational const&);
bool operator>= (Rational const&, long);
bool operator>= (double, Rational const&);
bool operator>= (Rational const&, double);

bool operator<= (Rational const&, Rational const&);
bool operator<= (int, Rational const&);
bool operator<= (Rational const&, int);
bool operator<= (long, Rational const&);
bool operator<= (Rational const&, long);
bool operator<= (double, Rational const&);
bool operator<= (Rational const&, double);

#endif