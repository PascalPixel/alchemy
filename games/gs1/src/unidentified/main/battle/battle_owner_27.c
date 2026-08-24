#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *GetBattleObjectSlot();

s32 Func_080b6cd0(void) {
    return M2C_FIELD(GetBattleObjectSlot(), s32 *, 0x14);
}
