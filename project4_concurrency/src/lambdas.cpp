#include <iostream>
#include <thread>

using namespace std;


// NOTE: need to compile with pthreads:
// g++ -pthread ../src/lambdas.cpp -o lambdas.o

/* LAMBDA FUNCTIONS:
* Another very useful way of starting a thread and passing information to
* it is by using a lambda expression ("Lambda" for short). With a Lambda
* you can easily create simple function objects.
* 
* A lambda formally consists of three parts: a capture list [] , a
* parameter list () and a main part {}, which contains the code to be
* executed when the Lambda is called.
* 
* it is important to note that a Lambda does not exist at runtime. The
* runtime effect of a Lambda is the generation of an object, which is
* known as "closure". The difference between a Lambda and the
* corresponding closure is similar to the distinction between a class and
* an instance of that class. A class exists only in the source code while
* the objects created from it exist at runtime.
* 
* We can use (a copy of) the closure (i.e. f0, f1, …) to execute the code
* within the Lambda at a position in our program different to the line
* where the function object was created.
*/

int main() {

  int id = 0; // Define an integer variable

  // Error: 'id' cannot be accessed
  // auto f0 = []() { std::cout << "ID = " << id << std::endl; };

  // OK, 'id' is captured by value (is read-only in the scope of f1)
  auto f1 = [id]() { std::cout << "f1()\t ID = " << id << std::endl; };

  // OK, 'id' is captured by reference - modifies variable in main()
  auto f2 = [&id]() { std::cout << "f2()\t ID = " << ++id << std::endl; };

  // Error, 'id' may not be modified
  // auto f3 = [id]() { std::cout << "ID = " << ++id << std::endl; }; 

  // OK, 'id' may be modified - does NOT modify variable in main()
  auto f4 = [id]() mutable { std::cout << "f4()\t ID = " << ++id << std::endl; };

  // execute lambdas, i.e. call the lambda "closure"
  cout << "main()\t ID = " << id << endl;
  f1();
  cout << "main()\t ID = " << id << endl;
  f2();
  cout << "main()\t ID = " << id << endl;
  f4();
  cout << "main()\t ID = " << id << endl;

  /* OUTPUT:
  * main()	 ID = 0
  * f1()	   ID = 0
  * main()	 ID = 0
  * f2()	   ID = 1
  * main()	 ID = 1
  * f4()	   ID = 1
  * main()	 ID = 1
  */

  // ID passed as parameter
  auto f5 = [](const int id) { std::cout << "\nf5()\t ID = " << id << std::endl; };
  f5(id);

  /* NOTE: On the capture list []
  * A Lambda is just an object and, like other objects it may be copied,
  * passed as a parameter, stored in a container, etc. The Lambda object has
  * its own scope and lifetime which may, in some circumstances, be
  * different to those objects it has ‘captured’. Programers need to take
  * special care when capturing local objects by reference because a
  * Lambda’s lifetime may exceed the lifetime of its capture list: It must
  * be ensured that the object to which the reference points is still in
  * scope when the Lambda is called. This is especially important in
  * multi-threading programs.
  */
  std::cout << "\n";
  id = 0;   // reset id

  // starting a first thread (by reference)
  auto f0 = [&id]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "a) ID in Thread (call-by-reference) = " << id << std::endl;
  };
  std::thread t1(f0);

  // starting a second thread (by value)
  std::thread t2([id]() mutable {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      std::cout << "b) ID in Thread (call-by-value) = " << id << std::endl;
  });

  // increment and print id in main
  ++id;
  std::cout << "c) ID in Main (call-by-value) = " << id << std::endl;

  /* OUTPUT:
  * c) ID in Main (call-by-value) = 1
  * b) ID in Thread (call-by-value) = 0
  * a) ID in Thread (call-by-reference) = 1
  */

  // wait for threads before returning
  t1.join();
  t2.join();

  return 0; 
}