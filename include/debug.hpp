/*
 * debug.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_


#include <cstdio>
#include <exception>

#define DEBUG

#ifndef __GNUC__
    #error GCC Required
#endif

#ifdef DEBUG
    #define DEBUG_PRINT_AVAILABLE 1
    #include <string.h>
    #ifdef __MINGW32__
        #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #else
        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
    #endif
#else
    #define DEBUG_PRINT_AVAILABLE 0
    #define __FILENAME__
#endif

#define DEBUG_PRINT(fmt, ...) \
        do { if (DEBUG_PRINT_AVAILABLE) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILENAME__,   \
                                                __LINE__, __func__, ##__VA_ARGS__); } while (0)


#endif /* DEBUG_HPP_ */
