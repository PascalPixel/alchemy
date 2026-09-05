#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Modulo(s32);

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1) {
    return Modulo(arg0 + arg1);
}
