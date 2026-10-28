#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02000ab4(struct ObjectRuntime *object) {
  struct ObjectRuntime *owner = Object_GetById(gAdventure.currentOwner);
  struct SpriteAttributes *to = (struct SpriteAttributes *)object->animation;
  struct SpriteAttributes *from = (struct SpriteAttributes *)owner->animation;
  struct SpriteBits source = from->bits;
  struct SpriteBits bits;

  /* FAKEMATCH: a one-pass block reads the sprite's bits after the owner's
   * (alchemy permute). */
  do {
    bits = to->bits;
    bits.priority = source.priority;
    to->bits = bits;
  } while (0);
}
