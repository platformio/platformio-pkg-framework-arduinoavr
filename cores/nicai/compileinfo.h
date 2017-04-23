#include <avr/io.h>
// #include "version.h"
#define VERSION "3.6.0"
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define COMPILEINFO "<compileinfo avr-gcc='" __VERSION__ "' avrlibc='" __AVR_LIBC_VERSION_STRING__ "' robolib='" VERSION "' />"
#if GCC_VERSION > 40400
#pragma message (COMPILEINFO)
#endif
