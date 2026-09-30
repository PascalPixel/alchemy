#include "TYPES.H"
#include "SCENE.H"

extern s32 gGameState[];
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
        p = (u8 *)gGameState + offset;
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

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct ItemData {
    u8 unknown_00[3];
    u8 flags;
};

struct EventWork {
    u8 unknown_000[472];
    s16 message_position;
};

extern struct EventWork *gEventWork;
extern s32 gGameState[];
extern u8 MsgKorosseoRobinGotItem;
extern u8 MsgCannotCarryAnyMore;
extern u8 MsgWhatWillYouDrop;
extern u8 MsgGaveItemToMember;

s32 PartyInventory_AddFar(s32 item);
struct ItemData *Item_Get(s32 item);
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
#endif
