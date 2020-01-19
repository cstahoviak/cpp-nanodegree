#include <stdlib.h>
#include <iostream>
#include <memory>

/* MyInt: An RAII Management class
* The constructor of class MyInt takes a pointer to a memory resource.
  When the destructor of a MyInt object is called, the resource is
  deleted from memory - which makes MyInt an RAII memory management
  class. Also, the * operator is overloaded which enables us to
  dereference MyInt objects in the same manner as with raw pointers.
*/

class MyInt {
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


/* RAW POINTER:
* 1. allocating memory on the heap with new and store the address in
a pointer variable.
* 2. assign a value to the memory block via the dereference operator *
* 3. delete the resource on the heap. NOTE: Forgetting to call delete
will result in a memory leak.
*/
void RawPointer() {
  int *raw = new int; // create a raw pointer on the heap
  *raw = 1;           // assign value via the dereference operator
  delete raw;         // delete the heap resource
}

/* UNIQUE POINTER:
* A smart pointer is a class template that is declared on the STACK
  and then initialized by a raw pointer (returned by new) to a
  HEAP-allocated object. The smart pointer is now responsible for
  deleting the memory that the raw pointer specifies - which happens
  as soon as the smart pointer goes out of scope. NOTE: smart
  pointers always need to be declared on the stack, otherwise the
  scoping mechanism would not work.

  NOTE: a key feature of a unique pointer, which makes it so well-
  suited as a return type for many functions, is the possibility to
  convert it to a "shared pointer".
*/
void UniquePointer() {
  std::unique_ptr<int> unique(new int); // create a unique pointer on the stack
  *unique = 2; // assign a value
  // delete is not neccessary
}

class MyClass {
  private:
    std::string _text;

  public:
    MyClass() {}
    MyClass(std::string text) { _text = text; }
    ~MyClass() { std::cout << this << " destroyed" << std::endl; }
    void setText(std::string text) { _text = text; }
    std::string getText() { return _text; }
};

class SharedClass {
public:
    ~SharedClass() { std::cout << "Destructor of SharedClass called" << std::endl; }
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

  // UNIQUE POINTERS:
  std::cout << "UNIQUE POINTERS:" << "\n";

  // create unique pointers to proprietary class
  std::unique_ptr<MyClass> myClass1(new MyClass());
  std::unique_ptr<MyClass> myClass2(new MyClass("String 2"));

  /* NOTE: On stack/heap locations of smart pointers and the objects
     that they point to.
  
    1. The unique pointers MyClass1 and myClass2 exist on the STACK
    2. The instances of Myclass object which are managed by the
    myClass1 and myClass2 pointers, respectively, exist on the HEAP. We
    can use the .ge() function to return the momory address of the
    object on the HEAP.
  */

  /* NOTE: *, ->, and .

  (*) the dereference operator - used to access the content (member
  variables) of myClass1 and myClass2 and assign the one to the other.
  (->) used to access public members (variables and functions) of the
  class.
  (.) gives us access to proprietary functions of the smart pointer,
  such as retrieving the internal raw pointer with get().
  */

  myClass1->setText("String 1");  // call member function using ->
  std::cout << "myClass1._text = " << myClass1->getText() << "\n";
  std::cout << "myClass2._text = " << myClass2->getText() << "\n";

  // use the dereference operator * to copy "content" of object
  *myClass1 = *myClass2;
  std::cout << "myClass1._text = " << myClass1->getText() << "\n";
  std::cout << "myClass2._text = " << myClass2->getText() << "\n";

  // use the .get() function to retrieve a raw pointer to the object
  std::cout << "Objects have stack addresses " << &myClass1 << " and " << &myClass2 << std::endl;
  std::cout << "Objects have heap addresses " << myClass1.get() << " and " << myClass2.get() << std::endl;

  // use std::move to have myClass3 to "point to" heap address of
  // myClass object previously pointed to/managed by myClass1 pointer
  myClass1->setText("String 1");
  auto myClass3 = std::move(myClass1);
  std::cout << "Objects have stack addresses " << &myClass3 << " and " << &myClass2 << std::endl;
  std::cout << "Objects have heap addresses " << myClass3.get() << " and " << myClass2.get() << std::endl;

  // line below will cause a segfault because the unique pointer
  // myClass1 no longer "points to" a myClass object
  // NOTE: no compiler error/warning is generated!
  // std::cout << "myClass1._text = " << myClass1->getText() << "\n";

  std::cout << "myClass2._text = " << myClass2->getText() << "\n";
  std::cout << "myClass3._text = " << myClass3->getText() << "\n\n";


  // SHARED POINTERS:
  std::cout << "SHARED POINTERS:" << "\n";

  std::shared_ptr<int> shared1(new int);
  std::cout << "shared pointer count = " << shared1.use_count() << std::endl;

  { // creates its own scope within { and }, thus shared2 goes out of
    // scope after the end curly brace }
    std::shared_ptr<int> shared2 = shared1;
    std::cout << "shared pointer count = " << shared1.use_count() << std::endl;
  }
  
  std::cout << "shared pointer count = " << shared1.use_count() << std::endl;
  std::cout << "\n";

  /* SHARED POINTER: RESET
  * A shared pointer can also be redirected by using the reset()
  * function. If the resource which a shared pointer manages is no
  * longer needed in the current scope, the pointer can be reset to
  * manage a difference resource.
  */
  std::shared_ptr<SharedClass> shared(new SharedClass);
  std::cout << "shared pointer count = " << shared.use_count() << std::endl;

  shared.reset(new SharedClass);
  std::cout << "shared pointer count = " << shared.use_count() << std::endl;

  /* WEAK POINTERS:
  * Similar to shared pointers, there can be multiple weak pointers to
  * the same resource. The main difference though is that weak pointers
  * do not increase the reference count. Weak pointers hold a non-
  * owning reference to an object that is managed by another shared
  * pointer.
  * 
  * 
  * The following rule applies to weak pointers: You can only create\
  * weak pointers out of shared pointers or out of another weak pointer.
  */
  std::cout << "\nWEAK POINTERS:" << "\n";

  std::shared_ptr<int> mySharedPtr(new int);
  std::cout << "shared pointer count = " << mySharedPtr.use_count() << std::endl;

  std::weak_ptr<int> myWeakPtr1(mySharedPtr);
  std::weak_ptr<int> myWeakPtr2(myWeakPtr1);
  std::cout << "shared pointer count = " << mySharedPtr.use_count() << std::endl;

  // COMPILE ERROR: violated the rule mentioned above
  // std::weak_ptr<int> myWeakPtr3(new int);

  /*NOTE:
  * s we have seen with raw pointers, you can never be sure wether the
  * memory resource to which the pointer refers is still valid.
  * 
  * With a weak pointer, even though this type does not prevent an
  * object from being deleted, the validity of its resource can be
  * checked via std::weak_ptr::expired()
  * 
  * Thus, with smart pointers, there will always be a managing
  * instance which is responsible for the proper allocation and
  * deallocation of a resource.
  */
  std::shared_ptr<int> mySharedPtr2(new int);
  std::weak_ptr<int> myWeakPtr(mySharedPtr2);

  mySharedPtr2.reset(new int);

  if (myWeakPtr.expired() == true) {
    std::cout << "Weak pointer expired!" << std::endl;
  }

  // converting between smart pointer types:
  // construct a unique pointer
  std::unique_ptr<int> uniquePtr(new int);
  
  // (1) shared pointer from unique pointer
  std::shared_ptr<int> sharedPtr1 = std::move(uniquePtr);

  // (2) shared pointer from weak pointer
  std::weak_ptr<int> weakPtr(sharedPtr1);
  std::shared_ptr<int> sharedPtr2 = weakPtr.lock();

  // (3) raw pointer from shared (or unique) pointer   
  int *rawPtr = sharedPtr2.get();
  delete rawPtr;  // doesn't produce malloc error as expected..??

  return 0;
}