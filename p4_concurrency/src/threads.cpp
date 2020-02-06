#include <iostream>
#include <thread>
#include <vector>

using namespace std;


// NOTE: need to compile with pthreads:
// g++ -pthread ../src/threads.cpp -o threads.o

class Vehicle {
  private:
    int _id{0};
    string _name{""};
  public:
    Vehicle() {}
    Vehicle(int id) : _id(id) {}

    void operator()() {
      cout << "Vehicle #" << _id << " has been created" << endl;
    }

    void addID(int id) { _id = id; }
    void printID() { std::cout << "Vehicle ID: " << _id << std::endl; }

    void setName(string name) {_name = name; }
    void printName() {
      cout << "Vehicle #" << _id << " named " << _name << endl;
    }
};

void printName(std::string name, int waitTime)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
  std::cout << "Name (from Thread) = " << name << std::endl;
}

void printName2(std::string &name, int waitTime)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
  name += " (from Thread)";
  std::cout << name << std::endl;
}

void printHello()
{
  // perform work
  std::cout << "Hello from Worker thread #" << std::this_thread::get_id() << std::endl;
}

int main() {

  cout << "main() thread ID = " << this_thread::get_id() << endl;

  size_t nCores = thread::hardware_concurrency();
  cout << "This machine supports concurrency with " <<
    nCores << " cores available" << endl;

  /*NOTE: On Thread Declaration (below)
  * std::thread t(Vehicle());
  * 
  * 1. a variable definition for variable t of class std::thread,
  * initialized with an anonymous instance of class Vehicle, or
  * 
  * 2. a function declaration for a function t that returns an object of
  * type std::thread and has a single (unnamed) parameter that is a pointer
  * to function returning an object of type Vehicle.
  * 
  * Most programmers would presumable expect the first case to be true, but
  * the C++ standard requires it to be interpreted as the second - hence the
  * compiler error.
  * 
  * There are three ways of forcing the compiler to consider the line as the
  * first case, which would create the thread object we want:
  * 
  * std::thread t1( (Vehicle()) );             // Add extra pair of parantheses
  * std::thread t2 = std::thread( Vehicle() ); // Use copy initialization
  * std::thread t3{ Vehicle() };               // Use uniform initialization with braces
  */

  // create a thread (won't compile)
  // std::thread t(Vehicle());   // C++'s most vexing parse

  // alternative to line above (will compile)
  std::thread t = std::thread( Vehicle(1) ); // Use copy initialization

  // do something in main()
  cout << "Finshed work in main()" << endl;

  // wait for the thread to finish
  t.join();

  std::cout << "\n";

  /* VARIADIC TEMPLATES AND MEMBER FUNCTIONS:
  * he thread constructor may be called with a function and all its
  * arguments. That is possible because the thread constructor is a
  * "variadic template" that takes multiple arguments.
  *
  * There is one more difference between calling a function directly and
  * passing it to a thread: With the former, arguments may be passed by
  * value, by reference or by using move semantics - depending on the
  * signature of the function. When calling a function using a variadic
  * template, the arguments are by default either moved or copied -
  * depending on wether they are rvalues or lvalues.
  * 
  * In the following example, two threads are started, each with a different
  * string as a parameter. With t1, the string name1 is copied by value, 
  * which allows us to print name1 even after join has been called. The
  * second string name2 is passed to the thread function using move
  * semantics, which means that it is not available any more after join has
  * been called on t2.
  */

  // EXAMPLE 1:
  std::string name1 = "MyThread1";
  std::string name2 = "MyThread2";

  // starting threads using value-copy and move semantics 
  std::thread t1(printName, name1, 50);
  std::thread t2(printName, std::move(name2), 100);

  // wait for threads before returning
  t1.join();
  t2.join();

  // print name from main
  std::cout << "Name (from Main) = " << name1 << std::endl;
  std::cout << "Name (from Main) = " << name2 << std::endl;
  std::cout << "\n";

  /* EXAMPLE 2: std::ref() 
  * When passing the string variable name to the thread function, we need
  * to explicitly mark it as a reference, so the compiler will treat it as
  * such. This can be done by using the std::ref() function
  */
  std::string name("MyThread");

  // starting thread
  std::thread t3(printName2, std::ref(name), 50);

  // wait for thread before returning
  t3.join();

  // print name from main
  name += " (from Main)";
  std::cout << name << "\n\n";

  /* EXAMPLE 3: calling object member function within a thread
  * In the example above, the Vehicle object v1 is passed to the thread
  * function by value, thus a copy is made which does not affect the
  * "original“ living in the main thread. Changes to its member variable
  * _id will thus not show when printing calling printID() later in main.
  * The second Vehicle object v2 is instead passed by reference. Therefore,
  * changes to its _id variable will also be visible in the main thread.
  */

  // create thread
  Vehicle v1, v2;
  // pass Vehicle object to thread by value
  std::thread t4 = std::thread(&Vehicle::addID, v1, 1);
  // pass Vehicle object to thread by reference
  std::thread t5 = std::thread(&Vehicle::addID, &v2, 2);

  // wait for thread to finish
  t4.join();
  t5.join();

  // print Vehicle id
  v1.printID();
  v2.printID();
  cout << "\n";

  /* EXAMPLE 4: Shared Pointers & Threads
  * In the previous example, we have to ensure that the existence of v2
  * outlives the completion of the thread t2 - otherwise there will be an
  * attempt to access an invalidated memory address. An alternative is to
  * use a heap-allocated object and a reference-counted pointer such as
  * std::shared_ptr<Vehicle> to ensure that the object lives as long as it
  * takes the thread to finish its work.
  */

  // create thread
  auto v = std::make_shared<Vehicle>();
  std::thread t6 = std::thread(&Vehicle::addID, v, 1);
  std::thread t7 = std::thread(&Vehicle::setName, v, "GTR");
  
  // wait for thread to finish
  t6.join();
  t7.join();
  
  // print Vehicle id and name
  v->printID();
  v->printName();
  cout << "\n";

  /* RUNNING MULTIPLE THEADS AT ONCE:
  * A number of threads is created and added to a vector. The basic idea is
  * to loop over the vector at the end of the main function and call join
  * on all the thread objects inside the vector.
  * 
  * Note the use of push_back() causes a compiler error. The problem is that
  * by pushing the thread object into the vector, we attempt to make a copy
  * of it. However, thread objects do not have a copy constructor and thus
  * cannot be duplicated.
  * 
  * To solve the problem, we can use the function emplace_back() instead of
  * push_back(), which internally uses move semantics to move our thread
  * object into the vector without making a copy.
  */

  // create threads
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 5; ++i) {
    // copying thread objects causes a compile error
    /*
    std::thread t(printHello);
    threads.push_back(t); 
    */

    // moving thread objects will work
    threads.emplace_back(std::thread(printHello));
  }

  // do something in main()
  cout << "Hello from Main thread #" << std::this_thread::get_id() << endl;

  // call join on all thread objects using a range-based loop
  for (auto &t : threads) {
    t.join();
  }
  cout << "\n";

  /* OUTPUT:
  * Hello from Worker thread #Hello from Worker thread #140410393507584140410385114880
  * 
  * Hello from Worker thread #140206766860032
  * Hello from Worker thread #140206758467328
  * Hello from Main thread #140206801651520
  * Hello from Worker thread #140206750074624
  * 
  * This is a bit unexpected, and demonstrates 2 important properties of
  * concurrent programs:
  * 
  * 1. The order in which threads are executed is non-deterministic. Every
  * time a program is executed, there is a chance for a completely different 
  * of execution.
  * 
  * 2. Threads may get preempted in the middle of execution and another
  * thread may be selected to run (as in the first line)
  */

  /* FIXING THE EXAMPLE ABOVE:
  * if i is passed to the thread lambda as a reference (&i), this causes a
  * concurrency bug in the form of each thread attempting to access a 
  * shared memory location.
  */

  // create threads
  std::vector<std::thread> threads2;
  for (size_t i = 0; i < 10; ++i) {
    // create new thread from a Lambda
    threads2.emplace_back([i]() {
      // wait for certain amount of time
      std::this_thread::sleep_for(std::chrono::milliseconds(10 * i));

      // perform work
      std::cout << "Hello from Worker thread #" << i << std::endl;
    });
  }

  // do something in main()
  std::cout << "Hello from Main thread" << std::endl;

  // call join on all thread objects using a range-based loop
  for (auto &t : threads2)
    t.join();

  return 0; 
}