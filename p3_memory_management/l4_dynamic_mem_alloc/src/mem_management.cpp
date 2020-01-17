#include <iostream>
#include<stdio.h>
#include <string.h>

int main() {
  // BUFFER OVERRUNS:
  char str[5];
  // strcpy(str,"BufferOverrun");  // causes segfault
  // printf("%s",str);

  // UNINITIALOZED MEMORY:
  // the behavior of the following code is potentially undefined:
  int a;
  int b = a*42;
  printf("%d\n", b);

  // INCORRECT PAIRNING OF NEW / DELETE OPERATIONS:
  // ex1: the wrong new and delete are paired:
  double *pDbl = new double[5];
  delete pDbl;

  // ex2: the pairing is correct but a double deletion is performed:
  char *pChr = new char[5];
  delete[] pChr;
  delete[] pChr;

  // INVALID MEMORY ACCESS:
  // In this example, the heap memory has already been deallocated at
  // the time when strcpy() tries to access it:
  char *pStr = new char[25];
  delete[] pStr;
  strcpy(pStr, "Invalid Access");
  // printf("%s\n", *pStr);sh
}