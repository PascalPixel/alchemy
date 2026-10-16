#ifndef ALCHEMY_LAYOUT_GUARD_H
#define ALCHEMY_LAYOUT_GUARD_H

#include "types.h"

#ifdef MODERN
// Keep cross-structure comparisons constant expressions in the modern build.
#define LAYOUT_OFFSET(type, field) __builtin_offsetof(type, field)
#else
#define LAYOUT_OFFSET(type, field) ((u32)&(((type *)0)->field))
#endif
#define LAYOUT_OFFSET_GUARD(name, type, field, expected)                       \
  typedef char name[LAYOUT_OFFSET(type, field) == (expected) ? 1 : -1]
#define LAYOUT_SIZE_GUARD(name, type, expected)                                \
  typedef char name[sizeof(type) == (expected) ? 1 : -1]

#endif
