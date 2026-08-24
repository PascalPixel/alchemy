#include "types.h"

void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length)
{
    s32 entry_count;
    s32 entry_index;

    entry_count = byte_length >> 1;
    entry_index = 1;
    entries++;
    if (entry_index < entry_count) {
        u32 next_marker_mask;
        u32 current_marker_mask;
        u32 payload_mask = 0x7ff;
        next_marker_mask = 0xf800;
        current_marker_mask = 0xf000;
        entries -= 2;
        do {
            u16 current_entry;
            u16 next_entry;
            u32 next_marker_bits;
            u32 current_marker_bits;

            next_entry = entries[2];
            next_marker_bits = next_entry & next_marker_mask;
            entries++;
            if (next_marker_bits == next_marker_mask) {
                current_entry = entries[0];
                current_marker_bits = current_entry & next_marker_bits;
                if (current_marker_bits == current_marker_mask) {
                    u32 packed_offset;
                    u32 adjusted_current;
                    u32 adjusted_next;

                    packed_offset = (((current_entry & payload_mask) << 12)
                        | ((next_entry & payload_mask) * 2)) - (entry_index * 2);
                    adjusted_current = ((packed_offset >> 12) & payload_mask)
                        | current_marker_bits;
                    adjusted_next = ((packed_offset >> 1) & payload_mask)
                        | next_marker_bits;
                    entries[0] = adjusted_current;
                    entries[1] = adjusted_next;
                }
            }
            entry_index++;
        } while (entry_index < entry_count);
    }
}
