#pragma once
#include <util/base_type.h>
#include <util/time.hpp>


namespace program {

u8               getContextID();
Time::Timestamp& getFrameTime();
bool             getSlowRenderFlag();
void initializeLibrary();
void destroyLibrary();
void initializeGraphics();
void destroyGraphics();
void render();

}