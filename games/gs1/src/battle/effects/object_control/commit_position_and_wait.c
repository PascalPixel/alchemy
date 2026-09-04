#include "types.h"

#define BattleEffect_CommitObjectPositionAndWait Func_08093530

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Object_CommitPosition(s32);
void BattleRuntime_WaitIfModeZero(s32 arg0);

void BattleEffect_CommitObjectPositionAndWait(void)
{
    Object_CommitPosition(FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0));
    BattleRuntime_WaitIfModeZero(2);
}
