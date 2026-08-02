#include "layout_guard.h"
#include "types.h"

struct TurnOrder_080bad7c {
    u8 padding00[0x58];
    s16 order_a[7];
    s16 order_b[7];
};

struct BattleUnit_080bad7c {
    u8 padding00[0x38];
    s16 health;
};

LAYOUT_OFFSET_GUARD(
    TurnOrder080bad7c_OrderA,
    struct TurnOrder_080bad7c,
    order_a,
    0x58);
LAYOUT_OFFSET_GUARD(
    TurnOrder080bad7c_OrderB,
    struct TurnOrder_080bad7c,
    order_b,
    0x66);
LAYOUT_OFFSET_GUARD(
    BattleUnit080bad7c_Health,
    struct BattleUnit_080bad7c,
    health,
    0x38);

extern struct TurnOrder_080bad7c *Data_03001e74;

struct BattleUnit_080bad7c *Func_08077008(s32);
u32 Func_08004458(void);

/*
 * Choose a random encoded roster position.  Order-A tokens use 0x100 and
 * exclude defeated units; order-B tokens use 0x180 and retain every real
 * entry.  The low bits preserve the entry's original position in its roster.
 */
u16 Func_080bad7c(s32 use_order_a)
{
    const s16 *order = use_order_a != 0
        ? Data_03001e74->order_a
        : Data_03001e74->order_b;
    u16 token_base = use_order_a != 0 ? 0x100 : 0x180;
    u16 candidates[6];
    s32 count = 0;
    s32 index;

    for (index = 0; order[index] != 0xff; index++) {
        s16 unit = order[index];

        if (unit == 0xfe)
            continue;
        if (use_order_a != 0 && Func_08077008(unit)->health == 0)
            continue;
        candidates[count++] = (u16)(token_base | index);
    }

    if (count == 0)
        return 0;
    return candidates[((u32)count * Func_08004458()) >> 16];
}
