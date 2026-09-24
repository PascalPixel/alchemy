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

    /* FAKEMATCH: the do-while keeps the cursor store in source order */
    do {
        work->cursor = result;
    } while (0);
    if (Func_080a77a4(0) == -1)
        result = -1;
    else
        result = work->choice;
    return result;
}
