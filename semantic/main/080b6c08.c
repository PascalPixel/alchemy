#include "layout_guard.h"
#include "types.h"

struct TurnOrder_080b6c08 {
    u8 padding00[0x58];
    s16 order_a[7];
    s16 order_b[7];
};

LAYOUT_OFFSET_GUARD(
    TurnOrder080b6c08_OrderA,
    struct TurnOrder_080b6c08,
    order_a,
    0x58);
LAYOUT_OFFSET_GUARD(
    TurnOrder080b6c08_OrderB,
    struct TurnOrder_080b6c08,
    order_b,
    0x66);

extern struct TurnOrder_080b6c08 *Data_03001e74;

/*
 * Collect IDs from either or both battle turn-order lists.  Bit 0 selects
 * order A and bit 1 selects order B.  Each list ends at 0xff; placeholder
 * 0xfe occupies a roster position but is omitted from the result.  When an
 * output buffer is supplied it receives its own 0xff terminator.
 */
s32 Func_080b6c08(s32 groups, u16 *output)
{
    const s16 *orders[2];
    s32 count = 0;
    s32 group;

    orders[0] = Data_03001e74->order_a;
    orders[1] = Data_03001e74->order_b;

    for (group = 0; group < 2; group++) {
        const s16 *entry;

        if ((groups & (1 << group)) == 0)
            continue;

        for (entry = orders[group]; *entry != 0xff; entry++) {
            if (*entry == 0xfe)
                continue;
            if (output != 0)
                *output++ = (u16)*entry;
            count++;
        }
    }

    if (output != 0)
        *output = 0xff;
    return count;
}
