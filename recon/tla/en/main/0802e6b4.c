#include "TYPES.H"
#include "METADATA_LOOKUP.H"
extern u8 Data_03001e60[];

extern u8 *gSpriteObjects;

void Ui_SetGridColumnByte5(s32 slot, s32 value)
{
    u8 *base = gSpriteObjects;
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[5] = value;
        base += 56;
    } while (count >= 0);
}
