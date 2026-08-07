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

s32 Func_080770c0();
void Func_080770c8();
void Func_0808a018();
void Func_0808a460();
void Func_080f9010();
void Func_08009180();
void Func_08009128();
void Func_0808a010();
void Func_08015040();
void Func_080091c0();
void Func_080091f0();
void Func_0808a1e8();
void Func_0808a1b8();
void Func_0808a128();
void Func_0808a020();

void Func_02000258(void)
{
    s32 scene;

    if (Func_080770c0(0xf01) == 0) {
        return;
    }
    if (Func_080770c0(0x81a) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a460();

    Func_080f9010(182);
    Func_08009180(0, 70, 30, 42, 1, 1);
    Func_08009128();
    Func_0808a010(40);

    scene = 0x1032;
    Func_08015040(scene, 1);
    Func_0808a010(20);

    Func_080f9010(183);
    Func_08009180(0, 29, 3, 1, 3, 2);
    Func_080091c0(0, 29, 3, 2, 3, 1);
    Func_08009180(1, 109, 4, 81, 1, 1);
    Func_08009128();

    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(20);
    Func_0808a1e8(0, 256, 0);

    Func_080091f0(0x20000, 0x20000, 0x10000);
    Func_0808a010(20);

    Func_0808a1b8(0, 0x4000, 40);
    Func_0808a1b8(0, 0x8000, 20);
    Func_0808a1b8(0, 0, 20);
    Func_0808a1b8(0, 0x4000, 10);

    Func_0808a128(0, 4, 20);
    Func_0808a128(0, 6, 40);

    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    scene++;
    Func_08015040(scene, 1);

    Func_080770c8(0x143);
    Func_080770c8(0x81a);
    Func_0808a020();
}
