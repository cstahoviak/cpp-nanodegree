#include <iostream>
#include <string>
#include <vector>

/* How to launch Debugger:
 * 1. cd obj 
 * 2. compile file with debug symbols: g++ -g ../src/howdy.cpp -o howdy.o
 * 3. Make sure that .o file is compiled with most current saved version of .cpp file
 */

int main() {
    std::vector<std::string> brothers{"Carl", "Chris", "Calvin"};
    for (std::string const& brother : brothers) {
        std::cout << "Howdy, " << brother << "!\n";
    }
}