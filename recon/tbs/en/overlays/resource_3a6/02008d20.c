/* Draft of resource_3a6 0x02008d20, from
 * games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/STAGED_PAIR_REGION.C.
 * Remaining difference: the ROM loads scenes 0x5d-0x5f from the literal
 * pool, as link-time scene symbols would; C builds those constants with
 * movs. The Value_ spellings below are the old address-named forms. The
 * listing keeps these rows. */
#include "HAIDIA.H"
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;

s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_0000005d) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&Value_0000005e) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&Value_0000005f) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

/* Contiguous unnamed leaf-owner run for resource_3a6. */
