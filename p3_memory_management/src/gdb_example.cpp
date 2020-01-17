#include <stdio.h>

/* IN TERMINAL:
*   cd obj
*   g++ -g ../src/example.cpp -o example.o
*   gdb example.o       // launch gdb
*   (gdb) list          // prints contents of .cpp file
*   (gdb) break 5       // sets breakpoint at line 5 of example.cpp
*   (gdb) run           // will run executable and halt at line 5
*   (gdb) step          // step over line 5 to line 6
*   (gdb) p str1        // print str1 to terminal
*   (gdb) p &str1       // print hex memory address of str1
*   (gdb) x/7tb &str1   // print str1 as binary integer (t)
*   (gdb) x/7xb &str1   // print str1 as hexedecimal integer (x)
*/

int main()
{
    char str1[]= "UDACITY";
    printf("%s\n",str1);

    return 0;
}