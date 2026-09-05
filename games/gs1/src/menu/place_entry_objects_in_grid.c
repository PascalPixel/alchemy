#include "a9_motion.h"
#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase) {
    s32 i;
    struct Object080a9bd8 *obj;
    struct Object080a9bd8 **tbl;

    i = 0;
    tbl =
        (struct Object080a9bd8 **)(*(s32 *)ADDR_03001F2C + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Func_080a9bd8(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}
