/// @file
/// Using Psynergy on the field.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "flags.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

#define FIELD_PSYNERGY_MESSAGE(user, ability, message)                         \
  (MessageArgument_PushFar((user), 1), MessageArgument_PushFar((ability), 4),  \
   FieldMessage_ShowFar((s32)&(message), 1))

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9) and its
/// user (bits 10 to 13; 15 for the first party member).
s32 Field_UsePsynergy(u32 code) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 abilityId = code & 0x3ff;
  struct PendingEvent *before;
  s32 group = Ability_Get(abilityId)->fieldEffect;
  s32 userId = (code >> 10) & 15;
  s32 retreat = 0;
  struct PendingEvent *after;
  struct PendingEvent *during;
  struct PendingEvent *pending;
  struct Actor *object;
  s32 selector;
  s32 target;
  s32 cost;
  s32 hit;
  u8 *effect;

  Leader_StopMoving();
  event->unkd30 = 255;
  event->standing = retreat;
  GameFlag_Clear(FLAG_ITEM_USE);
  if (userId == 15)
    userId = 0;
  if (GameFlag_Get(FLAG_IN_BATTLE_ARENA)) {
    FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
    return 0;
  }
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (abilityId == 144 || abilityId == 155) {
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
    if (abilityId == 153) {
      if (gMainRoutineWork.ownerOverride != 0 &&
          GameFlag_Get(FLAG_SHIP_FLIES)) {
        GameFlag_Set(0x147);
        event->raisedTrigger = 253;
        return 0;
      }
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
  }
  if (gMainRoutineWork.ownerOverride != 0 && abilityId == 139) {
    FIELD_PSYNERGY_MESSAGE(userId, 139, MsgKey0dbb);
    return 0;
  }
  if (abilityId == 149) {
    if (GameFlag_Get(FLAG_RETREAT_BLOCKED)) {
      FIELD_PSYNERGY_MESSAGE(userId, 149, MsgKey0dbd);
      return 0;
    }
    MessageArgument_PushFar(149, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgKey0dbc, 13);
    cost = FieldYesNo_Ask(1);
    Message_CloseFinishedWindowsFar();
    if (cost != 0)
      return 0;
    {
      struct MainRoutineWork *state = &gMainRoutineWork;
      s32 scene, entrance;

      scene = (u16)state->retreatScene;
      state->next = scene;
      entrance = (u16)state->retreatEntrance;
      state->entrance = entrance;
    }
    event->exit = 999;
    retreat = 1;
  }

  selector = Scene_SelectPsynergyTarget(group);
  pending = Scene_FindPsynergyTrigger(0x80000005, group, selector);
  if (pending != NULL) {
    target = -1;
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    Field_RunPsynergyEvent(pending, userId, target);
  }
  if (code & 0x2000)
    return Field_RunPsynergyEndEvents(code);

  selector = Scene_SelectPsynergyTarget(group);
  before = Scene_FindPsynergyTrigger(0x10000005, group, selector);
  after = Scene_FindPsynergyTrigger(5, group, selector);
  during = Scene_FindPsynergyTrigger(0x50000005, group, selector);
  if (abilityId == 156 && gMainRoutineWork.next != SCENE_WORLD_MAP &&
      before == NULL && after == NULL && during == NULL) {
    FIELD_PSYNERGY_MESSAGE(userId, 156, MsgKey0dbd);
    return 0;
  }
  if (userId <= 7) {
    struct BattleAct *action = Ability_Get(abilityId);

    struct FieldUnit *unit = Unit_Get(userId);
    s32 ppCost = action->ppCost;

    if (unit->pp < ppCost) {
      if (retreat)
        event->exit = 0;
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dba);
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }
  if (group == 23) {
    object = ObjectTable_Get(Party_GetLeaderActor());
    if (MapGrid_IsSandCell(object->x.fixed, object->z.fixed, object->layer) ==
        0) {
      before = NULL;
      after = NULL;
      during = NULL;
    }
  }
  GameFlag_Set(0x140);
  GameFlag_Clear(0x141);
  target = -1;
  if (before != NULL || after != NULL || during != NULL) {
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    GameFlag_Set(0x141);
    if (after != NULL && (after->flags & 0x400)) {
      GameFlag_Clear(0x140);
      GameFlag_Clear(0x141);
    }
  }
  BattleEffect_LoadActionEffectResources(abilityId, 0);
  event->psynergyActing = 1;
  {
    u32 zero = 0;

    if (before != NULL && (before->flags & 0x200)) {
      effect = Ram_HeapSlots->fieldEffectWork;
      effect[34] = zero;
      effect[35] = zero;
    }
  }
  if (after != NULL && (after->flags & 0x200)) {
    effect = Ram_HeapSlots->fieldEffectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  if (during != NULL && (during->flags & 0x200)) {
    effect = Ram_HeapSlots->fieldEffectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  FieldPsynergy_SetupActorPair(Party_GetLeaderActor(), target);
  EventObject_Initialize();
  Field_RunPsynergyEvent(before, userId, target);
  if (GameFlag_Get(0x140)) {
    if (GameFlag_Get(0x141))
      FieldPsynergy_Dispatch();
    else
      FieldPsynergy_Run();
  }
  BattleEffect_StopMainActor();
  hit = event->unkd30;
  if (hit != -1) {
    selector = hit | 0x100;
    pending = Scene_FindPsynergyTrigger(0x20000005, group, selector);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, userId, event->unkd30);
  }
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_Get(0x140))
    FieldPsynergy_FinishMindRead();
  event->psynergyActing = 0;
  BattleEffect_CleanupSceneObjects();
  GameFlag_Clear(0x140);
  GameFlag_Clear(0x141);
  return 0;
}

/// Runs the event tied to a field Psynergy: a result below 0x10000 is a
/// message, shown at target; a larger one is a routine, called with mode and
/// target. With FLAG_NOTHING_HAPPENS set it then says nothing happens.
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target) {
  if (event != NULL) {
    if (event->action != 0) {
      if (event->action < 0x10000) {
        Event_Start();
        Event_SetMessageId(event->action);
        Event_ShowMessageAndWait(target, 0);
        Event_End();
      } else {
        ((void (*)(s32, s32))event->action)(mode, target);
      }
    }
    if (GameFlag_Get(FLAG_NOTHING_HAPPENS)) {
      Event_Start();
      FieldMessage_ShowFar((s32)MsgNothingHappens, 1);
      Event_End();
    }
  }
  return 0;
}

/// Returns whether the scene has a trigger for the field Psynergy kind on what
/// it would act on (Scene_SelectPsynergyTarget), at any stage.
s32 BattleEffect_HasMatchingEvent5(s32 fieldPsynergy) {
  u16 kind = fieldPsynergy;
  s32 result = (s32)Scene_FindPsynergyTrigger(0xf0000005, kind,
                                              Scene_SelectPsynergyTarget(kind));

  return (u32)((-result) | result) >> 31;
}

/// Returns whether the scene has an event for item and what the leader faces.
u32 BattleEffect_HasTrigger(u16 item) {
  s32 result;

  result = (s32)Field_FindItemTrigger(item);
  return (u32)((0 - result) | result) >> 31;
}
