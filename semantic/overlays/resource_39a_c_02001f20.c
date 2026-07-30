typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001f20.
 *
 * Complete owner: `push {lr}` at 0x02001f20 and `pop {r0} / bx r0` at
 * its epilogue, so the owner returns nothing.  A one-word literal pool holding
 * 0x17e2 follows the return and is data, not code.
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

/* Imports. */
void Func_0200425c();
void Func_0200422c();
void Func_02004270();

void Func_02001f20(void)
{
    Func_0200425c();
    /* 0x17e2 is a plain pooled constant: it is below the overlay's 0x02008000
     * link base, so it is an identifier and not an in-image address. */
    Func_0200422c((s32)0x17e2, 1);
    Func_02004270();
}
