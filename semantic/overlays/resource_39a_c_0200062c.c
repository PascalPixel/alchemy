typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x0200062c.
 *
 * Complete owner: `push {lr}` at 0x0200062c and `pop {r0} / bx r0` at
 * 0x0200065c.  The popped word is the return address, so nothing is returned.
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
void Func_02007ca();
void Func_02001926();
void Func_020028b6();
void Func_02002962();
void Func_02002968();
void Func_02002a42();
void Func_02002a60();

void Func_0200062c(void)
{
    Func_02002a42((s32)0xf1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308 */
    Func_02002962((s32)0x308);
    Func_02002968((s32)0x309);
    Func_02007ca(8, 48, 0);
    Func_02002a60((s32)0x121);
    Func_020028b6(2);
    Func_02001926();
}
