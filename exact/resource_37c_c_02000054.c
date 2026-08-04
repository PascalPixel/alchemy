#include "resource_37c.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02000118(s32);
void Func_0200011a(s32);
void Func_0200012a(s32, s32, s32);
void Func_0200013a(s32);
void Func_02000146(void);

s32 Func_02000054(void) {
    /* The three scale values are locals, not literals at the call, so that
       their materialisation sits in the entry block instead of the call's.
       Local CSE only unifies identical large constants inside one basic
       block; folding these back into the argument list collapses the three
       movs/lsls pairs to one pair plus two register copies. */
    s32 sx = 0x10000;
    s32 sy = 0x10000;
    s32 sz = 0x10000;

    M2C_FIELD(*(void **)0x03001EBC, s32 *, 0x1C0) = 0x204;
    Func_0200011a(0x144);
    if (Func_02000118(0x814) != 0) {
        Func_0200013a(0x8D);
        Func_0200012a(sx, sy, sz);
        Func_02000146();
    }
    return 0;
}
