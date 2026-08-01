typedef signed int s32;

/*
 * resource_37b owner at 0x02000038, 4 bytes: a return-zero stub --
 * `movs r0, #0 / bx lr`, the whole function.
 *
 * Complete owner: NO PROLOGUE, no stack, no literal pool, lr never
 * written, bare `bx lr` at 0x0200003a. FOUR bytes, not eight: unlike
 * its four neighbours it carries no pool word, which is exactly why a
 * cohort's span cannot be taken from its shape. Next owner at
 * 0x0200003c. Takes no argument.
 *
 * Published, not called: its address is stored as the in-image spelling
 * 0x02008039 (offset + 0x8000, Thumb bit set) in exactly one word,
 * at overlay offset 0x2c. That word is one field of a DESCRIPTOR TABLE
 * filling the overlay's first 0x30 bytes: five function pointers at offsets
 * 0xc, 0x14, 0x1c, 0x24 and 0x2c -- stride 8 -- aimed at the five stubs that
 * begin immediately after it at 0x30. No `bl` anywhere in the image reaches
 * any of them.
 *
 * It sits in the middle of four constant getters that each return a
 * different in-image address (see 0x02000030's draft). This one
 * returns nothing at all, so one slot of the descriptor table is
 * deliberately wired to a null answer rather than left unfilled. That is
 * a fact about the table, not about this stub.
 *
 * Uncertainty: whether the caller reads the zero as a null pointer or as
 * an integer is not established, so the return is left as the widest
 * thing the row proves -- a word set to zero.
 */

s32 Func_02000038(void)
{
    return 0;
}
