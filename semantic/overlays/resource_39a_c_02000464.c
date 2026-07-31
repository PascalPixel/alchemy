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
void Func_02000180();
void Func_02001154();
void Func_080000c0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_080f9010();

void Func_02000464(void)
{
    Func_080f9010((s32)0xf1);
    if (Func_080770c0((s32)0x306) != 0) {
        Func_02000180(8, 16, 0);
        Func_080770d0((s32)0x305);
    } else {
        Func_02000180(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        Func_080770c8((s32)0x304);
    }
    Func_080f9010((s32)0x121);
    Func_080000c0(2);
    Func_02001154();
}
