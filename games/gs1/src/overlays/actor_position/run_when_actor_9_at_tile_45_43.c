#include "types.h"

#define SceneActor_RunWhenActor9AtTile45x43 Func_02001c28

extern s32 *Func_0200416e(s32);
extern void Func_02003816(void);

void SceneActor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Func_0200416e(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        Func_02003816();
    }
}
