#include "types.h"

#define SceneActor_IsActorAtTile Func_02001be8
#define NULL ((void *)0)

s32 *Func_0200405c(s32);

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z) {
    s32 *p = Func_0200405c(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}
