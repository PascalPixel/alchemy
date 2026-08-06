#include "types.h"

/*
 * Resource 39a overlay dialogue layout at 0x020015dc.
 *
 * Complete owner: `push {lr}` and `sub sp, #8` at 0x020015dc, and the
 * matching `add sp, #8 / pop {r0} / bx r0` at 0x020016fa, so nothing is
 * returned.  Three pool words follow the return and are data.  All 17
 * distinct branch targets in the row are placed.
 *
 * The eight bytes of frame are the fifth and sixth arguments of the
 * six-argument layout calls.
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

/* 0x02000240 is below the 0x02008000 link base, so it is a resident table;
 * entry 225 is read here as an unsigned halfword. */
extern u16 Data_02000240[];

/* Imports; the queried ones are typed for their return value. */




extern void Func_020038f8();
extern void Func_0200390e();
extern s32 Func_0200393c();
extern void Func_02002636();
extern void Func_02003936();
extern void Func_02002656();
extern void Func_02003956();
extern s32 Func_02003986();
extern void Func_02002680();
extern void Func_02003980();
extern void Func_020026a0();
extern void Func_020039a0();
extern s32 Func_020039ce();
extern void Func_020026c8();
extern void Func_020039c8();
extern void Func_020026e8();
extern void Func_020039e8();
void Func_020015dc(void)
{
    /* movs r2,#0xe1 / lsls r2,#1 gives the byte offset 450, i.e. entry 225.
     * The test is (u32)((entry - 1) << 16) <= 0x10000 with an unsigned
     * compare, which selects exactly entries 1 and 2. */
    if ((u32)((u32)(Data_02000240[225] - 1) << 16) <= (u32)0x10000) {
        Func_020038f8(22, 20, 9, 8, 14, 10);
    } else {
        Func_0200390e(20, 45, 11, 4, 7, 45);
    }

    if (Func_0200393c((s32)0x313) != 0) {
        Func_02002636(8, 20, 17);
        Func_02003936(19, 11, 3, 1, 19, 10);
    } else {
        Func_02002656(8, 20, 10);
        Func_02003956(19, 11, 3, 1, 19, 17);
    }

    /* movs r0,#0xc5 / lsls r0,#2 builds 0x314. */
    if (Func_02003986((s32)0x314) != 0) {
        Func_02002680(9, 14, 16);
        Func_02003980(16, 15, 1, 3, 22, 15);
    } else {
        Func_020026a0(9, 22, 16);
        Func_020039a0(16, 15, 1, 3, 14, 15);
    }

    if (Func_020039ce((s32)0x315) != 0) {
        Func_020026c8(10, 17, 46);
        Func_020039c8(15, 15, 1, 3, 7, 45);
    } else {
        Func_020026e8(10, 7, 46);
        Func_020039e8(15, 15, 1, 3, 17, 45);
    }
}
