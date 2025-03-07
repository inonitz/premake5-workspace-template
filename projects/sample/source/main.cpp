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
    
    
    return 0x42;
}