#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/act/run_wait_mode0.h"

void Battle_WaitMode0(s32 arg0);
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    Battle_Check();
    Battle_WaitMode0(value);
}
