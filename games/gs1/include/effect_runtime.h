#ifndef GUARD_EFFECT_RUNTIME_H
#define GUARD_EFFECT_RUNTIME_H

#include "types.h"
#include "global_cells.h"

struct EffectRuntimeSlot {
    void *object;
    u8 active;
    u8 padding5;
    u8 tileX;
    u8 tileY;
};

struct EffectRuntime {
    u8 padding0[0x11C];
    struct EffectRuntimeSlot slots[10];
};

#define EFFECT_RUNTIME (*(struct EffectRuntime **)ADDR_03001EBC)

#endif
