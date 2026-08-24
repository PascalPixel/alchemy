#include "inventory_menu.h"
#include "m7_interfaces.h"
#include "global_cells.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void InventoryMenu_SetItemWindowFiveRowBounds(void) {
    void *menu = *(void **)ADDR_03001F2C;
    struct WindowBounds *window =
        FIELD(menu, struct WindowBounds **, 0x20);

    WindowBounds_Set(window, 0xD, 5, 0x11, 0xA);
}
