/*
 * Correctness fix, veneer audit (2026-08-01).
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

extern u8 *Data_03001eec;

u32 Func_08005534(void *, void *, u32);
u32 Func_080054e4(void *, void *, u32);
u32 Func_08005490(void *, u32, void *, u32);

void Func_080cd358(void)
{
    u8 *base = Data_03001eec;

    if (*(s32 *)(base + 0x7824) == 1) {
        switch (*(s32 *)(base + 0x7780)) {
        case 0:
            ((WordCopy)0x03001388)((void *)0x06008000, (const void *)0x02010000, 0x7800);
            break;
        case 1:
            ((WordCopy)0x03001388)((void *)0x06008000, (const void *)0x02010000, 0x7800);
            ((ArmFill)0x03000168)((void *)0x02010000, 0x7800, *(u32 *)(base + 0x7784));
            break;
        case 2:
            if (*(s32 *)(base + 0x7784) == 50) {
                Func_08005534((void *)0x02010000, (void *)0x06008000, 0x7800);
            } else {
                Func_080054e4((void *)0x02010000, (void *)0x06008000, 0x7800);
            }
            break;
        case 3:
            Func_08005490((void *)0x02010000, *(u32 *)(base + 0x7784), (void *)0x06008000, 0x7800);
            break;
        }
        *(s32 *)(base + 0x7824) = 0;
    }
}
