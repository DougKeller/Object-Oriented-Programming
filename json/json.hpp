// Douglas Keller

#ifndef JSON_HPP
#define JSON_HPP

#include <map>
#include <vector>
#include <iostream>

// All of the datastructures and json functions are
// in this namespace to avoid overlapping of generic
// names such as Value, String or Object.
namespace json {

	// Value is an abstract type that is derived
	// by all the json value types.
	struct Value;
	struct String;
	struct Object;
	struct Array;
	struct True;
	struct False;
	struct Null;
	struct Number;

	// A visitor abstract class to perform operations
	// on different value types.
	struct Visitor {
		virtual ~Visitor() { }

		virtual void visit(String*) = 0;
		virtual void visit(Object*) = 0;
		virtual void visit(Array*)  = 0;
		virtual void visit(True*)   = 0;
		virtual void visit(False*)  = 0;
		virtual void visit(Null*)   = 0;
		virtual void visit(Number*) = 0;
	};	

	struct Value {
		virtual ~Value() { }
		virtual void accept(Visitor&) = 0;
	};
	struct String : Value {
		std::string value;
		void accept(Visitor& v) { v.visit(this); }
	};
	struct Object : Value {
		/* To retain the order of values within an object,
			I made the vector insertOrder to keep track of
			the order in which strings were read to the map.
			This also makes it easier to iterate through the
			map's values. 
		*/
		std::map<std::string, Value*> values;
		std::vector<std::string> insertOrder;
		void accept(Visitor& v) { v.visit(this); }
	};
	struct Array : Value {
		std::vector<Value*> values;
		void accept(Visitor& v) { v.visit(this); }
	};
	struct True : Value {
		void accept(Visitor& v) { v.visit(this); }
	};
	struct False : Value {
		void accept(Visitor& v) { v.visit(this); }
	};
	struct Null : Value {
		void accept(Visitor& v) { v.visit(this); }
	};
	struct Number : Value {
		std::string value;
		void accept(Visitor& v) { v.visit(this); }
	};

	/*	
	The Document class contains a pointer to the head
	of the document, as well as public methods for
	operations dealing with output, copying, etc..
	The head variable is private to prevent direct
	manipulation of document values/members.

	To keep Value pointers private, Document contains
	its own parse method to make sure pointer values
	cannot be manipulated outside of Document's scope.
	*/

	class Document {
	private:
		Value* head;

		// Private constructor
		Document(Value* v) : head(v) { } 

		Value* parse(std::istream&);
		void clrWS(std::istream&);
		
	public:
		// Constructors
		Document() : head(nullptr) { }
		Document(std::istream& is) : head(parse(is)) { }
		Document(Document const& doc) {
			if(doc.head) {
				Duplicator d;
				doc.head->accept(d);
				delete head;
				head = d.copy;
			} else {
				head = nullptr;
			}
		}

		// Deconstructor
		~Document() {
			delete head;
		}

		// Public member functions
		void print(std::ostream&) const;
		Document filter(std::vector<std::string>&) const;
		Document copy() const;
		std::string output() const;

		// Overloaded operator=
		Document& operator= (Document const& doc) {
			if(doc.head) {
				Duplicator d;
				doc.head->accept(d);

				delete head;	// Free any space before reassigning head pointer.
				head = d.copy;
			} else {
				delete head;
				head = nullptr;
			}
			return *this;
		}

		// Declarations for visitor structures
	private:
		// The Printer visitor is used for outputting any value
		// to a given ostream in proper 'pretty print' format.
		struct Printer : Visitor {
			std::ostream& os;
			int tab;
			Printer(std::ostream& o) : os(o), tab(0) { }

			void visit(String*);
			void visit(Object*);
			void visit(Array*);
			void visit(True*);
			void visit(False*);
			void visit(Null*);
			void visit(Number*);

			void printTabs();
		};

		// The Filter visitor is used for finding objects that
		// contain a given list of key values
		struct Filter : Visitor {
			Value* result;
			std::vector<std::string>& args;
			Filter(std::vector<std::string>& a) : result(nullptr), args(a) { }

			void visit(String*);
			void visit(Object*);
			void visit(Array*);
			void visit(True*);
			void visit(False*);
			void visit(Null*);
			void visit(Number*);
		};

		// The Exporter visitor is used for creating a string 
		// representation for a json document in proper json format.
		struct Exporter : Visitor {
			std::string output;

			void visit(String*);
			void visit(Object*);
			void visit(Array*);
			void visit(True*);
			void visit(False*);
			void visit(Null*);
			void visit(Number*);
		};

		// The Duplicator visitor is used for creating a copy
		// of a Value. This is used in the big 3.
			// This visitor was a challenge to implement, as I had to
			// try to make sure no memory would be leaked and that
			// documents would not share pointers to the same values.
		struct Duplicator : Visitor {
			Value* copy;

			Duplicator() : copy(nullptr) { }

			void visit(String*);
			void visit(Object*);
			void visit(Array*);
			void visit(True*);
			void visit(False*);
			void visit(Null*);
			void visit(Number*);
		};

	};

	// Function designed for more flexibility in parsing json objects.
	Document parse(std::istream&);
};

// Global operator overload for printing Documents.
std::ostream& operator<< (std::ostream&, json::Document&);

#endif