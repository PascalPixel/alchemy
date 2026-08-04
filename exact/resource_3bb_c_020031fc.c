#include "types.h"

extern u8 Data_0200c250[];

void Func_020031fc(u8 *out, u32 value) {
    s32 i;

    out += 8;
    *out = 0;
    out--;
    for (i = 7; i >= 0; i--) {
        *out = Data_0200c250[value & 15];
        value >>= 4;
        out--;
    }
}
