typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000cd8.
 *
 * Complete owner: `push {lr}` at 0x02000cd8 and `pop {r0} / bx r0` at
 * 0x02000cfe.  The popped word is the return address, so nothing is returned.
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
void Func_0200e68();
void Func_020018ba();
void Func_02001fc8();
void Func_02002f58();
void Func_020030ee();
void Func_020030fe();

void Func_02000cd8(void)
{
    Func_020030ee((s32)0xf1);
    Func_0200e68(11, 0, 80);
    Func_020030fe((s32)0x121);
    Func_020018ba();
    Func_02002f58(2);
    Func_02001fc8();
}
