typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x0200029c.
 *
 * Complete owner: `push {r5, lr}` at 0x0200029c and `pop {r5} / pop {r0} /
 * bx r0` at 0x020002d2.  The popped word is the return address, so nothing is
 * returned.  The literal pool after the return is data, not code.
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
void Func_02000430();
void Func_0200043a();
void Func_02001320();
void Func_0200252c();
void Func_020025ee();
void Func_020026b6();
void Func_020026d0();

void Func_0200029c(void)
{
    /* r5 holds -112 (movs r5,#0x70 / negs r5,r5) across the first call and is
     * the second argument of both placement calls. */
    s32 offset = -112;

    Func_020026b6((s32)0xf1);
    Func_02000430(8, offset, 0);
    Func_0200043a(8, offset, 0);
    Func_020026d0((s32)0x121);
    Func_020025ee((s32)0x301);
    Func_0200252c(2);
    Func_02001320();
}
