// Include iostream for output
#include <cassert>
#include <iostream>
#include <vector>

/* to build and run:
 * $ cd obj/
 * $ g++ ../src/classes.cpp -o classes.o && ./classes.o
 * (w/ dbg sym) $ g++ -g ../src/classes.cpp -o classes.o && ./classes.o
 */

struct Date {
  public:   // use 'public' access modifier
    int getDay() {return day; };    // "accessor" of private var 'day'
    void setDay( int d ) {          // "mutator" of private var 'day'
      // example of an 'invariant' - logical conditions that member variables must adhere too
      if( d >= 1 && d <= 31) {
        this->day = d;          // not necessary to use 'this' ptr                
      }
    }

    int month{1};
    int year{2000};

  private:    // use 'private' access modifier
    int day{1};       // equiv to int day = 1;
};

class Date2 {
  public:
    // this does NOT work
    // Date2( int d, int m, int y ) : Day(d), Month(m), Year(y) { }

    // constructor "declaration" - constructor "definition" below
    Date2( int d, int m, int y );

    int Day() { return day_; }
    void Day(int d);

    int Month() { return month_; }
    void Month(int m);

    int Year() { return year_; }
    void Year(int y) { year_ = y; }

    const std::string String();

  private:
    const bool LeapYear( int year );
    const int DaysInMonth( int month, int year );

    int day_{1};
    int month_{1};
    int year_{2000};
};

/* SCOPE RESOLUTION OPERATOR (::)
* We can use this operator to specify which namespace or class to search in order to
* resolve an identifier. Example:

Person::move(); \\ Call the move the function that is a member of the Person class.
std::map m; \\ Initialize the map container from the C++ Standard Library.
*/

// use "scope resolution" operator (::) to define constructor outside class definition
// use the "initializer list" to set year_ because it doesn't use an invariant to set its value
/* NOTE: In general, prefer initialization to assignment. Initialization sets the value as
*   soon as the object exists, whereas assignment sets the value only after the object comes
*   into being. This means that assignment creates and opportunity to accidentally use a
*   variable before its value is set.
*/
/* NOTE: (about consts) In fact, initialization lists ensure that member variables are
*   initialized before the object is created. This is why class member variables can be
*   declared const, but only if the member variable is initialized through an initialization
*   list. Trying to initialize a const class member within the body of the constructor will
*   not work.
*/
Date2::Date2( int d, int m, int y ) : year_(y) {
  Day(d);
  Month(m);
}

void Date2::Day( int d ) {
  if ( d >= 1 && d <= DaysInMonth(Month(), Year()) ) {
    day_ = d;
  }
}

void Date2::Month( int m ) {
  if (m >= 1 && m <= 12) month_ = m;
}

const bool Date2::LeapYear( int year ) {
  if( year % 4 != 0 ) {
    return false;
  }
  else if( year % 100 != 0 ) {
    return true;
  }
  else if( year % 400 != 0) {
    return false;
  }
  else {
    return true;
  }
}

const int Date2::DaysInMonth( int month, int year ) {
  if( month == 2 ) {
    return LeapYear( year ) ? 29 : 28;  // conditional assignment
  }
  else if( month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  }
  else {
    return 31;
  }
}

const std::string Date2::String() {
  // should problably be declared as private member variable
  std::vector<std::string> months = {
    "January",   "February", "March",    "April",
    "May",       "June",     "July",     "August",
    "September", "October",  "November", "December"};;

    return months[month_ - 1] + " " + std::to_string(day_) + ", " + std::to_string(year_); 
}

/* INITIALIZER LISTS:
* 1. The compiler can optimize initialization faster from an initialization list than from
*   within the constructor.
* 2. If you have a const class attribute, you can only initialize it using an initialization
*   list. Otherwise, you would violate the const keyword simply by initializing the member in
*   the constructor!
* 3. Class attributes defined as references MUST use initialization lists.
*/

class Person {
  public:
    Person( std::string n ) : name(n) { } // uses an "initalizer list" in constructor
    std::string name;
};

class Birthday {
  public:
    Birthday( int d, int m, int y );

    int Day() { return day_; }
    int Month() { return month_; }
    int Year() { return year_; }

  private:
    // const member variables
    const int day_;
    const int month_;
    const int year_;
};

// NOTE: const member variables MUST be set using an initializer list
Birthday::Birthday( int d, int m, int y ) : day_(d), month_(m), year_(y) { }

// Define a main function to instantiate and test 
int main() {
  // create class of type Date2
  Date2 date2(29,2,2016);

  int d = 10;
  // date2.Day( d );
  
  // TEST
  assert(date2.Day() == 29);
  assert(date2.Month() == 2);
  assert(date2.Year() == 2016);
  assert(date2.String() == "February 29, 2016");

  // Print the data in the structure
  std::cout << date2.Day() << "/" << date2.Month() << "/" << date2.Year() << "\n";

  Person alice("Alice");
  Person bob("Bob");
  assert(alice.name != bob.name);
}