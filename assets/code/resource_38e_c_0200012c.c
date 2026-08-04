#include "types.h"

extern void Func_02000c04(void);
extern void Func_02000bdc(s32, s32);
extern void Func_02000be4(s32, s32);
extern void Func_02000c20(void);

void Func_0200012c(void)
{
    Func_02000c04();
    Func_02000bdc(0x947, 1);
    Func_02000be4(0x29DE, 1);
    Func_02000c20();
}
