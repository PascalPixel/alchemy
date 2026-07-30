typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000ab4.
 *
 * Complete owner: `push {lr}` at 0x02000ab4 and `pop {r0} / bx r0` at
 * 0x02000af0, so nothing is returned.  Four pool words follow the return.
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
void Func_02000c44();
void Func_02001dba();
void Func_02002d4a();
void Func_02002df8();
void Func_02002e00();
void Func_02002e06();
void Func_02002e0c();
void Func_02002eca();
void Func_02002eda();

void Func_02000ab4(void)
{
    Func_02002eca((s32)0xf1);
    Func_02000c44(10, 0, 64);
    Func_02002eda((s32)0x121);
    Func_02002df8((s32)0x30b);
    /* movs r0,#0xc3 / lsls r0,#2 builds 0x30c. */
    Func_02002e00((s32)0x30c);
    Func_02002e06((s32)0x30d);
    Func_02002e0c((s32)0x30e);
    Func_02002d4a(2);
    Func_02001dba();
}
