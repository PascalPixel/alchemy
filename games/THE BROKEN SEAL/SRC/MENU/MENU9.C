#include "A9_MOTION.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"

extern u8 Data_03001f2c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void UiIcon_PrepareObject(void *arg0);

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct Object080a9bd8 *obj;
    struct Object080a9bd8 **tbl;

    i = 0;
    tbl =
        (struct Object080a9bd8 **)(*(s32 *)((u32)&Data_03001f2c) + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct Object080a9bd8 *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((Math_Div(no, phase) * 0x10) + origin_y);
    obj->x =
        (s16)((Math_Mod(no, phase) * 0x10) + origin_x);
    UiIcon_PrepareObject(obj);
}
