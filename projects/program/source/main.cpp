#include <util/marker2.hpp>
#include <util/types.hpp>
#include <util/macro.h>


#define which_main_flag 2


#if which_main_flag == 0
#   include "main/gpu-gems38/gem38.hpp"
#elif which_main_flag == 1
#   include "main/awc2fluid/simple.hpp"
#elif which_main_flag == 2
#   include "main/awc2cfluid/simplec.hpp"
#endif


int main(__unused int argc, __unused char* argv[]) {
    i32 out = 0x69;
    markstr("Successful main enter");


#if which_main_flag == 0
    out = render_gpugems38();

#elif which_main_flag == 1
    out = render_fluid_awc2_fuckyou();

#elif which_main_flag == 2
    out = render_fluid_awc2_c();
#endif


    markstr("Successful Exit");
    return out;
}