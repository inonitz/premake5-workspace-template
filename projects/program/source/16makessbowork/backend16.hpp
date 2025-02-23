#pragma once
#include <util/base_type.h>
#include <util/time.hpp>


namespace ssbowork {

u8               getContextID();
Time::Timestamp& getFrameTime();
Time::Timestamp& getRenderTime();
Time::Timestamp& getTimer0();
Time::Timestamp& getTimer1();
bool             getSlowRenderFlag();
void initializeLibrary();
void destroyLibrary();
void initializeGraphics();
void destroyGraphics();
void render();

}