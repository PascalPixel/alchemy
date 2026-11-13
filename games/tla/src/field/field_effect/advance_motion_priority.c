#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event.h"

/* VIEW(advance_motion_priority): tag struct EffectSpriteBits as struct
 * EffectSpriteBits__advance_motion_priority; module/scene/trial_road.h has
 * another definition. */
/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits__advance_motion_priority {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/* VIEW(advance_motion_priority): tag struct EffectSpriteAttributes as struct
 * EffectSpriteAttributes__advance_motion_priority; module/scene/trial_road.h
 * has another definition. */
/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes__advance_motion_priority {
  u8 unk00[9];
  struct EffectSpriteBits__advance_motion_priority bits;
};

/// FieldEffect_AdvanceMotion, then gives the effect's sprite the leader's
/// sprite priority, so the effect is drawn at the leader's depth.
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct FieldSprite *sprite;
  struct Actor *leader;
  struct EffectSpriteAttributes__advance_motion_priority *to;
  struct EffectSpriteAttributes__advance_motion_priority *from;
  struct EffectSpriteBits__advance_motion_priority source;
  struct EffectSpriteBits__advance_motion_priority bits;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  effect->scaleX += effect->scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->currentOwner);
  to = (struct EffectSpriteAttributes__advance_motion_priority *)effect->sprite;
  from =
      (struct EffectSpriteAttributes__advance_motion_priority *)leader->sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}
