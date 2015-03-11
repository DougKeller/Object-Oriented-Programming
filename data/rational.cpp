// Douglas Keller

#include "rational.hpp"
#include <cassert>
#include <cmath>

#include <ostream>

/////////////////////////////////
//        Constructors         //
/////////////////////////////////

Rational::Rational () : _numerator(0), _denominator(1) {   }

Rational::Rational (long num) : _numerator(num), _denominator(1) {   }

Rational::Rational (long num, long den) : _numerator(num), _denominator(den) {   
	assert(den); // Aborts if den == 0
	_simplify();
}

Rational::Rational (Rational const& r) : _numerator(r.numerator()), _denominator(r.denominator()) {   }

/////////////////////////////////
//  Private Member Functions   //
/////////////////////////////////

// Precondition:  _numerator and _denominator are defined, _denominator != 0
// Postcondition: _numerator and _denominator do not share a common factor
void Rational::_simplify () {
	// Simplify any 0 value to 0/1 and return
	if(_numerator == 0) {
		_denominator = 1;
		return;
	}

	// If _denominator is negative, multiply top and bottom by -1
	if(_denominator < 0) {
		_denominator *= -1;
		_numerator *= -1;
	}

	// Find and divide by the Greatest Common Factor
	long factor = _gcf();
	_numerator   /= factor;
	_denominator /= factor;
}

// Precondition:  _numerator and _denominator are defined, _denominator != 0 and _numerator != 0
// Postcondition: Returns the greatest common factor of _numerator and _denominator
long Rational::_gcf () const {
	long temp;
	long a = _numerator;
	long b = _denominator;

	while (b) { // Loops until b == 0
		temp = a;
		a = b;
		b = temp % a;
	}

	return std::abs(a); // Returns positive, non-zero value;
}

/////////////////////////////////
//         Accessors           //
/////////////////////////////////

long Rational::numerator   () const { return _numerator;   }
long Rational::denominator () const { return _denominator; }
double Rational::toDouble  () const { return static_cast<double>(_numerator) / _denominator; }

/////////////////////////////////
//      Member operators       //
/////////////////////////////////

// Equals
Rational& Rational::operator= (Rational const& val) {
	_numerator = val.numerator();
	_denominator = val.denominator();
	_simplify();
	return *this;
}
Rational& Rational::operator= (long val) { return *this = Rational(val); }
Rational& Rational::operator= (int val)  { return *this = Rational(val); }

// Plus equals
Rational& Rational::operator+= (Rational const& val) {
	_numerator = _numerator * val.denominator() + _denominator * val.numerator();
	_denominator = _denominator * val.denominator();
	_simplify();
	return *this;
}
Rational& Rational::operator+= (long val) { return *this += Rational(val); }
Rational& Rational::operator+= (int val)  { return *this += Rational(val); }

// Minus equals
Rational& Rational::operator-= (Rational const& val) {
	_numerator = _numerator * val.denominator() - _denominator * val.numerator();
	_denominator = _denominator * val.denominator();
	_simplify();
	return *this;
}
Rational& Rational::operator-= (long val) { return *this -= Rational(val); }
Rational& Rational::operator-= (int val)  { return *this -= Rational(val); }

// Times equals
Rational& Rational::operator*= (Rational const& val) {
	_numerator *= val.numerator();
	_denominator *= val.denominator();
	_simplify();
	return *this;
}
Rational& Rational::operator*= (long val) { return *this *= Rational(val); }
Rational& Rational::operator*= (int val)  { return *this *= Rational(val); }

// Divided-by equals
Rational& Rational::operator/= (Rational const& val) {
	_numerator *= val.denominator();
	_denominator *= val.numerator();
	_simplify();
	return *this;
}
Rational& Rational::operator/= (long val) { return *this /= Rational(val); }
Rational& Rational::operator/= (int val)  { return *this /= Rational(val); }

/////////////////////////////////
//      Global Operators       //
/////////////////////////////////

// Output to stream
std::ostream& operator<<(std::ostream& os, Rational const& r) {
	os << r.numerator() << "/" << r.denominator();
	return os;
}

/////////////////////////////////
//        Arithmetic           //
/////////////////////////////////

// Addition
Rational operator+ (Rational const& a, Rational const& b) {
	long num = a.numerator() * b.denominator() + b.numerator() * a.denominator();
	long den = a.denominator() * b.denominator();
	return Rational(num, den);
}
Rational operator+ (long a, Rational const& b)   { return Rational(a) + b;  }
Rational operator+ (Rational const& a, long b)   { return a + Rational(b);  }
Rational operator+ (int a, Rational const& b)    { return Rational(a) + b;  }
Rational operator+ (Rational const& a, int b)    { return a + Rational(b);  }
double   operator+ (double a, Rational const& b) { return a + b.toDouble(); }
double   operator+ (Rational const& a, double b) { return a.toDouble() + b; }

// Subtraction
Rational operator- (Rational const& a, Rational const& b) {
	long num = a.numerator() * b.denominator() - b.numerator() * a.denominator();
	long den = a.denominator() * b.denominator();
	return Rational(num, den);
}
Rational operator- (long a, Rational const& b)   { return Rational(a) - b;  }
Rational operator- (Rational const& a, long b)   { return a - Rational(b);  }
Rational operator- (int a, Rational const& b)    { return Rational(a) - b;  }
Rational operator- (Rational const& a, int b)    { return a - Rational(b);  }
double   operator- (double a, Rational const& b) { return a - b.toDouble(); }
double   operator- (Rational const& a, double b) { return a.toDouble() - b; }

// Multiplication
Rational operator* (Rational const& a, Rational const& b) {
	long num = a.numerator() * b.numerator();
	long den = a.denominator() * b.denominator();
	return Rational(num, den);
}
Rational operator* (long a, Rational const& b)   { return Rational(a) * b;  }
Rational operator* (Rational const& a, long b)   { return a * Rational(b);  }
Rational operator* (int a, Rational const& b)    { return Rational(a) * b;  }
Rational operator* (Rational const& a, int b)    { return a * Rational(b);  }
double   operator* (double a, Rational const& b) { return a * b.toDouble(); }
double   operator* (Rational const& a, double b) { return a.toDouble() * b; }

// Division
Rational operator/ (Rational const& a, Rational const& b) {
	long num = a.numerator() * b.denominator();
	long den = a.denominator() * b.numerator();
	return Rational(num, den);
}
Rational operator/ (long a, Rational const& b)   { return Rational(a) / b;  } 
Rational operator/ (Rational const& a, long b)   { return a / Rational(b);  }
Rational operator/ (int a, Rational const& b)    { return Rational(a) / b;  }
Rational operator/ (Rational const& a, int b)    { return a / Rational(b);  }
double   operator/ (double a, Rational const& b) { return a / b.toDouble(); }
double   operator/ (Rational const& a, double b) { return a.toDouble() / b; }

/////////////////////////////////
//         Comparisons         //
/////////////////////////////////

// Equal to
bool operator== (Rational const& a, Rational const& b) { return a.numerator() == b.numerator() && a.denominator() == b.denominator(); }
bool operator== (int a, Rational const& b)			   { return Rational(a) == b;  }
bool operator== (Rational const& a, int b)			   { return a == Rational(b);  }
bool operator== (long a, Rational const& b)   		   { return Rational(a) == b;  }
bool operator== (Rational const& a, long b)   		   { return a == Rational(b);  }
bool operator== (double a, Rational const& b) 		   { return a == b.toDouble(); }
bool operator== (Rational const& a, double b) 		   { return a.toDouble() == b; }

// Not equal to
bool operator!= (Rational const& a, Rational const& b) { return !(a == b); }
bool operator!= (int a, Rational const& b)			   { return !(a == b); }
bool operator!= (Rational const& a, int b)			   { return !(a == b); }
bool operator!= (long a, Rational const& b)   		   { return !(a == b); }
bool operator!= (Rational const& a, long b)   		   { return !(a == b); }
bool operator!= (double a, Rational const& b) 		   { return !(a == b); }
bool operator!= (Rational const& a, double b) 		   { return !(a == b); }

// Greater than
bool operator>  (Rational const& a, Rational const& b) { return a.toDouble() > b.toDouble(); }
bool operator>  (int a, Rational const& b)			   { return a > b.toDouble(); }
bool operator>  (Rational const& a, int b)			   { return a.toDouble() > b; }
bool operator>  (long a, Rational const& b)   		   { return a > b.toDouble(); }
bool operator>  (Rational const& a, long b)   		   { return a.toDouble() > b; }
bool operator>  (double a, Rational const& b) 		   { return a > b.toDouble(); }
bool operator>  (Rational const& a, double b) 		   { return a.toDouble() > b; }

// Less than
bool operator<  (Rational const& a, Rational const& b) { return a.toDouble() < b.toDouble(); }
bool operator<  (int a, Rational const& b)			   { return a < b.toDouble(); }
bool operator<  (Rational const& a, int b)			   { return a.toDouble() < b; }
bool operator<  (long a, Rational const& b)  		   { return a < b.toDouble(); }
bool operator<  (Rational const& a, long b)   		   { return a.toDouble() < b; }
bool operator<  (double a, Rational const& b) 		   { return a < b.toDouble(); }
bool operator<  (Rational const& a, double b) 		   { return a.toDouble() < b; }

// Greater than or equal to
bool operator>= (Rational const& a, Rational const& b) { return !(a < b); }
bool operator>= (int a, Rational const& b)			   { return !(a < b); }
bool operator>= (Rational const& a, int b)			   { return !(a < b); }
bool operator>= (long a, Rational const& b)   		   { return !(a < b); }
bool operator>= (Rational const& a, long b)   		   { return !(a < b); }
bool operator>= (double a, Rational const& b) 		   { return !(a < b); }
bool operator>= (Rational const& a, double b) 		   { return !(a < b); }

// Less than or equal to
bool operator<= (Rational const& a, Rational const& b) { return !(a > b); }
bool operator<= (int a, Rational const& b)			   { return !(a > b); }
bool operator<= (Rational const& a, int b)			   { return !(a > b); }
bool operator<= (long a, Rational const& b)   		   { return !(a > b); }
bool operator<= (Rational const& a, long b)   		   { return !(a > b); }
bool operator<= (double a, Rational const& b) 		   { return !(a > b); }
bool operator<= (Rational const& a, double b) 		   { return !(a > b); }