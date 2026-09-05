#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 SaveState_GetLatestSequence(s32 record_id)
{
    u16 *sequence_cursor;
    u16 sequence;
    u32 latest_sequence;
    u32 slot_index;
    void *save_state;
    void *slot_cursor;

    save_state = *(void **)ADDR_03001F1C;
    slot_index = 0;
    latest_sequence = 0;
    sequence_cursor = save_state + 0x20;
    slot_cursor = save_state;
    do {
        if ((FIELD_AT_OFFSET(slot_cursor, u8 *, 0) != 0) && (record_id == FIELD_AT_OFFSET(slot_cursor, u8 *, 0x10))) {
            sequence = *sequence_cursor;
            if (latest_sequence < (u32) sequence) {
                latest_sequence = (u32) sequence;
            }
        }
        slot_index += 1;
        sequence_cursor += 1;
        slot_cursor += 1;
    } while (slot_index <= 0xFU);
    return latest_sequence;
}
