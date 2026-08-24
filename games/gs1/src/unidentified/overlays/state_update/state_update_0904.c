#include "types.h"

extern void Func_02005ffc(void);
extern void Func_02006012(s32);
extern void Func_02005f78(s32);
extern void Func_020060c8(s32, s32);
extern s32 Func_02006036(s32);
extern void Func_02005fd4(s32, s32);
extern void Func_0200617e(void);
extern void Func_02006192(void);
extern void Func_02006030(s32);
extern void Func_020060e8(s32, s32);
extern void Func_02006106(s32);
extern void Func_02006128(s32, s32, s32);
extern void Func_0200576e(s32, s32);
extern void Func_020060fc(s32, s32, s32);
extern void Func_0200610c(s32, s32);
extern void Func_0200614e(s32, s32, s32);
extern void Func_020061d2(void);
extern void Func_020061de(void);
extern void Func_020061bc(s32);

extern u8 Data_0200d160[];
extern s32 *Data_03001ebc;

void Func_02001d0c(void)
{
    Func_02005ffc();
    Func_02006012((s32)Data_0200d160);
    Func_02005f78(1);
    Func_020060c8(0, 15);
    Func_02005fd4(Func_02006036(0), 0);
    Data_03001ebc[0x70] = 0x202;
    Func_0200617e();
    Func_02006192();
    Func_02006030(20);
    Func_020060e8(20, 1);
    Func_02006106(0x1e41);
    Func_02006128(20, 0, 10);
    Func_0200576e(22, 0x5000);
    Func_020060fc(22, 4, 20);
    Func_0200610c(22, 2);
    Func_0200614e(0x6016, 0, 20);
    Func_020061d2();
    Func_020061de();
    Func_020061bc(11);
}
