#include "types.h"
#include "scene.h"
#include "abi/ui/icon/set_grid_column_byte5.h"

extern u8 *gIw;

void Ui_SetGridColumnByte5(s32 slot, s32 value)
{
    u8 *base = gIw;
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[5] = value;
        base += 56;
    } while (count >= 0);
}
