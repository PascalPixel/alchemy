/* Draft of resource_3a4 0x0200b410 (SceneState_SetWorkspaceHalfword382To1018), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: the ROM loads 0x1018 as a pool word after the work pointer and stores its low half; the C loads the constant with ldrh from the first pool word.
 * The listing keeps these rows. */
#include "YAMA.H"

extern u8 Value_00001018;

void SceneState_SetWorkspaceHalfword382To1018(void)
{
    extern s32 Data_03001e40;

    *(u16 *)(*(u8 **)Data_03001ebc + (191 << 1)) = (int)&Value_00001018;
}
