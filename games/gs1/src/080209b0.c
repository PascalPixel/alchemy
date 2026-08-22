#include "types.h"

s32 Func_08003fa4(s32, s32, s32);
s32 Func_08004080();
extern u8 Data_080310a4[];

s32 Func_080209b0(void) {
    s32 temp_r0;

    temp_r0 = Func_08004080();
    Func_08003fa4(temp_r0, 0x80, Data_080310a4);
    return temp_r0;
}
