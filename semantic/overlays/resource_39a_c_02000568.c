typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000568.
 *
 * Complete owner: `push {r5, lr}` at 0x02000568 and `pop {r5} / pop {r0} /
 * bx r0` at 0x020005aa, so nothing is returned.  Two pool words follow the
 * return and are data.
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
void Func_02000180();
void Func_02001154();
void Func_080000c0();
void Func_080770c8();
void Func_080f9010();
void Func_020006fc();
void Func_02000706();
void Func_02000712();
void Func_020016fc();
void Func_02002804();
void Func_020028be();
void Func_02002982();
void Func_020029a8();

void Func_02000568(void)
{
    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Func_080f9010((s32)0xf1);
    Func_02000180(10, 0, depth);
    Func_02000180(10, 0, depth);
    Func_02000180(10, 0, -80);
    Func_080f9010((s32)0x121);
    Func_080770c8((s32)0x307);
    Func_080000c0(2);
    Func_02001154();
}
