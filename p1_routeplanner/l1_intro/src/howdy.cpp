#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> brothers{"Carl", "Chris", "Calvin"};
    for (std::string const& brother : brothers) {
        std::cout << "Howdy, " << brother << "!\n";
    }
}