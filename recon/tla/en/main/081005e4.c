#include "A9_MOTION.H"
#include "TYPES.H"
extern u8 Data_03001f2c[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct RenderOutput *obj;
    struct RenderOutput **tbl;

    i = 0;
    tbl =
        (struct RenderOutput **)(*(s32 *)((u32)&Data_03001f2c) + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}
