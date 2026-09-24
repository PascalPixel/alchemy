/* Draft, not exact (2026-09-24): candidate=52 reference=56 differing_halfwords=22.
   Residual: the reference zeroes r2 before forming the 0x174 offset and keeps
   the result in r2 (copied before the -1 compare, moved to r0 at the end).
   The owner was bundled with main:080a7478 until 2026-09-24. */
#include "TYPES.H"

struct CharacterSelectorWork {
    u8 pad0[0x174];
    u16 cursor;
    u8 pad176[0x21a - 0x176];
    u8 choice;
};

s32 Func_080a77a4(s32 mode);

/* Run the character selector and return the chosen slot, or -1 when it was
   cancelled. */
s32 CharacterSelector_Run(void)
{
    struct CharacterSelectorWork *work = *(struct CharacterSelectorWork **)0x03001f2c;
    s32 result;
    s32 zero = 0;

    work->cursor = zero;
    result = Func_080a77a4(0);
    if (result != -1)
        result = work->choice;
    return result;
}
