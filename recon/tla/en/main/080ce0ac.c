/* Draft, differ 6 of 266, from the listing: an item used from the field
 * menu. It finds the holder with the most of the item when no actor is
 * given, runs the facing trigger's message or callback if there is one,
 * and otherwise runs the item's action effect, asking first for action 149
 * and setting an exit; the used item is removed when flag 0x143 is set.
 * The item id lives in fp in the native, and its 0x3ff mask is built in fp
 * before the and: here a register pin on r11 and an empty asm barrier over
 * the mask give both (plain, differ 16; the pin alone, 16; the barrier
 * alone, 52); both are devices to tag under S2 before adoption. Left: two
 * scheduling swaps, the -1 result's negs issued before mov fp instead of
 * after it, and the party pointer's literal load after movs r0, #128
 * instead of before it. Tried: alchemy permute, two jobs (best weighted
 * 255 at differ 18, by moving one statement). */
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ItemUnit {
    u8 unknown_000[0xd8];
    u16 inventory[15];
};

struct ItemTrigger {
    u8 unknown_00[4];
    u16 trigger;
    u8 unknown_06[2];
    u32 value;
};

struct ItemDefinitionView {
    u8 unknown_00[12];
    u8 use_type;
    u8 unknown_0d[27];
    u16 action_id;
};

struct ItemFieldWork {
    u8 unknown_000[0x158];
    s16 exit;
    u8 unknown_15a[0xcb6 - 0x15a];
    u8 resolving_action;
};

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

extern char MsgNothingHappens;
extern char MsgActorUsesItem;
extern char MsgUseItemQuestion;

s32 Party_CountActiveOwnersFar(void);
struct ItemUnit *UnitGet(s32 owner);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);
struct ItemTrigger *Event_FindFacingTrigger(s32 item);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void UiText_DrawQuantity(s32 value, s32 slot);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080cd91c(s32 id);
void EventRuntime_Begin(void);
void EventRuntime_SetMessage(s32 message);
void EventRuntime_ShowMessageAndWait(s32 actor, s32 flags);
void EventRuntime_End(void);
struct ItemDefinitionView *Item_Get(s32 item);
s32 Func_080d295c(s32 mode);
void UiWork_FinalizePendingCoreFar(void);
void BattleFx_LoadActionEffectResources(s32 action, s32 flags);
void BattleFx_Run(void);
void BattleEffect_CleanupSceneObjects(void);
void Inventory_RemoveFar(s32 actor, s32 slot);

s32 Func_080ce0ac(s32 arg, s32 slot)
{
    s32 result;
    register s32 item_id asm("r11");
    s32 actor;
    struct ItemUnit *obj;
    struct ItemTrigger *event;
    s32 j;
    s32 i;
    s32 best;
    s32 matches;
    s32 count;
    struct ItemFieldWork *runtime;

    result = -1;
    item_id = 0x3ff;
    asm("" : "+r"(item_id));
    item_id &= arg;
    actor = (arg >> 10) & 0xf;
    {
        best = 0;
        count = Party_CountActiveOwnersFar();

        if (actor == 15) {
            actor = 0;
            for (i = 0; i < count; i++) {
                obj = UnitGet(gPartyState.active_owners[i]);
                matches = 0;
                for (j = 0; j < 15; j++) {
                    if ((obj->inventory[j] & 0x1ff) == item_id)
                        matches++;
                }

                if (best < matches) {
                    best = matches;
                    actor = gPartyState.active_owners[i];
                }
            }
        } else {
            obj = UnitGet(actor);
            for (j = 0; j < 15; j++) {
                if ((obj->inventory[j] & 0x1ff) == item_id)
                    best++;
            }
        }

        if (best == 0) {
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
            return -1;
        }
    }

    event = Event_FindFacingTrigger(item_id);
    if (event != 0 && event->value != 0) {
        GameFlag_ClearBit(0x143);
        GameFlag_ClearBit(0x142);
        if (!(event->trigger & 0x400)) {
            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
        }
        if ((s32)event->value < 0x10000) {
            s32 objref = Func_080cd91c(EventRuntime_GetControlledOwner());

            EventRuntime_Begin();
            EventRuntime_SetMessage(event->value);
            EventRuntime_ShowMessageAndWait(objref, 0);
            EventRuntime_End();
        } else {
            ((ItemTriggerCallback)event->value)(item_id, actor, slot);
        }
        result = 0;
    } else {
        s32 action_id;

        GameFlag_ClearBit(0x143);
        GameFlag_SetBit(0x142);
        action_id = Item_Get(item_id)->action_id;
        runtime = Ram_HeapSlots->event_work;

        if (action_id != 0) {
            GameFlag_SetBit(0x145);
            GameFlag_ClearBit(0x142);

            if (action_id == 149 && !GameFlag_Test(0x144)) {
                UiText_DrawQuantity(item_id, 2);
                UiText_ShowPositionedMessageAndWaitFar((s32)&MsgUseItemQuestion, 13);
                i = Func_080d295c(1);
                UiWork_FinalizePendingCoreFar();
                if (i != 0)
                    return 0;

                gPartyState.scene = *(u16 *)&gPartyState.unknown_25e[2];
                gPartyState.entrance = *(u16 *)&gPartyState.unknown_25e[4];
                runtime->exit = 999;
            }

            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
            BattleFx_LoadActionEffectResources(action_id, 0);
            runtime->resolving_action = 1;
            BattleFx_Run();
            runtime->resolving_action = 0;
            BattleEffect_CleanupSceneObjects();

            if (Item_Get(item_id)->use_type & 1)
                GameFlag_SetBit(0x143);
        }
    }

    if (GameFlag_Test(0x142))
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
    if (GameFlag_Test(0x143))
        Inventory_RemoveFar(actor, slot);
    return result;
}
