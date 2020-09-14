# Lesson 1: Introduction and Running Threads

## Lesson 1.3: Running a Single Thread

The concurrency support in C++ makes it possible for a program to execute multiple threads in parallel. Concurrency was first introduced into the standard with C++11. Since then, new concurrency features have been added with each new standard update, such as in C++14 and C++17. Before C++11, concurrent behavior had to be implemented using native concurrency support from the OS, using POSIX Threads, or third-party libraries such as BOOST. The standardization of concurrency in C++ now makes it possible to develop cross-platform concurrent programs, which is as significant improvement that saves time and reduces error proneness. Concurrency in C++ is provided by the thread support library, which can be accessed by including the header.

### Randomness of Events
One very important trait of concurrent programs is their non-deterministic behavior. It can not be predicted which thread the scheduler will execute at which point in time. In the code on the right, This element of randomness is an important characteristic of concurrent programs and we have to take measures to deal with it in a controlled way that prevent unwanted behavior or even program crashes.

### Using `join()` as a Barrier
In later sections of this course, we will make extended use of the join() function to carefully control the flow of execution in our programs and to ensure that results of thread functions are available and complete where we need them to be.

### Detach
Let us now take a look at what happens if we don’t join a thread before its destructor is called. When we comment out join in the example above and then run the program again, it aborts with an error. The reason why this is done is that the designers of the C++ standard wanted to make debugging a multi-threaded program easier: Having the program crash forces the programer to remember joining the threads that are created in a proper way. Such a hard error is usually much easier to detect than soft errors that do not show themselves so obviously.

There are some situations however, where it might make sense to not wait for a thread to finish its work. This can be achieved by "detaching" the thread, by which the internal state variable "joinable" is set to "false". This works by calling the detach() method on the thread. The destructor of a detached thread does nothing: It neither blocks nor does it terminate the thread. In the following example, detach is called on the thread object, which causes the main thread to immediately continue until it reaches the end of the program code and returns. Note that a detached thread can not be joined ever again.

## Lesson 1.4: Starting a Thread with a Function Object
In C++, __callable objects__ are objects that can appear as the left-hand operand of the call operator. These can be pointers to functions, objects of a class that defines an overloaded function call operator and __lambdas__ (an anonymous inline function), with which function objects can be created in a very simple way. __In the context of concurrency, we can use callable objects to attach a function to a thread.__

In the last section, we constructed a thread object by passing a function to it without any arguments. If we were limited to this approach, the only way to make data available from within the thread function would be to use global variables - which is definitely not recommendable and also incredibly messy.

In this section, we will therefore look at several ways of passing data to a thread function.

### 1.4.1 Starting Threads with Function Objects
The `std::thread` constructor can also be called with instances of classes that implement the function-call operator.

__NOTE__: See `threads.cpp` for notes on _"C++'s most vexing parse"_ realted to instantiating a thread with a class instance as its target.

### 1.4.2 Lambdas
Another very useful way of starting a thread and passing information to it is by using a lambda expression ("Lambda" for short). With a Lambda you can easily create simple function objects. C ++ Lambdas have the properties of being unnamed and capturing variables from the surrounding context, but lack the ability to execute and return functions.

A Lambda is often used as an argument for functions that can take a callable object. This can be easier than creating a named function that is used only when passed as an argument. In such cases, Lambdas are generally preferred because they allow the function objects to be defined inline. If Lambdas were not available, we would have to define an extra function somewhere else in our source file - which would work but at the expense of the clarity of the source code.

A Lambda is a function object (a "functor"), so it has a type and can be stored and passed around. Its result object is called a "closure", which can be called using the operator `()` as we will see shortly.

A lambda formally consists of three parts: a __capture list__ `[]` , a __parameter list__ `()` and a __main part__ `{}`, which contains the code to be executed when the Lambda is called. __Note that in principal all parts could be empty__.

__The capture list__ `[]`: By default, variables outside of the enclosing {} around the main part of the Lambda can not be accessed. By adding a variable to the capture list however, it becomes __available within the Lambda either as a copy or as a reference__. The captured variables become a part of the Lambda.

By default, variables in the capture block can not be modified within the Lambda. Using the keyword `mutable` allows us to modify the parameters captured by copy, and to call their non-const member functions within the body of the Lambda. The following code examples show several ways of making the external variable `id` accessible within a Lambda.

```
#include <iostream>

int main()
{
    // create lambdas
    int id = 0; // Define an integer variable

    // Error: 'id' cannot be accessed
    //auto f0 = []() { std::cout << "ID = " << id << std::endl; };

    id++;
    // OK, 'id' is captured by value
    auto f1 = [id]() { std::cout << "ID = " << id << std::endl; };

    id++;
    // OK, 'id' is captured by reference
    auto f2 = [&id]() { std::cout << "ID = " << id << std::endl; }; 

    // Error, 'id' may not be modified
    //auto f3 = [id]() { std::cout << "ID = " << ++id << std::endl; }; 

    // OK, 'id' may be modified
    auto f4 = [id]() mutable { std::cout << "ID = " << ++id << std::endl; };

    // OK, ID is passed as a parameter
    auto f5 = [](const int id) { std::cout << "ID = " << id << std::endl; };

    // execute lambdas
    f1();
    f2();
    f4();
    f5(id)

    return 0;
}
```

We can use (a copy of) the closure (i.e. f0, f1, …) to execute the code within the Lambda at a position in our program different to the line where the function object was created.

__The parameter list__ `()` : The way parameters are passed to a Lambda is basically identical to calling a regular function. If the Lambda takes no arguments, these parentheses can be omitted (except when `mutable` is used).

__NOTE__: Even though we have been using Lambdas in the above example in various ways, it is important to note that a Lambda does not exist at runtime. The runtime effect of a Lambda is the generation of an object, which is known as closure. The difference between a Lambda and the corresponding closure is similar to the distinction between a class and an instance of the class. A class exists only in the source code while the objects created from it exist at runtime.

### 1.4.3 Starting Threads with Lambdas
A __lambda__ is, as we’ve seen, just an object and, like other objects it may be copied, passed as a parameter, stored in a container, etc. The lambda object has its own scope and lifetime which may, in some circumstances, be different to those objects it has _captured_. Programmers need to take special care when capturing local objects by reference because __a lambda’s lifetime may exceed the lifetime of its capture list: It must be ensured that the object to which the reference points is still in scope when the lambda is called__. This is especially important in multi-threading programs.

## 1.5 Starting a Thread with Variadic Templates

Multiple (4) ways of launching a thread:
- __Function Object__ (prev. covered)
- __Lambda Functions__ (prev. covered)
- __Variadic Templates__ (this sub-lesson)
- __Member Functions__ (this sub-lesson)




