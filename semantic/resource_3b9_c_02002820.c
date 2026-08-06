#include "types.h"

/*
 * resource_3b9 owner at 0x02002820, 228 bytes: a flag-gated three-call
 * prelude, then a linear setup run (id-tagged 0/11 calls, several with
 * a shared shifted-constant pair, six with a fixed 588 second argument
 * varying the first) closing with three parameterless calls and an id.
 *
 * Complete owner: `push {lr}` at 0x02002820 through `pop {r0} / bx r0`
 * at 0x020028f4-0x020028f6, followed by the three-word literal pool
 * 0x020028f8-0x02002903; the next owner's prologue is at 0x02002904
 * (this overlay's own resource_3b9_c_02002904.c). No incoming arguments
 * are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * Uncertainty: none of the seventeen resolved callees are identified beyond call
 * shape; `Func_0808a080`'s returned pointer has its own field 6 set to
 * 0x8000 immediately after the call -- the object's identity is not
 * established.
 */



















extern s32 Func_020053e2(s32 arg0);
extern void Func_020053f4(s32 arg0);
extern void Func_02005412(s32 arg0);
extern void Func_02005410(s32 arg0);
extern void Func_02005434(void);
extern void Func_020054a2(s32, s32, s32);
extern void Func_020053e0(s32 arg0);
extern void Func_02005530(s32 arg0, s32 arg1);
extern void Func_0200547a(s32, s32, s32);
extern void Func_02005484(s32, s32, s32);
extern u8 * Func_02005482(s32 arg0);
extern void Func_0200559c(void);
extern void Func_020054e4(s32 arg0, s32 arg1);
extern void Func_020054ec(s32 arg0, s32 arg1);
extern void Func_020054d2(s32, s32, s32);
extern void Func_020054e8(s32, s32, s32);
extern void Func_020054ee(s32, s32, s32);
extern void Func_02005504(s32, s32, s32);
extern void Func_0200550a(s32, s32, s32);
extern void Func_02005518(s32, s32, s32);
extern void Func_0200560c(void);
extern void Func_02005618(void);
extern void Func_020055ee(s32 arg0);
void Func_02002820(void)
{
    u8 *object;

    if (Func_020053e2(5) != 0) {
        Func_020053f4(0x16d);
        Func_02005412(5);
        Func_02005410(3);
    }

    Func_02005434();
    Func_020054a2(11, 0x3640000, 0x24c0000);
    Func_020053e0(1);
    Func_02005530(11, 1);
    Func_0200547a(11, 0x19999, 0xcccc);
    Func_02005484(0, 0x19999, 0xcccc);

    *(u16 *)(object + 6) = 0x8000;
    object = Func_02005482(11);

    Func_0200559c();
    Func_020054e4(0, 2);
    Func_020054ec(11, 2);
    Func_020054d2(0, 800, 588);
    Func_020054e8(11, 768, 588);
    Func_020054ee(0, 700, 588);
    Func_02005504(11, 668, 588);
    Func_0200550a(0, 600, 588);
    Func_02005518(11, 568, 588);
    Func_0200560c();
    Func_02005618();
    Func_020055ee(21);
}
