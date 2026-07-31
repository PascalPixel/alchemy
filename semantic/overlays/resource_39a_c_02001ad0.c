typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001ad0.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001ad0 and `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02001b16, so nothing is returned.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

/* Imports. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a100();
void Func_0808a128();

void Func_02001ad0(s32 x, s32 y)
{
    /* No argument register is written before this branch, so no argument is
     * asserted; r0 and r1 still hold this owner's own two arguments. */
    Func_0808a018();
    /* movs r1,#0xa0 / lsls r1,#10 and movs r2,#0xa0 / lsls r2,#9. */
    Func_0808a090(0, (s32)0x28000, (s32)0x14000);
    Func_0808a0e0(0, x, y);
    Func_0808a128(0, 4, 0);
    /* Same import, two arguments here: r2 is not rewritten in this window and
     * was clobbered by the preceding call. */
    Func_0808a100(0, 7);
    Func_0808a0e8(0);
    Func_0808a100(0, 6);
    Func_0808a020();
}
