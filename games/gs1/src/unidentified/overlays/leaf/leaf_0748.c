#include "types.h"

extern void Func_02006a9c(void);
extern s32 Func_02006a82(s32);
extern void Func_02006b74(s32);
extern void Func_02006b8c(s32, s32);
extern void Func_02006b84(s32);
extern void Func_02006b7e(s32, s32, s32);
extern void Func_02006bb6(s32, s32);
extern void Func_02006ada(void);

void Func_02000adc(void)
{
    Func_02006a9c();
    if (Func_02006a82(0x815) != 0) {
        Func_02006b74(0x11cc);
        Func_02006b8c(10, 0);
    } else {
        Func_02006b84(0xf81);
        Func_02006b7e(10, 0, 4);
        Func_02006bb6(10, 0);
    }
    Func_02006ada();
}
