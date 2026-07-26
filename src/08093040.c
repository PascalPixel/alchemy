#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_0809163c(s32 arg0);
s32 Func_08092f84();

void Func_08093040(s32 arg0, s32 arg1, s32 arg2) {
    Func_08092f84();
    Func_0809163c(arg2);
}
