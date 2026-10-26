/// @file
/// Using Psynergy on the field.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its
/// user (bits 10 to 13; 15 for character 0) and ACT_PACKED to use it without
/// paying PP. Checks it can be used here (Reveal not on the world map, Retreat
/// where it is not blocked, after asking), pays its PP, then plays its effect
/// between the map's Psynergy events for what the leader faces:
/// PSYNERGY_PHASE_BEFORE_CAST before and PSYNERGY_PHASE_AFTER_CAST after. A
/// PSYNERGY_PHASE_EFFECT_ACTS event is not run here; it only makes
/// the effect aim at the object event ahead, and the effects that act on it
/// (Force, Frost, Reveal) find it themselves. An after event whose trigger has
/// MAP_EVENT_TRIGGER_OWN_EFFECT plays the effect itself. Called by
/// SceneManager_Run for the Psynergy request (work->psynergy); sets
/// work->psynergyActing while the effect plays. On the world map, placed object
/// events far from the view are destroyed for the effect, and all placed object
/// events are placed again afterwards. Returns 0 whatever happens;
/// SceneManager_Run ignores it.
s32 Field_UsePsynergy(u32 code) {
  s32 abilityId = ABILITY_ID(code);
  struct FieldWork *work = gFieldWork;
  const struct MapEvent *before;
  const struct MapEvent *after;
  const struct MapEvent *during;
  s32 userId;
  s32 target;
  s32 retreat;
  s32 abilityGroup;
  s32 ppCost;
  s32 declined;

  abilityGroup = Ability_Get(abilityId)->fieldPsynergy;
  userId = ACT_UNIT(code);
  // The leader is looked up and the result is not used.
  ObjectEvent_GetByLocalId(gSaveBlock.leaderLocalId);
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
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP && abilityId == ABILITY_REVEAL) {
    PSYNERGY_MESSAGE(userId, ABILITY_REVEAL, MsgNothingHappened);
    return 0;
  }

  // Retreat: ask, then go back to where the party entered this map section.
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
      s32 sceneId, returnWarpDest;
      sceneId = (u16)state->retreatMap;
      state->sceneId = sceneId;
      returnWarpDest = (u16)state->retreatWarpDest;
      state->warpDest = returnWarpDest;
    }
    work->warp = WARP_RETREAT;
    retreat = 1;
  }
  // A use that pays no PP only runs the map's ending events.
  if (code & ACT_PACKED)
    return Field_RunPsynergyEvents(code);

  // A party member pays the PP.
  if (userId <= ALLY_MAX) {
    ppCost = Ability_Get(abilityId)->ppCost;
    if (Unit_GetFar(userId)->pp < ppCost) {
      PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
      // Retreat was already accepted: take its warp back (the map and
      // warp destination it chose stay in the save block).
      if (retreat)
        work->warp = 0;
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }

  // Find the map's events, and choose how the effect plays. The before
  // event runs before the two flags are read, so its script can change them.
  before = Scene_FindPsynergyEvent(PSYNERGY_EVENT(PSYNERGY_PHASE_BEFORE_CAST),
                                   abilityGroup, &target);
  after = Scene_FindPsynergyEvent(PSYNERGY_EVENT(PSYNERGY_PHASE_AFTER_CAST),
                                  abilityGroup, &target);
  during = Scene_FindPsynergyEvent(PSYNERGY_EVENT(PSYNERGY_PHASE_EFFECT_ACTS),
                                   abilityGroup, &target);
  target = -1;
  GameFlag_SetFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_SetFar(FLAG_PSYNERGY_TARGET);
  if (before || after || during) {
    target =
        Field_FindNearbyPsynergyTarget(gSaveBlock.leaderLocalId, abilityGroup);
    // The after event plays the effect itself.
    if (after && (after->trigger & MAP_EVENT_TRIGGER_OWN_EFFECT)) {
      GameFlag_ClearFar(FLAG_PSYNERGY_EFFECT);
      GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);
    }
  } else
    GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);

  // Play the effect.
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP)
    ObjectEvent_ClearOutOfBounds();
  FieldEffect_LoadActionEffectResources(abilityId, 0);
  work->psynergyActing = 1;
  FieldEffect_SetupObjectPair(gSaveBlock.leaderLocalId, target);
  FieldEffect_StartCast();

  Field_RunEventAction(before, userId, target);
  if (GameFlag_GetFar(FLAG_PSYNERGY_EFFECT)) {
    if (GameFlag_GetFar(FLAG_PSYNERGY_TARGET))
      FieldEffect_DispatchRequestKind();
    else
      FieldEffect_Run();
  }
  FieldEffect_StopCasterHook();
  Field_RunEventAction(after, userId, target);
  if (GameFlag_GetFar(FLAG_PSYNERGY_EFFECT))
    FieldEffect_FinishMindRead();
  GameFlag_ClearFar(FLAG_PSYNERGY_EFFECT);
  GameFlag_ClearFar(FLAG_PSYNERGY_TARGET);
  work->psynergyActing = 0;
  FieldEffect_End();
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP)
    ObjectEvent_Respawn();
  return 0;
}

/// Returns whether the map has a Psynergy event, in any phase, for
/// Psynergy of group and what the leader faces. Called by the Psynergy menu
/// (PsynergyMenu_ClassifySelectedPsynergy) through
/// Scene_HasFieldPsynergyEventFar.
s32 Scene_HasFieldPsynergyEvent(s32 group) {
  s32 target;
  s32 eventAddress =
      (s32)Scene_FindPsynergyEvent(PSYNERGY_EV_ANY, (u16)group, &target);
  return (u32)((-eventAddress) | eventAddress) >> 31;
}

/// Returns whether the map has an event for item and what the leader faces.
/// Called by the item menu (menu/item.c) through Scene_HasPsynergyEventFar:
/// such an item closes the menu and is used on the field.
u32 Scene_HasPsynergyEvent(u16 item) {
  s32 eventAddress;

  eventAddress = Field_FindItemEvent(item);
  return (u32)((0 - eventAddress) | eventAddress) >> 31;
}
