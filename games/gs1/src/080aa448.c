#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Func_08077018();
s32 Func_080aa460(s32);

void Func_080aa448(void) {
    Func_080aa460(0x3FFF & M2C_FIELD(Func_08077018(), u16 *, 0x28));
}
