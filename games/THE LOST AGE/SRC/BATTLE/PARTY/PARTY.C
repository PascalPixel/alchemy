#include "TYPES.H"
#include "BATTLE_WORK.H"

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BattleSession *order;
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
        for (i = 0; order->enemy_units[i] != 255; i++) {
            if (order->enemy_units[i] != 254) {
                if (dst != NULL)
                    *dst++ = order->enemy_units[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = 255;
    return count;
}
