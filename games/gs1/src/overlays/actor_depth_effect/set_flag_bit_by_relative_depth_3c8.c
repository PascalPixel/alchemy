#include "types.h"

struct Actor_020008c8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };
struct Actor_020008c8 *Func_0200577c();
s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_020008c8 *actor)
{
    struct Actor_020008c8 *ref;
    u8 *fp;
    u8 flags;
    ref = Func_0200577c(0);
    fp = &actor->flags23;
    flags = *fp | 2;
    *fp = flags;
    if (ref->z < actor->z) {
        s32 diff = actor->z - ref->z;
        s32 lim;
        diff += 0x00040000;
        lim = actor->y;
        lim += diff;
        if (ref->y <= lim) {
            flags &= 0xfd;
            *fp = flags;
        }
    }
    return 0;
}
