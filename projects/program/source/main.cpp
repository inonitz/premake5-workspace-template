#include <util/marker2.hpp>
#include <util/types.hpp>
#include <util/macro.h>

#define _SELECT_MAIN 10


#if _SELECT_MAIN == 0
#include "0gem38/gem38.hpp"
#elif _SELECT_MAIN == 1
#include "1simple_window/simple_window.hpp"
#elif _SELECT_MAIN == 2
#include "2compute_screen/compute_screen.hpp"
#elif _SELECT_MAIN == 3
#include "3compute_buffer/compute_buffer.hpp"
#elif _SELECT_MAIN == 4
#include "4advection_boundary/advection_boundary.hpp"
#elif _SELECT_MAIN == 5
#include "5test_diffusion/test_diffusion.hpp"
#elif _SELECT_MAIN == 6
#include "6compute_buffer_mouse/compute_buffer_mouse.hpp"
#elif _SELECT_MAIN == 7
#include "7compute_buffer_mouse_diffusion/compute_buffer_mouse_diffusion.hpp"
#elif _SELECT_MAIN == 8
#include "8compute_buffer_mouse_advection_diffusion/compute_buffer_mouse_advection_diffusion.hpp"
#elif _SELECT_MAIN == 9
#include "9forces_advection_diffusion/forces_advection_diffusion.hpp"
#elif _SELECT_MAIN == 10
#include "10velocityfield/velocityfield.hpp"
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
    out = test_diffusion();
#elif _SELECT_MAIN == 6
    out = compute_shader_render_buffer_to_screen_mouse_interaction();
#elif _SELECT_MAIN == 7
    out = compute_buffer_mouse_but_with_diffusion();
#elif _SELECT_MAIN == 8
    out = compute_buffer_mouse_but_with_advection_and_diffusion();
#elif _SELECT_MAIN == 9
    out = compute_shader_external_forces_advection_diffusion_draw();
#elif _SELECT_MAIN == 10
    out = compute_shader_draw_full_velocity_field();
#endif

    markstr("Successful Exit");
    return out;
}