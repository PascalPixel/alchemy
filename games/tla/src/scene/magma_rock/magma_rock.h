#ifndef TLA_MAGMA_ROCK_H
#define TLA_MAGMA_ROCK_H

#include "types.h"

struct ObjectRuntime;

struct MagmaRockWork {
    s32 unknown_00;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_10;
    struct ObjectRuntime *target;
};

#endif
