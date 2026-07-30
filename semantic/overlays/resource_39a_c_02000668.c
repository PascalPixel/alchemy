typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000668.
 *
 * Complete owner: `push {lr}` at 0x02000668 and `pop {r0} / bx r0` at
 * 0x020006b0, so nothing is returned.  Both early exits branch to that same
 * epilogue.  Three pool words follow the return.
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

/* Imports. Those used for their return value are typed. */
void Func_0200081e();
void Func_0200197a();
s32 Func_02002988();
s32 Func_02002992();
void Func_02002a94();
void Func_02002ab4();
void Func_020029ac();
void Func_020029ba();
void Func_0200290a();

void Func_02000668(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Func_02002988((s32)0x310) != 0) {
        return;
    }
    if (Func_02002992((s32)0x30d) != 0) {
        return;
    }
    Func_02002a94((s32)0xf1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Func_020029ac((s32)0x308);
    Func_020029ba((s32)0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Func_0200081e(8, -48, 0);
    Func_02002ab4((s32)0x121);
    Func_0200290a(2);
    Func_0200197a();
}
