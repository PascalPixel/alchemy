/* Draft of resource_3c5 0x0200a7a0..0x0200a7f4 (84 bytes with pool), the
 * events selector; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with 0xb0, 0xaf and 0xae loaded from its
 * literal pool, link-time values; integer scenes are immediates. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

s32 SceneData_SelectTableB91cByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_020027a0[];
    extern u8 SecondaryOverlayData_020027a0[];
    extern u8 TertiaryOverlayData_020027a0[];
    extern u8 DefaultOverlayData_020027a0[];

    s16 selector = gGameState.scene;

    if (selector == 0xb0) {
        return (s32)PrimaryOverlayData_020027a0;
    }
    if (selector == 0xaf) {
        return (s32)SecondaryOverlayData_020027a0;
    }
    if (selector == 0xae) {
        return (s32)TertiaryOverlayData_020027a0;
    }
    return (s32)DefaultOverlayData_020027a0;
}
