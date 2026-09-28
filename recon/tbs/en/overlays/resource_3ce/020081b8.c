/* Draft of resource_3ce 0x020081b8 (SceneState_ApplyBlockD77), built with
 * games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/MENU_TEST.H.
 * Remaining difference: none in its bytes, but the ROM loads message 0xd77 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "MENU_TEST.H"

extern u8 Value_00000d77;
s32 Func_02000240();

void SceneState_ApplyBlockD77(void)
{
    Func_02000240((s32)&Value_00000d77, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
