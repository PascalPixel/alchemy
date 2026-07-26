#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800b684(s32);

void Func_0800c598(void *arg0) {
    if ((arg0 != NULL) && (M2C_FIELD(arg0, u8 *, 0x54) == 1)) {
        Func_0800b684(M2C_FIELD(arg0, s32 *, 0x50));
    }
}
