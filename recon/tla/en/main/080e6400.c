// Draft, differ 598 of 596: the frame callback of Func_080e68d0's effect, a
// state machine on its phase (0x1928) and timer (0x192a) in one of four
// modes (0x1938). Mode 0: the second object appears full width, the first
// takes mode 3 and rises for 16 frames, then a picture rippled each frame
// (Func_080e62f4) is copied to sprite VRAM and drawn while 18 particles grow
// (BattleEffect_UpdateGrowingParticleSprite) until frame 50; mode 3 does the
// same with 64 rising, jittering sparks until frame 90; modes 1 and 2 only
// raise the first object (mode 4) for 16 frames, mode 2 with 20 growing
// particles. The phase parks at 999 when done.
// Same calls, offsets and control flow; left: the native keeps the work in
// r7 and the object in r8 (here swapped, whichever is declared first), which
// changes nearly every field access.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

struct RiseParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct RiseWork {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct RiseParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *object;
  struct Actor *other;
  s16 mode;
};

void Actor_SetMode(struct Actor *object, s32 mode);
void Func_080e62f4(u8 *source, u8 *destination, s32 phase);
void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite,
                                              struct RiseParticle *particle,
                                              s32 particleTile);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct RiseParticle *particle);
u32 Random16(void);

#define RISE_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e6400(void) {
  struct RiseWork *work;
  struct Actor *object;
  struct EffectSprite *sprite;
  struct RiseParticle *particle;
  u8 *vram;
  s32 jitter;
  s32 i;

  work = (struct RiseWork *)Ram_HeapSlots->battleFxWork[0];
  object = work->object;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = 0x10000;
      if (work->timer == 4)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(object, 3);
      if (work->timer <= 15)
        object->y += 0x24ccc;
      if (work->timer == 28)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      vram = (u8 *)0x06010000 + work->pictureTile * 32;
      Func_080e62f4(work->tiles, work->picture, work->timer + 14);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          work->picture, 32, 32, vram);
      Func_080eb01c(&work->pictureSprite);
      if (work->phase == 2) {
        particle = work->particles;
        sprite = work->sprites;
        for (i = 0; i <= 17; i++) {
          if ((u32)particle->timer <= 31)
            BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                     work->particleTile);
          particle->timer++;
          sprite++;
          particle++;
        }
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = 0x10000;
      if (work->timer == 4)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(work->object, 3);
      if (work->timer <= 15)
        object->y += 0x24ccc;
      if (work->timer == 28)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      particle = work->particles;
      sprite = work->sprites;
      for (i = 0; i <= 63; i++) {
        if ((u32)particle->timer <= 44) {
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (particle->timer <= 41) {
            particle->y += -0x10000;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
        sprite++;
        particle++;
      }
    }
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->object, 4);
      object->y += 0x24ccc;
      if (work->timer == 15)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 20)
        work->phase = 999;
      break;
    }
  } else {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->object, 4);
      object->y += 0x24ccc;
      if (work->timer == 15)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = 999;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 19; i >= 0; i--) {
      if ((u32)particle->timer <= 31)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
      sprite++;
      particle++;
    }
  }
  work->timer++;
}
