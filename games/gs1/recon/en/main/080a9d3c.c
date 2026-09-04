#include "types.h"
#include "global_cells.h"

void InventoryMenu_ResetCategoryItemPositions(void);
void Func_080a17c4(void *);

void InventoryMenu_ApplyCategoryItemFlags(const u8 *flags)
{
    u8 *base;
    void **slot;
    void *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = *(u8 **)ADDR_03001F2C;
    InventoryMenu_ResetCategoryItemPositions();
    index = 0;
    slot = (void **)(base + 200);
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            *(u16 *)((u8 *)entry + 6) = kind;
            *(u16 *)((u8 *)entry + 8) = value;
            *(u8 *)((u8 *)entry + 15) = 240;
            Func_080a17c4(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}
