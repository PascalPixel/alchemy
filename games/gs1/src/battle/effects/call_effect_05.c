#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080999f0();

void BattleEffect_CallEffect05(void) {
    Func_080999f0();
}
