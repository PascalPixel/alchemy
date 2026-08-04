#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0809abb4();

void Func_0809ab98(void) {
    M2C_FIELD(M2C_FIELD(*(void **)0x03001F30, void **, 0x14), s8 *, 0x5B) = 1;
    Func_0809abb4();
}
