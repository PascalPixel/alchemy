/* Draft of resource_3ce 0x020081d8 (SceneState_ApplyBlockDa2), built with
 * games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/MENU_TEST.H.
 * Remaining difference: none in its bytes, but the ROM loads message 0xda2 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "MENU_TEST.H"

extern u8 Value_00000da2;
s32 Func_02000260();

void SceneState_ApplyBlockDa2(void)
{
    Func_02000260((s32)&Value_00000da2, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
