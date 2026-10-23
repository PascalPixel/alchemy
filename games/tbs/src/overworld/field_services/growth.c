/// @file
/// Growth.
// Part of field_services.c.

#include "../field_services_internal.h"

#define GROWTH_PARTICLE_MAX 24 ///< particles round the target
#define GROWTH_FALL_OBJECT 217 ///< falling object
#define GROWTH_FALL_LAST 4     ///< five falling objects, 0 to 4

/// A point Growth works out round the effect's position.
struct BurstParticleVector {
  s32 values[3]; ///< x, y, z
};

/// A sprite part's OAM attributes (struct AnimationSpritePart) as Growth
/// copies them; the notes give the AnimationSpritePart fields, some under
/// other names here.
struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;  ///< affine, objectMode and mosaic
  u8 flip : 1;       ///< fullColor, despite the name
  u8 flagsAHigh : 2; ///< shape
  u8 reserved06;
  u8 flagsBLow : 6;  ///< x's top bit and affineIndex
  u8 flagsBHigh : 2; ///< size
  union {
    struct {
      u16 shape : 10; ///< tile, despite the name
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
  struct BurstParticleVisual primary;
  struct BurstParticleVisual child;
};

/// When Growth is cast on a target, particles fly from in front of the caster
/// to points round the target, which then takes the sprite priority of the
/// ground under it.
///
/// Abilities: Growth (FIELD_PSYNERGY_GROWTH).
///
/// The target plays animation 2, stops being solid,
/// loses its shadow and is depth-sorted as if 320 pixels higher
/// (OBJ_EVENT_PRIORITY_UNDERFOOT); the effect leaves it so.
void FieldEffect_RunGrowthOnTarget(s32 unused) {
  struct ObjectEvent *target;

  target = ((struct FieldEffectWork *)gFieldEffectWork)->child;
  if (target != NULL) {
    FieldEffect_SpawnBurstParticleField();
    ObjectDispatch_SelectAnimationFar(target, 2);
    target->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)target, 0);
    target->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    WaitFrames(10);
    Sound_PlaySongFar(SE_RECOVERY);
    WaitFrames(40);
    FieldEffect_FadeFromEffectColor();
  }
}

/// When Growth is cast with no target, particles fly out from in front of the
/// caster, and then five falling objects appear 4 to 10 pixels round the
/// effect's position.
///
/// Abilities: Growth (FIELD_PSYNERGY_GROWTH).
///
/// The objects (217) appear 2 or 3 frames apart and each lasts 60 frames
/// (gFieldEffect_BurstParticleObjectScript).
void FieldEffect_RunGrowth(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct BurstParticleVector pos;
  struct BurstParticleVector *p;
  s32 remainingObjects;

  FieldEffect_SpawnBurstParticleField();
  Sound_PlaySongFar(SE_HEAVY_IMPACT);
  p = &pos;
  remainingObjects = GROWTH_FALL_LAST;
  do {
    struct ObjectEvent *particle;
    s32 radius;

    p->values[0] = effectWork->x;
    p->values[2] = effectWork->z;
    radius = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(radius, Random16(), p);
    p->values[1] = effectWork->y;
    particle = (struct ObjectEvent *)Object_Spawn(
        GROWTH_FALL_OBJECT, p->values[0], p->values[1], p->values[2]);
    if (particle != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gFieldEffect_BurstParticleObjectScript);
      particle->motionFlags = OBJ_EVENT_FALLS;
    }
    WaitFrames(((Random16() * 2) >> 16) + 2);
    remainingObjects--;
  } while (remainingObjects >= 0);
  WaitFrames(30);
  FieldEffect_FadeFromEffectColor();
}

/// Sends 24 particles, one every 2 frames, from in front of the caster to
/// points round the effect's position. They start 10 pixels up facing south,
/// 24 pixels up facing north, and 10 pixels up and 10 ahead otherwise. Each
/// flies at 1.5 pixels a frame and is freed on arrival; facing north, the
/// points are 14 pixels further north.
void FieldEffect_SpawnBurstParticleField(void) {
  struct FieldEffectWork *effectWork;
  struct ObjectEvent *caster;
  s32 pos[3];
  s32 particleIndex;

  effectWork = gFieldEffectWork;
  caster = effectWork->mainObject;
  FieldEffect_FadeToEffectColor();

  for (particleIndex = 0; particleIndex < GROWTH_PARTICLE_MAX;
       particleIndex++) {
    struct ObjectEvent *particle;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 radius;

    if (effectWork->angle == 0x4000) {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + 0xa0000,
                 caster->z.fixed);
    } else if (effectWork->angle == 0xc000) {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + 0x180000,
                 caster->z.fixed);
    } else {
      SET_VECTOR(pos, caster->x.fixed, caster->y.fixed + 0xa0000,
                 caster->z.fixed);
      Vector_AddPolarOffset(0xa0000, effectWork->angle, pos);
    }

    // The shadow part takes the image's colour mode, shape, size, tile and
    // palette, so the shadow (turned on by SPRITE_FLAG_SHADOW below) is drawn
    // with the particle's picture. BUG: the copy comes before the NULL test, so
    // a failed spawn (a full object table) is read through.
    particle = (struct ObjectEvent *)Object_Spawn(SPRITE_PARTICLE, pos[0],
                                                  pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)particle->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)particle->sprite)->child;
    child->flip = visual->flip;
    child->flagsAHigh = visual->flagsAHigh;
    child->flagsBHigh = visual->flagsBHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (particle != 0) {
      particle->scaleY = 0xb333; // 0.7
      particle->scaleX = 0xb333;
      particle->acceleration = 0x18000;
      particle->speed = 0x18000;
      particle->motionFlags = 0;
      Animation_ApplyChildValuesFar((struct DispatchObject *)particle,
                                    SPRITE_COLOR_GREEN_TINT);
      ObjectDispatch_SelectAnimationFar(particle, 7);
      ObjectDispatch_InitializeFar((struct DispatchObject *)particle,
                                   gFieldEffect_CommonParticleScript + 1);
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)particle,
                                            1);

      // Destination: 4 to 10 pixels from the effect's position.
      SET_VECTOR(pos, effectWork->x, effectWork->y, effectWork->z);
      if (effectWork->angle == 0xc000)
        Vector_AddPolarOffset(0xe0000, effectWork->angle, pos);
      radius = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(radius, Random16(), pos);
      Object_SetMoveTargetFar((struct Object *)particle, pos[0], pos[1],
                              pos[2]);
    }
    Sound_PlaySongFar(SE_131);
    WaitFrames(2);
  }
  WaitFrames(8);
}

#undef GROWTH_PARTICLE_MAX
#undef SPRITE_PARTICLE
#undef GROWTH_FALL_OBJECT
#undef GROWTH_FALL_LAST
