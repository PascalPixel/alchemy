#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32 arg0, s32 arg1);
s32 Func_08009158(s32);
void Func_0809163c(s32 arg0);

void Func_08093530(void) {
    Func_08009158(M2C_FIELD(Func_080048f4(0x1B, 0xCCC), s32 *, 0x1E0));
    Func_0809163c(2);
}
