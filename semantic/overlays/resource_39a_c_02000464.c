typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000464.
 *
 * Complete owner: `push {lr}` at 0x02000464 and `pop {r0} / bx r0` at
 * 0x020004aa, so nothing is returned.  Three pool words follow the return.
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
void Func_020005fe();
void Func_02000610();
void Func_020015fc();
void Func_02002704();
s32 Func_02002788();
void Func_020027ac();
void Func_020027b8();
void Func_0200287a();
void Func_020028ae();

void Func_02000464(void)
{
    Func_0200287a((s32)0xf1);
    if (Func_02002788((s32)0x306) != 0) {
        Func_020005fe(8, 16, 0);
        Func_020027ac((s32)0x305);
    } else {
        Func_02000610(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Func_020027b8((s32)0x304);
    }
    Func_020028ae((s32)0x121);
    Func_02002704(2);
    Func_020015fc();
}
