typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000920.
 *
 * Complete owner: `push {lr}` at 0x02000920 and `pop {r0} / bx r0` at
 * 0x020009b8, so nothing is returned.  Five pool words follow the return.
 *
 * 0x02002c70 is reached both as a query (0x310) and as a setter (0x30c) - the
 * same import name with two different uses, which is why its result is simply
 * dropped at the setter site.  The first two arms share the tail call
 * 0x02002ca8(0x30e) at 0x0200097c; it is written out in each arm rather than
 * introducing a flag the original does not have.
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
void Func_02000aba();
void Func_02000ae6();
void Func_02000b0c();
void Func_02001c82();
void Func_02002c12();
s32 Func_02002c44();
void Func_02002c68();
void Func_02002c6e();
s32 Func_02002c70();
void Func_02002c94();
void Func_02002ca2();
void Func_02002ca8();
void Func_02002cba();
void Func_02002cc2();
void Func_02002cc8();
void Func_02002cce();
void Func_02002d36();
void Func_02002dbc();

void Func_02000920(void)
{
    Func_02002d36((s32)0xf1);
    if (Func_02002c44((s32)0x311) != 0) {
        Func_02000aba(10, 0, 48);
        Func_02002c68((s32)0x30b);
        /* movs r0,#0xc3 / lsls r0,#2 builds 0x30c; the result is unused. */
        Func_02002c70((s32)0x30c);
        Func_02002c6e((s32)0x30d);
        Func_02002ca8((s32)0x30e);
        /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    } else if (Func_02002c70((s32)0x310) != 0) {
        Func_02000ae6(10, 0, 32);
        Func_02002c94((s32)0x30b);
        Func_02002c94((s32)0x30c);
        Func_02002ca2((s32)0x30d);
        Func_02002ca8((s32)0x30e);
    } else {
        Func_02000b0c(10, 0, 112);
        Func_02002cba((s32)0x30b);
        Func_02002cc2((s32)0x30c);
        Func_02002cc8((s32)0x30d);
        Func_02002cce((s32)0x30e);
    }
    Func_02002dbc((s32)0x121);
    Func_02002c12(2);
    Func_02001c82();
}
