#ifndef ALCHEMY_LAYOUT_GUARD_H
#define ALCHEMY_LAYOUT_GUARD_H

#include "types.h"

#define LAYOUT_OFFSET_GUARD(name, type, field, expected) \
    typedef char name[(u32)&(((type *)0)->field) == (expected) ? 1 : -1]
#define LAYOUT_SIZE_GUARD(name, type, expected) \
    typedef char name[sizeof(type) == (expected) ? 1 : -1]

#endif
