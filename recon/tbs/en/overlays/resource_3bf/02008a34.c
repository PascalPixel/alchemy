/* Draft of SelectSecondarySceneData, resource_3bf at 0x02008a34, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM loads the scene numbers 0xa1-0xa3 from its
 * literal pool, as link-time values; as C constants GCC compares immediates.
 * The listing keeps these rows. */
#include "FORTRESS.H"

extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];

s32 SelectSecondarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == 0xa1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == 0xa2 || scene_variant == 0xa3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}
