/// @file
/// Using Psynergy on the field.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its
/// user (bits 10 to 13; 15 for character 0) and ACT_PACKED to use it without
/// paying PP. Checks it can be used here (Reveal not on the World Map, Retreat
/// where it is not blocked, after asking), pays its PP, then plays it
/// between the scene's Psynergy triggers for what the leader faces:
/// PSYNERGY_PHASE_BEFORE_CAST before and PSYNERGY_PHASE_AFTER_CAST after. A
/// PSYNERGY_PHASE_ACTS event is not run here; it only makes
/// the Psynergy aim at the actor ahead, and the Psynergy that acts on it
/// (Force, Frost, Reveal) finds it itself. An after trigger whose subject
/// has TRIGGER_SUBJECT_OWN_EFFECT plays the Psynergy itself. Called by
/// SceneManager_Run for the Psynergy request (work->psynergy); sets
/// work->psynergyActing while the Psynergy plays. On the World Map, placed
/// actors far from the view are released for the Psynergy, and all placed
/// actors are placed again afterwards. Returns 0 whatever happens;
/// SceneManager_Run ignores it.
s32 Field_UsePsynergy(u32 code) {
  s32 abilityId = ABILITY_ID(code);
  struct FieldWork *work = gFieldWork;
  const struct Trigger *before;
  const struct Trigger *after;
  const struct Trigger *during;
  s32 userId;
  s32 target;
  s32 retreat;
  s32 abilityGroup;
  s32 ppCost;
  s32 declined;

  abilityGroup = Ability_Get(abilityId)->fieldPsynergy;
  userId = ACT_UNIT(code);
  // The leader is looked up and the result is not used.
  Actor_Lookup(gSaveBlock.leaderActorId);
  retreat = 0;
  Leader_StopMoving();
  GameFlag_ClearFar(FLAG_ITEM_USE);
  if (userId == UNIT_NONE)
    userId = 0;

  // Where Psynergy has no effect.
  if (GameFlag_GetFar(FLAG_IN_BATTLE_ARENA)) {
    PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
    return 0;
  }
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP && abilityId == ABILITY_REVEAL) {
    PSYNERGY_MESSAGE(userId, ABILITY_REVEAL, MsgNothingHappened);
    return 0;
  }

  // Retreat: ask, then go back to where the party entered this area.
  if (abilityId == ABILITY_RETREAT) {
    if (GameFlag_GetFar(FLAG_RETREAT_BLOCKED)) {
      PSYNERGY_MESSAGE(userId, ABILITY_RETREAT, MsgDoesNotWorkHere);
      return 0;
    }
    MessageArgument_PushFar(ABILITY_RETREAT, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar(MsgUseAbilityConfirm, FIELD_MESSAGE_ZOOM |
                                                   FIELD_MESSAGE_KEEP_OPEN |
                                                   FIELD_MESSAGE_LOWER);
    declined = FieldYesNo_Ask(1); // 0 is yes
    TextPrinter_CloseFinishedWindowsFar();
    if (declined != 0)
      return 0;
    {
      struct SaveBlock *state = &gSaveBlock;
      s32 sceneId, returnEntrance;
      sceneId = (u16)state->retreatScene;
      state->sceneId = sceneId;
      returnEntrance = (u16)state->retreatEntrance;
      state->entrance = returnEntrance;
    }
    work->exit = EXIT_KEEP_DESTINATION;
    retreat = 1;
  }
  // A use that pays no PP only runs the scene's ending events.
  if (code & ACT_PACKED)
    return Field_RunPsynergyEndEvents(code);

  // A party member pays the PP.
  if (userId <= ALLY_MAX) {
    ppCost = Ability_Get(abilityId)->ppCost;
    if (Unit_GetFar(userId)->pp < ppCost) {
      PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
      // Retreat was already accepted: take its exit back (the scene and
      // entrance it chose stay in the save block).
      if (retreat)
        work->exit = 0;
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }

  // Find the scene's events, and choose how the Psynergy plays. The before
  // event runs before the two flags are read, so it can change them.
  before = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_CAST), abilityGroup, &target);
  after = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_CAST),
                                    abilityGroup, &target);
  during = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                     abilityGroup, &target);
  target = -1;
  GameFlag_SetFar(FLAG_PLAY_PSYNERGY_CAST);
  GameFlag_SetFar(FLAG_PSYNERGY_TARGET);
  if (before || after || during) {
    target =
        Field_FindNearbyPsynergyTarget(gSaveBlock.leaderActorId, abilityGroup);
    // The after event plays the Psynergy itself.
    if (after && (after->subject & TRIGGER_SUBJECT_OWN_EFFECT)) {
      GameFlag_ClearFar(FLAG_PLAY_PSYNERGY_CAST);
      GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);
    }
  } else
    GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);

  // Play the Psynergy.
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
    Actor_ClearOutOfBounds();
  FieldPsynergy_LoadResources(abilityId, 0);
  work->psynergyActing = 1;
  FieldPsynergy_SetupActorPair(gSaveBlock.leaderActorId, target);
  FieldPsynergy_StartCast();

  Field_RunPsynergyEvent(before, userId, target);
  if (GameFlag_GetFar(FLAG_PLAY_PSYNERGY_CAST)) {
    if (GameFlag_GetFar(FLAG_PSYNERGY_TARGET))
      FieldPsynergy_Dispatch();
    else
      FieldPsynergy_Run();
  }
  FieldPsynergy_StopCasterUpdate();
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_GetFar(FLAG_PLAY_PSYNERGY_CAST))
    FieldPsynergy_FinishMindRead();
  GameFlag_ClearFar(FLAG_PLAY_PSYNERGY_CAST);
  GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);
  work->psynergyActing = 0;
  FieldPsynergy_End();
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
    Actor_Respawn();
  return 0;
}

/// Returns whether the scene has a Psynergy trigger, in any phase, for
/// Psynergy of group and what the leader faces. Called by the Psynergy menu
/// (PsynergyMenu_ClassifySelectedPsynergy) through
/// Scene_HasFieldPsynergyTriggerFar.
s32 Scene_HasFieldPsynergyTrigger(s32 group) {
  s32 target;
  s32 eventAddress =
      (s32)Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER_ANY, (u16)group, &target);
  return (u32)((-eventAddress) | eventAddress) >> 31;
}

/// Returns whether the scene has an event for item and what the leader faces.
/// Called by the item menu (menu/item.c) through Scene_HasPsynergyTriggerFar:
/// such an item closes the menu and is used on the field.
u32 Scene_HasPsynergyTrigger(u16 item) {
  s32 eventAddress;

  eventAddress = Field_FindItemTrigger(item);
  return (u32)((0 - eventAddress) | eventAddress) >> 31;
}
