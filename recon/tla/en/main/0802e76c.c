#include "TYPES.H"
#include "METADATA_LOOKUP.H"
extern u8 Data_03001e60[];

extern u8 *gSpriteObjects;

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    u8 *base = gSpriteObjects;
    s32 offset;
    s32 count;

    Resource_GetMetadataRecordFar(no);
    offset = (slot & 3) * 4 + 40;
    count = 9;
    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        *(u16 *)entry = no;
        Animation_InitWorkFromMetadata(entry);
        base += 56;
    } while (count >= 0);
}
