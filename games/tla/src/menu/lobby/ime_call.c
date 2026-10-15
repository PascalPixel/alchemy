#include "types.h"
#include "io_reg.h"

void Func_0200191c(void);
void Func_02001914(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001ab4(s32 value);
void Func_02001abc(s32 value);

void Func_02000044(void)
{
    volatile u16 *ime = &REG_IME;
    u32           saved = *ime;

    *ime = (u16)(u32)ime;
    Func_0200191c();
    Func_02001914();
    *ime = saved;
}
