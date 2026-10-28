#include "local.h"

void Func_020015e8(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;
  s16 *amplitude;

  object->action += 0x2000;
  /* FAKEMATCH: keeps the action store before the amplitude address is formed
   * (differ 4 without) */
  __asm__("" : : "r"(sprite));
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
  /* FAKEMATCH: keeps the action store before the amplitude address is formed
   * (differ 4 without) */
  __asm__("" : : "r"(sprite));
  amplitude = (s16 *)&object->unk66;
  *amplitude -= 16;
  if (*amplitude < 0) {
    s32 floor = 0; /* FAKEMATCH: a word temporary keeps the 0 out of the
                      halfword pool */

    *amplitude = floor;
  }
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}
