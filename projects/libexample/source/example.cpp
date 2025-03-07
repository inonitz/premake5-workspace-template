#include "libexample/example.hpp"


static const char* preinit_string = "Pre-Initialization Welcome from libexample!\n";
static const char* init_string    = "Initialized        Welcome from libexample!\n";
static const char* destroy_string = "Goodbye from libexample!\n";
static const char* __random_string = preinit_string;


int example_library_init()
{
    __random_string = init_string;
    return 1;
}


void example_library_destroy()
{
    __random_string = destroy_string;
    return;
}


char const* example_library_get_string()
{
    return __random_string;
}