/* Draft, not exact (2026-09-24): candidate=56 reference=56 differing_halfwords=4.
   The tail, registers and pool match. Residual: the scheduler hoists the
   0x174 offset (movs/lsls) above the load of the work pointer and the zero;
   the reference keeps source order (ldr r5; movs r2, #0; movs r1, #186).
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
    s32 result = 0;

    work->cursor = result;
    if (Func_080a77a4(0) == -1)
        result = -1;
    else
        result = work->choice;
    return result;
}
