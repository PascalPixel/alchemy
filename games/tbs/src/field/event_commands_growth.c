/// @file
/// Growth: green sparks fly from the caster, and a sprout grows tall.
// Part of event_commands.c.

#include "event_commands_internal.h"

#define GROWTH_PARTICLE_MAX 24 ///< particles round the target

/// A point Growth works out round the Psynergy's position.
struct BurstParticleVector {
  s32 values[3]; ///< x, y, z
};

/// A sprite part's OAM attributes (struct AnimationSpritePart) as Growth
/// copies them; each member names the AnimationSpritePart fields it covers.
struct BurstParticleVisual {
  u8 link[4];
  u8 y;
  u8 flagsALow : 5; ///< affine, objectMode and mosaic
  u8 fullColor : 1;
  u8 flagsAHigh : 2; ///< shape
  u8 xLow;
  u8 flagsBLow : 6;  ///< x's top bit and affineIndex
  u8 flagsBHigh : 2; ///< size
  union {
    struct {
      u16 tile : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

/// A particle's sprite, seen as its two parts' attributes, which Growth copies
/// from the first to the second: the image and the shadow
/// (Sprite.image and shadow).
struct BurstParticleVisualGroup {
  struct BurstParticleVisual image;
  struct BurstParticleVisual shadow;
};

/// When Growth is cast on a target, particles fly from in front of the caster
/// to points round the target, which then takes the sprite priority of the
/// ground under it.
void FieldPsynergy_GrowthOnTarget(s32 unused) {
  struct Actor *target;

  target = ((struct FieldEffectWork *)gFieldEffectWork)->child;
  if (target != NULL) {
    FieldEffect_SpawnBurstParticleField();
    // A sprout grows tall: sprout.tsv's animation 2, pictures 0 to 6.
    Actor_SelectAnimationFar(target, 2);
    target->collisionFlags = 0;
    Actor_SetSpriteFlagsFar(target, 0);
    target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    wait(10);
    Sound_PlayFar(SE_RECOVERY);
    wait(40);
    FieldPsynergy_FadeFromCastColor();
  }
}

/// When Growth is cast with no target, particles fly out from in front of the
/// caster, and then five bits of vine (SPRITE_VINE) fall 4 to 10 pixels round
/// the Psynergy's position.
void FieldPsynergy_Growth(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct BurstParticleVector pos;
  s32 i;

  FieldEffect_SpawnBurstParticleField();
  Sound_PlayFar(SE_HEAVY_IMPACT);
  for (i = 0; i < 5; i++) {
    struct Actor *particle;
    FIXED radius;

    pos.values[0] = effectWork->x;
    pos.values[2] = effectWork->z;
    radius = (Random16() * 6) + toFIXED(4);
    Vector_AddPolarOffset(radius, Random16(), &pos);
    pos.values[1] = effectWork->y;
    particle =
        Actor_Spawn(SPRITE_VINE, pos.values[0], pos.values[1], pos.values[2]);
    if (particle != 0) {
      Actor_StartScriptFar(particle, gFieldEffect_BurstParticleActorScript);
      particle->motionFlags = ACTOR_FALLS;
    }
    wait(((Random16() * 2) >> 16) + 2);
  }
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

/// Sends GROWTH_PARTICLE_MAX green sparks, one every 2 frames, from in front of
/// the caster to points round the Psynergy's position.
void FieldEffect_SpawnBurstParticleField(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *caster;
  s32 pos[3];
  s32 particleIndex;

  effectWork = gFieldEffectWork;
  caster = effectWork->mainActor;
  FieldPsynergy_FadeToCastColor();

  for (particleIndex = 0; particleIndex < GROWTH_PARTICLE_MAX;
       particleIndex++) {
    struct Actor *particle;
    struct BurstParticleVisual *image;
    struct BurstParticleVisual *shadow;
    FIXED radius;

    if (effectWork->angle == FACING_SOUTH) {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(10),
                 caster->z.fixed);
    } else if (effectWork->angle == FACING_NORTH) {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(24),
                 caster->z.fixed);
    } else {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + toFIXED(10),
                 caster->z.fixed);
      Vector_AddPolarOffset(toFIXED(10), effectWork->angle, pos);
    }

    // The shadow part takes the image's colour mode, shape, size, tile and
    // palette, so the shadow (turned on by SPRITE_FLAG_SHADOW below) is drawn
    // with the particle's picture. BUG: the copy comes before the NULL test, so
    // a failed spawn (a full gActors) is read through.
    particle = Actor_Spawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
    image = &((struct BurstParticleVisualGroup *)particle->sprite)->image;
    shadow = &((struct BurstParticleVisualGroup *)particle->sprite)->shadow;
    shadow->fullColor = image->fullColor;
    shadow->flagsAHigh = image->flagsAHigh;
    shadow->flagsBHigh = image->flagsBHigh;
    shadow->attributes.bits.tile = image->attributes.bits.tile;
    shadow->attributes.bytes.palette = image->attributes.bytes.palette;

    if (particle != 0) {
      particle->scaleY = FIXED_ONE * 7 / 10;
      particle->scaleX = FIXED_ONE * 7 / 10;
      particle->acceleration = toFIXED(3) / 2;
      particle->speedLimit = toFIXED(3) / 2;
      particle->motionFlags = 0;
      Actor_SetSpriteColorModeFar(particle, SPRITE_COLOR_GREEN_TINT);
      Actor_SelectAnimationFar(particle, 7);
      // From the shared script's SC_SYNC: released once it arrives.
      Actor_StartScriptFar(particle, gFieldEffect_SharedParticleScript + 1);
      Actor_SetSpriteFlagsFar(particle, SPRITE_FLAG_SHADOW);

      // Destination: 4 to 10 pixels from the Psynergy's position.
      SET_VECTOR(pos, effectWork->x, effectWork->y, effectWork->z);
      if (effectWork->angle == FACING_NORTH)
        Vector_AddPolarOffset(toFIXED(14), effectWork->angle, pos);
      radius = Random16() * 6 + toFIXED(4);
      Vector_AddPolarOffset(radius, Random16(), pos);
      Actor_SetMoveTargetFar(particle, pos[0], pos[1], pos[2]);
    }
    Sound_PlayFar(SE_PSYNERGY_SPARKLE);
    wait(2);
  }
  wait(8);
}

#undef GROWTH_PARTICLE_MAX
