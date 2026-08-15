#include "types.h"

extern u8 Data_0200c250[];

#define HexDigits Data_0200c250
#define WriteU32AsHex Func_020031fc

void WriteU32AsHex(u8 *hex_text, u32 value) {
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = HexDigits[value & 15];
        value >>= 4;
        hex_text--;
    }
}
