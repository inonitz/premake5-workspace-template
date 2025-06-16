#include <stdio.h>
#include <util2/vec2.hpp>
#include <memory>
#include <util2/C/marker4.h>


int main()
{
    std::unique_ptr<int> test;
    printf("Hello world %u\n", 5);
    // mark();
    markstr("HELLO");
    int *prt = new int;

    int k = 0x7fffffff;
    k += 4;

    
    markfmt("OOGA %u BOOGA %p", 3, (unsigned long long)(0xDEADBEEF));
    return 1;
}