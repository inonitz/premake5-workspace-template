#include "optimize.hpp"
#include "backend14.hpp"
#include <threads.h>
#include <awc2/C/awc2.h>
#include <util/marker2.hpp>


i32 optimize_diffusion_and_work_distribution_across_shaders_also_revalidate_parallel_reduction()
{
    const struct timespec pause_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 6944444
    };
    const struct timespec slow_render_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 300 * 1000000
    };
    u8 alive {true};
    u8 paused{false}, prevPause{false};


    markstr("compute_shader_2d_fluid_also_cfl begin");
    optimize::initializeLibrary();
    optimize::initializeGraphics();

    markstr("Main App Loop Begin"); /* Main App Loop */
    awc2setCurrentContext(optimize::getContextID());
    while(alive) 
    {
        optimize::getFrameTime().begin();
        awc2newframe();
        awc2begin();
        if(paused) {
            thrd_sleep(&pause_sleep_duration, NULL);
        } else {
            if(optimize::getSlowRenderFlag()) {
                thrd_sleep(&slow_render_sleep_duration, NULL);
            }
            optimize::render();
        }
        alive   = !awc2getContextStatus(optimize::getContextID()) && !awc2isKeyPressed(AWC2_KEYCODE_ESCAPE);
        
        paused ^= awc2isKeyPressed(AWC2_KEYCODE_P);
        if(awc2getCurrentContextWindowState() & AWC2_WINDOW_STATE_FLAG_MINIMIZED)
            paused = true;
        awc2end();
        optimize::getFrameTime().end();
    }
    markstr("Main App Loop End");

    /* return resources */
    optimize::destroyGraphics();
    optimize::destroyLibrary();
    markstr("compute_shader_2d_fluid_also_cfl end");
    return 1;
}