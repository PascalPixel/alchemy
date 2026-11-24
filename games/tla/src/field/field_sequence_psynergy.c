/// @file
/// Using Psynergy on the field.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "field/field_psynergy_state.h"

/// Shows message with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(user, ability, message)                               \
  (MessageArgument_PushFar((user), MESSAGE_ARGUMENT_ACTOR),                    \
   MessageArgument_PushFar((ability), MESSAGE_ARGUMENT_ABILITY),               \
   FieldMessage_ShowFar((s32)&(message), FIELD_MESSAGE_ZOOM))

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its user
/// (bits 10 to 13; UNIT_NONE for the first party member) and ACT_PACKED to use
/// it without paying PP.
s32 Field_UsePsynergy(u32 code) {
  struct FieldState *fieldState = Ram_HeapSlots->fieldState;
  s32 abilityId = ABILITY_ID(code);
  struct PendingEvent *before;
  s32 group = Ability_Get(abilityId)->fieldPsynergy;
  s32 userId = ACT_UNIT(code);
  s32 retreat = 0;
  struct PendingEvent *after;
  struct PendingEvent *during;
  struct PendingEvent *event;
  struct Actor *actor;
  s32 selector;
  s32 target;
  s32 declined;
  s32 hit;
  struct FieldEffectState *effect;

  Leader_StopMoving();
  fieldState->psynergyHitObject = -1;
  fieldState->standing = retreat;
  GameFlag_Clear(FLAG_ITEM_USE);
  if (userId == UNIT_NONE)
    userId = 0;

  // Where Psynergy has no effect.
  if (GameFlag_Get(FLAG_IN_BATTLE_ARENA)) {
    // "<Psynergy>!!! ...Nothing happened."
    PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
    return 0;
  }
  if (fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (abilityId == ABILITY_REVEAL || abilityId == ABILITY_MAGNET) {
      // "<Psynergy>!!! ...Nothing happened."
      PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
    if (abilityId == ABILITY_HOVER) {
      if (gMainRoutineState.ownerOverride != 0 &&
          GameFlag_Get(FLAG_SHIP_FLIES)) {
        GameFlag_Set(FLAG_SHIP_HOVERING);
        fieldState->raisedTrigger = WORLD_MAP_RAISED_TAKE_OFF;
        return 0;
      }
      // "<Psynergy>!!! ...Nothing happened."
      PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
  }
  if (gMainRoutineState.ownerOverride != 0 && abilityId == ABILITY_SAND) {
    // "Sand!!! ...Nothing happened."
    PSYNERGY_MESSAGE(userId, ABILITY_SAND, MsgKey0dbb);
    return 0;
  }

  // Retreat: ask, then go back to where the party entered this area.
  if (abilityId == ABILITY_RETREAT) {
    if (GameFlag_Get(FLAG_RETREAT_BLOCKED)) {
      // "Retreat... It doesn't work here."
      PSYNERGY_MESSAGE(userId, ABILITY_RETREAT, MsgKey0dbd);
      return 0;
    }
    MessageArgument_PushFar(ABILITY_RETREAT, MESSAGE_ARGUMENT_ABILITY);
    // "Use Retreat?" Yes: retreat; No: nothing more.
    FieldMessage_ShowFar((s32)&MsgKey0dbc, FIELD_MESSAGE_ZOOM |
                                               FIELD_MESSAGE_KEEP_OPEN |
                                               FIELD_MESSAGE_LOWER);
    declined = FieldYesNo_Ask(TRUE); // 0 is yes
    Message_CloseFinishedWindowsFar();
    if (declined != 0)
      return 0;
    {
      struct MainRoutineState *state = &gMainRoutineState;
      s32 scene, entrance;

      scene = (u16)state->retreatScene;
      state->next = scene;
      entrance = (u16)state->retreatEntrance;
      state->entrance = entrance;
    }
    fieldState->exit = EXIT_KEEP_DESTINATION;
    retreat = 1;
  }

  // The scene's first event; a use that pays no PP only runs its ending events.
  selector = Scene_SelectPsynergyTarget(group);
  event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_FIRST),
                                    group, selector);
  if (event != NULL) {
    target = -1;
    if (selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
      target = selector & PSYNERGY_TARGET_ID;
    Field_RunPsynergyEvent(event, userId, target);
  }
  if (code & ACT_PACKED)
    return Field_RunPsynergyEndEvents(code);

  // Find the scene's events; Teleport off the World Map needs one.
  selector = Scene_SelectPsynergyTarget(group);
  before = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_CAST), group, selector);
  after = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_CAST),
                                    group, selector);
  during = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                     group, selector);
  if (abilityId == ABILITY_TELEPORT &&
      gMainRoutineState.next != SCENE_WORLD_MAP && before == NULL &&
      after == NULL && during == NULL) {
    // "Teleport... It doesn't work here."
    PSYNERGY_MESSAGE(userId, ABILITY_TELEPORT, MsgKey0dbd);
    return 0;
  }

  // A party member pays the PP.
  if (userId <= ALLY_MAX) {
    struct AbilityDefinition *ability = Ability_Get(abilityId);

    struct FieldUnit *unit = Unit_Get(userId);
    s32 ppCost = ability->ppCost;

    if (unit->pp < ppCost) {
      // Retreat was already accepted: take its exit back.
      if (retreat)
        fieldState->exit = 0;
      // "<Psynergy>... Not enough PP."
      PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dba);
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }

  // Sand used off sandy ground finds no events.
  if (group == FIELD_PSYNERGY_SAND) {
    actor = Actor_Lookup(Party_GetLeaderActor());
    if (MapGrid_IsSandCell(actor->x.fixed, actor->z.fixed, actor->layer) == 0) {
      before = NULL;
      after = NULL;
      during = NULL;
    }
  }

  // Choose how the Psynergy plays.
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

  // Play the Psynergy; an event marked quiet hides its effect.
  FieldPsynergy_LoadResources(abilityId, 0);
  fieldState->psynergyActing = 1;
  if (before != NULL && (before->flags & TRIGGER_SUBJECT_QUIET)) {
    effect = Ram_HeapSlots->fieldEffectState;
    effect->visible = 0;
    effect->active = 0;
  }
  if (after != NULL && (after->flags & TRIGGER_SUBJECT_QUIET)) {
    effect = Ram_HeapSlots->fieldEffectState;
    effect->visible = 0;
    effect->active = 0;
  }
  if (during != NULL && (during->flags & TRIGGER_SUBJECT_QUIET)) {
    effect = Ram_HeapSlots->fieldEffectState;
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
  // What the Psynergy hit has an event of its own.
  hit = fieldState->psynergyHitObject;
  if (hit != -1) {
    selector = hit | PSYNERGY_TARGET_ACTOR;
    event = Scene_FindPsynergyTrigger(
        PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), group, selector);
    if (event != NULL)
      Field_RunPsynergyEvent(event, userId, fieldState->psynergyHitObject);
  }
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_Get(FLAG_PLAY_PSYNERGY_CAST))
    FieldPsynergy_FinishMindRead();
  fieldState->psynergyActing = 0;
  FieldPsynergy_End();
  GameFlag_Clear(FLAG_PLAY_PSYNERGY_CAST);
  GameFlag_Clear(FLAG_PSYNERGY_TARGET);
  return 0;
}

/// Runs the event tied to a field Psynergy: an action below
/// TRIGGER_RESULT_MESSAGE_END is a message, shown at target; any other is a
/// routine, called with mode (the user, when a cast runs it) and target. With
/// FLAG_NOTHING_HAPPENS set it then says "But nothing happens."
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

  return NONZERO(result);
}

/// Returns whether the scene has an event for item and what the leader faces.
u32 Scene_HasItemTrigger(u16 item) {
  s32 result;

  result = (s32)Field_FindItemTrigger(item);
  return NONZERO(result);
}
