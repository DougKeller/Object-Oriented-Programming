// Douglas Keller

#include "json.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

//*****************************
// Namespace json functions
//*****************************

//Precondition: The istream is defined and contains characters to read.
//Postcondition: Returns a document containing the information from istream.
json::Document json::parse(std::istream& is) {
	return Document(is);
}

//*****************************
// Document member functions
//*****************************

// Recursive method that returns Values, 
// based on the next character in the istream.
// Postcondition: Document's Value* pointer
//			contains a proper json document
//			with information from the istream.
json::Value* json::Document::parse(std::istream& is) {
	try {
		clrWS(is);
		// Choose Value type based off the next character in the istream.
		switch(is.peek()) {
			case '\"': {
				String* str = new String();

				is.ignore();	// Ignore the first double quote.
				while(is.peek() != '\"') {	// Append characters until a non-escape sequenced double quote is found.
					if(is.peek() == '\\')	// Append an additional character if \ is detected.
						str->value += is.get();
					str->value += is.get();
				}
				is.ignore();	// Ignore the last double quote.

				return str;
			}
			case '{': {
				Object* obj = new Object();

				is.ignore();	// Ignore the first { symbol.
				clrWS(is);

				while(is.peek() != '}') {	// Append values until an end brace is found.
					// Get the string value from the istream and add set it as the key.
					String* str = dynamic_cast<String*>(parse(is));
					std::string key = str->value;
					delete str;
				
					// These three lines remove any whitespace and the colon between key/value.
					clrWS(is);
					is.ignore();
					clrWS(is);
				
					Value* val = parse(is);
				
					// Check if a value has already been assigned to the map. If it has, overwrite it.
					auto position = std::find(obj->insertOrder.begin(), obj->insertOrder.end(), key);
					if(position != obj->insertOrder.end())
						obj->insertOrder.erase(position);
					obj->insertOrder.push_back(key);

					// Assign the value to the key.
					obj->values[key] = val;

					clrWS(is);
					// Check if there is a comma, indicating more pairs.
					if(is.peek() == ',') {
						is.ignore();	// Ignore the comma symbol.
						clrWS(is);
					}
				}

				is.ignore(); // Ignore the ] symbol.

				return obj;
			}
			case '[': {
				Array* arr = new Array();

				is.ignore();	// Ignore the [ symbol.
				clrWS(is);

				while(is.peek() != ']') {	// Loop until end-brace is found.
					Value* val = parse(is);	// Parse the next value in the istream.
					arr->values.push_back(val);	// Append said value to the array.

					clrWS(is);
					// Check if there is a comma, indicating more values.
					if(is.peek() == ','){
						is.ignore();	// Ignore the comma symbol.
						clrWS(is);
					}
				}

				is.ignore();	// Ignore the ] symbol.

				return arr;
			}
			case 't': {
				is.ignore(4);	// 'true' detected; ignore the next four characters in the stream.
				return new True();
			}
			case 'f': {
				is.ignore(5);	// 'false' detected; ignore the next five characters in the stream.
				return new False();
			}
			case 'n': {
				is.ignore(4);	// 'null' detected; ignore the next four characters in the stream.
				return new Null();
			}
			default: {
				// If the next character doesn't match any other value type, it must be a number.
				Number* num = new Number();

				// Append characters to the number's string until whitespace, an end brace, or a comma is found.
				while(!(is.peek() == ' ' || is.peek() == ',' || is.peek() == '}' || is.peek() == ']'))
					num->value += is.get();

				return num;
			}
		}
	} catch (...) {
		// Any exception that might be found returns Null and displays error message.
		std::cout << "Unable to parse input." << std::endl;
		return new Null();
	}
}

//Precondition:  The istream is defined and contains characters to read.
//Postcondition: The istream's next character is not a space.
	/*	Note: When it came to clearing whitespace from the stream, I had
			the option of simply using the >> operator to get the next 
			line without any preceding whitespace. The problem with this
			is that it could easily include way more than intended. This
			is why I created a method that loops until it finds the start
			of the next readable, non-whitespace character.
				e.g. If the characters in the istream were 
					'  {"Key":"Value","Key2",   "Value"}', using >> would 
					read all the way up to the end of "Key2", making it 
					harder to detect the correct values from the istream.
	*/
void json::Document::clrWS(std::istream& is) {
	while(is.peek() == ' ')
		is.ignore();
}

// Creates a Printer visitor to navigate the
// document's Value pointer.
// Postcondition: Json Document is output to 
// 		given ostream in 'pretty print' format.
void json::Document::print(std::ostream& os) const {
	if(!head) {	// Print out null if head is undefined.
		os << "null";
		return;
	}

	Printer p(os);
	head->accept(p);
}

// Creates a Filter visitor to navigate the
// document's Value pointer.
// Postcondition: Returns an Object ponter containing
//			all key/value pairs containing args
json::Document json::Document::filter(std::vector<std::string>& args) const {
	if(!head) {	// Return a blank document if head is undefined.
		Document d;
		return d;
	}

	Filter f(args);
	head->accept(f);

	if(f.result) {		// If the result is not nullptr
		Duplicator d;	// Copy all values contained by the pointer.
		f.result->accept(d);
		return Document(d.copy);	// Return it as a document.
	} else {			// Otherwise, return a blank document.
		Document d;
		return d;
	}
}

// Creates a Duplicator visitor to copy
// the document.
// Postcondition: Returns a copy of the
// document.
json::Document json::Document::copy() const {
	if(!head){	// Return a blank document if head is undefined.
		Document d;
		return d;
	}
	Duplicator d;
	head->accept(d);
	return Document(d.copy);
}

// Creates an Exporter visitor to navigate the
// document's Value pointer.
// Postcondition: Returns a string in legal json format
//			that represents this Json Document.
std::string json::Document::output() const {
	if(!head)	// Return Null value if head is not defined.
		return "";

	Exporter e;
	head->accept(e);
	return e.output;
}

//*****************************
// Printer member functions
//*****************************

void json::Document::Printer::visit(String* s) {
	os << '\"' << s->value << '\"';
}
void json::Document::Printer::visit(Object* o) {
	os << "{\n";
	tab++;
	for(auto it = o->insertOrder.begin(); it != o->insertOrder.end();) {
		printTabs();

		// Print the key value followed by a colon.
		os << '\"' << *it << "\": ";

		// Send this visitor to the Value to print it.
		o->values[*it]->accept(*this);

		it++;
		// Print a comma if the object contains more pairs.
		if(it != o->insertOrder.end())
			os << ",\n";
	}
	tab--;
	os << '\n';
	printTabs();
	os << '}';
}
void json::Document::Printer::visit(Array* a) {
	os << "[\n";
	tab++;
	for(auto it = a->values.begin(); it != a->values.end();) {
		printTabs();

		// Send this visitor to the Value to print it.
		(*it)->accept(*this);

		it++;
		// Print a comma if the array contains more values.
		if(it != a->values.end())
			os << ",\n";
	}
	tab--;
	os << '\n';
	printTabs();
	os << ']';
}
void json::Document::Printer::visit(True* t) {
	os << "true";
}
void json::Document::Printer::visit(False* f) {
	os << "false";
}
void json::Document::Printer::visit(Null* n) {
	os << "null";
}
void json::Document::Printer::visit(Number* n) {
	os << n->value;
}

// Output 2 empty spaces for each tab of the printer visitor.
void json::Document::Printer::printTabs() {
	for(int i = 0; i < tab; ++i) 
		os << "  ";
}

//*****************************
// Filter member functions
//*****************************

void json::Document::Filter::visit(String* s) { }
void json::Document::Filter::visit(Object* o) {
	Object* object = new Object();

	for(std::string& s : o->insertOrder) {	// Iterate through each key value of o.
		if(std::find(args.begin(), args.end(), s) != args.end()) { 
			// Key matches an argument.

			Filter f(args);
			o->values[s]->accept(f);

			object->insertOrder.push_back(s); // Add the key value for this pair.
			object->values[s] = o->values[s]; // Add the value to the pair.

		} else {								
			// Key does not match an argument.
			Filter f(args);				// Filter the value to see if it contains an argument at a deeper level.
			o->values[s]->accept(f);

			if(f.result) {				// Value contains argument at a lower level.
				object->insertOrder.push_back(s);	// Add the result returned by the filter to the new object.
				object->values[s] = f.result;
			}
		}
	}

	if(object->insertOrder.empty()) 	// If no elements are found that match the arguments
		delete object;					// Delete the empty object.
	else
		result = object;				// Otherwise, set the object as the result for this filter.
}
void json::Document::Filter::visit(Array* a) {
	Array* array = new Array();

	for(Value* v : a->values) {	// Iterate through each value contained by a.
		Filter f(args);
		v->accept(f);			// Filter each element.

		if(f.result) 			// If filter finds a result, add it to the new array.
			array->values.push_back(f.result);
	}

	if(array->values.empty()) 	// If no elements are found
		delete array;			// Delete the empty array.
	else
		result = array;			// Otherwise, set the array as the result for this filter.
}
void json::Document::Filter::visit(True* t) { }
void json::Document::Filter::visit(False* f) { }
void json::Document::Filter::visit(Null* n) { }
void json::Document::Filter::visit(Number* n) { }

//*****************************
// Exporter member functions
//*****************************

void json::Document::Exporter::visit(String* s) { 
	output += '\"' + s->value + '\"';
}
void json::Document::Exporter::visit(Object* o) {
	output += '{';
	for(auto it = o->insertOrder.begin(); it != o->insertOrder.end();) {
		output += '\"' + *it + "\": ";
		o->values[*it]->accept(*this);

		it++;
		if(it != o->insertOrder.end())
			output += ", ";
	} 
	output += '}';
}
void json::Document::Exporter::visit(Array* a) { 
	output += '[';
	for(auto it = a->values.begin(); it != a->values.end();) {
		(*it)->accept(*this);

		it++;
		if(it != a->values.end())
			output += ", ";
	}
	output += ']';
}
void json::Document::Exporter::visit(True* t) { 
	output += "true";
}
void json::Document::Exporter::visit(False* f) { 
	output += "false";
}
void json::Document::Exporter::visit(Null* n) { 
	output += "null";
}
void json::Document::Exporter::visit(Number* n) { 
	output += n->value;
}

//*****************************
// Duplicator member functions
//*****************************

void json::Document::Duplicator::visit(String* s) {
	String* news = new String();
	news->value = s->value;
	copy = news;
}
void json::Document::Duplicator::visit(Object* o) {
	Object* newo = new Object();

	for(auto it = o->insertOrder.begin(); it != o->insertOrder.end(); ++it) {
		// Make a duplicator for each value, and add the copied value to the new object.
		Duplicator d;
		o->values[*it]->accept(d);
		newo->insertOrder.push_back(*it);
		newo->values[*it] = d.copy;
	}

	copy = newo;
}
void json::Document::Duplicator::visit(Array* a) {
	Array* newa = new Array();

	for(auto it = a->values.begin(); it != a->values.end(); ++it) {
		// Make a duplicator for each value, and add the copied value to the new array.
		Duplicator d;
		(*it)->accept(d);
		newa->values.push_back(d.copy);
	}

	copy = newa;
}
void json::Document::Duplicator::visit(True* t) {
	copy = new True();
}
void json::Document::Duplicator::visit(False* f) {
	copy = new False();
}
void json::Document::Duplicator::visit(Null* n) {
	copy = new Null();
}
void json::Document::Duplicator::visit(Number* n) {
	Number* newn = new Number();
	newn->value = n->value;
	copy = newn;
}

// Global operator overload for printing Documents.
std::ostream& operator<<(std::ostream& os, json::Document& j) {
	j.print(os);
	return os;
}