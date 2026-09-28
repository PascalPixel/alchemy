/* Draft of resource_3ce 0x02008180 (SceneState_ApplyBlockD21), built with
 * games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/MENU_TEST.H.
 * Remaining difference: none in its bytes, but the ROM loads message 0xd21 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "MENU_TEST.H"

extern u8 Value_00000d21;
extern u8 Value_00000d4c;
s32 Func_02000206();

void SceneState_ApplyBlockD21(void)
{
    Func_02000206((s32)&Value_00000d21, (s32)&Value_00000d4c - (s32)&Value_00000d21);
}
