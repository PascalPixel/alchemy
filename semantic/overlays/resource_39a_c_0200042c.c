typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x0200042c.
 *
 * Complete owner: `push {lr}` at 0x0200042c and `pop {r0} / bx r0` at
 * 0x02000456.  The popped word is the return address, so nothing is returned.
 * The literal pool after the return is data, not code.
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
 *
 */

/* Imports. */
void Func_02005be();
void Func_020015a8();
void Func_020026b0();
void Func_0200276a();
void Func_02002842();
void Func_02002854();

void Func_0200042c(void)
{
    Func_02002842((s32)0xf1);
    /* movs r1,#14 / negs r1,r1 */
    Func_02005be(8, -14, 0);
    Func_02002854((s32)0x121);
    Func_0200276a((s32)0x305);
    Func_020026b0(2);
    Func_020015a8();
}
