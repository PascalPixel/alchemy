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
 * by `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * Uncertainty: none of the seventeen resolved callees are identified beyond call
 * shape; `Func_0808a080`'s returned pointer has its own field 6 set to
 * 0x8000 immediately after the call -- the object's identity is not
 * established.
 */

extern s32 Func_080770c0(s32 arg0);
extern void Func_080770c8(s32 arg0);
extern void Func_08077168(s32 arg0);
extern void Func_08077150(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a200(s32 arg0, s32 arg1);
extern void Func_0808a090(s32, s32, s32);
extern u8 *Func_0808a080(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a0b8(s32, s32, s32);
extern void Func_0808a0c0(s32, s32, s32);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a248(s32 arg0);

void Func_02002820(void)
{
    u8 *object;

    if (Func_080770c0(5) != 0) {
        Func_080770c8(0x16d);
        Func_08077168(5);
        Func_08077150(3);
    }

    Func_0808a018();
    Func_0808a0f0(11, 0x3640000, 0x24c0000);
    Func_080000c0(1);
    Func_0808a200(11, 1);
    Func_0808a090(11, 0x19999, 0xcccc);
    Func_0808a090(0, 0x19999, 0xcccc);

    *(u16 *)(object + 6) = 0x8000;
    object = Func_0808a080(11);

    Func_0808a360();
    Func_0808a100(0, 2);
    Func_0808a100(11, 2);
    Func_0808a0b8(0, 800, 588);
    Func_0808a0c0(11, 768, 588);
    Func_0808a0b8(0, 700, 588);
    Func_0808a0c0(11, 668, 588);
    Func_0808a0b8(0, 600, 588);
    Func_0808a0b8(11, 568, 588);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(21);
}
