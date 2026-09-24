#include "TYPES.H"

void Battle_WaitMode0(s32 arg0);
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */
s32 BattleEv_RunWait();

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    BattleEv_RunWait();
    Battle_WaitMode0(value);
}
