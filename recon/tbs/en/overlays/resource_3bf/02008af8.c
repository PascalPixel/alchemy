/* Draft of SelectQuaternarySceneData, resource_3bf at 0x02008af8, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM loads the scene numbers 0xa0-0xa2 from its
 * literal pool, as link-time values; as C constants GCC compares immediates.
 * The listing keeps these rows. */
#include "FORTRESS.H"

extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];

s32 SelectQuaternarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == 0xa0) {
        return (s32)Data_0200eff4;
    }
    if (scene_variant == 0xa1) {
        return (s32)Data_0200f258;
    }
    if (scene_variant == 0xa2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}
