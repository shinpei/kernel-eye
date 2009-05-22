#ifndef __EYE_DEBUG_H
#define __EYE_DEBUG_H

#include <stdarg.h>

#define DEBUG_KEYE 1

#ifndef DEBUG_KEYE
#define KEYE_DEBUG(fmt, ...) \
  printk("[%s:%d](%s)", __FILE__, __LINE__, __FUNCTION__); \
  printk(fmt, ##__VA_ARGS__); \
  printk("\n")

#else

#define KEYE_DEBUG(fmt, ...) ;

#endif

#ifndef DEBUG_KEYE
#define KEYE_ASSERT(stmt) \
  if (!(stmt)) { \
  printk("[%s:%d](%s)", __FILE__, __LINE__, __FUNCTION__); \
  printk("assertion failed!\n");


#else
#define KEYE_ASSERT(stmt) ;

#endif

#endif   /* __EYE_DEBUG_H */
