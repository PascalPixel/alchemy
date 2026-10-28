#include "types.h"
#include "actor.h"
#include "field_sprite.h"

extern const u8 gData_020050d0[];

u32 Random16Far(void);
struct Actor *Actor_GetById(s32 id);
struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004dac(struct Actor *object, const u8 *script);

s32 Func_02000610(void) {
  struct FieldSprite *sprite = Actor_GetById(9)->animation;

  sprite->rotation -= 0x300;
  return 0;
}

/// Randomly vary the scale.
void Func_0200062c(struct Actor *object) {
  u32 random = Random16Far();
  s32 scale = object->scaleX;
  s32 bias;
  s32 limit;

  /* FAKEMATCH: two barriers load the scale and then the pooled -0x800 before
   * the random value is shifted; without them, 2 instructions differ in
   * tla-en. */
  __asm__ volatile("");
  bias = -0x800;
  /* FAKEMATCH: second barrier of the pair above: the pool load stays before the
   * shifts. */
  __asm__ volatile("" : "+r"(bias));
  scale += (random << 12) >> 16;
  scale += bias;
  object->scaleX = scale;
  object->scaleY = object->scaleY + ((Random16Far() << 12) >> 16) + bias;
  limit = 240;
  /* FAKEMATCH: second barrier of the three above: the limit is started before
   * scaleX is loaded. */
  __asm__ volatile("" : "+r"(limit));
  scale = object->scaleX;
  /* FAKEMATCH: third barrier of the three above: scaleX is loaded before the
   * limit is shifted. */
  __asm__ volatile("" : "+r"(limit));
  limit <<= 4;
  limit += 255;
  if (scale <= limit)
    object->scaleX = 0x2000;
  if (object->scaleY <= limit)
    object->scaleY = 0x2000;
}

s32 Func_02000678(void) {
  struct Actor *source = Actor_GetById(9);
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 x = source->x;

  /* FAKEMATCH: the native loads the source x before it starts the offset
   * 0x80000; without the barrier the offset is started first (one reordered
   * pair, differ 2). */
  __asm__ volatile("" : "+r"(x));
  object = Import_ActorCreate(14, x + 0x80000, source->y - 0x20000, source->z);
  object->motionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02004dac(object, gData_020050d0);
  object->scaleX = 0x6666;
  object->scaleY = 0x6666;
  /* FAKEMATCH: the object passed through this asm stores both scales before
   * the sprite is loaded; without it, 4 instructions differ in tla-en. */
  __asm__("" : "+r"(object));
  sprite = object->animation;
  sprite->priority = 1;
  object->bounce = 0;
  object->unk4c = 0;
  object->update = (void (*)(void))Func_0200062c;
  return 0;
}
