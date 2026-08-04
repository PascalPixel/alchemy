/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * `bl 0x080072fc` is __call_via_r6, and r6 is the RETURN of the Func_08004938
 * at 0x0801ee28 -- the `buffer` this function allocated and then DMA'd into
 * from Data_080158e8.  The call runs the copied bytes.  Same heap-kernel
 * idiom as semantic/main/08018cac.c, and the same family as the nine stack
 * kernels closed earlier in this audit.
 *
 * 0x080158e8 confirms as ARM: `push {r5,r6,r7,r8,r9,sl,lr}` then register
 * setup.  r6 is not an argument register, so the callee never occupied an
 * argument slot and both of the draft's arguments are real.  What the copied
 * routine DOES is not named -- only located.
 */
#include "types.h"

typedef void (*HeapKernel_0801edec)(s32 destination, void *handle);

extern void *Data_03001e8c;
extern u8 Data_080158e8;
extern u8 Value_00000214;

void *Func_08004938(u32);
void Func_08002df0(void *);

void Func_0801edec(s32 destination)
{
    void *handle = Data_03001e8c;

    if (handle == NULL) {
        u16 fill = 0xe0e0;
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&fill;
        u32 control = 0x810000a0;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    } else {
        u32 size = (u32)&Value_00000214;
        void *buffer = Func_08004938(size);
        u32 words = size >> 2;
        u32 control = 0x84000000;
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_080158e8;
        u32 copied_destination = (u32)buffer;

        control |= words;
        dma[0] = source;
        dma[1] = copied_destination;
        dma[2] = control;

        ((HeapKernel_0801edec)buffer)(destination, handle);
        Func_08002df0(buffer);
    }
}
