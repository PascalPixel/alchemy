#include "types.h"

extern void Func_02001c5c(void);
extern void Func_02001d58(void);
extern s32 Func_02001c4e(s32);
extern void Func_020005c2(void);
extern void Func_02000764(void);
extern void Func_02001c80(void);

void Func_020002c0(void)
{
    Func_02001c5c();
    Func_02001d58();
    if (Func_02001c4e(0x844) == 0) {
        Func_020005c2();
    } else {
        Func_02000764();
    }
    Func_02001c80();
}
