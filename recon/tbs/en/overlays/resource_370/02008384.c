/* resource_370 0x02008384..0x020083cc Scene_GetModeMask (72 bytes with pool),
 * formerly MENU/CLEAR/MODE_MASK.C; the listing keeps the rows. Compiles
 * exactly with this spelling. Remaining difference: it xors the mode with 2
 * loaded from its literal pool, as a link-time symbol does, which this draft
 * spells as the equate Value_00000002; a plain 2 compiles to movs.
 * Func_020017e0 is the overlay's flag-test import veneer. */
#include "TYPES.H"

extern s16 gGameState[];
extern u8 Value_00000002;

s32 Func_020017e0();

s32 Scene_GetModeMask(void)
{
    s32 mode;
    s32 normalized;

    /* The predicate is meaningful only after flag 324 is set. */
    if (Func_020017e0(324) == 0) {
        return 0;
    }
    if (gGameState[287] == 2) {
        return 0;
    }

    /* Return 0 for mode 2 and -1 for every other mode. */
    mode = gGameState[224] ^ (s32)&Value_00000002;
    normalized = (unsigned int)(-mode | mode) >> 31;
    return -normalized;
}
