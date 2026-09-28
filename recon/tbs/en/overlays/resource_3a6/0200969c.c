/* Draft of resource_3a6 0x0200969c, from
 * games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/STAGED_PAIR_REGION.C.
 * Remaining difference: the ROM loads scenes 0x5d-0x5f from the literal
 * pool, as link-time scene symbols would; C builds those constants with
 * movs. The Value_ spellings below are the old address-named forms. The
 * listing keeps these rows. */
#include "HAIDIA.H"
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000d80[];
    extern u8 SecondaryOverlayData_02000d80[];
    extern u8 TertiaryOverlayData_02000d80[];
    extern u8 DefaultOverlayData_02000d80[];

    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_0000005d) {
        return (s32)PrimaryOverlayData_02000d80;
    }
    if (selector == (s32)&Value_0000005e) {
        return (s32)SecondaryOverlayData_02000d80;
    }
    if (selector == (s32)&Value_0000005f) {
        return (s32)TertiaryOverlayData_02000d80;
    }
    return (s32)DefaultOverlayData_02000d80;
}

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */
