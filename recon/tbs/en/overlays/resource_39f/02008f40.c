/* Draft of resource_39f 0x02008f40 (SceneData_SelectDataByRuntimeSelector), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x44-0x46 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "MORI.H"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000f40[];
    extern u8 SecondaryOverlayData_02000f40[];
    extern u8 TertiaryOverlayData_02000f40[];
    extern u8 DefaultOverlayData_02000f40[];

    s16 selector = gGameState.scene;

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000f40;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000f40;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000f40;
    }
    return (s32)DefaultOverlayData_02000f40;
}
