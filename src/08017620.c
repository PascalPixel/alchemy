#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08017620(s32 arg0) {
    void *temp_r2;

    temp_r2 = *(void **)0x03001E8C;
    if (temp_r2 != NULL) {
        if (arg0 & 1) {
            M2C_FIELD(temp_r2, s8 *, 0x12FA) = 1;
        }
        if (2 & arg0) {
            M2C_FIELD(temp_r2, s8 *, 0x12FB) = 1;
        }
    }
}
