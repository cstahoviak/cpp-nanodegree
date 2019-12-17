#include <cassert>
#include <string>

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

template <typename Type> Type Max( Type a, Type b ) {
  return a > b ? a : b;
}

// TODO: Create a generic function Product that multiplies two parameters
template <typename Type> Type Product( Type a, Type b) {
  return a * b;
}

int main() {
  assert(Max<int>(2,4) == 4);
  assert(Max<double>(-1.0,-2.3) == -1.0);
  assert(Max<char>('a','b') == 'b');

  assert(Product<int>(10, 2) == 20);
}