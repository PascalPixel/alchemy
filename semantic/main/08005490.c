/*
 * Correctness fix, veneer audit (2026-07-31).  One of nine files
 * sharing this idiom; semantic/main/080052f4.c carries the full write-up
 * and semantic/main/080e15e8.c documented the thunk bank before any of us.
 *
 * `Func_08007318` was a phantom: there is no function at 0x08007318.  The
 * GCC `__call_via_rN` veneer table begins at 0x080072e4 -- fifteen
 * four-byte `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so
 * 0x08007318 is `__call_via_sp`, `bx sp`.
 *
 * Verified at this owner's own call site: `mov sp, r1` puts sp on the
 * stack VLA, the DMA `stmia` fills that VLA with 0x80 bytes copied from
 * 0x08001f38, and the `bl 0x8007318` then runs the copied bytes.  sp is
 * word-aligned, so the branch enters ARM state; 0x08001f38 disassembles as
 * coherent ARM.
 *
 * Uncertainty: the relocated routine is not named, only located.
 */

typedef unsigned int u32;
/*
 * The relocated kernel's signature once running from the stack.
 * Entered in ARM state by `bx sp`.
 */
typedef void (*StackKernel_08005490)(void *, void *, void *, void *);
extern char Value_00000080;
extern char Data_08001f38;

void Func_08005490(void *a, void *b, void *c, void *d)
{
    u32 words = (u32)&Value_00000080 >> 2;
    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001f38;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        /* `bl __call_via_sp` -- runs the bytes the DMA just copied. */
        ((StackKernel_08005490)(void *)buffer)(a, b, c, d);
    }
}
