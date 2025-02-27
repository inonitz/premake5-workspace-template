#include "fixfeature.hpp"
#include <threads.h>
#include <awc2/C/awc2.h>
#include "util/time.hpp"
#include "vars.hpp"
#include "render.hpp"


i32 fix_new_features_imgui_interfact_is_buggy()
{
    const struct timespec pause_sleep_duration{
        .tv_sec = 0,
        .tv_nsec = 6944444
    };
    u8 alive{true}, paused{false};


    markstr("fix_new_features_imgui_interfact_is_buggy begin");
    fixfeatures19::initializeLibrary();
    fixfeatures19::initializeGraphics();

    markstr("Main App Loop Begin"); /* Main App Loop */
    awc2setCurrentContext(fixfeatures19::getContextID());
    while(alive) 
    {
        fixfeatures19::getFrameTime().begin();
        TIME_NAMESPACE_TIME_CODE_BLOCK(fixfeatures19::getTimer0(), {
            awc2newframe();
            awc2begin();
        });

        
        if(likely(!paused)) {
            TIME_NAMESPACE_TIME_CODE_BLOCK(fixfeatures19::getRenderTime(), fixfeatures19::render());
        } else {
            thrd_sleep(&pause_sleep_duration, NULL);
        }


        alive   = !awc2getContextStatus(fixfeatures19::getContextID()) && !awc2isKeyPressed(AWC2_KEYCODE_ESCAPE);
        paused ^= awc2isKeyPressed(AWC2_KEYCODE_P);
        if(awc2getCurrentContextWindowState() & AWC2_WINDOW_STATE_FLAG_MINIMIZED)
            paused = true;
        
        TIME_NAMESPACE_TIME_CODE_BLOCK(fixfeatures19::getTimer1(), awc2end());
        fixfeatures19::getFrameTime().end();
        ++fixfeatures19::getFrameCounter();
    }
    markstr("Main App Loop End");

    /* return resources */
    fixfeatures19::destroyGraphics();
    fixfeatures19::destroyLibrary();
    markstr("fix_new_features_imgui_interfact_is_buggy end");
    return 1;
}