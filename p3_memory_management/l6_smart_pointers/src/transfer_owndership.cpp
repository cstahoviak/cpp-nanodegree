#include <iostream>
#include <memory>

/* UNIQUE POINTER: transfer ownership from caller to function
* The basic idea of a unique_ptr is that there exists only a single
  instance of it. This is why it can’t be copied to a local function
  but needs to be moved instead with the function std::move. The code
  example below illustrates the principle of transferring the object
  managed by the unique pointer uniquePtr into a function f.
*/

class MyClass {
  private:
    int _member;

  public:
    MyClass(int val) : _member{val} {}
    ~MyClass() {
      std::cout << this << " has been destroyed\n";
    }
    void printVal() { std::cout << ", managed object " << this << " with val = " << _member << std::endl; }
};

void f(std::unique_ptr<MyClass> ptr) {
  std::cout << "unique_ptr " << &ptr;
  ptr->printVal();
}

/* SHARED POINTER: share ownership between caller and function
* When passing a shared pointer by value, move semantics are not
  needed. As with unique pointers, there is an underlying rule for
  transferring the ownership of a shared pointer to a function:

  R.34: Take a shared_ptr parameter to express that a function is part
  owner.
*/
void g(std::shared_ptr<MyClass> ptr)
{
  std::cout << "shared_ptr (ref_cnt= " << ptr.use_count() << ") " << &ptr;
  ptr->printVal();
}

int main() {
  // UNIQUE POINTER EXAMPLE
  std::cout << "UNIQUE POINTERS:\n";
  std::unique_ptr<MyClass> uniquePtr = std::make_unique<MyClass>(23);
  std::cout << "unique_ptr " << &uniquePtr;
  uniquePtr->printVal();

  f(std::move(uniquePtr));

  /* NOTE: After the move is complete, we can still use the variable
  *  uniquePtr in main but it now is only an empty shell which does
  * not contain an object to manage.
  */
  if (uniquePtr) {
    uniquePtr->printVal();
  }
  else {
    std::cout << "unique_ptr " << &uniquePtr << " is invalid\n";
  }

  /* SHARED POINTER EXAMPLE
  * The address of the managed object does not change. When passed to
  * g(), the reference count changes to 2. After the function returns
  * and the local shared_ptr is destroyed, the reference count changes
  * back to 1.
  */
  std::cout << "\nSHARED POINTERS:\n";
  std::shared_ptr<MyClass> sharedPtr = std::make_shared<MyClass>(23);
  std::cout << "shared_ptr (ref_cnt= " << sharedPtr.use_count() << ") " << &sharedPtr;
  sharedPtr->printVal();

  g(sharedPtr);

  if (sharedPtr) {
    std::cout << "shared_ptr (ref_cnt= " << sharedPtr.use_count() << ") " << &sharedPtr;
    sharedPtr->printVal();
  }
  else {
    std::cout << "unique_ptr " << &uniquePtr << " is invalid\n";
  }

  return 0;
}