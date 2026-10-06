#include "TYPES.H"
#include "TRANSFORM.H"
#include "SCENE.H"
#include "IO_REG.H"

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

void Graphics_SetBlendControl(void)
{
    REG_BLDCNT = 0xbf;
}

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;

    CAMELOT_ASM("the fixed-register identity store of TRANSFORM.H")
    Transform_SetIdentity(entries);
    entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
    return value / 16;
}
