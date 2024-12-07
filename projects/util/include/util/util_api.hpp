#ifndef _UTIL_API_H_
#define _UTIL_API_H_


#ifdef UTIL_STATIC_DEFINE /* Define if compiling as a static library */
#  define UTIL_API
#  define UTIL_NO_EXPORT
#else
#  ifndef UTIL_API
#    ifdef UTIL_EXPORTS /* Windows Only!!! IF YOURE ON LINUX DONT DEFINE THIS */
/* We are building this library */
#      define UTIL_API __declspec(dllexport)
#    else
/* We are using this library */
#      define UTIL_API __declspec(dllimport)
#    endif
#  else
	define UTIL_API
#  endif

#  ifndef UTIL_NO_EXPORT
#    define UTIL_NO_EXPORT 
#  endif
#endif

#ifndef UTIL_DEPRECATED
#  define UTIL_DEPRECATED __declspec(deprecated)
#endif

#ifndef UTIL_DEPRECATED_EXPORT
#  define UTIL_DEPRECATED_EXPORT UTIL_API UTIL_DEPRECATED
#endif

#ifndef UTIL_DEPRECATED_NO_EXPORT
#  define UTIL_DEPRECATED_NO_EXPORT UTIL_NO_EXPORT UTIL_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef UTIL_NO_DEPRECATED
#    define UTIL_NO_DEPRECATED
#  endif
#endif

#endif /* _UTIL_API_H_ */