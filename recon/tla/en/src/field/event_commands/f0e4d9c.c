#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "dma.h"

struct GlowParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct GlowWork {
  u8 tiles[0x400];
  struct GlowParticle sparks[32];
  struct EffectSprite sparkSprites[32];
  struct EffectSprite burstSprites[32];
  struct GlowParticle bursts[32];
  s16 phase;
  s16 timer;
  s16 shake;
  u16 burstTile;
  s16 nextBurst;
  s16 newBursts;
  s8 paletteMode;
};

struct FxScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

extern const u16 Data_080f0fa0[];

s32 Trig_Sin(s32 angle);
u32 Random16(void);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct GlowParticle *particle);
void Sound_PlayFar(s32 cue);
void Map_SetScreenShakeFar(s32 x, s32 y, s32 decay);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);

#define GLOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e4d9c(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct GlowParticle *particle;
  struct EffectSprite *sprite;
  volatile u16 *palette;
  s32 pos[3];
  s32 i;
  s32 *frames;
  s32 red;
  s32 blue;

  work->shake = 0;
  if (work->paletteMode == 1) {
    palette = (volatile u16 *)0x050003c0;
    i = 0;
    frames = &canvas->drawnFrames;
    red = 31;
    blue = 31 << 10;
    for (; i <= 31; i++) {
      {
        /* FAKEMATCH(temporary): the green term in a local of its own; as one
         * expression, 2 of 950 instructions differ in tla-en. */
        s32 green =
            (((Trig_Sin((((u32)*frames >> 1) + i << 16) + 0x90000 >> 5) * 12) >>
              16) +
             16)
            << 5;
        /* FAKEMATCH(barrier): an empty asm between `s32 green =
         * (((Trig_Sin((((u32)*frames >> 1) + i << 16) + 0x90000 >> 5) * 12) >>
         * 16) + 16) << 5;` and `*palette++ = green | blue | red;`, which keeps
         * their instructions apart; as plain statements, 2 of 950 instructions
         * differ in tla-en. */
        asm("");
        *palette++ = green | blue | red;
      }
      /* FAKEMATCH(barrier): an empty asm between `}` and `}`, which keeps their
       * instructions apart; as plain statements, 4 of 950 instructions differ
       * in tla-en. */
      asm("");
    }
  } else if (work->paletteMode == 2) {
    Dma_Set(Data_080f0fa0, (void *)0x050003e0, DMA_ENABLE << 16 | 0x10,
            SPLIT_CONSTANT(REG_DMA3));
    work->paletteMode = 0;
  }
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);

  if (scene->enabled == 0) {
    particle = work->sparks;
    for (i = 0; i < 32; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(Random16() * 12 + 0x40000, Random16(),
                            &particle->x);
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 71)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 6:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 12:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 32:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 42:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 52:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 62:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(140);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x300;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX >= 0x8000) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 40)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0)
        work->paletteMode = 2;
      if (work->timer == 0)
        Sound_PlayFar(144);
      if ((work->timer & 7) == 0) {
        Map_SetScreenShakeFar(0x30000, 0x30000, SPLIT_CONSTANT(0xe666));
        work->newBursts = 1;
        work->shake = 2;
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if ((work->timer & 15) == 0)
        Sound_PlayFar(144);
      if ((work->timer & 7) == 0) {
        Map_SetScreenShakeFar(0x30000, 0x30000, SPLIT_CONSTANT(0xe666));
        work->newBursts = 1;
        work->shake = 2;
      }
      if (work->timer == 128)
        GLOW_NEXT_PHASE(work);
      break;
    case 5:
      if (work->timer == 35)
        GLOW_NEXT_PHASE(work);
      break;
    case 6:
      if (work->timer == 0)
        work->newBursts = 10;
      if (work->timer == 4)
        work->newBursts = 10;
      if (work->timer == 0) {
        Map_SetScreenShakeFar(0xc0000, 0xc0000, SPLIT_CONSTANT(0xe666));
        Sound_PlayFar(145);
        if (work->timer == 0)
          work->shake = 3;
      }
      if (work->timer == 20)
        work->shake = 1;
      if (work->timer == 72) {
        work->phase = 999;
        work->timer = -1;
      }
      break;
    }
  } else {
    particle = work->sparks;
    for (i = 0; i < 12; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 47)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 4:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 8:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 14:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 28:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 35:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 41:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(140);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x500;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX > 0x4ccb) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0) {
        work->paletteMode = 2;
        if (work->timer == 0) {
          Map_SetScreenShakeFar(0x30000, 0x30000, SPLIT_CONSTANT(0xe666));
          work->newBursts = 1;
          Sound_PlayFar(144);
        }
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if (work->timer == 5)
        BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
      if (work->timer == 20) {
        work->phase = SPLIT_CONSTANT(999);
        work->timer = -1;
      }
      break;
    }
  }
  work->timer++;
}
