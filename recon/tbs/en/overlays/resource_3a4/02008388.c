/* Draft of resource_3a4 0x02008388 (SceneState_SetWorkByte22bTo3), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: none in its bytes, but the ROM loads scene number 0x4d from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "YAMA.H"

void Func_02004042(s32, s32);
extern u8 Value_0000004d;
void Func_0200403a(s32, s32);

void SceneState_SetWorkByte22bTo3(void)
{
    extern u8 Data_02000240[];
    extern s32 Data_03001e40;

    Data_02000240[0x22b] = 3;
    Func_02004042((s32)&Value_0000004d, 99);
    Func_0200403a(53, 2);
}
