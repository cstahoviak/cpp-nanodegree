// Include iostream for output
#include <cassert>
#include <iostream>

/* to build and run:
 * $ cd obj/
 * $ g++ ../src/structures.cpp -o structures.o && ./structures.o
 * (w/ dbg sym) $ g++ -g ../src/structures.cpp -o structures.o && ./structures.o
 */

/* Define a simple structure:
* 3 types: public, private, and protected
* NOTE: all members of a structure are 'public' unless specifically marked 'private'
* NOTE: all members of a class are 'private' unless specifically marked 'private'
* NOTE: Avoid Trivial Getters And Setters:
*   Sometimes accessors are not necessary, or even advisable. The C++ Core Guidelines
*   recommend, "A trivial getter or setter adds no semantic value; the data item could just
*   as well be public." Below is an example from the Core Guidelines.
*/

class Point {
    int x;
    int y;
  public:
    Point(int xx, int yy) : x{xx}, y{yy} { }
    int get_x() const { return x; }  // const here promises not to modify the object
    void set_x(int xx) { x = xx; }
    int get_y() const { return y; }  // const here promises not to modify the object
    void set_y(int yy) { y = yy; }
    // no behavioral member functions
};

// This class could be made into a struct, with no logic or "invariants", just passive data.
// The member variables could both be public, with no accessor functions:

struct Point2 {   // Good: concise (cannot share same name as above class)
    int x {0};   // public member variable with a default initializer of 0
    int y {0};   // public member variable with a default initializer of 0
};


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
    int Day() { return day_; }
    void Day(int d) {
      if (d >= 1 && d <= 31) day_ = d;
    }

    int Month() { return month_; }
    void Month(int m) {
      if (m >= 1 && m <= 12) month_ = m;
    }

    int Year() { return year_; }
    void Year(int y) { year_ = y; }

  private:
    int day_{1};
    int month_{1};
    int year_{2000};
};

// Define a main function to instantiate and test 
int main()
{
  // create struct of type Date
  Date date;
  
  int d = 5;
  date.setDay( d );
  
  // TEST
  assert(date.getDay() == 5);
  assert(date.month == 1);
  assert(date.year == 2000);

  // Print the data in the structure
  std::cout << date.getDay() << "/" << date.month << "/" << date.year << "\n";

  // create class of type Date2
  Date2 date2;
  date2.Day( d );
  
  // TEST
  assert(date2.Day() == 5);
  assert(date2.Month() == 1);
  assert(date2.Year() == 2000);

  // Print the data in the structure
  std::cout << date2.Day() << "/" << date2.Month() << "/" << date2.Year() << "\n";
}