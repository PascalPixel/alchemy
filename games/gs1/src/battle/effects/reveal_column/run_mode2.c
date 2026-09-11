#include "types.h"
#include "scene.h"
#include "abi/battle/effects/reveal_column/run_mode2.h"

void BattleFx_RunRevealColumnMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}
