#include "types.h"

s32 Func_080f9010(s32);

void Func_0809202c(void) {
    s16 value = *(s16 *)(*(u8 **)0x03001EBC + 0xCC8);
    if (value != -1) {
        Func_080f9010(value);
    }
}
