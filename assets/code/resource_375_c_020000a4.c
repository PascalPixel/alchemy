#include "types.h"

extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];

extern s32 Func_02001b0e(s32);
extern s32 Func_02001b1c(s32);

u8 *Func_020000a4(void)
{
    if (Func_02001b0e(0x87A) != 0) {
        return Data_0200a028;
    }
    if (Func_02001b1c(0x834) != 0) {
        return Data_02009fb0;
    }
    return Data_02009efc;
}
