#include "types.h"

extern void Func_0200093c(void);
extern void Func_0200098c(s32, s32);
extern void Func_02000950(void);
extern u8 *Func_02000976(s32);
extern void Func_02000438(void);
extern u8 *Func_02000986(s32);
extern void Func_0200099c(s32, s32);

void Func_020002fc(void)
{
    u8 *slot;
    u8 clear = 0;

    Func_0200093c();
    Func_0200098c(16, 1);
    Func_02000950();
    slot = Func_02000976(16) + 91;
    *slot = 1;
    Func_02000438();
    slot = Func_02000986(16) + 91;
    *slot = clear;
    Func_0200099c(16, 2);
}
