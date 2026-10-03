#include "A9_MOTION.H"

void UiIcon_PrepareObject(struct RenderOutput *arg0);

void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((no / phase * 0x10) + origin_y);
    obj->x =
        (s16)((no % phase * 0x10) + origin_x);
    UiIcon_PrepareObject(obj);
}
