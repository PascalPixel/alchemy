#include "types.h"
#include "scene.h"
#include "abi/battle/effects/reveal_column/run_mode1.h"

void BattleFx_RunRevealColumnMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
