/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact reconstruction can settle
 * it, and the type below says only what this call site proves.
 */
#include "types.h"

typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

struct Entry {
    u16 size;
    u16 offset;
};

struct Dma {
    u32 source;
    u32 destination;
    u32 control;
};

extern struct Entry Data_03001b10[];
extern s32 Func_08003e58(s32, s32);
extern void Func_08003f3c(u32);

s32 Func_08003fa4(s32 index, s32 size, s32 source)
{
    u16 *entry;
    s32 offset;
    s32 destination;
    s32 dummy;
    u16 current;

    entry = (u16 *)Data_03001b10 + index * 2;
    destination = 0;
    if ((u32)index > 95)
        return destination;
    if ((u32)size > 0x2000)
        return destination;
    {
        dummy = 0;
        current = entry[0];
        if (current > 16) {
            if (current != size) {
                Func_08003f3c(index);
                goto allocate;
            }
            offset = entry[1];
            goto allocated;
        }

allocate:
        offset = Func_08003e58(index, size);
allocated:
        if (offset != -1) {
            destination = 0x06010000 + offset;
            entry[0] = size;
            entry[1] = offset;

            if (source != 0) {
                if (source == -1) {
                    ((Resident_03000164)0x03000164)((void *)destination, size, source);
                } else {
                    struct Dma *dma;
                    dma = (struct Dma *)0x040000d4;
                    *dma = (struct Dma) {
                        source,
                        destination,
                        0x84000000 | ((u32)size >> 2),
                    };
                }
            }

            return (u32)offset >> 5;
        }
        return 0;
    }
    return destination;
}
