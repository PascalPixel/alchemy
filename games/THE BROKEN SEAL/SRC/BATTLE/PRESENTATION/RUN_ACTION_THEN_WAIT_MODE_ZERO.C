#include "TYPES.H"

void Func_0809163c(s32 arg0);
#define Battle_WaitMode0 Func_0809163c
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */
s32 Func_08092f84();
#define BattleEv_RunWait Func_08092f84

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    BattleEv_RunWait();
    Battle_WaitMode0(value);
}
