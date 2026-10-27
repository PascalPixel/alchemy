/// @file
/// Avoid.
// Part of field_services.c.

#include "../field_services_internal.h"

/// End of the effect: switches the caster's display flags between 3 and 1 on
/// a 5-frame cycle, so its outline (bit 1) blinks beside its shadow (bit 0).
/// FieldEffect_RunAvoid adds it as a task (priority TASK_PASS_FRAME)
/// for its last 70 frames and removes it again.
void FieldEffect_BlinkAvoidCaster(void) {
  struct Actor *caster;
  struct Sprite *rec;
  struct SpriteAnim *entry;
  s32 phase;

  caster = Actor_GetByActorId(gSaveBlock.leaderActorId);
  rec = caster->sprite;
  entry = rec->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    rec->dirty = 1;
    rec->flags = SPRITE_FLAG_SHADOW | SPRITE_FLAG_OUTLINED;
  } else if (phase == 2) {
    entry->colorMode = SPRITE_COLOR_NORMAL; // restore its colours
    rec->dirty = 1;
    rec->flags = SPRITE_FLAG_SHADOW;
  }
}

/// The caster during the effect: sheds a pair of falling arcs every 5 frames
/// (7 on the world map), and spins in phase 1, 3/64 turn a frame.
/// FieldEffect_RunAvoid makes it the caster's callback; it counts the caster's
/// step and turns its angle. The arcs draw with the tiles FieldEffect_RunAvoid
/// loads into effectWork->arcTileSlot.
void FieldEffect_UpdatePairedArcSpawner(union ActorOrEffect *fieldObject) {
  struct Actor *caster = (struct Actor *)fieldObject;
  s16 phase;
  s32 elapsedFrames;

  phase = caster->arcPhase;
  elapsedFrames = caster->arcFrames++;

  // SceneId_MainMenu is the world map's map (SceneManager_Run).
  if (gSaveBlock.mapId == (s32)&SceneId_MainMenu) {
    if (elapsedFrames % 7 == 0)
      FieldEffect_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    FieldEffect_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->facing += 0xc00;
}

/// Avoid (or the Sacred Feather) has worn off, and the game says so.
/// "Avoid's effects wore off." (or "Sacred Feather's ...").
///
/// Clears gSaveBlock.avoidDistance. Reached through FieldScript_RunTypeHandler
/// when the Psynergy request the leader's steps raise as Avoid runs out
/// (overworld/step.c) is handled.
void FieldScript_ShowStatusMessage(void) {
  gSaveBlock.avoidDistance = 0;
  if (gSaveBlock.avoidByItem == 0) {
    MessageArgument_PushFar(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar(MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
    return;
  }
  MessageArgument_PushFar(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_ShowFar(MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
}

/// The caster spins and flashes, then blinks, and encounters are kept away
/// until Avoid wears off.
///
/// Abilities: Avoid (FIELD_PSYNERGY_AVOID).
///
/// Using the Sacred Feather plays it too, since the item's field action is
/// Avoid.
///
/// The caster spins shedding falling arcs and flashes 20 times, then blinks for
/// 55 frames; encounters are kept away (avoidDistance 0x9600000) until it wears
/// off, and the game says "Monsters won't attack so often now.". FLAG_ITEM_USE
/// marks it as cast from the Sacred Feather.
///
/// Called by FieldEffect_Run and FieldEffect_DispatchRequestKind. Sets
/// gSaveBlock.avoidDistance and avoidByItem; the leader's steps count the
/// distance down by their speed (overworld/step.c), 2,400 pixels in all.
void FieldEffect_RunAvoid(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *object;
  struct Sprite *group;
  struct SpriteAnim *entry;
  u32 angle;
  s32 slot;
  s32 count;

  effectWork = gFieldEffectWork;
  object = effectWork->mainObject;
  group = object->sprite;
  entry = group->entries[0];
  angle = object->facing;
  slot = VramCache_FindFreeEntry();
  {
    s16 *tileSlot = &effectWork->arcTileSlot;
    s32 zero = 0;

    // The arcs' tiles (256 bytes) stay loaded until the effect's end; the
    // distance is 2,400 pixels in 16.16.
    *tileSlot = slot;
    VramCache_Load((s16)slot, 0x100, gFieldEffect_ArcSparkTiles);
    gSaveBlock.avoidDistance = 0x09600000;
    gSaveBlock.avoidByItem = GameFlag_GetFar(FLAG_ITEM_USE);
    Anim_ApplyChildValuesFar(object, zero);
    object->update = FieldEffect_UpdatePairedArcSpawner;
    object->arcPhase = zero;
    object->arcFrames = zero;
  }
  Sound_PlaySongFar(SE_SPIN);
  WaitFrames(15);
  object->arcPhase = 1;
  WaitFrames(10);
  // Flash white for 2 frames in every 5.
  for (count = 0; count < 20; count++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    group->dirty = 1;
    WaitFrames(2);
    group->dirty = 1;
    entry->colorMode = SPRITE_COLOR_NORMAL;
    group->flags = SPRITE_FLAG_SHADOW;
    WaitFrames(3);
  }
  object->update = NULL;
  object->facing = angle;
  Task_Add(FieldEffect_BlinkAvoidCaster, TASK_PASS_FRAME);
  WaitFrames(15);
  Sound_PlaySongFar(SE_NOTE_MID);
  WaitFrames(55);
  Task_Remove(FieldEffect_BlinkAvoidCaster);
  // Back to the caster's own look: outlined while Cloak is on, else with its
  // shadow.
  if (gSaveBlock.cloaked != 0)
    Object_SetSpriteFlagsFar(object, SPRITE_FLAG_OUTLINED);
  else
    Object_SetSpriteFlagsFar(object, SPRITE_FLAG_SHADOW);
  Anim_ApplyChildValuesFar(object, SPRITE_COLOR_NORMAL);
  VramCache_FreeEntry(effectWork->arcTileSlot);
  FieldMessage_ShowFar(MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}
