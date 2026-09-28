/* Draft of resource_39f 0x0200a4ac (SceneData_SelectTableBa48ByRuntimeSelector), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x44-0x46 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "MORI.H"

s32 SceneData_SelectTableBa48ByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_020024ac[];
    extern u8 SecondaryOverlayData_020024ac[];
    extern u8 TertiaryOverlayData_020024ac[];
    extern u8 DefaultOverlayData_020024ac[];

    s16 selector = gGameState.scene;

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_020024ac;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_020024ac;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_020024ac;
    }
    return (s32)DefaultOverlayData_020024ac;
}
