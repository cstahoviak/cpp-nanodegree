# Lesson 2: Passing Data Between Threads
By this point, we should be able to do the following:

1. Start one or more threads.
2. Block execution of the main thread with a barrier (`join`).
3. Pass data to a thread at the time of creation.
4. Identify a concurreny bug where more than one thread attempts to access a 
shared memory location.

This lesson will cover means by which to pass data to a thread after it's
been created. This includes the following:

1. __Promises and Futures__: A _private_ communication channel between two
threads that can be used only a single time. Can be used to pass both data and
exceptions from the worker thread to the parent thread that created the worker
thread.
2. __Difference between Threads and Tasks__: __Tasks__ are a high-level concept
in which the CPU can choose to run tasks _in parallel_ or _synchronously_. Tasks
make it very easy to establish a _promise-future_ link in your code - means
writing less code than with standard threads.
3. Using __move semantics__ to pass data to a thread by value. Move semantics are
used everywhere in concurrent programming, and can make your code much more
efficient - __do not shy away from move semantics especially when it comes to
rvalue references!__
4. __Data Races__: )ne of the primary sources of error in concurrent programming.


## 2.2: Promises and Futures
Two major drawbacks to __promises__ and __futures__:

1. Data can only be passed in one direction - from the worker thread back to the
parent thread.
2. Promises and Futures are meant for one-time use only. In a later section, we
will discuss an alternative to this single-use communication method.

## 2.3: Threads vs. Tasks (Processes)
In the previous section, we have seen how data can be passed from a worker
thread to the parent thread using _promises_ and _futures_. An additional
drawback to promises in futures is that using them requires writing a lot of
_boiler plate_ code for comparitively minor functionality.

If you simply wish to return data (or an exception) from a worker thread to the
parent, there is a much simpler way - using `std::async` rather than
`std::thread`. Calling `std::async` generates something called a __task__ - a
high-level construct in which the system can decide whether to run the task
_synchronously_ or _asynchronously_ (the code must be capable of being
parallelized).

__NOTE__: It is important to consider the __runtime performance__ of concurrent
a program - it is not _always_ the case that using threads to parallelize a task
will improve the runtime performance of a program. __It is possible that the
overhead required for creating and starting threads slows down the execution of
the code!__


## 2.5: Avoiding Data Races