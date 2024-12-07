#include "main/awc2fluid/simple.hpp"
#include "main/gpu-gems38/gem38.hpp"
#include <util/marker2.hpp>
#include <util/types.hpp>


#define run_legacy_code 1


int main() {
    i32 out = (run_legacy_code == 1) ? render_fluid_awc2_fuckyou() : render_gpugems38();
     markstr("Successful Exit");
    return out;
    // return 1;

}