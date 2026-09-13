#include "item_menu.h"
#include "m7_interfaces.h"
#include "global_cells.h"

void ItemMenu_SetMsgWin7(void)
{
  int state_cell;
  state_cell = ADDR_03001F2C;
  WindowBounds_Set(*((struct WindowBounds **) (((u8 *)(*((void **)state_cell))) + 0x10C)), 0xD, 0, 0x11, 7);
}
