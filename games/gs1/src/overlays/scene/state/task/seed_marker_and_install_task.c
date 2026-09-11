#include "types.h"
#include "scene.h"

/* Seed the marker cycle directly from caller coordinates and install it. */
extern u16 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

void Effect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style)
{
    State_Place(x, y, style);

    gOv = (u16)x;
    gOv2 = (u16)y;
    gOv3 = (u16)(style & 3);
    gOv4 = 0;
    gOv5 = 0;

    {
        s32 budget = 0xc80;
        State_Apply(0x0200ae85, budget);
    }
}
