#include <iostream>

// (DEFAULT) SHALLOW COPY
class MyClass {
  private:
    int *_myInt;

  public:
    MyClass() { _myInt = (int *)malloc(sizeof(int)); }
    ~MyClass() { free(_myInt); }

    void printOwnAddress() { std::cout << "SHALLOW COPY: Own address on the stack is " << this << std::endl; }
    void printMemberAddress() { std::cout << "SHALLOW COPY: Managing memory block on the heap at " << _myInt << std::endl; }
};

/* NO COPYING POLCIY:
* The simplest policy of all is to forbid copying and assigning class
* instances all together.
*
* Method 1: NoCopyClass1
*   This can be achieved by declaring, but not defining a private copy
*   constructor and assignment operator.
*
* Method 2: NoCopyClass2
*   alternatively this can be done by making both public and assigning
*   the delete operator.
*
* The second choice is more explicit and makes it clearer to the
* programmer that copying has been actively forbidden.
*/
class NoCopyClass1 {
  private:
    NoCopyClass1(const NoCopyClass1 &);
    NoCopyClass1 &operator=(const NoCopyClass1 &);

  public:
    NoCopyClass1(){};
};

class NoCopyClass2 {
  public:
    NoCopyClass2(){}
    NoCopyClass2(const NoCopyClass2 &) = delete;
    NoCopyClass2 &operator=(const NoCopyClass2 &) = delete;
};

/* EXCLUSIVE OWNERSHIP POLICY:
* This policy states that whenever a resource management object is
* copied, the resource handle is transferred from the source pointer
* to the destination pointer. In the process, the source pointer is
* set to nullptr to make ownership exclusive. At any time, the resource
* handle belongs only to a single object, which is responsible for its
* deletion when it is no longer needed.
*
* NOTE: this example is NOT the way exclusive ownership is handled in
* the standard template library. One problem in this implementation is
* that for a short time there are effectively two valid handles to the
* same resource - after the handle has been copied and before it is set
* to nullptr. In concurrent programs, this would cause a data race for
* the resource. A much better alternative to handle exclusive ownership
* in C++ would be to use move semantics, which we will discuss shortly
* in a very detailed lesson.
*/
class ExclusiveCopy {
  private:
    int *_myInt;  // pointer to heap allocated resource or size int

  public:
    // resource allocation for _myInt performed by constructor
    ExclusiveCopy() {
      _myInt = (int *)malloc(sizeof(int));
      std::cout << "EXCLUSIVE COPY: resource allocated" << std::endl;
    }

    ~ExclusiveCopy() {
      // only deallocate resource if pointer is NOT null. This protects
      // against "double-deletion" of same resource
      if (_myInt != nullptr) {
        free(_myInt);
        std::cout << "EXCLUSIVE COPY: resource freed" << std::endl;
      }     
    }

    // ensures that copied object is the only object with a pointer to
    // the allocated memory pointed to by int* _myInt.
    ExclusiveCopy(ExclusiveCopy &source) {
      _myInt = source._myInt;
      source._myInt = nullptr;
    }

    ExclusiveCopy &operator=(ExclusiveCopy &source) {
      _myInt = source._myInt;
      source._myInt = nullptr;
      // dereference "this" ptr so that an ExclusiveCopy object is returned
      return *this;
    }
};

/* DEEP COPYING POLICY:
* With this policy, copying and assigning class instances to each other
* is possible without the danger of resource conflicts. The idea is to
* allocate proprietary memory in the destination object and then to
* copy the content to which the source object handle is pointing into
* the newly allocated block of memory. This way, the content is
* preserved during copy or assignment. However, this approach increases
* the memory demands and the uniqueness of the data is lost: After the
* deep copy has been made, two versions of the same resource exist in
* memory.
*/

class DeepCopy {
  private:
    int *_myInt;

  public:
    DeepCopy(int val) {
      _myInt = (int *)malloc(sizeof(int));
      *_myInt = val;
      std::cout << "DEEP COPY: resource allocated at address " << _myInt << std::endl;
    }

    ~DeepCopy() {
      free(_myInt);
      std::cout << "DEEP COPY: resource freed at address " << _myInt << std::endl;
    }

    DeepCopy(DeepCopy &source) {
      _myInt = (int *)malloc(sizeof(int));
      *_myInt = *source._myInt;
      std::cout << "DEEP COPY: resource allocated at address " << _myInt << " with _myInt = " << *_myInt << std::endl;
    }

    DeepCopy &operator=(DeepCopy &source) {
      _myInt = (int *)malloc(sizeof(int));
      std::cout << "DEEP COPY: resource allocated at address " << _myInt << " with _myInt=" << *_myInt << std::endl;
      *_myInt = *source._myInt;
      return *this;
    }
};

/* SHARED OWNEESHIP POLICY:
* The idea is to perform a copy or assignment similar to the default
* behavior, i.e. copying the handle instead of the content (as with a
* shallow copy) while at the same time keeping track of the number of
* instances that also point to the same resource. Each time an instance
* goes out of scope, the counter is decremented. Once the last object
* is about to be deleted, it can safely deallocate the memory resource.
* We will see later in this course that this is the central idea of
* unique_ptr, which is a representative of the group of smart pointers.
*/

class SharedCopy {
  private:
    int *_myInt;
    static int _cnt;

  public:
    SharedCopy(int val);
    ~SharedCopy();
    SharedCopy(SharedCopy &source);
};

int SharedCopy::_cnt = 0;

SharedCopy::SharedCopy(int val) {
  _myInt = (int *)malloc(sizeof(int));
  *_myInt = val;
  ++_cnt;
  std::cout << "SHARED COPY: resource allocated at address " << _myInt << std::endl;
}

SharedCopy::~SharedCopy() {
  --_cnt;
  if (_cnt == 0) {
    free(_myInt);
    std::cout << "SHARED COPY: resource freed at address " << _myInt << std::endl;
  }
  else {
    std::cout << "SHARED COPY: instance at address " << this << " goes out of scope with _cnt = " << _cnt << std::endl;
  }
}

SharedCopy::SharedCopy(SharedCopy &source)
{
    _myInt = source._myInt;
    ++_cnt;
    std::cout << "SHARED COPY: " << _cnt << " instances with handles to address " << _myInt << " with _myInt = " << *_myInt << std::endl;
}

/* RULE OF THREE:
* The Rule of Three states that if a class needs to have an overloaded
* copy constructor, copy assignment operator, ~or~ destructor, then it
* must also implement the other two as well to ensure that memory is
* managed consistently. As we have seen, the copy constructor and copy
* assignment operator (which are often almost identical) control how
* the resource gets copied between objects while the destructor manages
* the resource deletion.
*
* NOTE: On SharedCopy Class
*   SharedCopy does not implement the assignment operator, thus
*   violating the Rule of Three. If we were to use something like:
      destination3 = source
    instead of:
      SharedCopy destination3(source)
    then the counter variable would not be properly decremented.
*
* NOTE: RULE OF FIVE:
    In modern (C++11 and greater), the concept of "move sematics" is
    introduced and along with this comes the "Rule of Five" to be
    discussed in a later lesson.
*/

int main() {
  // for testing purposes only
  int a = 0; int b = 0; int c = 0;
  std::cout << "a = " << a << "\tb= " << b << "\tc= " << c << "\n";
  b = a++;
  std::cout << "a = " << a << "\tb= " << b << "\tc= " << c << "\n";
  c = ++a;
  std::cout << "a = " << a << "\tb= " << b << "\tc= " << c << "\n\n";

  // instantiate object 1
  MyClass myClass1;
  myClass1.printOwnAddress();
  myClass1.printMemberAddress();

  // copy object 1 into object 2 via "copy constructor"
  // performs a "shallow copy" in which the the _myInt pointers
  // associated with each object will point to the location in memory
  MyClass myClass2(myClass1); 
  myClass2.printOwnAddress();
  myClass2.printMemberAddress();
  std::cout << "\n";

  /* NOTE:
  * I am not getting the error described my pg 3/11 of the "Copy
  * Semantics" lesson:
  *   Own address on the stack is 0x7ffeefbff670
  *   Managing memory block on the heap at 0x100300060
  *   Own address on the stack is 0x7ffeefbff658
  *   Managing memory block on the heap at 0x100300060
  *   copy_constructor_1(87582,0x1000a95c0) malloc: *** error for
  *   object 0x100300060: pointer being freed was not allocated
  */

  NoCopyClass1 original1;
  // NOTE: following 2 lines will not compile
  // NoCopyClass1 copy1a(original1); // copy constructor
  // NoCopyClass1 copy1b = original1; // assigment operator

  NoCopyClass2 original2;
  // NOTE: following 2 lines will not compile
  // NoCopyClass2 copy2a(original2); // copy constructor
  // NoCopyClass2 copy2b = original2; // assigment operator

  // EXCLUSIVE COPY POLICY
  ExclusiveCopy source;
  ExclusiveCopy destination(source);  // copy constructor
  std::cout << "\n";

  // DEEP COPY POLICY
  DeepCopy source2(42);
  DeepCopy dest1(source2);    // copy constructor
  DeepCopy dest2 = dest1;     // how is this different than above?
  std::cout << "\n";

  // SHARED OWNERSHIP POLICY:
  SharedCopy source3(42);
  SharedCopy destination1(source3);   // copy constructor
  SharedCopy destination2(source3);   // copy constructor
  SharedCopy destination3(source3);   // copy constructor
  std::cout << "\n";

  return 0;
}