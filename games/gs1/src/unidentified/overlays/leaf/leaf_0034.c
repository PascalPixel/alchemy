#include "resource_38c.h"

#include "types.h"

extern void Func_020009ac(void);
extern s32 Func_0200099a(s32);
extern void Func_020009f4(s32);
extern void Func_020009fc(s32);
extern void Func_02000a14(s32, s32);
extern void Func_020009d8(void);

void Func_02000490(void)
{
    Func_020009ac();
    if (Func_0200099a(3) != 0) {
        Func_020009f4(0x146F);
    } else {
        Func_020009fc(0x13D9);
    }
    Func_02000a14(10, 0);
    Func_020009d8();
}
