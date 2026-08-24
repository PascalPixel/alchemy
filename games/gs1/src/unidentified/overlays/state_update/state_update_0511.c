#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_0200445c(s32);
extern void Func_02002038(s32 *);

void Func_02001348(void)
{
    s32 point[3];
    struct Actor *actor = Func_0200445c(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Func_02002038(point);
}
