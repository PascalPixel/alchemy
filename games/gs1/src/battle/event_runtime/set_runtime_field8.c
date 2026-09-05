#include "types.h"
#include "global_cells.h"

#define BattleEvent_SetRuntimeField8 Func_080bb8d8

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEvent_SetRuntimeField8(void) {
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}
