#ifndef _LIBEXAMPLE_API_H_
#define _LIBEXAMPLE_API_H_


#ifdef LIBEXAMPLE_STATIC_DEFINE /* Define if compiling as a static library (-DLIBEXAMPLE_STATIC_DEFINE) */
#	define LIBEXAMPLE_API
#	define LIBEXAMPLE_NO_EXPORT
#else
#	ifndef LIBEXAMPLE_API
#		ifdef LIBEXAMPLE_EXPORTS /* We are building this library */
#			if defined _WIN32 || defined _WIN64 || defined __CYGWIN__ || defined __MINGW64__
#				ifdef __GNUC__
#					define LIBEXAMPLE_API __attribute__ ((dllexport))
#				else
#					define LIBEXAMPLE_API __declspec(dllexport)
#				endif
#			else 
#				if defined __GNUC__ && __GNUC__ >= 4
#					define LIBEXAMPLE_API __attribute__ ((visibility ("default")))
#				endif
#			endif
#    	else /* We are using this library */
#			if defined _WIN32 || defined _WIN64 || defined __CYGWIN__ || defined __MINGW64__
#				ifdef __GNUC__
#					define LIBEXAMPLE_API __attribute__ ((dllimport))
#				else
#					define LIBEXAMPLE_API __declspec(dllimport)
#				endif
#			else 
#				if defined __GNUC__ && __GNUC__ >= 4
#					define LIBEXAMPLE_API
#				endif
#			endif
#       endif
#	else /* Should Only reach here for non-*nix, un-supported platforms */
#		define LIBEXAMPLE_API
#   endif
#	ifndef LIBEXAMPLE_NO_EXPORT
#       if defined __GNUC__ && __GNUC__ >= 4 /* Symbols exported by default on *nix systems */
#           define LIBEXAMPLE_NO_EXPORT __attribute__((visibility ("hidden")))
#       else /* (DLL) Symbols on platforms like windows must be exported manually [__declspec(dllexport)] */
#		    define LIBEXAMPLE_NO_EXPORT 
#       endif
#	endif
#endif


#ifndef LIBEXAMPLE_DEPRECATED
#   if defined(__cplusplus)
#       if __cplusplus > 201402L /* [[deprecated]] Supported since C++14 */
#           define LIBEXAMPLE_DEPRECATED [[deprecated]]
#           define LIBEXAMPLE_DEPRECATED_MSG(MSG) [[deprecated(MSG)]]
#       endif
#   else
#       if defined _WIN32 || defined _WIN64
#           if defined __GNUC__ || defined __clang__ /* Cygwin, MinGW32/64 */
#               define LIBEXAMPLE_DEPRECATED          __attribute__((deprecated))
#               define LIBEXAMPLE_DEPRECATED_MSG(MSG) __attribute__((deprecated(MSG)))
#           else
#               define LIBEXAMPLE_DEPRECATED          __declspec(deprecated)
#               define LIBEXAMPLE_DEPRECATED_MSG(MSG) __declspec(deprecated(MSG))
#           endif
#       elif defined __GNUC__ || defined __clang__
#           define LIBEXAMPLE_DEPRECATED __attribute__((deprecated))
#           define LIBEXAMPLE_DEPRECATED_MSG(MSG) __declspec(deprecated(MSG))
#       else /* Should Only reach here for non-*nix, un-supported platforms */
#           define LIBEXAMPLE_DEPRECATED
#           define LIBEXAMPLE_DEPRECATED_MSG(MSG)
#       endif
#   endif
#endif


#ifndef LIBEXAMPLE_DEPRECATED_EXPORT
#  define LIBEXAMPLE_DEPRECATED_EXPORT LIBEXAMPLE_API LIBEXAMPLE_DEPRECATED
#endif


#ifndef LIBEXAMPLE_DEPRECATED_NO_EXPORT
#  define LIBEXAMPLE_DEPRECATED_NO_EXPORT LIBEXAMPLE_NO_EXPORT LIBEXAMPLE_DEPRECATED
#endif


/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBEXAMPLE_NO_DEPRECATED
#    define LIBEXAMPLE_NO_DEPRECATED
#  endif
#endif

#endif /* _LIBEXAMPLE_API_H_ */