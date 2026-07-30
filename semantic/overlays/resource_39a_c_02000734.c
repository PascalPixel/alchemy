typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000734.
 *
 * Complete owner: `push {lr}` at 0x02000734 and `pop {r0} / bx r0` at
 * 0x0200075c.  The popped word is the return address, so nothing is returned.
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
void Func_02008c4();
void Func_02001a26();
void Func_020029b6();
void Func_02002a78();
void Func_02002b4a();
void Func_02002b5a();

void Func_02000734(void)
{
    Func_02002b4a((s32)0xf1);
    Func_02008c4(9, 32, 0);
    Func_02002b5a((s32)0x121);
    Func_02002a78((s32)0x30a);
    Func_020029b6(2);
    Func_02001a26();
}
