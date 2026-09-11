#include "scene.h"
#include "abi/item_menu/win/msg_5.h"
#include "item_menu.h"
#include "m7_interfaces.h"

void ItemMenu_SetMsgWin5(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 5);
}
