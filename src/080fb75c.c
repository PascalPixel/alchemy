#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u8 *Func_080fb75c(s32 arg0, void *arg1) {
    u8 *cursor;
    u8 value;

    cursor = M2C_FIELD(arg1, u8 **, 0x40);
    value = *cursor;
    M2C_FIELD(arg1, u8 *, 0x1F) = value;
    cursor++;
    M2C_FIELD(arg1, u8 **, 0x40) = cursor;
    return cursor;
}
