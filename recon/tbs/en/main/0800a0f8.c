#include "PROJECT.H"
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

extern struct FarCallStub WaitFramesFar[];
extern u32 gScanlineAngle;

extern u32 gFrameCount;

static __inline__ s32 FixedMul(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %0, %1, %2, %3" : "=r"(lo), "=r"(hi) : "r"(a), "r"(b));
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
