#include "TYPES.H"
#include "RESOURCE.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"
#include "RAM_BUFFER.H"

enum { RESOURCE_SLOT_COUNT = 8 };

/* Which resource holds each animation number, ended by number 0. */
struct ResourceSlotNumber {
    u16 resource;
    s16 number;
};

extern struct ResourceSlotNumber ResourceSlot_NumberTable[];
extern u8 ResourceSlot_ConversionTables[][256];

/* ☀️'s: loads animation number into the slot's buffer, relocating its
   offset table and converting its text with one of five tables; returns
   the frame's area. */
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

/* The buffer of the loaded resource slot tagged key, or 0. */
s32 Animation_LookupValueByKey(s32 key)
{
    struct ObjectSystemWork *work = Ram_HeapSlots->object_system;
    u32 no;
    struct ResourceSlot *p;

    no = 0;
    for (p = work->slots; no < RESOURCE_SLOT_COUNT; no++, p++) {
        if (p->tag == key)
            return (s32)p->buffer;
    }
    return 0;
}
