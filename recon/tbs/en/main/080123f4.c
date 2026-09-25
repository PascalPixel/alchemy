/* NONMATCHING: 292 bytes, candidate 292, 41 differing halfwords, 26 halfword edits.
 * Fresh reconstruction from the complete listing (2026-09-25). A routine-last
 * transform wrapper restores the view-pointer spill and the high-register roles.
 * WALL: Stack-slot order and the squared-distance call sequence still differ.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

struct ScanlineRow {
    s32 x;
    s32 y;
    s32 zero1;
    s32 zero2;
    s32 unknown;
};

extern struct Projection gProjection;

typedef s32 (*RatioFn)(s32 numerator, s32 denominator);
typedef s32 (*SqrtFn)(s32 value);
typedef void (*TransformFn)(s32 *source, s32 *destination);

static __inline__ void Transform(s32 *source, s32 *destination, TransformFn routine)
{
    routine(source, destination);
}

static __inline__ s32 DivideQ16(s32 numerator, s32 denominator)
{
    return ((RatioFn)0x0300013c)(numerator, denominator);
}

static __inline__ s32 SqrtInteger(s32 value)
{
    return ((SqrtFn)0x030001d8)(value);
}

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct ScanlineRow *row)
{
    s32 ground[3];
    s32 viewed[3];
    s32 horizon;
    s32 focus;
    s32 *view;
    register s32 line;
    s32 distance;
    s32 difference;
    s32 scale;
    s32 x;
    s32 y;
    s32 diagonal;
    s32 vertical;

    ground[0] = position[0];
    ground[1] = 0;
    ground[2] = position[2];
    Transform(ground, viewed, (TransformFn)0x03000250);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.focal;
    for (line = 0; line < 160; line++) {
        distance = DivideQ16(focus, (gProjection.center_y - line) << 16);
        difference = distance - depth;
        if (difference == 0)
            difference = 1;
        scale = DivideQ16(difference, horizon);
        if (scale < 0) {
            diagonal = Iwram_MulQ16(-scale, 0x8000);
            row->x = DivideQ16(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            x = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(x, x);
            y = Iwram_MulQ16(vertical, vertical);
            y = SqrtInteger(x + y) << 12;
            if (vertical < 0)
                y = -y;
            row->y = Iwram_MulQ16(y, 0x8000);
        } else {
            row->x = 0;
            row->y = 0;
        }
        row->zero1 = 0;
        row->zero2 = 0;
        row++;
    }
}
