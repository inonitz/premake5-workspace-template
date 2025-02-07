#include <util/marker2.hpp>
#include <util/types.hpp>
#include <util/macro.h>

#define _SELECT_MAIN 0

#if _SELECT_MAIN == 0
#include "main/gem38/gem38.hpp"
#elif _SELECT_MAIN == 1
#include "main/examples/simple_window.hpp"
#elif _SELECT_MAIN == 2
#include "main/examples/compute_screen.hpp"
#elif _SELECT_MAIN == 3
#include "main/examples/compute_buffer.hpp"
#elif _SELECT_MAIN == 4
#include "main/examples/advection_boundary.hpp"
#elif _SELECT_MAIN == 5
#include "main/examples/compute_buffer_mouse.hpp"
#elif _SELECT_MAIN == 6
#include "main/examples/test_diffusion.hpp"
#endif

int main(__unused int argc, __unused char* argv[]) {
    i32 out = 0x69;
    markstr("Successful main enter");


#if _SELECT_MAIN == 0
    out = gpugems38_main();
#elif _SELECT_MAIN == 1
    out = simple_window();
#elif _SELECT_MAIN == 2
    out = compute_shader_render_to_screen();
#elif _SELECT_MAIN == 3
    out = compute_shader_render_buffer_to_screen();
#elif _SELECT_MAIN == 4
    out = advection_and_boundary_conditions();
#elif _SELECT_MAIN == 5
    out = compute_shader_render_buffer_to_screen_mouse_interaction();
#elif _SELECT_MAIN == 6
    out = test_diffusion();
#endif

    markstr("Successful Exit");
    return out;
}