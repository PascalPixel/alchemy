/* Transform_UpdateVertices draft (2026-09-29), not exact: 161 of 161 words,
 * branches and pool line up, remaining differences are register choice and
 * scheduling in the first loop and prologue.
 * Compiler: agscc (GCC 2.96) cannot produce this routine. It if-converts
 * signed x/256 and x/4 to add; movlt, while the reference has cmp; addlt,
 * which agbcc's own ARM compiler (agbcc/gcc_arm, built as pret's
 * agbcc_arm) emits for plain x/4 and x/256 at -O2 -fomit-frame-pointer.
 * That binary is not an approved digest, so this stays a draft.
 * CAMELOT_ASM proof for FixedMul: the ROM's smull puts RdHi in the same
 * register as an input (smull r3, r2, r1, r2), which GCC's own
 * early-clobber mulsidi3 pattern never allocates; an asm with plain "=r"
 * outputs does. */
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;

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

struct Projection {
    s32 unk00[3];
    s32 scale;
    s32 unk10;
};

extern struct FarCallStub WaitFramesFar[];
extern u32 Data_03001f60;
extern struct Projection gProjection;
extern u32 gFrameCount;

static __inline__ s32 FixedMul(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    /* CAMELOT_ASM: see header */
    asm volatile("smull %0, %1, %2, %3" : "=r"(lo), "=r"(hi) : "r"(a), "r"(b));
    return (u32)lo >> 16 | hi << 16;
}
#define FIXED_MUL(a, b) FixedMul(a, b)

typedef s32 (*ArcTan2Fn)(s32, s32);
typedef s32 (*TrigFn)(s32);

void Transform_UpdateVertices(struct Position *from, struct Position *to, struct Vertex *v, struct VertexPair *out)
{
    struct FarCallStub *stubs;
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

    stubs = WaitFramesFar;
    angle = (u16)((ArcTan2Fn)stubs[8].target)((to->x - from->x) >> 4, (to->z - from->z) >> 4);
    ox = FIXED_MUL(0x8000, to->x);
    oz = FIXED_MUL(0x8000, to->z);
    if (angle != Data_03001f60) {
        Data_03001f60 = angle;
        sn = ((TrigFn)stubs[11].target)(angle);
        cs = ((TrigFn)stubs[12].target)(angle);
        for (i = 0; i < 160; i++) {
            x = v->x;
            z = v->z;
            t = -(FIXED_MUL(cs, x) / 256);
            v->u = t;
            out->a = t;
            u = FIXED_MUL(sn, x) / 256;
            v->v = u;
            out->b = u;
            k = gProjection.scale;
            a = k * ((s16)t << 8);
            b = -k * ((s16)u << 8);
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
                out->y = v->u / 4 + ((oz + v->dz) >> 8) + v->v / 2;
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
