#include "TYPES.H"
extern u8 Flash_Handler3[];

struct AudioTrackSlotWork {
    u8 unknown0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 unknown4404[0x34];
    u32 input_cursor;
    s32 unknown443c;
    u32 input_limit;
};

void AudioTrack_RemoveSlotNode(s32 slot)
{
    s32 next_node;
    s32 track_table;
    s32 slot_offset;
    s32 next_link_offset;
    void *previous_node;

    track_table = *(s32 *)Flash_Handler3;
    slot_offset = slot * 12;
    next_link_offset = slot_offset + 4;
    next_node = *(s32 *)(track_table + next_link_offset);
    if (next_node != 0) {
        previous_node = *(void **)(track_table + slot_offset);
        if (previous_node != 0) {
            *(s32 *)((u8 *)previous_node + 4) = next_node;
        }
        **(s32 **)(track_table + next_link_offset) =
            *(s32 *)(track_table + slot_offset);
    }
}
