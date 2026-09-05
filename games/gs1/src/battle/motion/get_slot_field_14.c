#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *GetBattleObjectSlot();

s32 BattleMotion_GetSlotField14(void) {
    return FIELD_AT_OFFSET(GetBattleObjectSlot(), s32 *, 0x14);
}
