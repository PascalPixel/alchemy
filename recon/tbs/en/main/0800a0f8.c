/* 0800a0f8: ARM draft for the camelot_arm route (CAMELOT_ARM_SOURCES, K1).
   alchemy diff --camelot-arm: listing 157, draft 157, common 105, differ
   104 (stock agbcc_arm 124). What remains is register choice around the
   smull pairs and the ldrsh of the stored halfword (native keeps it in lr).
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

    /* FAKEMATCH: early-clobber both outputs; ordinary constraints let agscc
       ARM overlap RdLo and Rm, which the assembler diagnoses as invalid. */
    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %0, %1, %2, %3" : "=&r"(lo), "=&r"(hi) : "r"(a), "r"(b));
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
    s32 x;
    s32 z;

    z = to->z - from->z;
    x = to->x - from->x;
    angle = (u16)((ArcTan2Fn)WaitFramesFar[8].target)(x >> 4, z >> 4);
    ox = FIXED_MUL(0x8000, to->x);
    oz = FIXED_MUL(0x8000, to->z);
    if (angle != gScanlineAngle) {
        gScanlineAngle = angle;
        sn = ((TrigFn)WaitFramesFar[11].target)(angle);
        cs = ((TrigFn)WaitFramesFar[12].target)(angle);
        for (i = 0; i < 160; i++) {
            x = v->x;
            z = v->z;
            t = -(FIXED_MUL(cs, x) / 256);
            v->u = t;
            out->a = t;
            u = FIXED_MUL(sn, x) / 256;
            v->v = u;
            out->b = u;
            k = gProjection.center_x;
            a = (v->u << 8) * k;
            b = ((s16)u << 8) * -k;
            v->dx = -(a + FIXED_MUL(z, -sn));
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
