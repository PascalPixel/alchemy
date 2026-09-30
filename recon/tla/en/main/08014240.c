/*
 * Draft: Resource_ClearSlotReferences does not yet match; 4 halfwords differ from ☀️'s C, first at +0x8 (ldr r2, [pc, #36]).
 * Links as recon/tla/raw/08014220.s.
 */
#include "TYPES.H"

extern u8 ResourceBlockOwners[];

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id > 0x5f)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = 0xff;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}
