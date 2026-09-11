#include "types.h"
#include "scene.h"
#include "abi/battle/effects/member_burst/run_mode1.h"

void BattleFx_RunMemberBurstMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
