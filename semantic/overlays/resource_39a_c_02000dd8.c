typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000dd8.
 *
 * Complete owner: `push {lr}` at 0x02000dd8 and `pop {r0} / bx r0` at
 * 0x02000e02.  The popped word is the return address, so nothing is returned.
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
 * 0x02000f68 falls strictly inside 0x02000f30, which `assets/code/resource_39a_c_02000f30.c` already reproduces byte-exactly as a self-contained table lookup with no entry point at +0x38.  That is why the branch is recorded by identity like every other import in this overlay rather than resolved as a location.
 *
 */

/* Imports. */
void Func_0200f68();
void Func_020023dc();
void Func_0200305c();
void Func_0200311e();
void Func_020031ee();
void Func_020031fe();

void Func_02000dd8(void)
{
    Func_020031ee((s32)0xf1);
    Func_0200f68(9, 128, 0);
    Func_020031fe((s32)0x121);
    /* movs r0,#0xc5 / lsls r0,#2 builds 0x314 */
    Func_0200311e((s32)0x314);
    Func_0200305c(2);
    Func_020023dc();
}
