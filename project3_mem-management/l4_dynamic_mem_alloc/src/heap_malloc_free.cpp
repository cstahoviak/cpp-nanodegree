#include <iostream>

/* UNDERSTANDING THE HEAP:
* To allocate dynamic memory on the heap means to make a contiguous
* memory area accessible to the program at runtime and to mark this
* memory as occupied so that no one else can write there by mistake.
*
* To reserve memory on the heap, one of the two functions malloc
* (Memory Allocation) or calloc (Cleared Memory Allocation) is used.
* The header file stdlib.h or malloc.h must be included to use the
* functions.
*
*   pointer_name = (cast-type*) malloc(size);
*   pointer_name = (cast-type*) calloc(num_elems, size_elem);
*
* malloc is used to dynamically allocate a single large block of memory
* with the specified size. It returns a pointer of type void which can
* be cast into a pointer of any form.
*
* calloc is used to dynamically allocate the specified number of blocks
* of memory of the specified type. It initializes each block with a
* default value '0'.
*
* Both functions return a pointer of type void which can be cast into
* a pointer of any form. If the space for the allocation is
* insufficient, a NULL pointer is returned.
*/

struct MyStruct {
  int i; 
  double d; 
  char a[5];
};

int main() {
  /* MALLOC:
  *   At compile time, only the space for the pointer is reserved
  *   (on the stack). When the pointer is initialized, a block of
  *   memory of sizeof(int) bytes is allocated (on the heap) at
  *   program runtime. The pointer on the stack then points to this
  *   memory location on the heap.
  */
  int *p1 = (int*) malloc(sizeof(int));
  printf("address=%p, value=%d\n", p1, *p1);

  // since arrays and pointers are displayed and processed identically
  // internally, individual blocks of data can also be accessed using
  // array syntax:
  int *p2 = (int*)malloc(3*sizeof(int));
  p2[0] = 1;
  p2[1] = 2;
  p2[2] = 3;
  printf("address=%p, second value=%d\n", p2, p2[1]);

  // we can also define a proprietary structure which consists of
  // several primitive data types and use malloc or calloc in the
  // same manner as before.
  MyStruct *p3 = (MyStruct*)calloc(4,sizeof(MyStruct));
  p3[0].i = 1;
  p3[0].d = 3.14159;
  p3[0].a[0] = 'a';

  /* REALLOC:
  *   The size of the memory area reserved with malloc or calloc can
  *   be increased or decreased with the realloc function.
  */

  // reserve memory for two integers
  int *p = (int*)malloc(2*sizeof(int));
  p[0] = 1; p[1] = 2; 

  // resize memory to hold four integers
  p = (int*)realloc(p,4*sizeof(int));
  p[2] = 3; p[3] = 4; 

  printf("address=%p, value=%d\n", p+0, *(p+0)); // valid
  printf("address=%p, value=%d\n", p+1, *(p+1)); // valid
  printf("address=%p, value=%d\n", p+2, *(p+2)); // valid
  printf("address=%p, value=%d\n", p+3, *(p+3)); // valid

  // resize memory again to hold two integers
  p = (int*)realloc(p,2*sizeof(int));

  printf("address=%p, value=%d\n", p+0, *(p+0)); // valid
  printf("address=%p, value=%d\n", p+1, *(p+1)); // valid
  // NOTE: It should be noted however that accessing a memory location
  // after shrinking the allocated memory must be avoided as it could
  // cause a segmentation fault.
  printf("address=%p, value=%d\n", p+2, *(p+2)); // DANGEROUS
  printf("address=%p, value=%d\n", p+3, *(p+3)); // DANGEROUS

  /* FREE:
  *   The free function releases the reserved memory area so that it
  *   can be used again or made available to other programs.
  * 
  *   1. free can only free memory that was reserved with malloc or calloc.
  *   2. free can only release memory that has not been released before.
  *   Releasing the same block of memory twice will result in an error.
  * 
  *   The pointer p5 in the example is invalid as soon as free(p4) is
  *   called. It still holds the address to the memory location which
  *   has been freed, but may not access it anymore. Such a pointer is
  *   called a "dangling pointer".
  */
  double* p4 = (double*) malloc(100*sizeof(double));
  double* p5 = p4;

  free(p4);
  free(p5);   // ERROR.. I'm not actually getting an error. Why??

  return 0;
}