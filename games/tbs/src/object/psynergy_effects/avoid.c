/// @file
/// Avoid.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// End of the effect: switches the caster's display flags between 3 and 1 on
/// a 5-frame cycle, so its outline (bit 1) blinks beside its shadow (bit 0).
/// FieldFx_RunAvoid schedules it (order 0xc80) for its last 70 frames and
/// removes it again.
void FieldFx_BlinkAvoidCaster(void) {
  struct ArcEffectObject *caster;
  struct AnimationObject *rec;
  struct AnimationEntry *entry;
  s32 phase;

  caster = ObjectTable_Get(gGameState.selectedActor);
  rec = caster->records;
  entry = rec->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    rec->dirty = 1;
    rec->flags = 3;
  } else if (phase == 2) {
    entry->param = 0; // restore its colours
    rec->dirty = 1;
    rec->flags = 1;
  }
}

/// The caster during the effect: sheds a pair of falling arcs every 5 frames
/// (7 on the world map), and spins in phase 1, 3/64 turn a frame.
/// FieldFx_RunAvoid makes it the caster's callback; it counts the caster's
/// step and turns its angle. The arcs draw with the tiles FieldFx_RunAvoid
/// loads into scene->arcTileSlot.
void BattleFx_UpdatePairedArcSpawner(union FieldObject *fieldObject) {
  struct ArcEffectObject *caster = (struct ArcEffectObject *)fieldObject;
  s16 phase;
  s32 elapsedFrames;

  phase = caster->phase;
  elapsedFrames = caster->step++;

  // SceneId_Clear is the world map's map (Field_RunScene).
  if (gGameState.map == (s32)&SceneId_Clear) {
    if (elapsedFrames % 7 == 0)
      BattleFx_SpawnDescendingArcParticles((struct FieldActor *)caster);
  } else if (elapsedFrames % 5 == 0) {
    BattleFx_SpawnDescendingArcParticles((struct FieldActor *)caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

/// Avoid (or the Sacred Feather) has worn off, and the game says so.
/// "Avoid's effects wore off." (or "Sacred Feather's ...").
///
/// Clears gGameState.avoidDistance. Reached through FieldEvent_RunTypeHandler
/// when the Psynergy request the leader's steps raise as Avoid runs out
/// (object/step.c) is handled.
void FieldEvent_ShowStatusMessage(void) {
  gGameState.avoidDistance = 0;
  if (gGameState.avoidByItem == 0) {
    UiWork_PushValueSlotFar(ABILITY_AVOID, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
    return;
  }
  UiWork_PushValueSlotFar(ITEM_SACRED_FEATHER, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

/// The caster spins and flashes, then blinks, and encounters are kept away
/// until Avoid wears off.
///
/// Abilities: Avoid (field kind 16), and the Sacred Feather.
///
/// The caster spins shedding falling arcs and flashes 20 times, then blinks for
/// 55 frames; encounters are kept away (avoidDistance 0x9600000) until it wears
/// off, and the game says "Monsters won't attack so often now.". FLAG_ITEM_USE
/// marks it as cast from the Sacred Feather.
///
/// Called by BattleFx_Run and BattleFx_DispatchRequestKind. Sets
/// gGameState.avoidDistance and avoidByItem; the leader's steps count the
/// distance down by their speed (object/step.c), 2,400 pixels in all.
void FieldFx_RunAvoid(void) {
  struct BattleFxScene *scene;
  struct ArcEffectObject *object;
  struct AnimationObject *group;
  struct AnimationEntry *entry;
  u32 angle;
  s32 slot;
  s32 count;

  scene = gEffectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  slot = Resource_FindFreeEntry();
  {
    s16 *tileSlot = &scene->arcTileSlot;
    s32 zero = 0;

    // The arcs' tiles (256 bytes) stay loaded until the effect's end; the
    // distance is 2,400 pixels in 16.16.
    *tileSlot = slot;
    VramCache_Load((s16)slot, 0x100, gBattleFx_ArcSparkTiles);
    gGameState.avoidDistance = 0x09600000;
    gGameState.avoidByItem = GameFlag_TestFar(FLAG_ITEM_USE);
    Animation_ApplyChildValuesFar((struct DispatchObject *)object, zero);
    object->callback = BattleFx_UpdatePairedArcSpawner;
    object->phase = zero;
    object->step = zero;
  }
  AudioCommand_PlayFar(SE_140);
  WaitFrames(15);
  object->phase = 1;
  WaitFrames(10);
  // Flash white for 2 frames in every 5.
  for (count = 0; count < 20; count++) {
    entry->param = SPRITE_COLOR_WHITE;
    group->dirty = 1;
    WaitFrames(2);
    group->dirty = 1;
    entry->param = 0;
    group->flags = 1;
    WaitFrames(3);
  }
  object->callback = NULL;
  object->angle = angle;
  Scheduler_AddOrUpdateCallback(FieldFx_BlinkAvoidCaster, 0xc80);
  WaitFrames(15);
  AudioCommand_PlayFar(SE_174);
  WaitFrames(55);
  Scheduler_RemoveCallback(FieldFx_BlinkAvoidCaster);
  // Back to the caster's own look: flags 2 while Cloak is on, else 1.
  if (gGameState.cloaked != 0)
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 2);
  else
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
  Animation_ApplyChildValuesFar((struct DispatchObject *)object, 0);
  Resource_ResetEntry(scene->arcTileSlot);
  UiText_ShowPositionedMessageAndWaitFar(MsgMonstersAttackLess, 1);
}
