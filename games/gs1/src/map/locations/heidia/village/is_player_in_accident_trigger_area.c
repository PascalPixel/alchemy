#include "types.h"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

SceneActor *Func_0200675e(s32);

#define IsPlayerInAccidentTriggerArea Func_02001108
s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Func_0200675e(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}
