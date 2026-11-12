#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event.h"

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 unk00[9];
  struct EffectSpriteBits bits;
};

/// Effect_AdvanceMotion, then gives the effect's sprite the leader's sprite
/// priority, so the effect is drawn at the leader's depth.
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct FieldSprite *sprite;
  struct Actor *leader;
  struct EffectSpriteAttributes *to;
  struct EffectSpriteAttributes *from;
  struct EffectSpriteBits source;
  struct EffectSpriteBits bits;

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
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->animation;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}
