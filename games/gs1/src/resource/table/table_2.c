#include "types.h"
#include "scene.h"

/* resource/table/get_longest_free_block_run.c */
extern u8 ResourceBlockOwners[];

/* Keep the 128-block scan bound live independently from the run counter. */
#define SCAN_BLOCKS(seed) (((u32)(seed) | ~(u32)(seed)) + 129)

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 current_run = 0;
    s32 remaining = SCAN_BLOCKS(marker);
    s32 longest_run = 0;

    remaining <<= 2;
    do {
        if (*marker++ != 0xff) {
            current_run = 0;
        } else {
            current_run++;
            if (longest_run < current_run) {
                longest_run = current_run;
            }
        }
        remaining--;
    } while (remaining != 0);
    return longest_run;
}

/* resource/table/clear_entry_references.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 ResourceBlockOwners[];

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;
    if ((u32)resource_id > 0x5F) {
        return -1;
    }
    marker = ResourceBlockOwners;
    empty_marker = 0xFF;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0) {
        return -1;
    }
    return 0;
}

/* resource/table/reset_entry.c */
struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern struct ResourceTableEntry ResourceTableEntries[];
s32 Resource_ClearSlotReferences(s32 resource_id);

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *resource_entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95) {
        return -1;
    }
    if (resource_entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        resource_entry->flags |= 0xffff;
        resource_entry->value = 0;
    }
    return 0;
}

/* resource/table/activate_entry.c */
extern u8 ResourceTableEntries[];
s32 Resource_ClearSlotReferences(s32 resource_id);

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *resource_entry = (u16 *)(ResourceTableEntries + resource_index * 4);

    if (resource_index > 95)
        return -1;
    if (*resource_entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *resource_entry = value;
    }
    return 0;
}
