#include <stdlib.h>
#include <iostream>

/* MALLOC/FREE vs NEW/DELETE:
*   Major differences between malloc/free and new/delete:
*   
*   1. Constructors / Destructors: Unlike malloc( sizeof(MyClass) ),
*   the call new MyClass() calls the constructor. Similarly, delete
*   calls the destructor.
*
*   2. Type safety: malloc returns a void pointer, which needs to be
*   cast into the appropriate data type it points to. This is not type
*   safe, as you can freely vary the pointer type without any warnings
*   or errors from the compiler as in the following small example:
*
*     MyObject *p = (MyObject*)malloc(sizeof(int));
*
*   In C++, the call MyObject *p = new MyObject() returns the correct
*   type automatically - it is thus type-safe.
*/

/* REASONS FOR OVERLOADING NEW / DELETE:
*   1. The overloaded new operator function allows to add additional
*   parameters. Therefore, a class can have multiple overloaded new
*   operator functions. This gives the programmer more flexibility in
*   customizing the memory allocation for objects.
*
*   2. Overloaded the new and delete operators provides an easy way to
*   integrate a mechanism similar to garbage collection capabilities
*   (such as in Java), as we will shorty see later in this course.
*
*   3. By adding exception handling capabilities into new and delete,
*   the code can be made more robust.
*
*   4. t is very easy to add customized behavior, such as overwriting
*   deallocated memory with zeros in order to increase the security of
*   critical application data.
*/

class MyClass {
private:
  int *_number;

public:
  MyClass() {
    std::cout << "Allocate memory\n";
    _number = (int *)malloc(sizeof(int));
  }
  ~MyClass() {
    std::cout << "Delete memory\n";
    free(_number);
  }
  void setNumber(int number) {
    *_number = number;
    std::cout << "Number: " << *_number << "\n";
  }
};


// NEW/DELETE OVERLOADING:
class MyClass2 {
  int _mymember;

  public:
    MyClass2() {
      std::cout << "Constructor is called\n";
    }

    ~MyClass2() {
      std::cout << "Destructor is called\n";
    }

    // The operator receives a parameter size of type size_t, which
    // specifies the number of bytes of memory to be allocated. The
    // return type of the overloaded new is a void pointer, which
    // references the beginning of the block of allocated memory.
    void *operator new(size_t size) {
      std::cout << "new: Allocating " << size << " bytes of memory" << std::endl;
      void *p = malloc(size);

      return p;
    }

    // overlaoad new to allocate array of MyClass2 objects
    void *operator new[](size_t size) {
      std::cout << "new: Allocating " << size << " bytes of memory" << std::endl;
      void *p = malloc(size);

      return p;
    }

    // The operator takes a pointer to the object which is to be
    // deleted. As opposed to new, the operator delete does not have
    // a return value.
    void operator delete(void *p) {
      std::cout << "delete: Memory is freed again " << std::endl;
      free(p);
    }

    void operator delete[](void *p) {
      std::cout << "delete: Memory is freed again " << std::endl;
      free(p);
    }
};

int main() {
  /* Create object pointer using MALLOC:
  * If we were to create a C++ object with malloc, the constructor
  * and destructor of such an object would not be called.
  */
  // allocate memory using malloc (causes segfault)
  // MyClass *myClass = (MyClass *)malloc(sizeof(MyClass));
  // myClass->setNumber(42); // EXC_BAD_ACCESS
  // free(myClass);
  
  /* The call to new does the following:
  *   1. Memory is allocated to hold a new object of type MyClass.
  *   2. A new object of type MyClass is constructed within the
  *   allocated memory by calling the constructor of MyClass.
  */
  MyClass *myClass = new MyClass();
  myClass->setNumber(42); // works as expected

  /* The call to delete does the following:
  *   1. The object of type MyClass is destroyed by calling its destructor.
  *   2. The memory which the object was placed in is deallocated.
  */
  delete myClass;
  std::cout << "\n";

  /* PERFORMANCE OPTIMIZATION with "PLACEMENT NEW":
  * In some cases, it makes sense to separate memory allocation from
  * object construction. Consider a case where we need to reconstruct
  * an object several times. If we were to use the standard new/delete
  * construct, memory would be allocated and freed unnecessarily as
  * only the content of the memory block changes but not its size. By
  * separating allocation from construction, we can get a significant
  * performance increase.
  * 
  * C++ allows us to do this by using a construct called "placement
  * new". With placement new, we can pass a preallocated memory and
  * construct an object at that memory location. Consider the
  * following code:
  * 
  * The syntax "new (memory)"" is denoted as "placement new". The
  * difference to the "conventional" new we have been using so far is
  * that that no memory is allocated.
  */
  void *memory = malloc(sizeof(MyClass));
  MyClass *object = new (memory) MyClass;
  object->setNumber(42);

  // There isno delete equivalent to placement new, so we have to call
  // the destructor explicitly in this case instead of using delete as
  // we would have done with a regular call to new:
  // IMPORTANT NOTE: should never be done outside of "placement new".
  object->~MyClass();
  free(memory);
  std::cout << "\n";

  /* NEW/DELETE OVERLOADING terminal output:
  *   new: Allocating 4 bytes of memory
  *   Constructor is called
  *   Destructor is called
  *   delete: Memory is freed again
  */
  MyClass2 *p = new MyClass2();
  delete p;
  std::cout << "\n";

  /* NEW/DELETE OVERLOADING for array of objects:
  *   new: Allocating 20 bytes of memory
  *   Constructor is called
  *   Constructor is called
  *   Constructor is called
  *   Destructor is called
  *   Destructor is called
  *   Destructor is called
  *   delete: Memory is freed again
  *
  * Interestingly, the memory requirement is larger than expected: With
  * new, the block size was 4 bytes, which is exactly the space
  * required for a single integer. Thus, with three integers, it should
  * now be 12 bytes instead of 20 bytes. The reason for this is the
  * memory allocation overhead that the compiler needs to keep track
  * of the allocated blocks of memory - which in itself consumes memory.
  * If we change the above call to e.g. new MyClass[100](), we will see
  * that the overhead of 8 bytes does not change:
  */ 
  MyClass2 *p2 = new MyClass2[3]();
  delete[] p2;
  std::cout << "\n";

  return 0;
}