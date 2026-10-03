#include "TYPES.H"
#include "BATTLE_PARTY.H"

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);

s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts)
{
    u16 unit_ids[8];
    struct DjinnRecoveryList *list;
    s32 total;
    s32 count;
    s32 i;
    s32 j;
    s32 side_mask;
    s32 side;

    count = 0;
    side_mask = BATTLE_SIDE_PARTY;
    if (unit_id > 7)
        side_mask = BATTLE_SIDE_ENEMIES;
    total = BattleParty_ListActorIds(side_mask, unit_ids);
    side = 0;
    if (unit_id > 7)
        side = 1;
    list = &Trade_GetOfferStateFar(side)->list;
    if (counts != 0)
        for (j = 3; j >= 0; j--)
            counts[j] = 0;
    i = 0;
    if (list->count != 0) {
        do {
            if (list->entries[i].turns == -1) {
                for (j = 0; j < total; j++)
                    if (unit_ids[j] == list->entries[i].unit_id)
                        break;
                if (j != total) {
                    if (counts != 0)
                        counts[list->entries[i].element]++;
                    count++;
                }
            }
            i++;
        } while (i != list->count);
    }
    return count;
}
