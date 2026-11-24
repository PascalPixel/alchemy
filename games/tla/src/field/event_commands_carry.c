/// @file
/// Carry cast where there is nothing to carry, and the hands Carry spawns.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "sounds.h"
#include "macros.h"

#include "types.h"
#include "system/system.h"
#include "field/objdisp.h"
#include "field/actor_view.h"
#include "field/actor.h"
#include "field/field_psynergy_state.h"
#include "system/ram_buffer.h"
#include "battle/battle.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "fixed_math.h"

/// Carry cast with no target: two hands appear 32 pixels to either side of
/// the spot, close 24 pixels in and meet with a crash that scatters
/// ITEM_BREAK_SHARD_MAX sparkles, then vanish.
void FieldPsynergy_Carry(void) {
  struct FieldEffectState *state;
  struct Actor *actor;
  struct Actor *pair[2];
  struct Vector3 pos;
  struct Actor **hand;
  s32 n;
  FIXED distance;

  state = Ram_HeapSlots->fieldEffectState;
  FieldPsynergy_FadeToCastColor();
  SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
  pair[0] = FieldEffect_SpawnCarryHand(pos.x + toFIXED(32), pos.y, pos.z,
                                       FACING_WEST);
  pair[1] = FieldEffect_SpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z,
                                       FACING_EAST);

  wait(15);
  hand = pair;
  for (n = 0; n < 2; n++) {
    actor = *hand++;
    if (actor != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(actor, toFIXED(24),
                                                actor->facing);
  }

  Actor_WaitForMoveFar(pair[0]);
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {
    SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
    actor = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (actor != 0) {
      Actor_StartScriptFar(actor, gFieldEffect_FragmentScript);
      actor->speedLimit = Random16() + toFIXED(2);
      actor->acceleration = toFIXED(2);
      actor->motionFlags = 0;
      distance = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(actor, distance, Random16());
    }
  }

  Actor_ReleaseFar(pair[0]);
  Actor_ReleaseFar(pair[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Spawns a hand (SPRITE_MOVE_HAND) with the item break sound and grows it,
/// spinning 1/8 turn a frame, from a quarter to full size over 24 frames; angle
/// is the way it faces once grown.
struct Actor *FieldEffect_SpawnCarryHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *actor;
  FIXED scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  actor = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (actor != NULL) {
    actor->scaleY = FIXED_ONE / 4;
    actor->scaleX = FIXED_ONE / 4;
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(3);
    actor->steeringFlags = 0;
    Actor_SelectAnimationFar(actor, MOVE_HAND_ANIMATION_OPEN);
    scale = actor->scaleX;
    if (scale < FIXED_ONE) {
      do {
        scale += FIXED_ONE / 32;
        actor->scaleY = scale;
        actor->scaleX = scale;
        actor->facing += DEGtoANG(45);
        wait(1);
        scale = actor->scaleX;
      } while (scale < FIXED_ONE);
    }
    actor->facing = angle;
  }
  return actor;
}
