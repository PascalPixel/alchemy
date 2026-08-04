#include "types.h"

/*
 * Resource 39a overlay dialogue layout at 0x020012cc.
 *
 * Complete owner: `push {r5, lr}` and `sub sp, #8` at 0x020012cc, and the
 * matching `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x020015b4, so
 * nothing is returned.  Eight pool words follow the return and are data.  All
 * 47 distinct branch targets in the row are placed - the count agrees with
 * the inventory, which is the completeness proof for this owner.
 *
 * The eight bytes of frame are the fifth and sixth arguments of the
 * six-argument layout calls; r5 holds the repeated constant (24 in the second
 * group, 14 in the last two) across a run of them.
 *
 * Three chains share a converged tail.  0x020014c0 finishes four arms of the
 * third group with r2 = 3, r3 = 1 and the fifth argument 14, and 0x0200159e
 * finishes four arms of the fourth group the same way; only the first two
 * arguments and the sixth differ.  The tails are written out per arm rather
 * than introducing control flow the original does not have.
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
void Func_020022fc();
void Func_02002326();
void Func_0200236a();
void Func_02002392();
void Func_020023b2();
void Func_020023da();
void Func_0200240e();
void Func_02002450();
void Func_02002494();
void Func_020024d8();
void Func_020024ec();
void Func_02002520();
void Func_02002552();
void Func_02002584();
void Func_020025b6();
void Func_020035d2();
void Func_020035fc();
s32 Func_02003602();
void Func_02003626();
s32 Func_0200362c();
void Func_02003638();
void Func_0200364a();
void Func_0200366a();
void Func_02003692();
s32 Func_02003698();
void Func_020036b2();
void Func_020036da();
s32 Func_020036e0();
void Func_0200370e();
s32 Func_02003714();
void Func_02003720();
void Func_02003750();
s32 Func_02003756();
void Func_02003762();
void Func_02003794();
s32 Func_0200379a();
void Func_020037a6();
void Func_020037b8();
void Func_020037ec();
s32 Func_020037f2();
void Func_02003820();
s32 Func_02003826();
void Func_02003852();
s32 Func_02003858();
void Func_02003884();
s32 Func_0200388a();
void Func_02003896();

void Func_020012cc(void)
{
    { s32 k5 = 12, k6 = 21; Func_020035d2(12, 3, 9, 16, k5, k6); }

    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    if (Func_02003602((s32)0x308) != 0) {
        Func_020022fc(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Func_020035fc(16, 24, 1, 3, k5, k6); }
    } else if (Func_0200362c((s32)0x309) != 0) {
        Func_02002326(8, 17, 25);
        { s32 k6 = 24;
          Func_02003626(18, 24, 1, 3, 20, k6);
          Func_02003638(18, 24, 1, 3, 14, k6);
          Func_0200364a(8, 41, 1, 3, 17, k6);
        }
    } else {
        Func_0200236a(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Func_0200366a(16, 24, 1, 3, k5, k6); }
    }

    if (Func_02003698((s32)0x30a) != 0) {
        Func_02002392(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Func_02003692(14, 34, 1, 3, k5, k6); }
    } else {
        Func_020023b2(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Func_020036b2(14, 34, 1, 3, k5, k6); }
    }

    if (Func_020036e0((s32)0x30b) != 0) {
        Func_020023da(10, 15, 22);
        { s32 k5 = 14;
          Func_020036da(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 41, 3, 1, k5, 22);
        }
        /* movs r0,#0xc3 / lsls r0,#2 builds 0x30c. */
    } else if (Func_02003714((s32)0x30c) != 0) {
        Func_0200240e(10, 15, 23);
        { s32 k5 = 14;
          Func_0200370e(5, 42, 3, 1, k5, 23);
          Func_02003720(14, 29, 3, 1, k5, 30);
          Func_020037b8(10, 44, 3, 1, k5, 21);
        }
    } else if (Func_02003756((s32)0x30d) != 0) {
        Func_02002450(10, 15, 26);
        { s32 k5 = 14;
          Func_02003750(14, 29, 3, 1, k5, 22);
          Func_02003762(5, 43, 3, 1, k5, 26);
          Func_020037b8(14, 29, 3, 1, k5, 30);
        }
    } else if (Func_0200379a((s32)0x30e) != 0) {
        Func_02002494(10, 15, 27);
        { s32 k5 = 14;
          Func_02003794(14, 29, 3, 1, k5, 22);
          Func_020037a6(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 44, 3, 1, k5, 27);
        }
    } else {
        Func_020024d8(10, 15, 30);
    }

    if (Func_020037f2((s32)0x30f) != 0) {
        Func_020024ec(11, 15, 23);
        { s32 k5 = 14;
          Func_020037ec(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 40, 3, 1, k5, 23);
        }
        /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    } else if (Func_02003826((s32)0x310) != 0) {
        Func_02002520(11, 15, 24);
        { s32 k5 = 14;
          Func_02003820(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 41, 3, 1, k5, 24);
        }
    } else if (Func_02003858((s32)0x311) != 0) {
        Func_02002552(11, 15, 27);
        { s32 k5 = 14;
          Func_02003852(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 42, 3, 1, k5, 27);
        }
    } else if (Func_0200388a((s32)0x312) != 0) {
        Func_02002584(11, 15, 28);
        { s32 k5 = 14;
          Func_02003884(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 43, 3, 1, k5, 28);
        }
    } else {
        Func_020025b6(11, 15, 31);
    }
}
