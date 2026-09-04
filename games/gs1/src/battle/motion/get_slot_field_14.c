#include "types.h"

#define BattleMotion_GetSlotField14 Func_080b6cd0

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *GetBattleObjectSlot();

s32 BattleMotion_GetSlotField14(void) {
    return FIELD_AT_OFFSET(GetBattleObjectSlot(), s32 *, 0x14);
}
