#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_0800bc48(void *destination);

void ResourceMetadata_Unregister(void *state, s32 handle)
{
    s32 *remaining_slot;
    s32 *slot_cursor;
    s32 slot_value;
    s32 later_slot_count;
    u32 slot_index;
    u32 later_index;
    u32 slot_offset;

    if ((state != 0) && (handle != 0)) {
        Func_0800bc48(handle);
        slot_index = 0;
        if (handle != FIELD_AT_OFFSET(state, s32, 0x28)) {
            slot_cursor = (s32 *)((u8 *)state + 0x28);
loop_4:
            slot_index += 1;
            if (slot_index <= 3U) {
                slot_cursor += 1;
                if (handle != *slot_cursor) {
                    goto loop_4;
                }
            }
        }
        if (slot_index != 4) {
            slot_offset = (slot_index * 4) + 0x28;
            FIELD_AT_OFFSET(state, s32, slot_offset) = 0;
            later_index = slot_index + 1;
            later_slot_count = 0;
            if (later_index <= 3U) {
                remaining_slot = (s32 *)((later_index * 4) + (u32)state + 0x28);
                do {
                    slot_value = *remaining_slot++;
                    if (slot_value != 0) {
                        later_slot_count += 1;
                    }
                    later_index += 1;
                } while (later_index <= 3U);
            }
            if (later_slot_count == 0) {
                FIELD_AT_OFFSET(state, s8, 0x27) = (s8) slot_index;
            }
        }
    }
}
