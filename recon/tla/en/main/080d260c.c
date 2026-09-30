#include "TYPES.H"
#include "SCENE.H"

extern s32 gGameState[];
extern u8 MsgCannotCarryMore;

extern void UiWork_PushValueSlotFar(s32, s32);
extern void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

s32 PartyInventory_GiveItem(s32 item)
{
    struct EventWork *work;
    s32 saved;
    s32 owner;
    s32 result;
    s32 count;
    s32 i;
    s32 member;
    s32 slot;

    work = gEventWork;
    saved = work->message_position;
    owner = PartyInventory_AddFar(item);
    if (owner == -1) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 1);
        UiText_ShowPositionedMessageAndWaitFar(&MsgCannotCarryAnyMore, 1);
    retry:
        UiText_ShowPositionedMessageAndWaitFar(&MsgWhatWillYouDrop, 1);
        result = Shop_PickUnitItemFar(&member, &slot);
        if (result == -1) {
            if (Item_Get(item)->flags & 8) {
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(&MsgWhatWillYouDrop + 4, 1);
                goto retry;
            }
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(&MsgWhatWillYouDrop + 1, 5);
            result = Object_CallSpawnRoutineAtOrigin(1);
            UiWork_FinalizePendingCoreFar();
            if (result != 0)
                goto retry;
            Item_AdjustCounterFar(item, 1);
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(&MsgWhatWillYouDrop + 2, 1);
            work->message_position = saved;
            return owner;
        } else {
            Owner_GetStateFar(member);
            {
                /* FAKEMATCH: the ROM loads the slot argument before the member. */
                s32 s = slot;

                count = Shop_GetSelectionState(member, s);
            }
            for (i = 0; i < count; i++)
                Inventory_DiscardFar(member, slot);
            owner = PartyInventory_AddFar(item);
            Audio_PlayCue(83);
            if (owner == gGameState[125]) {
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
            } else {
                UiWork_PushValueSlotFar(item, 2);
                UiWork_PushValueSlotFar(owner, 1);
                UiText_ShowPositionedMessageAndWaitFar(&MsgGaveItemToMember, 3);
            }
            work->message_position = saved;
            return owner;
        }
    } else {
        Audio_PlayCue(83);
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
        if (owner != gGameState[125]) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
            UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem + 1, 3);
        }
        work->message_position = saved;
    }
    return owner;
}
