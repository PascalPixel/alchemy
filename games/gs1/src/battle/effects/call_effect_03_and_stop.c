#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080994d0();

void BattleEffect_CallEffect03AndStop(void) {
    Func_080994d0();
    EffectRuntime_StopCurrentObject();
}
