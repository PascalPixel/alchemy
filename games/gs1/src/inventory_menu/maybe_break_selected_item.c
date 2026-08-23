#include "fixed_math.h"
#include "inventory_menu.h"
#include "global_cells.h"
#include "inventory.h"
#include "item.h"
#include "sound_ids.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Random16();
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
s32 Func_080a1d08(s32, s32, s32);
s32 Func_080f9010(s32);

void InventoryMenu_MaybeBreakSelectedItem(void) {
    void *menu;

    menu = *(void **)ADDR_03001F2C;
    if ((FIELD(Item_Get(0x1FF & FIELD(menu, u16 *, 0x178)), u8 *, 0xC) == 2) && (Rand() < 0x2000U)) {
        Inventory_BreakFar(
            FIELD(menu, u8 *, 0x21A),
            FIELD(menu, u16 *, 0x174));
        Func_080f9010(SOUND_ITEM_BREAK);
        Func_080a1d08(0xB86, 0, -1);
    }
}
