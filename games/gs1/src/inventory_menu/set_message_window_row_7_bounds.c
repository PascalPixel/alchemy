#include "inventory_menu.h"
#include "m7_interfaces.h"
#include "global_cells.h"

void InventoryMenu_SetMessageWindowRow7Bounds(void)
{
  int state_cell;
  state_cell = ADDR_03001F2C;
  Func_080a23f4(*((struct Bounds080a23f4 **) (((u8 *) (*((void **) state_cell))) + 0x10C)), 0xD, 0, 0x11, 7);
}
