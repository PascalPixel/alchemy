typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001c08.
 *
 * Complete owner: `push {lr}` at 0x02001c08 and `pop {r1} / bx r1` at
 * 0x02001c12.  r1 carries the return address, so r0 survives the epilogue and
 * the constant zero moved into it at 0x02001c10 is the result.
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
void Func_02003f9e();

s32 Func_02001c08(s32 subject)
{
    /* r0 is never written before the branch: the owner's own first argument is
     * forwarded unchanged, and only r1 is set here. */
    Func_02003f9e(subject, 15);
    return 0;
}
