#include "A9_MOTION.H"
#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
void UiIcon_PrepareObject(void *arg0);

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
