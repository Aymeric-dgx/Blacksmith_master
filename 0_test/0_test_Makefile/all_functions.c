//------------------------------- main.c ----------------------------------//
#include "header.h"


int main() {
    print_hello();
    print_hello();
    return 0;
}


//------------------------------- options.c ----------------------------------//
#include <stdio.h>
#include "header.h"


void print_hello() {
    printf("Hello from options.c\n");
}




//------------------------------- header.h ----------------------------------//
#ifndef HEADER
#define HEADER

void print_hello();

#endif
