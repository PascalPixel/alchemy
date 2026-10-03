/* 2026-10-03 resource-record ownership closure: use COMMON VRAM_TAB.H's
   two-halfword size/state and byte-offset record. Native VramBlock_LoadCached
   writes the requested byte size at +0 and VRAM byte offset at +2; initialize
   and reset retain their existing zero/0xffff state policies. Current EN
   score 520/6; ResourceBlockOwners remains unresolved. This is a draft, with no new byte credit. */
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"

/* resource/initialize.c */
/* resource/table/initialize.c */
#include "VRAM_TAB.H"
extern u8 ResourceBlockOwners[];

void Resource_InitializeTable(void)
{
    u32 limit = 0x1ff;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = 0xff;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct VramBlockCacheEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->offset |= 0xffff;
            resource_entry->size = 0;
            resource_entry++;
            count++;
        } while (count <= 95);
    }
}
