typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000da4.
 *
 * Complete owner: `push {lr}` at 0x02000da4 and `pop {r0} / bx r0` at
 * 0x02000dd0.  The popped word is the return address, so nothing is returned.
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
 * 0x02000f36 and its sibling 0x02000f68 are the clearest evidence in this overlay that an encoded branch address is an identity and not a location: both fall strictly inside 0x02000f30, which `assets/code/resource_39a_c_02000f30.c` already reproduces byte-exactly as a self-contained table lookup with no entry point at +6 or +0x38.  The call is therefore recorded by identity like every other import here.
 *
 */

/* Imports. */
void Func_0200f36();
void Func_020023aa();
void Func_0200302a();
void Func_020030e4();
void Func_020031ba();
void Func_020031cc();

void Func_02000da4(void)
{
    Func_020031ba((s32)0xf1);
    /* movs r1,#0x80 / negs r1,r1 */
    Func_0200f36(9, -128, 0);
    Func_020031cc((s32)0x121);
    /* movs r0,#0xc5 / lsls r0,#2 builds 0x314 */
    Func_020030e4((s32)0x314);
    Func_0200302a(2);
    Func_020023aa();
}
