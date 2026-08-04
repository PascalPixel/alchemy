#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_080030f8(s32);

void Func_08012350(void)
{
    s32 *work;
    s32 count;

    work = *(s32 **)0x03001E70;
    count = 0;
    if (work[1] > 255 || work[2] > 255) {
        goto body;
body:
        Func_080030f8(1);
        ++count;
        if (count >= 300) {
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
