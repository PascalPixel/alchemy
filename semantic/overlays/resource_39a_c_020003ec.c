typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x020003ec.
 *
 * Complete owner: `push {lr}` at 0x020003ec and `pop {r0} / bx r0` at
 * 0x0200041e.  The popped word is the return address, so nothing is returned.
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
void Func_020057e();
void Func_02001570();
void Func_02002678();
void Func_02002732();
void Func_02002734();
void Func_02002802();
void Func_02002814();

void Func_020003ec(void)
{
    Func_02002802((s32)0xf1);
    /* movs r1,#0x90 / negs r1,r1 */
    Func_020057e(8, -144, 0);
    Func_02002814((s32)0x121);
    /* movs r0,#0xc1 / lsls r0,#2 builds 0x304 */
    Func_02002734((s32)0x304);
    Func_02002732((s32)0x305);
    Func_02002678(2);
    Func_02001570();
}
