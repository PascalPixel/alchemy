#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void WaitFrames(s32);

void Map_WaitWorkValuesBelow256(void)
{
    s32 *work;
    s32 cnt;

    work = *(s32 **)ADDR_03001E70;
    cnt = 0;
    if (work[1] > 255 || work[2] > 255) {
        goto body;
body:
        WaitFrames(1);
        ++cnt;
        if (cnt >= 300) {
            goto done;
        }
        if (work[1] > 255) {
            goto body;
        }
        if (work[2] > 255) {
            goto body;
        }
    }
done:
    work[3] = 0;
}
