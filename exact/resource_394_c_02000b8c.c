#include "types.h"

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

const struct TileRun *Func_02000b8c(
    const struct TileRun *run,
    s32 x,
    s32 y)
{
    while (run->id != -1) {
        s32 startX = run->x;
        s32 endX = startX;
        s32 startY = run->y;
        s32 endY = startY;

        if (run->vertical == 0)
            endX += 3;
        else
            endY += 3;

        if (x >= startX && x <= endX &&
            y >= startY && y <= endY)
            return run;
        run++;
    }
    return 0;
}
