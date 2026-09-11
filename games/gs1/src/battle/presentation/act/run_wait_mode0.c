#include "types.h"

void Battle_WaitMode0(s32 arg0);
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */
s32 Func_08092f84();

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    Func_08092f84();
    Battle_WaitMode0(value);
}
