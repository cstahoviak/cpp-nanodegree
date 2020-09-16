#include <cassert>
#include <string>
#include <vector>
#include <sstream>

/* TEMPLATES:
* Templates enable generic programming by generalizing a function to apply to
* any class. Specifically, templates use types as parameters so that the same
* implementation can operate on different data types.
* We use the keyword template to specify which function is generic. Generic
* code is the term for code that is independent of types. It is mandatory to
* put the template<> tag before the function signature, to specify and mark
* that the declaration is generic.

* "Templates are how C++ implements 'generic programming'." - Bjarne

* "C++ supports generic programming in two ways: templates & concepts. Templates
* do not allow us to express/specify the 'requirements' of the elements of a
* vector<>, for example. C++20 will have introduce 'concepts' which will allow
* you to precisely specify a template's requirements of its occupants." - Bjarne
*/

template <typename type> type Max( type a, type b ) {
  return a > b ? a : b;
}

// TODO: Create a generic function Product that multiplies two parameters
template <typename type> type Product( type a, type b) {
  return a * b;
}

// TODO: Declare a generic, templatized average function
template <typename type> type Average( type a, type b) {
  return (a + b) / (type)2;
}

/* CLASS TEMPLATES:
* Classes are the building blocks of object oriented programming in C++.
* Templates support the creation of generic classes! Class templates can
* declare and implement generic attributes for use by generic methods. These
* templates can be very useful when building classes that will serve multiple
* purposes.
*/

// TODO: Add the correct template specification
template <typename KeyType, typename ValueType> class Mapping {
public:
  Mapping(KeyType key, ValueType value) : key(key), value(value) {}
  std::string Print() const {
    std::ostringstream stream;
    stream << key << ": " << value;
    return stream.str();
  }
  // KeyType and ValueType are 'generic templates' in this context
  KeyType key;
  ValueType value;
};

int main() {
  assert(Max<int>(2,4) == 4);
  assert(Max<double>(-1.0,-2.3) == -1.0);
  assert(Max<char>('a','b') == 'b');

  assert(Product<int>(10, 2) == 20);

  /* DEDEUCTION:
  * Deduction occurs when you instantiate an object without explicitly identifying the
  * types. Instead, the compiler "deduces" the types. This can be helpful for writing
  * code that is generic and can handle a variety of inputs.
  */
  std::vector<int> v1{1,2,3};
  // std::vector v2{1,2,3};

  assert(v1.size() == 3);
  // assert(v2.size() == 3);   // will work if compiled with c++17 (g++ -std=c++17 NAME.cpp)

  // type int is 'deduced' by the compliter without explicitly calling Average<int>()
  assert(Average(2,4) == 3);

  Mapping<std::string, int> mapping("age", 20);
  assert(mapping.Print() == "age: 20");

}