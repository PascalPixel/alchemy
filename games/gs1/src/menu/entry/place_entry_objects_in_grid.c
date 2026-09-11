#include "a9_motion.h"
#include "types.h"
#include "scene.h"
#include "abi/menu/entry/place_entry_objects_in_grid.h"
#include "global_cells.h"


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
