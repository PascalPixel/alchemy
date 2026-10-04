/* ☀️'s ResourceSlot_Load over ⚓️'s object-system heap slot: 4 differ, the metadata copy to r10 and the first number load swap. */
#include "TYPES.H"
#include "RESOURCE.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"
#include "RAM_BUFFER.H"

struct ResourceSlotNumber {
    u16 resource;
    s16 number;
};

enum { RESOURCE_SLOT_COUNT = 8 };

extern struct ResourceSlotNumber Data_0802e91c[];
extern u8 Data_080203a8[][256];
#define ResourceSlot_NumberTable Data_0802e91c
#define ResourceSlot_ConversionTables Data_080203a8
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant)
{
    struct AnimationMetadata *metadata;
    struct ObjectSystemWork *work;
    struct ResourceSlot *record;
    struct ResourceSlotNumber *entry;
    u32 *offset;
    u8 *text;
    u8 *end;
    u8 *table;
    u32 size;
    u32 count;
    u32 index;
    u16 entry_number;
    s32 resource;

    if (slot >= RESOURCE_SLOT_COUNT)
        return 0;
    work = Ram_HeapSlots->object_system;
    record = &work->slots[slot];
    metadata = Resource_GetMetadataRecordFar(number);
    record->tag = (slot << 12) | number;
    record->buffer = buffer;

    entry = ResourceSlot_NumberTable;
    for (count = 0; count < 256; count++) {
        s16 *numbers = &entry->number;

        resource = entry->resource;
        entry_number = *numbers;
        entry++;
        if (entry_number == 0)
            return 0;
        if (entry_number == number)
            break;
    }
    size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

    offset = buffer;
    for (count = 0; count < 256; count++) {
        if (*offset == 0)
            break;
        *offset += (u32)buffer;
        offset++;
    }
    if (variant != 0) {
        index = variant - 1;
        text = (u8 *)(offset + 1);
        end = (u8 *)buffer + size;
        if (index > 4)
            index = 0;
        table = ResourceSlot_ConversionTables[index];
        for (; text < end; text++) {
            u32 c = *text;

            if (c <= 0xdf) {
                c = table[c];
                *text = c;
            }
        }
    }
    return metadata->width * metadata->height;
}
