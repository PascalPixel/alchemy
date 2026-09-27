/* NONMATCHING: complete ARM owner [0800a0f8,0800a37c), 644 bytes including
 * its four literal words. Loader 080109e8 copies 0x284 bytes to heap slot 46
 * and calls it without a Thumb tag. No neighbours or padding are included.
 * Baseline hypothesis: typed 20-byte vertex and paired 16-byte output records,
 * signed Q16 products, and the existing runtime veneer callback slots recover
 * both 160-record loops, including aggregate copies and signed division.
 * Prediction: ARM smull/ldm/stm, 8-byte frame and three indirect math calls.
 * Acceptance: all 644 bytes exact, compare-all, test, coverage and verify.
 * Diagnostics: complete ARM aligned diff and sizes. Budget: baseline plus two
 * structural follow-ups, stop at 00:25 Lisbon; record each result here.
 * Baseline stop: approved -marm -mno-apcs-frame compiles and assembles, but
 * the linker rejects historical GAS's ELF flags 0x4 against the generated
 * symbol object's 0x204 (software FP). No linked score or adoption is claimed.
 * Emitted frame is 68 bytes versus ROM's 8; two source hypotheses remain
 * unused pending the separate assembler/ABI policy decision. No extra flags,
 * compiler changes or patched output were used.
 * Read-only trace (2026-09-27): ld receives exactly 0800a0f8.o (0x4)
 * and 0800a0f8.symbols.o (0x204), with -Ttext=0x0800a0f8 and
 * --unresolved-symbols=ignore-all. The latter has empty text/data/bss and
 * only ABS Data_03001cec/03001e40/03001f60/080000c0. candidate.rs emits
 * .thumb for it and uses assembly_command's -mfpu=fpa -mfloat-abi=soft.
 * The compiled object's instructions are integer-only; the link failure is
 * metadata incompatibility, not evidence of FP instruction disagreement.
 * The normal claimed build also assembles externals.o through that command:
 * all 1004 audited current inputs were 0x204 Thumb objects. Raw ARM listings
 * link separately (0x5000000) before binary placement, so the final ROM mixes
 * instruction states without testing this historical ARM/soft-FP ELF mix.
 * This trace neither establishes a scorer-only fix nor changes ABI policy.
 */
#include "FIXED_MATH.H"

struct PlaneVertex {
    s32 u, v;
    s32 x, y;
    s16 dx, dy;
};

struct PlaneLine {
    s16 dx;
    u16 unknown_02;
    s16 dy;
    u16 unknown_06;
    s32 x, y;
};

/* Runtime veneers contain an instruction word followed by a callable address.
 * Slots 8, 11 and 12 target ArcTan2, Trig_Sin and Trig_Cos respectively. */
struct PlaneMathVeneers {
    u32 unknown_00[16];
    u32 arc_opcode;
    s32 (*arc)(s32, s32);
    u32 unknown_48[4];
    u32 sin_opcode;
    s32 (*sin)(s32);
    u32 cos_opcode;
    s32 (*cos)(s32);
};

extern struct PlaneMathVeneers Data_080000c0;
extern s32 Data_03001f60;
extern s32 Data_03001cec;
extern u32 Data_03001e40;

static __inline__ s32 Plane_Mul(s32 left, s32 right)
{
    return ((s64)left * right) >> 16;
}

void Func_0800a0f8(s32 *camera, s32 *pos, struct PlaneVertex *vertex,
                   struct PlaneLine *line)
{
    u16 angle;
    s32 x, y;
    s32 cnt;

    angle = Data_080000c0.arc((pos[0] - camera[0]) >> 4,
                             (pos[2] - camera[2]) >> 4);
    x = Plane_Mul(0x8000, pos[0]);
    y = Plane_Mul(0x8000, pos[2]);
    if (angle != Data_03001f60) {
        s32 sin, cos, neg;
        Data_03001f60 = angle;
        sin = Data_080000c0.sin(angle);
        cos = Data_080000c0.cos(angle);
        neg = -sin;
        for (cnt = 159; cnt >= 0; cnt--) {
            vertex->dx = -(Plane_Mul(cos, vertex->u) / 256);
            line->dx = vertex->dx;
            vertex->dy = Plane_Mul(sin, vertex->u) / 256;
            line->dy = vertex->dy;
            vertex->x = -(Data_03001cec * (vertex->dx << 8)
                          + Plane_Mul(vertex->v, neg));
            vertex->y = -Data_03001cec * (vertex->dy << 8)
                          + Plane_Mul(vertex->v, cos);
            line->x = (x + vertex->x) >> 8;
            line->y = (y + vertex->y) >> 8;
            vertex++;
            line[1] = line[0];
            line += 2;
        }
    } else {
        for (cnt = 159; cnt >= 0; cnt--) {
            if (Data_03001e40 & 1) {
                line->x = ((x + vertex->x) >> 8)
                          + vertex->dy / 4 + vertex->dx / 2;
                line->y = ((y + vertex->y) >> 8)
                          + vertex->dx / 4 + vertex->dy / 2;
            } else {
                line->x = (x + vertex->x) >> 8;
                line->y = (y + vertex->y) >> 8;
            }
            line->dx = vertex->dx;
            line->dy = vertex->dy;
            vertex++;
            line[1] = line[0];
            line += 2;
        }
    }
}
