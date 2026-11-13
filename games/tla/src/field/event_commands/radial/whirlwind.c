/// @file
/// Whirlwind's ring of particles.
// Part of radial.c.

#include "../radial_internal.h"
#include "module/field/event_commands.h"

/* VIEW(radial): variable gBattleEffect_SharedParticleScript as const u8 gBattleEffect_SharedParticleScript[]; object_effect_scripts.h has const s32 gBattleEffect_SharedParticleScript[]. */
extern const u8 gBattleEffect_SharedParticleScript__radial[] __asm__("gBattleEffect_SharedParticleScript");

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

/* VIEW(radial): routine Effect_AdvanceMotion as void Effect_AdvanceMotion(struct FieldEffect *effect); module/field/field_effect.h has void Effect_AdvanceMotion(union FieldObject *object). */
extern void Effect_AdvanceMotion__radial(struct FieldEffect *effect) __asm__("Effect_AdvanceMotion");

void BattleEffect_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions options;
  s32 velocity[3];
  s32 sine;
  s32 angle;
  u32 i;

  options.palette = 0;
  options.update = Effect_AdvanceMotion__radial;
  options.startScaleX = 0xCCCC;
  options.startScaleY = 0xCCCC;
  i = 0;
  do {
    angle = i << 12;
    velocity[0] = Trig_Cos(angle) * 3 / 2;
    velocity[1] = 0;
    sine = Trig_Sin(angle);
    velocity[2] = sine;
    FieldEffect_SpawnResident(origin->x, origin->y, origin->z, velocity[0], velocity[1],
                  sine, 0x01090001, &options);
    i++;
  } while (i <= 16);
}

void BattleEffect_WanderAroundAnchor(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_StartScriptFar((struct Actor *)effect,
                         (u32)gBattleEffect_SharedParticleScript__radial);
}

/// Per-frame update of a Whirlwind particle: circles the anchor at 8 pixels,
/// a sixteenth of a turn a frame; after 121 frames it starts to wander
/// (BattleEffect_WanderAroundAnchor) with upward speed and gravity. As The
/// Broken Seal's FieldEffect_CircleAnchor.
void BattleEffect_CircleAnchor(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 radius;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  radius = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleEffect_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

#define Actor_SetPositionAndResetMotionFar                                     \
  ((s32 (*)(void *, s32, s32, s32))Actor_SetPositionAndResetMotionFar)

void BattleEffect_ShrinkObjectScaleUntilHalf(struct Actor *object) {
  s32 scale;

  scale = object->scaleX - 0x80;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < 0x8000) {
    Actor_SetPositionAndResetMotionFar(object, 0, 0, 0);
    object->update = 0;
  }
}

void FieldEffect_RunWhirlwindOnTarget(void) { FieldEffect_RunWhirlwind(); }
#undef Actor_SetPositionAndResetMotionFar
