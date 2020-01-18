#include <stdlib.h>
#include <iostream>

/* MyInt: An RAII Management class
* The constructor of class MyInt takes a pointer to a memory resource.
  When the destructor of a MyInt object is called, the resource is
  deleted from memory - which makes MyInt an RAII memory management
  class. Also, the * operator is overloaded which enables us to
  dereference MyInt objects in the same manner as with raw pointers.
*/

class MyInt
{
  private:
    int *_p; // pointer to heap data
  public:
    MyInt(int *p = NULL) { _p = p; }
    ~MyInt() { 
      std::cout << "resource " << *_p << " deallocated" << std::endl;
      delete _p; 
    }
    int &operator*() { return *_p; }  // overload dereferencing operator
};

int main()
{
  double den[] = {1.0, 2.0, 3.0, 4.0, 5.0};

  // ASIDE: how to get size of an array
  std::size_t sz_den = sizeof(den);
  std::size_t sz_den0 = sizeof(den[0]);
  std::size_t size = sizeof(den) / sizeof(den[0]);

  std::cout << "sizeof(den) = " << sz_den << "\n";
  std::cout << "sizeof(den[0]) = " << sz_den0 << "\n";
  std::cout << "size of den = " << size << "\n\n";

  for (std::size_t i = 0; i < size; ++i) {
    // allocate the resource on the heap
    double *en = new double(i);

    // use the resource
    std::cout << *en << "/" << den[i] << " = " << *en / den[i] << std::endl;

    // deallocate the resource
    delete en;
  }
  std::cout << "\n";

  /* We can make use of RAII by rewriting the above for loop:
  * 1. The part new int(i) creates a new block of memory on the heap
  * and initializes it with the value of i. The returned result is the
  * address of the block of memory.
  * 
  * 2. The part MyInt en(...) calls the constructor of class MyInt,
  * passing the address of a valid memory block as a parameter.
  * 
  * NOTE: By creating the MyInt object on the stack, we ensure that
  * the deallocation occurs as soon as the object goes out of scope,
  * i.e. after each iteration of the for loop
  */
  for (std::size_t i = 0; i < size; ++i) {
    // allocate the resource on the STACK
    MyInt en(new int(i));

    // use the resource
    std::cout << *en << "/" << den[i] << " = " << *en / den[i] << std::endl;
  }
  std::cout << "\n";

  /* Q: What would happen in the implementation below?
  *  A: The destructor of MyInt would never be called, hence causing a
  *  memory leak with each loop iteration.
  */
  // for (std::size_t i = 0; i < size; ++i) {
  //   // allocate the resource on the HEAP
  //   MyInt *en1 = new MyInt(new int(i));

  //   // use the resource
  //   std::cout << *en1 << "/" << den[i] << " = " << *en1 / den[i] << std::endl;
  // }
  // std::cout << "\n";


  return 0;
}