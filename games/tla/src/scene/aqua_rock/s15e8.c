#include "local.h"

void Func_020015e8(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;
  s16 *amplitude;

  object->action += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude += 32;
  if (*amplitude > 0x200)
    *amplitude = 0x200;
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}

void Func_02001630(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;
  s16 *amplitude;

  object->action += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude -= 16;
  if (*amplitude < 0) {
    *amplitude = 0;
  }
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}
