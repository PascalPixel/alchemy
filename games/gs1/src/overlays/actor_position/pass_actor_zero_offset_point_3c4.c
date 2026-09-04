#include "types.h"

#define SceneActor_PassActorZeroOffsetPoint Func_02000df4

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02003f08(s32);
extern void Func_02001ae4(s32 *);

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor *actor = Func_02003f08(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    Func_02001ae4(pos);
}
