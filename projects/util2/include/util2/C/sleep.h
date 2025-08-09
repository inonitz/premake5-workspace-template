#ifndef __UTIL2_C_DEFINITION_MACRO_SLEEP__
#define __UTIL2_C_DEFINITION_MACRO_SLEEP__
#include <stdint.h>
#include "util2_api.h"


void microsleep(uint32_t microseconds);
void millisleep(uint32_t milliseconds);
uint64_t nowms();


#endif /* __UTIL2_C_DEFINITION_MACRO_SLEEP__ */