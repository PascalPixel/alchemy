#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Modulo(s32);

s32 Func_080aa538(s32 arg0, s32 arg1) {
    return Modulo(arg0 + arg1);
}
