#include "boundary.hpp"
#include <threads.h>
#include <awc2/C/awc2.h>
#include "util/time.hpp"
#include "vars.hpp"
#include "render.hpp"


i32 arbitrary_boundaries_in_sim2()
{
    const struct timespec pause_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 6944444
    };
    u8 alive{true}, paused{false};


    markstr("arbitrary_boundaries_in_sim begin");
    boundary22::initializeLibrary();
    boundary22::initializeGraphics();

    markstr("Main App Loop Begin"); /* Main App Loop */
    awc2setCurrentContext(boundary22::getContextID());
    while(alive) 
    {
        boundary22::g_frameTime.begin();
        TIME_NAMESPACE_TIME_CODE_BLOCK(boundary22::g_beginFrameTime, {
            awc2newframe();
            awc2begin();
        });

        
        if(likely(!paused)) {
            TIME_NAMESPACE_TIME_CODE_BLOCK(boundary22::g_renderTime, boundary22::render());
        } else {
            thrd_sleep(&pause_sleep_duration, NULL);
        }


        alive   = !awc2getContextStatus(boundary22::getContextID()) && !awc2isKeyPressed(AWC2_KEYCODE_ESCAPE);
        paused ^= awc2isKeyPressed(AWC2_KEYCODE_P);
        if(awc2getCurrentContextWindowState() & AWC2_WINDOW_STATE_FLAG_MINIMIZED)
            paused = true;
        
        TIME_NAMESPACE_TIME_CODE_BLOCK(boundary22::g_endFrameTime, awc2end());
        boundary22::g_frameTime.end();
        ++boundary22::g_frameCounter;
    }
    markstr("Main App Loop End");

    /* return resources */
    boundary22::destroyGraphics();
    boundary22::destroyLibrary();
    markstr("arbitrary_boundaries_in_sim end");
    return 1;
}