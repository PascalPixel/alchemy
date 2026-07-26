#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08015390(s32, s32, s32, s32);

s32 Func_08091d84(s32 arg0) {
    return Func_08015390(arg0, 0, 0, 0);
}
