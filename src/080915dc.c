#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_08091560();
s32 Func_08091584();

u8 Func_080915dc(void) {
    Func_08091584();
    return M2C_FIELD(Func_08091560(), u8 *, 3);
}
