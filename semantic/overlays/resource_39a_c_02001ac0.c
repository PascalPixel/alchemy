/*
 * Resource 39a overlay routine at 0x02001ac0.
 *
 * Complete owner: `push {lr}` at 0x02001ac0 and `pop {r0} / bx r0` at
 * 0x02001acc - the popped word is the return address, so this returns nothing.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address here is an import identity, not a place to
 * disassemble.  That is the convention the byte-exact sources in this same
 * overlay already use (`assets/code/resource_39a_c_02000030.c` declares
 * `Func_02002442`), so imports are named by the address their call site
 * computes and their interfaces are left open.  Declarations are old-style
 * because one name is reached with different argument counts.
 */

/* Import. */
void Func_0200359a();

void Func_02001ac0(void)
{
    /* movs r0,#32 / negs r0,r0 builds the signed -32. */
    Func_0200359a(-32, 0);
}
