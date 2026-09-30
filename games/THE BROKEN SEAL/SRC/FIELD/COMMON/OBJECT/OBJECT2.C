#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

s32 Party_ListActiveOwnersFar(u16 *objects);
#define RESOURCE_ID_MASK_0801C7FC 0x3FFF

struct ObjectResource_0801c7fc {
    u16 id;
    u16 padding_02;
};

struct Object_0801c7fc {
    u8 padding_00[0x58];
    struct ObjectResource_0801c7fc resources[32];
};

struct ResourcePair_0801c7fc {
    u16 object_id;
    u16 resource_id;
};

struct Object_0801c7fc *Runtime_GetObject(s32 object_id);
void *Ability_GetData(s32 resource_id);

/* One entry of a collected ability list, in the packed form of a shortcut:
   the owner in the top six bits, the ability in the low ten. */
struct ShortcutListEntry {
    u16 owner;
    u16 ability;
};

/* FAKEMATCH: the first shortcut is read through a volatile field. That keeps
   its read one zero-extended ldrh from the state base plus 0x220; a plain
   read is folded into the address and combined into sign-extending shifts. */
struct ShortcutState {
    u32 unknown_000[0x220 / 4];
    volatile u16 first;
    u16 second;
};

extern struct ShortcutState gGameState;

s32 Menu_Check();

s32 Object_CollectResources(struct ResourcePair_0801c7fc *output)
{
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Party_ListActiveOwnersFar(object_ids);

    if (output_count < object_count) {
        u16 *object_id = object_ids;
        s32 remaining = object_count;

        do {
            struct Object_0801c7fc *object;
            struct ObjectResource_0801c7fc *resource;
            u32 id;
            s32 index;
            u32 resource_id;
            s32 resource_offset;

            id = *object_id;
            object_id++;
            object = Runtime_GetObject(id);
            index = 0;
            resource_offset = sizeof(object->padding_00);
            resource_id =
                *(u16 *)((u8 *)object + resource_offset)
                & RESOURCE_ID_MASK_0801C7FC;

            if (resource_id != 0) {
                struct ResourcePair_0801c7fc *pair;

                resource = object->resources;
                pair = (struct ResourcePair_0801c7fc *)
                    (output_count *sizeof(*pair) + (s32)output);
                do {
                    Ability_GetData(resource_id);
                    pair->object_id = id;
                    pair->resource_id = resource_id;
                    output_count++;
                    pair++;

                    if (++index >= (s32)(sizeof(object->resources)
                                      / sizeof(object->resources[0]))) {
                        break;
                    }

                    resource++;
                    resource_id =
                        resource->id & RESOURCE_ID_MASK_0801C7FC;
                } while (resource_id != 0);
            }
        } while (--remaining != 0);
    }

    return output_count;
}

/* Find the positions of the two Psynergy shortcuts in a 448-entry ability
   list; a shortcut that is not in the list leaves its position at 0. */
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct ShortcutListEntry *entries)
{
    s32 i;
    u16 first;

    *first_index = 0;
    *second_index = 0;

    first = gGameState.first;
    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (first & 0x3ff) &&
            entries[i].owner == (first >> 10)) {
            *first_index = i;
            break;
        }
    }

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.second & 0x3ff) &&
            entries[i].owner == (gGameState.second >> 10)) {
            *second_index = i;
            break;
        }
    }
}

/* menu/run_selection_hook.c */
/* menu/sel/run_selection_hook.c */
void Menu_RunSelectionHook(void)
{
    Menu_Check();
}
