/* Draft of resource_3a4 0x020081c8 (SceneData_SelectTableC80cOrC83c), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x55 and 0x56 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "YAMA.H"

extern u8 Value_00000055;
extern u8 Data_0200c80c[];
extern u8 Value_00000056;
extern u8 Data_0200c83c[];

s32 SceneData_SelectTableC80cOrC83c(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c80c;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c83c;
    }
    return 0;
}
