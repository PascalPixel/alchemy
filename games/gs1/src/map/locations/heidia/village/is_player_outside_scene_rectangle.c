#include "types.h"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

SceneActor *Func_02006cb6(s32);

#define IsPlayerOutsideSceneRectangle Func_02001660
s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Func_02006cb6(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}
