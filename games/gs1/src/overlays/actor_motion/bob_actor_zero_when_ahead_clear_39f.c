#include "types.h"

#define SceneActor_BobActorZeroWhenAheadClear Func_02001078

extern s32 *Func_02003e40(s32 a);
extern s32 Func_02001d82(s32 *actor);
extern void Func_02003e5e(void);
extern void Func_02003ec2(s32 a, s32 b);
extern void Func_02003ec2_b(void);
extern void Func_02003dc6(s32 a);
extern void Func_02003dd8(s32 a);
extern void Func_02003dee(s32 a);

void SceneActor_BobActorZeroWhenAheadClear(void) {
    s32 pos[3];
    s32 *actor = Func_02003e40(0);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (Func_02001d82(pos) != 0) {
        Func_02003e5e();
        *fp = 0;
        Func_02003ec2(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dc6(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dd8(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        Func_02003dee(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Func_02003ec2_b();
    }
}
