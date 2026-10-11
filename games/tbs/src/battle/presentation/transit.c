#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "transform.h"
#include "scene.h"

void Graphics_FadeAlphaOut(void)
{
    s32 alpha_step;

    REG_BLDCNT = 0x2044;
    for (alpha_step = 1; alpha_step <= 16; alpha_step += 2)
    {
        REG_BLDALPHA = 0x1010 - alpha_step;
        WaitFrames(1);
    }
}

void Graphics_FadeAlphaIn(void)
{
    s32 step;

    REG_BLDCNT = 0x2044;
    for (step = 1; step <= 16; step += 2) {
        REG_BLDALPHA = step + 0x1000;
        WaitFrames(1);
    }
}

struct BattleTransitionEntry
{
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

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(entries);
	entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
	return value / 16;
}
