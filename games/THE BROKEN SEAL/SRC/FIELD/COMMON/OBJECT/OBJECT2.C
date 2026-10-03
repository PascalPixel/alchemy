#include "TYPES.H"
#include "OWNER_STATE.H"
#include "GAME_STATE.H"

s32 Party_ListActiveOwnersFar(u16 *objects);
#define ACTION_ID_MASK 0x3FFF

/* An ability collected for an active owner; shortcuts pack the two values. */
struct AbilityListEntry {
    u16 owner;
    u16 ability;
};

struct BattleUnit *Runtime_GetObject(s32 object_id);
void *Ability_GetData(s32 resource_id);

struct ShortcutState {
    u32 unknown_000[0x220 / 4];
    volatile u16 first;
    u16 second;
};

void Debug_SelectAbilityPair(void);

s32 Object_CollectResources(struct AbilityListEntry *output)
{
    /* FAKEMATCH: retain the initial byte-offset resource read and the integer
     * output-cursor address while their native source form is unresolved. */
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Party_ListActiveOwnersFar(object_ids);

    if (output_count < object_count) {
        u16 *object_id = object_ids;
        s32 remaining = object_count;

        do {
            struct BattleUnit *object;
            struct OwnerActionSlot *resource;
            u32 id;
            s32 index;
            u32 resource_id;
            s32 resource_offset;

            id = *object_id;
            object_id++;
            object = Runtime_GetObject(id);
            index = 0;
            resource_offset = (u8 *)object->action_slots - (u8 *)object;
            resource_id =
                *(u16 *)((u8 *)object + resource_offset)
                & ACTION_ID_MASK;

            if (resource_id != 0) {
                struct AbilityListEntry *pair;

                resource = object->action_slots;
                pair = (struct AbilityListEntry *)
                    (output_count *sizeof(*pair) + (s32)output);
                do {
                    Ability_GetData(resource_id);
                    pair->owner = id;
                    pair->ability = resource_id;
                    output_count++;
                    pair++;

                    if (++index >= (s32)(sizeof(object->action_slots)
                                      / sizeof(object->action_slots[0]))) {
                        break;
                    }

                    resource++;
                    resource_id =
                        resource->encoded_action & ACTION_ID_MASK;
                } while (resource_id != 0);
            }
        } while (--remaining != 0);
    }

    return output_count;
}

/* Find the positions of the two Psynergy shortcuts in a 448-entry ability
   list; a shortcut that is not in the list leaves its position at 0. */
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries)
{
    /* FAKEMATCH: keep the existing volatile shortcut view. The canonical
       member-address attempt changes this module from 306 to 298 bytes and
       the shortcut function from 132 to 124 bytes in all six editions. */
    s32 i;
    u16 first;

    *first_index = 0;
    *second_index = 0;

    first = ((struct ShortcutState *)&gGameState)->first;
    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (first & 0x3ff) &&
            entries[i].owner == (first >> 10)) {
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
