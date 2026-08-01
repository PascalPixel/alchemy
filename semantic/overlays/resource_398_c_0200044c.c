typedef unsigned char u8;

/*
 * resource_398 owner at 0x0200044c, 16 bytes:
 *
 *   0200044c  4b02  ldr  r3, [pc, #8]   @ (0x2000458)
 *   0200044e  681a  ldr  r2, [r3, #0]
 *   02000450  2301  movs r3, #1
 *   02000452  75d3  strb r3, [r2, #23]
 *   02000454  4770  bx   lr
 *   02000456  0000        alignment halfword
 *   02000458  .word 0x03001e70
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that word
 * and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * SPAN 16, NOT 10. The literal pool word at 0x02000458 sits past the
 * `bx lr` and is read by this row's own `pc`-relative load, so it belongs
 * to this owner, and the halfword at 0x02000456 is the alignment the pool
 * needs. The next owner begins at 0x0200045c. Recording 10 would orphan
 * both and manufacture exactly the kind of gap sweep D exists to rule.
 *
 * NOT one of the shared-body rows. This is a distinct two-instance body:
 * its twin at 0x0200045c is byte-identical except for the stored constant,
 * writing 0 where this writes 1. Identical shape, opposite effect.
 *
 * 0x03001e70 holds a pointer to the shared workspace block, spelled here
 * the way the rest of the tree spells it. The byte at +23 of that block is
 * a flag; this row raises it and the twin clears it.
 */

void Func_0200044c(void)
{
    *((u8 *)*(void **)0x03001e70 + 23) = 1;
}
