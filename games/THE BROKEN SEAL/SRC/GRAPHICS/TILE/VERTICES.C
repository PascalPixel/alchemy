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
    s32 u : 16;
    s32 v : 16;
};

struct VertexPair {
    s16 a;
    s16 unk2;
    s16 b;
    s16 unk6;
    s32 x;
    s32 y;
};

extern struct FarCallStub FarCall_RuntimeTable[];
extern u32 gScanlineAngle;

extern u32 gFrameCount;

static __inline__ s32 FixedMul(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    /* FAKEMATCH: register choice; RdHi is operand 0 so local-alloc ties it
       with Rs when Rs dies (the loop's sn * x takes x's register, as listed);
       RdLo early-clobbered; stock 52 to 36 with FixedMulPre below. */
    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %1, %0, %2, %3" : "=r,&r"(hi), "=&r,&r"(lo) : "r,r"(a), "0,r"(b));
    return (u32)lo >> 16 | hi << 16;
}
#define FIXED_MUL(a, b) FixedMul(a, b)

static __inline__ s32 FixedMulPre(s32 a, s32 b)
{
    s32 lo;
    s32 hi;
    s32 res;

    /* FAKEMATCH: register choice; the same smull with RdLo as operand 0, so
       local-alloc ties nothing: with FixedMul's order RdHi of oz's smull
       takes the dying 0x8000 (Rm) and reload copies it out (differ 49). */
    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %0, %1, %2, %3" : "=&r,&r"(lo), "=r,&r"(hi) : "r,r"(a), "1,r"(b));
    hi <<= 16;
    /* FAKEMATCH: register choice; as an asm the orr's output reload (ox and oz are spilled) takes reload's next spill register, r2, not the dying RdLo combine_reloads hands a compiled orr (r0); that round robin then lands every later reload register as listed (36 to 10 with FixedMulPreR3) */
    asm("orr %0, %1, %2, lsr #16" : "=r"(res) : "r"(hi), "r"(lo));
    return res;
}

static __inline__ s32 FixedMulPreR3(s32 a, s32 b)
{
    /* FAKEMATCH: register choice; oz's RdLo in r3 (local-alloc's widened lifetimes give it ip), so its output reload goes on to r1 */
    register s32 lo asm("r3");
    s32 hi;
    s32 res;

    /* FAKEMATCH: register choice, as in FixedMulPre */
    CAMELOT_ASM("FixedMul: the ROM's smull puts RdHi in an input register, which GCC's early-clobber mulsidi3 never allocates")
    asm volatile("smull %0, %1, %2, %3" : "=&r,&r"(lo), "=r,&r"(hi) : "r,r"(a), "1,r"(b));
    hi <<= 16;
    /* FAKEMATCH: register choice, as in FixedMulPre */
    asm("orr %0, %1, %2, lsr #16" : "=r"(res) : "r"(hi), "r"(lo));
    return res;
}

union Word {
    s32 i;
};

typedef s32 (*ArcTan2Fn)(s32, s32);
typedef s32 (*TrigFn)(s32);

void Transform_UpdateVertices(struct Position *from, struct Position *to_arg, struct Vertex *v, struct VertexPair *out)
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
    struct Position *to;
    /* FAKEMATCH: the far-call table lives in r9 so the mov below can clobber it; its load then follows the mov's free link, not the push's store cost */
    register struct FarCallStub *far asm("r9");

    /* FAKEMATCH: the parameter copy as an asm, so sched2 links it to the push at cost 1 (an unrecognized consumer) and places it and the r9 load above sub sp as listed; any recognized first writer of a saved register waits the push's cost 5 */
    asm("mov %0, %1" : "=r"(to) : "r"(to_arg) : "r9");
    far = FarCall_RuntimeTable;

    x = to->x - from->x;
    z = to->z - from->z;
    angle = (u16)((ArcTan2Fn)far[8].target)(x >> 4, z >> 4);
    {
        s32 half = 0x8000;
        ox = FixedMulPre(half, to->x);
        oz = FixedMulPreR3(half, to->z);
    }
    /* FAKEMATCH: sched1 barrier; without it oz's orr, a birthing insn, outranks the compare and the gScanlineAngle load lands between smull and orr, taking r3 from oz's RdLo */
    asm volatile("");
    if (angle != gScanlineAngle) {
        {
            u32 arg;

            /* FAKEMATCH: Sin's argument through an asm, so its copy into r0 (an input reload) precedes the store in the insn stream and the call waits on the asm at cost 1; the store, no longer queued behind the call, then sits between them as listed (mov r0, r8; str r8, [r2]) */
            asm("" : "=r"(arg) : "0"(angle));
            gScanlineAngle = angle;
            sn = ((TrigFn)far[11].target)(arg);
        }
        cs = ((TrigFn)far[12].target)(angle);
        center = &gProjection.center_x;
        for (i = 0; i < 160; i++) {
            s32 x = v->x;

            z = v->z;
            t = -(FIXED_MUL(cs, x) / 256);
            a = (s16)t;
            out->a = v->u = t;
            u = FIXED_MUL(sn, x) / 256;
            v->v = u;
            out->b = u;
            /* FAKEMATCH: k read in alias set 0, so its load conflicts with the short stores of v and out and stays below both u stores, as listed */
            k = ((union Word *)center)->i;
            b = ((s16)u << 8) * -k;
            a = (a << 8) * k;
            v->dx = -(a + FIXED_MUL(z, -sn));
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
                {
                    /* FAKEMATCH: y's v->u in r3, as listed; through the two asms below local-alloc gives it r2 and the reloaded oz r3, and the two out->y stores then cross-jump (2 to 0) */
                    register s32 vu asm("r3");

                    /* FAKEMATCH: y's v->u read by an asm, so its anti dependence on str r3, [r6, #8] costs 1, not the store's 2: ldrsh r3 then follows the store as listed (6 to 2) */
                    asm("ldrsh %0, [%1, #16]" : "=r"(vu) : "r"(v));
                    /* FAKEMATCH: v->u / 4's compare and conditional add as one asm, so sched2 places them as one insn: the oz reload, a load, then wins its tie with the ldrsh asm on function-unit hazard and sits between ldrsh and cmp as listed (2 to 0; split, cmp ties with the reload and loses) */
                    asm("cmp %0, #0\n\taddlt %0, %0, #3" : "=r"(vu) : "0"(vu) : "cc");
                    out->y = ((oz + v->dz) >> 8) + (vu >> 2) + v->v / 2;
                }
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
