#include "types.h"

/*
 * Complete actor-13 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/assets/code/resource_385_overlay.s (lines 757-769, still unadopted at the
 * time of writing): `bl sub_020016da`, `bl sub_020016d4`, `bl sub_02000a36`.
 */
void Func_020016da(s32 dialogue);
void Func_020016d4(s32 actor, s32 mode, s32 value);
void Func_02000a36(s32 actor);

void SceneDialogue_ShowLine1CB0ForActor13(void)
{
    Func_020016da(0x1cb0);
    Func_020016d4(13, 0, 2);
    Func_02000a36(13);
}
