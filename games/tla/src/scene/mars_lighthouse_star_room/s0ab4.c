#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct SpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
  u8 unk00[9];
  struct SpriteBits bits;
};

struct Actor *Actor_GetById(s32 id);

void Func_02000ab4(struct Actor *object) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
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
