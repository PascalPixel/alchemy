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
    u32 available_mask;
    u8 padding[4];
    struct PlacementList list;
};

struct BattleObjectSlot;

struct PlacementTable *Battle_Run(s32 side);

struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);

s32 BattlePlacement_UpdateEntries(void)
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

    if (GameFlag_IsSet(364) != 0)
        return;

    {
        struct PlacementList *list = &Battle_Run(0)->list;
        struct PlacementEntry *entry;

        i = 0;
        if (i < list->count) {
            s32 permanent_timer = -1;

            entry = list->entries;
            do {
                if (entry->timer == permanent_timer && GetBattleObjectSlot(entry->id) == 0) {
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
