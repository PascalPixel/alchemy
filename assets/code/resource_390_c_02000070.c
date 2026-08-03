#include "resource_390.h"

typedef signed int s32;
typedef unsigned char u8;

extern s32 Func_0200033e(s32);
extern void Func_020000b0(u8 *);
extern void Func_02000368(u8 *);

u8 *Func_02000070(void)
{
    u8 *buffer;

    if (Func_0200033e(0x845) == 0) {
        Func_020000b0((u8 *)0x020084D8);
    }
    buffer = (u8 *)0x020084D8;
    Func_02000368(buffer);
    return buffer;
}
