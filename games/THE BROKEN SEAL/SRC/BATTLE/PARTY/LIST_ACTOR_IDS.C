#include "TYPES.H"
#include "BATTLE_COMMAND.H"

#define BattleParty_ListActorIds Func_080b6c08

/* Counts the units in the turn orders selected by groups, bit 0 for the
 * normal order and bit 1 for the mirrored one, skipping empty 254 entries.
 * When dst is given, the unit ids are also written there and terminated
 * with 255. */
s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BattleTurnOrder *order;
    s32 count;
    s32 i;

    order = BATTLE_TURN_ORDER;
    count = 0;
    if (groups & 1) {
        for (i = 0; order->normal[i] != 255; i++) {
            if (order->normal[i] != 254) {
                if (dst != NULL)
                    *dst++ = order->normal[i];
                count++;
            }
        }
    }
    if (groups & 2) {
        for (i = 0; order->mirrored[i] != 255; i++) {
            if (order->mirrored[i] != 254) {
                if (dst != NULL)
                    *dst++ = order->mirrored[i];
                count++;
            }
        }
    }
    if (dst != NULL)
        *dst = 255;
    return count;
}
