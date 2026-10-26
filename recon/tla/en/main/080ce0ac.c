// Under -mthumb-slow-loads (2026-10-26): differ 2 of 266 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Draft, differ 6 of 266, from the listing: an item used from the field
// menu. It finds the holder with the most of the item when no actor is
// given, runs the facing trigger's message or callback if there is one,
// and otherwise runs the item's action effect, asking first for action 149
// and setting an exit; the used item is removed when flag 0x143 is set.
// The item id lives in fp in the native, and its 0x3ff mask is built in fp
// before the and: here a register pin on r11 and an empty asm barrier over
// the mask give both (plain, differ 16; the pin alone, 16; the barrier
// alone, 52); both are devices to tag under S2 before adoption. Left: two
// scheduling swaps, the -1 result's negs issued before mov fp instead of
// after it, and the party pointer's literal load after movs r0, #128
// instead of before it. Tried: alchemy permute, two jobs (best weighted
// 255 at differ 18, by moving one statement).
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ItemUnit {
  u8 unk000[0xd8];
  u16 inventory[15];
};

struct ItemTrigger {
  u8 unk00[4];
  u16 trigger;
  u8 unk06[2];
  u32 value;
};

struct ItemDefinitionView {
  u8 unk00[12];
  u8 useType;
  u8 unk0d[27];
  u16 actionId;
};

struct ItemFieldWork {
  u8 unk000[0x158];
  s16 exit;
  u8 unk15a[0xcb6 - 0x15a];
  u8 resolvingAction;
};

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

extern char MsgNothingHappens;
extern char MsgActorUsesItem;
extern char MsgUseItemQuestion;

s32 Party_CountActiveOwnersFar(void);
struct ItemUnit *Unit_Get(s32 owner);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);
struct ItemTrigger *Event_FindFacingTrigger(s32 item);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void UiText_DrawQuantity(s32 value, s32 slot);
s32 EventRuntime_GetControlledOwner(void);
s32 Field_FindNearbyTalkTarget(s32 id);
void EventRuntime_Start(void);
void EventRuntime_SetMessage(s32 message);
void EventRuntime_ShowMessageAndWait(s32 actor, s32 flags);
void EventRuntime_End(void);
struct ItemDefinitionView *Item_Get(s32 item);
s32 Func_080d295c(s32 mode);
void UiWork_FinalizePendingCoreFar(void);
void BattleEffect_LoadActionEffectResources(s32 action, s32 flags);
void BattleEffect_Run(void);
void BattleEffect_CleanupSceneObjects(void);
void Inventory_RemoveFar(s32 actor, s32 slot);

s32 Func_080ce0ac(s32 arg, s32 slot) {
  s32 result;
  register s32 itemId asm("r11");
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
  itemId = 0x3ff;
  asm("" : "+r"(itemId));
  itemId &= arg;
  actor = (arg >> 10) & 0xf;
  {
    best = 0;
    count = Party_CountActiveOwnersFar();

    if (actor == 15) {
      actor = 0;
      for (i = 0; i < count; i++) {
        obj = Unit_Get(gPartyState.activeOwners[i]);
        matches = 0;
        for (j = 0; j < 15; j++) {
          if ((obj->inventory[j] & 0x1ff) == itemId)
            matches++;
        }

        if (best < matches) {
          best = matches;
          actor = gPartyState.activeOwners[i];
        }
      }
    } else {
      obj = Unit_Get(actor);
      for (j = 0; j < 15; j++) {
        if ((obj->inventory[j] & 0x1ff) == itemId)
          best++;
      }
    }

    if (best == 0) {
      UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
      return -1;
    }
  }

  event = Event_FindFacingTrigger(itemId);
  if (event != 0 && event->value != 0) {
    GameFlag_ClearBit(0x143);
    GameFlag_ClearBit(0x142);
    if (!(event->trigger & 0x400)) {
      UiText_DrawQuantity(actor, 1);
      UiText_DrawQuantity(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
    }
    if ((s32)event->value < 0x10000) {
      s32 objref = Field_FindNearbyTalkTarget(EventRuntime_GetControlledOwner());

      EventRuntime_Start();
      EventRuntime_SetMessage(event->value);
      EventRuntime_ShowMessageAndWait(objref, 0);
      EventRuntime_End();
    } else {
      ((ItemTriggerCallback)event->value)(itemId, actor, slot);
    }
    result = 0;
  } else {
    s32 actionId;

    GameFlag_ClearBit(0x143);
    GameFlag_SetBit(0x142);
    actionId = Item_Get(itemId)->actionId;
    runtime = Ram_HeapSlots->eventWork;

    if (actionId != 0) {
      GameFlag_SetBit(0x145);
      GameFlag_ClearBit(0x142);

      if (actionId == 149 && !GameFlag_Test(0x144)) {
        UiText_DrawQuantity(itemId, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgUseItemQuestion, 13);
        i = Func_080d295c(1);
        UiWork_FinalizePendingCoreFar();
        if (i != 0)
          return 0;

        gPartyState.scene = *(u16 *)&gPartyState.retreatScene;
        gPartyState.entrance = *(u16 *)&gPartyState.retreatEntrance;
        runtime->exit = 999;
      }

      UiText_DrawQuantity(actor, 1);
      UiText_DrawQuantity(itemId, 2);
      UiText_ShowPositionedMessageAndWaitFar((s32)&MsgActorUsesItem, 1);
      BattleEffect_LoadActionEffectResources(actionId, 0);
      runtime->resolvingAction = 1;
      BattleEffect_Run();
      runtime->resolvingAction = 0;
      BattleEffect_CleanupSceneObjects();

      if (Item_Get(itemId)->useType & 1)
        GameFlag_SetBit(0x143);
    }
  }

  if (GameFlag_Test(0x142))
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
  if (GameFlag_Test(0x143))
    Inventory_RemoveFar(actor, slot);
  return result;
}
