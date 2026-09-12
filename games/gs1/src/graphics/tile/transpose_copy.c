#include "types.h"

void Graphics_TransposeCopy(const u8 *source_base, u8 *destination_base,
                            s32 row_size, s32 column_count)
{
    s32 column_index;
    s32 row_index;
    s32 destination_column;
    s32 source_offset;
    u8 *destination;
    const u8 *source;

    column_index = 0;
    if (column_count != 0) {
        source_offset = 0;
        destination_column = column_count;
        do {
            row_index = 0;
            if (row_size != 0) {
                destination = (u8 *)(destination_column + (u32)destination_base - 1);
                source = (const u8 *)(source_offset + (u32)source_base);
                do {
                    row_index++;
                    *destination = *source;
                    source++;
                    destination += column_count;
                } while (row_index != row_size);
            }
            column_index++;
            source_offset += row_size;
            destination_column--;
        } while (column_index != column_count);
    }
}
