#include "inventory_menu.h"
#include "m7_interfaces.h"

void InventoryMenu_SetMessageWindowRow6Bounds(void) {
    Func_080a23f4(
        (struct Bounds080a23f4 *)Data_03001f2c->message_window,
        13, 0, 17, 6);
}
