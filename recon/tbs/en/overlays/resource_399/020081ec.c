/* Draft of resource_399 0x020081ec (SceneData_SelectTableByWord224B), built with
 * games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMIRU.H.
 * Remaining difference: none in its bytes, but the ROM loads scene number 0x33 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "IMIRU.H"

extern u8 Value_00000033;

s32 SceneData_SelectTableByWord224B(void)
{
    if (gGameState.scene == (s32)&Value_00000033) {
        return (s32)Data_0200adb8;
    }
    return (s32)Data_0200ac80;
}
