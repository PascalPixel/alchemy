#include "alhafra_interiors.h"

void Func_020004f4(s32 actor)
{
    s32 msg = (s32)MsgAlhafraInteriors74;

    Func_02000b24(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000ba4(), 0) == 0)
    {
        Battle_WaitMode0(10);
        Func_02000b24(msg + 1);
    }
    else
    {
        Battle_WaitMode0(20);
        Func_02000b24(msg + 2);
    }
    Func_02000b3c(actor, 0);
}

u8 *Func_02000540(void)
{
    if (GameFlag_Test(0x8a7))
        return gData_020019dc;
    if (GameFlag_Test(0x8ab))
        return gData_02001730;
    return gData_020013dc;
}
