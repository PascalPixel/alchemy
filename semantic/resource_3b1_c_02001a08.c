#include "types.h"

/*
 * resource_3b1 owner at 0x02001a08, 88 bytes.  `push {lr}` starts the
 * owner; `pop {r0} / bx r0` returns at 0x02001a4c-0x02001a4f.  The code is
 * already four-byte aligned at the pool boundary, so four pool words begin
 * immediately at 0x02001a50 and run through 0x02001a5f; the next owner's
 * `push {lr}` is 0x02001a60.
 */










extern s32 Func_02007eae(s32 flag);
extern void Func_02007ede(void);
extern void Func_02007ff4(s32 id);
extern void Func_02007eb6(s32 id, s32 mode, s32 arg2);
extern void Func_02007f28(s32 id, s32 arg1, s32 arg2);
extern void Func_02007f6c(s32 id, s32 arg1, s32 arg2);
extern void Func_020062c6(s32 id, unsigned short value);
extern void Func_02007f1a(void);
void Func_02001a08(void)
{
    if (Func_02007eae(0x301) == 0)
        return;

    Func_02007ede();
    Func_02007ff4(8);
    Func_02007eb6(0, 0x198, 0x86);
    Func_02007f28(0x1e48, 1, 8);
    Func_02007f6c(0, 0x19999, 0xcccc);
    Func_020062c6(0, 0x4000);
    Func_02007f1a();
}
