#include "types.h"

/*
 * Resource 37f, owner at 0x02000258 (334 bytes of code, prologue
 * `push {r5, r6, lr}` + the r8 spill + `sub sp, #8` at 0x02000258 through the
 * interworking return at 0x0200039c, plus a five-word literal pool at
 * 0x020003a8-0x020003bb).  r0 is the popped return address, so `void`.
 *
 * All 32 call sites were resolved with `tools/overlay_call_targets.ts`; all 32
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

















extern s32 Func_02001df0();
extern s32 Func_02001dfc();
extern void Func_02001e26();
extern void Func_02001f3a();
extern void Func_02001f50();
extern void Func_02001df2();
extern void Func_02001dde();
extern void Func_02001e44();
extern void Func_02001e30();
extern void Func_02001e56();
extern void Func_02001f84();
extern void Func_02001e28();
extern void Func_02001e40();
extern void Func_02001e48();
extern void Func_02001e34();
extern void Func_02001e7c();
extern void Func_02001eaa();
extern void Func_02001f7e();
extern void Func_02001e9e();
extern void Func_02001ecc();
extern void Func_02001f90();
extern void Func_02001f9c();
extern void Func_02001fa6();
extern void Func_02001fb2();
extern void Func_02001f74();
extern void Func_02001ef4();
extern void Func_02001f22();
extern void Func_02001f0e();
extern void Func_02001f24();
extern void Func_02001f2a();
extern void Func_02001f4e();
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
