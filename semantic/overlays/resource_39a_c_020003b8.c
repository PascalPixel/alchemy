typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x020003b8.
 *
 * Complete owner: `push {lr}` at 0x020003b8 and `pop {r0} / bx r0` at
 * 0x020003e2.  The popped word is the return address, so nothing is returned.
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
void Func_0200548();
void Func_02001534();
void Func_0200263c();
void Func_020026f6();
void Func_020027ce();
void Func_020027de();

void Func_020003b8(void)
{
    Func_020027ce((s32)0xf1);
    Func_0200548(8, 144, 0);
    Func_020027de((s32)0x121);
    /* movs r0,#0xc1 / lsls r0,#2 builds 0x304 */
    Func_020026f6((s32)0x304);
    Func_0200263c(2);
    Func_02001534();
}
