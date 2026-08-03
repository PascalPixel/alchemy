#include "resource_38c.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern void Func_020006fc(void);
extern void Func_0200073a(s32);
extern void Func_0200075c(s32, s32, s32);
extern void Func_02000746(s32, s32, s32);
extern void Func_02000714(s32);
extern s32 Func_02000764(s32, s32);
extern s32 Func_02000744(s32, s32);
extern void Func_02000790(s32, s32);
extern void Func_020007b4(s32, s32, s32);
extern void Func_02000760(void);

#define WORKSPACE (*(u8 **) 0x03001EBC)

void Func_020001e0(void)
{
    Func_020006fc();
    Func_0200073a(0x13F0);
    Func_0200075c(24, 0, 20);
    Func_02000746(24, 0, 0);
    Func_02000714(10);
    Func_02000764(24, 0);

    if (Func_02000744(0, 0) != 0) {
        ++*(u16 *) (WORKSPACE + 472);
    }

    Func_02000790(24, 0);
    Func_020007b4(24, 0x4000, 10);
    Func_02000760();
}
