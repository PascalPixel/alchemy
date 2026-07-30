typedef signed int s32;

/*
 * Resource 375, owner at 0x020000a4 (56 bytes: 0x020000a4-0x020000c7 code,
 * 0x020000c8-0x020000db literal pool).
 *
 * Complete owner: `push {lr}` at 0x020000a4, matching interworking return
 * `pop {r1} / bx r1` at 0x020000c4.  Because the popped return address goes to
 * r1 and not r0, r0 survives the epilogue and IS the result (HANDOVER §0).
 *
 * Link base.  This overlay is linked at 0x02008000, so a pool word in the
 * 0x0200_8xxx..0200_bxxx band is an in-image address at `value - 0x8000`.
 * Witness: the word 0x02008171 at file offset 0x1f04 is the start of this
 * overlay's own owner at offset 0x0170 plus the Thumb bit, and it sits inside
 * the very table this function returns (0x02009efc -> offset 0x1efc).  The
 * three returned pool words are therefore in-image descriptor tables, not RAM
 * globals.  Each is a run of 12-byte records shaped
 * { u32, 0xffff00NN, <in-image handler + 1> }.
 *
 * Both `bl` sites resolve, through the import veneer table at 0x02001a54, to
 * the same import Func_080770c0 (the two encodings are bit-identical; the
 * printed pc-relative annotations are wrong for every overlay `bl`).  The
 * import is called with one argument and its result is tested for zero, so it
 * is declared s32 here.  The two arguments 0x087a and 0x0834 look like event or
 * flag identifiers; their meaning is not established, so they are left numeric.
 */

/* Import reached through the veneer at 0x02001a64 -> Func_080770c0.
 * Old-style declaration: the real arity is not established, but the return
 * type must be given because the result is used in a condition. */
s32 Func_080770c0();

void *Func_020000a4(void)
{
    if (Func_080770c0(0x87a) != 0) {
        return (void *)0x0200a028;
    }
    if (Func_080770c0(0x834) != 0) {
        return (void *)0x02009fb0;
    }
    return (void *)0x02009efc;
}
