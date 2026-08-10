#include "types.h"

extern s32 Data_02002080;
extern u8 Data_02002220[4];
extern u16 Data_04000208;
extern u16 Data_02002008;
extern u8 Data_020023a4;

s32 Func_080063bc(s32 arg0, s32 arg1) {
    s32 *busy = (s32 *)0x02002080;
    s32 current = *busy;

    if (current != 0)
        return -1;

    {
        u8 *channel = (u8 *)0x02002220;
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u32 saved_ime;

        saved_ime = *ime;
        *ime = (u16)(u32)ime;
        channel[1] = 128;
        Data_02002008 = arg1;
        Data_020023a4 = (u8)current;
        *busy = arg0;
        channel[0] = 1;
        *ime = saved_ime;
    }
    return 0;
}
