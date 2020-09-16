#include <iostream>

/* L-VALUES & R-VALUES:
* L-VALUES
    Have an address that can be accessed. They are expressions whose
    evaluation by the compiler determines the identity of objects or
    functions.

* R-VALUES:
    Do not have an address that is accessible directly. They are
    temporary expressions used to initialize objects or compute the
    value of the operand of an operator.

* In more general terms, an lvalue is an entity that points to a
  specific memory location. An rvalue is usually a short-lived object,
  which is only needed in a narrow local scope. To simplify things a
  little, one could think of lvalues as named containers for rvalues.
*/

/* L-VALUE REFERENCES in PASS-BY-REFERENCE:
* One of the primary use-cases for lvalue references is the
  pass-by-reference semantics in function calls as in the example on
  the right.

* The function myFunction has an lvalue reference as a parameter,
  which establishes an alias to the integer i which is passed to it
  in main.
*/
void myFunction(int &val)
{
    ++val;
}

int main() {
  // initialize some variables on the stack
  int i, j, *p;

  // correct usage of lvalues and rvalues
  
  i = 42; // i is an lvalue and 42 is an rvalue
  
  // Q: a de-referenced pointer can also be an rvalue? Yes, I think so
  p = new int;
  *p = i; // the dereferenced pointer is an lvalue
  delete p; 
  
  ((i < 42) ? i : j) = 23; // the conditional operator returns an lvalue (eiter i or j)

  // incorrect usage of lvalues and rvalues
  //42 = i; // error : the left operand must be an lvalue
  //j * 42 = 23; // error : the left operand must be an lvalue

  /* L-VALUE REFERENCES:
  * The short code sample below declares an integer i and a reference
    j which can be used as an alias for the existing object.
  */
  int k = 1; 
  int &l = k; 
  ++k;
  ++l;
  std::cout << "k = " << k << ", l = " << l << std::endl;

  // L-VALUE REFERENCES: PASS-BY-VALUE
  int x = 1; 
  myFunction(x);
  std::cout << "x = " << x << std::endl;

  /* The below lines will produce the following compiler error:
      error: cannot bind non-const lvalue reference of type ‘int&’
      to an rvalue of type ‘int’
  */
  // myFunction(42);
  // int y = 23; 
  // myFunction(x+y);

  /* R-VALUE REFERENCES:
  * Since C++11, there is a new type available called rvalue reference,
  * which can be identified from the double ampersand && after a type
  * name. With this operator, it is possible to store and even modify
  * an rvalue, i.e. a temporary object which would otherwise be lost
  * quickly. But what do we need this for?
  * 
  * The expression int &&w creates an rvalue reference, to which the
  * address of the temporary object is assigned, that holds the result
  * of the addition. So instead of first creating the rvalue i+j , then
  * copying it and finally deleting it, we can now hold the temporary
  * object in memory. This is much more efficient than the first
  * approach.
  * 
  * One of the most important aspects of rvalue references is that they
  * pave the way for move semantics, which is a mighty technique in
  * modern C++ to optimize memory usage and processing speed. Move
  * semantics and rvalue references make it possible to write code that
  * transfers resources such as dynamically allocated memory from one
  * object to another in a very efficient manner.
  */
  int y = 2; 
  int z = x + y; 
  int &&w = x + y; 
  std::cout << "z = " << z << ", w = " << w << std::endl;

  return 0;
}