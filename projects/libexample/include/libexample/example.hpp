#ifndef __LIBEXAMPLE_DEFINITION_HEADER__
#define __LIBEXAMPLE_DEFINITION_HEADER__
#include "example_api.h"


LIBEXAMPLE_API int   example_library_init();
LIBEXAMPLE_API void  example_library_destroy();
LIBEXAMPLE_API char const* example_library_get_string(); 


#endif