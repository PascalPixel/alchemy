#include "types.h"

/*
 * Resource 37f, owner at 0x02000420 (310 bytes of code, prologue
 * `push {r5, r6, lr} / sub sp, #8` at 0x02000420 through the interworking
 * return `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x0200054e, plus a
 * five-word literal pool at 0x02000558-0x0200056b).  r0 is the popped return
 * address, so the owner is `void`.
 *
 * All 31 call sites were resolved with `tools/overlay_call_targets.ts`; all 31
 * are import veneers.  The inventory's `calls=29` is the documented undercount
 * of that field (it counts distinct targets under the old decoding), and the
 * tool's `sites=31` is used here instead.
 *
 * Control flow is a gate followed by one straight-line cutscene: the two
 * `Func_080770c0` tests both jump to the shared epilogue, so there is no
 * branching inside the body and every call below appears exactly once.
 *
 * Flags 0xf02 / 0x821 are the same pair its sibling 0x020003bc tests, and the
 * scene ends by setting 0x143 and 0x821 through Func_080770c8 — i.e. this is
 * the one-shot body whose gate 0x821 it sets on the way out.
 *
 * Argument notes: Func_08009180 and Func_080091c0 are the six-argument
 * renderer-family calls, arguments 5 and 6 being [sp, #0] and [sp, #4].  r5
 * holds the constant 1 across the owner and r6 the scene id 0x1032, bumped to
 * 0x1033 before the second Func_08015040.  All the large operands are built by
 * shifts (0x80 << 9 = 0x10000 and so on) rather than pooled.
 */

















extern s32 Func_02001fb4();
extern s32 Func_02001fc0();
extern void Func_02001fea();
extern void Func_020020fe();
extern void Func_02002114();
extern void Func_02001fb6();
extern void Func_02001fa2();
extern void Func_02002008();
extern void Func_02001ff2();
extern void Func_02002018();
extern void Func_02002146();
extern void Func_02001fe8();
extern void Func_02002004();
extern void Func_02002030();
extern void Func_0200205e();
extern void Func_02002132();
extern void Func_02002052();
extern void Func_02002080();
extern void Func_02002144();
extern void Func_02002150();
extern void Func_0200215a();
extern void Func_02002166();
extern void Func_02002128();
extern void Func_020020a8();
extern void Func_020020d8();
extern void Func_020020c0();
extern void Func_020020d6();
extern void Func_020020dc();
extern void Func_02002100();
void Func_02000420(void)
{
    s32 scene;

    if (Func_02001fb4(0xf02) == 0) {
        return;
    }
    if (Func_02001fc0(0x821) != 0) {
        return;
    }

    Func_02001fea();
    Func_020020fe();

    Func_02002114(182);
    Func_02001fb6(0, 71, 100, 71, 1, 1);
    Func_02001fa2();
    Func_02002008(40);

    Func_02001ff2(scene, 1);
    scene = 0x1032;
    Func_02002018(20);

    Func_02002146(183);
    Func_02001fe8(122, 20, 120, 30, 1, 2);
    Func_02002004(122, 20, 1, 2, 120, 30);
    Func_02001fe8();

    Func_02002030(0x10000, 0x10000, 0x10000);
    Func_0200205e(20);
    Func_02002132(0, 256, 0);

    Func_02002052(0x20000, 0x20000, 0x10000);
    Func_02002080(20);

    Func_02002144(0, 0x4000, 40);
    Func_02002150(0, 0x8000, 20);
    Func_0200215a(0, 0, 20);
    Func_02002166(0, 0x4000, 10);

    Func_02002128(0, 4, 20);
    Func_02002132(0, 6, 40);

    Func_020020a8(-1, -1, 0xe666);

    scene++;
    Func_020020d8(40);
    Func_020020c0(scene, 1);

    Func_020020d6(0x143);
    Func_020020dc(0x821);
    Func_02002100();
}
