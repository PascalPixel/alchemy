#include "types.h"

/*
 * resource_3a2 owner at 0x02000030, 36 bytes:
 *
 *   02000030  1c01  adds r1, r0, #0
 *   02000032  3123  adds r1, #35
 *   02000034  780a  ldrb r2, [r1, #0]
 *   02000036  23fe  movs r3, #254
 *   02000038  4013  ands r3, r2
 *   0200003a  700b  strb r3, [r1, #0]
 *   0200003c  6d02  ldr  r2, [r0, #80]
 *   0200003e  7a53  ldrb r3, [r2, #9]
 *   02000040  210c  movs r1, #12
 *   02000042  430b  orrs r3, r1
 *   02000044  7253  strb r3, [r2, #9]
 *   02000046  6d02  ldr  r2, [r0, #80]
 *   02000048  7d53  ldrb r3, [r2, #21]
 *   0200004a  430b  orrs r3, r1
 *   0200004c  7553  strb r3, [r2, #21]
 *   0200004e  2000  movs r0, #0
 *   02000050  4770  bx   lr
 *   02000052  0000        alignment halfword
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that word
 * and, before 2026-08-01, discarded it for not opening with a `push`. It
 * saves no register: every temporary lives in r1-r3, and the argument stays
 * in r0 for the whole body, so the row returns bare with `bx lr`.
 *
 * NO LITERAL POOL. It is the one row in this slice that reads its argument
 * -- the shared-body getters and the zero stubs take none -- and the one
 * that is a leaf for the ordinary reason rather than the degenerate one.
 *
 * The record's own byte at +35 has bit 0 cleared. The record at +80 holds a
 * pointer to a second structure, RE-READ rather than cached across the two
 * updates, and bits 2 and 3 are set in its bytes at +9 and +21. The re-read
 * is in the reference and is preserved: a cached local would compile
 * differently and would be wrong if anything between the two stores can
 * republish that slot.
 *
 * Returns 0 unconditionally -- no branch, no failure path.
 *
 * The next owner begins at 0x02000054 with `push {r5, lr}`; the halfword at
 * 0x02000052 is the alignment between them and is recorded with this row so
 * no stray halfword is left for sweep D to find.
 */

s32 Func_02000030(u8 *record)
{
    u8 *linked;

    record[35] &= ~1;

    linked = *(u8 **)(record + 80);
    linked[9] |= 12;

    linked = *(u8 **)(record + 80);
    linked[21] |= 12;

    return 0;
}
