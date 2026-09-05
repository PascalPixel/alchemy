#include "types.h"

#define SceneActor_PassRaisedPointOfActorZero Func_02000dc8

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02003edc(s32);
extern void Func_02001aba(s32 *);

void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor *p = Func_02003edc(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    Func_02001aba(pos);
}
