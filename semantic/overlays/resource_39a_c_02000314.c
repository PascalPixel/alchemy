typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000314.
 *
 * Complete owner: `push {lr}` at 0x02000314 and `pop {r0} / bx r0` at
 * 0x0200033e.  The popped word is the return address, so nothing is returned.
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
void Func_02004a6();
void Func_0200138c();
void Func_02002598();
void Func_02002652();
void Func_0200272a();
void Func_0200273c();

void Func_02000314(void)
{
    Func_0200272a((s32)0xf1);
    /* movs r2,#64 / negs r2,r2 */
    Func_02004a6(9, 0, -64);
    Func_0200273c((s32)0x121);
    Func_02002652((s32)0x302);
    Func_02002598(2);
    Func_0200138c();
}
