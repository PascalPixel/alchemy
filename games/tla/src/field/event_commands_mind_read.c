/// @file
/// The field Psynergy from Mind Read to Cloak, as The Broken Seal's
/// event_commands.c has them in its parts mind_read.c to cloak.c: Mind Read's
/// wave, Move, Reveal, Growth, Lift, Force, Frost, Ply and the start of Cloak.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "sounds.h"

#include "system/io_reg.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/objdisp.h"
#include "system/input.h"
#include "edition.h"
#include "fixed_math.h"
#include "battle/object_effect_scripts.h"
#include "field/field_event.h"
#include "field/field_psynergy_work.h"
#include "debug/debug.h"
#include "field/field_psynergy_work.h"
#include "graphics/display_transition.h"
#include "field/field_sequence_event.h"
#include "system/scene.h"
#include "macros.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "window/item_icon.h"

// The parts, in ROM order.

/// Each frame after the delay: rewrites the 160-line wave into the other
/// page, cycles the colour every other frame, and at counts 0, 8 and 16 of
/// its 61-frame cycle sends a particle from the target back to the user.
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
  struct Actor *object;
  u16 *line;
  s32 i;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  line = work->lines[work->page ^ 1];
  for (i = 0; i < 160; i++)
    *line++ = Trig_Sin(((work->phase + i * 8) << 16) / 160) >> 14;
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    FieldEffect_AdvanceHueCycle();
    PaletteFade_SetTarget(
        RGB(work->red, work->green, work->blue) | PALETTE_TINT_CUSTOM, 1);
    PaletteFade_StartInterpolation(1);
  }
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    source = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (source != NULL && target != NULL) {
      descriptor =
          (struct ActorPlacement *)Scene_FindActorPlacement(work->sourceId);
      spriteId = descriptor->sprite;
      from.x = source->x.fixed;
      metadata = Resource_GetMetadataRecordFarFar(spriteId);
      from.y = source->y.fixed + px((s8)metadata->boxX) - px(2);
      from.z = source->z.fixed;
      sprite = target->sprite;
      descriptor =
          (struct ActorPlacement *)Scene_FindActorPlacement(work->targetId);
      spriteId = descriptor->sprite;
      to.x = target->x.fixed;
      metadata = Resource_GetMetadataRecordFarFar(spriteId);
      {
        s32 box = (s8)metadata->boxX;
        s32 y = target->y.fixed;

        y += px(box);
        to.y = y - px(2);
      }
      to.z = target->z.fixed;
      if (sprite->rotation == TRIG_TURN / 2) {
        to.y -= px(24);
      } else if (sprite->rotation == TRIG_TURN / 4) {
        to.x += px(6);
        to.y -= px(12);
      } else if (sprite->rotation == TRIG_TURN * 3 / 4) {
        to.x -= px(6);
        to.y -= px(12);
      }
      object = Actor_CreateFar(SPRITE_CRESCENT, to.x, to.y, to.z);
      if (object != NULL) {
        sprite = object->sprite;
        object->motionFlags = 0;
        object->speedLimit = px(16) / 25; // 0.64 pixels a frame
        object->acceleration = px(16) / 25;
        object->facing = ArcTan2(from.z - to.z, from.x - to.x);
        object->update = (void (*)(void))FieldEffect_ReleaseOnArrival;
        sprite->flags = 0;
        sprite->priority = 1;
        Actor_SetMoveTargetFar(object, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter != 99) {
    if (work->counter == 0)
      Sound_PlayFar(SE_PSYNERGY_START);
    if (++work->counter > 60)
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

  angle = (angle + 360) % 360;
  if (angle < 60) {
    level = angle * 31 / 60;
  } else if (angle < 180) {
    level = RGB_LEVEL_MAX;
  } else if (angle < 240) {
    level = (240 - angle) * 31 / 60;
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
  g = FieldEffect_HueChannelRamp(work->hue + 120);
  b = FieldEffect_HueChannelRamp(work->hue + 240);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & 31;
  work->green = (g / 4 + 4) & 31;
  work->blue = (b / 4 + 4) & 31;
  limit = px(359);
  if ((u32)((work->hue += 4) << 16) > limit)
    work->hue = 0;
}

/// Per-frame update: once the actor has arrived (no target left on any axis),
/// releases it through gBattleEffect_UntargetedObjectScript.
void FieldEffect_ReleaseOnArrival(struct Actor *target) {
  s32 tx;
  s32 ty;

  tx = target->targetX;
  if (tx == 0x80000000) {
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
  u16 *words = gBattleEffect_EightWayHeadingWords;

  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

// Effects: motion of the main object in an item-breaking effect.

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
    s32 dx = target->x.fixed - source->x.fixed;
    s32 dz = target->z.fixed - source->z.fixed;

    if (dx != 0 || dz != 0) {
      turn = (s16)(ArcTan2(dz, dx) - source->facing);
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
  position.y = source->y.fixed - (Random16() << 4) - px(8);
  position.z = source->z.fixed;
  radius = Random16() * 3;
  radius <<= 4;
  Vector_AddPolarOffset(radius, Random16(), &position);

  fragment = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  FieldEffect_InitializeFallingFragment(fragment);
}

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

struct PendingEvent;

#define MOVE_CELL_MASK 0xfff00000 ///< a 16-pixel cell

/// When Move is cast on a target, the pushing hand appears beside it and pushes
/// it 16 pixels the way the d-pad points, for as long as the player pushes; A,
/// B, L or R end it.
void FieldPsynergy_SteerPushedTarget(s32 unused) {
  struct Vec3 position;
  struct Vec3 beyond;
  struct FieldEffectWork *scene;
  struct Actor *user;
  struct Actor *target;
  struct Actor *pusher;
  struct Actor *hit;
  s32 x;
  s32 z;
  s32 angle;
  s32 moved;
  s32 direction;
  s32 cellX;
  s32 cellZ;
  s32 (*callback)(void);
  u8 savedLayer;
  u8 *layer;
  struct PendingEvent *event;
  s32 pushed;
  u8 *userFlags;
  const u8 *modes;

  scene = Ram_HeapSlots->fieldEffectWork;
  user = scene->mainActor;
  target = scene->child;
  angle = (u16)scene->angle + TRIG_TURN / 2;
  moved = 0;
  /* FAKEMATCH(temporary): a byte pointer standing in for target->terrainId at
   * the save and the restore. */
  layer = &target->layer;
  savedLayer = *layer;
  pushed = 0;
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
  SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
               target->z.fixed);
  Vector_AddPolarOffset(px(16), angle, &position);
  Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
  FieldEffect_SnapScaleToFull(pusher);
  pusher->speedLimit = px(4);
  pusher->acceleration = Q16_ONE / 2;
  pusher->motionFlags = 4;
  target->update = (void (*)(void))Actor_ApplyRandomChildValues;
  target->speedLimit = px(2) / 5;
  target->acceleration = px(1) / 5;
  target->steeringFlags = moved;
  // Through target rather than layer, which keeps moved's load after the
  target->layer = 2;
  while (1) {
    wait(1);
    // The mode read as a signed byte, which the test against 0 does not change.
    if ((s8)scene->mode == 0 &&
        (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)))
      break;

    direction = (u16)Input_GetDpadAngle(gInput.held);
    // The bytes read the hook's return as the direction; the field is
    // declared void (*)(void) in field_psynergy_work.h, which needs s32.
    callback = (s32 (*)(void))scene->finishCallback;
    if (callback != NULL) {
      direction = callback();
      if (direction == 0xffff)
        break;
    } else if (direction == 0xffff) {
      SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
                   target->z.fixed);
      Vector_AddPolarOffset(px(16), angle, &position);
      Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
      Actor_SelectAnimationFar(pusher, 1);
      pusher->velocityX = 0;
      pusher->velocityY = 0;
      pusher->velocityZ = 0;
      continue;
    }

    SET_POSITION(&position, target->x.fixed, target->y.fixed + px(16),
                 target->z.fixed);
    Vector_AddPolarOffset(px(16), angle, &position);
    Vector_AddPolarOffset(px(2), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y, position.z);
    Actor_WaitForMoveFar(pusher);
    SET_POSITION(&position, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(px(16), direction, &position);
    SET_POSITION(&beyond, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(px(32), direction, &beyond);
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
      cellX = user->x.fixed & MOVE_CELL_MASK;
      cellZ = user->z.fixed & MOVE_CELL_MASK;
      if (cellX == (position.x & MOVE_CELL_MASK) &&
          cellZ == (position.z & MOVE_CELL_MASK)) {
        Actor_SelectAnimationFar(pusher, 4);
        if ((gFrameCount & 15) == 0)
          Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      if (cellX == (beyond.x & MOVE_CELL_MASK) &&
          cellZ == (beyond.z & MOVE_CELL_MASK)) {
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
    Actor_SelectAnimationFar(pusher, modes[(u16)(angle - direction) >> 14]);
    wait(15);
    target->paused = 0;
    target->speedLimit = px(1) / 5;
    target->acceleration = px(1) / 5;
    Actor_SetMoveTargetFar(target, position.x, position.y, position.z);
    pusher->motionFlags = 0;
    pusher->speedLimit = px(1) / 5;
    pusher->acceleration = px(1) / 5;
    Vector_AddPolarOffset(px(16), direction, &position);
    Actor_SetMoveTargetFar(pusher, position.x, position.y + px(16), position.z);
    if (moved == 1) {
      Actor_GetById(scene->firstActorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
      user->speedLimit = px(1) / 5;
      user->acceleration = px(1) / 5;
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

  *layer = savedLayer;
  Actor_SetSpriteColorModeFar(target, scene->savedPalette);
  Actor_StartScriptFar(target, scene->savedScript);
  target->update = (void (*)(void))scene->savedCallback;
  if (pushed) {
    pusher->update = NULL;
    Actor_SelectAnimationFar(pusher, 4);
    Actor_SetPositionAndResetMotionFar(pusher, pusher->x.fixed, pusher->y.fixed,
                                       pusher->z.fixed);
    Field_RunPsynergyEvent(event, scene->firstActorId, scene->secondActorId);
  }
  BattleEffect_StopMainActor();
  if (moved == 1) {
    /* FAKEMATCH(temporary): a byte pointer standing in for the actor's
     * actionFlags. */
    userFlags = &Actor_GetById(scene->firstActorId)->steeringFlags;
    *userFlags |= 1;
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
    Vector_AddPolarOffset(px(30), (u16)angle, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = px(4);
    effect->maxSpeed = px(4);
    effect->limitTurnStep = state;
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
    effect->maxTurnStep = TRIG_TURN / 64;
    effect->limitTurnStep = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}
#undef MOVE_CELL_MASK

/// When Move is cast with no target, the hand appears beside the caster for 30
/// frames and then turns into particles.
void FieldPsynergy_Move(void) {
  // Unused: gives the stack frame its 12 bytes.
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
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->facing + TRIG_TURN / 8) & TRIG_TURN * 3 / 4;
  parent = Actor_Spawn(SPRITE_MOVE_HAND, source->x.fixed,
                       source->y.fixed + px(16), source->z.fixed);
  if (parent == 0)
    return 0;
  parent->scaleY = Q16_ONE / 4;
  parent->scaleX = Q16_ONE / 4;
  parent->update = FieldEffect_UpdateMoveHand;
  parent->speedLimit = px(2);
  parent->acceleration = px(2);
  parent->motionFlags = 0;
  Actor_SelectAnimationFar(parent, 3);
  Actor_SetTargetPositionFromMagnitudeAngle(parent, px(16), angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn(SPRITE_SPARKLE, source->x.fixed,
                        source->y.fixed + px(16), source->z.fixed);
    if (child != 0) {
      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16() + Q16_ONE;
      child->acceleration = px(1);
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = px(1) / 50;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + px(8);
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
  s32 next;
  s32 scale;

  if (actor != NULL) {
    scale = actor->scaleX;
    if (scale < Q16_ONE) {
      do {
        next = scale + Q16_ONE / 16;
        scale = next;
      } while (next < Q16_ONE);
      actor->scaleX = next;
      actor->scaleY = next;
    }
    Actor_WaitForMoveFar(actor);
  }
}

/// The object spins as it rises for 31 frames, shrinks, and bursts into 8
/// particles (object 673).
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(SE_CAST);
  for (i = 30; i >= 0; i--) {
    src->y.fixed += px(1);
    src->facing += TRIG_TURN / 8; // 1/8 turn
    src->scaleX += -Q16_ONE / 32;
    src->scaleY += -Q16_ONE / 32;
    wait(1);
  }
  for (i = 7; i >= 0; i--) {
    child =
        Actor_Spawn(SPRITE_SPARKLE, src->x.fixed, src->y.fixed, src->z.fixed);
    if (child != 0) {
      s32 speed;
      // One 1.0 for both uses, set before the block's calls; a literal at
      // each use differs.
      s32 unit = 0x10000;

      Actor_StartScriptFar(child, gFieldEffect_FragmentScript);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = px(1) / 25;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += px(8);
      Actor_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_ReleaseFar(src);
}

/// Reveal's colour for the rocks it shows (sprite 200), given at its start
/// and taken back at its end. Only the loop over the 64 actors is left; value
/// goes unused.
void Actor_SetRevealSpriteColor(s32 value) {
  s32 remainingActors = 63;

  // DEAD: the loop over the actors, its body gone; the ROM keeps its count
  // and its branch.
loop:
  remainingActors--;
  if (remainingActors >= 0)
    goto loop;
}

/// As The Broken Seal's, with this game's offsets: counts the timer down and,
/// once the leader is 60 pixels from the centre or the timer has run out,
/// requests the effect's end.
void FieldPsynergy_WatchLeaderDistance(void) {
  struct FieldWork *work;
  struct Actor *actor;
  s32 distanceX;
  s32 distanceZ;

  work = gFieldWork;
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  if (work->effectTimerRunning != 0) {
    if (work->revealFrames != 0)
      work->revealFrames--;
  }

  // On screen pixels (y is z less the height). x is scaled by 0xd105 (about
  // 0.82) for the comparison, so the edge is an ellipse 60 pixels up and
  // down from the centre and about 73 to either side.
  distanceX =
      Iwram_MulQ16(work->revealCenterX - actor->x.fixed / Q16_ONE, 0xd105);
  distanceZ = work->revealCenterY - (actor->z.fixed - actor->y.fixed) / Q16_ONE;
  if (distanceX * distanceX + distanceZ * distanceZ >= 3600 ||
      work->revealFrames == 0) {
    // Raise Reveal as a Psynergy request that pays no PP (ACT_PACKED
    // | ABILITY_REVEAL), which runs its ending events.
    u16 *slot = (u16 *)&work->psynergy;
    u32 request = ABILITY_REVEAL | ACT_PACKED;

    *slot = request;
  }
}

#define REVEAL_FRAMES 600

/// Reveal: open a circle of the transition on the caster, wash the screen
/// plain, run the Reveal event that matches, then show the markers nearby
/// and watch how far the leader walks.
void FieldPsynergy_Reveal(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event;
  struct ScreenTransitionState *transition;
  struct Actor *caster;
  void *pending;
  s32 frame;

  Actor_SetRevealSpriteColor(6);
  event = Ram_HeapSlots->fieldWork;
  ScreenTransition_InitializeBattleEffectState(FIELD_PSYNERGY_REVEAL);
  transition = Ram_HeapSlots->screenTransition;
  caster = scene->mainActor;
  transition->centreX = caster->x.fixed;
  transition->centreY = caster->z.fixed - caster->y.fixed;
  PaletteFade_SetSource(Q16_ONE, 0);
  PaletteFade_SetTarget(0x10001, 1);
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
  event->revealCenterX = transition->centreX / Q16_ONE;
  event->revealCenterY = transition->centreY / Q16_ONE;
  event->revealFrames = REVEAL_FRAMES;
  event->effectTimerRunning = 1;
  FieldEffect_SpawnRevealMarkers();
  frame = 0;
  do {
    wait(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  if ((s8)scene->mode == 0)
    Task_Add(FieldPsynergy_WatchLeaderDistance, TASK_PASS_VBLANK);
}

/// Ends Reveal if it is running: stops its watch, brings the colours back from
/// black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES event.
void FieldPsynergy_EndReveal(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct ScreenTransitionState *transition = Ram_HeapSlots->screenTransition;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  void *pending;

  if (event->revealActive != 0) {
    Sound_PlayFar(SE_MAP_TRANSITION);
    Task_Remove(FieldPsynergy_WatchLeaderDistance);
    event->revealActive = 0;
    event->revealFrames = 0;
    Actor_SetRevealSpriteColor(0);
    PaletteFade_SetTarget(Q16_ONE, 1);
    PaletteFade_StartInterpolation(1);
    PaletteFade_SetSource(0, 0);
    PaletteFade_SetTarget(Q16_ONE, 0);
    PaletteFade_StartInterpolation(30);
    wait(1);
    pending = Scene_FindPsynergyTrigger(
        PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES), FIELD_PSYNERGY_REVEAL,
        Scene_SelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstActorId,
                             scene->secondActorId);
    if (scene->childOption == 0) {
      transition->nextMode = 0;
      transition->duration = 1;
      transition->step = 1;
      wait(10);
    }
  }
}

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two sprites of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void FieldPsynergy_GrowthOnTarget(s32 unused) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  FieldEffect_SpawnBurstParticleField();
  if (Actor_GetSpriteIdFar(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Actor_GetSpriteIdFar(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Actor_SelectAnimationFar(target, 2);
    Actor_SetSpriteFlagsFar(target, 0);
    target->collisionFlags = 0;
    target->priorityFlags |= 2;
    wait(10);
    Sound_PlayFar(SE_RECOVERY);
    wait(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    struct BurstParticleVector *p = &position;
    s32 n = 4;

    do {
      struct Actor *piece;
      s32 distance;

      p->values[0] = scene->x;
      p->values[2] = scene->z;
      distance = Random16() * 6 + px(4);
      Vector_AddPolarOffset(distance, Random16(), p->values);
      p->values[1] = scene->y;
      piece = Actor_Spawn(BURST_FALL_OBJECT, p->values[0], p->values[1],
                          p->values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, gFieldEffect_HeavyImpactScript);
        piece->motionFlags = 2;
      }
      wait((Random_Below(2)) + 2);
      n--;
    } while (n >= 0);
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
  u8 *state = Ram_HeapSlots->fieldEffectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  FieldEffect_SpawnBurstParticleField();
  Sound_PlayFar(SE_HEAVY_IMPACT);
  p = &position;
  entryCount = 4;
  do {
    void *object;
    s32 randomValue;

    p->values[0] = *(s32 *)(state + 4);
    p->values[2] = *(s32 *)(state + 12);
    randomValue = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(randomValue, Random16(), p->values);
    p->values[1] = *(s32 *)(state + 8);
    object = Actor_Spawn(SPRITE_VINE, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, gFieldEffect_HeavyImpactScript);
      ((struct Actor *)object)->motionFlags = 2;
    }
    wait((Random_Below(2)) + 2);
    entryCount--;
  } while (entryCount >= 0);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

/// Sends 24 particles, one every 2 frames, from in front of the caster to
/// points round the Psynergy's position.
void FieldEffect_SpawnBurstParticleField(void) {
  struct FieldEffectWork *work;
  struct Actor *user;
  s32 pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->fieldEffectWork;
  user = work->mainActor;
  FieldPsynergy_FadeToCastColor();

  p = pos;
  for (n = 23; n >= 0; n--) {
    struct Actor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (*(u16 *)&work->angle == TRIG_TURN / 4) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + px(10);
      pos[2] = user->z.fixed;
    } else if (*(u16 *)&work->angle == TRIG_TURN * 3 / 4) {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + px(24);
      pos[2] = user->z.fixed;
    } else {
      pos[0] = user->x.fixed;
      pos[1] = user->y.fixed + px(10);
      pos[2] = user->z.fixed;
      Vector_AddPolarOffset(px(10), *(u16 *)&work->angle, pos);
    }

    obj = Actor_Spawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsBHigh = visual->flagsBHigh;
    child->flagsAHigh = visual->flagsAHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      obj->scaleY = 0xb333;
      obj->scaleX = 0xb333;
      obj->acceleration = px(3) / 2;
      obj->speedLimit = px(3) / 2;
      obj->motionFlags = 0;
      Actor_SetSpriteColorModeFar(obj, SPRITE_COLOR_GREEN_TINT);
      Actor_SelectAnimationFar(obj, 7);
      Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript + 1);

      Actor_SetSpriteFlagsFar(obj, 1);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == TRIG_TURN * 3 / 4)
        Vector_AddPolarOffset(px(14), *(u16 *)&work->angle, p);
      dist = Random16() * 6 + px(4);
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
  s32 position[3];
  s32 x, y, z;
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
  y = effectWork->y + px(16);
  position[1] = y;
  z = effectWork->z;
  position[2] = z;
  anchors[0] = FieldEffect_SpawnLiftHand(x + px(32), y, z, FACING_WEST);
  anchors[1] = FieldEffect_SpawnLiftHand(position[0] - px(32), position[1],
                                         position[2], FACING_EAST);

  wait(15);
  for (index = 0; index < 2; index++) {
    void *anchor = anchors[index];
    if (anchor != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(
          anchor, px(14), ((struct Actor *)anchor)->facing);
  }

  Actor_WaitForMoveFar(anchors[0]);
  ((struct EffectChild *)child)->callback =
      (void (*)(void))Actor_ApplyRandomChildValues;
  Sound_PlayFar(SE_PSYNERGY_START);
  ((struct EffectChild *)child)->flag = 4;

  startY = ((struct EffectChild *)child)->y;
  if (anchors[0] != 0 && anchors[1] != 0 && startY <= startY + px(32)) {
    do {
      ((struct Actor *)anchors[0])->y.fixed += px(1) / 4;
      ((struct Actor *)anchors[1])->y.fixed += px(1) / 4;
      ((struct EffectChild *)child)->y += px(1) / 4;
      wait(1);
    } while (((struct EffectChild *)child)->y <= startY + px(32));
  }

  UpdateRisingParticleBurst(anchors[0]);
  UpdateRisingParticleBurst(anchors[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Lift cast with no target: two hands appear 32 pixels to either side of the
/// spot, close 24 pixels in and meet with a crash that scatters 24 fragments,
/// then vanish.
void FieldPsynergy_Lift(void) {
  struct FieldEffectWork *work;
  struct Actor *obj;
  struct Actor *pair[2];
  struct ScatterPosition pos;
  struct Actor **walk;
  s32 n;
  s32 dist;

  work = Ram_HeapSlots->fieldEffectWork;
  FieldPsynergy_FadeToCastColor();
  SET_POSITION(&pos, work->x, work->y + px(16), work->z);
  pair[0] = FieldEffect_SpawnLiftHand(pos.x + px(32), pos.y, pos.z, 0x8000);
  pair[1] = FieldEffect_SpawnLiftHand(pos.x - px(32), pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Actor_SetTargetPositionFromMagnitudeAngle(obj, px(24), obj->facing);
  }

  Actor_WaitForMoveFar(pair[0]);
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (n = 0; n < 24; n++) {
    SET_POSITION(&pos, work->x, work->y + px(16), work->z);
    obj = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gFieldEffect_FragmentScript);
      obj->speedLimit = Random16() + px(2);
      obj->acceleration = px(2);
      obj->motionFlags = 0;
      dist = Random16() * 24 + px(8);
      Actor_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_ReleaseFar(pair[0]);
  Actor_ReleaseFar(pair[1]);
  FieldPsynergy_FadeFromCastColor();
}

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *FieldEffect_SpawnLiftHand(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(SE_ITEM_BREAK);
  object = Actor_Spawn(SPRITE_MOVE_HAND, x, y, z);
  if (object != NULL) {
    object->scaleX = object->scaleY = Q16_ONE / 4;
    object->acceleration = object->speedLimit = px(3);
    ((struct Sprite *)object->sprite)->image.priority = 0;
    Actor_SelectAnimationFar(object, 3);
    scale = object->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->facing += TRIG_TURN / 8;
        wait(1);
        scale = object->scaleX;
      } while (scale <= 0xffff);
    }
    object->facing = angle;
  }
  return object;
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
    Vector_AddPolarOffset(px(25), (u16)Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = px(3);
    effect->maxSpeed = px(3);
    effect->limitTurnStep = 0;
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
    pos.y = caster->y.fixed + px(16);
    pos.z = caster->z.fixed;
    Vector_AddPolarOffset(px(8), effectWork->angle, &pos);
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(px(4), Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = TRIG_TURN / 32;
    effect->limitTurnStep = 1;
    effect->state++;
    return;
  }

  if (state == OUT_AND_BACK_FLYING_BACK &&
      FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// A heavy impact, as The Broken Seal's: an actor placed below the source
/// for a moment, scattering shards from it.
s32 SpawnHeavyImpactEffect(struct Actor *source) {
  struct Vector3 pos;
  struct Actor *impact;
  struct Actor *fragment;
  s32 fragmentIndex;

  Sound_PlayFar(SE_HEAVY_IMPACT);
  pos.x = source->x.fixed;
  pos.y = source->y.fixed;
  pos.z = source->z.fixed;
  impact = Actor_Spawn(SPRITE_STAR_BURST, pos.x, pos.y - px(32), pos.z);
  if (impact != NULL) {
    impact->motionFlags = 0;
    impact->wait = 20;
    Actor_StartScriptFar(impact, gBattleEffect_SharedParticleScript);
  }

  // Shards: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction.
  for (fragmentIndex = 0; fragmentIndex < 12; fragmentIndex++) {
    fragment = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    if (fragment != NULL) {
      Actor_StartScriptFar(fragment, gFieldEffect_FragmentScript);
      fragment->speedLimit = Random16() + px(1);
      fragment->acceleration = px(1);
      fragment->motionFlags = 0;
      Actor_SetTargetPositionFromMagnitudeAngle(
          fragment, Random16() * 24 + px(8), Random16());
    }
  }
  return 0;
}

/// Runs Force cast on a target, through FieldPsynergy_Force.
void FieldPsynergy_ForceOnTarget(s32 unused) { FieldPsynergy_Force(); }

/// Force: restores the colours the cast changed, and does nothing more.
void FieldPsynergy_Force(void) {
  struct Actor *copies[4];
  struct Vector3 pos;
  struct FieldEffectWork *scene;
  struct Actor *target;
  struct Actor *source;
  struct Actor *object;
  struct Actor *copy;
  struct FieldParticle *slot;
  struct ResourceEntry *resource;
  struct PendingEvent *event;
  s32 scale;
  s32 i;
  u8 marker;

  scene = Ram_HeapSlots->fieldEffectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_PSYNERGY_START);
  slot = scene->particles;

  for (i = 0; i <= 11; i++) {
    source = scene->mainActor;
    SET_POSITION(&pos, source->x.fixed, source->y.fixed + px(16),
                 source->z.fixed);
    Camera_WorldToScreen(&pos);
    FieldParticle_Initialize(slot, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateRadialBurst);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, SPRITE_COLOR_BLUE_TINT);
    slot->scaleY = Q16_ONE * 7 / 10;
    slot->scaleX = Q16_ONE * 7 / 10;
    wait(2);
    slot++;
  }

  source = scene->mainActor;
  SET_POSITION(&pos, source->x.fixed, source->y.fixed + px(16),
               source->z.fixed);
  Vector_AddPolarOffset(px(8), scene->angle, &pos);
  object = Actor_Spawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
  if (object == NULL) {
    FieldPsynergy_FadeFromCastColor();
    return;
  }
  object->scaleY = Q16_ONE / 4;
  object->scaleX = Q16_ONE / 4;
  object->facing = scene->angle;
  object->speedLimit = px(4);
  object->acceleration = px(4);
  object->motionFlags = 0;
  Actor_SelectAnimationFar(object, 5);
  Actor_SetSpriteColorModeFar(object, SPRITE_COLOR_BGR);
  scale = object->scaleX;
  if (scale < Q16_ONE) {
    do {
      scale += 0x500;
      object->scaleY = scale;
      object->scaleX = scale;
      wait(1);
      scale = object->scaleX;
    } while (scale <= 0xffff);
  }
  wait(3);

  resource = NULL;
  for (i = 2; i >= 0; i--) {
    copy = copies[i] = Actor_Spawn(SPRITE_MOVE_HAND, object->x.fixed,
                                   object->y.fixed, object->z.fixed);
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
      copy->facing = scene->angle;
      copy->speedLimit = px(4);
      copy->acceleration = px(4);
      copy->motionFlags = 0;
      Actor_SelectAnimationFar(copy, 5);
      Actor_SetSpriteColorModeFar(copy, SPRITE_COLOR_RBG);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
    }
  }
  marker = resource->vramBlock;

  if (scene->enabled != 0) {
    source = scene->mainActor;
    SET_POSITION(&pos, source->x.fixed, source->y.fixed + px(16),
                 source->z.fixed);
    Vector_AddPolarOffset(px(56), scene->angle, &pos);
  } else {
    SET_POSITION(&pos, scene->x, scene->y + px(16), scene->z);
  }
  Actor_SetMoveTargetFar(object, pos.x, pos.y, pos.z);
  Actor_StartScriptFar(object, gBattleEffect_HeavyImpactScript);
  for (i = 0; i < 3; i++) {
    copy = copies[i];
    if (copy != NULL) {
      wait(3);
      Actor_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
      Actor_StartScriptFar(copy, gBattleEffect_SharedParticleScript + 1);
    }
  }

  for (i = 0; i <= 59 && object->script != NULL; i++)
    wait(1);

  if (target != NULL && scene->childMode == 0) {
    if (scene->childOption != 0)
      target->velocityY = px(8);
    SET_POSITION(&pos, target->x.fixed, target->y.fixed, target->z.fixed);
    Vector_AddPolarOffset(px(16), scene->angle, &pos);
    if (MapGrid_CheckCollisionFar(target, &pos) == 0 &&
        ActorScript_CheckOverlapFar(target, &pos) == 0) {
      target->acceleration = px(1);
      target->speedLimit = px(1);
      Actor_SetMoveTargetFar(target, pos.x, pos.y, pos.z);
    }
  }

  event = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), 4,
                                    Scene_SelectPsynergyTarget(4));
  if (event != NULL)
    Field_RunPsynergyEvent(event, scene->firstActorId, scene->secondActorId);
  wait(10);
  FieldPsynergy_FadeFromCastColor();
  wait(20);
  if (marker != 96)
    VramCache_FreeEntry(marker);
}

struct OrbitingParticle;

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
  s32 scale;

  if (particle != NULL) {
    scale = particle->scaleX - Q16_ONE / 16;
    particle->scaleY -= Q16_ONE / 16;
    particle->scaleX = scale;
    if (scale <= Q16_ONE / 16)
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
      Vector_AddPolarOffset(life << 17, (s16)particle->userValue + (life << 11),
                            &pos);
      SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))FieldEffect_UpdateOrbitingParticleFade;
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
      Vector_AddPolarOffset(life << 17, (s16)particle->userValue - (life << 11),
                            &pos);
      SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
    } else {
      particle->update = (void (*)(void))FieldEffect_UpdateOrbitingParticleFade;
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

/// Sixteen particles, one every three frames, orbit points scattered 2 to 6
/// pixels around the effect; the target then flashes eight times, takes the
/// orbiting particles' main update and runs its pending event.
void FieldPsynergy_Frost(void) {
  struct Vector3 pos;
  struct Vector3 *p;
  struct FieldEffectWork *scene;
  struct Actor *target;
  struct Actor *particle;
  struct PendingEvent *ev;
  s32 cnt;

  scene = Ram_HeapSlots->fieldEffectWork;
  target = scene->child;
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ICE);

  p = &pos;
  cnt = 15;
  do {
    particle = Actor_Spawn(SPRITE_SNOWFLAKE, 0, 0, 0);
    if (particle != NULL) {
      u32 scale;
      s32 dist;

      scale = (Random16() >> 1) + Q16_ONE / 2;
      particle->scaleY = scale;
      particle->scaleX = scale;
      if ((Random16() & 1) != 0)
        particle->update =
            (void (*)(void))FieldEffect_UpdateOrbitingParticleLeft;
      else
        particle->update =
            (void (*)(void))FieldEffect_UpdateOrbitingParticleRight;

      particle->facing = Random16();
      particle->timer = 60;
      particle->userValue = Random16();
      Actor_SetSpriteColorModeFar((struct Actor *)particle,
                                  SPRITE_COLOR_BLUE_TINT);

      SET_POSITION(p, scene->x, scene->y, scene->z);
      dist = (Random16() << 2) + px(2);
      Vector_AddPolarOffset(dist, Random16(), p);
      particle->targetX = p->x;
      particle->targetY = p->y;
      particle->targetZ = p->z;
    }

    wait(3);
    cnt--;
  } while (cnt >= 0);

  wait(10);
  Sound_PlayFar(SE_ICE);
  wait(50);

  if (target != NULL && scene->enabled == 0) {
    Sound_PlayFar(SE_POWER_UP);

    cnt = 7;
    do {
      Actor_SetSpriteColorModeFar((struct Actor *)target, SPRITE_COLOR_WHITE);
      wait(1);
      Actor_SetSpriteColorModeFar((struct Actor *)target, SPRITE_COLOR_NORMAL);
      wait(4);
      cnt--;
    } while (cnt >= 0);

    if (scene->childOption == 0) {
      Sound_PlayFar(SE_SHIMMER);
      Actor_SelectAnimationFar(target, 2);
    }

    target->update = (void (*)(void))FieldEffect_UpdateFrostGlint;
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
  s32 amplitude = px(4);

  actor->x.fixed =
      actor->targetX + Iwram_MulQ16(amplitude, Trig_Sin(actor->timer << 9));
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
      // The angle turns 1/32 of a circle (TRIG_TURN / 32) each frame left.
      pos.x = effectWork->x;
      pos.y = effectWork->y + px(10);
      pos.z = effectWork->z;
      Vector_AddPolarOffset(px(remainingFrames),
                            (s16)particle->userValue + (remainingFrames << 11),
                            &pos);
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
  s32 pos[3];

  effectWork = Ram_HeapSlots->fieldEffectWork;
  if (emitter->timer != -1) {
    s32 amplitude = px(6);

    SET_ACTOR_POSITION(
        emitter,
        effectWork->x + Iwram_MulQ16(amplitude, Trig_Sin(emitter->timer << 10)),
        effectWork->y + px(16), effectWork->z);
    emitter->timer++;
    emitter->timer = (emitter->timer + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    // From 2 pixels up, 0 to 6 pixels in a random direction.
    pos[0] = emitter->x.fixed;
    pos[1] = emitter->y.fixed + px(2);
    pos[2] = emitter->z.fixed;
    Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
    spark = Actor_Spawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
    // Each falls under a gravity of 458 (0.007 pixels a frame, each frame)
    // and is released after 72 frames, its script's wait (lifetime).
    if (spark != 0) {
      spark->update = (void (*)(void))FieldEffect_SwayParticle;
      spark->scaleX = spark->scaleY = Q16_ONE * 3 / 5;
      spark->motionFlags = ACTOR_FALLS;
      spark->gravity = 458;
      spark->timer = Random16() >> 9;
      spark->targetX = spark->x.fixed;
      Actor_SetSpriteColorModeFar(spark, SPRITE_COLOR_BLUE_TINT);
      spark->wait = 72;
      Actor_StartScriptFar(spark, gBattleEffect_SharedParticleScript);
    }
  }
}

/// Effect 3 cast on a target: runs effect 3, then stops the caster's hook.
void FieldPsynergy_PlyOnTarget(void) {
  FieldPsynergy_Ply();
  BattleEffect_StopMainActor();
}

/// Eight particles spiral in on the main object, one a frame; then a centre
/// object flashes for 60 frames, emits sparks and flies off.
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
                        target->y.fixed + px(32), target->z.fixed);
    if (actor != 0) {
      actor->scaleY = Q16_ONE * 7 / 10;
      actor->scaleX = Q16_ONE * 7 / 10;
      actor->update = (void (*)(void))FieldEffect_UpdateShrinkingOrbitActor;
      actor->timer = 120;
      actor->userValue = i << 13;
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
  actor = target;
  if (target != 0) {
    target->scaleY = Q16_ONE * 7 / 10;
    target->scaleX = Q16_ONE * 7 / 10;
    target->x.fixed = effectWork->x;
    target->y.fixed = effectWork->y + px(16);
    target->z.fixed = effectWork->z;
    target->motionFlags = ACTOR_FLOATS;
    Actor_SetSpriteColorModeFar(target, SPRITE_COLOR_WHITE);
  }

  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  wait(12);
  if (target != 0) {
    // Switch its sprite param every 2 frames (10 one time in 4).
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(dead): target set again to actor at each step; as the loop
       * without it, 123 of 188 instructions differ in tla-en. */
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
  Sound_PlayFar(MUS_CURE);
  if (actor != 0) {
    target->update = (void (*)(void))FieldEffect_RunSparkEmitter;
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
    target->speedLimit = px(5);
    target->acceleration = px(2) / 5;
    target->steeringFlags = 0;
    Actor_SetTargetPositionFromMagnitudeAngle(target, px(192),
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
/// stand on a tile Cloak can hide on, requests CLOAK_END (0x2092).
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
    Task_Add(FieldEffect_CheckCloakTile, TASK_PASS_VBLANK);
}
