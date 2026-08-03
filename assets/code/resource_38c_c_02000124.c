#include "resource_38c.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_02000662(s32);
extern void Func_02000648(void);
extern void Func_020006ca(s32, s32);
extern s32 Func_0200064a(s32);
extern void Func_020006a4(s32);
extern void Func_020006a6(s32, s32, s32);
extern void Func_02000674(s32);
extern s32 Func_020006dc(s32, s32);
extern void Func_020006f0(s32, s32, s32);
extern void Func_020006d0(s32);
extern void Func_020006e8(s32, s32);
extern void Func_020006ac(void);

void Func_02000124(void)
{
    u32 place;

    place = *(u16 *) (Func_02000662(0) + 6);
    Func_02000648();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_020006ca(8, 17);
    } else if (Func_0200064a(0x845) == 0) {
        Func_020006a4(0x13E5);
        Func_020006a6(17, 0, 0);
        Func_02000674(10);
        Func_020006dc(17, 0);
        Func_020006f0(17, 0x3000, 10);
    } else {
        Func_020006d0(0x16F7);
        Func_020006e8(17, 0);
    }

    Func_020006ac();
}
