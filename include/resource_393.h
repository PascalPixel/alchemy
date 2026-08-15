#ifndef ALCHEMY_RESOURCE_393_H
#define ALCHEMY_RESOURCE_393_H

#include "types.h"

/* Object position view used by the resource_393 coordinate wrapper. */
struct Resource393Object {
    u8 unknown_00[8];
    u32 position_x;
    s32 position_y;
    u32 position_z;
};

struct Resource393Work {
    u8 unknown_000[0x1f4];
    s32 object_id;
};

struct Resource393Position {
    u32 x;
    s32 y;
    u32 z;
};

extern struct Resource393Work Data_02000240;
extern struct Resource393Object *Func_02001a38(s32 object_id);
extern void Func_0200168a(struct Resource393Position *position);

#endif
