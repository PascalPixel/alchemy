/* Draft of resource_37f 0x0200807c, from
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.C.
 * Remaining difference: the ROM loads scene or message numbers from the
 * literal pool, as link-time symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "SORU.H"

s32 Func_0200007c(void)
{
    s32 base5_2009fd8;
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000010) {
        if (Data_02000240_t[225][0] >= lo) {
            if (Data_02000240_t[225][0] > 13) {
                if (Data_02000240_t[225][0] > 16) {
                    goto L_020000ae;
                }
                return (s32)Data_0200a1b8;
            }
            return (s32)Data_0200a050;
        }
        L_020000ae:;
        base5_2009fd8 = (s32)Data_02009fd8;
        Func_02001c70(base5_2009fd8);
        return base5_2009fd8;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000013) {
            return (s32)Data_0200a2a8;
        }
    }
    return (s32)Data_02009fc0;
}

