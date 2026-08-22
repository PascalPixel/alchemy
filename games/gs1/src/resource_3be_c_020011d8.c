#include "types.h"

/*
 * resource_3be owner at 0x020011d8, 32 bytes.
 *
 * Runs a step at most 40 times, stopping early once the caller's +12 field has
 * come down to the limit. Both the counter and the field test guard the loop.
 */

struct Work_3be {
    u8 pad00[12];
    s32 f12;                    /* +12 */
};

void Func_02002674();

void Func_020011d8(struct Work_3be *work, s32 limit)
{
    s32 n = 40;

    while (n != 0) {
        Func_02002674(1);
        n--;
        if (work->f12 <= limit) {
            break;
        }
    }
}
