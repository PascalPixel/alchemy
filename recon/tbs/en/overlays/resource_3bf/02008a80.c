/* Draft of SelectTertiarySceneData, resource_3bf at 0x02008a80, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM loads the scene numbers 0x6a and 0xa0-0xa3
 * from its literal pool, as link-time values; as C constants GCC compares
 * immediates. The listing keeps these rows. */
#include "FORTRESS.H"

extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];

s32 SelectTertiarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == 0x6a) {
        return (s32)Data_0200e9d0;
    }
    if (scene_variant == 0xa2) {
        return (s32)Data_0200ee08;
    }
    if (scene_variant == 0xa1) {
        return (s32)Data_0200ec28;
    }
    if (scene_variant == 0xa0) {
        return (s32)Data_0200eac0;
    }
    if (scene_variant == 0xa3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}
