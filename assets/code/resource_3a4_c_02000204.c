typedef unsigned char u8;

/*
 * resource_3a4 owner at 0x02000204, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000208 holding 0x0200c85c.
 *
 * A LEAF, published from the overlay's OWN HEADER TABLE. The word at
 * image offset 0x14 is 0x02008205, which is this offset with the Thumb
 * bit under the base + 0x8000 spelling. It is a driver entry point, and
 * it went undrafted while sixteen larger rows around it were finished,
 * because sweep B resolved the header word correctly and then discarded
 * it for not opening with a `push`. A leaf never does.
 *
 * That is what makes this row the cleanest argument for the fix: the
 * evidence was never missing. The header published the address, the
 * sweep read the header, and the answer was thrown away by one gate.
 *
 * The pool word is an ADDRESS: 0x0200c85c is image offset 0x485c, loaded
 * and returned without being dereferenced, so this is a getter for an
 * in-image table. Same shape as this overlay's siblings elsewhere and as
 * resource_3b9's Func_02000074.
 *
 * Complete owner: the two instructions plus the pool word the
 * `pc`-relative load at 0x02000204 reads. Bounds are exact from the
 * overlay source -- `AlchemyC_020001c8` runs `.space 0x3c` to
 * 0x02000204, and `AlchemyC_0200020c` begins at 0x0200020c.
 */

extern u8 Data_0200c85c[];   /* image offset 0x485c */

u8 *Func_02000204(void)
{
    return Data_0200c85c;
}
