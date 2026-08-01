typedef unsigned char u8;

/*
 * resource_3b9 owner at 0x02000074, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000078 holding 0x0200b2bc.
 *
 * A LEAF, and the sibling of Func_02000070 immediately above it. Same
 * story: the overlay's own header table publishes it at image offset
 * 0x14 as 0x02008075 -- this offset with the Thumb bit under the
 * base + 0x8000 spelling -- and sweep B resolved that word correctly and
 * then discarded it for not opening with a `push`. It has no `push`
 * because it is a leaf.
 *
 * The pool word is an ADDRESS, not a value: 0x0200b2bc is image offset
 * 0x32bc. The row loads it and returns it without dereferencing, so this
 * is a getter handing out the address of an in-image table.
 *
 * Complete owner: the two instructions plus the pool word they read.
 * The literal pool is INSIDE the row, which is why a span that stopped
 * at `bx lr` would report 4 bytes and leave a stray word behind -- the
 * `pc`-relative load at 0x02000074 reads 0x02000078, so the word belongs
 * to this owner and to nothing else.
 *
 * Bounds are exact: the previous raw word at 0x02000070 is
 * Func_02000070, and `AlchemyC_0200007c` begins with a real `push {lr}`
 * at 0x0200007c.
 */

extern u8 Data_0200b2bc[];   /* image offset 0x32bc */

u8 *Func_02000074(void)
{
    return Data_0200b2bc;
}
