#include "types.h"

/*
 * resource_3c9 owner at 0x02002360, 4708 bytes: the largest owner in
 * this overlay and the last of its residue -- a long scripted cutscene
 * driver over the same actor-id/camera call family the rest of
 * resource_3c9 uses, with 433 call sites, two 17-iteration orbit loops
 * that both feed this overlay's own eight-argument Func_0200013c, and
 * one two-armed branch on Func_0808a070.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,fp / mov r6,sl /
 * mov r5,r9 / push {r5, r6, r7} / mov r7,r8 / push {r7} / sub sp,#136`
 * at 0x02002360-0x0200236f through `add sp,#136 / pop {r3, r5, r6, r7}
 * / mov r8,r3 / mov r9,r5 / mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x02003598-0x020035a8, a two-byte alignment
 * halfword at 0x020035aa, and the trailing six-word literal pool
 * 0x020035ac-0x020035c3; the next owner's prologue is exactly at
 * 0x020035c4, and this row calls it seven times, which confirms the
 * upper bound from the callee side exactly as 0x020012c8's loop
 * confirmed its own. No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/lib/overlay_call_targets.ts resource_3c9
 * 2360 35c4`'s `+2` rule, which reports sites=433 distinct_targets=56
 * veneer=356 prologue=76 unknown=1 over exactly these bounds. The one
 * `unknown` is NOT a defect: it is the real `bl` at 0x02002af8 to
 * Func_02005688, the push-less leaf this overlay's sweep D turned up
 * and which is already drafted; `classify` keys on a `push` prologue
 * and that leaf has none. 356 + 76 + 1 = 433, fully accounted.
 *
 * SIX INTERIOR LITERAL POOLS, each jumped over rather than fallen into.
 * They are at 0x020023d0 (1 word, skipped by the `b.n 0x020023d4` at
 * 0x020023ce), 0x02002830-0x0200285f (12 words, `b.n 0x02002860` at
 * 0x0200282e), 0x0200292c (1 word, `b.n 0x02002930` at 0x0200292a),
 * 0x02002c58-0x02002c6f (6 words, reached only by the CONDITIONAL
 * `bne.n 0x02002c70` at 0x02002c48 -- the forward-`b.n` tell never
 * fires for this one), 0x02003064-0x02003093 (12 words, `b.n
 * 0x02003094` at 0x02003062) and 0x020034d0-0x020034f7 (10 words, `b.n
 * 0x020034f8` at 0x020034cc).
 *
 * FOUR OF THE TEN APPARENT `b.n` ARE POOL WORDS WEARING A BRANCH:
 * 0x02002830 (=0x0200e074), 0x02003070 (=0x0200e088), 0x02003080
 * (=0x0200e0ac) and 0x020034e0 (=0x0200e22c). The last one decodes as
 * `b.n 0x0200393c`, which is PAST this owner's end -- a reader who
 * follows it leaves the function entirely. Every branch target here was
 * cross-checked against the pool address list before being believed.
 *
 * Structurally: nine conditional branches in 4664 bytes of code, which
 * is the same long-not-hard shape as this overlay's 0x020012c8 and
 * bigger only by instruction count (1918 against 1438). Two of the nine
 * are the loop backedges at 0x02003402 and 0x0200347a; the rest are
 * forward `if` blocks. Of the six real unconditional `b.n`, FIVE are
 * the pool skips above and the sixth (0x02002c54) is a genuine if/else
 * join -- the then-arm of the second Func_0808a070 test jumps over the
 * else-arm at 0x02002c70. (An earlier measuring pass recorded
 * "no unconditional joins at all"; that was wrong, and this is the
 * sixth handoff figure corrected by re-reading.)
 *
 * THE TWO ORBIT LOOPS are the point of the 136-byte frame. Each runs
 * i = 0..16 inclusive, computes 3*Func_08000120(i << 12) and
 * 2*Func_08000118(i << 12) into a three-word scratch vector at sp+84,
 * and hands the anchor record's +8/+12/+16 plus that vector plus a
 * flags word 0x01090000 plus a 40-byte descriptor buffer to this
 * overlay's own Func_0200013c. The two differ only in the anchor
 * (record 19 then record 20) and in which descriptor buffer they fill
 * (sp+96 then sp+44). The descriptor's +36 slot is the published word
 * 0x020083a1 in both.
 *
 * IT PUBLISHES THREE OF THIS OVERLAY'S OWN OWNERS, by the base + 0x8000
 * rule (tracked HANDOVER, "In-image pointers are spelled base +
 * 0x8000"): 0x020083a1 -> 0x020003a0, 0x0200b6d1 -> 0x020036d0 and
 * 0x0200b7c5 -> 0x020037c4. The first goes into both orbit
 * descriptors; the other two are handed to Func_080000d0, the
 * callback-registration idiom the rest of this overlay uses.
 *
 * SHARED IDIOMS with the rest of resource_3c9: the `Func_0808a080(id)`
 * record fetch followed by a `+90` bit edit, the `Func_0808a098(id,
 * ptr)` run over a shared cached pointer (0x0200e074, 0x0200e088,
 * 0x0200e0ac, 0x0200e22c here), the `Data_03001ebc` workspace +472
 * halfword counter, and the closing `0x02000240 + 0x22b` byte-3 store
 * followed by Func_0808a260/268/250 -- the trailing pool holds
 * 0x02000240, 0x22b and 0xbb, the SAME three closing constants as
 * 0x020012c8's trailing pool, so the two rows end with one idiom.
 *
 * Observed but NOT interpreted: the same high-bit id pairing 0x020012c8
 * records -- 0x2013 and 0x2014 alongside plain 19 and 20, 0x0101/0x0103
 * /0x0105 alongside plain 1/3/5, and 0xa015 alongside 21. Nothing in
 * this row establishes what the 0x2000 and 0x0100 bits select, so they
 * are written as the literal values the pool holds.
 *
 * Uncertainty: none of the 56 distinct callees are identified beyond
 * call shape. Record offsets +6/+100 (halfwords), +8/+12/+16/+24/+28/
 * +40/+48/+52/+68/+72/+80 (words) and +5/+9/+28/+35/+85/+90/+92
 * (bytes) are raw offsets, no layout established. One dead spill is
 * dropped: `str r6,[sp,#16]` at 0x0200350c writes 254 to a frame slot
 * that is never read again before the epilogue.
 */

extern u8 *Data_03001ebc;

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 arg0, s32 arg1);
extern s32 Func_08000118(s32 angle);   /* sine of a binary angle */
extern s32 Func_08000120(s32 angle);   /* cosine of a binary angle */
extern u8 *Func_08000140(s32 arg0, s32 arg1);
extern void Func_08000150(s32 arg0);
extern void Func_080001c8(s32 arg0, s32 arg1, u8 *arg2);
extern void Func_08009128(void);
extern u8 *Func_080090c8(s32 kind, s32 a, s32 b, s32 c);   /* spawner */
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091e0(u8 *record, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08009150(u8 *record, s32 arg1, s32 arg2, s32 arg3);
extern void Func_08009158(u8 *record);
extern void Func_08015250(s32 arg0);
extern void Func_080770c8(s32 flagId);
extern void Func_08077268(void);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
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
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 *Func_0808a228(void);
extern void Func_0808a250(s32 group, s32 arg1);
extern void Func_0808a260(s32 group, s32 arg1);
extern void Func_0808a268(s32 group, s32 arg1);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a370(void);

extern void Func_02000894(s32 arg0);
extern void Func_020008a8(s32 arg0, s32 arg1);
extern void Func_0200013c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);
extern void Func_02005688(u8 *record);
extern void Func_020035c4(void);

void Func_02002360(void)
{
    u8 *record;
    u8 *anchor;
    u8 *attached;
    u8 *spawned;
    u8 *tiles;
    u8 *vec;            /* three-word scratch vector, frame slot +84 */
    u8 *descriptor;     /* 40-byte orbit descriptor, +96 then +44 */
    s32 vec_store[3];
    s32 desc_a[10];
    s32 desc_b[10];
    u32 i;
    s32 first_pass;

    vec = (u8 *)vec_store;

    Func_0808a018();
    Func_080091c0(17, 10, 4, 2, 17, 8);

    /* six records get a +6 halfword and a camera pair, in id order */
    *(u16 *)(Func_0808a080(1) + 6) = 128 << 8;
    Func_0808a0f0(1, 164 << 17, 168 << 16);
    *(u16 *)(Func_0808a080(2) + 6) = 128 << 8;
    Func_0808a0f0(2, 170 << 17, 196 << 16);
    *(u16 *)(Func_0808a080(3) + 6) = 128 << 8;
    Func_0808a0f0(3, 163 << 17, 204 << 16);
    *(u16 *)(Func_0808a080(6) + 6) = 192 << 6;
    Func_0808a0f0(6, 134 << 17, 154 << 16);
    *(u16 *)(Func_0808a080(21) + 6) = 192 << 6;
    Func_0808a0f0(21, 134 << 17, 164 << 16);

    record = Func_0808a080(20);
    *(u16 *)(record + 100) = 10;
    *(u16 *)(record + 6) = 208 << 8;
    Func_0808a0f0(20, 147 << 17, 212 << 16);
    Func_0808a100(20, 9);
    Func_0808a098(20, 0x0200e074);
    Func_080091e0(Func_0808a080(20), 0);

    record = Func_0808a080(19);
    *(u16 *)(record + 100) = 10;
    *(u16 *)(record + 6) = 0;
    Func_0808a0f0(19, 143 << 17, 192 << 16);
    Func_0808a100(19, 7);
    Func_0808a098(19, 0x0200e074);
    Func_080091e0(Func_0808a080(19), 0);

    *(u8 *)(Func_0808a228() + 85) = 0;
    Func_0808a210(152 << 17, 128 << 14, 180 << 16, 0);

    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(80);
    Func_0808a128(1, 2, 20);
    Func_020008a8(1, 128 << 6);
    Func_0808a170(0x27cf);
    Func_02000894(0x1001);
    Func_020008a8(3, 160 << 8);
    Func_02000894(3);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_020008a8(2, 160 << 8);
    Func_0808a138(21, 2);
    Func_0808a090(21, 0xcccc, 0x6666);
    *(u8 *)(Func_0808a080(21) + 90) &= 0xfe;
    Func_0808a0d0(21, 140 << 1, 164);
    Func_0808a010(1);
    *(u8 *)(Func_0808a080(21) + 90) |= 1;
    Func_0808a010(20);
    Func_0808a1e8(2, 129 << 1, 40);
    Func_02000894(2);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Func_0808a1e8(1, 0x0103, 20);
    Func_0808a138(1, 1);
    Func_02000894(1);
    Func_0808a138(21, 1);
    Func_020008a8(21, 0);
    Func_02000894(21);
    Func_0808a130(3, 2);
    Func_02000894(3);
    Func_0808a100(3, 4);
    Func_02000894(3);
    Func_0808a1e8(21, 0x0105, 40);
    Func_020008a8(21, 192 << 6);
    Func_02000894(21);
    Func_0808a128(2, 2, 20);
    Func_02000894(2);
    Func_0808a110(21, 3);
    Func_02000894(21);
    Func_0808a138(1, 1);
    Func_020008a8(1, 128 << 6);
    Func_0808a178(1, 0);
    Func_0808a1b8(2, 224 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);

    /* first Func_0808a070 test: one arm bumps the workspace counter now,
       the other defers it past the next block */
    first_pass = 1;
    if (Func_0808a070(0, 0) != 0) {
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        first_pass = 0;
    }

    Func_080000c0(20);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_020008a8(3, 160 << 8);
    Func_020008a8(21, 0);
    Func_0808a180(21, 0);
    Func_080f9010(17);
    Func_0808a010(40);

    if (first_pass != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_0808a0a8(20);
    Func_0808a0a8(19);
    Func_080000c0(1);

    record = Func_0808a080(20);
    *(s32 *)(record + 24) = 128 << 9;
    *(s32 *)(record + 28) = 128 << 9;
    record = Func_0808a080(19);
    *(s32 *)(record + 24) = 128 << 9;
    *(s32 *)(record + 28) = 128 << 9;

    Func_080000c0(1);
    Func_02000894(20);
    Func_0808a1b8(21, 192 << 6, 0);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 40);
    Func_0808a100(20, 1);
    Func_080091e0(Func_0808a080(20), 1);
    Func_0808a010(20);
    Func_0808a090(20, 0x3333, 0x1999);
    Func_0808a0d0(20, 150 << 1, 206);
    Func_0808a010(20);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    *(s32 *)(Func_0808a080(20) + 24) = 0xffff0000;
    Func_080f9010(161);
    Func_0808a100(20, 8);
    Func_0808a010(20);
    Func_0808a100(19, 1);
    Func_080091e0(Func_0808a080(19), 1);
    Func_0808a128(19, 4, 40);
    Func_0808a090(19, 0x3333, 0x1999);
    *(u8 *)(Func_0808a080(19) + 90) &= 0xfe;
    Func_0808a0d0(19, 148 << 1, 186);
    Func_0808a090(19, 0x1999, 0x0ccc);
    Func_0808a0d0(19, 146 << 1, 186);
    *(u8 *)(Func_0808a080(19) + 90) |= 1;
    *(s32 *)(Func_0808a080(19) + 24) = 0xffff0000;
    Func_080f9010(161);
    Func_0808a100(19, 5);
    Func_0808a010(20);
    Func_0808a138(19, 2);
    Func_0808a010(20);
    Func_0808a138(20, 1);
    Func_0808a010(80);
    Func_0808a110(3, 4);
    Func_0808a188(3, 0, 20);
    Func_0808a138(19, 2);
    Func_0808a010(40);
    Func_02000894(19);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 40);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 20);

    anchor = Func_0808a080(20);
    *(u16 *)(anchor + 6) = 192 << 6;
    *(s32 *)(anchor + 24) = 128 << 9;
    Func_0808a100(20, 1);
    Func_0808a010(10);
    Func_020008a8(20, 208 << 8);
    Func_0808a128(20, 6, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 160 << 8, 0);
    Func_0808a1b8(21, 208 << 8, 0);
    Func_0808a1b8(6, 0, 0);

    /* spawn a record of kind 22 at the anchor's position, +0x80000 on
       the middle axis; the spawn may fail and every use is guarded */
    spawned = Func_080090c8(22,
                            *(s32 *)(anchor + 8),
                            *(s32 *)(anchor + 12) + (128 << 12),
                            *(s32 *)(anchor + 16));
    if (spawned != 0) {
        attached = *(u8 **)(spawned + 80);
        *(u8 *)(attached + 39) = 0;
        *(u8 *)(attached + 5) &= (u8)-33;
        *(u8 *)(attached + 9) = (u8)((*(u8 *)(attached + 9) & 15) & (u8)-13);
        *(u8 *)(spawned + 35) &= 0xfe;
        *(u8 *)(spawned + 85) = 0;
        *(u8 *)(spawned + 92) = 1;
        *(s32 *)(spawned + 48) = 0x00019999;
        *(s32 *)(spawned + 52) = 0x0000cccc;

        tiles = Func_08000140(17, 193 << 3);
        Func_08015250(220);
        Func_080001c8(*(u8 *)(attached + 28), 128, tiles + (128 << 3));
        Func_08000150(17);
    }

    Func_0808a1e0(22, 1);
    record = Func_0808a080(22);
    *(s32 *)(record + 8) = *(s32 *)(anchor + 8);
    *(s32 *)(record + 12) = 128 << 14;
    *(s32 *)(record + 16) = *(s32 *)(anchor + 16);
    *(u8 *)(record + 85) = 3;
    *(s32 *)(record + 48) = 0x00019999;
    *(s32 *)(record + 52) = 0x0000cccc;
    *(s32 *)(record + 24) = 192 << 8;
    *(s32 *)(record + 28) = 192 << 8;

    if (spawned != 0) {
        *(u8 *)(spawned + 85) = 3;
        *(s32 *)(spawned + 72) = 0x00009999;
        *(s32 *)(spawned + 68) = 0x0000cccc;
        *(s32 *)(spawned + 40) = 128 << 12;
        Func_08009150(spawned, 154 << 17, 128 << 14, 164 << 16);
    }

    Func_0808a0c0(22, 154 << 1, 164);
    Func_0808a0f0(22, 0, 0);
    Func_0808a1e0(21, 0);

    if (spawned != 0) {
        /* three timed moves of the spawned record, each a
           Func_08009150 aim followed by a Func_08009158 run */
        Func_080f9010(0x135);
        Func_080091e0(spawned, 0);
        *(s32 *)(spawned + 40) = 128 << 11;
        Func_08009150(spawned, 157 << 17, 128 << 14, 137 << 16);
        Func_08009158(spawned);

        Func_080f9010(0x135);
        attached = *(u8 **)(spawned + 80);
        *(u8 *)(attached + 9) = (u8)((*(u8 *)(attached + 9) & (u8)-13) | 4);
        *(s32 *)(spawned + 40) = 192 << 11;
        Func_08009150(spawned, 0x011d0000, 128 << 14, 146 << 16);
        Func_08009158(spawned);

        Func_080f9010(0x135);
        *(s32 *)(spawned + 40) = 160 << 11;
        Func_08009150(spawned, 150 << 17, 128 << 14, 154 << 16);
        Func_08009158(spawned);

        Func_080000c0(6);
        *(s32 *)(spawned + 8) = 0;
        *(s32 *)(spawned + 12) = 0;
        *(s32 *)(spawned + 16) = 0;
        Func_02005688(spawned);
    }

    Func_0808a1e8(21, 128 << 1, 0);
    Func_0808a1e8(6, 128 << 1, 0);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 30);
    Func_0808a1e0(21, 1);
    *(u8 *)(Func_0808a080(21) + 35) |= 1;
    Func_0808a130(2, 2);
    Func_02000894(2);
    Func_0808a138(3, 1);
    Func_0808a188(3, 0, 40);
    Func_0808a138(20, 1);
    Func_0808a010(20);
    Func_02000894(20);
    Func_0808a1b8(0, 160 << 7, 0);
    Func_0808a1b8(1, 160 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 0);
    Func_0808a1b8(6, 192 << 6, 0);
    Func_020008a8(21, 192 << 6);
    Func_0808a1e8(21, 0x0101, 0);
    Func_0808a1e8(6, 0x0101, 0);
    Func_0808a1e8(0, 0x0101, 0);
    Func_0808a1e8(1, 0x0101, 0);
    Func_0808a1e8(2, 0x0101, 0);
    Func_0808a1e8(3, 0x0101, 60);
    Func_0808a1e8(20, 132 << 1, 40);
    Func_02000894(20);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 40);
    Func_0808a178(1, 0);

    /* second Func_0808a070 test, and the only real if/else in the row:
       the two arms differ in the id passed to Func_0808a100 and in
       whether the counter is bumped now or after Func_02000894 */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a100(1, 3);
        first_pass = 1;
    } else {
        (*(u16 *)(Data_03001ebc + 236 * 2))++;
        Func_0808a100(1, 4);
        first_pass = 0;
    }
    Func_02000894(1);
    if (first_pass != 0)
        (*(u16 *)(Data_03001ebc + 236 * 2))++;

    Func_0808a010(20);
    Func_080091e0(Func_0808a080(24), 0);
    Func_0808a158(24, 7);
    record = Func_0808a080(24);
    *(s32 *)(record + 28) = 0xffff0000;
    *(s32 *)(record + 24) = 0x00001999;
    *(u8 *)(record + 85) = 0;
    *(s32 *)(record + 12) = 128 << 15;
    *(s32 *)(record + 16) = 158 << 16;
    *(s32 *)(record + 8) = 152 << 17;

    Func_080091e0(Func_0808a080(25), 0);
    Func_0808a158(25, 7);
    record = Func_0808a080(25);
    *(s32 *)(record + 28) = 0xffff0000;
    *(s32 *)(record + 24) = 0x00001999;
    *(u8 *)(record + 85) = 0;
    *(s32 *)(record + 12) = 192 << 15;
    *(s32 *)(record + 8) = 152 << 17;
    *(s32 *)(record + 16) = 158 << 16;

    Func_0808a1e8(21, 128 << 1, 0);
    Func_0808a1e8(6, 128 << 1, 0);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 160 << 8, 0);
    Func_0808a1b8(21, 208 << 8, 0);
    Func_0808a1b8(6, 0, 0);
    Func_0808a098(24, 0x0200e088);
    Func_0808a098(25, 0x0200e088);

    Func_080f9010(145);
    Func_080091f0(192 << 11, 192 << 11, 128 << 9);
    Func_0808a330(0x004063ff, 0);
    Func_0808a348(16);
    Func_080000c0(20);
    Func_0808a330(0x00007fff, 0);
    Func_0808a348(24);
    Func_080000c0(60);
    Func_080000d0(0x0200b6d1, 200 << 4);
    Func_080f9010(141);
    Func_080091f0(128 << 11, 128 << 11, 128 << 9);
    Func_0808a330(0x004063ff, 0);
    Func_0808a348(120);
    Func_0808a098(24, 0x0200e0ac);
    Func_0808a098(25, 0x0200e0ac);
    Func_080000c0(120);
    Func_080091f0(192 << 10, 192 << 10, 128 << 9);
    Func_0808a330(0x00203210, 0);
    Func_0808a348(120);
    Func_080000c0(120);
    Func_080f9010(63);
    Func_080091f0(128 << 10, 128 << 10, 128 << 9);
    Func_0808a330(128 << 9, 0);
    Func_0808a348(120);
    Func_080000c0(120);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);

    Func_0808a100(19, 1);
    Func_0808a1b8(19, 0, 0);
    *(s32 *)(Func_0808a080(19) + 24) = 128 << 9;
    Func_0808a128(19, 4, 40);
    Func_0808a138(19, 1);
    Func_0808a188(19, 0, 20);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 0);
    Func_0808a1b8(21, 192 << 6, 0);
    Func_0808a1b8(6, 192 << 6, 20);
    Func_0808a138(20, 1);
    Func_02000894(20);
    Func_0808a138(3, 1);
    Func_020008a8(3, 128 << 8);
    Func_02000894(3);
    Func_020008a8(20, 0);
    Func_0808a110(20, 4);
    Func_02000894(20);
    Func_0808a1e8(19, 0x0103, 20);
    Func_0808a130(19, 2);
    Func_02000894(19);
    Func_0808a1e8(1, 128 << 1, 20);
    Func_020008a8(1, 192 << 7);
    Func_02000894(1);
    Func_020008a8(20, 208 << 8);
    Func_0808a110(20, 3);
    Func_0808a188(20, 0, 20);
    Func_0808a138(1, 1);
    Func_02000894(1);
    Func_0808a1b8(19, 176 << 8, 20);
    Func_0808a138(19, 1);
    Func_02000894(0x2013);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(2, 1);
    Func_0808a138(3, 1);
    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_020008a8(3, 160 << 8);
    Func_0808a1e8(21, 0x0101, 80);
    Func_02000894(21);
    Func_0808a1b8(0, 160 << 8, 0);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 40);
    Func_0808a138(20, 1);
    Func_02000894(20);
    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 20);
    Func_020008a8(20, 176 << 8);
    Func_02000894(0x2014);
    Func_0808a110(21, 3);
    Func_0808a1b8(21, 176 << 8, 60);
    Func_0808a1b8(21, 192 << 6, 40);
    Func_0808a138(19, 1);
    Func_02000894(0x2013);
    Func_0808a1b8(21, 0, 40);
    Func_02000894(21);
    Func_0808a1b8(20, 208 << 8, 40);
    Func_020008a8(20, 176 << 8);
    Func_02000894(0x2014);
    Func_0808a1b8(21, 192 << 6, 40);
    Func_0808a1b8(21, 192 << 6, 20);
    Func_0808a110(21, 3);
    Func_0808a1b8(19, 0, 40);
    Func_0808a1b8(19, 176 << 8, 20);
    Func_0808a1e8(19, 0x0101, 40);
    Func_02000894(0x2013);
    Func_020008a8(21, 192 << 6);
    Func_0808a1e8(21, 0x0101, 20);
    Func_02000894(21);
    Func_0808a1b8(20, 176 << 8, 20);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x0103, 80);
    Func_02000894(21);
    Func_0808a110(19, 3);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x0103, 20);
    Func_0808a130(21, 2);
    Func_02000894(21);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 129 << 1, 60);
    Func_0808a138(21, 1);
    Func_02000894(21);
    Func_0808a1e8(20, 132 << 1, 40);
    Func_0808a100(20, 4);
    Func_0808a188(0x2014, 0, 40);
    Func_0808a138(21, 1);
    Func_0808a010(20);
    Func_02000894(21);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 160 << 8, 20);
    Func_0808a1b8(19, 192 << 6, 20);
    Func_0808a100(19, 3);
    Func_0808a110(20, 3);
    Func_0808a1b8(21, 176 << 8, 20);
    Func_02000894(0xa015);
    Func_0808a090(6, 128 << 9, 128 << 8);
    Func_0808a090(21, 128 << 9, 128 << 8);
    Func_0808a098(21, 0x0200e22c);
    Func_0808a010(20);
    Func_0808a098(6, 0x0200e22c);
    Func_020008a8(1, 192 << 7);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_080f9010(17);
    Func_0808a330(0x0040250d, 1);
    Func_0808a348(40);
    Func_020008a8(20, 208 << 8);
    Func_02000894(20);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 0);
    Func_020035c4();
    Func_0808a158(20, 7);
    Func_0808a158(19, 7);
    Func_0808a010(20);
    Func_0808a158(20, 128 << 1);
    Func_0808a158(19, 128 << 1);
    Func_0808a010(20);
    Func_020035c4();
    Func_0808a128(3, 2, 20);
    Func_02000894(3);
    Func_020008a8(19, 0);
    Func_02000894(19);
    Func_020035c4();

    /* first orbit loop: 17 steps around record 19 */
    anchor = Func_0808a080(19);
    descriptor = (u8 *)desc_a;
    *(s32 *)(descriptor + 4) = 7;
    *(s32 *)(descriptor + 36) = 0x020083a1;
    *(s32 *)(descriptor + 8) = 128 << 9;
    *(s32 *)(descriptor + 12) = 128 << 9;
    i = 0;
    do {
        *(s32 *)(vec + 4) = 0;
        *(s32 *)(vec + 0) = Func_08000120(i << 12);
        *(s32 *)(vec + 0) = *(s32 *)(vec + 0) + (*(s32 *)(vec + 0) << 1);
        *(s32 *)(vec + 8) = Func_08000118(i << 12) << 1;
        i++;
        Func_0200013c(*(s32 *)(anchor + 8),
                      *(s32 *)(anchor + 12),
                      *(s32 *)(anchor + 16),
                      *(s32 *)(vec + 0),
                      *(s32 *)(vec + 4),
                      *(s32 *)(vec + 8),
                      0x01090000,
                      descriptor);
    } while (i <= 16);
    Func_080f9010(212);
    Func_080000c0(6);
    Func_020035c4();

    /* second orbit loop: the same 17 steps around record 20, into a
       second descriptor; the scratch vector is shared */
    anchor = Func_0808a080(20);
    descriptor = (u8 *)desc_b;
    *(s32 *)(descriptor + 4) = 7;
    *(s32 *)(descriptor + 36) = 0x020083a1;
    *(s32 *)(descriptor + 8) = 128 << 9;
    *(s32 *)(descriptor + 12) = 128 << 9;
    i = 0;
    do {
        *(s32 *)(vec + 4) = 0;
        *(s32 *)(vec + 0) = Func_08000120(i << 12);
        *(s32 *)(vec + 0) = *(s32 *)(vec + 0) + (*(s32 *)(vec + 0) << 1);
        *(s32 *)(vec + 8) = Func_08000118(i << 12) << 1;
        i++;
        Func_0200013c(*(s32 *)(anchor + 8),
                      *(s32 *)(anchor + 12),
                      *(s32 *)(anchor + 16),
                      *(s32 *)(vec + 0),
                      *(s32 *)(vec + 4),
                      *(s32 *)(vec + 8),
                      0x01090000,
                      descriptor);
    } while (i <= 16);
    Func_080f9010(212);

    Func_0808a128(2, 6, 20);
    Func_080f9010(54);
    Func_02000894(2);
    Func_0808a100(20, 4);
    Func_02000894(20);
    Func_020035c4();
    Func_0808a090(20, 0x3333, 0x1999);
    Func_0808a090(19, 0x3333, 0x1999);
    *(u8 *)(Func_0808a080(20) + 90) &= 0xfe;
    *(u8 *)(Func_0808a080(19) + 90) &= 0xfe;
    Func_0808a0b8(20, 147 << 1, 196);
    Func_0808a0b8(19, 147 << 1, 196);
    Func_080000d0(0x0200b7c5, 200 << 4);
    Func_0808a130(1, 2);
    Func_0808a180(1, 0);
    Func_080770c8(141 << 2);
    Func_0808a180(2, 0);
    Func_080770c8(0x235);
    Func_020035c4();
    Func_0808a010(20);
    Func_020035c4();
    Func_0808a010(20);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_0808a260(0xbb, 3);
    Func_0808a268(0xbb, 9);
    Func_0808a250(98, 0);
    Func_08077268();
    Func_080770c8(0x351);
}
