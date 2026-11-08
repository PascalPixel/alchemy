#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"

struct ShineParticle {
  s32 x;
  s32 y;
  u8 unk08[16];
  s32 timer;
};

struct ShineSprite {
  u8 unk00[12];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct ShineWork {
  u8 unk0000[0x1000];
  struct ShineParticle particles[12];
  u8 unk1150[0x230];
  struct ShineSprite sprites[12];
  u8 unk1560[0x348];
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  u8 fading;
};

struct BlendWork {
  u8 unk00[160];
  u32 phase;
  u8 unka4[12];
  s32 scaleX;
  s32 scaleY;
  u8 unkb8[9];
  u8 mode;
};

void Func_080eb01c(struct ShineSprite *sprite);
void BattleEffect_IntegrateVector2(struct ShineParticle *particle, s32 frames,
                                   s32 flags);
void Map_SetScreenShakeFar(s32 a, s32 b, s32 c);

void Func_080e5a64(void) {
  struct ShineWork *work = (struct ShineWork *)Ram_HeapSlots->battleFxWork[0];
  struct BlendWork *blend = Ram_HeapSlots->unkec[1];
  struct ShineParticle *particle;
  struct ShineSprite *sprite;
  s32 i;
  s32 base, blue;
  s32 color;
  s32 red, green;
  u32 *phase;

  work->fading = 0;
  if (work->state == 1) {
    i = 0;
    /* FAKEMATCH(temporary): the phase read through a pointer kept across the
     * loop; as blend->phase, 4 of 302 instructions differ in tla-en. */
    phase = &blend->phase;
    /* FAKEMATCH(temporary): the phase offset built in two steps, so it stays
     * in a register across the loop; as base = 0x90000, 48 of 302
     * instructions differ in tla-en. */
    base = 9;
    base <<= 16;
    blue = 0x7c00;
  palette:
    red = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    green = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    red = (red >> 16) + 16;
    green = (green >> 16) + 16;
    // A once-only block around the palette store.
    do {
      color = (green << 5) | blue | red;
      *(u16 *)(0x050003c0 + i * 2) = color;
    } while (0);
    if (++i < 32)
      goto palette;
  }
  if (work->sparkles) {
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 12; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (work->state) {
  case 0:
    work->object->y -= 0x38000;
    if (work->timer == 8) {
      blend->scaleX = 0x2000;
      blend->scaleY = 0x2000;
      Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
      Sound_PlayFar(567);
      work->sparkles = 1;
      work->fading = 1;
    }
    if (work->timer == 8) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    blend->scaleX += 0x1000;
    blend->scaleY = blend->scaleX;
    if (blend->scaleX > 0xcccb) {
      blend->scaleX = 0xcccc;
      blend->scaleY = 0xcccc;
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    blend->mode = 3;
    if (work->timer == 32) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0)
      Map_SetScreenShakeFar(1, 1, 1);
    if (work->timer == 10) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 4:
    work->object->y += 0x18000;
    if (work->timer == 13)
      work->state = 999;
    break;
  }
  work->timer++;
}
