#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02003edc(s32);
extern void Func_02001aba(s32 *);

void Func_02000dc8(void)
{
    s32 point[3];
    struct Actor *actor = Func_02003edc(0);

    point[0] = actor->f08;
    point[1] = actor->f0c;
    point[2] = actor->f10 + 0x200000;
    Func_02001aba(point);
}
