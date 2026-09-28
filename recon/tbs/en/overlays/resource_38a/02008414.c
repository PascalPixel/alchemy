/* Draft of resource_38a 0x02008414, from
 * games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/BURST.C.
 * Remaining difference: the ROM loads scene 0x1d (or 0x1c) from the
 * literal pool, as a link-time scene symbol would; C builds the constant
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "GOMA.H"

s32 FieldScene_PlaceActor8OnEntry(void)
{
    extern u8 Data_02000240[];

    u8 *record;
    u8 *base;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    base = Data_02000240;
    if (*(s16 *)(base + 0x1c0) == (s32)Data_0000001c) {
        if (*(s16 *)(base + 0x1c2) == 5) {
            Call1(Func_02000b5e, 0x12f);
        } else {
            SetFlagBits(Func_02000b6e(8) + 89, 16);
            if (Value1(Func_02000b66, 0x864) != 0) {
                Call3(Func_02000bb0, 8, 0x15a0000, 0x1240000);
                record = Func_02000b96(8);
                Func_02000b64((s32)record, 0);
                *(u8 *)(Func_02000ba2(8) + 35) |= 2;
                Func_02000bdc(8, 2);
                Call6(Func_02000b88, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

