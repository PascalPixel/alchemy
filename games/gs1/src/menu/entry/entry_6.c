#include "a9_motion.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* menu/entry/place_entry_objects_in_grid.c */
void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct Object080a9bd8 *obj;
    struct Object080a9bd8 **tbl;

    i = 0;
    tbl =
        (struct Object080a9bd8 **)(*(s32 *)ADDR_03001F2C + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_Run(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

/* menu/entry/place_entry_object_in_grid.c */
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

void Menu_PlaceEntryObjectInGrid(struct Object080a9bd8 *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((FixedPoint_Ratio(no, phase) * 0x10) + origin_y);
    obj->x =
        (s16)((Modulo(no, phase) * 0x10) + origin_x);
    Menu_Do(obj);
}
