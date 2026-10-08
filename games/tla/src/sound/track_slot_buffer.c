#include "trackbuf.h"

void AudioTrack_RemoveSlotNode(s32 slot)
{
    /* FAKEMATCH: signed back-link reads preserve the reload and 40-byte body; typed pointers use 36. */
    s32 next_node;
    s32 track_table;
    s32 slot_offset;
    s32 next_link_offset;
    void *previous_node;

    track_table = (s32)Flash_Handler3;
    slot_offset = slot * sizeof(struct AudioSlotNode);
    next_link_offset = slot_offset + (u32)&((struct AudioSlotNode *)0)->back;
    next_node = *(s32 *)(track_table + next_link_offset);
    if (next_node != 0) {
        previous_node = *(void **)(track_table + slot_offset);
        if (previous_node != 0) {
            *(s32 *)&((struct AudioSlotNode *)previous_node)->back = next_node;
        }
        **(s32 **)(track_table + next_link_offset) =
            *(s32 *)(track_table + slot_offset);
    }
}
