// This example demonstrates the privacy levels
// between parent and child classes
#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

/* INHERITANCE: 
* Inheritance modifiers (public, protected, private) refer to how an object of the
* derived class exposes the members (variables and methods) of the base class to the user.
* In practice, 'protected' is not used all that often.

* PUBLIC: the public and protected members of the base class listed after the specifier keep
* their member access in the derived class

* PROTECTED: the public and protected members of the base class listed after the specifier
* are protected members of the derived class

* PRIVATE: the public and protected members of the base class listed after the specifier are
* private members of the derived class
*/

class Vehicle {
  public:
    int wheels = 0;
    std::string color = "blue";
    
    void Print() const
    {
        std::cout << "This " << color << " vehicle has " << wheels << " wheels!\n";
    }
};

class Car : public Vehicle {
  public:
    bool sunroof = false;
};

class Bicycle : protected Vehicle {
  public:
    bool kickstand = true;
    void Wheels( int w ) { wheels = w; }
};

class Scooter : private Vehicle {
  public:
    bool electric = false;
    void Wheels( int w ) { wheels = w; }
};

class Animal {
  public:
    std::string name{""};
    std::string color{""};
    int age{0};
};

class Snake : public Animal {
  public:
    float length{0.0f};
    void MakeSound() { std::cout << "hissss!\n"; }
};

class Cat : public Animal {
  public:
    float height{0.0f};
    void MakeSound() { std::cout << "meow!\n"; }
};

/* COMPOSITION:
* Composition is a closely related alternative to inheritance. Composition involves
* constructing ("composing") classes from other classes, instead of inheriting traits from a
* parent class.

* A common way to distinguish "composition" from "inheritance" is to think about what an object
* can do, rather than what it is. This is often expressed as "has a" versus "is a".
*/

// Define LineSegment struct
struct LineSegment {
  public:
    double length;
};

// Define Circle class
class Circle {
  public:
    // the Circle() class is "composed of" the LineSegment struct
    Circle(LineSegment& radius) : radius_(radius) { }
    double Area();

private:
    LineSegment& radius_;   // radius_ is a reference to a var of type LineSegment
};

double Circle::Area() 
{
    return pow(Circle::radius_.length, 2) * M_PI;
}

/* FRIEND ACCESS MODIFIER:
* NOTE: The 'friend' keyword grants access to the private members of a class.

* NOTE: The main difference between classical inheritance and friend inheritance is that a
* friend class can access private members of the base class, which isn't the case for classical
* inheritance. In classical inheritance, a derived class can only access public and protected
* members of the base class.
*/

// NOTE: Square must be declared before Rectangle in order to pass an object of type Square
// to the Rectangle constructor.
class Square {
  public:
    Square(float side) : side_(side) { }

  private:
    friend class Rectangle; // "class Rectangle is a 'friend' of class Square"
    float side_;
};

class Rectangle {
  public:
    Rectangle( const Square& square ) : width_(square.side_), hieght_(square.side_) { }
    float Area() { return width_ * hieght_; }
  
  private:
    float width_;
    float hieght_;
};

int main() 
{
    Car car;
    Bicycle bike;
    Scooter scooter;

    car.Print();
    // bike.Print();     // "Vehcle::Print() inaccessible within this context"
    // scooter.Print();  // "Vehcle::Print() inaccessible within this context"

    car.wheels = 4;
    bike.Wheels(2);
    scooter.Wheels(2);

    Snake snake;
    Cat cat;

    cat.name = "Snowball";
    cat.color = "white";
    cat.age = 19;
    cat.height = 24;
    cat.MakeSound();

    LineSegment radius{3};
    Circle circle( radius );
    assert(int(circle.Area()) == 28);

    Square square(4);
    Rectangle rectangle(square);
    assert(rectangle.Area() == 16.0);
};