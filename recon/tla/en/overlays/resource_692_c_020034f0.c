/* Near miss of Func_020034f0 of overlay 692 (34 instructions, 4 differ): in the two calls of
 * the first branch the native loads the first argument's zero last (r1, r2, r3, r0) and second
 * (r1, r0, r2, r3); direct calls and Call4 forwarding load it first both times. */
#include "local.h"

extern char SceneId_Ov692Room3;

void Func_02000694(s32 a, s32 b, s32 c, s32 d);

void Func_020034f0(void)
{
    GameFlag_SetBit(0x301);
    if (gPartyState.scene == (s32)&SceneId_Ov692Room3) {
        Func_02000694(0, 24, 15, 0);
        Func_02000694(0, 9, 16, 0);
        Object_SetModeById(11, 4);
    } else {
        Func_02000694(0, 26, 14, 0);
    }
}
