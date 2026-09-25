/* Draft, not exact (2026-09-25): 296 of 292 bytes, 112 differing halfwords.
   Written from the listing after splitting main:08012518 out of this
   owner. What lined up: the ground point and viewed point in stack order,
   horizon before focus for their spill slots, the division, square root and
   transform routines called through IWRAM function pointers.
   Remaining: the ROM passes the transform's output as add r1, sp, #16 and
   only afterwards builds the view pointer (mov r2, sp; adds r2, #16) that
   it spills to sp+0; here CSE gives the argument pseudo and the view pointer
   one register (r9), which pushes the line counter out of fp and gProjection
   out of r9. Tried: &viewed[0], a cast, a struct wrapper, an inline
   transform helper and register/volatile pointers. With the view pointer
   first set after the horizon line (not valid C) the rest is 43 halfwords
   away, so the loop body itself is close. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct ScanlineRow {
    s32 step;
    s32 offset;
    s32 unknown_08;
    s32 unknown_0c;
    s32 unknown_10;
};

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 gProjection;

#define Iwram_DivQ16 ((s32 (*)(s32, s32))0x0300013c)

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct ScanlineRow *row)
{
    s32 *view;
    s32 horizon;
    s32 focus;
    s32 ground[3];
    s32 viewed[3];
    s32 line;
    s32 distance;
    s32 slope;
    s32 x;
    s32 y;
    s32 length;

    ground[0] = position[0];
    ground[1] = 0;
    ground[2] = position[2];
    ((void (*)(s32 *, s32 *))0x03000250)(ground, viewed);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.unk00[0];
    for (line = 0; line < 160; line++) {
        distance = Iwram_DivQ16(focus, (gProjection.unk10 - line) << 16);
        slope = distance - depth;
        if (slope == 0)
            slope = 1;
        slope = Iwram_DivQ16(slope, horizon);
        if (slope < 0) {
            row->step = Iwram_DivQ16(gProjection.unk00[0], Iwram_MulQ16(-slope, 0x8000));
            y = Iwram_MulQ16(slope, distance);
            x = (view[2] - slope) >> 4;
            y = (y - view[1]) >> 4;
            length = ((s32 (*)(s32))0x030001d8)(Iwram_MulQ16(x, x) + Iwram_MulQ16(y, y)) << 12;
            if (y < 0)
                length = -length;
            row->offset = Iwram_MulQ16(length, 0x8000);
        } else {
            row->step = 0;
            row->offset = 0;
        }
        row->unknown_08 = 0;
        row->unknown_0c = 0;
        row++;
    }
}
