#pragma once
#include <util/base_type.h>
#include <util/time.hpp>


namespace program0 { 


u8               getContextID();
Time::Timestamp& getFrameTime();
void initializeLibrary();
void destroyLibrary();
void initializeGraphics();
void destroyGraphics();
void render();


}