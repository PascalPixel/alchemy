#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08018038(s32, s32);

s32 Func_08019ba0(s32 arg0) {
    return Func_08018038(arg0, 1);
}
