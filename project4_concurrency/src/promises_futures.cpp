#include <iostream>
#include <thread>
#include <future>
#include <cmath>

using Duration = std::chrono::milliseconds;

void printMessage(std::string message)
{
  std::this_thread::sleep_for(Duration(10)); // simulate work
  std::cout << "Thread " << std::this_thread::get_id() << ": " << message <<
    std::endl;
}

void modifyMessage(std::promise<std::string> &&prms, std::string message)
{
  // 3. modify message via std::promise::set_value
  std::this_thread::sleep_for(Duration(2000)); // simulate work
  std::string modifiedMessage = message + " has been modified"; 
  prms.set_value(modifiedMessage);
}

void computeSqrt(std::promise<double> &&prms, double input)
{
  std::this_thread::sleep_for(Duration(2000)); // simulate work
  double output = sqrt(input);
  prms.set_value(output);
}

void divideByNumber(std::promise<double> &&prms, double num, double denom)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
  try
  {
    if (denom == 0)
      throw std::runtime_error("Exception from thread: Division by zero!");
    else
      prms.set_value(num / denom);
  }
  catch (...)
  {
    prms.set_exception(std::current_exception());
  }
}

int main() {

  /* SO FAR: Two methods for passing data to a thread
  *
  * 1. We can pass arguments to the thread function using variadic templates
  * 2. We can use a Lambda to capture arguments by value or by reference
  * 
  * In both cases, information flows from the parent thread (main) to the worker
  * threads (t1 and t2). In this section, we will look at a way to pass data in
  * the opposite direction - that is from the worker threads back to the parent
  * thread.
  * 
  * In order to achieve this, the threads need to adhere to a strict
  * synchronization protocol - this mechanism acts as a single-use channel
  * between the threads. The sending end of the channel is called "promise"
  * while the receiving end is called "future".
  * 
  * The following two examples illustrate the two methods descitbed above. The
  * output shown below is possible due to the randomness of thread execution:
  * 
  * Example 1: Starting threads with Variadic Templates and Lambdas
  * Thread Thread 140097871222528: 140097879615232My Message: My Message 
  */
  std::cout << "Example 1: Starting threads with Variadic Templates and Lambdas"
    << std::endl;

  // define message
  std::string message = "My Message";

  // start thread using variadic templates
  std::thread t1(printMessage, message);

  // start thread using a Lambda
  std::thread t2([message] {
      std::this_thread::sleep_for(Duration(10)); // simulate work
      std::cout << "Thread " << std::this_thread::get_id() << ": " << message <<
        std::endl;
  });

  // thread barrier
  t1.join();
  t2.join();

  /* PROMISE AND FUTURE:

  * In the C++ standard, the class template std::promise provides a convenient
  * way to store a value or an exception that will acquired asynchronously at a
  * later time via a std::future object. Each std::promise object is meant to be
  * used only a single time.
  * 
  * In the following example, we want to declare a promise which allows for
  * transmitting a string between two threads and modifying it in the process.
  * 
  * 1. After defining a message, we have to create a suitable promise that can
  * take a string object. To obtain the corresponding future, we need to call
  * the method get_future() on the promise.
  * 
  * 2. We can now create a thread that takes a function and we will pass it the
  * promise as an argument as well as the message to be modified. Promises
  * cannot be copied, because the promise-future concept is a two-point
  * communication channel for one-time use. Therefore, we must pass the promise
  * to the thread function using std::move.
  * 
  * 3. The thread function takes the promise as an rvalue reference in
  * accordance with move semantics. After waiting for several seconds, the
  * message is modified and the method set_value() is called on the promise.
  * 
  * std::future::get will block until data is available - which happens as soon
  * as set_value has been called on the promise (from the thread). If the result
  * is movable (which is the case for std::string), it will be moved - otherwise
  * it will be copied instead.
  * 
  * NOTE: EACH STD::PROMISE OBJECT IS ONLY MEANT TO BE USED A SINGLE TIME
  */
 std::cout << "\nExample 2: Promises and Futures" << std::endl;

  std::string messageToThread = "My Message";

  // 1. create promise and future
  std::promise<std::string> prms;
  std::future<std::string> ftr = prms.get_future();

  // 2. start thread and pass promise as argument
  std::thread t(modifyMessage, std::move(prms), messageToThread);

  // print original message to console
  std::cout << "Original message from main(): " << messageToThread << std::endl;

  // 4. retrieve modified message via future and print to console
  std::string messageFromThread = ftr.get();
  std::cout << "Modified message from worker thread(): " << messageFromThread
    << std::endl;

  // thread barrier
  t.join();

  /* GET() vs. WAIT():
  * 
  * There are some situations where we may want to separate the waiting for the
  * data from the actual retrieving. Futures allow us to do that using the
  * wait() function. This method will block until the future is ready. Once it
  * returns, it is guaranteed that data is available and we can use get() to
  * retrieve it without delay.
  * 
  * In addition to wait, the C++ standard also offers the method wait_for, which
  * takes a time duration as an input and also waits for a result to become
  * available. The method wait_for() will block either until the specified
  * timeout duration has elapsed or the result becomes available - whichever
  * comes first. The return value identifies the state of the result.
  */
  std::cout << "\nExample 3: get(), wait() and wait_for()" << std::endl;

  // define input data
  double inputData = 42.0;

  // create promise and future
  std::promise<double> prms2;
  std::future<double> ftr2 = prms2.get_future();

  // start thread and pass promise as argument
  std::thread t3(computeSqrt, std::move(prms2), inputData);

  // wait for result to become available
  auto status = ftr2.wait_for(Duration(2001));
  if (status == std::future_status::ready) // result is ready
  {
    std::cout << "Result = " << ftr2.get() << std::endl;
  }

  //  timeout has expired or function has not yet been started
  else if (status == std::future_status::timeout || status == std::future_status::deferred)
  {
    std::cout << "Result unavailable" << std::endl;
  }  

  // thread barrier
  t3.join();

  /* PASSING EXCPETIONS FROM WORKER THREAD TO PARENT:
  *
  * The future-promise communication channel may also be used for passing
  * exceptions. To do this, the worker thread simply sets an exception rather
  * than a value in the promise. In the parent thread, the exception is then
  * re-thrown once get() is called on the future.
  */
  std:: cout << "Example 4: Passing exceptions" << std::endl;

  // create promise and future
  std::promise<double> prms3;
  std::future<double> ftr3 = prms3.get_future();

  // start thread and pass promise as argument
  double num = 42.0, denom = 0.0;
  std::thread t4(divideByNumber, std::move(prms3), num, denom);

  // retrieve result within try-catch-block
  try
  {
    double result = ftr3.get();
    std::cout << "Result = " << result << std::endl;
  }
  catch (const std::runtime_error& err)
  {
    std::cout << err.what() << std::endl;
  }

  // thread barrier
  t4.join();

  return 0;
}