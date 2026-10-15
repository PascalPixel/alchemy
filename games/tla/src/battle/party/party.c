#include "types.h"
#include "btlwork.h"
#include "battle_unit.h"

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BtlWork *order;
    s32 count;
    s32 i;

    order = Ram_HeapSlots->battle_work;
    count = 0;
    if (groups & 1) {
        for (i = 0; order->party_units[i] != 255; i++) {
            if (order->party_units[i] != 254) {
                if (dst != NULL)
                    *dst++ = order->party_units[i];
                count++;
            }
        }
    }
    if (groups & 2) {
        for (i = 0; order->enm_no[i] != 255; i++) {
            if (order->enm_no[i] != 254) {
                if (dst != NULL)
                    *dst++ = order->enm_no[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = 255;
    return count;
}

struct BtlUnit *UnitGet(s32 unit_id);

// As BattleParty_ListActorIds, listing only the actors whose HP is not 0.
s32 Func_0811a39c(s32 groups, u16 *dst)
{
    struct BtlWork *order;
    s32 count;
    s32 i;

    order = Ram_HeapSlots->battle_work;
    count = 0;
    if (groups & 1) {
        for (i = 0; order->party_units[i] != 255; i++) {
            if (order->party_units[i] != 254 && UnitGet(order->party_units[i])->hp != 0) {
                if (dst != NULL)
                    *dst++ = order->party_units[i];
                count++;
            }
        }
    }
    if (groups & 2) {
        for (i = 0; order->enm_no[i] != 255; i++) {
            if (order->enm_no[i] != 254 && UnitGet(order->enm_no[i])->hp != 0) {
                if (dst != NULL)
                    *dst++ = order->enm_no[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = 255;
    return count;
}
