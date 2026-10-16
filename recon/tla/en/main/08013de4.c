/* Draft, differ 6 of 48: The Broken Seal's Fade_Update as it is.
 * Three schedules differ: the native loads &gBlendFramesLeft after the BLDCNT
 * store and &gBlendTargetLevel after the frame count's store, where this
 * hoists both, and finishes BLDY's address after adding the quotient. Barriers
 * after the two stores fix the first two but hoist the duration's address
 * instead (differ 12 to 18). Compiled without -mtune=arm9tdmi (analysis only,
 * not a route) the first two come out as the native's. Natural forms without
 * the blendControl pointer and the explicit __divsi3 are differ 41 to 58. */
//--------------------------------------------------------------------------
// blend.c
// Display: configure white/black brightness fades and wait for completion.
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "io_reg.h"
s32 __divsi3(s32, s32);
void Fade_Update(void)
{
	if (gBlendDuration != 0)
	{
		{
			volatile u16 *blendControl;
			u32			  control;

			if (gBlendBrighten != 0)
			{
				control = gBlendLayers | 0x80;
				blendControl = &REG_BLDCNT;
			}
			else
			{
				control = gBlendLayers | 0xc0;
				blendControl = &REG_BLDCNT;
			}
			*blendControl = control;
		}
		{
			u8 *remaining = &gBlendFramesLeft;
			s32 delta;
			s32 level;
			s32 step;

			(*remaining)--;
			level = gBlendTargetLevel;
			delta = gBlendStartLevel - gBlendTargetLevel;
			step = *remaining;
			/* FAKEMATCH: an explicit libgcc call, which clobbers memory, so the zero test reloads *remaining after it as the native does. With the / operator the division is a libcall block CSE sees through: the test reuses the step's load and the registers shift (differ 37 in all six editions). The operator matches only if every read of *remaining is forced to memory by its qualifier, which trades this call for that device. */
			level += __divsi3(delta * step, gBlendDuration);
			REG_BLDY = level;
			if (*remaining == 0)
				gBlendDuration = 0;
		}
	}
}
