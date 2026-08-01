typedef unsigned char u8;

/*
 * resource_398 owner at 0x0200045c, 16 bytes:
 *
 *   0200045c  4b02  ldr  r3, [pc, #8]   @ (0x2000468)
 *   0200045e  681a  ldr  r2, [r3, #0]
 *   02000460  2300  movs r3, #0
 *   02000462  75d3  strb r3, [r2, #23]
 *   02000464  4770  bx   lr
 *   02000466  0000        alignment halfword
 *   02000468  .word 0x03001e70
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that word
 * and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * SPAN 16, NOT 10, for the same reason as its twin at 0x0200044c: the pool
 * word past the `bx lr` is read by this row's own `pc`-relative load, and
 * the halfword at 0x02000466 is the alignment it needs. The next owner
 * begins at 0x0200046c, a real `push {lr}` prologue.
 *
 * The pool word is resolved on its own rather than copied from the twin --
 * it happens to be the same address, 0x03001e70, and the CONSTANT is what
 * differs: this row stores 0 where 0x0200044c stores 1. A cohort is not a
 * copy-paste even when the pool word agrees.
 */

void Func_0200045c(void)
{
    *((u8 *)*(void **)0x03001e70 + 23) = 0;
}
