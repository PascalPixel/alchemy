/// @file
/// The field Psynergy from Mind Read to Cloak, as The Broken Seal's
/// event_commands.c has them in its parts mind_read.c to cloak.c: Mind Read's
/// wave, Move, Reveal, Growth, Lift, Force, Frost, Ply and the start of Cloak.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "sounds.h"

#include "graphics/palette_buffers.h"
#include "system/io_reg.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/field_sequence.h"
#include "field/objdisp.h"
#include "system/input.h"
#include "edition.h"
#include "fixed_math.h"
#include "battle/object_effect_scripts.h"
#include "field/field_event.h"
#include "field/field_psynergy_work.h"
#include "debug/debug.h"
#include "graphics/display_transition.h"
#include "field/field_sequence_event.h"
#include "system/scene.h"
#include "macros.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "window/item_icon.h"

/// A line's offset in Mind Read's wave at a phase: a sine 4 pixels either way,
/// a turn every 160 phase steps.
#define MIND_READ_WAVE_OFFSET(phase) (Trig_Sin(((phase) << 16) / 160) >> 14)
/// The wave's cycle runs to this count and starts again, 61 frames; at counts
/// 0, 8 and 16 a particle flies from the target to the caster.
#define MIND_READ_CYCLE_LAST 60
#define MIND_READ_PARTICLE_GAP 8

/// Task of the wave effect: once its delay has run out, writes the next page
/// of line offsets, steps the colour fade and the hue every other frame,
/// keeps the caster's colours lit, and three times a cycle sends a particle
/// (SPRITE_CRESCENT) from the target to the caster, with SE_PSYNERGY_START.
void FieldPsynergy_UpdateMindReadWave(void) {
  struct MindReadWork *work = Ram_HeapSlots->mindRead;
  struct Vector3 from;
  struct Vector3 to;
  struct ActorPlacement *descriptor;
  s32 spriteId;
  struct AnimationMetadata *metadata;
  struct PlacedSprite *sprite;
  struct Actor *source;
  struct Actor *target;
  struct Actor *actor;
  u16 *line;
  s32 i;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  line = work->lines[work->page ^ 1];
  for (i = 0; i < DISPLAY_HEIGHT; i++)
    *line++ = MIND_READ_WAVE_OFFSET(work->phase + i * 8);
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    FieldEffect_AdvanceHueCycle();
    PaletteFade_SetTarget(
        RGB(work->red, work->green, work->blue) | PALETTE_TINT_CUSTOM, 1);
    PaletteFade_StartInterpolation(1);
  }
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == MIND_READ_PARTICLE_GAP ||
      work->counter == 2 * MIND_READ_PARTICLE_GAP) {
    source = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (source != NULL && target != NULL) {
      descriptor =
          (struct ActorPlacement *)Scene_FindActorPlacement(work->sourceId);
      spriteId = descriptor->sprite;
      from.x = source->x.fixed;
      metadata = Resource_GetMetadataRecordFarFar(spriteId);
      from.y = source->y.fixed + INTtoFIXED((s8)metadata->boxX) - toFIXED(2);
      from.z = source->z.fixed;
      sprite = target->sprite;
      descriptor =
          (struct ActorPlacement *)Scene_FindActorPlacement(work->targetId);
      spriteId = descriptor->sprite;
      to.x = target->x.fixed;
      metadata = Resource_GetMetadataRecordFarFar(spriteId);
      {
        s32 box = (s8)metadata->boxX;
        FIXED y = target->y.fixed;

        y += INTtoFIXED(box);
        to.y = y - toFIXED(2);
      }
      to.z = target->z.fixed;
      if (sprite->rotation == DEGtoANG(180)) {
        to.y -= toFIXED(24);
      } else if (sprite->rotation == DEGtoANG(90)) {
        to.x += toFIXED(6);
        to.y -= toFIXED(12);
      } else if (sprite->rotation == TRIG_TURN * 3 / 4) {
        to.x -= toFIXED(6);
        to.y -= toFIXED(12);
      }
      actor = Actor_CreateFar(SPRITE_CRESCENT, to.x, to.y, to.z);
      if (actor != NULL) {
        sprite = actor->sprite;
        actor->motionFlags = 0;
        actor->speedLimit = toFIXED(16) / 25; // 0.64 pixels a frame
        actor->acceleration = toFIXED(16) / 25;
        actor->facing = Trig_Atan(from.z - to.z, from.x - to.x);
        actor->update = (ObjectUpdate)FieldEffect_ReleaseOnArrival;
        sprite->flags = 0;
        sprite->priority = 1;
        Actor_SetMoveTargetFar(actor, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter != MIND_READ_QUIET) {
    if (work->counter == 0)
      Sound_PlayFar(SE_PSYNERGY_START);
    if (++work->counter > MIND_READ_CYCLE_LAST)
      work->counter = 0;
  }
}

/// Applies BG0 line scrolling with HBlank DMA.
void FieldEffect_ArmBg0HBlankDma(void) {
  struct MindReadWork *work = Ram_HeapSlots->mindRead;
  if (!work->delay) {
    u16 *line = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;
    dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma[5] &= ~DMA_ENABLE;
    (void)dma[5];
    // One halfword at each HBlank (repeating, destination reloaded).
    Dma_Set(line, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

/// One colour channel's level (0 to 31) at a hue in degrees: rising from 0
/// to 60, full from 60 to 180, falling from 180 to 240, else 0.
s32 FieldEffect_HueChannelRamp(s16 hue) {
  s32 level = 0;
  s16 angle = hue;

  angle = (angle + HUE_DEGREES) % HUE_DEGREES;
  if (angle < HUE_DEGREES / 6) {
    level = angle * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
  } else if (angle < HUE_DEGREES / 2) {
    level = RGB_LEVEL_MAX;
  } else if (angle < HUE_DEGREES * 2 / 3) {
    level = (HUE_DEGREES * 2 / 3 - angle) * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
  }
  return level;
}

/// Advances the hue by four degrees and calculates its colour.
void FieldEffect_AdvanceHueCycle(void) {
  struct MindReadWork *work = Ram_HeapSlots->mindRead;
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  r = FieldEffect_HueChannelRamp(work->hue);
  g = FieldEffect_HueChannelRamp(work->hue + HUE_DEGREES / 3);
  b = FieldEffect_HueChannelRamp(work->hue + HUE_DEGREES * 2 / 3);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & RGB_LEVEL_MAX;
  work->green = (g / 4 + 4) & RGB_LEVEL_MAX;
  work->blue = (b / 4 + 4) & RGB_LEVEL_MAX;
  limit = toFIXED(HUE_DEGREES - 1);
  if ((u32)INTtoFIXED(work->hue += 4) > limit)
    work->hue = 0;
}

/// Per-frame update: once the actor has arrived (no target left on any axis),
/// releases it through gBattleEffect_UntargetedObjectScript.
void FieldEffect_ReleaseOnArrival(struct Actor *target) {
  s32 tx;
  s32 ty;

  tx = target->targetX;
  if (tx == ACTOR_NO_TARGET) {
    ty = target->targetY;
    if ((ty == tx) && (target->targetZ == ty)) {
      Actor_StartScriptFar((struct Actor *)target,
                           (u32)gBattleEffect_UntargetedObjectScript);
    }
  }
}

/// Gives every part of an actor's sprite (but those at palette 15) palette - 1,
/// or for palette 0 the next palette of a cycle that changes every 2 frames.
void Actor_SetSpritePalette(void *object, s32 value) {
  s32 child = value - 1;

  if (value == 0)
    child = gAnim_ChildPaletteCycle[(gFrameCount >> 1) & 7];
  Actor_SetSpriteColorModeFar(object, child);
}

/// Returns the direction of the d-pad (TRIG_TURN a full turn): right
/// FACING_EAST, down FACING_SOUTH, left FACING_WEST, up FACING_NORTH; 0xffff
/// when none or a diagonal is held.
s32 Input_GetDpadAngle(u32 unused) {
  // Indexed by the right, left, up and down bits.
  return gFieldEffect_CyclePatternWords[DPAD_INDEX(gInput.held)];
}

/// Returns the entry of gBattleEffect_EightWayHeadingWords for the direction
/// pad's keys held.
u16 BattleEffect_GetEightWayHeadingWord(void) {
  struct InputState *input = &gInput;
  const u16 *words = gBattleEffect_EightWayHeadingWords;

  return *(u16 *)((u8 *)words + (DPAD_INDEX(input->held) << 1));
}

/// Per-frame update of Move's hand: turns it toward the actor it watches, at
/// most 1/16 turn a frame, and sheds a particle (SPRITE_SPARKLE) round it.
void FieldEffect_UpdateMoveHand(struct Actor *source) {
  struct Actor *target;
  struct Vector3 position;
  struct Actor *fragment;
  s32 turn;
  s32 radius;

  target = source->watchedActor;
  if (target != 0) {
    FIXED dx = target->x.fixed - source->x.fixed;
    FIXED dz = target->z.fixed - source->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(Trig_Atan(dz, dx) - source->facing);
      if (turn > FACING_STEP)
        turn = FACING_STEP;
      if (turn < -FACING_STEP)
        turn = -FACING_STEP;
      source->facing += turn;
    }
    // ACTOR_FACES_MOTION off: its motion must not undo the turn.
    source->steeringFlags = 0;
  }

  // The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
  // random direction.
  position.x = source->x.fixed;
  position.y = source->y.fixed - (Random16() << 4) - toFIXED(8);
  position.z = source->z.fixed;
  radius = Random16() * 3;
  radius <<= 4;
  Vector_AddPolarOffset(radius, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

struct PendingEvent;

/// When Move is cast on a target, the pushing hand appears beside it and pushes
/// it 16 pixels the way the d-pad points, for as long as the player pushes; A,
/// B, L or R end it.
void FieldPsynergy_SteerPushedTarget(s32 unused) {
  struct Vector3 position;
  struct Vector3 beyond;
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *user = scene->mainActor;
  struct Actor *target = scene->child;
  struct Actor *pusher;
  struct Actor *hit;
  FIXED x;
  FIXED z;
  s32 angle = (u16)scene->angle + DEGtoANG(180);
  s32 moved = 0;
  s32 direction;
  s32 cellX;
  s32 cellZ;
  s32 (*callback)(void);
  u8 savedLayer = target->layer;
  struct PendingEvent *event;
  s32 pushed = 0;
  const u8 *modes;

  if (target == NULL)
    return;
  FieldPsynergy_FadeToCastColor();
  user->watchedActor = target;
  Actor_StartScriptFar(user, gFieldEffect_SourceHoldScript);
  pusher = FieldEffect_SpawnMoveHand(user);
  if (pusher == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }

  event = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
      Scene_SelectPsynergyTarget(scene->fieldPsynergy));
  pusher->watchedActor = target;
  SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
               target->z.fixed);
  Vector_AddPolarOffset(toFIXED(16), angle, &position);
  Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
  FieldEffect_SnapScaleToFull(pusher);
  pusher->speedLimit = toFIXED(4);
  pusher->acceleration = FIXED_ONE / 2;
  pusher->motionFlags = ACTOR_FLOATS;
  target->update = (ObjectUpdate)Actor_CycleSpriteColors;
  target->speedLimit = toFIXED(2) / 5;
  target->acceleration = toFIXED(1) / 5;
  target->steeringFlags = moved;
  target->layer = 2;
  while (1) {
    wait(1);
    if ((s8)scene->mode == 0 &&
        (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)))
      break;

    direction = (u16)Input_GetDpadAngle(gInput.held);
    // A scene's hook gives the way to push in place of the d-pad, or
    // DPAD_ANGLE_NONE to stop.
    callback = (s32 (*)(void))scene->finishCallback;
    if (callback != NULL) {
      direction = callback();
      if (direction == DPAD_ANGLE_NONE)
        break;
    } else if (direction == DPAD_ANGLE_NONE) {
      SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
                   target->z.fixed);
      Vector_AddPolarOffset(toFIXED(16), angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
                 target->z.fixed);
    Vector_AddPolarOffset(toFIXED(16), angle, &position);
    Vector_AddPolarOffset(toFIXED(2), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    SET_POSITION(&position, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(toFIXED(16), direction, &position);
    SET_POSITION(&beyond, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(toFIXED(32), direction, &beyond);
    if (MapGrid_CheckCollisionFar(target, &position) > 0) {
      Actor_SelectAnimationFar(pusher, 4);
      if ((gFrameCount & 15) == 0)
        Sound_PlayFar(SE_MENU_ERROR);
      continue;
    }
    hit = ActorScript_FindOverlappingEntryFar(target, &position);
    if (hit != NULL) {
      if (hit != user) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      cellX = user->x.fixed & FIELD_CELL_MASK;
      cellZ = user->z.fixed & FIELD_CELL_MASK;
      if (cellX == (position.x & FIELD_CELL_MASK) &&
          cellZ == (position.z & FIELD_CELL_MASK)) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & FIELD_CELL_MASK) &&
          cellZ == (beyond.z & FIELD_CELL_MASK)) {
        if (MapGrid_GetTerrainKindFar(user->layer, beyond.x, beyond.z) != 0) {
          Actor_SelectAnimationFar(pusher, 4);
          if ((gFrameCount & 15) == 0)
            Sound_PlayFar(SE_MENU_ERROR);
          continue;
        }
        moved = 1;
      }
    }

    Sound_PlayFar(SE_NOTE_HIGH);
    pushed = 1;
    x = position.x;
    z = position.z;
    modes = gUnused_BattleEffectBytes;
    // The hand's pose for the quarter turn between its side and the push.
    Actor_SelectAnimationFar(pusher,
                             modes[(u16)(angle - direction) / (TRIG_TURN / 4)]);
    wait(15);
    target->paused = 0;
    target->speedLimit = toFIXED(1) / 5;
    target->acceleration = toFIXED(1) / 5;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = toFIXED(1) / 5;
    pusher->acceleration = toFIXED(1) / 5;
    Vector_AddPolarOffset(toFIXED(16), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + toFIXED(16),
                           position.z);
    if (moved == 1) {
      Actor_GetById(scene->firstActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
      user->speedLimit = toFIXED(1) / 5;
      user->acceleration = toFIXED(1) / 5;
      Actor_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
    }
    Actor_WaitForMoveFar(target);
    if (event == NULL) {
      target->x.fixed = x;
      target->z.fixed = z;
      target->velocityX = 0;
      target->velocityZ = 0;
    }
    break;
  }

  target->layer = savedLayer;
  Actor_SetSpriteColorModeFar(target, scene->savedPalette);
  Actor_StartScriptFar(target, scene->savedScript);
  target->update = (ObjectUpdate)scene->savedCallback;
  if (pushed) {
    pusher->update = NULL;
    Actor_SelectAnimationFar(pusher, 4);
    Actor_SetPositionAndResetMotionFar(pusher, pusher->x.fixed, pusher->y.fixed,
                                       pusher->z.fixed);
    Field_RunPsynergyEvent(event, scene->firstActorId, scene->secondActorId);
  }
  BattleEffect_StopMainActor();
  if (moved == 1) {
    Actor_GetById(scene->firstActorId)->steeringFlags |= ACTOR_FACES_MOTION;
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(pusher);
}

/// Field effect sprite callback of a particle that flies 30 pixels out from
/// where it started, comes back and is freed.
void FieldEffect_UpdateOutAndBack(struct FieldParticle *effect) {
  struct Vector3 position;
  s32 state;

next_state:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    u32 angle;

    position.x = effect->originX;
    position.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(toFIXED(30), (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = toFIXED(4);
    effect->speedMax = toFIXED(4);
    effect->turnStepLimited = state;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto next_state;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    effect->targetX = effect->originX;
    effect->targetZ = effect->originZ;
    effect->turnStepMax = TRIG_TURN / 64;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// When Move is cast with no target, the hand appears beside the caster for 30
/// frames and then turns into particles.
void FieldPsynergy_Move(void) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  s32 work[3];
  struct FieldEffectWork *scene;
  void *object;

  scene = Ram_HeapSlots->fieldEffectWork;
  object = scene->mainActor;

  FieldPsynergy_FadeToCastColor();
  object = FieldEffect_SpawnMoveHand(object);
  FieldEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SelectAnimationFar(object, 4);
    wait(30);
  }
  FieldPsynergy_FadeFromCastColor();
  UpdateRisingParticleBurst(object);
}

/// The Broken Seal's FieldEffect_SpawnMoveHand with this game's objects
/// 278 and 673: spawns the breaking item above object and eight fragments
/// that scatter from it.
void *FieldEffect_SpawnMoveHand(void *object) {
  struct Actor *source = object;
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  FIXED radius;
  FIXED speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  parent = Actor_Spawn(SPRITE_MOVE_HAND, source->x.fixed,
                       source->y.fixed + toFIXED(16), source->z.fixed);
  if (parent == 0)
    return 0;
  parent->scaleY = FIXED_ONE / 4;
  parent->scaleX = FIXED_ONE / 4;
  parent->update = FieldEffect_UpdateMoveHand;
  parent->speedLimit = toFIXED(2);
  parent->acceleration = toFIXED(2);
  parent->motionFlags = 0;
  Actor_SelectAnimationFar(parent, 3);
  Actor_SetTargetPositionFromMagnitudeAngle(parent, toFIXED(16), angle);

  for (fragmentIndex = 0; fragmentIndex < 8; fragmentIndex++) {
    child = Actor_Spawn(SPRITE_SPARKLE, source->x.fixed,
                        source->y.fixed + toFIXED(16), source->z.fixed);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16() + FIXED_ONE;
      child->acceleration = toFIXED(1);
      child->speedLimit = speed;
      child->motionFlags = ACTOR_FALLS;
      child->gravity = toFIXED(1) / 50;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + toFIXED(8);
      jitter = Random16();
      Actor_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}

/// Raises the object's scale in sixteenths to full size and commits its
/// position.
void FieldEffect_SnapScaleToFull(struct Actor *actor) {
  FIXED next;
  FIXED scale;

  if (actor != NULL) {
    scale = actor->scaleX;
    if (scale < FIXED_ONE) {
      do {
        next = scale + FIXED_ONE / 16;
        scale = next;
      } while (next < FIXED_ONE);
      actor->scaleX = next;
      actor->scaleY = next;
    }
    Actor_WaitForMoveFar(actor);
  }
}

#define BURST_RISE_FRAMES 31
#define BURST_SPARKLES 8

/// The hand spins as it rises for BURST_RISE_FRAMES, shrinks, and bursts into
/// BURST_SPARKLES sparkles (SPRITE_SPARKLE).
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(SE_CAST);
  for (i = 0; i < BURST_RISE_FRAMES; i++) {
    src->y.fixed += toFIXED(1);
    src->facing += DEGtoANG(45);
    src->scaleX += -FIXED_ONE / 32;
    src->scaleY += -FIXED_ONE / 32;
    wait(1);
  }
  for (i = BURST_SPARKLES - 1; i >= 0; i--) {
    child =
        Actor_Spawn(SPRITE_SPARKLE, src->x.fixed, src->y.fixed, src->z.fixed);
    if (child != 0) {
      s32 speed;
      s32 unit = toFIXED(1);

      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = ACTOR_FALLS;
      child->gravity = toFIXED(1) / 25;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(src);
}

/// Reveal's colour for the rocks it shows (sprite 200), given at its start
/// and taken back at its end. Only the loop over the actors is left; value
/// goes unused.
void Actor_SetRevealSpriteColor(s32 value) {
  struct Actor *actor = Ram_HeapSlots->actors;
  s32 remainingActors = ACTOR_COUNT - 1;

  // DEAD: The Broken Seal's loop over the actors with what it did to a rock's
  // sprite gone, so its tests do nothing; the ROM keeps its count and its
  // branch.
  do {
    if (actor->script != NULL) {
      u8 kind = actor->spriteStorageKind;

      if (kind == ACTOR_SPRITE_ONE) {
      }
    }
    remainingActors--;
    actor++;
  } while (remainingActors >= 0);
}

/// How far the leader may walk from where Reveal was cast, in screen pixels up
/// and down; sideways x counts at about 0.82 of itself, so the edge is an
/// ellipse about 73 pixels to either side.
#define REVEAL_RADIUS 60

/// As The Broken Seal's, with this game's offsets: counts the timer down and,
/// once the leader is REVEAL_RADIUS from the centre or the timer has run out,
/// requests the effect's end.
void FieldPsynergy_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct Actor *actor;
  FIXED distanceX;
  s32 distanceZ;

  work = gFieldWork;
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  if (work->effectTimerRunning != 0) {
    if (work->revealFrames != 0)
      work->revealFrames--;
  }

  // In screen pixels: y is z less the height.
  distanceX = Iwram_MulFX(work->revealCenterX - actor->x.fixed / FIXED_ONE,
                          toFIXED(0.81649));
  distanceZ =
      work->revealCenterY - (actor->z.fixed - actor->y.fixed) / FIXED_ONE;
  if (distanceX * distanceX + distanceZ * distanceZ >=
          REVEAL_RADIUS * REVEAL_RADIUS ||
      work->revealFrames == 0) {
    // Raise Reveal as a Psynergy request that pays no PP (ACT_PACKED
    // | ABILITY_REVEAL), which runs its ending events.
    u16 *slot = (u16 *)&work->psynergy;
    u32 request = ABILITY_REVEAL | ACT_PACKED;

    *slot = request;
  }
}

#define REVEAL_FRAMES 600 ///< ten seconds of Reveal

/// Reveal: open a circle of the transition on the caster, wash the screen
/// plain, run the Reveal event that matches, then show the markers nearby
/// and watch how far the leader walks.
void FieldPsynergy_Reveal(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event;
  struct ScreenTransitionState *state;
  struct Actor *caster;
  void *pending;
  s32 frame;

  Actor_SetRevealSpriteColor(6);
  event = Ram_HeapSlots->fieldWork;
  ScreenTransition_InitializeBattleEffectState(FIELD_PSYNERGY_REVEAL);
  state = Ram_HeapSlots->screenTransition;
  caster = scene->mainActor;
  state->centreX = caster->x.fixed;
  state->centreY = caster->z.fixed - caster->y.fixed;
  PaletteFade_SetSource(FIXED_ONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, 1);
  PaletteFade_StartInterpolation(1);
  event->revealActive = 0;
  wait(1);

  pending = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_REVEAL,
      Scene_SelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
  if (pending != 0)
    Field_RunPsynergyEvent(pending, scene->firstActorId, scene->secondActorId);
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Io_Write8(1, &event->revealActive);
  event->revealCenterX = state->centreX / FIXED_ONE;
  event->revealCenterY = state->centreY / FIXED_ONE;
  event->revealFrames = REVEAL_FRAMES;
  event->effectTimerRunning = 1;
  FieldEffect_SpawnRevealMarkers();
  for (frame = 0; frame <= 18; frame++) {
    wait(1);
    state->value = frame;
  }
  if ((s8)scene->mode == 0)
    Task_Add(FieldPsynergy_WatchLeaderDistance, TASK_PASS_FRAME);
}

/// Ends Reveal if it is running: stops its watch, brings the colours back from
/// black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES event.
void FieldPsynergy_EndReveal(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  void *pending;

  if (event->revealActive != 0) {
    Sound_PlayFar(SE_MAP_TRANSITION);
    Task_Remove(FieldPsynergy_WatchLeaderDistance);
    event->revealActive = 0;
    event->revealFrames = 0;
    Actor_SetRevealSpriteColor(0);
    PaletteFade_SetTarget(FIXED_ONE, 1);
    PaletteFade_StartInterpolation(1);
    PaletteFade_SetSource(0, 0);
    PaletteFade_SetTarget(FIXED_ONE, 0);
    PaletteFade_StartInterpolation(30);
    wait(1);
    pending = Scene_FindPsynergyTrigger(
        PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES), FIELD_PSYNERGY_REVEAL,
        Scene_SelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstActorId,
                             scene->secondActorId);
    if (scene->childOption == 0) {
      state->nextMode = 0;
      state->duration = 1;
      state->step = 1;
      wait(10);
    }
  }
}

/// Growth cast on a target: the burst of particles, then the target takes its
/// grown animation and the underfoot priority, with SE_RECOVERY. Sprouting
/// leaves (SPRITE_SPROUTING_LEAVES) instead drop five vines round the
/// Psynergy's position and hang a vine (HangingVine_Hang). On an arrow plant
/// Growth's own effect only plays SE_RECOVERY; the scene's event turns it into
/// an arrow (GaiaRockInside_PlantPointsTheWay).
void FieldPsynergy_GrowthOnTarget(s32 unused) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  FieldEffect_SpawnBurstParticleField();
  if (Actor_GetSpriteIdFar(target) == SPRITE_SPROUTING_LEAVES)
    kind = 1;
  if (Actor_GetSpriteIdFar(target) == SPRITE_ARROW_PLANT)
    kind = 2;

  if (kind == 0) {
    Actor_SelectAnimationFar(target, 2);
    Actor_SetSpriteFlagsFar(target, 0);
    target->collisionFlags = 0;
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    wait(10);
    Sound_PlayFar(SE_RECOVERY);
    wait(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    s32 n;

    for (n = 0; n < 5; n++) {
      struct Actor *piece;
      FIXED distance;

      position.values[0] = scene->x;
      position.values[2] = scene->z;
      distance = Random16() * 6 + toFIXED(4);
      Vector_AddPolarOffset(distance, Random16(), position.values);
      position.values[1] = scene->y;
      piece = Actor_Spawn(SPRITE_VINE, position.values[0], position.values[1],
                          position.values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, gFieldEffect_HeavyImpactScript);
        piece->motionFlags = ACTOR_FALLS;
      }
      wait((Random_Below(2)) + 2);
    }
    HangingVine_Hang(scene->secondActorId, 1);
  } else {
    wait(10);
    Sound_PlayFar(SE_RECOVERY);
  }
  FieldPsynergy_FadeFromCastColor();
}

/// When Growth is cast with no target, particles fly out from in front of the
/// caster, and then five falling actors appear 4 to 10 pixels round the
/// Psynergy's position.
void FieldPsynergy_Growth(void) {
  struct FieldEffectWork *state = Ram_HeapSlots->fieldEffectWork;
  struct BurstParticleVector position;
  s32 entryCount;

  FieldEffect_SpawnBurstParticleField();
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (entryCount = 0; entryCount < 5; entryCount++) {
    void *object;
    s32 randomValue;

    position.values[0] = state->x;
    position.values[2] = state->z;
    randomValue = (Random16() * 6) + toFIXED(4);
    Vector_AddPolarOffset(randomValue, Random16(), position.values);
    position.values[1] = state->y;
    object = Actor_Spawn(SPRITE_VINE, position.values[0], position.values[1],
                         position.values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, gFieldEffect_HeavyImpactScript);
      ((struct Actor *)object)->motionFlags = ACTOR_FALLS;
    }
    wait((Random_Below(2)) + 2);
  }
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

#define BURST_FIELD_PARTICLES 24

/// Sends BURST_FIELD_PARTICLES particles, one every 2 frames, from in front of
/// the caster to points round the Psynergy's position.
void FieldEffect_SpawnBurstParticleField(void) {
  struct FieldEffectWork *work;
  struct Actor *user;
  FIXED pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->fieldEffectWork;
  user = work->mainActor;
  FieldPsynergy_FadeToCastColor();

  p = pos;
  for (n = 0; n < BURST_FIELD_PARTICLES; n++) {
    struct Actor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    FIXED dist;

    if (*(u16 *)&work->angle == DEGtoANG(90)) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + toFIXED(10);
      pos[2] = user->z.fixed;
    } else if (*(u16 *)&work->angle == TRIG_TURN * 3 / 4) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + toFIXED(24);
      pos[2] = user->z.fixed;
    } else {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + toFIXED(10);
      pos[2] = user->z.fixed;
      Vector_AddPolarOffset(toFIXED(10), *(u16 *)&work->angle, pos);
    }

    obj = Actor_Spawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->fullColor = visual->fullColor;
    child->size = visual->size;
    child->shape = visual->shape;
    child->attributes.bits.tile = visual->attributes.bits.tile;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      obj->scaleY = toFIXED(0.7);
      obj->scaleX = toFIXED(0.7);
      obj->acceleration = toFIXED(3) / 2;
      obj->speedLimit = toFIXED(3) / 2;
      obj->motionFlags = 0;
      Actor_SetSpriteColorModeFar(obj, SPRITE_COLOR_GREEN_TINT);
      Actor_SelectAnimationFar(obj, 7);
      Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript + 1);

      Actor_SetSpriteFlagsFar(obj, SPRITE_FLAG_SHADOW);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == TRIG_TURN * 3 / 4)
        Vector_AddPolarOffset(toFIXED(14), *(u16 *)&work->angle, p);
      dist = Random16() * 6 + toFIXED(4);
      Vector_AddPolarOffset(dist, Random16(), p);
      Actor_SetMoveTargetFar(obj, p[0], p[1], p[2]);
    }
    Sound_PlayFar(SE_PSYNERGY_SPARKLE);
    wait(2);
  }
  wait(8);
}

/// A hand appears 32 pixels to either side, each moves 14 pixels in, and they
/// lift the target 32 pixels with them.
void FieldPsynergy_LiftOnTarget(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *caster;
  void *child;
  void *anchors[2];
  FIXED position[3];
  s32 x;
  FIXED y;
  s32 z;
  s32 index;
  s32 startY;

  effectWork = Ram_HeapSlots->fieldEffectWork;
  child = effectWork->child;
  caster = effectWork->mainActor;
  if (child == 0)
    return;

  FieldPsynergy_FadeToCastColor();
  caster->watchedActor = child;
  Actor_StartScriptFar(caster, gFieldEffect_SourceHoldScript);

  x = effectWork->x;
  position[0] = x;
  y = effectWork->y + toFIXED(16);
  position[1] = y;
  z = effectWork->z;
  position[2] = z;
  anchors[0] = FieldEffect_SpawnLiftHand(x + toFIXED(32), y, z, FACING_WEST);
  anchors[1] = FieldEffect_SpawnLiftHand(position[0] - toFIXED(32), position[1],
                                         position[2], FACING_EAST);

  wait(15);
  for (index = 0; index < 2; index++) {
    void *anchor = anchors[index];
    if (anchor != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(
          anchor, toFIXED(14), ((struct Actor *)anchor)->facing);
  }

  Actor_WaitForMoveFar(anchors[0]);
  ((struct EffectChild *)child)->callback =
      (void (*)(void))Actor_CycleSpriteColors;
  Sound_PlayFar(SE_PSYNERGY_START);
  ((struct EffectChild *)child)->flag = 4;

  startY = ((struct EffectChild *)child)->y;
  if (anchors[0] != 0 && anchors[1] != 0 && startY <= startY + toFIXED(32)) {
    do {
      ((struct Actor *)anchors[0])->y.fixed += toFIXED(1) / 4;
      ((struct Actor *)anchors[1])->y.fixed += toFIXED(1) / 4;
      ((struct EffectChild *)child)->y += toFIXED(1) / 4;
      wait(1);
    } while (((struct EffectChild *)child)->y <= startY + toFIXED(32));
  }

  UpdateRisingParticleBurst(anchors[0]);
  UpdateRisingParticleBurst(anchors[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Lift cast with no target: two hands appear 32 pixels to either side of the
/// spot, close 24 pixels in and meet with a crash that scatters
/// ITEM_BREAK_SHARD_MAX sparkles, then vanish.
void FieldPsynergy_Lift(void) {
  struct FieldEffectWork *work;
  struct Actor *obj;
  struct Actor *pair[2];
  struct Vector3 pos;
  struct Actor **walk;
  s32 n;
  FIXED dist;

  work = Ram_HeapSlots->fieldEffectWork;
  FieldPsynergy_FadeToCastColor();
  SET_POSITION(&pos, work->x, work->y + toFIXED(16), work->z);
  pair[0] = FieldEffect_SpawnLiftHand(pos.x + toFIXED(32), pos.y, pos.z,
                                      DEGtoANG(180));
  pair[1] = FieldEffect_SpawnLiftHand(pos.x - toFIXED(32), pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(obj, toFIXED(24), obj->facing);
  }

  Actor_WaitForMoveFar(pair[0]);
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {
    SET_POSITION(&pos, work->x, work->y + toFIXED(16), work->z);
    obj = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gFieldEffect_FragmentScript);
      obj->speedLimit = Random16() + toFIXED(2);
      obj->acceleration = toFIXED(2);
      obj->motionFlags = 0;
      dist = Random16() * 24 + toFIXED(8);
      Actor_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_ReleaseFar(pair[0]);
  Actor_ReleaseFar(pair[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Spawns one of Lift's hands (SPRITE_MOVE_HAND) and grows it from a quarter
/// to full size, an eighth of a turn a frame, then faces it to angle.
struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *actor;
  s32 scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  actor = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (actor != NULL) {
    actor->scaleX = actor->scaleY = FIXED_ONE / 4;
    actor->acceleration = actor->speedLimit = toFIXED(3);
    ((struct Sprite *)actor->sprite)->image.priority = 0;
    Actor_SelectAnimationFar(actor, 3);
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

/// Field effect sprite callback of Force's particles (enum OutAndBackState):
/// each flies 25 pixels from where it appeared, then back to a point 8 pixels
/// toward the caster's facing from 16 pixels above it, on screen, give or take
/// 4 pixels, and is freed on arrival.
void FieldEffect_UpdateRadialBurst(struct FieldParticle *effect) {
  // OUT_AND_BACK_FLYING_OUT must enter OUT_AND_BACK_AIM_BACK immediately.
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Vector3 pos;
  s32 state;

again:
  state = effect->state;
  if (state == OUT_AND_BACK_AIM_OUT) {
    // 25 pixels from the origin, in a random direction.
    pos.x = effect->originX;
    pos.z = effect->originZ;
    Vector_AddPolarOffset(toFIXED(25), (u16)Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = toFIXED(3);
    effect->speedMax = toFIXED(3);
    effect->turnStepLimited = 0;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_OUT) {
    // On arrival, go straight on.
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == OUT_AND_BACK_AIM_BACK) {
    // 8 pixels toward effectWork->angle from 16 pixels above the main actor,
    // on screen, give or take 4 pixels.
    struct Actor *caster = effectWork->mainActor;

    pos.x = caster->x.fixed;
    pos.y = caster->y.fixed + toFIXED(16);
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(toFIXED(8), effectWork->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->turnStepMax = TRIG_TURN / 32;
    effect->turnStepLimited = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

#define IMPACT_SHARDS 12

/// A heavy impact, as The Broken Seal's: an actor placed below the source
/// for a moment, scattering IMPACT_SHARDS shards from it.
s32 SpawnHeavyImpactEffect(struct Actor *source) {
  struct Vector3 pos;
  struct Actor *impact;
  struct Actor *fragment;
  s32 fragmentIndex;

  Sound_PlayFar(SE_HEAVY_IMPACT);
  pos.x = source->x.fixed;
  pos.y = source->y.fixed;
  pos.z = source->z.fixed;
  impact = Actor_Spawn(SPRITE_STAR_BURST, pos.x, pos.y - toFIXED(32), pos.z);
  if (impact != NULL) {
    impact->motionFlags = 0;
    impact->wait = 20;
    Actor_StartScriptFar(impact, gBattleEffect_SharedParticleScript);
  }

  // Shards: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction.
  for (fragmentIndex = 0; fragmentIndex < IMPACT_SHARDS; fragmentIndex++) {
    fragment = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (fragment != NULL) {
      Actor_StartScriptFar(fragment, gFieldEffect_FragmentScript);
      fragment->speedLimit = Random16() + toFIXED(1);
      fragment->acceleration = toFIXED(1);
      fragment->motionFlags = 0;
      Actor_SetTargetPositionFromMagnitudeAngle(
          fragment, Random16() * 24 + toFIXED(8), Random16());
    }
  }
  return 0;
}

/// Runs Force cast on a target, through FieldPsynergy_Force.
void FieldPsynergy_ForceOnTarget(s32 unused) { FieldPsynergy_Force(); }

/// Force's ring of particles round the caster, one every 2 frames, and the
/// frames it waits at most for the fist to strike.
#define FORCE_RING_PARTICLES 12
#define FORCE_STRIKE_FRAMES 60

/// Force: a ring of particles round the caster, then a fist (SPRITE_MOVE_HAND)
/// grows in front of them and strikes 56 pixels ahead, or at the target, with
/// three afterimages; the target is pushed 16 pixels the way the fist flew
/// (and thrown up when its option asks), and the scene's Force event runs.
void FieldPsynergy_Force(void) {
  struct Actor *copies[4];
  struct Vector3 pos;
  struct FieldEffectWork *scene;
  struct Actor *target;
  struct Actor *source;
  struct Actor *actor;
  struct Actor *copy;
  struct FieldParticle *slot;
  struct Sprite *resource;
  struct PendingEvent *event;
  s32 scale;
  s32 i;
  u8 marker;

  scene = Ram_HeapSlots->fieldEffectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_PSYNERGY_START);
  slot = scene->particles;

  for (i = 0; i < FORCE_RING_PARTICLES; i++) {
    source = scene->mainActor;
    SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
                 source->z.fixed);
    Camera_WorldToScreen(&pos);
    FieldParticle_Initialize(slot, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateRadialBurst);
    FieldParticle_SetActorMode(slot, 7);
    Sprite_SetAnimationColorModesFar(slot->actor, SPRITE_COLOR_BLUE_TINT);
    slot->scaleY = FIXED_ONE * 7 / 10;
    slot->scaleX = FIXED_ONE * 7 / 10;
    wait(2);
    slot++;
  }

  source = scene->mainActor;
  SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
               source->z.fixed);
  Vector_AddPolarOffset(toFIXED(8), scene->angle, &pos);
  actor = Actor_Spawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
  if (actor == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  actor->scaleY = FIXED_ONE / 4;
  actor->scaleX = FIXED_ONE / 4;
  actor->facing = scene->angle;
  actor->speedLimit = toFIXED(4);
  actor->acceleration = toFIXED(4);
  actor->motionFlags = 0;
  Actor_SelectAnimationFar(actor, 5);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_BGR);
  scale = actor->scaleX;
  if (scale < FIXED_ONE) {
    do {
      scale += FIXED_ONE * 5 / 256;
      actor->scaleY = scale;
      actor->scaleX = scale;
      wait(1);
      scale = actor->scaleX;
    } while (scale < FIXED_ONE);
  }
  wait(3);

  resource = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = Actor_Spawn(SPRITE_MOVE_HAND, actor->x.fixed,
                                   actor->y.fixed, actor->z.fixed);
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
      copy->facing = scene->angle;
      copy->speedLimit = toFIXED(4);
      copy->acceleration = toFIXED(4);
      copy->motionFlags = 0;
      Actor_SelectAnimationFar(copy, 5);
      Actor_SetSpriteColorModeFar(copy, SPRITE_COLOR_RBG);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
    }
  }
  marker = resource->slot;

  if (scene->enabled != 0) {
    source = scene->mainActor;
    SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
                 source->z.fixed);
    Vector_AddPolarOffset(toFIXED(56), scene->angle, &pos);
  } else {
    SET_POSITION(&pos, scene->x, scene->y + toFIXED(16), scene->z);
  }
  Actor_SetMoveTargetFar(actor, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(actor, gBattleEffect_HeavyImpactScript);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      wait(3);
      Actor_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      Actor_StartScriptFar(copy, gBattleEffect_SharedParticleScript + 1);
    }
  }

  for (i = 0; i < FORCE_STRIKE_FRAMES && actor->script != NULL; i++)
    wait(1);

  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = toFIXED(8);
    SET_POSITION(&pos, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(toFIXED(16), scene->angle, &pos);
    if (MapGrid_CheckCollisionFar(target, &pos) == 0 &&
        ActorScript_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = toFIXED(1);
      target->speedLimit = toFIXED(1);
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  event = Scene_FindPsynergyTrigger(
      PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_FORCE,
      Scene_SelectPsynergyTarget(FIELD_PSYNERGY_FORCE));
  if (event != NULL)
    Field_RunPsynergyEvent(event, scene->firstActorId, scene->secondActorId);
  wait(10);
  FieldPsynergy_FadeFromCastColor();
  wait(20);
  // VRAM_CACHE_ENTRY_COUNT: no afterimage took a cache entry.
  if (marker != VRAM_CACHE_ENTRY_COUNT)
    VramCache_FreeEntry(marker);
}

struct OrbitingParticle;

/// Per-frame update of a Frost glint: of every eight frames, two show the
/// particle in swapped colours (SPRITE_COLOR_RBG).
s32 FieldEffect_UpdateFrostGlint(struct OrbitingParticle *particle) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    Actor_SetSpriteColorModeFar((struct Actor *)particle, SPRITE_COLOR_RBG);
  } else if (phase == 2) {
    Actor_SetSpriteColorModeFar((struct Actor *)particle, SPRITE_COLOR_NORMAL);
  }
  return 0;
}

/// Per-frame update: shrinks by 1/16 a frame and is released at 1/16.
void FieldEffect_UpdateOrbitingParticleFade(struct Actor *particle) {
  FIXED scale;

  if (particle != NULL) {
    scale = particle->scaleX - FIXED_ONE / 16;
    particle->scaleY -= FIXED_ONE / 16;
    particle->scaleX = scale;
    if (scale <= FIXED_ONE / 16)
      Actor_StartScriptFar(particle, gBattleEffect_SharedParticleScript);
  }
}

/// Spirals anticlockwise at twice the frames left in pixels, then fades.
void FieldEffect_UpdateOrbitingParticleLeft(struct Actor *particle) {
  struct Vector3 pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      SET_POSITION(&pos, particle->targetX, particle->targetY,
                   particle->targetZ);
      Vector_AddPolarOffset(life * toFIXED(2),
                            (s16)particle->userValue + life * (TRIG_TURN / 32),
                            &pos);
      SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (ObjectUpdate)FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

/// Per-frame update: as FieldEffect_UpdateOrbitingParticleLeft, clockwise.
void FieldEffect_UpdateOrbitingParticleRight(struct Actor *particle) {
  struct Vector3 pos;
  s16 life;
  s32 count;

  if (particle != NULL) {
    count = (u16)particle->timer - 1;
    particle->timer = count;
    life = count;
    if (life != 0) {
      SET_POSITION(&pos, particle->targetX, particle->targetY,
                   particle->targetZ);
      Vector_AddPolarOffset(life * toFIXED(2),
                            (s16)particle->userValue - life * (TRIG_TURN / 32),
                            &pos);
      SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (ObjectUpdate)FieldEffect_UpdateOrbitingParticleFade;
    }
  }
}

/// Runs Frost cast on a target.
void FieldPsynergy_FrostOnTarget(s32 unused) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *target = effectWork->child;

  if (target != 0) {
    // enabled set skips the target's part of FieldPsynergy_Frost.
    if (effectWork->childMode != 0) {
      effectWork->enabled = 1;
    }
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    FieldPsynergy_Frost();
  }
}

#define FROST_SNOWFLAKES 16 ///< circling the target

/// FROST_SNOWFLAKES particles, one every three frames, orbit points scattered 2
/// to 6 pixels around the effect; the target then flashes eight times, takes
/// the orbiting particles' main update and runs its pending event.
void FieldPsynergy_Frost(void) {
  struct Vector3 pos;
  struct FieldEffectWork *scene;
  struct Actor *target;
  struct Actor *particle;
  struct PendingEvent *ev;
  s32 cnt;

  scene = Ram_HeapSlots->fieldEffectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ICE);
  for (cnt = 0; cnt < FROST_SNOWFLAKES; cnt++) {
    particle = Actor_Spawn(SPRITE_SNOWFLAKE, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      FIXED dist;

      scale = (Random16() >> 1) + FIXED_ONE / 2;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update = (ObjectUpdate)FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (ObjectUpdate)FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->userValue = Random16();
      Actor_SetSpriteColorModeFar((struct Actor *)particle,
                                  SPRITE_COLOR_BLUE_TINT);

      SET_POSITION(&pos, scene->x, scene->y, scene->z);
      dist = (Random16() << 2) + toFIXED(2);
      Vector_AddPolarOffset(dist, Random16(), &pos);
      particle->targetX = pos.x;
      particle->targetY = pos.y;
      particle->targetZ = pos.z;
    }

    wait(3);
  }

  wait(10);
  Sound_PlayFar(SE_ICE);
  wait(50);

  if (target != NULL && scene->enabled == 0) {
    Sound_PlayFar(SE_POWER_UP);

    for (cnt = 0; cnt < 8; cnt++) {
      Actor_SetSpriteColorModeFar((struct Actor *)target, SPRITE_COLOR_WHITE);
      wait(1);
      Actor_SetSpriteColorModeFar((struct Actor *)target, SPRITE_COLOR_NORMAL);
      wait(4);
    }

    if (scene->childOption == 0) {
      Sound_PlayFar(SE_SHIMMER);
      Actor_SelectAnimationFar(target, 2);
    }

    target->update = (ObjectUpdate)FieldEffect_UpdateFrostGlint;
    ev = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), 6,
                                   Scene_SelectPsynergyTarget(6));
    if (ev != NULL)
      Field_RunPsynergyEvent(ev, scene->firstActorId, scene->secondActorId);
    wait(20);
  }

  FieldPsynergy_FadeFromCastColor();
}

/// Per-frame update: sways 4 pixels either side of targetX, once every 128
/// frames.
void FieldEffect_SwayParticle(struct Actor *actor) {
  FIXED amplitude = toFIXED(4);

  actor->x.fixed =
      actor->targetX +
      Iwram_MulFX(amplitude, Trig_Sin(actor->timer * (TRIG_TURN / 128)));
  actor->timer++;
  actor->timer = (actor->timer + 128) % 128;
}

/// The particle circles 10 pixels above the scene's position, its radius in
/// pixels the frames it has left; at 0 it runs the shared particle script.
void FieldEffect_UpdateShrinkingOrbitActor(struct Actor *particle) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Vector3 pos;
  s16 remainingFrames;
  s32 decrementedTimer;

  if (particle != NULL) {
    decrementedTimer = (u16)particle->timer - 1;
    particle->timer = decrementedTimer;
    remainingFrames = decrementedTimer;
    if (remainingFrames != 0) {
      // The angle is 1/32 turn further round for each frame left.
      pos.x = effectWork->x;
      pos.y = effectWork->y + toFIXED(10);
      pos.z = effectWork->z;
      Vector_AddPolarOffset(
          INTtoFIXED(remainingFrames),
          (s16)particle->userValue + remainingFrames * (TRIG_TURN / 32), &pos);
      particle->x.fixed = pos.x;
      particle->y.fixed = pos.y;
      particle->z.fixed = pos.z;
    } else {
      Actor_StartScriptFar(particle, gBattleEffect_SharedParticleScript);
    }
  }
}

/// Per-frame update: sways 6 pixels either side (a 64-frame cycle) 16 pixels
/// above the Psynergy's position, and every 3 frames sheds a spark that sways
/// as it slowly falls.
void FieldEffect_RunSparkEmitter(struct Actor *emitter) {
  struct FieldEffectWork *effectWork;
  struct Actor *spark;
  FIXED pos[3];

  effectWork = Ram_HeapSlots->fieldEffectWork;
  if (emitter->timer != -1) {
    FIXED amplitude = toFIXED(6);

    SET_ACTOR_POSITION(
        emitter,
        effectWork->x +
            Iwram_MulFX(amplitude, Trig_Sin(emitter->timer * (TRIG_TURN / 64))),
        effectWork->y + toFIXED(16), effectWork->z);
    emitter->timer++;
    emitter->timer = (emitter->timer + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // From 2 pixels up, 0 to 6 pixels in a random direction.
    pos[0] = emitter->x.fixed;
    pos[1] = emitter->y.fixed + toFIXED(2);
    pos[2] = emitter->z.fixed;
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = Actor_Spawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
    // Each is released after 72 frames, its script's wait.
    if (spark != 0) {
      spark->update = (ObjectUpdate)FieldEffect_SwayParticle;
      spark->scaleX = spark->scaleY = FIXED_ONE * 3 / 5;
      spark->motionFlags = ACTOR_FALLS;
      spark->gravity = toFIXED(0.007);
      spark->timer = Random16() >> 9;
      spark->targetX = spark->x.fixed;
      Actor_SetSpriteColorModeFar(spark, SPRITE_COLOR_BLUE_TINT);
      spark->wait = 72;
      Actor_StartScriptFar(spark, gBattleEffect_SharedParticleScript);
    }
  }
}

/// Ply cast on a target: runs Ply, then stops the caster's hook.
void FieldPsynergy_PlyOnTarget(void) {
  FieldPsynergy_Ply();
  BattleEffect_StopMainActor();
}

/// Ply's flashing figure: its steps, of 2 frames each.
#define PLY_FLASH_STEPS 30

/// Ply: eight winged figures spiral in on the Psynergy's position, one a
/// frame; then one in the centre flashes for 60 frames, sheds sparks and flies
/// off.
void FieldPsynergy_Ply(void) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *target = effectWork->mainActor;
  struct Actor *actor;
  struct Sprite *last;
  u8 resourceSlot;
  s32 i;

  FieldPsynergy_FadeToCastColor();

  // The spiralling particles: one a frame, each 1/8 turn on from the last.
  last = 0;
  for (i = 0; i < 8; i++) {
    actor = Actor_Spawn(SPRITE_WINGED_FIGURE, target->x.fixed,
                        target->y.fixed + toFIXED(32), target->z.fixed);
    if (actor != 0) {
      actor->scaleY = FIXED_ONE * 7 / 10;
      actor->scaleX = FIXED_ONE * 7 / 10;
      actor->update = (ObjectUpdate)FieldEffect_UpdateShrinkingOrbitActor;
      actor->timer = 120;
      actor->userValue = i * (TRIG_TURN / 8);
      actor->motionFlags = ACTOR_FLOATS;
      last = Sprite_ShareVramCacheEntry(actor->sprite, last);
    }
    wait(1);
  }

  // The particles share one sprite cache; keep its slot to free at the end.
  // last is read with no check that any particle spawned.
  resourceSlot = last->slot;
  Sound_PlayFar(SE_PSYNERGY_START);
  // The last particle spawned 7 frames ago with 120 frames to live: wait
  // until the spiral is nearly closed.
  wait(110);

  // The one in the centre.
  target = Actor_Spawn(SPRITE_WINGED_FIGURE, 0, 0, 0);
  /* FAKEMATCH(temporary): actor, a copy of target, the figure read by both
   * names. */
  actor = target;
  if (target != 0) {
    target->scaleY = FIXED_ONE * 7 / 10;
    target->scaleX = FIXED_ONE * 7 / 10;
    target->x.fixed = effectWork->x;
    target->y.fixed = effectWork->y + toFIXED(16);
    target->z.fixed = effectWork->z;
    target->motionFlags = ACTOR_FLOATS;
    Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_WHITE);
  }

  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  wait(12);
  if (target != 0) {
    // Red one step in four, else blue, a step each 2 frames.
    for (i = 0; i < PLY_FLASH_STEPS; i++) {
      /* FAKEMATCH(dead): target set again to actor at each step; as the loop
       * without it. */
      target = actor;
      if (i & 3)
        Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_BLUE_TINT);
      else
        Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_RED_TINT);
      wait(2);
    }
  }

  // Sparks.
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  Sound_PlayFar(MUS_POISON_CURED);
  if (actor != 0) {
    target->update = (ObjectUpdate)FieldEffect_RunSparkEmitter;
    target->timer = 0;
    // Cast on a target (enabled cleared by FieldPsynergy_Dispatch),
    // the sparks fall for 192 frames instead of 128.
    if (effectWork->enabled != 0)
      wait(128);
    else
      wait(192);
  }

  // Fly away: 192 pixels at angle TRIG_TURN * 29 / 32, and release it once it
  // arrives.
  if (target != 0) {
    target->timer = -1;
    target->speedLimit = toFIXED(5);
    target->acceleration = toFIXED(2) / 5;
    target->steeringFlags = 0;
    Actor_SetTargetPositionFromMagnitudeAngle(target, toFIXED(192),
                                              TRIG_TURN * 29 / 32);
    Actor_WaitForMoveFar(target);
    Actor_ReleaseFar(target);
  }
  // VramCache_FindFreeEntry returns VRAM_CACHE_ENTRY_COUNT for no entry.
  if (resourceSlot != VRAM_CACHE_ENTRY_COUNT)
    VramCache_FreeEntry(resourceSlot);
  if (effectWork->finishCallback != 0)
    effectWork->finishCallback();
  FieldPsynergy_FadeFromCastColor();
}

/// A task of Cloak: when the controlled character does not
/// stand on a tile Cloak can hide on, requests CLOAK_END (Cloak, packed).
void FieldEffect_CheckCloakTile(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Lookup(Party_GetLeaderActor());

  if (MapGrid_IsCloakCell(leader->x.fixed, leader->z.fixed, leader->layer) == 0)
    event->psynergy = ABILITY_CLOAK | ACT_PACKED;
}

/// Show the Cloaked caster again: stop checking the ground under it, then
/// flicker it back into view over twenty frames and clear the cloak
/// (FieldPsynergy_RunExtraHandler).
void FieldPsynergy_EndCloak(void) {
  struct Actor *caster =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  struct Sprite *sprite = caster->sprite;
  struct SpriteAnimation *entry = sprite->entries[0];
  u32 i;

  Sound_PlayFar(SE_CAST);
  Task_Remove(FieldEffect_CheckCloakTile);
  Actor_SelectAnimationFar(caster, ANIM_STILL);
  caster->update = 0;

  // Blink between the flicker palette half transparent and solid...
  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    sprite->dirty = 1;
    wait(2);
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
  }

  // ...then between the flicker palette and the caster's own.
  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->flags = 0;
    sprite->dirty = 1;
    wait(2);
    entry->colorMode = SPRITE_COLOR_NORMAL;
    sprite->dirty = 1;
    wait(2);
  }

  sprite->flags = SPRITE_FLAG_SHADOW;
  gMainRoutineWork.cloaked = 0;
}

/// Starts the Cloak cell check if the leader is hidden. FieldSequence_Run calls
/// it as a scene loads, since gMainRoutineWork.cloaked outlives the scene.
void FieldPsynergy_ResumeCloakCheck(void) {
  if (gMainRoutineWork.cloaked != 0)
    Task_Add(FieldEffect_CheckCloakTile, TASK_PASS_FRAME);
}
