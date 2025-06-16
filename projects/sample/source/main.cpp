#include <cstdio>
#include <libexample/example.hpp>


int main()
{
    std::printf("Hello From Main!\n");

    std::printf("%s", example_library_get_string());
    example_library_init();
    
    std::printf("%s", example_library_get_string());
    example_library_destroy();
    
    std::printf("%s", example_library_get_string());
    

    /* Tests for leak-sanitizer, address-sanitizer, undefined-behaviour-sanitizer */
    // std::printf("Hello world %u\n", 5);
    // int *prt = new int;

    // int k = 0x7fffffff;
    // k += 4;

    // int *array = new int[100];
    // delete [] array;
    // return array[200];  // BOOM
    
    // std::printf("AH");
    // return 0x42;
}