#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080d2464(s32, s32);

void Func_080d2458(s32 arg0) {
    Func_080d2464(arg0, 0);
}
