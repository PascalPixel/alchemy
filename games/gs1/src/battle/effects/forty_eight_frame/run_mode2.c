#include "types.h"
#include "scene.h"
#include "abi/battle/effects/forty_eight_frame/run_mode2.h"

void BattleFx_RunFortyEightFrameMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}
