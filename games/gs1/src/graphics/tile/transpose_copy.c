#include "types.h"

void Graphics_TransposeCopy(s32 source_address, s32 destination_address, s32 row_size, s32 column_count) {
    s32 column_index;
    s32 row_index;
    s32 destination_column;
    s32 source_offset;
    u8 *destination;
    u8 *source;

    column_index = 0;
    if (column_count != 0) {
        source_offset = 0;
        destination_column = column_count;
        do {
            row_index = 0;
            if (row_size != 0) {
                destination = (destination_column + destination_address) - 1;
                source = source_offset + source_address;
                do {
                    row_index += 1;
                    *destination = *source;
                    source += 1;
                    destination += column_count;
                } while (row_index != row_size);
            }
            column_index += 1;
            source_offset += row_size;
            destination_column -= 1;
        } while (column_index != column_count);
    }
}
