#include "types.h"
#include "scene.h"
#include "abi/ui/message/show_choice.h"
#include "global_cells.h"

s32 UiMessage_ShowChoice(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)ADDR_03001F2C + 0x380);
    u8 saved = (*slot)[5];
    Ui_Do(*slot);
    arg0 = Ui_Place(7, 5, arg0);
    (*slot)[5] = saved;
    return arg0;
}
