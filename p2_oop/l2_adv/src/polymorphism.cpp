#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>

/* FUNCTION OVERLOADING:
* Overloading requires that we leave the function name the same, but we modify the function
* signature. For example, we might define the same function name with multiple different
* configurations of input arguments.
*/

class Date {
  public:
    Date(int day, int month, int year) : day_(day), month_(month), year_(year) {}
    Date(int day, int month) : day_(day), month_(month)  // automatically sets the Date to the current year
    {
        time_t t = time(NULL);
        tm* timePtr = localtime(&t);
        year_ = timePtr->tm_year;
    }

  private:
    int day_;
    int month_;
    int year_;
};

class Human {};

void hello() {
  std::cout << "Hellow, World!\n";
}

void hello(Human human) {
  std::cout << "Hellow, Human!\n";
}

/* OPERATOR OVERLOADING:
* " Operator overlaoding allows us to assing our own logic to operators. For example, you
* might want to perform vector addition on a pair of points to add their x and y components.
* The compiler won't recognize this type of operation on its own, because this data is user
* defined. However, you can overload the + operator so it performs the action that you want
* to implement."
*/

// TODO: Define Point class
class Point {
  public:
    // TODO: Define public constructor
    Point(float x=0.0f, float y=0.0f) : x(x), y(y) {}

    // TODO: Define + operator overload
    Point operator+ (const Point& addend) {
      Point sum;
      sum.x = x + addend.x;
      sum.y = y + addend.y;
      return sum;
    }

    // TODO: Declare attributes x and y
    float x;
    float y;

  private:

};

/* VIRTUAL FUNCTIONS:
* "Virtual functions allow us to define an abstract class that can function as an interface
* from which other classes can be derived"
*/

int main(void) {

  Human human;

  // FUNCTION OVERLOADING
  hello();
  hello(human);
  hello(Human());   // don't need to create an object of type Human to call hello(Human human)

  // OPERATOR OVERLAODING
  Point p1(10.0, 5.0), p2(2.0, 4.0);
  Point p3 = p1 + p2; // An example call to "operator +";
  assert(p3.x == p1.x + p2.x);
  assert(p3.y == p1.y + p2.y);

  // VIRTUAL FUNCTIONS
  
}