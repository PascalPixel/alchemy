#include "local.h"

void Func_02000a24(void)
{
    s32 seen;
    const u8 *message;

    Func_02003d7c();
    Func_02003eac(0);
    seen = GameFlag_Test(0x95d);
    message = MsgCheckedTheColumn;
    if (!seen) {
        UiText_ShowPositionedMessageAndWait(message, 1);
        UiText_ShowPositionedMessageAndWait(message + 1, 1);
        if (PartyInventory_FindOwner(0x1c8) != -1)
            ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
    } else {
        UiText_ShowPositionedMessageAndWait(message, 1);
        Func_02003d64(0x1c8, 2);
        UiText_ShowPositionedMessageAndWait(message + 2, 1);
    }
    Func_02003d84();
}
