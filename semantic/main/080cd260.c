/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef void (*ArmFill)(void *destination, u32 size, u32 value);

void Func_08005534(s32 source, s32 destination, s32 size);
void Func_080054e4(void *source, void *destination, u32 size);
void Func_08005490(s32 source, s32 mode, s32 destination, s32 size);
void Func_0800543c(s32 source, s32 mode, s32 destination, s32 size);

void Func_080cd260(void)
{
    u8 *state = *(u8 **)0x03001eec;
    s32 work = *(s32 *)0x03001ef0;
    s32 mode;

    if (*(s32 *)(state + 0x7824) != 1) {
        (*(s32 *)(state + 0x7820))++;
        return;
    }

    mode = *(s32 *)(state + 0x7780);
    switch (mode) {
    case 0:
        ((WordCopy)0x03001388)((void *)0x06004000, (const void *)work, 0x4000);
        break;
    case 1:
        ((WordCopy)0x03001388)((void *)0x06004000, (const void *)work, 0x4000);
        ((ArmFill)0x03000168)(
            (void *)work, 0x4000, *(u32 *)(state + 0x7784));
        break;
    case 2:
        if (*(s32 *)(state + 0x7784) == 0x32)
            Func_08005534(work, 0x06004000, 0x4000);
        else
            Func_080054e4((void *)work, (void *)0x06004000, 0x4000);
        break;
    case 3:
        Func_08005490(
            work, *(s32 *)(state + 0x7784), 0x06004000, 0x4000);
        break;
    case 4:
        Func_0800543c(
            work, *(s32 *)(state + 0x7784), 0x06004000, 0x4000);
        break;
    }

    *(s32 *)(state + 0x7824) = 0;
    *(s32 *)(state + 0x7820) = 1;
}
