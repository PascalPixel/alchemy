#include "types.h"
#include "actor.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02002f88(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSingleChildFlags(Actor_GetById(id), 0);
  object->angle = FACING_SOUTH;
  object->motionFlags = 0;
  {
    s32 x = object->x;

    /* FAKEMATCH(barrier): an operand asm on x that keeps its load ahead of
     * the constant; without it, 2 of 32 instructions differ in tla-en. */
    asm("" : "+l"(x));
    object->x = x + 0xc0000;
  }
  object->y = PIXELS(16);
  object->collisionFlags = 0;
  {
    /* FAKEMATCH(pin): the sprite pointer in r2; as a plain local, 8 of 32
     * instructions differ in tla-en. */
    register struct ActorSprite *sprite asm("r2") =
        (struct ActorSprite *)object->animation;

    /* FAKEMATCH(barrier): an operand asm on the sprite pointer that keeps
     * its load ahead of the constant; without it, 2 of 32 instructions
     * differ in tla-en. */
    asm("" : "+l"(sprite));
    sprite->rotation = 0xc000;
  }
}
