/// @file
/// Avoid: the caster spins among falling arcs and flashes, and monsters attack
/// less often until it wears off.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// End of the Psynergy: switches the caster's sprite between SPRITE_FLAG_SHADOW
/// | SPRITE_FLAG_OUTLINED and SPRITE_FLAG_SHADOW on a 5-frame cycle, so it
/// flashes as a dark outline.
void FieldEffect_BlinkAvoidCaster(void) {
  struct Actor *caster;
  struct Sprite *sprite;
  struct SpriteAnimation *entry;
  s32 phase;

  caster = Actor_Lookup(gMainRoutineState.leaderActorId);
  sprite = caster->sprite;
  entry = sprite->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_SHADOW | SPRITE_FLAG_OUTLINED;
  } else if (phase == 2) {
    entry->colorMode = SPRITE_COLOR_NORMAL; // restore its colours
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_SHADOW;
  }
}

/// The caster during the Psynergy: sheds a pair of falling arcs every 5 frames
/// (7 on the World Map), and spins in phase 1, 3/64 turn a frame.
void FieldEffect_UpdateAvoidCaster(struct Actor *caster) {
  s16 phase;
  s32 elapsedFrames;

  phase = caster->arcPhase;
  elapsedFrames = caster->arcFrames++;

  if (gMainRoutineState.mapId == MAP_INDEX_WORLD) {
    if (elapsedFrames % 7 == 0)
      FieldEffect_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    FieldEffect_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->facing += TRIG_TURN * 3 / 64;
}

/// Says that Avoid, or the Sacred Feather, has worn off: "Avoid's effects wore
/// off." Clears gMainRoutineState.avoidDistance. Reached through
/// FieldPsynergy_RunExtraHandler when the Psynergy request the leader's walking
/// raises as Avoid runs out (field_sequence_walking.c) is handled.
void FieldPsynergy_ShowAvoidWoreOff(void) {
  gMainRoutineState.avoidDistance = 0;
  if (gMainRoutineState.avoidByItem == 0) {
    MessageArgument_PushFar(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
    FieldMessage_ShowFar(MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
    return;
  }
  MessageArgument_PushFar(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_ShowFar(MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
}

#define AVOID_FLASHES 20      ///< white flashes, 5 frames each
#define AVOID_BLINK_FRAMES 70 ///< the outline flashes
#define AVOID_NOTE_FRAME 15   ///< SE_NOTE_MID, into the outline's flashing

/// The caster spins among falling arcs and flashes white, then flashes as an
/// outline, and encounters are kept away until Avoid wears off.
void FieldPsynergy_Avoid(void) {
  struct FieldEffectState *effectState;
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *entry;
  u32 angle;
  s32 slot;
  s32 count;

  effectState = gFieldEffectState;
  actor = effectState->caster;
  sprite = actor->sprite;
  entry = sprite->entries[0];
  angle = actor->facing;
  slot = VramCache_FindFreeEntry();
  // The arcs' tiles (256 bytes) stay loaded until the Psynergy's end; the
  // distance is 2,400 pixels in 16.16.
  effectState->arcTileSlot = slot;
  VramCache_Load((s16)slot, 256, gFieldEffect_ArcSparkTiles);
  gMainRoutineState.avoidDistance = toFIXED(2400);
  gMainRoutineState.avoidByItem = GameFlag_GetFar(FLAG_ITEM_USE);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  actor->update = FieldEffect_UpdateAvoidCaster;
  actor->arcPhase = 0;
  actor->arcFrames = 0;
  Sound_PlayFar(SE_SPIN);
  wait(15);
  actor->arcPhase = 1;
  wait(10);
  // Flash white for 2 frames in every 5.
  for (count = 0; count < AVOID_FLASHES; count++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    wait(2);
    sprite->dirty = 1;
    entry->colorMode = SPRITE_COLOR_NORMAL;
    sprite->flags = SPRITE_FLAG_SHADOW;
    wait(3);
  }
  actor->update = NULL;
  actor->facing = angle;
  Task_Add(FieldEffect_BlinkAvoidCaster, TASK_PASS_FRAME);
  wait(AVOID_NOTE_FRAME);
  Sound_PlayFar(SE_NOTE_MID);
  wait(AVOID_BLINK_FRAMES - AVOID_NOTE_FRAME);
  Task_Remove(FieldEffect_BlinkAvoidCaster);
  // Back to the caster's own look: outlined while Cloak is on, else with its
  // shadow.
  if (gMainRoutineState.cloaked != 0)
    Actor_SetSpriteFlagsFar(actor, SPRITE_FLAG_OUTLINED);
  else
    Actor_SetSpriteFlagsFar(actor, SPRITE_FLAG_SHADOW);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  VramCache_FreeEntry(effectState->arcTileSlot);
  // "Monsters won't attack so often now."
  FieldMessage_ShowFar(MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}

#undef AVOID_FLASHES
#undef AVOID_BLINK_FRAMES
#undef AVOID_NOTE_FRAME
