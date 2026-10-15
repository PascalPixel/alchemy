#include "types.h"
#include "system.h"
extern u8 gData_03001f2c[];

u8 *BattleActGet(s32 action);
s32 BattleFx_HasMatchingEvent5Far(u8 effect);

s32 PsynergyMenu_SelectPartySlot(s32 partySlot)
{
    void *menu = gMenuWork;
    s32 offset = partySlot + 28;
    s32 cursorOffset = partySlot * 4 + 20;
    void *icon;
    u8 byteVal;
    s32 ownerIndex;
    s32 combinedOffset;
    s32 objOff;
    s32 objId;
    void *objPtr;
    void *p456;
    u8 *p2;
    s32 badge;
    s32 result;
    s32 cursorOffset2;

    result = 0;
    icon = *(void **)(menu + cursorOffset);
    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = result;
    ((struct Rec5 *)(*(u8 **)(menu + 540)))->flag = 13;
    p2 = (u8 *)menu + 2;
    ownerIndex = *(s8 *)(menu + offset);
    byteVal = ((struct Cur *)(menu + 537))->mark;
    p2[offset] = byteVal;

    if (ownerIndex == -1) {
        *(u8 *)(menu + offset) = 0;
        combinedOffset = 0;
    } else {
        combinedOffset = ownerIndex * 2;
        UiMenu_SlideCursor(ownerIndex * 24 - 10, 16);
    }

    objOff = combinedOffset + 520;
    objId = *(u16 *)(menu + objOff);
    objPtr = UnitGetFar(objId);
    p456 = menu + 456;
    badge = PsynergyMenu_CollectActions(objPtr, p456, 2);
    *(u8 *)(menu + 536) = (u8)badge;
    result = PsynergyMenu_SetupActionIcons(menu + 520, p456);

    cursorOffset2 = partySlot * 4 + 20;
    icon = *(void **)(menu + cursorOffset2);
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
