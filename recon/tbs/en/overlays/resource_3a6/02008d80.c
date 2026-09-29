/* Draft of resource_3a6 0x02008d80, from
 * games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/STAGED_PAIR_REGION.C.
 * Remaining difference: the ROM loads scenes 0x5d-0x5f from the literal
 * pool, as link-time scene symbols would; C builds those constants with
 * movs. The Value_ spellings below are the old address-named forms. The
 * listing keeps these rows. */
#include "HAIDIA.H"
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;

s32 SelectSceneDataByState(void)
{
    s16 state = gGameState.scene;

    if (state == (s32)&Value_0000005d) {
        return (s32)Data_0200a234;
    }
    if (state == (s32)&Value_0000005e) {
        return (s32)Data_0200a2c4;
    }
    if (state == (s32)&Value_0000005f) {
        return (s32)Data_0200a39c;
    }
    return (s32)Data_0200a234;
}

/* Contiguous unnamed leaf-owner run for resource_3a6. */

/* Configure the 16x15 scene rectangle at row 15. */
