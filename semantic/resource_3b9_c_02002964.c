#include "types.h"

/*
 * resource_3b9 owner at 0x02002964, 440 bytes: another scripted-scene
 * bracket -- three actor-position setup calls, this overlay's own
 * id-8/9/10/11 trampoline runs, a block of `Func_0808a1b8`/
 * `Func_0808a090`/`Func_0808a098` position/animation calls sharing a
 * pool pointer, then a closing run touching ids 0/11/60/64. No branches.
 *
 * Complete owner: `push {r5, lr}` at 0x02002964 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02002b00-0x02002b04, followed by the five-word
 * literal pool 0x02002b06-0x02002b1b; the next owner's prologue is
 * exactly at 0x02002b1c (this overlay's own resource_3b9_c_02002b1c.c,
 * already drafted). No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2`
 * rule (`bun tools/lib/overlay_call_targets.ts resource_3b9 2964 2b1c`).
 *
 * SHARED IDIOMS: scripted-scene bracket open/close and the id-8/9/10/11
 * trampoline pair (Func_02001c48/Func_02001c5c) cited from
 * resource_3b9_c_0200160c.c and resource_3b9_c_02002668.c's headers.
 * `Func_0808a210(0x1e5a0000, -1, 0x137d000, 1)`'s call shape --
 * big-shifted, -1, big-shifted, 1 -- matches
 * resource_38b_c_02000584.c's `Func_0808a210(0x01070000, -1, 0x00ad0000,
 * 1)` exactly (same import, same argument pattern, different scene).
 * `Func_0808a208`'s two-raw-constant-pair shape likewise matches that
 * file's `Func_0808a208(0x00026666, 0x00004ccc)`.
 *
 * Func_0808a098 and the new Func_0808a0b0 both take (id, pointer) --
 * 0x0200adac is an in-image data address (even, EVEN-parity link-base
 * witness per resource_397_c_0200005c.c's rule), the same "animation
 * script pointer" shape as Func_0808a098(16, 0x0200af88) in
 * resource_3b9_c_02001298.c.
 *
 * Uncertainty: none of the fourteen callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */

extern void Func_0808a018(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a0b0(s32 id, void *data);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a248(s32 arg0);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_02002964(void)
{
    void *shared = (void *)0x0200adac;

    Func_0808a018();
    Func_0808a0f0(1, 198 << 18, 136 << 16);
    Func_0808a0f0(2, 206 << 18, 136 << 16);
    Func_0808a0f0(3, 202 << 18, 152 << 16);
    Func_0808a360();
    Func_0808a370();

    Func_0808a010(40);
    Func_0808a138(8, 1);
    Func_0808a100(8, 3);
    Func_0808a170(0x2138);
    Func_02001c48(8);
    Func_0808a138(9, 1);
    Func_02001c48(9);
    Func_0808a138(10, 1);
    Func_02001c48(10);
    Func_0808a138(11, 1);
    Func_0808a100(11, 3);
    Func_02001c48(11);

    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 20);
    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 128 << 9, 128 << 8);
    Func_0808a098(1, shared);
    Func_0808a098(2, shared);
    Func_0808a0b0(3, shared);

    Func_0808a010(20);
    Func_02001c5c(0, 0);
    Func_0808a110(0, 3);
    Func_0808a110(11, 3);
    Func_0808a090(11, 128 << 9, 128 << 8);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a100(11, 2);
    Func_0808a0c0(11, 202 << 2, 152);
    Func_0808a0c0(11, 202 << 2, 164);
    Func_0808a0b8(11, 202 << 2, 156 << 1);

    Func_0808a010(20);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(202 << 18, -1, 156 << 17, 1);
    Func_0808a0d0(0, 202 << 2, 164);
    Func_0808a0c8(0, 202 << 2, 156 << 1);

    Func_0808a010(60);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(64);
}
