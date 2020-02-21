#include <iostream>
#include <thread>
#include <future>
#include <memory>

class Vehicle {
  public:
    //default constructor
    Vehicle() : _id(0)
    {
      std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle(int id) : _id(id)
    {
      std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    // setter and getter
    void setID(int id)
    {
      std::cout << "Setting Vehicle ID to " << id << " for Vehicle " << this << std::endl;
      _id = id;
    }
    int getID(){ return _id; }

  private:
    int _id;
};

class Vehicle2 {
  public:
    //default constructor
    Vehicle2() : _id(0), _name(new std::string("Default Name"))
    {
      std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle2(int id, std::string name) : _id(id), _name(new std::string(name))
    {
      std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    // copy constructor 
    Vehicle2(Vehicle2 const &src)
    {
      // QUIZ: Student code STARTS here
      _id = src._id;
      if (src._name != nullptr)
      {
        _name = new std::string;  // NOTE: "new" return a pointer of specified type
        *_name = *src._name;
      }
      // QUIZ: Student code ENDS here
      std::cout << "Vehicle #" << _id << " copy constructor called" << std::endl;
    };

    // move constructor
    Vehicle2(Vehicle2 &&src)
    {
      // my deep-copy implementation
      _id = src._id;
      _name = new std::string;  // NOTE: "new" return a pointer of specified type
      *_name = *src._name;

      src._id = 0;
      src._name = nullptr;

      // deep-copy implementation from lesson (is there a difference?)
      // _id = src.getID();
      // _name = new std::string{src.getName()};

      // src.setID(0);
      // src.setName("Default");

      // QUIZ: Student code ENDS here
      std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
    }

    // setter and getter
    void setID(int id) { _id = id; }
    int getID() { return _id; }
    void setName(std::string name)
    {
      std::cout << "Setting Vehicle name to " << name << " for Vehicle " << this << std::endl;
      *_name = name;
    }
    std::string getName() { return *_name; }

  private:
    int _id;
    std::string *_name;   // addition of pointer variable to Vehicle2 class

    /* NOTE: Since a shallow-copy is performed by default (when the copy constr.)
    * is not overridden, the resulting copied Vehicle2 object will have a pointer
    * called _name that points to the SAME location in memory as the Vehicle2
    * object that it was copied from.
    * 
    * This can lead to a data-race when attempting to modify (or access) the string
    * value pointed to by _name. This can be solved by overriding the Vehicle2 copy
    * constructor (as has been done) to implement a deep-copy.
    */
};

class Vehicle3 {
  public:
    //default constructor
    Vehicle3() : _id(0), _name(new std::string("Default Name"))
    {
      std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle3(int id, std::string name) : _id(id), _name(new std::string(name))
    {
      std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    // Q: Why is the copy constructor not implemented?
    // A: Is is because Vehicle3 has a member variable that is a unique_ptr and
    // it's not valid to copy a unique_ptr?

    // NOTE: Code is not compiling because of bug below. I havne't been able to
    // figure out why. See test.cpp for an example of a version of the Vehicle3
    // class that properly compiles... they seem identical to me??

    // move constructor
    Vehicle3(Vehicle3 &&src) : _name(std::move(src,_name))
    {
      _id = src._id;
      // _name = std::move(src,_name);

      std::cout << "Vehicle #" << _id << " move constructor called" << std::endl;
    }

    // setter and getter
    void setID(int id) { _id = id; }
    int getID() { return _id; }
    void setName(std::string name)
    {
      std::cout << "Setting Vehicle name to " << name << " for Vehicle " << this << std::endl;
      *_name = name;
    }
    std::string getName() { return *_name; }

  private:
    int _id;
    std::unique_ptr<std::string> _name;
};


int main()
{
  // create instances of class Vehicle
  Vehicle v0; // default constructor
  Vehicle v1(1); // initializing constructor

  std::cout << "Vehicle #" << v0.getID() << " at " << &v0 << std::endl;
  std::cout << "Vehicle #" << v1.getID() << " at " << &v1 << std::endl;

  // an instance of Vehicle is created and passed to a thread by value, thus
  // creating a copy of it.
  // NOTE: Vehicle class implements a shallow-copy by default
  std::future<void> ftr = std::async([](Vehicle v) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
      v.setID(2);
  }, v0);

  v0.setID(3);

  ftr.wait();
  std::cout << "Vehicle #" << v0.getID() << " at " << &v0 << "\n\n";

  // create instances of class Vehicle2
  Vehicle2 v2;    // default constructor
  Vehicle2 v3(3, "Vehicle 3"); // initializing constructor

  std::cout << "Vehicle #" << v2.getID() << " at " << &v2 << std::endl;
  std::cout << "Vehicle #" << v3.getID() << " at " << &v3 << std::endl;

  // launch a thread that modifies the Vehicle name
  // NOTE: Vehicle2 class implements a deep-copy
  std::future<void> ftr2 = std::async([](Vehicle2 v) {
      // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
      v.setName("Vehicle 4");
  },v2);

  v2.setName("Vehicle 5");

  ftr2.wait();
  std::cout << v2.getName() << " at " << &v2 << "\n\n";

  /* PREVENTING DATA RACES WITH MOVE SEMANTICS:
  *
  * "One of the keys to thread safety is to use move semantics in conjection with
  * uniqueness. It is the responsibilty of the programmer to ensure that the
  * pointers to objects that are moved between threads are always unique."
  */

  // create instances of class Vehicle
  Vehicle3 v4;    // default constructor
  Vehicle3 v5(1, "Vehicle 5"); // initializing constructor

  // launch a thread that modifies the Vehicle name
  std::future<void> ftr3 = std::async([](Vehicle3 v) {
      v.setName("Vehicle 2");
  },std::move(v4));

  ftr3.wait();

  // std::cout << v4.getName() << std::endl; // this will now cause an exception

  return 0;
}