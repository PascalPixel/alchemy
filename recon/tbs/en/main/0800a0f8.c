/* 0800a0f8: ARM draft (Transform_UpdateVertices) for the game ARM route
   (K1: stock agbcc_arm at the library ARM setting, alchemy diff --arm).
   Stock agbcc_arm (2026-10-07, session 3): listing 157, draft 157,
   common 127, differ 60. Five empty non-volatile asm devices (FAKEMATCH,
   in the loops) make both loop tails match: in gcc_arm's sched.c,
   insn_cost sets LINK_COST_FREE on any link whose consumer fails recog,
   so an asm that rewrites a live register in place turns a costly anti
   dependence (ldm r6 -> add r6) into two links of cost 1, an asm that
   reads memory after the stm becomes bpl's only predecessor so the stm
   no longer waits its store cost above the branch, and in the else loop
   one asm on v fills the third cycle the strh needs above the ldm.
   Still unreachable, with the mechanism: the prologue. priority() is
   the longest cost path from the block's head; mov r5, r1 (6) and
   ldr r9 (33) are the first writers of r5 and r9 after the push, so
   each anti-depends on it at the push's cost (5) and has priority 5,
   like sub sp (576) and its clobber (577); rank_for_schedule then takes
   the higher LUID, 6 and 33, first in the backward pass. A link is free
   only when its consumer is unrecognized, so 6 and 33 would have to be
   asm instructions themselves (33 is a pool load, so it cannot); an
   empty asm placed after them reads them at priority 5 and also beats
   576 on LUID, and an asm before them would be dead (deleted) or, if
   volatile, a barrier depending on 576. Neither has a latency that
   could hold it back (6 costs 1; 33's consumers are the call and an
   asm, whose link is free). The same holds for mov r0, r8 above str
   r8, [r2] before the Sin call: the call depends on the pending store
   directly at its store cost. Other remaining rows: to->z in r4 for ip
   and the half in r2 for r1, with the two smulls interleaved
   differently; the loop's x in r1 for r2 (ldm r7, {r2, r4}), (s16)t
   reloaded by ldrsh instead of extended into lr, and the k load above
   the u stores (k's alias set, see camelot_arm below); the odd-frame
   y's ldr r2, [sp] and ldr r1, [r7, #12] above ldrsh r3, which the
   anti dependence of ldrsh r3 on str r3, [r6, #8] (hard register r3,
   cost 2) puts there and no source device reaches.
   Session 3 trials, from 76:
   - Else loop: asm on out between out[1] = out[0] and out += 2: 76
     (ldm and add r6 reorder, stm still waits); with v and out[1]'s
     memory in one asm: 74; the memory read through a block-local next
     = out + 1: 74 (CSE folds it to r6 + 16, so the out asm anti-depends
     on it); "m"(*v) instead: 72; plus an asm on v before v++: 68.
   - First loop: the same three asms: 126 (v and out swap r6 and r7
     throughout); the memory asm on out: 64; on cs: 64, the tail from
     str r3 on matches; without the asm on v before v++ (here it took
     the add ip slot): 60.
   - Odd-frame y as v->u / 4 + ((oz + v->dz) >> 8) + v->v / 2: 68; an
     asm on v between out->x and out->y: 60, unchanged.
   - On 60, a = (s16)t before out->a = v->u = t, then a <<= 8; a *= k
     (the camelot_arm 52 form's loop alone): 97, draft 158; z = to->z
     - from->z in one expression: 66 (to->z then loads into r1 and to->x
     into ip, swapped as under camelot_arm).
   Stock agbcc_arm (2026-10-07, session 1): listing 157, draft 157,
   common 119, differ 76 (14 register, 20 reordered, 4 inserted, 4
   deleted). Not reachable from source under this route: the prologue.
   The listing runs mov r5, r1; ldr r9, =WaitFramesFar; sub sp, sp, #8,
   two body insns above the frame's sub sp. gcc_arm's sched.c schedules
   each block backwards, by priority, then by class, then LUID. In
   sched2 (alchemy diff --arm --rtl: 0800a0f8.c.sched2) the push (574) has
   priority 1; sub sp (576) and its mem clobber (577) have 5; mov r5,
   r1 (6) and the r9 load (33) also have 5, solely through a REG_DEP_ANTI
   on the push, which stock arm_adjust_cost charges the push's full
   store4 cost (5). At equal priority and class the prologue's lower
   LUID places sub sp first; only the latency queue could hold 6 or 33
   back, and neither waits on a user placed just after it (6 feeds
   ldr r1, [r5] at cost 1; 33 feeds only the far call's ldr ip). A
   barrier depends on the sub sp as well, and any pseudo in a
   callee-saved register anti-depends on the push the same way, so no
   order of statements, block, barrier or register variable is known
   to move those two above it. Session 2 (2026-10-07): no source and
   no option set of stock gcc_arm reaches it. The prologue is RTL
   emitted ahead of the body, so without sched2 sub sp precedes every
   body insn. With sched2, 6 and 576 each have the push as their only
   predecessor, at the same cost: INSN_COST of the push (store4, at
   least 5 on every -mcpu; arm_adjust_cost changes only a load after a
   store), so
   their priorities are equal. 6 becomes ready together with 577 (r5's
   users are loads, which depend on the clobber), 6's own cost is 1 so
   rank_for_schedule puts both in class 3, and the higher LUID (6) is
   taken first in the backward pass, placing it below the frame. Only a
   rule that makes an anti dependence cheaper than the true one lowers
   6 and 33 under 576. Editions: the 0x284 bytes are the same code in
   all six (German differs only in the pool words of gScanlineAngle,
   gProjection + 0xc and gFrameCount, which it places 0x10 later), so
   one C body serves all six once it matches; per-edition --arm scoring
   is not needed for it. Under the camelot_arm trials below, with
   2.96's arm_adjust_cost (anti/output dependences free; sched.c clamps
   them to 1, so 6 and 33 drop to priority 1), "the prologue matches
   the listing up to the half constant". So this listing appears to
   need that scheduling cost in the route (a K3 question for Pascal),
   not an S2 device. Every stock form tried below keeps push; sub sp
   first.
   Stock trials, from 76:
   - Session 3/4 form (x = to->x - from->x; z = to->z - from->z; loop
     block-local x; a = (s16)t; out->a = v->u = t; a <<= 8; a *= k;
     union Word k): 83; the prologue then matches but for sub sp and
     the r9 load, and the loop loads z, x by two ldr (x in ip), no ldm.
     Without the block-local x: 97; z = v->z before x = v->x: 83.
   - Only the one-expression x and z in the prologue: 89.
   - v->dx = -(a + FIXED_MUL(z, -sn)): 80.
   - far = WaitFramesFar as the first statement, calls through far[n]:
     76, identical; the r9 load still lands after ldr r3, [r0].
   - alchemy permute --arm --heat, 5 minutes, 2 jobs: best weighted
     2190 -> 1950 at differ 77; lowest differ 74, only a temporary
     shared by the odd-frame out->x and out->y; neither taken.
   Route decision (2026-10-07), this draft under
   agscc -marm with the game options: differ 183; with -mno-apcs-frame
   190; with -mno-apcs-frame -ffixed-fp 166.
   alchemy diff --camelot-arm, upstream's whole arm_adjust_cost
   (2026-10-06, session 5): listing 157, draft 157, common 133, differ
   48, as under the anti/output half alone; stock agbcc_arm 76. The
   trials below were scored under that half. What remains: the shared 0x8000 lands in r2
   where the listing has r1, and the second smull is scheduled before the
   gScanlineAngle pool load; z's load in r4 where the listing has ip; x in
   r1, not r2, so no ldm r7, {r2, r4} and the second loop smull copies its
   low word; (s16)t read back by ldrsh where the listing extends it into lr
   at once.
   Trial (whole upstream rule set): alchemy permute --camelot-arm
   --heat, aimed at 48 rows on 15 lines, 2.5 minutes, 4 jobs: best
   weighted 1340 -> 1270 but differ 53, by shared temporaries
   (tmp = v->dx before out->x); not taken.
   camelot_arm trials (2026-10-06, session 4), from 48:
   - The k load's place is an alias conflict, not an option: needs no
     -fno-strict-aliasing. Read k in alias set 0 (union Word { s32 i; };
     k = ((union Word *)center)->i) and, under the current setting, the
     pool and k loads stay after both u stores (strh v->v, strh out->b)
     as listed, and the else loop still matches. On the session 3 form
     (block-local loop x; x = to->x - from->x; z = to->z - from->z;
     a = (s16)t before out->a = v->u = t; a <<= 8; a *= k): 57, and the
     first loop then has the listing's order; only registers differ.
     With b = ((s16)u << 8) * -k moved before a <<= 8 (or before v->dx):
     54 and draft 157, the ldm r7 as listed, but x in r0 for r2, the
     first smull's low word r2 for r1, a in ip for lr, b in lr for ip, k
     in r3 for r0, and the prologue's half in r2 for r1. On this draft
     as it stands (out->a read back by ldrsh after the k load): 96.
   - That 54 is stable under z read first, b declared before a, k
     block-local, -k * ((s16)u << 8), k * a: all 54. A second FixedMul
     tying RdHi to b ("=&r", "=r", "r", "1") for u's smull: x stays in
     r0 and is copied to r2 (mov r2, r0): 59; on the 57 form: 56.
   - The other side instead: out's a, unk2, b, unk6 as s32 : 16 bitfields
     (bitfields take their declared type's alias set) or out through a
     union: 63; v's u, v as s32 : 16 too: the else loop's ldrsh of v->u
     then waits for the str of out->x, which the listing's does not, so
     v->u and v->v are not read in int's alias set. What fits the listing
     is a k load that conflicts with the short stores of v and out alike,
     as an alias-set-0 load does; whatever Camelot's declaration of k's
     source was, it is not yet known, and the union is a FAKEMATCH
     device until it is.
   - Union k on other forms: prologue x = to->x; z = to->z; ...: 61;
     no block-local x: 71; z block-local too: 71; a = (a << 8) * k: 104.
   camelot_arm trials (2026-10-06, session 3), from 51:
   - One half = 0x8000 in a block around ox and oz, as the listing's one
     mov r1, #32768 feeds both smulls: 51 to 48 (k reused for it: 104).
   - z = to->z - from->z in one expression: to->z gets its own register
     as listed, but to->x and to->z swap r1 and ip: 54; z first: 54; x
     and z each in one expression: 63.
   - s16 t (chained or separate stores): the separate lsl/asr extension,
     after both strh rather than between them into lr, and the else loop
     reallocates: 85.
   - x from v->x at each use: 53 (s16 t too: 58); register s32 x
     asm("r2") in the loop: no change, the inline copies it.
   - alchemy permute --camelot-arm --function Transform_UpdateVertices,
     9 minutes from 48: best differ 53.
   - The listing's z is r4 both before and in the loop, but its x is r1
     before and r2 in the loop: one z, and a loop x of its own. A block
     local s32 x in the first loop with x = to->x - from->x and z = to->z
     - from->z: the prologue matches the listing up to the half constant
     (to->z in ip, z in r4, x in r1), loop x in r0: 49. Without the
     one-expression forms: 53; z local too: 63.
   - On that 49: t = ...; a = (s16)t; out->a = v->u = t; ... a = (a << 8)
     * k gives the listing's lsl r1, r3, #16; strh; asr lr, r1, #16;
     strh and lsl r3, lr, #8 (with r2 for r1, x being in r1), but the k
     load and a's mul are scheduled above u's smull, where the listing
     has them after both u stores: 70; a <<= 8; a *= k: 52; s16 t: 54;
     k = gProjection.center_x: 70; a from v->u: 91. Without the first
     scheduling pass (-fno-schedule-insns, study only) the k load still
     rises above u's stores, so in the listing something the k load
     depends on, not the scheduler's choice, holds it there.
   - Why: gcc_arm turns on -fstrict-aliasing at -O2, and the k load (alias
     set of int) and u's strh (short) get different alias sets, so sched
     moves the load up to the barrier of u's asm volatile smull. Study
     only, with -fno-strict-aliasing added to the camelot_arm setting:
     the k load, lsl #8, mul, lsl/asr #8, rsb and mul follow both u stores
     exactly as listed (a <<= 8; a *= k form: 63, the loop's x then loaded
     by two ldr into ip), but the else loop, which matches under the
     current setting, reorders its ldr r1, [r7, #12]; this draft 100,
     Object_UpdateAll 161 and UiText_DrawGlyphArm 165 unchanged. An
     option for all of ☀️'s game ARM C, so a K3 question, not settled
     here.
   camelot_arm trials (2026-10-06, session 2), from 104:
   - smull with two alternatives, "=&r,&r"(lo), "=r,&r"(hi), "r,r"(a),
     "1,r"(b): RdHi takes Rs when b dies, as the listing's ldr r3; smull
     r0, r3, r1, r3 does: 104 to 99 (with the x = to->x order below).
   - x = to->x; z = to->z - from->z; x -= from->x: the listing's load order,
     103; x = to->x; z = to->z; z -= from->z; x -= from->x: 77 (with all
     below). x first as one expression: 108; Arctan arguments inline: 112.
   - center = &gProjection.center_x before the loop, k = *center: the
     listing's pool word and a reload each iteration, as listed.
   - out->a = v->u = t, then a = (out->a << 8) * k: 83; from v->u: 99.
   - a = ((s16)t << 8) * k: 97 (one lsl/asr pair, the listing has two
     shifts then lsl #8); (s16)t * 256 * k: 107 (k << 8).
   - s16 t, out->a = v->u = t = ...: stores of the unextended value and a
     separate lsl/asr as listed, but center then keeps r9: 107; with u
     chained too, or k = gProjection.center_x: 107.
   - Declaration order of x and z, z loaded before x: no change; asm
     without volatile: 157.
   - v->dx = -(FIXED_MUL(z, -sn) + a) before b, found by alchemy permute
     --camelot-arm: 77 to 51; the same with a + FIXED_MUL(z, -sn): 55.
     The permuter's own best (differ 52, 8 minutes) adds only a temporary
     shared by v->x and v->dx.
   Control 1, alchemy diff --arm (agscc, -marm): listing 157, draft 157,
   differ 188. Its APCS frame (mov ip, sp; fp; ldmea) and asr #31 / lsr #24
   division never appear in the listing, so it cannot reach 0.
   Control 2, agbcc_arm -O2 -mthumb-interwork -fomit-frame-pointer
   -fcall-used-r4 (library setting, analysis only): listing 157, draft 157,
   differ 124. Remaining: prologue order, x in r0 not r2, (s16)t reloaded
   by ldrsh where the listing extends it into lr, gProjection + 0xc split
   as gProjection, [rN, #12], and load delay slots the listing fills
   (ldrh; subs; strh) that agbcc_arm never fills, even with
   -mcpu=strongarm.
   Trials (control 2 unless noted):
   - Early-clobber smull outputs: --arm 216 to 214; agbcc_arm 144.
   - k = gProjection.center_x: the pool word is gProjection + 0xc (was focal).
   - y written as ((oz + dz) >> 8) + u / 4 + v / 2, like x: 144 to 138.
   - x as v / 4 + ((ox + dx) >> 8) + u / 2: 158; both reordered: 152.
   - a = k * (v->u << 8): 124, cs r5, -sn r9, count r8, z r4 as listed.
   - Also b = -k * (v->v << 8): 150; b from out->b: 150; a from out->a: 136.
   - s16 t and u: 145; s16 t only: 137; center_x inline, no k: 145.
   - Arctan arguments inline: 146; separate dx, dz block locals: 140.
   - Operand order (s16)t << 8 * k, or (s16)u << 8 * -k: no change. With
     a from v->u, (v->u << 8) * k and ((s16)u << 8) * -k put k and -k in
     Rm as the listing's two muls do: still 124, --arm 188; kept.
   - A pointer, center = &gProjection.center_x, k = *center, gives the
     listing pool word gProjection + 0xc and [rN]: still 124, --arm 198.
   - With center: (s16)t << 8 again: 138; t = v->u = ...: 135; center set
     after the trig calls: 124; x inline at the top, z before x in the
     loop, or out[1] = out[0] before v++: 124.
   - FixedMul: hi as "=r": 124 (agbcc_arm still never puts RdHi in Rs);
     hi tied to b: draft 161, 124; asm without volatile: 183; plain
     (s64)a * b >> 16: 288. */
#include "PROJECT.H"

struct FarCallStub {
    u32 code;
    void *target;
};

struct Position {
    s32 x;
    s32 y;
    s32 z;
};

struct Vertex {
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;
    s16 u;
    s16 v;
};

struct VertexPair {
    s16 a;
    s16 unk2;
    s16 b;
    s16 unk6;
    s32 x;
    s32 y;
};

struct VertexOut {
    struct VertexPair cur;
    struct VertexPair prev;
};

extern struct FarCallStub WaitFramesFar[];
extern u32 gScanlineAngle;

extern u32 gFrameCount;

static __inline__ s32 FixedMul(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    /* FAKEMATCH: register choice; RdLo early-clobbered, RdHi either tied to
       Rs (when b dies, as listed) or early-clobbered; camelot_arm 104 to 99. */
    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %0, %1, %2, %3" : "=&r,&r"(lo), "=r,&r"(hi) : "r,r"(a), "1,r"(b));
    return (u32)lo >> 16 | hi << 16;
}
#define FIXED_MUL(a, b) FixedMul(a, b)

typedef s32 (*ArcTan2Fn)(s32, s32);
typedef s32 (*TrigFn)(s32);

void Transform_UpdateVertices(struct Position *from, struct Position *to, struct Vertex *v, struct VertexPair *out)
{
    u32 angle;
    s32 ox;
    s32 oz;
    s32 sn;
    s32 cs;
    s32 i;
    s32 t;
    s32 u;
    s32 a;
    s32 b;
    s32 k;
    s32 *center;
    s32 z;
    s32 x;

    x = to->x;
    z = to->z;
    z -= from->z;
    x -= from->x;
    angle = (u16)((ArcTan2Fn)WaitFramesFar[8].target)(x >> 4, z >> 4);
    {
        s32 half = 0x8000;
        ox = FIXED_MUL(half, to->x);
        oz = FIXED_MUL(half, to->z);
    }
    if (angle != gScanlineAngle) {
        gScanlineAngle = angle;
        sn = ((TrigFn)WaitFramesFar[11].target)(angle);
        cs = ((TrigFn)WaitFramesFar[12].target)(angle);
        center = &gProjection.center_x;
        for (i = 0; i < 160; i++) {
            x = v->x;
            z = v->z;
            t = -(FIXED_MUL(cs, x) / 256);
            out->a = v->u = t;
            u = FIXED_MUL(sn, x) / 256;
            v->v = u;
            out->b = u;
            k = *center;
            a = (out->a << 8) * k;
            v->dx = -(FIXED_MUL(z, -sn) + a);
            b = ((s16)u << 8) * -k;
            v->dz = b + FIXED_MUL(z, cs);
            out->x = (ox + v->dx) >> 8;
            out->y = (oz + v->dz) >> 8;
            v++;
            out[1] = out[0];
            /* FAKEMATCH: reads memory after the stm so bpl waits on this free link, not on the stm's store cost; stm then sits right above bpl (with the next device, differ 68 -> 60) */
            asm("" : "=r"(cs) : "0"(cs), "m"(*v));
            /* FAKEMATCH: rewrites out in place so add r6 no longer waits on the ldm's cost through the anti dependence; add r6 then sits between ldm and stm */
            asm("" : "=r"(out) : "0"(out));
            out += 2;
        }
    } else {
        for (i = 0; i < 160; i++) {
            if (gFrameCount & 1) {
                out->x = ((ox + v->dx) >> 8) + v->v / 4 + v->u / 2;
                out->y = ((oz + v->dz) >> 8) + v->u / 4 + v->v / 2;
            } else {
                out->x = (ox + v->dx) >> 8;
                out->y = (oz + v->dz) >> 8;
            }
            out->a = v->u;
            out->b = v->v;
            /* FAKEMATCH: one empty cycle between strh and ldm, which the strh's store cost of 3 needs, so subs rises above strh as listed (differ 72 -> 68) */
            asm("" : "=r"(v) : "0"(v));
            v++;
            out[1] = out[0];
            /* FAKEMATCH: reads memory after the stm so bpl waits on this free link, not on the stm's store cost; stm then sits right above bpl (with the next device, differ 76 -> 72) */
            asm("" : "=r"(v) : "0"(v), "m"(*v));
            /* FAKEMATCH: rewrites out in place so add r6 no longer waits on the ldm's cost through the anti dependence; add r6 then sits between ldm and stm */
            asm("" : "=r"(out) : "0"(out));
            out += 2;
        }
    }
}
