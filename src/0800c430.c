#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800b8ac(s32);

void Func_0800c430(void *arg0, s32 arg1) {
    if ((arg0 != NULL) && ((0xF & M2C_FIELD(arg0, u8 *, 0x54)) == 1)) {
        arg0 = M2C_FIELD(arg0, void **, 0x50);
        if (arg1 >= 0) {
            Func_0800b8ac((s32) arg0);
        }
    }
}
