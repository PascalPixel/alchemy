#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "io_reg.h"


void BlendTransition_Update(void)
{
    if (gBlendDuration != 0) {
        {
            volatile u16 *blend_control;
            u32 control;

            if (gBlendBrighten != 0) {
                control = gBlendLayers | 0x80;
                blend_control = &REG_BLDCNT;
            } else {
                control = gBlendLayers | 0xc0;
                blend_control = &REG_BLDCNT;
            }
            *blend_control = control;
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
            level += __divsi3(delta * step, gBlendDuration);
            REG_BLDY = level;
            if (*remaining == 0)
                gBlendDuration = 0;
        }
    }
}
