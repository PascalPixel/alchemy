#include "TYPES.H"
#include "BATTLE_COMMAND.H"


/* Counts the units in the lists selected by groups, bit 0 for the
 * party and bit 1 for the enemies, skipping removed 254 entries.
 * When dst is given, the unit ids are also written there and terminated
 * with 255. */
s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    struct BattleSession *order;
    s32 count;
    s32 i;

    order = gBattleWork;
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
