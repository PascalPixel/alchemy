/*
 * Correctness fix, veneer audit (mars, 2026-07-31).  Same idiom as
 * semantic/main/080052f4.c, which carries the full write-up.
 *
 * `void Func_08007318(void *, void *, void *)` was a phantom: there is no
 * function at 0x08007318.  It is `__call_via_sp` in the GCC veneer table
 * that begins at 0x080072e4 (fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320).  The `bl 0x8007318` at 0x08005564 runs
 * whatever sp points at, and sp was moved onto the stack VLA at
 * 0x08005554 before the DMA at 0x0800555a filled that VLA with 64 bytes
 * from 0x08001ef8.  The call runs the copied bytes, in ARM state, because
 * sp is word-aligned.
 *
 * 0x08001ef8 confirms as ARM and confirms the argument count: it opens
 * `push {r5, r6}`, computes an end pointer with `add r2, r2, r0`, and
 * then streams four words at a time with `ldm r0, {r3-r6}` /
 * `stmia r1!, {r3-r6}` while masking each word with 0x3f3f3f3f after a
 * one-bit right shift.  It reads r0, r1 and r2 -- the three arguments
 * this function forwards.
 *
 * Uncertainty: the routine is not named.  Halving six-bit fields packed
 * three to a word, in place, over a source/destination pair, is the shape
 * of a colour-component halving pass over 15-bit BGR data, but nothing
 * here proves the data is palette entries, so the claim is left as this
 * comment rather than a name.
 */

typedef unsigned int u32;

/*
 * The relocated kernel's signature once running from the stack.  Entered
 * in ARM state by `bx sp`.
 */
typedef void (*StackKernel_08005534)(void *a, void *b, void *c);

extern char Value_00000040;
extern char Data_08001ef8;

void Func_08005534(void *a, void *b, void *c)
{
    u32 words = (u32)&Value_00000040 >> 2;
    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001ef8;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;

        /* `bl __call_via_sp` -- runs the bytes the DMA just copied. */
        ((StackKernel_08005534)(void *)buffer)(a, b, c);
    }
}
