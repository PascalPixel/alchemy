/* 0800a0f8: ARM draft for the camelot_arm route (CAMELOT_ARM_SOURCES, K1).
   alchemy diff --camelot-arm: listing 157, draft 157, common 133, differ
   48 (stock agbcc_arm 124). What remains: the shared 0x8000 lands in r2
   where the listing has r1, and the second smull is scheduled before the
   gScanlineAngle pool load; z's load in r4 where the listing has ip; x in
   r1, not r2, so no ldm r7, {r2, r4} and the second loop smull copies its
   low word; (s16)t read back by ldrsh where the listing extends it into lr
   at once.
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
     3 minutes from 48: best differ 53.
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
            v++;
            out[1] = out[0];
            out += 2;
        }
    }
}
