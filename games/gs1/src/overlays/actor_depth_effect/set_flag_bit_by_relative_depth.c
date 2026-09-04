#include "types.h"

#define SceneActor_SetFlagBitByRelativeDepth Func_02000ec8
struct Actor { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };
struct Actor *Func_02003fdc();
s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor *actor)
{
    struct Actor *ref;
    u8 *fp;
    u8 flag;
    ref = Func_02003fdc(0);
    fp = &actor->flags23;
    flag = *fp | 2;
    *fp = flag;
    if (ref->z < actor->z) {
        s32 lim = actor->z - ref->z;
        s32 ay;
        lim += 0x00040000;
        ay = actor->y;
        ay += lim;
        if (ref->y <= ay) {
            flag &= 0xfd;
            *fp = flag;
        }
    }
    return 0;
}
