#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08092b94(s16 arg0) {
    M2C_FIELD(*(void **)0x03001EBC, s16 *, 0x1D8) = arg0;
}
