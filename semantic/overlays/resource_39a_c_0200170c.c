typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 39a overlay routine at 0x0200170c.
 *
 * Complete owner: `push {r5, lr}` at 0x0200170c and `pop {r5} / pop {r0} /
 * bx r0` at 0x02001722, so the owner returns nothing.  Its two pool words sit
 * after the return and are data.
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

/* 0x0200b328 is one word past the end of the 0x3328-byte image, so it is a
 * loader/RAM word rather than in-image data; the byte-exact sibling
 * `assets/code/resource_39a_c_02001730.c` spells it the same way. */
extern u8 *Data_0200b328;
extern u8 *Data_03001ee0;

/* Import; used for its return value. */
s32 Func_02003a6e();

void Func_0200170c(void)
{
    if (Data_0200b328 != 0) {
        /* The workspace pointer is loaded before the call, so the store below
         * uses the pre-call value. */
        u8 *workspace = Data_03001ee0;

        *(s32 *)(workspace + 24) = Func_02003a6e(0);
    }
}
