#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02000a14(s32);

s32 Func_02000044(void) {
    if (Func_02000a14(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}
