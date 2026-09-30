#include "TYPES.H"

struct MetadataSlotState {
    u8 unknown_00[24];
    s32 shifted;
    u8 unknown_1c[4];
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 unknown_24[3];
    u8 count;
    s32 slots[4];
};

struct MetadataRecord {
    u8 first;
    u8 second;
    u16 value;
    u8 unknown_04[2];
    u8 third;
    u8 fourth;
};

s32 AnimationObject_Allocate(s32);
struct MetadataRecord *Resource_GetMetadataRecordFar(s32);
void ResourceMetadata_ClearRecord(void *);

void ResourceMetadata_Unregister(struct MetadataSlotState *state, s32 handle)
{
    s32 *remaining_slot;
    s32 *slot_cursor;
    s32 slot_value;
    s32 later_slot_count;
    u32 slot_index;
    u32 later_index;
    u32 slot_offset;

    if (state != NULL && handle != 0) {
        ResourceMetadata_ClearRecord((void *)handle);
        slot_index = 0;
        if (handle != state->slots[0]) {
            slot_cursor = state->slots;
        next_slot:
            slot_index++;
            if (slot_index <= 3U) {
                slot_cursor++;
                if (handle != *slot_cursor)
                    goto next_slot;
            }
        }
        if (slot_index != 4) {
            slot_offset = slot_index * 4 + 0x28;
            *(s32 *)((u8 *)state + slot_offset) = 0;
            later_index = slot_index + 1;
            later_slot_count = 0;
            if (later_index <= 3U) {
                remaining_slot = (s32 *)(later_index * 4 + (u32)state + 0x28);
                do {
                    slot_value = *remaining_slot++;
                    if (slot_value != 0)
                        later_slot_count++;
                    later_index++;
                } while (later_index <= 3U);
            }
            if (later_slot_count == 0)
                state->count = (s8)slot_index;
        }
    }
}
