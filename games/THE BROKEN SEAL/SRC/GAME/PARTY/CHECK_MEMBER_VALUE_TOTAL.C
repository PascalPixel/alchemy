#include "TYPES.H"
#include "SCENE.H"
#include "ITEM_IDS.H"
#include "GAME_STATE.H"
#include "EVENT_RUNTIME.H"
#include "ITEM.H"

extern u8 MsgCannotCarryMore;

extern void UiWork_PushValueSlotFar(s32, s32);
extern void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

s32 Party_CheckMemberValueTotal(s32 id)
{
    s32 count;
    s32 value;
    s32 offset = 0;
    s32 cnt;
    s32 sum = offset;
    u8 *p;

    count = Party_CountActiveOwnersFar(id);
    if (sum < count) {
        offset = 252;
        offset <<= 1;
        p = gGameState.active_owners;
        cnt = count;
        do {
            value = Inventory_CountItemFar(*p, id);
            cnt--;
            p++;
            sum += value;
        } while (cnt != 0);
    }

    if (sum >= count * 30) {
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgCannotCarryMore, 1);
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgCannotCarryMore + 1, 1);
        return -1;
    }
    return 0;
}

extern struct EventRuntime *gEventWork;
extern u8 MsgKorosseoRobinGotItem;
/* The German Black Orb has a line of its own, which names it. */
extern u8 MsgGotBlackOrb;
extern u8 MsgCannotCarryAnyMore;
extern u8 MsgWhatWillYouDrop;
extern u8 MsgGaveItemToMember;

s32 PartyInventory_AddFar(s32 item);
struct ItemDefinition *Item_Get(s32 item);
void *Owner_GetStateFar(s32 owner);
s32 Shop_GetSelectionState(s32 owner, s32 slot);
void Inventory_DiscardFar(s32 owner, s32 slot);
s32 Item_AdjustCounterFar(s32 item, s32 delta);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(void *message, s32 position);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_PickUnitItemFar(s32 *owner, s32 *slot);
s32 Object_CallSpawnRoutineAtOrigin(s32 mode);
void Audio_PlayCue(s32 cue);

s32 PartyInventory_GiveItem(s32 item)
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
            UiText_ShowPositionedMessageAndWaitFar(&MsgGotBlackOrb, 1);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 1);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 1);
#endif
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
            work->message = saved;
            return owner;
        } else {
            Owner_GetStateFar(member);
            {
                /* FAKEMATCH: the ROM loads the slot argument before the member. */
                s32 s = slot;

                count = Shop_GetSelectionState(member, s);
            }
#if !defined(TBS_EDITION_EN)
            /* The other editions turn away a member who already carries
               thirty of the item, and ask again. */
            if (Inventory_CountItemFar(member, item) > 29) {
                UiWork_PushValueSlotFar(member, 1);
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(&MsgWhatWillYouDrop + 7, 1);
                goto retry;
            }
#endif
            for (i = 0; i < count; i++)
                Inventory_DiscardFar(member, slot);
            owner = PartyInventory_AddFar(item);
            Audio_PlayCue(83);
            if (owner == gGameState.selected_actor) {
#if defined(TBS_EDITION_DE)
                if (item == ITEM_BLACK_ORB)
                    UiText_ShowPositionedMessageAndWaitFar(&MsgGotBlackOrb, 3);
                else {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
                }
#else
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
#endif
            } else {
                UiWork_PushValueSlotFar(item, 2);
                UiWork_PushValueSlotFar(owner, 1);
                UiText_ShowPositionedMessageAndWaitFar(&MsgGaveItemToMember, 3);
            }
            work->message = saved;
            return owner;
        }
    } else {
        Audio_PlayCue(83);
#if defined(TBS_EDITION_DE)
        if (item == ITEM_BLACK_ORB)
            UiText_ShowPositionedMessageAndWaitFar(&MsgGotBlackOrb, 3);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem, 3);
#endif
        if (owner != gGameState.selected_actor) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
            UiText_ShowPositionedMessageAndWaitFar(&MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
            UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem + 1, 1);
#else
            UiText_ShowPositionedMessageAndWaitFar(&MsgKorosseoRobinGotItem + 1, 3);
#endif
        }
        work->message = saved;
    }
    return owner;
}
