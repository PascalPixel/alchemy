#include "types.h"

/*
 * Resource 39a overlay dialogue layout at 0x02001154.
 *
 * Complete owner: `push {r5, r6, r7, lr}` and `sub sp, #8` at 0x02001154,
 * and the matching `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020012b6, so nothing is returned.  Three pool words follow the return and
 * are data.  All 24 distinct branch targets in the row are placed.
 *
 * The eight bytes of frame are the fifth and sixth arguments of the
 * six-argument layout calls; r5, r6 and r7 hold the repeated constants 19, 13
 * and 12 across the second group.
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
void Func_02001004();
void Func_080091c0();
s32 Func_080770c0();

void Func_02001154(void)
{
    Func_080091c0(0, 28, 10, 18, 12, 8);

    /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
    if (Func_080770c0((s32)0x304) != 0) {
        Func_02001004(8, 21, 20);
        Func_080091c0(20, 19, 1, 3, 13, 19);
    } else {
        Func_02001004(8, 13, 20);
        Func_080091c0(20, 19, 1, 3, 21, 19);
    }

    if (Func_080770c0((s32)0x305) != 0) {
        Func_02001004(8, 12, 20);
        Func_080091c0(5, 19, 1, 3, 12, 19);
        Func_080091c0(20, 19, 1, 3, 13, 19);
        if (Func_080770c0((s32)0x304) != 0) {
            Func_02001004(8, 21, 20);
            Func_080091c0(20, 19, 1, 3, 13, 19);
            Func_080091c0(20, 19, 1, 3, 12, 19);
        }
    }

    if (Func_080770c0((s32)0x306) != 0) {
        Func_02001004(9, 15, 21);
        Func_080091c0(14, 18, 3, 1, 14, 17);
    } else {
        Func_02001004(9, 15, 17);
        Func_080091c0(14, 18, 3, 1, 14, 21);
    }

    if (Func_080770c0((s32)0x307) != 0) {
        Func_02001004(10, 19, 8);
        Func_080091c0(14, 18, 3, 1, 18, 25);
    } else {
        Func_02001004(10, 19, 25);
        Func_080091c0(14, 18, 3, 1, 18, 8);
    }
}
