#include "TYPES.H"
#include "IWRAM_CALL.H"

/* field/common/map/build_scanline_table.c */

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

typedef void (*TransformFn)(const s32 *source, s32 *destination);

/* The routine is the last argument, so its address is loaded before the
   vectors, as at Iwram_Call2's call sites. */
static __inline__ void Transform(const s32 *source, s32 *destination, TransformFn routine)
{
    routine(source, destination);
}

/* Fill the world map's 160 scanline rows from the camera's view of the
   ground plane at POSITION: a line that meets the plane gets a scale and a
   signed ground distance, any other line zeros. */
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
    s32 zero;
    s32 horizontal;

    zero = 0;
    ground[0] = position[0];
    ground[1] = zero;
    ground[2] = position[2];
    Transform(ground, viewed, Iwram_TransformVector);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.focal;
    for (line = 0; line < 160; line++) {
        distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
        difference = distance - depth;
        if (difference == 0)
            difference = 1;
        scale = Iwram_RatioMulQ14(difference, horizon);
        if (scale < 0) {
            diagonal = Iwram_MulQ16(-scale, 0x8000);
            row->x = Iwram_RatioMulQ14(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            horizontal = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(horizontal, horizontal);
            y = Iwram_MulQ16(vertical, vertical);
            y = Iwram_Sqrt(x + y) << 12;
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
