typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x020007f0.
 *
 * Complete owner: `push {lr}` at 0x020007f0 and `pop {r0} / bx r0` at
 * 0x020008b4, so nothing is returned.  Five pool words follow the return.
 * All 23 distinct branch targets in the row are placed.
 *
 * The first three arms share the tail call 0x02002ba4(0x30e) at 0x02000878;
 * it is written out in each arm rather than introducing control flow the
 * original does not have.
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

/* Imports; the queried ones are typed for their return value. */
void Func_0200098c();
void Func_020009b8();
void Func_020009e2();
void Func_02000a08();
void Func_02001b7e();
void Func_02002b0e();
s32 Func_02002b16();
void Func_02002b3a();
s32 Func_02002b42();
void Func_02002b48();
void Func_02002b66();
s32 Func_02002b6c();
void Func_02002b74();
void Func_02002b90();
void Func_02002b96();
void Func_02002b98();
void Func_02002ba4();
void Func_02002bb6();
void Func_02002bbe();
void Func_02002bc4();
void Func_02002bca();
void Func_02002c06();
void Func_02002cb8();

void Func_020007f0(void)
{
    Func_02002c06((s32)0xf1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    if (Func_02002b16((s32)0x308) != 0) {
        Func_0200098c(10, 0, 16);
        Func_02002b3a((s32)0x30b);
        Func_02002b3a((s32)0x30c);
        Func_02002b48((s32)0x30d);
        Func_02002ba4((s32)0x30e);
        /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    } else if (Func_02002b42((s32)0x310) != 0) {
        Func_020009b8(10, 0, 16);
        Func_02002b66((s32)0x30b);
        Func_02002b66((s32)0x30c);
        Func_02002b74((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else if (Func_02002b6c((s32)0x311) != 0) {
        Func_020009e2(10, 0, 64);
        Func_02002b90((s32)0x30b);
        Func_02002b98((s32)0x30c);
        Func_02002b96((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else {
        Func_02000a08(10, 0, 128);
        Func_02002bb6((s32)0x30b);
        Func_02002bbe((s32)0x30c);
        Func_02002bc4((s32)0x30d);
        Func_02002bca((s32)0x30e);
    }
    Func_02002cb8((s32)0x121);
    Func_02002b0e(2);
    Func_02001b7e();
}
