#include "types.h"
#include "scene.h"
#include "abi/battle/effects/member_burst/run_mode0.h"

void BattleFx_RunMemberBurstMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
