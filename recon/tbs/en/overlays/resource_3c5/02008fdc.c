/* Draft of resource_3c5 0x02008fdc..0x02009030 (84 bytes with pool), the
 * placements selector; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with 0xb0, 0xaf and 0xae loaded from its
 * literal pool, link-time values; integer scenes are immediates. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000fdc[];
    extern u8 SecondaryOverlayData_02000fdc[];
    extern u8 TertiaryOverlayData_02000fdc[];
    extern u8 DefaultOverlayData_02000fdc[];

    s16 selector = gGameState.scene;

    if (selector == 0xb0) {
        return (s32)PrimaryOverlayData_02000fdc;
    }
    if (selector == 0xaf) {
        return (s32)SecondaryOverlayData_02000fdc;
    }
    if (selector == 0xae) {
        return (s32)TertiaryOverlayData_02000fdc;
    }
    return (s32)DefaultOverlayData_02000fdc;
}
