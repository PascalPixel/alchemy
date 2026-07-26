#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32, s32);

s32 Func_08093554(void) {
    return M2C_FIELD(Func_080048f4(0x1B, 0xCCC), s32 *, 0x1E0);
}
