#include "types.h"

/*
 * Resource 37f, owner at 0x02000258 (334 bytes of code, prologue
 * `push {r5, r6, lr}` + the r8 spill + `sub sp, #8` at 0x02000258 through the
 * interworking return at 0x0200039c, plus a five-word literal pool at
 * 0x020003a8-0x020003bb).  r0 is the popped return address, so `void`.
 *
 * All 32 call sites were resolved with `tools/lib/overlay_call_targets.ts`; all 32
 * are import veneers (the inventory's `calls=31` is the documented undercount).
 *
 * Near-identical sibling of 0x02000420 — same gate, same tail, same constants
 * — differing only in the flag pair (0xf01 / 0x81a instead of 0xf02 / 0x821),
 * in the three Func_08009180/Func_080091c0 layout calls in the middle, and in
 * holding the scene id in r8 rather than r6.  That two-value diff between
 * siblings is the correctness proof for both files.
 *
 * Func_08009180 and Func_080091c0 are the six-argument renderer-family calls;
 * arguments 5 and 6 are [sp, #0] and [sp, #4].  r5 holds 1 and r6 holds 3
 * across the middle block.
 */

s32 Func_02001df0();
s32 Func_02001dfc();
void Func_02001e26();
void Func_02001f3a();
void Func_02001f50();
void Func_02001df2();
void Func_02001dde();
void Func_02001e44();
void Func_02001e30();
void Func_02001e56();
void Func_02001f84();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e34();
void Func_02001e7c();
void Func_02001eaa();
void Func_02001f7e();
void Func_02001e9e();
void Func_02001ecc();
void Func_02001f90();
void Func_02001f9c();
void Func_02001fa6();
void Func_02001fb2();
void Func_02001f74();
void Func_02001ef4();
void Func_02001f22();
void Func_02001f0e();
void Func_02001f24();
void Func_02001f2a();
void Func_02001f4e();

                     

                     

                     

                     

                     

void Func_02000258(void)
{
    s32 scene;

    if (Func_02001df0(0xf01) == 0) {
        return;
    }
    if (Func_02001dfc(0x81a) != 0) {
        return;
    }

    Func_02001e26();
    Func_02001f3a();

    Func_02001f50(182);
    Func_02001df2(0, 70, 30, 42, 1, 1);
    Func_02001dde();
    Func_02001e44(40);

    scene = 0x1032;
    Func_02001e30(scene, 1);
    Func_02001e56(20);

    Func_02001f84(183);
    Func_02001e28(0, 29, 3, 1, 3, 2);
    Func_02001e40(0, 29, 3, 2, 3, 1);
    Func_02001e48(1, 109, 4, 81, 1, 1);
    Func_02001e34();

    Func_02001e7c(0x10000, 0x10000, 0x10000);
    Func_02001eaa(20);
    Func_02001f7e(0, 256, 0);

    Func_02001e9e(0x20000, 0x20000, 0x10000);
    Func_02001ecc(20);

    Func_02001f90(0, 0x4000, 40);
    Func_02001f9c(0, 0x8000, 20);
    Func_02001fa6(0, 0, 20);
    Func_02001fb2(0, 0x4000, 10);

    Func_02001f74(0, 4, 20);
    Func_02001f7e(0, 6, 40);

    Func_02001ef4(-1, -1, 0xe666);
    Func_02001f22(40);

    scene++;
    Func_02001f0e(scene, 1);

    Func_02001f24(0x143);
    Func_02001f2a(0x81a);
    Func_02001f4e();
}
