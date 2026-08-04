#include "types.h"

struct Actor02001060 {
    u8 head[12];
    s32 rank;
    u8 body[19];
    u8 flags;
};

extern struct Actor02001060 *Func_02002570(s32);

s32 Func_02001060(struct Actor02001060 *actor)
{
    if (Func_02002570(0)->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}
