#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0800c300(void *object, s32 argument);

s32 Func_0800d8c4(void *arg0) {
    Func_0800c300(arg0, M2C_FIELD(((M2C_FIELD(arg0, s16 *, 4) * 4) + M2C_FIELD(arg0, s32 *, 0)), s32 *, 4));
    M2C_FIELD(arg0, s16 *, 4) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 4) + 2);
    return 1;
}
