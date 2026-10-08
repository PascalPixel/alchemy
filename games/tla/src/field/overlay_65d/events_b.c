#include "local.h"

void Func_02000188(void)
{
    s32 msg;

    Func_02000648();
    Func_020006d0(0);
    msg = (s32)&MsgFieldAreYouLookingForSomeone;
    Func_02000698(msg);
    UiText_OpenMessageAtObject(9, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020006e8(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02000698(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02000698(msg + 2);
    }
    Func_020006a8(9, 0);
    Func_02000650();
}

void Func_020001e0(void)
{
    Func_02000618(5, 49, 5, 41, 1, 1);
    Func_02000630_(4, 48, 3, 2, 4, 40);
    GameFlag_SetBit(0x85b);
}

void Func_02000218(void)
{
    if (GameFlag_Test_(0x200) == 0) {
        Func_02000620(0);
        Func_02000620(1);
        Func_02000620(2);
        Func_02000618(56, 46, 30, 46, 16, 16);
        Func_02000618(56, 174, 30, 174, 16, 16);
        Func_02000628_(28, 23, 16, 16, 15, 23);
        Func_02000628_(28, 90, 2, 2, 21, 88);
        Func_02000628(28, 90, 4, 4, 17, 89);
        Func_02000628(28, 90, 2, 1, 17, 91);
        Func_02000628(28, 90, 2, 2, 19, 91);
        Func_02000628(28, 92, 2, 2, 15, 92);
        Func_02000628(30, 92, 2, 2, 17, 92);
        GameFlag_SetBit_(0x200);
    }
}

s32 Func_020002f8(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x100;
    return 0;
}
