#include "item_menu.h"
#include "m7_interfaces.h"

void ItemMenu_SetMsgWin3(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)Data_03001f2c->message_window,
        13, 0, 17, 3);
}
