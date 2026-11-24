/// @file
/// Retreat, the caster spinning away in a spray of sparks, and the golden
/// rings that fall round the caster of Avoid.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Per-frame update: spins by FACING_STEP + timer * 80 a frame, speeding up
/// (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a
/// frame to just over 1/8 in 52 frames, as FieldEffect_AdvanceAngleWithRamp.
void FieldEffect_UpdateSpinAngle(struct Actor *actor) {
  u32 step;

  step = actor->timer * 80;
  actor->facing = actor->facing + step + FACING_STEP;
  if (step < FACING_STEP)
    actor->timer = actor->timer + 1;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks by 5/512 of its full
/// size, rising a pixel a frame; once its scale falls below 3/16 (84 frames
/// from full size) its sprites are hidden, but it is not released. Retreat
/// gives it to the caster.
void FieldEffect_UpdateRetreatCaster(struct Actor *actor) {
  u32 scale;

  actor->scaleY -= FIXED_ONE * 5 / 512;
  scale = actor->scaleX - FIXED_ONE * 5 / 512;
  actor->facing += DEGtoANG(45);
  actor->y.fixed += toFIXED(1);
  actor->scaleX = scale;
  if ((s32)scale < FIXED_ONE * 3 / 16)
    actor->spriteStorageKind = ACTOR_SPRITE_NONE;
}

/// The states of a Retreat particle (Retreat_UpdateSpark).
enum RetreatParticleState {
  RETREAT_PARTICLE_AIM = 0,    ///< placed on the main actor and aimed
  RETREAT_PARTICLE_FLYING = 1, ///< back to RETREAT_PARTICLE_AIM on arrival
  /// Set by FieldPsynergy_Retreat: freed on arrival.
  RETREAT_PARTICLE_LAST_FLIGHT = 2
};

/// Field effect sprite callback: a particle flies 120 pixels from the main
/// actor's screen position (8 pixels lower) in a direction within 1/8 turn of
/// up the screen, at up to 5 pixels a frame, and starts again from the main
/// actor on arrival until it takes its last flight.
void Retreat_UpdateSpark(struct FieldParticle *effect) {
  struct Vector3 pos;
  struct Actor *main;
  s32 state;
  u32 spread;

  main = ((struct FieldEffectState *)gFieldEffectState)->caster;
  state = effect->state;

  if (state == RETREAT_PARTICLE_AIM) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    effect->x = pos.x;
    effect->z = pos.z + toFIXED(8);
    effect->originZ = effect->z;
    effect->originX = effect->x;
    pos.z = effect->originZ;
    pos.x = effect->originX;

    // Up the screen, within 1/8 turn either way.
    spread = Random16();
    Vector_AddPolarOffset(toFIXED(120),
                          FIXEDtoINT(spread * (TRIG_TURN / 8)) -
                              Random16_Below(TRIG_TURN / 8) + FACING_NORTH,
                          &pos);

    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = toFIXED(5);
    effect->speedMax = toFIXED(5);
    effect->turnStepLimited = state;
    effect->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(SE_GLITTER);
  } else if (state == RETREAT_PARTICLE_FLYING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == RETREAT_PARTICLE_LAST_FLIGHT) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}

/// Spins the main actor and shrinks it while 24 particles fly out, one a frame.
void FieldPsynergy_Retreat(void) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *actor = effectState->caster;
  struct Vector3 pos;
  struct FieldParticle *effect;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_FaceDirectionById(effectState->casterId, FACING_SOUTH, 0);
  wait(30);
  actor->update = Actor_CycleSpriteColors;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  Actor_SelectAnimationFar(actor, ANIM_LEADER_ARMS_RAISED);
  wait(40);

  // Start spinning.
  Sound_PlayFar(SE_SHIMMER);
  Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  Actor_SelectAnimationFar(actor, 3);
  actor->update = FieldEffect_UpdateSpinAngle;
  actor->timer = 0;
  wait(70);

  // Start shrinking, and send particles from where it is on screen.
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->motionFlags = 0;
  actor->update = FieldEffect_UpdateRetreatCaster;
  actor->targetX = ACTOR_NO_TARGET;
  pos.x = actor->x.fixed;
  pos.y = actor->y.fixed;
  pos.z = actor->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  effect = effectState->particles;
  do {
    FIXED scale;
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, Retreat_UpdateSpark);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, Random16_Below(7));
    scale = (Random16() >> 1) + FIXED_ONE * 6 / 5; // 1.2 to 1.7
    effect->scaleY = scale;
    effect->scaleX = scale;
    i++;
    wait(1);
    effect++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  wait(70);

  // The flying particles vanish once they arrive.
  for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
    if (effectState->particles[i].active != 0)
      effectState->particles[i].state = RETREAT_PARTICLE_LAST_FLIGHT;
  }
  wait(40);
  FieldPsynergy_FadeFromCastColor();
  wait(10);
}

/// Per-frame update of a falling arc: grows toward full size (3/64 a frame)
/// as it falls 2 pixels a frame on the actor it follows, 9 pixels in front
/// of it and up to 5 pixels further while small; released 10 pixels above
/// the ground (4 on the World Map, MAP_INDEX_WORLD).
void FieldEffect_UpdateDescendingParticlePositiveArc(struct Actor *particle) {
  struct Actor *source;
  FIXED y;
  FIXED limit;
  FIXED scale;

  limit = particle->groundHeight + toFIXED(10);
  source = particle->watchedActor;
  if (gMainRoutineState.mapId == MAP_INDEX_WORLD)
    limit = particle->groundHeight + toFIXED(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + FIXED_ONE * 3 / 64;
  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - toFIXED(2);
  particle->z.fixed = source->z.fixed + ((FIXED_ONE - scale) * 5) + toFIXED(9);
}

/// As FieldEffect_UpdateDescendingParticlePositiveArc for the arc drawn upside
/// down, 16 pixels in front of the actor and up to 5 pixels nearer.
void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
  struct Actor *source;
  FIXED y;
  FIXED limit;
  FIXED scale;

  limit = particle->groundHeight + toFIXED(10);
  source = particle->watchedActor;
  if (gMainRoutineState.mapId == MAP_INDEX_WORLD)
    limit = particle->groundHeight + toFIXED(4);

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + FIXED_ONE * 3 / 64;
  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - toFIXED(2);
  particle->z.fixed = source->z.fixed - ((FIXED_ONE - scale) * 5) + toFIXED(16);
}

/// A falling arc's sprite (struct Sprite), seen as the image attributes, VRAM
/// cache entry and first layer that Avoid's arcs set on it, under struct
/// Sprite's names; its image bit fields are read here as bytes.
struct DescendingArcSprite {
  u8 link[4];
  u8 y;
  u8 affine : 2;
  u8 objectMode : 2;
  u8 mosaic : 1;
  u8 fullColor : 1; ///< 0 for 16 colours
  u8 shape : 2;     ///< 1 wide
  u8 xLow;
  u8 xHigh : 1;
  u8 affineIndex : 5;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10; ///< the first sprite tile in VRAM
      u16 priority : 2;
      u16 palette : 4;
    } half;
    struct {
      u8 tileLow;
      u8 tileHigh : 2;
      u8 priority : 2;
      u8 palette : 4;
    } byte;
  } attr;
  struct AnimationSpritePart shadow;
  s32 scale;
  u8 slot;                 ///< the VRAM cache entry holding its tiles
  u8 retainCacheEntry : 1; ///< the borrowed tiles stay loaded
  u8 useOwnScale : 1;
  u8 unusedFlags : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags; ///< 0 drops the shadow
  u8 count;
  struct SpriteAnimation *firstEntry; ///< entries[0], its first layer
};

/// Avoid's rings: a pair of falling arcs (one upside down) 64 pixels above
/// source, at 0.4 of full size, drawn with the tiles in
/// effectState->arcTileSlot.
void FieldEffect_SpawnDescendingArcParticles(struct Actor *source) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *leader = effectState->caster;
  struct Actor *actors[2];
  struct Actor *actor;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    actor = Actor_CreateFar(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed + toFIXED(64), source->z.fixed);
    actors[arcIndex] = actor;
    if (actor == NULL)
      continue;
    actor->groundHeight = source->groundHeight;
    sprite = (struct DescendingArcSprite *)actor->sprite;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->watchedActor = source;
    actor->scaleY = FIXED_ONE * 2 / 5;
    actor->scaleX = FIXED_ONE * 2 / 5;
    if (sprite == NULL)
      continue;
    Sprite_StartAnimationFar((struct Sprite *)sprite, 0);
    sprite->flags = 0;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectState->arcTileSlot;
    sprite->retainCacheEntry = 1;
    sprite->attr.half.tile = gVramCacheEntries[sprite->slot].offset >> 5;
    sprite->fullColor = 0;
    sprite->shape = 1;
    sprite->size = 2;
    sprite->firstEntry->picture = 0;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  actors[0]->update = FieldEffect_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)actors[0]->sprite)->attr.byte.priority = 0;
  actors[1]->update = FieldEffect_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)actors[1]->sprite)->attr.byte.priority =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.priority;
}
