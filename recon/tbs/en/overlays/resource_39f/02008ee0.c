/* Draft of resource_39f 0x02008ee0 (SceneData_SelectByRuntimeSelector), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x44-0x46 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "MORI.H"

s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}
