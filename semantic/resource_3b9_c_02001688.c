#include "types.h"

/*
 * resource_3b9 owner at 0x02001688, 908 bytes: the largest owner in
 * this overlay, and the only one of the six large linear-setup siblings
 * with a real loop. Same scripted-scene-bracket family as
 * resource_3b9_c_02001298.c (884 bytes, byte-exact-adjacent sibling
 * two owners back); drafted for the same reason -- mechanically simple,
 * one loop and no other branches, unlike the two rows still parked for
 * a dedicated pass.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02001688 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x020019dc-0x020019e0, followed by
 * eight words of literal pool 0x020019e2-0x02001a13; the next owner is
 * NOT the recorded row at 0x02001a4c -- there is a previously
 * unindexed 56-byte owner at 0x02001a14 in between (this overlay's own
 * resource_3b9_c_02001a14.c, found while confirming this pool's true
 * end). No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3b9
 * 1688 1a4c`'s `+2` rule.
 *
 * SHARED IDIOMS cited from resource_3b9_c_0200160c.c's and
 * resource_3b9_c_02001298.c's headers: scripted-scene bracket
 * open/close, wait-n-frames, and the additive/subtractive workspace
 * write pair (here computed by re-adding the workspace base each time
 * rather than caching a displacement in r8/r9/sl -- this owner's
 * prologue only saves r5-r7, no r8/r9/sl -- same result, simpler
 * register use: workspace+448 = 512 then 513, workspace+456 = 32 then
 * 16, matching resource_3b9_c_02001298.c's own two write blocks
 * exactly).
 *
 * `Func_080091f0`/`Func_080091f8` are a known pair
 * (resource_372_c_02000d5c.c): the second call here,
 * `Func_080091f0(-1, -1, 0xe666); Func_080091f8();`, is IDENTICAL to
 * that file's own call -- same import, same three arguments, same
 * no-argument follow-up.
 *
 * THE LOOP (0x020018b2-0x020018d2, 20 iterations, r5 = 0..19 inclusive,
 * unsigned `bls`): each pass adds two pool constants to the id-21
 * record's +12 field in turn, +0x9999 then -0x4ccc (0xffffb334 as
 * s32), with a `Func_080000c0(4)` frame-advance after each add -- a
 * slow oscillating counter animated at 4 ticks per half-step, 40 ticks
 * total. Field +85 of the same record is a byte flag: 0 before the
 * loop, set to 3 afterward, both direct writes with no read.
 *
 * Uncertainty: none of the callees are identified beyond call shape;
 * every id/constant is recorded as a passed value; the struct offsets
 * (+6, +12, +24, +28, +85) are raw byte offsets, no layout established.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern u8 *Func_0808a080();
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_02001c5c(s32 arg0, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080091f8(void);
extern void Func_080770c8(s32 flag_id);
extern void Func_0808a248(s32 arg0);

void Func_02001688(void)
{
    u8 *workspace = Data_03001ebc;
    u8 *record;
    u8 *record21;
    s32 i;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080f9010(247);

    Func_0808a100(8, 2);
    Func_0808a100(9, 2);
    Func_0808a100(10, 2);
    Func_0808a100(11, 2);
    Func_0808a100(12, 2);
    Func_0808a100(13, 2);
    Func_0808a100(14, 0);
    Func_0808a100(15, 0);
    Func_0808a0f0(16, 0, 0);
    Func_0808a100(17, 0);
    Func_0808a100(18, 0);

    Func_080091e0(Func_0808a080(21), 0);

    record = Func_0808a080(19);
    *(s32 *)(record + 24) = (s32)0xffff0000;
    record = Func_0808a080(20);
    *(s32 *)(record + 24) = (s32)0xffff0000;

    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_080000c0(1);

    *(s32 *)(workspace + 448) = 512;   /* additive: 224<<1 + 64 */
    *(s32 *)(workspace + 456) = 32;    /* subtractive: 512 - 56 */

    Func_0808a360();
    Func_0808a370();

    Func_0808a010(40);
    Func_0808a138(17, 1);
    Func_0808a010(20);
    Func_0808a090(17, 0xcccc, 0x6666);
    Func_0808a0d0(17, 164, 226 << 2);
    Func_0808a010(20);
    Func_0808a100(17, 9);
    Func_0808a010(40);
    Func_0808a100(17, 10);
    Func_0808a010(60);
    Func_0808a100(17, 1);
    Func_0808a010(20);
    Func_0808a0d0(17, 164, 230 << 2);
    Func_0808a0d0(17, 185, 230 << 2);
    Func_0808a1b8(17, 192 << 8, 20);
    Func_0808a0d0(17, 185, 229 << 2);
    Func_0808a100(17, 11);
    Func_0808a010(40);
    Func_0808a138(17, 1);
    Func_0808a010(60);
    Func_0808a138(17, 3);
    Func_0808a010(40);
    Func_0808a098(17, (void *)0x0200af88);
    Func_0808a010(80);
    Func_0808a1f0(17, 129 << 1);
    Func_0808a010(60);

    Func_0808a1b8(14, 208 << 8, 0);
    Func_0808a1b8(15, 160 << 7, 0);
    Func_0808a1b8(18, 128 << 8, 20);
    Func_0808a1f0(14, 129 << 1);
    Func_0808a1f0(15, 129 << 1);
    Func_0808a1f0(17, 129 << 1);
    Func_0808a1f0(18, 129 << 1);

    Func_0808a010(60);
    Func_0808a1b8(14, 192 << 6, 0);
    Func_0808a1b8(15, 192 << 6, 0);
    Func_02001c5c(18, 192 << 6);
    Func_0808a1f0(17, 0x101);

    record21 = Func_0808a080(21);
    *(u8 *)(record21 + 85) = 0;

    for (i = 0; i <= 19; i++) {
        *(s32 *)(record21 + 12) += 0x9999;
        Func_080000c0(4);
        *(s32 *)(record21 + 12) += (s32)0xffffb334;   /* -0x4ccc */
        Func_080000c0(4);
    }

    Func_0808a100(19, 6);
    Func_0808a100(20, 6);
    Func_0808a010(60);
    Func_0808a1f0(17, 128 << 1);
    Func_0808a0a8(17);
    Func_0808a100(17, 1);

    record = Func_0808a080(17);
    *(u16 *)(record + 6) = 208 << 8;
    *(u8 *)(record21 + 85) = 3;
    *(s32 *)(record21 + 24) = 128 << 9;
    *(s32 *)(record21 + 28) = 128 << 9;

    Func_0808a010(10);
    Func_080f9010(107);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(10);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    Func_0808a090(17, 0x19999, 0xcccc);
    Func_0808a0d0(17, 208, 232 << 2);
    Func_080f9010(92);
    Func_0808a1b8(17, 192 << 6, 20);
    Func_0808a100(17, 9);
    Func_0808a010(20);
    Func_0808a100(17, 10);
    Func_0808a010(40);
    Func_0808a100(17, 9);
    Func_0808a010(20);
    Func_0808a100(17, 10);
    Func_0808a010(80);

    *(s32 *)(workspace + 448) = 513;   /* additive: 224<<1 + 65 */
    *(s32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();
    Func_080770c8(0x8a4);
    Func_0808a248(69);
    Func_0808a020();
}
