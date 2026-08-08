#include "types.h"

/*
 * resource_3c9 owner at 0x020012c8, 3604 bytes: the largest owner in
 * this overlay and the larger of its two remaining residues -- a long
 * scripted cutscene driver over the same actor-id/camera call family
 * the rest of resource_3c9 uses, with 363 call sites, one 40-iteration
 * loop, and two copies of one two-armed branch.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,sl / mov r6,r8 /
 * push {r6, r7}` at 0x020012c8-0x020012cf (no stack frame -- every call
 * in the row is four arguments or fewer) through `pop {r3, r5} / mov
 * r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020020ae-0x020020b9, a two-byte alignment halfword at 0x020020ba,
 * and the trailing eight-word literal pool 0x020020bc-0x020020db; the
 * next owner's prologue is exactly at 0x020020dc (this overlay's own
 * resource_3c9_c_020020dc.c, already drafted -- and called from inside
 * this row's loop, which confirms the bound from the other direction).
 * No incoming arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 12c8 20dc`'s `+2` rule, which reports sites=363 distinct_targets=41
 * veneer=280 prologue=83 over exactly these bounds.
 *
 * THREE INTERIOR LITERAL POOLS, each jumped over rather than fallen
 * into, which is why a naive read of this row stops roughly a third of
 * the way in. They are at 0x020016c8-0x020016ff (14 words, skipped by
 * the `b.n 0x02001700` at 0x020016c6), 0x02001a30-0x02001a4f (8 words,
 * skipped by the `bne.n 0x02001a50` at 0x02001a1c and by the `b.n
 * 0x02001a70` at 0x02001a2e), and 0x02001e54-0x02001e73 (8 words,
 * skipped by the `b.n 0x02001e74` at 0x02001e50). `overlay_show` also
 * prints a spurious pool footer for 0x02001d7c: the halfword at
 * 0x02001a48 is the pool word 0x00004ccc, objdump renders it as `ldr
 * r4,[pc,#816]`, and the tool then chases that fake load. 0x02001d7c is
 * ordinary code (`lsls r1,r1,#8`). Only the four pools listed above are
 * real.
 *
 * Structurally: eight conditional branches in 3570 bytes of code, and
 * six of them belong to three repeated shapes -- three id-0 record
 * null-guards at 0x0200137e/0x02001392/0x020013a6, and two copies of
 * the same `Func_0808a070(0, 0)` two-armed test (0x02001968 and
 * 0x02001a1c) each followed by a `if (taken) counter++` re-test
 * (0x020019a4 and 0x02001a78). The remaining two are the 40-iteration
 * loop's `bls.n` at 0x020016b6 and nothing else. Of the four
 * unconditional `b.n`, three are the pool skips above and the fourth
 * (0x02001a2e) is the join of the second two-armed test.
 *
 * Pool pointers, resolved with the tree's existing base + 0x8000 rule
 * (tracked HANDOVER, "In-image pointers are spelled base + 0x8000") --
 * no new rule here, only its application. The image is 26464 bytes and
 * is linked at 0x02008000, so the word 0x0200a351 at 0x020016fc is
 * offset 0x2351 = this overlay's own owner 0x02002350 with the Thumb
 * bit set (resource_3c9_c_02002350.c, already drafted, and a `push
 * {lr}` prologue at that exact offset); it is handed to Func_080000d0
 * and then to Func_080000d8, which is a callback registration.
 * 0x0200e004 and 0x0200e03c are offsets 0x6004 and 0x603c, inside the
 * image's data region. All three are written here as the raw pool
 * values, matching this overlay's other drafts.
 *
 * SHARED IDIOMS with the rest of resource_3c9: the `Func_0808a080(id)`
 * record fetch followed by a `+90` bit edit (clear bit 0 / set bit 0),
 * the `Func_0808a098(id, ptr)` run over a shared cached pointer, and
 * the `Data_03001ebc` workspace +472 halfword counter. Actor ids used
 * here are 0, 1, 2, 3, 6, 19, 20 and 21 throughout.
 *
 * Observed but NOT interpreted: several ids reach Func_02000894 and
 * Func_0808a1e8/Func_0808a188 with a high bit set -- 0x2013 and 0x2014
 * alongside plain 19 and 20, and 0x101/0x103/0x105 alongside plain
 * 1/3/5. The pairing is exact in every case, but nothing in this row
 * establishes what the 0x2000 and 0x0100 bits select, so they are
 * written as the literal values the pool holds.
 *
 * Uncertainty: none of the 41 callees are identified beyond call shape.
 * Record offsets +6 (halfword), +85 and +90 (bytes) are raw offsets, no
 * layout established. The absolute byte cell written as 3 near the end
 * is spelled `0x02000240 + 0x22b` because the source computes it in two
 * steps from two separate pool words rather than one; it is below the
 * load base, so it is a real absolute address and not an image offset.
 */

extern u8 *Data_03001ebc;

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 arg0, s32 arg1);
extern void Func_080000d8(s32 arg0);
extern void Func_08015040(s32 arg0, s32 arg1);
extern s32 Func_08077040(s32 flagId);
extern void Func_08077048(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0b0(s32 id, s32 arg1);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 *Func_0808a228(void);
extern void Func_0808a250(s32 group, s32 arg1);
extern void Func_0808a260(s32 group, s32 arg1);
extern void Func_0808a268(s32 group, s32 arg1);
extern void Func_02000894(s32 arg0);
extern void Func_020008a8(s32 arg0, s32 arg1);
extern void Func_020020dc(void *record);
extern void Func_02002334(void);
extern void Func_02002344(void);

void Func_020012c8(void)
{
    u8 *record;
    s32 i;

    Func_0808a018();
    *(u8 *)(Func_0808a228() + 85) = 0;
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(166 << 17, 128 << 14, 180 << 16, 1);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 170 << 1, 184);
    Func_020008a8(0, 128 << 8);
    Func_0808a138(21, 1);
    Func_0808a170(0x278e);
    Func_02000894(0x9015);

    *(u8 *)(Func_0808a228() + 85) = 0;
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(152 << 17, 128 << 14, 180 << 16, 1);
    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 0x16666, 0xb333);

    /* the same id-0 record is re-fetched and null-guarded three times */
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_0808a0c8(1, 164 << 1, 168);
    Func_0808a0c8(2, 170 << 1, 196);
    Func_0808a0d0(3, 163 << 1, 204);

    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_020008a8(3, 128 << 8);
    Func_0808a1b8(20, 0, 0);
    Func_0808a1b8(19, 0, 40);
    Func_0808a138(20, 2);
    Func_02000894(20);
    Func_020008a8(19, 128 << 8);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x103, 20);
    Func_02000894(21);
    Func_0808a130(21, 2);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 208 << 8, 40);
    Func_02000894(21);
    Func_0808a110(21, 4);
    Func_020008a8(21, 0);
    Func_0808a188(21, 0, 20);
    Func_0808a138(20, 1);
    Func_020008a8(20, 128 << 8);
    Func_02000894(0x2014);
    Func_0808a1f0(3, 129 << 1);
    Func_0808a130(3, 2);
    Func_02000894(3);
    Func_0808a138(2, 1);
    Func_02000894(2);
    Func_020008a8(19, 0);
    Func_02000894(19);
    Func_0808a1e8(20, 131 << 1, 40);
    Func_0808a1b8(20, 0, 20);
    Func_02000894(20);

    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a1b8(21, 128 << 8, 0);
    Func_020008a8(19, 192 << 6);
    Func_0808a1e8(19, 129 << 1, 40);
    Func_02000894(19);
    Func_020008a8(20, 176 << 8);
    Func_02000894(0x2014);
    Func_0808a1b8(20, 128 << 8, 20);
    Func_02000894(0x2014);
    Func_0808a1b8(19, 128 << 8, 0);
    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 0);
    Func_0808a1b8(21, 0, 20);
    Func_0808a1e8(6, 0x101, 40);
    Func_02000894(6);
    Func_0808a1e8(20, 0x103, 20);
    Func_02000894(0x2014);
    Func_0808a138(6, 2);
    Func_0808a010(20);
    Func_0808a100(6, 3);
    Func_02000894(6);
    Func_0808a110(20, 3);
    Func_02000894(0x2014);
    Func_0808a090(6, 0xcccc, 0x6666);
    Func_0808a0d0(6, 130 << 1, 186);
    Func_0808a1b8(21, 192 << 6, 0);
    Func_0808a0d0(6, 138 << 1, 192);

    *(u16 *)(Func_0808a080(19) + 6) = 160 << 7;
    Func_080000c0(1);
    Func_0808a130(19, 2);
    Func_02000894(0x2013);
    Func_0808a128(6, 2, 20);
    Func_0808a090(6, 0x26666, 0x13333);
    Func_0808a0d0(6, 130 << 1, 186);
    Func_0808a1b8(21, 128 << 8, 0);
    Func_0808a0d0(6, 248, 172);
    Func_0808a1b8(19, 128 << 8, 0);
    Func_0808a1b8(21, 128 << 8, 0);
    Func_0808a1b8(6, 0, 20);
    Func_0808a110(6, 3);
    Func_0808a010(40);
    Func_02002334();
    Func_080000c0(1);

    /* 40 iterations: the id-6 record is stepped by the next owner */
    for (i = 0; i <= 39; i++) {
        Func_020020dc(Func_0808a080(6));
        Func_080000c0(1);
    }

    Func_080000d0(0x0200a351, 200 << 4);
    Func_0808a010(80);

    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 20);
    Func_0808a1b8(21, 0, 40);
    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 0);
    Func_020008a8(21, 128 << 8);
    Func_0808a1e8(20, 0x101, 40);
    Func_0808a188(0x2014, 0, 20);
    Func_0808a1e8(6, 0x105, 80);
    Func_0808a130(19, 2);
    Func_02000894(0x2013);
    Func_080000d8(0x0200a351);
    Func_080000c0(1);
    Func_0808a158(6, 0);
    Func_080000c0(10);
    Func_02002344();
    Func_0808a128(6, 2, 40);
    Func_02000894(6);
    Func_0808a1e8(20, 0x103, 20);
    Func_02000894(0x2014);
    Func_0808a138(6, 2);

    *(u8 *)(Func_0808a080(6) + 90) &= 0xfe;
    Func_0808a0d0(6, 250, 176);
    Func_0808a010(1);
    *(u8 *)(Func_0808a080(6) + 90) |= 1;

    Func_0808a1e8(21, 0x103, 20);
    Func_0808a1b8(21, 0, 20);
    Func_02000894(21);
    Func_0808a110(19, 4);
    Func_02000894(0x2013);
    Func_0808a138(6, 2);
    Func_0808a010(40);
    Func_0808a090(6, 0x9999, 0x4ccc);

    *(u8 *)(Func_0808a080(6) + 90) &= 0xfe;
    Func_0808a0d0(6, 248, 172);
    Func_0808a010(1);
    *(u8 *)(Func_0808a080(6) + 90) |= 1;

    Func_0808a010(20);
    Func_0808a110(6, 3);
    Func_0808a010(20);
    Func_0808a110(19, 3);
    Func_02000894(0x2013);
    Func_0808a110(6, 3);
    Func_02000894(6);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_0808a1b8(20, 176 << 8, 20);
    Func_0808a1e8(19, 0x105, 0);
    Func_0808a1e8(20, 0x105, 60);
    Func_0808a100(20, 4);
    Func_02000894(0x2014);
    Func_0808a1e8(19, 0x101, 40);
    Func_02000894(19);
    Func_0808a1e8(20, 0x105, 100);
    Func_0808a138(20, 1);
    Func_0808a010(20);
    Func_020008a8(20, 0);
    Func_020008a8(19, 0);
    Func_0808a178(20, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);

    /* first two-armed test; the counter is bumped once on either arm --
     * on the taken arm here, and on the fall-through arm at the re-test
     * below */
    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(20);
        Func_0808a110(20, 4);
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        i = 0;
    } else {
        Func_0808a010(20);
        Func_0808a110(20, 3);
        i = 1;
    }
    Func_02000894(20);
    if (i != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 20);
    Func_0808a1e8(20, 132 << 1, 40);
    Func_0808a178(20, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);

    /* second copy of the same shape, on actor 2 instead of 20 */
    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(20);
        Func_0808a100(2, 4);
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        i = 0;
    } else {
        Func_0808a010(20);
        Func_0808a100(2, 3);
        i = 1;
    }
    Func_02000894(2);
    if (i != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_0808a138(1, 2);
    Func_020008a8(1, 128 << 7);
    Func_02000894(1);
    Func_0808a138(19, 1);
    Func_02000894(19);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_020008a8(3, 128 << 8);
    Func_0808a110(1, 4);
    Func_0808a110(20, 3);
    Func_0808a188(20, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_020008a8(19, 192 << 6);
    Func_0808a100(19, 3);
    Func_02000894(19);
    Func_0808a1b8(20, 176 << 8, 20);
    Func_0808a110(20, 3);
    Func_0808a010(40);
    Func_020008a8(20, 128 << 8);
    Func_02000894(0x2014);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a188(21, 0, 40);
    Func_02000894(0x2014);
    Func_0808a1e8(21, 0x103, 60);
    Func_020008a8(19, 128 << 8);
    Func_0808a138(19, 1);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x105, 60);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a0d0(21, 144 << 1, 192);
    Func_0808a1b8(19, 0, 0);
    Func_0808a1b8(20, 0, 0);
    Func_0808a0d0(21, 155 << 1, 192);
    Func_0808a0d0(21, 164 << 1, 186);
    Func_0808a010(20);
    Func_0808a138(21, 2);
    Func_08015040(0x27ba, 1);
    Func_0808a0d0(21, 155 << 1, 192);
    Func_0808a1b8(19, 128 << 8, 0);
    Func_0808a1b8(20, 128 << 8, 0);
    Func_0808a0d0(21, 144 << 1, 192);
    Func_0808a0d0(21, 131 << 1, 176);
    Func_0808a1b8(21, 0, 40);
    Func_0808a170(0x27bb);
    Func_02000894(21);
    Func_0808a110(20, 3);
    Func_02000894(0x2014);
    Func_0808a110(21, 3);
    Func_020008a8(20, 0);
    Func_020008a8(21, 128 << 8);
    Func_0808a110(21, 3);
    Func_0808a110(6, 3);
    Func_0808a090(6, 0xcccc, 0x6666);
    Func_0808a0d0(6, 130 << 1, 186);
    Func_0808a1b8(21, 192 << 6, 0);
    Func_0808a0d0(6, 138 << 1, 192);
    Func_080f9010(19);

    *(u16 *)(Func_0808a080(19) + 6) = 160 << 7;
    Func_080000c0(1);
    Func_0808a138(19, 1);
    Func_02000894(19);
    Func_0808a138(6, 2);
    Func_0808a1b8(21, 0, 0);
    Func_0808a1b8(20, 160 << 7, 0);
    Func_0808a1b8(6, 208 << 8, 20);
    Func_0808a128(3, 2, 20);
    Func_020008a8(3, 160 << 8);
    Func_02000894(3);
    Func_0808a1b8(21, 0, 0);
    Func_0808a1b8(6, 0, 0);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_0808a1b8(20, 176 << 8, 80);
    Func_0808a1b8(19, 0, 0);
    Func_0808a1b8(20, 0, 40);
    Func_080f9010(29);
    Func_02000894(20);
    Func_0808a1b8(21, 192 << 6, 0);
    Func_0808a1b8(6, 176 << 8, 20);
    Func_0808a130(2, 2);
    Func_02000894(2);
    Func_020008a8(19, 192 << 6);
    Func_0808a110(19, 3);
    Func_02000894(19);
    Func_0808a1b8(21, 0, 0);
    Func_020008a8(6, 208 << 8);
    Func_0808a100(3, 3);
    Func_02000894(3);
    Func_0808a1e8(20, 128 << 1, 20);
    Func_0808a1b8(20, 192 << 6, 20);
    Func_02000894(20);
    Func_0808a1e8(6, 129 << 1, 0);
    Func_0808a1e8(21, 129 << 1, 60);
    Func_0808a1b8(6, 0, 0);
    Func_020008a8(21, 0);
    Func_0808a1e8(1, 0x103, 40);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_020008a8(19, 0);
    Func_0808a110(19, 4);
    Func_02000894(19);
    Func_020008a8(20, 0);
    Func_0808a100(20, 4);
    Func_02000894(20);
    Func_0808a1b8(6, 208 << 8, 0);
    Func_0808a1e8(6, 0x101, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(3, 0, 20);
    Func_020008a8(19, 192 << 6);
    Func_0808a100(19, 4);
    Func_02000894(19);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a188(2, 0, 20);
    Func_020008a8(19, 0);
    Func_0808a110(20, 3);
    Func_02000894(20);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a138(3, 2);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 40);
    Func_0808a138(3, 1);
    Func_020008a8(3, 160 << 8);
    Func_02000894(3);
    Func_0808a1b8(1, 128 << 8, 20);
    Func_0808a1e8(1, 0x103, 20);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_0808a1b8(6, 0, 0);
    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 40);
    Func_0808a1e8(20, 0x103, 40);
    Func_0808a130(20, 2);
    Func_02000894(20);
    Func_0808a138(19, 1);
    Func_02000894(19);

    Func_080770c8(Func_08077040(65) + 0x345);
    Func_08077048(65);

    *(u8 *)(Func_0808a080(0) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(1) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(2) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(3) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(19) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(20) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(21) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(6) + 90) &= 0xfe;

    Func_0808a098(0, 0x0200e004);
    Func_0808a098(1, 0x0200e004);
    Func_0808a098(2, 0x0200e004);
    Func_0808a098(3, 0x0200e004);
    Func_0808a098(19, 0x0200e03c);
    Func_0808a098(20, 0x0200e03c);
    Func_0808a098(21, 0x0200e004);
    Func_0808a0b0(6, 0x0200e004);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_0808a260(0xbb, 2);
    Func_0808a268(0xbb, 9);
    Func_0808a250(98, 1);
    Func_080770c8(212 << 2);
}
