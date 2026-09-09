#include "types.h"

#define NULL ((void *)0)
#define SceneActor_UseActorTenCellAndNext Func_02001a84
#define SceneActor_ApplyActorTwelveZCellPair Func_02001af4
#define SceneActor_IsActorAtTile Func_02001be8

/* Scene-record accessor. */
extern u8 *Func_02003ef8(s32 index);
/* Old-style: the interface is not established, and the call site passes five
 * arguments. */
void Func_020035e6();
s32 *Func_02003f68(s32);
void Func_02003656(s32, s32, s32, s32, s32);
s32 *Func_0200405c(s32);

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void SceneActor_UseActorTenCellAndNext(void)
{
    u8 *record = Func_02003ef8(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Func_020035e6(10, 13, cell + 1, 13, cell);
}

void SceneActor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Func_02003f68(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_02003656(12, 22, v + 1, 22, v);
    }
}

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Func_0200405c(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}
