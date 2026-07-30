/*
 * Resource 39a overlay routine at 0x02000ff8.
 *
 * Complete owner: `push {lr}` at 0x02000ff8 and `pop {r0} / bx r0` at
 * 0x02000ffe.  The popped word is the return address, so nothing is returned.
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

/* Import. No argument register is written before the branch, so the call site
 * asserts no arguments; r0-r3 still hold whatever this owner was entered with. */
void Func_02003090();

void Func_02000ff8(void)
{
    Func_02003090();
}
