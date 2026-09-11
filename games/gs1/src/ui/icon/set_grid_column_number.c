#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/ui/icon/set_grid_column_number.h"

extern u8 *gIw;

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    u8 *base = gIw;
    s32 offset;
    s32 count;

    Ui_Run(no);
    offset = (slot & 3) * 4 + 40;
    count = 9;
    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        *(u16 *)entry = no;
        Ui_Do(entry);
        base += 56;
    } while (count >= 0);
}
