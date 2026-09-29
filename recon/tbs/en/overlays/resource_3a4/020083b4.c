/* Draft of resource_3a4 0x020083b4 (SceneState_SetByte22bTo3), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: none in its bytes, but the ROM loads scene number 0x4f from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "YAMA.H"

void Func_0200406e(s32, s32);
extern u8 Value_0000004f;
void Func_02004066(s32, s32);

void SceneState_SetByte22bTo3(void)
{
    extern u8 Data_02000240[];
    extern s32 Data_03001e40;

    Data_02000240[0x22b] = 3;
    Func_0200406e((s32)&Value_0000004f, 99);
    Func_02004066(53, 2);
}
