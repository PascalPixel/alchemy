typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000ca8.
 *
 * Complete owner: `push {lr}` at 0x02000ca8 and `pop {r0} / bx r0` at
 * 0x02000cce.  The popped word is the return address, so nothing is returned.
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
void Func_0200e38();
void Func_0200188a();
void Func_02001f98();
void Func_02002f28();
void Func_020030be();
void Func_020030ce();

void Func_02000ca8(void)
{
    Func_020030be((s32)0xf1);
    Func_0200e38(11, 0, 64);
    Func_020030ce((s32)0x121);
    Func_0200188a();
    Func_02002f28(2);
    Func_02001f98();
}
