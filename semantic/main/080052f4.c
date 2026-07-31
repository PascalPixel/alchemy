/*
 * Correctness fix, veneer audit (mars, 2026-07-31).
 *
 * This file previously declared `void Func_08007318(s32, s32)` and called
 * it as an ordinary function.  There is no function at 0x08007318.
 * 0x080072e4 begins the GCC `__call_via_rN` veneer table -- fifteen
 * four-byte entries, `bx rN; nop`, running r0..lr and ending at
 * 0x08007320 -- so 0x08007318 is `__call_via_sp`, i.e. `bx sp`.
 *
 * The `bl 0x8007318` at 0x08005320 is therefore an indirect call to
 * whatever sp points at, and the instructions before it are the callee
 * setup, not incidental scratch: `mov sp, r1` at 0x08005312 has already
 * moved sp onto the stack VLA, and the DMA at 0x08005318 has filled that
 * VLA with 84 bytes copied from 0x08002298.  The call runs those bytes.
 *
 * sp is word-aligned, so bit 0 of the branch target is clear and `bx sp`
 * enters ARM state.  That is checkable and checks out: 0x08002298
 * disassembles as coherent ARM (`mov r3, #0x80000000`, a bit accumulator
 * driven by `lsls r3, r3, #1`, post-indexed `ldrh r2, [r0], #2` and
 * `strh r2, [r1], #2` halfword streaming), and it uses exactly the two
 * arguments this function forwards in r0 and r1.
 *
 * Uncertainty: the copied routine's job is NOT named here.  Its shape --
 * a shifted bit accumulator driving halfword reads and writes over a
 * source/destination pair -- reads like a bitstream decoder, but that is
 * an inference from a dozen instructions and this function does not prove
 * it.  The routine stays addressed by its ROM location.
 */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * The relocated kernel's signature once it is running from the stack.
 * Entered in ARM state by `bx sp`.
 */
typedef void (*StackKernel_080052f4)(s32 first, s32 second);

extern u8 Data_08002298;
extern u8 Value_00000054;

void Func_080052f4(s32 first, s32 second)
{
    u32 words = (u32)&Value_00000054 >> 2;

    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08002298;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;

        /*
         * `bl __call_via_sp` -- sp is the buffer, so this runs the bytes
         * the DMA just copied.  Written through `buffer` rather than
         * through a literal sp read because the two hold the same value
         * here and the buffer is the thing that was filled.
         */
        ((StackKernel_080052f4)(void *)buffer)(first, second);
    }
}
