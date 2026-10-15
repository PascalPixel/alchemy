#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002318(void)
{
    if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1)
    {
        if (!GameFlag_Test(0x304))
            Call3((void (*)())Func_02004da0, 22, 0, 0);
        if (!GameFlag_Test(0x303))
            Call3((void (*)())Func_02004da0, 21, 0, 0);
    }
}

extern const u8 MsgBlazeStoneEmber[];

void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 mode);
s32  UiText_OpenMessageAtObject(s32 id, s32 flags);
s32  Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
s32  PartyInventory_GiveItem(s32 item, s32 count);
s32  PartyInventory_FindOwner(s32 item);
void Object_SetModeById(u32 id, s32 mode);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004dd0(const u8 *message);
void Func_02004e58(s32 item, s32 mode);

void Func_02002364(void)
{
    const u8 *message;

    Func_02004d40();
    Func_02004e90(0);
    message = MsgBlazeStoneEmber;
    UiText_ShowPositionedMessageAndWait(message, 1);
    Func_02004dd0(message + 1);
    UiText_OpenMessageAtObject(12, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0)
    {
        Value2((s32 (*)())PartyInventory_GiveItem, 460, 0);
        if (Value1((s32 (*)())PartyInventory_FindOwner, 460) == -1)
        {
            Func_02004d48();
            return;
        }
        Func_02004e58(460, 3);
        Object_SetModeById(12, 2);
        GameFlag_SetBit(0x9f9);
    }
    Func_02004d48();
}
