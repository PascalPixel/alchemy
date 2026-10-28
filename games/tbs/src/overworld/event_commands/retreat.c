/// @file
/// Retreat.
// Part of event_commands.c.

#include "../event_commands_internal.h"

// The event owns PSYNERGY_EFFECT_SLOT_COUNT particle slots.

/// Per-frame update: spins by 0x1000 + timer * 0x50 a frame, speeding up
/// (timer counts up) until timer * 0x50 reaches 0x1000: from 1/16 turn a
/// frame to just over 1/8 in 52 frames, as FieldEffect_AdvanceAngleWithRamp.
void FieldEffect_UpdateSpinAngle(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  u32 step;

  step = actor->timer * 0x50;
  actor->facing = actor->facing + step + 0x1000;
  if (step < 0x1000)
    actor->timer = actor->timer + 1;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks by 0x280, rising a
/// pixel a frame; once its scale falls below 0x3000 (84 frames from full
/// size) its sprites are hidden (spriteStorageKind 0), but it is not
/// released. Retreat gives it to the caster.
void FieldEffect_UpdateDescendingActor(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  u32 scale;

  actor->scaleY -= 0x280;
  scale = actor->scaleX - 0x280;
  actor->facing += 0x2000;
  actor->y.fixed += 1 << 16;
  actor->scaleX = scale;
  if ((s32)scale < 0x3000)
    actor->spriteStorageKind = ACTOR_SPRITE_NONE;
}

/// The states of a Retreat particle (FieldEffect_UpdateRadialLaunch).
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
void FieldEffect_UpdateRadialLaunch(struct FieldParticle *effect) {
  struct Vector3 pos;
  struct Actor *main;
  s32 state;
  u32 spread;

  main = ((struct FieldEffectWork *)gFieldEffectWork)->mainActor;
  state = effect->state;

  if (state == RETREAT_PARTICLE_AIM) {
    pos.x = main->x.fixed;
    pos.y = main->y.fixed;
    pos.z = main->z.fixed;
    Camera_WorldToScreen(&pos);

    effect->x = pos.x;
    effect->z = pos.z + (8 << 16);
    effect->originZ = effect->z;
    effect->originX = effect->x;
    pos.z = effect->originZ;
    pos.x = effect->originX;

    spread = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((spread << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 5 << 16;
    effect->speedMax = 5 << 16;
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

/// Spins the main actor and shrinks it while 24 particles fly out, one a
/// frame.
///
/// Abilities: Retreat (FIELD_PSYNERGY_RETREAT).
///
/// The caster turns south, plays ANIM_LEADER_ARMS_RAISED with its colours
/// cycling, spins up for 70 frames (FieldEffect_UpdateSpinAngle), then rises
/// and shrinks out of sight (FieldEffect_UpdateDescendingActor) as the
/// particles fly. It is hidden, not released, and the Psynergy opens an event
/// (Event_Start) it does not end: the Retreat exit raised before it
/// (EXIT_KEEP_DESTINATION, by Field_UsePsynergy or Field_UseItem) then leaves
/// the scene. Called by FieldPsynergy_Run and FieldPsynergy_Dispatch.
void FieldPsynergy_Retreat(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *actor = effectWork->mainActor;
  struct Vector3 pos;
  struct FieldParticle *effect;
  u8 last;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_ArmCallback(effectWork->firstActorId, FACING_SOUTH, 0);
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
  actor->update = FieldEffect_UpdateDescendingActor;
  actor->targetX = ACTOR_NO_TARGET;
  pos.x = actor->x.fixed;
  pos.y = actor->y.fixed;
  pos.z = actor->z.fixed;
  Camera_WorldToScreen(&pos);
  i = 0;
  effect = effectWork->particles;
  do {
    s32 scale;
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, pos.x, pos.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRadialLaunch);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333; // 1.2 to 1.7
    effect->scaleY = scale;
    effect->scaleX = scale;
    i++;
    wait(1);
    effect++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  wait(70);

  // The flying particles vanish once they arrive.
  i = 0;
  last = RETREAT_PARTICLE_LAST_FLIGHT;
  do {
    if (effectWork->particles[i].active != 0)
      effectWork->particles[i].state = last;
    i++;
  } while (i < PSYNERGY_EFFECT_SLOT_COUNT);
  wait(40);
  FieldPsynergy_FadeFromCastColor();
  wait(10);
}

/// Per-frame update of a falling arc: grows toward full size (0xc00 a frame)
/// as it falls 2 pixels a frame on the actor it follows, 9 pixels in front
/// of it and up to 5 pixels further while small; released 10 pixels above
/// the ground (4 on the World Map, MAP_INDEX_WORLD).
void FieldEffect_UpdateDescendingParticlePositiveArc(struct Actor *actor) {
  // One shared inline kernel with constant inversion grows the positive and
  // negative update routines 0x74 -> 0x7c and 0x78 -> 0x7c in native English.
  struct Actor *particle = (struct Actor *)actor;
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->groundHeight + 0xA0000;
  source = particle->watchedActor;
  if (gSequenceParentWork.mapId == MAP_INDEX_WORLD)
    limit = particle->groundHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed + ((Q16_ONE - scale) * 5) + 0x90000;
}

/// As FieldEffect_UpdateDescendingParticlePositiveArc for the arc drawn upside
/// down, 16 pixels in front of the actor and up to 5 pixels nearer.
void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *actor) {
  struct Actor *particle = (struct Actor *)actor;
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;

  limit = particle->groundHeight + 0xA0000;
  source = particle->watchedActor;
  if (gSequenceParentWork.mapId == MAP_INDEX_WORLD)
    limit = particle->groundHeight + 0x40000;

  y = particle->y.fixed;
  if (y <= limit) {
    Actor_ReleaseFar(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x.fixed = source->x.fixed;
  particle->y.fixed = y - 0x20000;
  particle->z.fixed = source->z.fixed - ((Q16_ONE - scale) * 5) + 0x100000;
}

/// A falling arc's sprite (struct Sprite), seen as the image attributes, VRAM
/// cache entry and first layer that Avoid's arcs set on it, under struct
/// Sprite's names; its image bit fields are read here as bytes.
struct DescendingArcSprite {
  u8 padding00[5];
  u8 affine : 2;
  u8 objectMode : 2;
  u8 mosaic : 1;
  u8 fullColor : 1; ///< 0 for 16 colours
  u8 shape : 2;     ///< 1 wide
  u8 padding06;
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
  u8 padding0c[16];
  u8 slot;                 ///< the VRAM cache entry holding its tiles
  u8 retainCacheEntry : 1; ///< the borrowed tiles stay loaded
  u8 useOwnScale : 1;
  u8 unusedFlags : 6;
  u8 padding1e[8];
  u8 flags; ///< 0 drops the shadow
  u8 count;
  struct SpriteAnimation *firstEntry; ///< entries[0], its first layer
};

/// Spawns a pair of falling arcs (one upside down) on arc, 64 pixels above
/// it, at 0.4 of full size, drawn with the tiles in effectWork->arcTileSlot.
/// The upside-down arc draws in front of everything (priority 0), the other at
/// the caster's priority, as FieldEffect_SpawnScaledArcActors does. Called by
/// FieldEffect_UpdatePairedArcSpawner, so arc is Avoid's caster.
void FieldEffect_SpawnDescendingArcParticles(struct Actor *arc) {
  struct Actor *source = arc;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *leader = effectWork->mainActor;
  struct Actor *actors[2];
  struct Actor *actor;
  struct DescendingArcSprite *sprite;
  s32 arcIndex;
  u8 zero;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    actor = Actor_CreateFar(SPRITE_GOLDEN_RINGS, source->x.fixed,
                            source->y.fixed + 0x400000, source->z.fixed);
    actors[arcIndex] = actor;
    if (actor == NULL)
      continue;
    actor->groundHeight = source->groundHeight;
    sprite = (struct DescendingArcSprite *)actor->sprite;
    actor->motionFlags = 0;
    actor->timer = 0;
    zero = 0;
    actor->watchedActor = source;
    actor->scaleY = 0x6666;
    actor->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    Sprite_StartAnimationFar((struct Sprite *)sprite, 0);
    sprite->flags = zero;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectWork->arcTileSlot;
    sprite->retainCacheEntry = 1;
    sprite->attr.half.tile = gVramCacheEntries[sprite->slot].offset >> 5;
    sprite->fullColor = 0;
    sprite->shape = 1;
    sprite->size = 2;
    sprite->firstEntry->picture = zero;
  }
  // BUG: failed allocations above leave a missing arc or sprite here.
  // Preserve the original partial-pair behavior until recovery is established.
  actors[0]->update = FieldEffect_UpdateDescendingParticleNegativeArc;
  ((struct DescendingArcSprite *)actors[0]->sprite)->attr.byte.priority = 0;
  actors[1]->update = FieldEffect_UpdateDescendingParticlePositiveArc;
  ((struct DescendingArcSprite *)actors[1]->sprite)->attr.byte.priority =
      ((struct DescendingArcSprite *)leader->sprite)->attr.byte.priority;
}
