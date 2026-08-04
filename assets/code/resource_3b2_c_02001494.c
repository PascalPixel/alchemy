#include "types.h"

extern u8 Data_02000240[];
extern u8 Data_0200b2bc[];
extern u8 Value_0000007e;
extern u8 Value_000008c8;

extern void Func_02004508(void);
extern void Func_02004532(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004596(void);
extern void Func_020045a2(void);
extern void Func_02004522(s32 arg0);
extern void Func_02004530(s32 arg0);
extern void Func_020044f2(u8 *arg0, s32 arg1, s32 arg2);
extern void Func_0200457e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020045bc(s32 arg0);
extern void Func_02004560(void);

void Func_02001494(void) {
    Func_02004508();
    Func_02004532(0, 0x6666, 0x3333);
    Func_02004596();
    Func_020045a2();
    {
        u8 *base = Data_02000240;

        Func_02004522(*(s16 *)(base + 448)
                      + ((s32)&Value_000008c8 - (s32)&Value_0000007e));
    }
    Func_02004530(30);
    Func_020044f2(Data_0200b2bc, 44, 7);
    Func_0200457e(0, 3, -16);
    Func_020045bc(3);
    Func_02004560();
}
