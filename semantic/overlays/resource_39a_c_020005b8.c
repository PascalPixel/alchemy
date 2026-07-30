typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x020005b8.
 *
 * Complete owner: `push {lr}` at 0x020005b8 and `pop {r0} / bx r0` at
 * 0x0200061c, so nothing is returned.  Three pool words follow the return.
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

/* Imports; the two queried ones are typed for their return value. */
void Func_02000760();
void Func_0200077c();
void Func_020018e6();
void Func_02002876();
s32 Func_020028de();
s32 Func_020028e8();
void Func_0200290e();
void Func_02002910();
void Func_02002924();
void Func_02002932();
void Func_020029ce();
void Func_02002a20();

void Func_020005b8(void)
{
    Func_020029ce((s32)0xf1);
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310.  The second test is only
     * reached when the first fails, and both truths take the same path. */
    if (Func_020028de((s32)0x310) != 0 || Func_020028e8((s32)0x30d) != 0) {
        Func_02000760(8, -48, 0);
        Func_02002910((s32)0x308);
        Func_0200290e((s32)0x309);
    } else {
        Func_0200077c(8, -96, 0);
        Func_02002924((s32)0x308);
        Func_02002932((s32)0x309);
    }
    Func_02002a20((s32)0x121);
    Func_02002876(2);
    Func_020018e6();
}
