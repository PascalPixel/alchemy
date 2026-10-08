#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "item_ids.h"
#include "game_state.h"
#include "event_runtime.h"
#include "item.h"
#include "itemgift.h"
#include "btlsys.h"
#include "text/msg_ids.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);


extern void UiWork_PushValueSlotFar(s32, s32);
extern void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

s32 Party_CheckMemberValueTotal(s32 id)
{
    s32 count;
    s32 sum = 0;
    s32 i;

    count = Party_CountActiveOwnersFar(id);
    for (i = 0; i < count; i++)
        sum += Inventory_CountItemFar(gGameState.active_owners[i], id);

    if (sum >= count * 30) {
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
        return -1;
    }
    return 0;
}

extern struct EventRuntime *gEventWork;

void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(void *message, s32 position);
void UiWork_FinalizePendingCoreFar(void);
void AudioCommand_PlayFar(s32 cue);

s32 PartyInventory_GiveItem(s32 item, s32 unused)
{
    struct EventRuntime *work;
    s32 saved;
    s32 owner;
    s32 result;
    s32 count;
    s32 i;
    s32 member;
    s32 slot;

    work = gEventWork;
    saved = work->message;
    owner = PartyInventory_AddFar(item);
    if (owner == -1) {
#if defined(TBS_EDITION_DE)
        if (item == ITEM_BLACK_ORB)
            UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 1);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 1);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 1);
#endif
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
    retry:
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
        result = Shop_PickUnitItemFar(&member, &slot);
        if (result == -1) {
            if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
                goto retry;
            }
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
            result = Object_CallSpawnRoutineAtOrigin(1);
            UiWork_FinalizePendingCoreFar();
            if (result != 0)
                goto retry;
            Item_AdjustCounterFar(item, 1);
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
            work->message = saved;
            return owner;
        } else {
            UnitGetFar(member);
            {
                /* FAKEMATCH: the ROM loads the slot argument before the member. Only the English build, without the thirty-item check below, needs the copy; the direct call matches the rest. */
                s32 s = slot;

                count = Shop_GetSelectionState(member, s);
            }
#if !defined(TBS_EDITION_EN)
            if (Inventory_CountItemFar(member, item) > 29) {
                UiWork_PushValueSlotFar(member, 1);
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
                goto retry;
            }
#endif
            for (i = 0; i < count; i++)
                Inventory_DiscardFar(member, slot);
            owner = PartyInventory_AddFar(item);
            AudioCommand_PlayFar(83);
            if (owner == gGameState.selected_actor) {
#if defined(TBS_EDITION_DE)
                if (item == ITEM_BLACK_ORB)
                    UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 3);
                else {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
                }
#else
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
#endif
            } else {
                UiWork_PushValueSlotFar(item, 2);
                UiWork_PushValueSlotFar(owner, 1);
                UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
            }
            work->message = saved;
            return owner;
        }
    } else {
        AudioCommand_PlayFar(83);
#if defined(TBS_EDITION_DE)
        if (item == ITEM_BLACK_ORB)
            UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 3);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
#endif
        if (owner != gGameState.selected_actor) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
            UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 1);
#else
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 3);
#endif
        }
        work->message = saved;
    }
    return owner;
}
