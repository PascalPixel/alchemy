#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

struct ObjectRuntime *Object_GetById(s32 id);

// Moves the object by its velocity, slows its x and z velocity, grows its
// scale, turns its sprite, then gives the sprite the current owner's sprite
// priority.
void Func_0200342c(struct ObjectRuntime *object) {
  s32 vx = object->bounce;
  s32 vz;
  struct PartyState *party;
  struct FieldSprite *sprite;
  struct ObjectRuntime *owner;
  struct SpriteAttributes *to;
  struct SpriteAttributes *from;
  struct SpriteBits source;
  struct SpriteBits bits;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH(barrier): an empty asm ends the velocity stores, so the scale
   * step's loads follow the z velocity's store, as native; without it, 2 of 59
   * instructions differ in tla-en. */
  asm volatile("");
  {
    /* FAKEMATCH(temporary): the scale x step read into a block temporary,
     * which loads it before the scale x, as native; as object->scaleX +=
     * object->speedLimit, 2 of 59 instructions differ in tla-en. */
    s32 add = object->speedLimit;

    object->scaleX += add;
  }
  sprite = object->animation;
  object->scaleY += object->acceleration;
  /* FAKEMATCH(barrier): an empty asm ends the scale stores, so the turn's
   * address of object->action is built after the scale y's store, as native;
   * without it, 2 of 59 instructions differ in tla-en. */
  asm volatile("");
  sprite->rotation += object->action;

  party = &gPartyState;
  /* FAKEMATCH(one-pass): an empty one-pass block after the party state's
   * address; without it, 2 of 59 instructions differ in tla-en. */
  do {
  } while (0);
  owner = Object_GetById(party->currentOwner);
  to = (struct SpriteAttributes *)object->animation;
  from = (struct SpriteAttributes *)owner->animation;
  source = from->bits;
  /* FAKEMATCH(one-pass): a one-pass block reads the sprite's bits after the
   * owner's, as in Func_02000ab4 of the Mars Lighthouse star room; as plain
   * statements, 2 of 59 instructions differ in tla-en. */
  do {
    bits = to->bits;
    bits.priority = source.priority;
    to->bits = bits;
  } while (0);
}
