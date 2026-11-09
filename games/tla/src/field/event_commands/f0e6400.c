#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "fixed_math.h"
#include "io_reg.h"

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct Work080e6400 {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct EffectParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *anchor;
  struct Actor *other;
  s16 mode;
};

void Func_080e62f4(u8 *source, u8 *destination, s32 phase);
void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite,
                                              struct EffectParticle *particle,
                                              s32 tiles);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct EffectParticle *particle);
u32 Random16(void);

#define NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e6400(void) {
  struct Work080e6400 *work;
  struct Actor *anchor;
  struct EffectSprite *sprite;
  struct EffectParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  work = (struct Work080e6400 *)Ram_HeapSlots->battleFxWork[0];
  anchor = work->anchor;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = 999;
      break;
    }
    phase = &work->phase;
    if (*phase == 2) {
      picture = work->picture;
      vram = (u8 *)OBJ_VRAM0 + work->pictureTile * TILE_SIZE_4BPP;
      Func_080e62f4(work->tiles, picture, work->timer + 14);
      /* FAKEMATCH(signature): the heap copy of Render_CopyStrided, void
       * elsewhere (UploadFn), called as returning a pointer, so the call sets
       * r0 and the later r0 move depends on it; as void, 2 of 596
       * instructions differ in tla-en. */
      ((const u8 *(*)(const u8 *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          picture, 32, 32, vram);
      sprite = &work->pictureSprite;
      Func_080eb01c(sprite);
    }
    if (*phase == 2) {
      for (i = 0, particle = work->particles; i < 18; i++, particle++) {
        sprite = &work->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                   work->particleTile);
        particle->timer++;
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(work->anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      for (i = 0, particle = work->particles; i < 64; i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &work->sprites[i];
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (particle->timer < 42) {
            particle->y -= Q16_ONE;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
      }
    }
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
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
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = 999;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 20; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
}
