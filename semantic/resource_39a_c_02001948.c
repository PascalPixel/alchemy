#include "types.h"

extern s16 Data_02000240[];







extern void Func_02003c60(s32, s32, s32, s32, s32, s32);
extern void Func_020029c4(void);
extern void Func_02003c80(s32, s32, s32, s32, s32, s32);
extern void Func_02002ae8(void);
extern void Func_02003cae(s32, s32, s32, s32, s32, s32);
extern void Func_02002c8e(void);
extern void Func_02003cec(s32, s32, s32, s32, s32, s32);
extern void Func_02003d02(s32, s32, s32, s32, s32, s32);
extern void Func_02002ff2(void);
void Func_02001948(void)
{
    s32 state = Data_02000240[224];

    if (state == 0x3e) {
        Func_02003c60(8, 29, 15, 5, 8, 42);
        Func_020029c4();
    } else if (state == 0x3f) {
        Func_02003c80(12, 8, 10, 18, 0, 28);
        Func_02002ae8();
    } else if (state == 0x40 && Data_02000240[225] != 1) {
        Func_02003cae(12, 21, 9, 16, 12, 3);
        Func_02002c8e();
    } else if (state == 0x41) {
        u32 selector = (u16)Data_02000240[225];

        if (selector - 1 <= 1)
            Func_02003cec(14, 10, 9, 8, 22, 20);
        else
            Func_02003d02(7, 45, 11, 4, 20, 45);
        Func_02002ff2();
    }
}
