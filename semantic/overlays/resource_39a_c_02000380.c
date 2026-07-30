typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000380.
 *
 * Complete owner: `push {lr}` at 0x02000380 and `pop {r0} / bx r0` at
 * 0x020003aa.  The popped word is the return address, so nothing is returned.
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
void Func_0200512();
void Func_020013f8();
void Func_02002604();
void Func_020026be();
void Func_02002796();
void Func_020027a8();

void Func_02000380(void)
{
    Func_02002796((s32)0xf1);
    /* movs r2,#64 / negs r2,r2 */
    Func_0200512(10, 0, -64);
    Func_020027a8((s32)0x121);
    Func_020026be((s32)0x303);
    Func_02002604(2);
    Func_020013f8();
}
