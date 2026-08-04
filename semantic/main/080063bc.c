#include "types.h"

extern s32 Data_02002080;
extern u8 Data_02002220[4];
extern u16 Data_04000208;
extern u16 Data_02002008;
extern u8 Data_020023a4;

s32 Func_080063bc(s32 arg0, s32 arg1) {
    u8 *channel;
    u32 saved_ime;

    channel = Data_02002220;
    if (Data_02002080 != 0)
        return -1;

    saved_ime = *(volatile u16 *)&Data_04000208;
    *(volatile u16 *)&Data_04000208 = (u16)(u32)&Data_04000208;
    channel[1] = 128;
    Data_02002008 = arg1;
    Data_020023a4 = Data_02002080;
    Data_02002080 = arg0;
    channel[0] = 1;
    *(volatile u16 *)&Data_04000208 = saved_ime;
    return 0;
}
