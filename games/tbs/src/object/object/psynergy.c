// Part of object.c, the field object, compiled only through it.
// Using Psynergy on the field.

#include "../object_internal.h"

// Uses a Psynergy on the field. code holds the Psynergy (bits 0 to 9), its
// user (bits 10 to 13; 15 for character 0) and ACT_PACKED to use it without
// paying PP. Checks it can be used here (Reveal not on the world map, Retreat
// where it is not blocked, after asking), pays its PP, then plays its effect
// between the scene's Psynergy events for what the leader faces: phase 1
// before, phase 0 after and phase 5 while it plays. An event whose trigger
// has flag 0x400 plays the effect itself.
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
  ObjectTable_Get(gGameState.selectedActor);
  retreat = 0;
  Leader_StopMoving();
  GameFlag_ClearBitFar(FLAG_ITEM_USE);
  if (userId == UNIT_NONE)
    userId = 0;

  // Where Psynergy has no effect.
  if (GameFlag_TestFar(FLAG_PSYNERGY_NONE)) {
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
  if (code & ACT_PACKED)
    return Field_RunPsynergyEvents(code);

  // A party member pays the PP.
  if (userId <= ALLY_MAX) {
    ppCost = BattleActGet(abilityId)->ppCost;
    if (UnitGetFar(userId)->pp < ppCost) {
      PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
      if (retreat)
        work->exit = 0;
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }

  // Find the scene's events, and choose how the effect plays.
  before = BattleFx_FindMatchingEvent(PSYNERGY_EV(1), abilityGroup, &target);
  after = BattleFx_FindMatchingEvent(PSYNERGY_EV(0), abilityGroup, &target);
  during = BattleFx_FindMatchingEvent(PSYNERGY_EV(5), abilityGroup, &target);
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
    BattleEffect_ClearOutOfBoundsObjects();
  BattleFx_LoadActionEffectResources(abilityId, 0);
  work->resolvingAction = 1;
  BattleFx_SetupObjectPair(gGameState.selectedActor, target);
  EventObject_Initialize();

  Field_RunEventAction(before, userId, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT)) {
    if (GameFlag_TestFar(FLAG_PSYNERGY_TARGET))
      BattleFx_DispatchRequestKind();
    else
      BattleFx_Run();
  }
  EffectRuntime_StopCurrentObject();
  Field_RunEventAction(after, userId, target);
  if (GameFlag_TestFar(FLAG_PSYNERGY_EFFECT))
    BattleFx_ClearChildValueOnMismatch();
  GameFlag_ClearBitFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_ClearBitFar(FLAG_PSYNERGY_TARGET);
  work->resolvingAction = 0;
  BattleEffect_CleanupSceneObjects();
  if (work->mode == MODE_WORLD_MAP)
    BattleEffect_ClearAllObjects();
  return 0;
}

// Returns whether the scene has a Psynergy event, in any phase, for a
// Psynergy of group and what the leader faces.
s32 BattleFx_HasMatchingEvent5(s32 group) {
  s32 target;
  s32 eventAddress =
      (s32)BattleFx_FindMatchingEvent(PSYNERGY_EV_ANY, (u16)group, &target);
  return (u32)((-eventAddress) | eventAddress) >> 31;
}

// Returns whether the scene has an event for item and what the leader faces.
u32 BattleFx_HasTrigger(u16 item) {
  s32 eventAddress;

  eventAddress = Field_FindItemEvent(item);
  return (u32)((0 - eventAddress) | eventAddress) >> 31;
}
