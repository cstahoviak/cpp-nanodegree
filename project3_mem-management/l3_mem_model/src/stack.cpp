#include <stdio.h>
#include <iostream>

/* SCOPE:
*   Only 2 local variables (myInt and isBelowThreshold) exist within
*   the "scope" of MyLocalFunction(). These two variables are
*   automatically created on the "stack".
*/
bool MyLocalFunction(int myInt) {
	bool isBelowThreshold = myInt < 42 ? true : false;
	return isBelowThreshold;
}

/* PASS-BY-VALUE:
*   1. Passing parameters by value means that a copy is created,
*   which is an expensive operation that might consume large amounts
*   of memory, depending on the data that is being transferred.
*
*   2. Passing by value also means that the created copy can not be
*   used as a back channel for communicating with the caller, for
*   example by directly writing the desired information into the
*   variable.
*/
void AddTwo(int val) {
  val += 2;
}

/* PASS-BY-POINTER:
*   1. In this case, a local variable which stores an address is
*   created on the stack by calling AddThree(). But by passing an
*   address, we have a "backchannel" to modify the variable val that
*   is in the scope of main().

*   Q: What if I change * to &? -> See AddFour()!
*/
void AddThree(int *val) {
  // pointer dereference operator (*) allows us modify variable val
  // that is in scope of main()
  *val += 3;    
}

/* PASS-BY-REFERENCE:
*   1. AddFour() receives a reference to the parameter, rather than a
*   copy of its value.
*
*   2. As with the example of AddThree above, the function can now
*   modify the argument such that the changes also happen on the
*   caller side.
*
*   3. Additionally, passing variables by reference is also faster as
*   no information needs to be copied, as well as more memory-efficient.
*
*   4. Steps can be taken to protect data from innappropriate modification
*   by the called function (AddFour() in this case).
*/
void AddFour(int &val) {
  val += 4;
}

/* PASS-BY-REFERENCE: MODIFY MULTIPLE VARIABLES
*   Returning multiple variables can be cumbersome. An alternative is
*   is to modify modify multiple variables in the scope of the caller
*   via passing references to multiple variables.
*/
void AddFive(int &val, bool &success) {
  val += 5;
  success = true;
}

/* POINTER VS REFERENCES:
*   the use of pointers and references to directly manipulate function
*   arguments in a memory-effective way is very similar. Pointers and
*   references are both implemented by using a memory address.
*
*   1. Pointers can be declared without initialization. This means we
*   can pass an uninitialized pointer to a function who then internally
*   performs the initialization for us.
*
*   2. Pointers can be reassigned to another memory block on the heap.
*
*   3. References are usually easier to use (depending on the expertise
*   level of the programmer). Sometimes however, if a third-party function
*   is used without properly looking at the parameter definition, it
*   might go unnoticed that a value has been modified.
*/

int main() {

  bool res = MyLocalFunction(23);

  int val = 0;
  AddTwo(val);
  std::cout << "val = " << val << std::endl;

  val = 0;
  AddThree(&val);
  std::cout << "val = " << val << std::endl;

  val = 0;
  int* val_ptr = &val;
  AddThree(val_ptr);
  std::cout << "val = " << val << std::endl;

  val = 0;
  AddFour(val);
  std::cout << "val = " << val << std::endl;

  val = 0;
  bool success = false;
  AddFive(val, success);
  val += 2;
  std::cout << "val = " << val << ", success = " << success << std::endl;

  /* NOTE:
  *   In order to benefit from call-by-reference, the size of the data
  *   type passed to the function has to surpass the size of the
  *   pointer on the respective architecture (i.e. 32 bit or 64 bit).
  * 
  *   As an experiment, you could use the -m32 compiler flag to build
  *   a 32 bit version of the program. This yields the following output:
  * 
  *   size of int: 4
  *   size of *int: 4
  */
  printf("\nsize of int: %lu\n", sizeof(int));
  printf("size of *int: %lu\n", sizeof(int *));

  return 0;
}