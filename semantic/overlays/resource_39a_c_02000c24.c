typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000c24.
 *
 * Complete owner: `push {lr}` at 0x02000c24 and `pop {r0} / bx r0` at
 * 0x02000c62, so nothing is returned.  Five pool words follow the return.
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
 */

/* Imports. */
void Func_02000db6();
void Func_02001f2c();
void Func_02002ebc();
void Func_02002f62();
void Func_02002f72();
void Func_02002f78();
void Func_02002f7e();
void Func_0200303a();
void Func_0200304c();

void Func_02000c24(void)
{
    Func_0200303a((s32)0xf1);
    /* movs r2,#16 / negs r2,r2 */
    Func_02000db6(11, 0, -16);
    Func_0200304c((s32)0x121);
    Func_02002f62((s32)0x30f);
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    Func_02002f72((s32)0x310);
    Func_02002f78((s32)0x311);
    Func_02002f7e((s32)0x312);
    Func_02002ebc(2);
    Func_02001f2c();
}
