typedef int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3b9 owner at 0x02000dcc, 992 bytes: the other half of a
 * two-owner story-flag handshake. It is gated on flag 564, sets flag
 * 0x235 the moment it opens, and then either plays a long scene or, if a
 * Func_0808a070 gate says otherwise, bumps the skip-beat counter and
 * leaves.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl spill at
 * 0x02000dcc through `pop {r3, r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02001160-0x0200116a, then the
 * sixteen-word tail literal pool 0x0200116c-0x020011ab. The next owner's
 * prologue is exactly at 0x020011ac. **992 bytes**, measured to the named
 * epilogue.
 *
 * Unlike its two big neighbours this row has NO interior literal pool: the
 * single listing covers all 101 call sites, which equals
 * `overlay_call_targets`' own `sites=` figure for the true bounds. That
 * check is worth running anyway -- 0x02000710 and 0x02001cd4 both hid 40%
 * of themselves behind an interior pool that `overlay_show` stops at, and
 * a complete listing is not something to assume.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`. All 101 targets resolved with
 * `bun tools/overlay_call_targets.ts resource_3b9 dcc 11ac` under the `+2`
 * rule -- 86 veneer sites, 15 to this overlay's own trampolines
 * Func_02001c48/Func_02001c5c, zero intra-body.
 *
 * THE HANDSHAKE, AND IT IS RECIPROCAL. `resource_3b9_c_020011ac.c` is a
 * 24-byte owner that does exactly two things: `Func_080770d0(0x235)`
 * (clear) and `Func_080770c8(564)` (set). This row opens with
 * `Func_080770c0(564)` and, when that is non-zero, immediately calls
 * `Func_080770c8(0x235)`. So the small owner hands off to the large one by
 * setting the flag the large one waits on, and the large one hands back by
 * setting the flag the small one cleared. Neither calls the other and
 * neither appears in the other's call graph; the entire linkage is two
 * flag ids. This is the H3 case in the published-owner taxonomy -- owners
 * wired through STATE -- and it has no tool. The only instrument is
 * grepping story-flag ids across an overlay and lining them up.
 *
 * 564 is built as `movs #141 / lsls #2` at the test here and at the set in
 * 0x020011ac, while its neighbour 0x235 (565) is a pool word in both. Two
 * adjacent ids, two encodings, in two different functions: the compiler
 * picks per VALUE, not per meaning.
 *
 * THE GATE RESULT IS ALSO A STORED VALUE, and it is only zero because of
 * the branch that reached it. `r7` takes the return of
 * `Func_0808a070(0, 0)` at 0x02000e5c, the `beq`-over-`b` at 0x02000e60
 * selects the ZERO arm, and then r7 is stored six times into the record at
 * 0x020010ac-0x020010ba. Reading those as "stores the gate result" is
 * true and useless; they are stores of 0, and only the control flow says
 * so. `resource_3b9_c_020011c4.c` does the same thing with the
 * Func_080770c0 result, so this is a habit of the overlay, not a one-off.
 *
 * THE SKIP-BEAT COUNTER FIRES ONLY ON THE NON-ZERO ARM. There is no
 * duplicate bump on the other path, so `*(u16 *)(workspace + 472)`
 * advances once or not at all. That is a third shape for this counter in
 * one overlay: 0x02001cd4 converges so it advances exactly once either
 * way, 0x0200039c advances once OR twice, and this row advances once or
 * zero times. Do not carry the shape between rows.
 *
 * TWO ARMS, TWO DIFFERENT CALLEES, ONE ARGUMENT. The opening block calls
 * `Func_0808a178(0x8008, 0)` and the non-zero arm calls
 * `Func_0808a180(0x8008, 0)` -- same pool word, adjacent veneers,
 * genuinely different functions. Resolved per site with `targetOffset`;
 * nothing in the raw listing distinguishes them, because the `bl`
 * encodings differ at every site in this overlay whether the callee does
 * or not.
 *
 * Func_02001c48 IS NOT ONLY CALLED WITH SMALL ACTOR IDS HERE. Four of its
 * calls pass pool words -- 0x400c, 0x4011, 0x4012, 0x800b -- alongside the
 * ordinary `Func_02001c48(8)` and `(15)`. Whatever the trampoline's
 * argument is, it is wider than the 0-20 id space the other rows use it
 * with, and the two forms are interleaved in one function. Recorded as
 * passed values.
 *
 * The `Func_0808a080(8)` byte at +90 is cleared of bit 0, a one-frame wait
 * runs, and the bit is set again -- a read/modify/write pair around
 * Func_0808a0d0. The AND is emitted mask-first (`movs r3,#254 / ands
 * r3,r2`), which is §4's mask-first rule showing up in a semantic row.
 * Func_0808a080(13)'s record then gets three words zeroed at +36/+40/+44,
 * three set to 0x80000000 at +56/+60/+64, a zero word at +108 and two zero
 * halfwords at +100/+102.
 *
 * Four pool words are 0x02008000-based image data pointers: 0x0200ae20,
 * 0x0200aed4, 0x0200ae5c and 0x0200af24 (image 0x2e20, 0x2ed4, 0x2e5c,
 * 0x2f24). All EVEN, so descriptors and not published function pointers --
 * do not feed them to the published sweep.
 *
 * SHARED IDIOMS: scripted-scene bracket, the trampoline pair, and the
 * Func_0808a098/Func_0808a0b0 `(id, descriptor)` pair are cited from
 * resource_3b9_c_02002964.c and resource_3b9_c_0200160c.c; the skip-beat
 * counter from resource_3b9_c_0200039c.c.
 *
 * Uncertainty: none of the twenty-eight callees are identified beyond call
 * shape; every id, coordinate, flag and record offset below is recorded as
 * a passed or written value. What Func_0808a070(0, 0) reports is unknown.
 */

extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0b0(s32 id, void *data);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a248(s32 arg0);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_02001c48(s32 id);
extern void Func_02001c5c(s32 id, s32 arg1);

extern u8 *Data_03001ebc;

void Func_02000dcc(void)
{
    s32 gate;
    u8 *bits;
    u8 *record;
    u8 *workspace;

    if (Func_080770c0(564) == 0)
        return;

    Func_080770c8(0x235);

    Func_0808a018();
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 218 << 2, 120);
    Func_02001c5c(0, 160 << 8);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(204 << 18, -1, 224 << 15, 1);
    Func_0808a218();
    Func_0808a1b8(8, 192 << 6, 20);
    Func_0808a138(8, 1);
    Func_0808a170(0x2125);
    Func_0808a178(0x8008, 0);

    gate = Func_0808a070(0, 0);
    if (gate != 0) {
        goto skipped_scene;
    }

    {
        Func_0808a010(20);
        Func_02001c5c(12, 208 << 8);
        Func_0808a138(12, 1);
        Func_0808a170(0x212b);
        Func_02001c48(0x400c);
        Func_02001c5c(17, 0);
        Func_02001c5c(0, 128 << 8);
        Func_0808a110(17, 3);
        Func_0808a110(0, 3);
        Func_0808a100(15, 1);
        Func_0808a1b8(15, 208 << 8, 20);
        Func_0808a138(15, 1);
        Func_02001c48(15);
        Func_02001c5c(16, 128 << 8);
        Func_0808a110(16, 3);
        Func_0808a138(17, 2);
        Func_02001c5c(17, 160 << 8);
        Func_02001c48(0x4011);
        Func_02001c5c(18, 176 << 8);
        Func_0808a130(18, 2);
        Func_02001c48(0x4012);
        Func_02001c5c(11, 192 << 6);
        Func_0808a1f0(11, 129 << 1);
        Func_0808a010(60);
        Func_0808a130(11, 2);
        Func_02001c48(0x800b);
        Func_0808a0a8(13);
        Func_080000c0(1);
        Func_0808a130(13, 2);
        Func_0808a130(14, 2);
        Func_0808a138(16, 2);
        Func_0808a010(20);
        Func_0808a1b8(13, 0, 0);
        Func_0808a1b8(14, 128 << 8, 0);
        Func_0808a1b8(16, 176 << 8, 40);
        Func_0808a100(13, 3);
        Func_0808a100(14, 3);
        Func_0808a110(16, 3);
        Func_0808a090(13, 0x19999, 0xcccc);
        Func_0808a090(14, 0x19999, 0xcccc);
        Func_0808a090(16, 0x19999, 0xcccc);
        Func_0808a098(13, (void *)0x0200ae20);
        Func_0808a098(16, (void *)0x0200aed4);
        Func_0808a010(20);
        Func_0808a1b8(15, 208 << 8, 0);
        Func_0808a1b8(17, 176 << 8, 0);
        Func_0808a1b8(0, 160 << 8, 0);
        Func_0808a1b8(12, 208 << 8, 0);
        Func_0808a1b8(18, 176 << 8, 0);
        Func_0808a0b0(14, (void *)0x0200ae5c);
        Func_0808a010(20);
        Func_0808a1b8(8, 0, 0);
        Func_0808a1b8(11, 128 << 8, 40);
        Func_0808a100(8, 3);
        Func_0808a110(11, 3);
        Func_0808a210(202 << 18, -1, 172 << 15, 1);
        Func_0808a218();
        Func_0808a138(8, 2);
        Func_02001c48(8);
        Func_0808a090(8, 128 << 9, 128 << 8);

        bits = Func_0808a080(8) + 90;
        *bits = (u8)(0xfe & *bits);
        Func_0808a0d0(8, 198 << 2, 72);
        Func_0808a010(1);
        bits = Func_0808a080(8) + 90;
        *bits = (u8)(1 | *bits);
        Func_02001c5c(8, 0);

        Func_0808a0a8(13);
        record = Func_0808a080(13);
        *(s32 *)(record + 108) = gate;
        *(u16 *)(record + 100) = (u16)gate;
        *(u16 *)(record + 102) = (u16)gate;
        *(s32 *)(record + 36) = gate;
        *(s32 *)(record + 40) = gate;
        *(s32 *)(record + 44) = gate;
        *(s32 *)(record + 56) = (s32)0x80000000;
        *(s32 *)(record + 60) = (s32)0x80000000;
        *(s32 *)(record + 64) = (s32)0x80000000;
        Func_080000c0(1);

        Func_0808a098(15, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(13, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(17, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(14, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(16, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(12, (void *)0x0200af24);
        Func_0808a010(20);
        Func_0808a098(18, (void *)0x0200af24);
        Func_0808a010(60);
        Func_0808a098(0, (void *)0x0200af24);
        Func_0808a010(80);
        Func_0808a248(66);
    }

    goto close;

skipped_scene:
    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;
    Func_0808a180(0x8008, 0);

close:
    Func_0808a020();
}
