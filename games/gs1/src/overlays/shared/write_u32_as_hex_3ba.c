#include "types.h"

extern u8 Data_0200bfd0[];

#define HexDigits Data_0200bfd0
#define Text_WriteU32AsHex Func_02002f64

void Text_WriteU32AsHex(u8 *buf, u32 value) {
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}
