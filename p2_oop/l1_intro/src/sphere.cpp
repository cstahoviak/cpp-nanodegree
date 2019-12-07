#include <cassert>
#include <cmath>
#include <stdexcept>
#include <iostream>

class Sphere {
  public:
    Sphere( float radius ) : radius_(radius),
                             volume_((4.0/3.0) * pi_ * pow(radius_,3)) {
      if( radius <= 0 ) {
        throw std::invalid_argument("radius must be positive");
      }
    }
    
    // accessors
    float Radius() { return radius_; }
    float Volume() { return volume_; }

    // mutators
    void Radius( float radius );
    static float Volume( float radius ) { return (4.0/3.0) * pi_ * pow(radius,3); }

  private:
    /* NOTE: a 'static' variable is a variable that belongs to the *whole* class and not just
    *  an object of that class. More specifically, a static member is created only once and
    * then shared by all instances (i.e. objects) of the class. That means that if the static
    * member gets changed, either by a user of the class or within a member function of the
    * class itself, then all members of the class will see that change the next time they
    * access the static member.
    */
    static constexpr float pi_{M_PI};

    /* ADDITIONAL NOTES ON STATIC AND CONSTEXPR
    * 1. static members are declared within their class (often in a header file) but in most
    * cases they must be defined within the global scope. That's because memory is allocated
    * for static variables immediately when the program begins, at the same time any global
    * variables are initialized.
    *
    * class Foo {
    *   public:
    *     static int count;
    *     Foo() { Foo::count += 1; }
    * };

    * int Foo::count{0};
    * 
    * 2. An exception to the global definition of static members is if such members can be
    * marked as constexpr. In that case, the static member variable can be both declared and
    * defined within the class definition:
    * 
    * struct Kilometer {
    *   static constexpr int meters{1000};
    * };
    */

    // cannot be const because a mutator can change these values
    float radius_{0.0f};
    float volume_{0.0f};
};

void Sphere::Radius( float radius ) {
  if( radius >= 0 ){
    radius_ = radius;
    volume_ = (4.0/3.0) * pi_ * pow(radius_,3);
  }
  else {
    throw std::invalid_argument("radius must be positive");
  }
}

// NOTE: "a storage class may not be specified here"
// static float Sphere::Volume( float radius ) {
//   return (4.0/3.0) * pi_ * pow(radius,3);
// }

int main() {

  Sphere sphere(5);

  assert(sphere.Radius() == 5);
  assert(abs(sphere.Volume() - 523.6) < 1);

  sphere.Radius(3.0);
  assert(sphere.Radius() == 3.0);
  assert(abs(sphere.Volume() - 113.1) < 1);

  bool caught{false};
  try {
    sphere.Radius(-1.0);
  } catch (...) {
    caught = true;
  }
  assert(caught);

  assert(abs(Sphere::Volume(5) - 523.6) < 1);
}