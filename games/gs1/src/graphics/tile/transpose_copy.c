#include "types.h"
#include "scene.h"
#include "resource.h"

/* graphics/tile/transpose_copy.c */
void Graphics_TransposeCopy(s32 source_address, s32 destination_address, s32 row_size, s32 column_count)
{
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

/* resource/load_and_decompress.c */
void Resource_LoadAndDecompress(void *resource_id, void *destination, s32 destination_offset, s32 copy_palette)
{
    u8 *resource = GetResource((s32)resource_id);

    if (copy_palette != 0) {
        void (*copy)(void *, void *, s32) = (void (*)(void *, void *, s32))0x03001388;
        copy((void *)0x05000000, resource, 0x80);
    }
    if (destination_offset != 0) {
        resource += 0x80;
    }
    Sys_Apply(resource, destination);
}
