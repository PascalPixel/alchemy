typedef unsigned char u8;

/*
 * resource_3cd owner at 0x020007b8, 16 bytes: 12 of code, a one-word pool.
 *
 * TAIL LEAF. Same class as resource_395's 0x02001838 pair and found the same
 * way: past the last recorded owner so sweep D's gap loop never reached it, no
 * `push` for sweep C, absent from `overlay_call_targets` (sites=82, a live
 * run), and no published word in the image under base+0x8000 with or without
 * the Thumb bit.
 *
 * The ROM, verbatim:
 *
 *   ldr r3, [pc, #8]   -> 0x03001f30
 *   ldr r3, [r3]
 *   movs r2, #1
 *   adds r3, #53
 *   strb r2, [r3]
 *   bx lr
 *
 * SPAN IS 16 BYTES, NOT 12. The pool word at 0x20007c4 is reached by this
 * row's own `ldr r3, [pc, #8]` -- pc 0x20007bc after alignment, plus 8 -- so
 * it belongs to this owner. The far bound is the ROM's: `b500` at 0x20007c8,
 * the next function's prologue.
 *
 * WHAT IT DOES. It dereferences the IWRAM pointer at 0x03001f30 and stores 1
 * into the BYTE at offset 53 of whatever record that pointer holds. A `strb`
 * of an immediate 1 at an odd offset is a flag set, not a count.
 *
 * WHAT IT DOES NOT SETTLE. Which record 0x03001f30 holds, and what the flag at
 * +53 means, are not decidable from this row: it reads the pointer and writes
 * one byte. The bound is the answer -- naming the field would be a guess
 * dressed as a reconstruction.
 *
 * THE NEIGHBOUR AT 0x020007c8 IS A DIFFERENT ROW and it has a `push`, which is
 * why resource_3cd's tail already read PROLOGUE-SUSPECT. That verdict was
 * firing on the neighbour and said nothing about this leaf -- a tail flagged
 * for one reason is not a tail that has been read.
 *
 * 0 of 0 callees.
 */

void Func_020007b8(void)
{
    u8 *record = *(u8 **)0x03001f30;

    record[53] = 1;
}
