/* Draft of resource_38a 0x020083e4, from
 * games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/BURST.C.
 * Remaining difference: the ROM loads scene 0x1d (or 0x1c) from the
 * literal pool, as a link-time scene symbol would; C builds the constant
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "GOMA.H"

s32 SceneData_GetTable8990OrTable89f0(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020089f0;
    }
    return (s32)Data_02008990;
}

