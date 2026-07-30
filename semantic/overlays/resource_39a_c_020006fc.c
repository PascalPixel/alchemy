typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x020006fc.
 *
 * Complete owner: `push {lr}` at 0x020006fc and `pop {r0} / bx r0` at
 * 0x02000726.  The popped word is the return address, so nothing is returned.
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
void Func_020088e();
void Func_020019f0();
void Func_02002980();
void Func_02002a3a();
void Func_02002b12();
void Func_02002b24();

void Func_020006fc(void)
{
    Func_02002b12((s32)0xf1);
    /* movs r1,#32 / negs r1,r1 */
    Func_020088e(9, -32, 0);
    Func_02002b24((s32)0x121);
    Func_02002a3a((s32)0x30a);
    Func_02002980(2);
    Func_020019f0();
}
