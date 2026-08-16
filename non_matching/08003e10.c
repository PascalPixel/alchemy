/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x08003e36  __call_via_r6  ->  the RETURN VALUE of the call at 0x08004938
 *     The callee is whatever that call produced, not a fixed address.
 */
#include "types.h"

u32 Func_08004938(s32 arg0);
void Func_080072fc(s32);
void Func_08002df0(void *);

extern u8 Data_08001dc8;
extern u8 Value_000000e0;

void Func_08003e10(s32 request)
{
    u32 size = (u32)&Value_000000e0;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;

    control |= words;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001dc8;
        u32 destination = (u32)buffer;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    Func_080072fc(request);
    Func_08002df0(buffer);
}
