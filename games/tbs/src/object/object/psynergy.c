/// @file
/// Using Psynergy on the field.
// Part of object.c.

#include "../object_internal.h"

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its
/// user (bits 10 to 13; 15 for character 0) and ACT_PACKED to use it without
/// paying PP. Checks it can be used here (Reveal not on the world map, Retreat
/// where it is not blocked, after asking), pays its PP, then plays its effect
/// between the scene's Psynergy events for what the leader faces:
/// PSYNERGY_PHASE_BEFORE_CAST before and PSYNERGY_PHASE_AFTER_CAST after. A
/// PSYNERGY_PHASE_EFFECT_ACTS event is not run here; it only makes
/// the effect aim at the actor ahead, and the effects that act on it (Force,
/// Frost, Reveal) find it themselves. An after event whose trigger has flag
/// 0x400 plays the effect itself. Called by Field_RunScene for the Psynergy
/// request (work->psynergy); sets work->resolvingAction while the effect plays.
/// On the world map, placed actors far from the view are destroyed for the
/// effect, and all placed actors are placed again afterwards. Returns 0
/// whatever happens; Field_RunScene ignores it.
s32 Field_UsePsynergy(u32 code) {
  s32 abilityId = ABILITY_ID(code);
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  const struct SceneEvent *before;
  const struct SceneEvent *after;
  const struct SceneEvent *during;
  s32 userId;
  s32 target;
  s32 retreat;
  s32 abilityGroup;
  s32 ppCost;
  s32 declined;

  abilityGroup = BattleActGet(abilityId)->type0c;
  userId = ACT_UNIT(code);
  // The leader is looked up and the result is not used.
  ObjectTable_Get(gGameState.selectedActor);
  retreat = 0;
  Leader_StopMoving();
  GameFlag_ClearBitFar(FLAG_ITEM_USE);
  if (userId == UNIT_NONE)
    userId = 0;

  // Where Psynergy has no effect.
  if (GameFlag_TestFar(FLAG_IN_BATTLE_ARENA)) {
    PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
    return 0;
  }
  if (work->mode == MODE_WORLD_MAP && abilityId == ABILITY_REVEAL) {
    PSYNERGY_MESSAGE(userId, ABILITY_REVEAL, MsgNothingHappened);
    return 0;
  }

  // Retreat: ask, then go back to where the party entered this place.
  if (abilityId == ABILITY_RETREAT) {
    if (GameFlag_TestFar(FLAG_RETREAT_BLOCKED)) {
      PSYNERGY_MESSAGE(userId, ABILITY_RETREAT, MsgDoesNotWorkHere);
      return 0;
    }
    UiWork_PushValueSlotFar(ABILITY_RETREAT, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgUseAbilityConfirm, 13);
    declined = Menu_AskYesNo(1); // 0 is yes
    UiWork_FinalizePendingCoreFar();
    if (declined != 0)
      return 0;
    {
      struct GameState *state = &gGameState;
      s32 scene, returnEntrance;
      scene = (u16)state->retreatScene;
      state->scene = scene;
      returnEntrance = (u16)state->retreatEntrance;
      state->entrance = returnEntrance;
    }
    work->exit = EXIT_RETREAT;
    retreat = 1;
  }
  // A use that pays no PP only runs the scene's ending events.
  if (code & ACT_PACKED)
    return Field_RunPsynergyEvents(code);

  // A party member pays the PP.
  if (userId <= ALLY_MAX) {
    ppCost = BattleActGet(abilityId)->ppCost;
    if (Unit_GetFar(userId)->pp < ppCost) {
      PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
      // Retreat was already accepted: take its exit back (the scene and
      // entrance it chose stay in the game state).
      if (retreat)
        work->exit = 0;
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }

  // Find the scene's events, and choose how the effect plays. The before
  // event runs before the two flags are read, so its script can change them.
  before = FieldEffect_FindMatchingEvent(
      PSYNERGY_EV(PSYNERGY_PHASE_BEFORE_CAST), abilityGroup, &target);
  after = FieldEffect_FindMatchingEvent(PSYNERGY_EV(PSYNERGY_PHASE_AFTER_CAST),
                                        abilityGroup, &target);
  during = FieldEffect_FindMatchingEvent(
      PSYNERGY_EV(PSYNERGY_PHASE_EFFECT_ACTS), abilityGroup, &target);
  target = -1;
  GameFlag_SetBitFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_SetBitFar(FLAG_PSYNERGY_TARGET);
  if (before || after || during) {
    target =
        Field_FindNearbyPsynergyTarget(gGameState.selectedActor, abilityGroup);
    // An event with trigger flag 0x400 plays the effect itself.
    if (after && (after->trigger & 0x400)) {
      GameFlag_ClearBitFar(FLAG_PSYNERGY_EFFECT);
      GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);
    }
  } else
    GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);

  // Play the effect.
  if (work->mode == MODE_WORLD_MAP)
    ObjectTable_ClearOutOfBounds();
  FieldEffect_LoadActionEffectResources(abilityId, 0);
  work->resolvingAction = 1;
  FieldEffect_SetupObjectPair(gGameState.selectedActor, target);
  EventObject_Initialize();

  Field_RunEventAction(before, userId, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT)) {
    if (GameFlag_TestFar(FLAG_PSYNERGY_TARGET))
      FieldEffect_DispatchRequestKind();
    else
      FieldEffect_Run();
  }
  FieldEffect_StopCasterHook();
  Field_RunEventAction(after, userId, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT))
    FieldEffect_ClearChildValueOnMismatch();
  GameFlag_ClearBitFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);
  work->resolvingAction = 0;
  FieldEffect_End();
  if (work->mode == MODE_WORLD_MAP)
    ObjectTable_Respawn();
  return 0;
}

/// Returns whether the scene has a Psynergy event, in any phase, for
/// Psynergy of group and what the leader faces. Exported as
/// FieldEffect_HasMatchingEvent5Far, which no ☀️ code names.
s32 FieldEffect_HasMatchingEvent5(s32 group) {
  s32 target;
  s32 eventAddress =
      (s32)FieldEffect_FindMatchingEvent(PSYNERGY_EV_ANY, (u16)group, &target);
  return (u32)((-eventAddress) | eventAddress) >> 31;
}

/// Returns whether the scene has an event for item and what the leader faces.
/// Called by the item menu (menu/item.c) through FieldEffect_HasTriggerFar:
/// such an item closes the menu and is used on the field.
u32 FieldEffect_HasTrigger(u16 item) {
  s32 eventAddress;

  eventAddress = Field_FindItemEvent(item);
  return (u32)((0 - eventAddress) | eventAddress) >> 31;
}
