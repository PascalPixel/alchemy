#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern struct Actor *Func_02004bb2(s32);

s32 Func_02001a98(u8 *owner)
{
    s16 *identifier = (s16 *)(owner + 100);
    struct Actor *actor = Func_02004bb2(*identifier);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}
