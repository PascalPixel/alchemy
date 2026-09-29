/* Draft of resource_3c5 0x02008f58..0x02008fac (84 bytes with pool), the
 * entrances selector; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with 0xb0, 0xaf and 0xae loaded from its
 * literal pool, link-time values; integer scenes are immediates. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"


/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Return this overlay's state block. */
s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0xb0) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == 0xaf) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == 0xae) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}
