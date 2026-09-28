/* Draft of resource_399 0x02008130 (SceneData_SelectTableByWord224), built with
 * games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMIRU.H.
 * Remaining difference: none in its bytes, but the ROM loads scene number 0x33 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "IMIRU.H"

extern u8 Value_00000033;

/*
 * Scene-script selection for resource_399: read the scenario id from the
 * shared table, branch on it and on story flag 0x881, and return the chosen
 * in-image script block.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_00000033) {
        return (s32)Data_0200a8a0;
    }
    return (s32)Data_0200a798;
}
