#include "types.h"

extern s32 Func_0200495a(s32);
extern void Func_02004b3c(s32);
extern void Func_0200491e(u8 *, s32, s32);
extern s32 Func_02004974(s32);
extern s32 Func_0200497e(s32);
extern void Func_020017b6(void);
extern void Func_02004994(s32);
extern void Func_02004a32(s32, s32, s32);
extern void Func_020049b8(s32);
extern void Func_02000426(s32);
extern u8 Value_00000206;
extern u8 Data_0200d7a0[];

void Func_02000278(void)
{
    if (Func_0200495a((s32)&Value_00000206) == 0) {
        Func_02004b3c(158);
        Func_0200491e(Data_0200d7a0, 45, 39);
    }
    if (Func_02004974(0x835) == 0 && Func_0200497e(0x831) == 0) {
        Func_020017b6();
        Func_02004994((s32)&Value_00000206);
    } else {
        Func_02004a32(0, 0x106, 0x325);
        Func_020049b8(3);
        Func_02000426(6);
    }
}
