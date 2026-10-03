#include "TYPES.H"
#include "TRANSFORM.H"
#include "SCENE.H"

#define REG_BLDCNT (*(volatile u16 *)0x04000050)

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

/*
 * Blend control setup.  The eight-byte owner at 0x080c0ea8 includes its two
 * trailing pool words, 0x000000bf and 0x04000050.
 */

/* Set the blend control bits to 0xbf.  No arguments, no result. */
void Graphics_SetBlendControl(void)
{
    REG_BLDCNT = 0xbf;
}

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(entries);
    entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
    return value / 16;
}
