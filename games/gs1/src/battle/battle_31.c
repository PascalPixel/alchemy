#include "types.h"
#include "scene.h"
#include "battle_summon.h"
#include "battle_party.h"
#include "owner_state.h"

/* battle/placement/update_entries.c */
struct PlacementEntry {
    u8 x;
    u8 y;
    u8 id;
    s8 timer;
};

struct PlacementList {
    struct PlacementEntry entries[64];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};

struct BattleObjectSlot;

struct PlacementTable *Battle_Run(s32 owner);

struct BattleObjectSlot *Battle_unk2_4(s32 object_id);

s32 Battle_unk3_4(void)
{
    u16 owners[10];
    s32 count;
    s32 i;
    s32 owner;
    s32 x;
    s32 y;

    count = Battle_Check(owners);

    for (i = 0; i < count; i++) {
        owner = owners[i];
        for (x = 0; x <= 3; x++) {
            for (y = 0; y <= 19; y++) {
                if (Battle_Place(owner, x, y) != 0) {
                    struct PlacementList *list = &Battle_Run((u32)owner > 7 ? 1 : 0)->list;
                    s32 j;

                    for (j = 0; j < list->count; j++) {
                        if (x == list->entries[j].x && y == list->entries[j].y)
                            break;
                    }
                    if (j == list->count)
                        Battle_unk2_3(owner, x, y);
                }
            }
        }
    }

    if (Battle_unk2(364) != 0)
        return;

    {
        struct PlacementList *list = &Battle_Run(0)->list;
        struct PlacementEntry *entry;

        i = 0;
        if (i < list->count) {
            s32 permanent_timer = -1;

            entry = list->entries;
            do {
                if (entry->timer == permanent_timer && Battle_unk2_4(entry->id) == 0) {
                    u8 id = entry->id;
                    u8 ex = entry->x;
                    u8 ey = entry->y;

                    Battle_unk3_3(id, ex, ey);
                    Battle_unk4_3(id, ex, ey);
                }
                i++;
                entry++;
            } while (i < list->count);
        }
    }
}

/* battle/summon/update_availability.c */
struct PartyDjinnTotals {
    u8 by_element[4];
};

struct BattleSummonState *Battle_Run(s32 side);

struct BattleSummonState *BattleSummon_UpdateAvailability(void)
{
    struct PartyDjinnTotals totals;
    u16 party_members[10];
    s32 party_size;
    s32 element;
    u32 available_mask;

    party_size = BattleParty_ListActiveMembers(party_members);
    available_mask = 0;

    element = 0;
    do {
        totals.by_element[element] = 0;
        {
            s32 party_slot;

            for (party_slot = 0; party_slot < party_size; party_slot++) {
                struct OwnerValueState *member =
                    OwnerState_GetFar(party_members[party_slot]);

                totals.by_element[element] += member->values[element];
            }
        }
        element++;
    } while (element <= 3);

    element = 0;
    do {
        const struct SummonDefinition *summon =
            SummonDefinition_Get(element);
        s32 elements_met;

        if (summon != 0) {
            const u8 *required = summon->djinn_required;

            elements_met = 0;
            if (totals.by_element[0] >= required[0]) {
                u8 *total = totals.by_element;

                do {
                    elements_met++;
                    if (elements_met > 3)
                        break;
                    total++;
                    required++;
                } while (*total >= *required);
            }

            if (elements_met == 4)
                available_mask |= 1u << element;
        }
        element++;
    } while (element <= 31);

    {
        struct BattleSummonState *state = Battle_Run(0);
        state->available_mask = available_mask;
        return state;
    }
}

void BattleSummon_ReservedNoOp5E10(void)
{
}
