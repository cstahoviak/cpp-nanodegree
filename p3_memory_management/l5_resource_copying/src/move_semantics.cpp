#include <iostream>
#include <stdlib.h>

using namespace std;

/* WHEN ARE MOVE SEMATICS USED:

* 1. Copying "heavy-weight" objects around a program without move
  semantics can cause serious performance issues. The idea in this
  scenario is to create the object a single time and then "simply"
  move it around using rvalue references and move semantics.

* 2. When ownership needs to be transferred (such as with unique
pointers, as we will soon see). The primary difference to shared
references is that with move semantics we are not sharing anything
but instead we are ensuring through a smart policy that only a single
object at a time has access to and thus owns the resource.
*/ 

/* R-VALUE REFERENCES:
* The important message of the function argument of myFunction to the
  programmer is : The object that binds to the rvalue reference &&val
  is yours, it is not needed anymore within the scope of the caller
  (which is main). Intetesting for 2 reasons:

* 1. Passing values like this improves performance as no temporary
  copy needs to be made anymore.

* 2. Ownership changes, since the object the reference binds to has
  been abandoned by the caller and now binds to a handle which is
  available only to the receiver. This could not have been achieved
  with lvalue references as any change to the object that binds to the
  lvalue reference would also be visible on the caller side.

* There is one more important aspect we need to consider: **rvalue
  references are themselves lvalues.** Even though the object it
  refers to (the number 42) may be disposable in the context it has
  been created (the main function), it is not disposable in the context
  of the reference . So within the scope of myFunction, val is an
  lvalue as it gives access to the memory location where the number
  42 is stored.
*/
void myFunction(int &&val) {
  std::cout << "val = " << val << std::endl;
}

/* RULE OF FIVE:
* 1. DESTRUCTOR: responsible for freeing the resource once the object
it belongs to goes out of scope.

* 2. ASSIGNMENT OPERATOR: the default assignment operation performs a
member-wise "shallow copy", which does not copy the content behind the
resource handle. If a "deep copy" is needed, it has be implemented by
the programmer.

* 3. COPY CONSTRUCTOR: as with the assignment operator, the default
copy constructor performs a "shallow copy" of the data members. If
something else is needed, the programmer has to implement it accordingly.

* 4. MOVE CONSTRUCTOR: because copying objects can be an expensive
operation which involves creating, copying and destroying temporary
objects, rvalue references are used to bind to an rvalue. Using this
mechanism, the move constructor transfers the ownership of a resource
from a (temporary) rvalue object to a permanent lvalue object.

* 5. MOVE ASSIGNMENT OPERATOR: with this operator, ownership of a
resource can be transferred from one object to another. The internal
behavior is very similar to the move constructor.
*/
class MyMovableClass {
  private:
    int _size;    // instantiated on the "stack"
    int *_data;   // instantiated on the "heap"

  public:
    MyMovableClass(size_t size) { // constructor
      _size = size;
      // constructor allocates memory on the heap via "new"
      _data = new int[_size];
      cout << "CREATING instance of MyMovableClass at " << this <<
        " allocated with size = " << _size*sizeof(int)  << " bytes"
        << endl;
    }

    ~MyMovableClass() { // 1 : destructor
      cout << "DELETING instance of MyMovableClass at " << this << endl;
      delete[] _data;   // deallocate heap memory
    }

    /* 2: COPY CONSTRUCTOR
    * the copy constructor takes an lvalue reference to the source
    * instance, allocates a block of memory of the same size as the
    * source and then copies the data into its members (deep copy).
    */
    MyMovableClass(const MyMovableClass &source) {
      _size = source._size;
      // the following 2 lines perform a "deep copy" of the source obj
      _data = new int[_size];   
      *_data = *source._data;
      cout << "COPYING content of instance " << &source <<
        " to instance " << this << endl;
    }

    /* 3: COPY ASSIGMENT OPERATOR
    * Identical to the copy structor aside from the if conditional,
      and the fact that the dereferened this pointer is returned.

      Both copy constructor and copy assignment operator take a const
      reference to the source object as an argument, by which they
      promise that they won’t (and can’t) modify the content of source.
    */
    MyMovableClass &operator=(const MyMovableClass &source) {
      cout << "ASSIGNING content of instance " << &source <<
        " to instance " << this << endl;
      // protects against self-assignment - standard for user-defined
      // assignment operator code
      if (this == &source)
          return *this;
      
      delete[] _data;   // why is the call to delete[] necessary?

      /* Explanation (from Sasha):
      * " Since both (move AND copy assignment operators) are
      * assignment operators and not constructors, it is (rightly)
      * assumed that _data points to some allocated object. Since we
      * are about to assign _data to point to something else, we need
      * to delete (release memory) whatever _data is currently pointing
      * to, or else we are causing a memory leak."
      */

      _data = new int[source._size];
      *_data = *source._data;
      _size = source._size;
      return *this;
    }

    /* 4: MOVE CONSTRUCTOR
    * Just like the copy constructor, the move constructor builds an
    * instance of a class using a source instance. The key difference
    * between the two is that with the move constructor, the source
    * instance will no longer be usable afterwards.
    * 
    * The move constructor takes as its input an rvalue reference to a
    * source object of the same class. In doing so, we are able to use
    * the object within the scope of the move constructor. As can be
    * seen, the implementation copies the data handle from source to
    * target and immediately invalidates source after copying is
    * complete.
    */
    MyMovableClass(MyMovableClass &&source) // 4 : move constructor
    {
      std::cout << "MOVING (c’tor) instance " << &source << " to instance " << this << std::endl;
      _data = source._data;
      _size = source._size;
      source._data = nullptr;
      source._size = 0;
    }

    /* 5: MOVE ASSIGNMENT OPERATOR
    * As with the move constructor, the data handle is copied from
    * source to target which is coming in as an rvalue reference again.
    * Afterwards, the data members of source are invalidated. The rest
    * of the code is identical with the copy constructor we have already
    * implemented.
    */
    MyMovableClass &operator=(MyMovableClass &&source) {
      std::cout << "MOVING (assign) instance " << &source << " to instance " << this << std::endl;
      if (this == &source)
          return *this;

      delete[] _data;

      _data = source._data;
      _size = source._size;

      source._data = nullptr;
      source._size = 0;

      return *this;
    }

    // assesor function for _data
    void Data() {
      cout << "_size = " << _size << ", _data = " << *_data <<
        " for instance " << this << endl;
      return;
    }
}; // MyMoveableClass

void useObject(MyMovableClass obj) {
  cout << "using object " << &obj << endl;
}

int main() {
  cout << "EXAMPLE 1: R-Value References\n";
  myFunction(42);

  int i = 23;
  // error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
  // myFunction(i);

  /* NOTE: The function std::move converts an lvalue into an rvalue
  (actually, to be exact, into an xvalue), which makes it possible to
  use the lvalue as an argument for the function.

  In doing this, we state that in the scope of main we will not use i
  anymore, which now exists only in the scope of myFunction.
  */
  myFunction(std::move(i));

  cout << "EXAMPLE 2: Copy Assigment operator\n";
  MyMovableClass obj1(10);    // regular constructor
  obj1.Data();
  MyMovableClass obj2(obj1);  // copy constructor
  obj2.Data();
  obj2 = obj2;                // copy assignment operator
  obj2.Data();
  cout << "\n";

  cout << "EXAMPLE 3: Copy Assigment operator\n";
  MyMovableClass obj3(100), obj4(200);  // constructor
  MyMovableClass obj5(obj3);            // copy constructor
  // even though the creation of obj4 looks like an assignment, the
  // compiler calls the copy constructor
  MyMovableClass obj6 = obj3;           // copy constructor
  obj6 = obj4;                          // copy assignment operator
  cout << "\n";

  // NOTE: Up till this point, no move operation has been performed yet
  // as all of the above calls were involving lvalues.

  /* In this version, we also have an instance of MyMovableClass, obj7.
  *  Then, a second instance of MyMovableClass is created as an rvalue,
  *  which is assigned to obj7. Finally, we have a second lvalue obj8,
  *  which is created by assigning it an rvalue object. Let us take a
  *  look at the output of the program:
  * 
  * NOTE: The advantage from a performance perspective in this case is
  * that no deep-copy of the rvalue object (MyMovableClass(200)) needs
  * to be made, we are simply redirecting the internal resource handle
  * thus making an efficient shallow copy.
  */
  cout << "EXAMPLE 4: Move Constructor / Assigment\n";
  MyMovableClass obj7(100);                   // constructor
  obj7 = MyMovableClass(200);                 // move assignment operator
  MyMovableClass obj8 = MyMovableClass(300);  // move constructor
  cout << "\n";

  /* MOVING L-VALUES:
  * 1. First, we are creating an instance of MyMovableClass, obj9, by
  * calling the constructor of the class (1).

  * 2. Then, we are passing obj9 by-value to a function useObject(),
  * which causes a temporary object obj to be instantiated, which is a
  * copy of obj9 (2) and is deleted immediately after the function
  * scope is left (3).

  * 3. Then, the function is called with a temporary instance of
  * MyMovableClass as its argument, which creates a temporary instance
  * of MyMovableClass as an rvalue (4). But instead of making a copy of
  * it as before, the move constructor is used (5) to transfer
  * ownership of that temporary object to the function scope, which
  * saves us one expensive deep-copy.
  * 
  * std::move accepts an lvalue argument and returns it as an rvalue
  * without triggering copy construction. So by passing an object to
  * std::move we can force the compiler to use move semantics, either
  * in the form of move constructor or the move assignment operator.
  */
  cout << "EXAMPLE 5: Pass-by-Value Temporary Objects\n";
  MyMovableClass obj9(100);     // (1) constructor
  useObject(obj9);              // (2) copy constuctor     
  useObject(std::move(obj9));   // (3) move consructor
  cout << "\n";

  return 0;
}