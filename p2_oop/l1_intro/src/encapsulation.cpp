// Include iostream for output
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <string.h>

/* to build and run:
 * $ cd obj/
 * $ g++ ../src/encapsulation.cpp -o encapsulation.o && ./encapsulation.o
 * (w/ dbg sym) $ g++ -g ../src/encapsulation.cpp -o encapsulation.o && ./encapsulation.o
 */

/* ENCAPSULATION:
* "Encapsulation is the grouping together of data and logic into a single unit. In
* object-oriented programming, classes encapsulate data and functions that operate on that
* data."

* "This can be a delicate balance, because on the one hand we want to group together relevant
* data and functions, but on the hand we want to limit member functions to only those
* functions that need direct access to the representation of a class."

* "The data has been 'encapsulated' within the object that provides the interface to that
* data." - Bjarne

*/

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

  private:
    const bool LeapYear( int year );
    const int DaysInMonth( int month, int year );

    int day_{1};
    int month_{1};
    int year_{2000};
};
// use "scope resolution" operator (::) to define constructor outside class definition
// use the "initializer list" to set year_ because it doesn't use an invariant to set its value
Date2::Date2( int d, int m, int y ) : year_(y) {
  Day(d);
  Month(m);
}

// NOTE: "The whole point of the mutator method is to allow us to set invariant logic to 
// prevent the user from setting member variables to an invalid state." - David

// mutator: mutates private member vairable 'day_'
void Date2::Day( int d ) {
  if ( d >= 1 && d <= DaysInMonth(Month(), Year()) ) {
    day_ = d;
  }
}

// mutator: mutates private member vairable 'month_'
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
    // conditional assignment - return 29 if true, 28 if false
    return LeapYear( year ) ? 29 : 28;
  }
  else if( month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  }
  else {
    return 31;
  }
}

class BankAccount {
  public:
    // constructor declaration 
    BankAccount( int num, std::string name );

    // declare "setter" methods
    void Funds( float num ) {
      if( num >= 0 ) funds_ = num;
    }

    // declare "getter" methods
    int AccountNum( ) { return acc_num_; }
    std::string Owner( ) { return owner_; }
    float Funds( ) { return funds_; } 

  private:
    // declare member variables
    const int acc_num_{0};
    const std::string owner_{""};
    float funds_{0.0f};
};

// constructor definition
BankAccount::BankAccount( int num, std::string name ) : acc_num_(num), owner_(name) { }

class Car {
  public:
    // accessor methods
    int Horsepower() { return hp_; }
    int Weight() { return weight_; }
    std::string Brand() {return brand_; }

    // mutator methods
    void Horsepower( int hp ) { if( hp > 0 ) hp_ = hp; }
    void Weight( float w ) { if( w > 0 ) weight_ = w; }
    void Brand( char* brand );

  private:
    int hp_{0};
    float weight_{0.0f};
    std::string brand_ = "";
};

void Car::Brand( char* brand ) {
  
  brand_ = std::string(brand);
}

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

  // Print the data in the structure
  std::cout << date2.Day() << "/" << date2.Month() << "/" << date2.Year() << "\n";

  // instantiate and output a bank account
  BankAccount account( 0001, "Carl Stahoviak");

  account.Funds(1000.0);

  std::cout << account.Owner() << "\n" << account.AccountNum() << "\n" << account.Funds() << "\n";

  Car car;
  char brand[] = "Peugeot";
  car.Brand(brand);
  std::cout << car.Brand() << "\n";

}