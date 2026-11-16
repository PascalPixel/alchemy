/// @file
/// Using Psynergy on the field.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "field/field_psynergy_work.h"

#define FIELD_PSYNERGY_MESSAGE(user, ability, message)                         \
  (MessageArgument_PushFar((user), MESSAGE_ARGUMENT_ACTOR),                    \
   MessageArgument_PushFar((ability), MESSAGE_ARGUMENT_ABILITY),               \
   FieldMessage_ShowFar((s32)&(message), FIELD_MESSAGE_ZOOM))

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9) and its
/// user (bits 10 to 13; 15 for the first party member).
s32 Field_UsePsynergy(u32 code) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 abilityId = ABILITY_ID(code);
  struct PendingEvent *before;
  s32 group = Ability_Get(abilityId)->fieldPsynergy;
  s32 userId = ACT_UNIT(code);
  s32 retreat = 0;
  struct PendingEvent *after;
  struct PendingEvent *during;
  struct PendingEvent *pending;
  struct Actor *object;
  s32 selector;
  s32 target;
  s32 cost;
  s32 hit;
  struct FieldEffectWork *effect;

  Leader_StopMoving();
  event->psynergyHitObject = -1;
  event->standing = retreat;
  GameFlag_Clear(FLAG_ITEM_USE);
  if (userId == UNIT_NONE)
    userId = 0;
  if (GameFlag_Get(FLAG_IN_BATTLE_ARENA)) {
    FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
    return 0;
  }
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (abilityId == ABILITY_REVEAL || abilityId == ABILITY_RING) {
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
    if (abilityId == ABILITY_HOVER) {
      if (gMainRoutineWork.ownerOverride != 0 &&
          GameFlag_Get(FLAG_SHIP_FLIES)) {
        GameFlag_Set(FLAG_HOVER_CAST);
        event->raisedTrigger = WORLD_MAP_RAISED_TAKE_OFF;
        return 0;
      }
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
  }
  if (gMainRoutineWork.ownerOverride != 0 && abilityId == ABILITY_SAND) {
    FIELD_PSYNERGY_MESSAGE(userId, ABILITY_SAND, MsgKey0dbb);
    return 0;
  }
  if (abilityId == ABILITY_RETREAT) {
    if (GameFlag_Get(FLAG_RETREAT_BLOCKED)) {
      FIELD_PSYNERGY_MESSAGE(userId, ABILITY_RETREAT, MsgKey0dbd);
      return 0;
    }
    MessageArgument_PushFar(ABILITY_RETREAT, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar((s32)&MsgKey0dbc, FIELD_MESSAGE_ZOOM |
                                               FIELD_MESSAGE_KEEP_OPEN |
                                               FIELD_MESSAGE_LOWER);
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
    event->exit = EXIT_KEEP_DESTINATION;
    retreat = 1;
  }

  selector = Scene_SelectPsynergyTarget(group);
  pending = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_FIRST),
                                      group, selector);
  if (pending != NULL) {
    target = -1;
    if (selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
      target = selector & PSYNERGY_TARGET_ID;
    Field_RunPsynergyEvent(pending, userId, target);
  }
  if (code & ACT_PACKED)
    return Field_RunPsynergyEndEvents(code);

  selector = Scene_SelectPsynergyTarget(group);
  before = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_CAST), group, selector);
  after = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_CAST),
                                    group, selector);
  during = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                     group, selector);
  if (abilityId == ABILITY_TELEPORT &&
      gMainRoutineWork.next != SCENE_WORLD_MAP && before == NULL &&
      after == NULL && during == NULL) {
    FIELD_PSYNERGY_MESSAGE(userId, ABILITY_TELEPORT, MsgKey0dbd);
    return 0;
  }
  if (userId <= ALLY_MAX) {
    struct AbilityDefinition *action = Ability_Get(abilityId);

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
  if (group == FIELD_PSYNERGY_SAND) {
    object = Actor_Lookup(Party_GetLeaderActor());
    if (MapGrid_IsSandCell(object->x.fixed, object->z.fixed, object->layer) ==
        0) {
      before = NULL;
      after = NULL;
      during = NULL;
    }
  }
  GameFlag_Set(FLAG_PLAY_PSYNERGY_CAST);
  GameFlag_Clear(FLAG_PSYNERGY_TARGET);
  target = -1;
  if (before != NULL || after != NULL || during != NULL) {
    if (selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
      target = selector & PSYNERGY_TARGET_ID;
    GameFlag_Set(FLAG_PSYNERGY_TARGET);
    if (after != NULL && (after->flags & TRIGGER_SUBJECT_OWN_EFFECT)) {
      GameFlag_Clear(FLAG_PLAY_PSYNERGY_CAST);
      GameFlag_Clear(FLAG_PSYNERGY_TARGET);
    }
  }
  FieldPsynergy_LoadResources(abilityId, 0);
  event->psynergyActing = 1;
  {
    u32 zero = 0;

    if (before != NULL && (before->flags & TRIGGER_SUBJECT_QUIET)) {
      effect = Ram_HeapSlots->fieldEffectWork;
      effect->visible = zero;
      effect->active = zero;
    }
  }
  if (after != NULL && (after->flags & TRIGGER_SUBJECT_QUIET)) {
    effect = Ram_HeapSlots->fieldEffectWork;
    effect->visible = 0;
    effect->active = 0;
  }
  if (during != NULL && (during->flags & TRIGGER_SUBJECT_QUIET)) {
    effect = Ram_HeapSlots->fieldEffectWork;
    effect->visible = 0;
    effect->active = 0;
  }
  FieldPsynergy_SetupActorPair(Party_GetLeaderActor(), target);
  FieldPsynergy_StartCast();
  Field_RunPsynergyEvent(before, userId, target);
  if (GameFlag_Get(FLAG_PLAY_PSYNERGY_CAST)) {
    if (GameFlag_Get(FLAG_PSYNERGY_TARGET))
      FieldPsynergy_Dispatch();
    else
      FieldPsynergy_Run();
  }
  BattleEffect_StopMainActor();
  hit = event->psynergyHitObject;
  if (hit != -1) {
    selector = hit | PSYNERGY_TARGET_ACTOR;
    pending = Scene_FindPsynergyTrigger(
        PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), group, selector);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, userId, event->psynergyHitObject);
  }
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_Get(FLAG_PLAY_PSYNERGY_CAST))
    FieldPsynergy_FinishMindRead();
  event->psynergyActing = 0;
  FieldPsynergy_End();
  GameFlag_Clear(FLAG_PLAY_PSYNERGY_CAST);
  GameFlag_Clear(FLAG_PSYNERGY_TARGET);
  return 0;
}

/// Runs the event tied to a field Psynergy: a result below 0x10000 is a
/// message, shown at target; a larger one is a routine, called with mode and
/// target. With FLAG_NOTHING_HAPPENS set it then says nothing happens.
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target) {
  if (event != NULL) {
    if (event->action != 0) {
      if (event->action < TRIGGER_RESULT_MESSAGE_END) {
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
      FieldMessage_ShowFar((s32)MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      Event_End();
    }
  }
  return 0;
}

/// Returns whether the scene has a trigger for the field Psynergy kind on what
/// it would act on (Scene_SelectPsynergyTarget), at any stage.
s32 Scene_HasPsynergyTrigger(s32 fieldPsynergy) {
  u16 kind = fieldPsynergy;
  s32 result = (s32)Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER_ANY, kind,
                                              Scene_SelectPsynergyTarget(kind));

  return (u32)((-result) | result) >> 31;
}

/// Returns whether the scene has an event for item and what the leader faces.
u32 Scene_HasItemTrigger(u16 item) {
  s32 result;

  result = (s32)Field_FindItemTrigger(item);
  return (u32)((0 - result) | result) >> 31;
}
