#include "types.h"
#include "field_effect.h"
#include "adventure.h"

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

union FieldObject *Object_GetById(s32 id);

/// Effect_AdvanceMotion, then gives the effect's sprite the leader's sprite
/// priority, so the effect is drawn at the leader's depth.
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct Adventure *party;
  struct FieldSprite *sprite;
  union FieldObject *leader;
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
  /* FAKEMATCH(barrier): an empty asm ends the velocity stores, so the scale
   * step's loads follow the z velocity's store; without it, 2 of 59
   * instructions differ in tla-en. */
  asm volatile("");
  {
    /* FAKEMATCH(temporary): the x scale rate read into a block temporary,
     * which loads it before the x scale; as effect->scaleX +=
     * effect->scaleRateX, 2 of 59 instructions differ in tla-en. */
    s32 rate = effect->scaleRateX;

    effect->scaleX += rate;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  /* FAKEMATCH(barrier): an empty asm ends the scale stores, so the spin's
   * address is built after the y scale's store; without it, 2 of 59
   * instructions differ in tla-en. */
  asm volatile("");
  sprite->rotation += effect->spin;

  party = &gAdventure;
  /* FAKEMATCH(one-pass): an empty one-pass block after the adventure's
   * address; without it, 2 of 59 instructions differ in tla-en. */
  do {
  } while (0);
  leader = Object_GetById(party->currentOwner);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  /* FAKEMATCH(one-pass): a one-pass block reads the effect sprite's bits after
   * the leader's; as plain statements, 2 of 59 instructions differ in
   * tla-en. */
  do {
    bits = to->bits;
    bits.priority = source.priority;
    to->bits = bits;
  } while (0);
}
