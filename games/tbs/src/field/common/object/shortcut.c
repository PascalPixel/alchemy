#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "game_state.h"
struct BtlAct * BtlActGet(s32 action_id);
struct BtlUnit *UnitGetFar(s32 object_id);

#define BtlActGet ((void * (*)(s32))BtlActGet)

#define ACTION_ID_MASK 0x3FFF

struct AbilityListEntry {
    u16 owner;
    u16 ability;
};

s32 Object_CollectResources(struct AbilityListEntry *output)
{
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Party_ListActiveOwnersFar(object_ids);
    s32 i;

    for (i = 0; i < object_count; i++) {
        struct BtlUnit *object;
        u32 id;
        s32 index;
        u32 resource_id;

        id = object_ids[i];
        object = UnitGetFar(id);
        for (index = 0; index < (s32)(sizeof(object->action_slots) / sizeof(object->action_slots[0])); index++) {
            resource_id = object->action_slots[index].encoded_action & ACTION_ID_MASK;
            if (resource_id == 0)
                break;
            BtlActGet(resource_id);
            output[output_count].owner = id;
            output[output_count].ability = resource_id;
            output_count++;
        }
    }

    return output_count;
}

void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries)
{
    s32 i;

    *first_index = 0;
    *second_index = 0;

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.first_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.first_shortcut >> 10)) {
            *first_index = i;
            break;
        }
    }

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.second_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.second_shortcut >> 10)) {
            *second_index = i;
            break;
        }
    }
}

void Menu_RunSelectionHook(void)
{
    Debug_SelectAbilityPair();
}
