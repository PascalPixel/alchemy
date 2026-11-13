/// @file
/// Event commands: waits, received items, questions and destinations.
// Part of field_sequence.c.

#include "../event_commands_internal.h"
#include "flag_call.h"
#include "frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"

void Event_UpdateDebugSkip(void) {
  struct EventWaitWork *work = Ram_HeapSlots->eventWork;

  if (gDebugMode != 0) {
    if (((struct InputState *)&gInput)->pressed & 0x200) {
      work->waitMode = 0;
    }
    if (((volatile struct InputState *)&gInput)->pressed & 0x100) {
      work->waitMode = -1;
    }
  }
}

void Event_Wait(s32 frames) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  if (state->waitMode == 0 && frames != 0)
    wait(frames);
}

void Leader_StopMoving(void) {
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  object->speedLimit = Q16_ONE;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  if (gMainRoutineWork.renderMode == 2)
    Actor_SelectAnimationFar(object, 12);
  else
    Actor_SelectAnimationFar(object, 1);
}

void Event_Start(void) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  AreaNameWindow_HideFar();
  Leader_StopMoving();
  if (state->actionCounter != 0)
    Event_ResolveAllPendingActions();
  state->unkcb0 = 0;
  state->unkcb2 = 0;
  state->unkcb4 = 0;
  state->delay = 16;
  state->waitMode = 0;
  state->messageActor = 0xffff;
  state->choice = -1;
  state->messageState = -1;
  Task_Add((s32)Event_UpdateDebugSkip, TASK_PASS_VBLANK);
  GameFlag_Clear(0x132);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    state->currentOwner = party->currentOwner;
    state->objectStatus = 0;
  }
}

void Event_End(void) {
  Task_Remove((u32)Event_UpdateDebugSkip);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    if (party->ownerOverride == 0)
      FieldCamera_FollowActor(party->currentOwner, 1);
    else
      FieldCamera_FollowActor(8, 1);
    GameFlag_RefreshLureCapFar();
  }
}

void Event_EmptyCallback(void) {}

void Event_EmptyObjectHook(void) {}

struct Actor *Event_RunObjectHookAndWait(s32 object) {
  Event_EmptyObjectHook();
  Actor_ResetForLeader(object);
  wait(1);
  return ObjectTable_Get(Party_GetLeaderActor());
}

void Event_CallWithLastActiveObjectId(s32 event_id) {
  Event_SpawnObjectTable((struct ActorPlacement *)event_id,
                         Actor_FindLastActiveId());
}

void Event_SetSceneEvents(s32 events) {
  ((struct EventWordWork *)Ram_HeapSlots->eventWork)->word10 = events;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value) {
  Unit_Get(event_id);
  Party_AddMemberFar(event_id);
  if (value != 0) {
    Party_ShowJoinedMessageFar(event_id, value);
  }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddMemberFar(first);
  Party_AddMemberFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

s32 Party_CheckShortcut(u32 packed_id) {
  s32 group = (packed_id >> 10) & 0xf;
  s32 entry = packed_id & 0x3ff;

  if (group > 7)
    return -1;
  if (GameFlag_Get(group) == 0)
    return -2;
  if (Unit_KnowsPsynergyFar(group, entry) == 0)
    return -3;
  return 0;
}

void Party_ClearUnusableShortcuts(void) {
  if (Party_CheckShortcut(gMainRoutineWork.packedEvents[0]))
    gMainRoutineWork.packedEvents[0] = 0;
  if (Party_CheckShortcut(gMainRoutineWork.packedEvents[1]))
    gMainRoutineWork.packedEvents[1] = 0;
}

static __inline__ s32 Vitals_Ratio(s32 value, s32 max) {
  s32 ratio = (value << 14) / max;
  s32 clamped = 0x4000;

  if (ratio <= 0x4000) {
    clamped = 0;
    if (ratio >= 0)
      clamped = ratio;
  }
  return clamped;
}

void Party_RemoveMemberRestored(s32 owner) {
  struct OwnerVitals *vitals;
  s32 i;
  s32 count;
  s32 standing;

  Party_RemoveMemberFar(owner);
  Party_ClearUnusableShortcuts();
  vitals = Unit_Get(owner);
  // The two copies in a once-only block keep both ahead of the ratio; as the
  // plain stores, 2 of 148 instructions differ in tla-en.
  do {
    vitals->hp = vitals->maxHp;
    vitals->pp = vitals->maxPp;
  } while (0);
  vitals->hpRatio = Vitals_Ratio(vitals->hp, vitals->maxHp);
  if (vitals->hpRatio == 0 && vitals->hp != 0)
    vitals->hpRatio = 1;
  vitals->ppRatio = Vitals_Ratio(vitals->pp, vitals->maxPp);
  if (vitals->ppRatio == 0 && vitals->pp != 0)
    vitals->ppRatio = 1;
  vitals->status = 0;
  standing = 0;
  count = Party_CountMembersFar();
  for (i = 0; i < count; i++) {
    vitals = Unit_Get(gMainRoutineWork.activeOwners[i]);
    if (vitals->hp != 0)
      standing++;
  }
  if (standing == 0) {
    vitals = Unit_Get(gMainRoutineWork.currentOwner);
    vitals->hp = 1;
    vitals->hpRatio = Vitals_Ratio(vitals->hp, vitals->maxHp);
    if (vitals->hpRatio == 0 && vitals->hp != 0)
      vitals->hpRatio = 1;
    vitals->ppRatio = Vitals_Ratio(vitals->pp, vitals->maxPp);
    if (vitals->ppRatio == 0 && vitals->pp != 0)
      vitals->ppRatio = 1;
  }
}

s32 Party_CheckItemLimit(s32 item) {
  Party_CountMembersFar();
  if (PartyInventory_FindRoomFar(item) < 0) {
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry, 1);
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry + 1, 1);
    return -1;
  }
  return 0;
}

// The mode of the message that an item went to another member: patch 2 (German
// alone) changes it, and patch 3 drops the message.
#if TLA_PATCH_2 && !TLA_PATCH_3
#define ITEM_GIVEN_MODE 1
#else
#define ITEM_GIVEN_MODE 3
#endif

s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s16 saved = work->messageId;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)&MsgItemReceived, 1);
    FieldMessage_ShowFar((s32)&MsgItemCannotCarry, 1);
  retry:
    {
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt, 1);
      result = ItemPicker_RunToDropFar(&member, &slot);
      if (result == -1) {
        if (((struct ItemHead *)Item_GetDirectFar(item))->flags & 8) {
          MessageArgument_PushFar(item, 2);
          FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 4, 1);
          goto retry;
        }
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 1, 5);
        result = FieldYesNo_Ask(1);
        Message_CloseFinishedWindowsFar();
        if (result != 0)
          goto retry;
        Item_AdjustCounterFar(item, 1);
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 2, 1);
        goto done;
      }
      Unit_Get(member);
      count = Inventory_GetQuantityFar(member, slot);
      if (Inventory_CountItemFar(member, item) > 29) {
        MessageArgument_PushFar(member, 1);
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_DiscardFar(member, slot);
      Unit_RefreshPsynergyFar(member);
      Unit_CalculateFar(member);
      owner = PartyInventory_AddFar(item);
      Sound_PlayFar(83);
      if (owner == gMainRoutineWork.currentOwner) {
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
      } else {
        MessageArgument_PushFar(item, 2);
        MessageArgument_PushFar(owner, 1);
#if !TLA_PATCH_3
        FieldMessage_ShowFar((s32)&MsgItemGiven, ITEM_GIVEN_MODE);
#endif
      }
    }
  done:
    work->messageId = saved;
    return owner;
  }
  Sound_PlayFar(83);
  MessageArgument_PushFar(item, 2);
  FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
  if (owner != gMainRoutineWork.currentOwner) {
    MessageArgument_PushFar(item, 2);
    MessageArgument_PushFar(owner, 1);
    FieldMessage_ShowFar((s32)&MsgItemReceived + 1, 1);
  }
  work->messageId = saved;
  return owner;
}

// Inventory: give an item and return its owner.

/// Gives the item. Returns the character ID on success, or -1 if the
/// inventory is full.
/// item: item ID.
/// owner: recipient character ID.
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 owner) {
  if (Inventory_AddItemFar(owner, item) >= 0)
    return owner;
  return -1;
}

void Inventory_NoOpCallback(void) {}

void Inventory_ReservedNoOp(void) {}

void Actor_WaitForAnimationEnd(s32 objectId, s32 value) {
  struct Actor *obj = ObjectTable_Get(objectId);

  if (obj != 0 && obj->animationKind == 1) {
    s32 cnt = 0;
    u8 *anim = (u8 *)obj->sprite;

    while (cnt <= 89) {
      wait(1);
      if (value != anim[0x18]) {
        break;
      }
      cnt++;
    }
  }
}

/// The event command "choose yes or no": waits for the message, asks yes or no
/// below it or above it when the message windows reach past row 15, and the
/// speaker nods (animation 3) at yes and shakes its head (4) at no. It waits
/// (2 - event mode) * 3 frames, passes the name and its width and a third value
/// to the menu, and skips the animation wait in event mode 2.
// As ☀️'s Event_AskYesNoWithActorAnimation
// (games/tbs/src/object/psynergy_effects.c), less its wait for released keys.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 sprite = *(s16 *)Scene_FindActorPlacement(*(s32 *)(work + 0x1e0));
  struct Window *window = *(struct Window **)(work + 0x1e4);
  struct Window *side = *(struct Window **)(work + 0x1e8);
  s32 useBottom = 1;
  s32 answer;

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait((2 - gMainRoutineWork.eventMode) * 3);

  if (forceBottom == 0) {
    s32 bottom = (s16)window->y + window->height;

    if (side != NULL) {
      s32 sideBottom = (s16)side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = YesNoMenu_RunFar(useBottom, *(s16 *)(work + 0xcb0),
                            *(s16 *)(work + 0xcb2), *(s16 *)(work + 0xcb4));
  if (answer != 0) {
    Actor_SetModeById(actor, 4);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineWork.eventMode != 2)
      Actor_WaitForAnimationEnd(actor, 4);
  } else {
    Actor_SetModeById(actor, 3);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineWork.eventMode != 2)
      Actor_WaitForAnimationEnd(actor, 3);
  }

  return answer;
}

s32 FieldYesNo_Ask(s32 a) { return YesNoMenu_RunFar(a, 0, 0, 0); }

/// Gives the party item from actor: with no room it shows the item, says it
/// was found and left behind and returns -1; otherwise it adds the item, sets
/// flag (unless -1) and returns 0.
s32 Event_PickUpItem(s32 actor, s32 item, s32 flag) {
  if (PartyInventory_FindRoomFar(item) < 0) {
    Actor_GetById(actor)->scaleX = 0;
    ItemDisplay_ShowItem(item, 3);
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)&MsgFoundItem, 1);
    // Four messages on: "But <protagonist>'s party can't carry any more".
    FieldMessage_ShowFar((s32)&MsgFoundItem + 4, 1);
    Actor_GetById(actor)->scaleX = Q16_ONE;
    Actor_FaceActorById(gMainRoutineWork.currentOwner, actor, 0);
    Event_Wait(20);
    return -1;
  }
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
  ItemDisplay_ShowItem(item, 3);
  PartyInventory_GiveItem(item, 0);
  if (flag != -1)
    GameFlag_Set(flag);
  return 0;
}

// Messages: teach an ability and display the learning message.

/* VIEW(event_commands): routine Unit_LearnPsynergyFar as void
 * Unit_LearnPsynergyFar(void); gamepak/far_call/game.h has void
 * Unit_LearnPsynergyFar(s32 owner, s32 action); tried as the 2 declarations'
 * void Unit_LearnPsynergyFar(void), it does not compile in tla-ja:
 * games/tla/src/field/field_sequence.c:6871: conflicting types for
 * `Unit_LearnPsynergyFar'. */
extern void
Unit_LearnPsynergyFar__event_commands(void) __asm__("Unit_LearnPsynergyFar");

/// Teaches the ability, plays a sound, shows a message and waits for it to
/// advance.
/// unit: character ID (message name 1).
/// ability: ability ID (message ability 4).
void Party_LearnAbility(s32 unit, s32 ability) {
  // Declared without arguments, but unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar__event_commands();
  Sound_PlayFar(83);
  MessageArgument_PushFar(unit, 1);
  MessageArgument_PushFar(ability, 4);
  FieldMessage_ShowFar(MsgAbilityLearned, 3);
}

void Event_StartScreenTransition(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  ScreenTransition_Start(event->screenEffect, event->delay);
  event->screenStatus = 1;
}

void Event_FinishScreenTransition(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  ScreenTransition_Finish(event->screenEffect, event->delay);
  event->screenStatus = 0;
}

void Event_WaitTransitionFrames(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  wait(event->delay);
}

void Event_SetDestination(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party;

  event->countdown = 999;
  party = &gMainRoutineWork;
  party->scene = scene;
  party->entrance = entrance;
}

void Event_SetRestartPoint(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party;

  event->countdown = 999;
  party = &gMainRoutineWork;
  party->savedScene = scene;
  party->savedEntrance = entrance;
}

void Event_SetExitRequest(u16 frames) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->countdown = frames;
}
