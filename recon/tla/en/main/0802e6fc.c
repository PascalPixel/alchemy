#include "TYPES.H"
#include "METADATA_LOOKUP.H"
extern u8 Data_03001e60[];

extern u8 *gSpriteObjects;

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct UiGridEntry *entry;
    u8 *work;

    work = *(u8 **)((u32)&Data_03001e60);
    count = 0;
    offset = ((3 & slot) * 4) + 0x28;
    index = 0;
    do {
        entry = *(struct UiGridEntry **)(work + offset);
        if (entry->table_0c != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->no);
            if (value < metadata->animation_count) {
                entry->value_04 = metadata->draw_kind;
                entry->value_10 = entry->table_0c[value];
                entry->x = count * 0x10;
                entry->value_15 = 0x10;
                entry->value_14 = index;
                entry->value_17 = index;
                entry->value_16 = 0xFF;
            }
            work[0x23] = (u8)metadata->adjust_y;
            *(s16 *)(work + 0x1e) = index;
        }
        count += 1;
        work += 0x38;
    } while (count <= 9);
}
