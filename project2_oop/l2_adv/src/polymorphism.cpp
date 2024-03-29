#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <cmath>

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
* " Operator overlaoding allows us to assign our own logic to operators. For example, you
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
* from which other classes can be derived." These functions are declared (and possibly
* defined) in a base class, and can be overridden by derived classes. This approach declares
* an interface at the base level, but delegates the implementation of the interface to the
* derived classes.
*/

// NOTE: A pure virtual function has the side effect of making its class abstract. This means
// that the class cannot be instantiated. Instead, only classes that derive from the abstract
// class and override the pure virtual function can be instantiated. 

class Shape { // define "abstract base class" Shape()
  public:
    // declare Area() and Perimiter() as "pure virtual" (= 0). A pure virtual function is a
    // virtual function that the base class declares but does not define. A derived class
    // must define this function, or else the derived class will also be abstract.

    // NOTE: double Area() const
    //  Use const after name of function to indicate that the function should not modify any
    //  variables of the associated class.
    // NOTE: const double& Area()
    //  this will return a const reference to a variable of type double

    virtual double Area() const = 0;
    virtual double Perimeter() const = 0;
};

// TODO: Define Rectangle to inherit publicly from Shape
  // TODO: Declare public constructor
  // TODO: Override virtual base class functions Area() and Perimeter()
  // TODO: Declare private attributes width and height
class Rectangle : public Shape {
  public:
    Rectangle( double w, double h) : width_(w), height_(h) {}  // constructor
    /* NOTE: On const functions:
    * The idea of const functions is not to allow them to modify the object on which they are
    * called. It is recommended the practice to make as many functions const as possible so
    * that accidental changes to objects are avoided.
    */
    double Area() const override { return width_ * height_; }
    double Perimeter() const override { return 2.0 * (width_ + height_); }

    private:
      double width_{0.0};
      double height_{0.0};
};

// TODO: Define Circle to inherit from Shape
  // TODO: Declare public constructor
  // TODO: Override virtual base class functions Area() and Perimeter()
  // TODO: Declare private member variable radius
class Circle : public Shape {
  public:
    Circle( double r ) : radius_(r) {}  // constructor

    double Area() const override { return M_PI * pow(radius_,2); }
    double Perimeter() const override { return 2.0 * M_PI * radius_; }

    private:
      double radius_{0.0};
};

/* FUNCTION OVERRDING:
* "Let's the compiler (and people reading your code) know that this function will 'override'
* a function declared as 'virtual' in the base class." Use the  override keyword as a 'best
* practice' - the compiler will verify that a function specified as 'override' does indeed
* override some other virtual function, or otherwise the compiler will generate an error.
*/

// TODO: Declare abstract class VehicleModel
  // TODO: Declare virtual function Move()
class VehcicleModel {
  public:
    // define "pure virtual" function Move()
    virtual void Move( double v, double theta ) = 0;
};

// TODO: Derive class ParticleModel from VehicleModel
  // TODO: Override the Move() function
  // TODO: Define x, y, and theta
class ParticleModel : public VehcicleModel {
  public:
    void Move( double v, double phi ) override {
      theta += phi;
      x += v * cos(theta);
      y += v * sin(theta);
    }

  double x{0.0};
  double y{0.0};
  double theta{0.0};
};

// TODO: Derive class BicycleModel from ParticleModel
  // TODO: Override the Move() function
  // TODO: Define L
class BicycleModel : public ParticleModel {
  public:
    void Move( double v, double phi ) override {
      theta += v / L * tan(phi);
      x += v * cos(theta);
      y += v* sin(theta);
    }

    double L{1.0};
};

/* MULTIPLE INHERITANCE:
* 
*
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
  double epsilon = 0.1; // useful for floating point equality

  // Test circle
  Circle circle(12.31);
  assert(abs(circle.Perimeter() - 77.35) < epsilon);
  assert(abs(circle.Area() - 476.06) < epsilon);

  // Test rectangle
  Rectangle rectangle(10, 6);
  assert(rectangle.Perimeter() == 32);
  assert(rectangle.Area() == 60);

  // Test function overriding
  ParticleModel particle;
  BicycleModel bicycle;
  particle.Move(10, M_PI / 9);
  bicycle.Move(10, M_PI / 9);
  assert(particle.x != bicycle.x);
  assert(particle.y != bicycle.y);
  assert(particle.theta != bicycle.theta);
}