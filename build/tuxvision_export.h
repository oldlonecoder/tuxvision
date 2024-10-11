
#ifndef TUXVISION_EXPORT_H
#define TUXVISION_EXPORT_H

#ifdef TUXVISION_STATIC_DEFINE
#  define TUXVISION_EXPORT
#  define TUXVISION_NO_EXPORT
#else
#  ifndef TUXVISION_EXPORT
#    ifdef tuxvision_EXPORTS
        /* We are building this library */
#      define TUXVISION_EXPORT 
#    else
        /* We are using this library */
#      define TUXVISION_EXPORT 
#    endif
#  endif

#  ifndef TUXVISION_NO_EXPORT
#    define TUXVISION_NO_EXPORT 
#  endif
#endif

#ifndef TUXVISION_DEPRECATED
#  define TUXVISION_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef TUXVISION_DEPRECATED_EXPORT
#  define TUXVISION_DEPRECATED_EXPORT TUXVISION_EXPORT TUXVISION_DEPRECATED
#endif

#ifndef TUXVISION_DEPRECATED_NO_EXPORT
#  define TUXVISION_DEPRECATED_NO_EXPORT TUXVISION_NO_EXPORT TUXVISION_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TUXVISION_NO_DEPRECATED
#    define TUXVISION_NO_DEPRECATED
#  endif
#endif

#endif /* TUXVISION_EXPORT_H */
