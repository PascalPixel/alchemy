#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080b7dd0();

s32 Func_080b6cd0(void) {
    return M2C_FIELD(Func_080b7dd0(), s32 *, 0x14);
}
