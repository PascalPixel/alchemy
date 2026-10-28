#include "scene.h"

void Func_02000038(void) { Func_02001a04(12, 1, 13); }

void Func_02000048(struct FieldActor *object) {
  struct FieldSprite *sprite = object->sprite;
  s16 time = object->timer++;

  sprite->rotation = Iwram_MulQ16(Math_Sine(time << 12), 0x100);
  if ((time & 15) == 0)
    Func_02001a2c(155);
}

void Func_02000080(s32 id) {
  struct FieldActor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;

  /* FAKEMATCH: the sprite is loaded before the zero is built; a struct field, a
   * chained assignment and a zero local all build the zero first (2 differ) */
  asm volatile("");
  WORD_AT(object, 108) = 0;
  object->timer = 0;
  sprite->rotation = 0;
}
