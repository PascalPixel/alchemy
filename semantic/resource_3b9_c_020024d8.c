#include "types.h"

/*
 * resource_3b9 owner at 0x020024d8, 280 bytes: a scripted-scene bracket
 * around a run of per-id `Func_0808a1b8` position/size calls (ids 0,
 * 8/12-18) with two direct field writes on the id-13 record in the
 * middle.
 *
 * Complete owner: `push {r5, lr}` at 0x020024d8 through `pop {r5} / pop
 * {r0} / bx r0` at 0x020025e4-0x020025e8, followed by the alignment
 * halfword and one-word literal pool 0x020025ea-0x020025ef; the next
 * owner's prologue is exactly at 0x020025f0 (this overlay's own
 * resource_3b9_c_020025f0.c, already drafted). No incoming arguments
 * are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule (`bun tools/overlay_call_targets.ts resource_3b9 24d8 25f0`).
 *
 * SHARED IDIOMS cited from resource_3b9_c_0200160c.c's header and
 * resource_3b9_c_02002668.c's: scripted-scene bracket open/close, and
 * `Func_02001c48` is this overlay's own single-argument trampoline
 * (resource_3b9_c_02001c48.c).
 *
 * The two direct field writes at record13+100 (u16, 720) and
 * record13+102 (u16, 112) are adjacent in the source: the original
 * advances the SAME register by 102 between them rather than
 * recomputing the base, a pointer-mutation shape distinct from the
 * cached-displacement idiom on the 0x03001ebc workspace
 * (resource_3b9_c_02001298.c) -- written here as two independent
 * offset writes since C has no need to preserve the register trick.
 *
 * Uncertainty: none of the eight callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_080000c0(s32 arg0);
extern u8 *Func_0808a080();
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_02001c48(s32 arg0);

void Func_020024d8(void)
{
    u8 *record13 = Func_0808a080(13);

    Func_0808a018();
    Func_0808a360();
    Func_0808a370();

    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a0a8(13);
    Func_080000c0(1);

    Func_0808a1b8(0, 224 << 8, 0);
    Func_0808a100(13, 1);
    Func_0808a1b8(12, 208 << 8, 0);
    Func_0808a1b8(13, 0, 0);
    Func_0808a1b8(14, 128 << 8, 0);
    Func_0808a1b8(15, 208 << 8, 0);
    Func_0808a1b8(16, 128 << 8, 0);
    Func_0808a1b8(17, 176 << 8, 0);
    Func_0808a1b8(18, 176 << 8, 0);

    Func_0808a170(0x2112);
    Func_02001c48(8);
    Func_0808a110(0, 3);

    *(u16 *)(record13 + 100) = 720;   /* 180 << 2 */
    *(u16 *)(record13 + 102) = 112;

    Func_0808a098(13, 2);
    Func_0808a1b8(12, 192 << 6, 0);
    Func_0808a1b8(14, 176 << 8, 0);
    Func_0808a1b8(15, 160 << 7, 0);
    Func_0808a1b8(16, 0, 0);
    Func_0808a1b8(17, 160 << 7, 0);
    Func_0808a1b8(18, 160 << 7, 0);

    Func_0808a020();
}
