/* 2026-10-03 resource-record ownership closure: use COMMON VRAM_TAB.H's
   two-halfword size/state and byte-offset record. Native VramBlock_LoadCached
   writes the requested byte size at +0 and VRAM byte offset at +2; initialize
   and reset retain their existing zero/0xffff state policies. Current EN
   score 60/1; the formerly missing resource record now compiles. This is a draft, with no new byte credit. */
#include "RESOURCE.H"
#include "TYPES.H"
#include "VRAM_TAB.H"

extern u8 ResourceBlockOwners[];

s32 Resource_ResetEntry(u32 resource_index)
{
    struct VramBlockCacheEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (entry->offset != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        entry->offset |= 0xffff;
        entry->size = 0;
    }
    return 0;
}
