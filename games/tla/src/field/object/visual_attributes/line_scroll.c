/// @file
/// The context effects of leaving through an exit, and the line scroll:
/// per-line scroll offsets fed by HBlank DMA.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/field/field.h"

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
// As ☀️'s function of the same name.
void Exit_PrepareAnimation(s32 animation) {
  u32 zero;
  u8 param;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  struct MainRoutineWork *party;
  s32 mode;

  party = &gMainRoutineWork;
  mode = animation;
  object = ObjectTable_Get(party->currentOwner);
  context = object->sprite;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  context->flags = zero;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  param = 15;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SelectAnimationFar(object, mode);
  wait(18);
}

void Exit_StartStairsUp(void) {
  Exit_PrepareAnimation(0x1A);
  GameFlag_Set(0x120);
}

void Exit_StartStairsDown(void) {
  Exit_PrepareAnimation(0x19);
  GameFlag_Set(0x121);
}

void Entrance_EndArrivalAnimation(s32 mode) {
  s32 zero;
  struct Actor *obj = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct ContextSprite *ctx = obj->sprite;
  struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & 0xfff00000) + 0x80000;
  obj->z &= 0xfff00000;
  Actor_SelectAnimationFar(obj, mode);
  wait(30);
  ctx->count = 1;
  SpriteAnimation_ClearFar(ctx->entries[1]);
  {
    /* FAKEMATCH(pin): the sprite copied into r1 for its two stores. */
    register struct ContextSprite *pinnedCtx asm("r1") = ctx;

    pinnedCtx->flags = 1;
    pinnedCtx->entries[1] = (void *)zero;
  }
  obj->acceleration = px(1);
  obj->speedLimit = px(1);
  Actor_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_WaitForMoveFar(obj);
}

void FieldCamera_MoveWithinMapBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Entrance_PlayArrival(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_Get(flag) != 0) {
    Entrance_EndArrivalAnimation(24);
    GameFlag_Clear(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_Get(flag) != 0) {
      Entrance_EndArrivalAnimation(23);
      GameFlag_Clear(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_Get(flag) != 0) {
        s32 id;
        struct Actor *object;

        GameFlag_Clear(flag);
        id = gMainRoutineWork.currentOwner;
        object = ObjectTable_Get(id);
        object->y += 0x00a00000;
        FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (object->y + object->velocityY > object->terrainHeight) {
          wait(1);
        }
        Sound_PlayFar(SE_DOOR_CLOSE);
        object->y = object->terrainHeight;
        Actor_SelectAnimationFar(object, 22);
        Event_Wait(15);
        FieldCamera_FollowActor(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

void Func_080d5fd4(void) {
  struct Actor *object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  u8 *metadata =
      ResourceMetadata_RegisterFar(*(void **)((u8 *)object + 0x50), 268);

  metadata[5] = 15;
}

/// Writes the current page's first entry each frame; HBlank DMA writes the
/// rest.
void LineScroll_ArmDma(void) {
  struct DmaFxPages *work = Ram_HeapSlots->unk84[1];
  u32 *line = work->pages[work->page].lines;
  volatile u32 *dest = (volatile u32 *)&REG_BG1HOFS;
  u8 *event = Ram_HeapSlots->eventWork;
  volatile u32 *target;

  if (event != NULL) {
    if (*(s8 *)(event + 0xca4) != 0)
      return;
    if (*(s16 *)(event + 0x164) != 0)
      return;
  }
  {
    volatile u16 *dma = REG_DMA0;
    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    target = dest;
    (void)dma[5];
  }
  *dest = *line++;
  *dest = *line++;
  *dest = *line++;
  Dma_Set(
      line, (void *)target,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)
              << 16 |
          3,
      REG_DMA0);
}

void LineScroll_BuildPage(void) {
  struct DisplayScrollWork *work = Ram_HeapSlots->unk84[1];
  u8 mode = work->mode;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  u8 *event = Ram_HeapSlots->eventWork;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amplitude;
  s32 scanline;
  u16 wave;
  u16 waveX3;
  u16 waveY3;

  if (event != NULL && *(s8 *)(event + 0xca4) != 0)
    return;
  if (mode == 0) {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      u16 a = x1;
      u16 b = x2;

      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        row[0] = a + wave;
        row[2] = b + wave;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      u16 a = y1;
      u16 b = y2;
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        *row = a + wave;
        row += 2;
        *row = b + wave;
        row += 2;
        *row = waveY3 + wave;
        row += 2;
        phase += step;
      }
    }
  } else {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        row[0] = x1;
        row[2] = x2;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(gLineScroll_WaveSine[(phase >> 16) & 255], amplitude) /
            256;
        row[0] = y1;
        row[2] = y2;
        row[4] = waveY3 + wave;
        row += 6;
        phase += step;
      }
    }
  }
  work->frame++;
  work->page ^= 1;
}

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA_ENABLE << 16) != 0) {
  }
}

/// Starts the line scroll: its work (heap slot 136) holds the mode and, for x
/// and y, the wave's frequency, its step each frame and its amplitude; two
/// tasks, one in the vertical blank pass and one in the frame pass, build and
/// send each frame's line offsets.
void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY) {
  struct DmaFxWork *work = HeapSlot_AllocEwram(136, sizeof(struct DmaFxWork));
  volatile u32 zero;

  zero = 0;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  WaitDma((volatile struct DmaRegisters *)&REG_DMA3SAD);
  work->kind = mode;
  work->unkf08 = freqX;
  work->unkf0c = freqY;
  work->unkf18 = ampX;
  work->unkf1c = ampY;
  work->unkf10 = stepX;
  work->unkf14 = stepY;
  Task_Add(LineScroll_BuildPage, TASK_PASS_VBLANK);
  Task_Add(LineScroll_ArmDma, TASK_PASS_FRAME);
}

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  Task_Remove(LineScroll_ArmDma);
  Task_Remove(LineScroll_BuildPage);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}

/* VIEW(visual_attributes): tag struct GroundParticle as struct
 * GroundParticle__visual_attributes; ground_mask.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/field/object/visual_attributes.c:2397: field
 * `particles' has incomplete type. */
struct GroundParticle__visual_attributes {
  u8 unk00[4];
  u16 y : 8;
  u16 attr0 : 6;
  u16 shape : 2;
  u16 x : 9;
  u16 attr1 : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  u8 unk00[4];
  s32 tile;
  struct GroundParticle__visual_attributes particles[64];
};

/// Task of GroundParticles_Start: The Broken Seal's Weather_UpdateRain, kept
/// whole. Each particle plays its 16 frames of
/// gFieldEffect_GroundParticleFrames on a random spot near the leader, held
/// while flag 358 is set.
void GroundParticles_Update(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle__visual_attributes *particle = work->particles;
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Get(358))
      particle->timer++;
    frame = &gFieldEffect_GroundParticleFrames[particle->timer * 5];
    x = (particle->posX - originX) / Q16_ONE + *frame++;
    y = (particle->posZ - particle->posY - originZ) / Q16_ONE + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x800000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}
