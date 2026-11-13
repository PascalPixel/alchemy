/// @file
/// The context effects of leaving through an exit, and the line scroll:
/// per-line scroll offsets fed by HBlank DMA.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

void GameFlag_SetBit(s32 flag);

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
// As ☀️'s function of the same name.
void ObjectEffect_PrepareContextEffect(s32 animation) {
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
  // An empty once-only loop keeps the cleared flags' store before the kind's
  // load; without it, 8 of 47 instructions differ in tla-en.
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
  Actor_SetMode(object, mode);
  wait(18);
}

void ObjectEffect_StartContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBit(0x120);
}

void ObjectEffect_StartContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBit(0x121);
}

struct ContextSprite {
  u8 unk00[0xc];
  s32 scale;
  u8 unk10[0xa];
  u8 flags;
  u8 count;
  u8 unk1c[0xc];
  void *entries[4];
};

struct ContextEffectEntry {
  u8 unk00[5];
  u8 param;
};

void ResourceMetadata_ClearRecordFar(void *record);

void ObjectEffect_EndContextEffect(s32 mode) {
  s32 zero;
  struct Actor *obj = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct ContextSprite *ctx = obj->sprite;
  struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & 0xfff00000) + 0x80000;
  obj->z &= 0xfff00000;
  Actor_SetMode(obj, mode);
  wait(30);
  ctx->count = 1;
  ResourceMetadata_ClearRecordFar(ctx->entries[1]);
  {
    /* FAKEMATCH(pin): the sprite copied into r1 for its two stores, leaving
     * r3 to the zero's copy from sl; as ctx in both stores, 8 of 69
     * instructions differ in tla-en. */
    register struct ContextSprite *pinnedCtx asm("r1") = ctx;

    pinnedCtx->flags = 1;
    pinnedCtx->entries[1] = (void *)zero;
  }
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Actor_SetTargetPosition(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_CommitPosition(obj);
}


s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Camera_MoveWithinMapBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_Test(flag) != 0) {
    ObjectEffect_EndContextEffect(24);
    GameFlag_ClearBit(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_Test(flag) != 0) {
      ObjectEffect_EndContextEffect(23);
      GameFlag_ClearBit(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_Test(flag) != 0) {
        s32 id;
        struct Actor *object;

        GameFlag_ClearBit(flag);
        id = gMainRoutineWork.currentOwner;
        object = ObjectTable_Get(id);
        object->y += 0x00a00000;
        Camera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (object->y + object->velocityY > object->terrainHeight) {
          wait(1);
        }
        Sound_PlayFar(159);
        object->y = object->terrainHeight;
        Actor_SetMode(object, 22);
        Event_Wait(15);
        Camera_FollowActor(id, 1);
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


struct DmaFxPage {
  u32 lines[0x1e0];
};

struct DmaFxPages {
  struct DmaFxPage pages[2];
  u8 page;
};

/// Writes the current page's first entry each frame; HBlank DMA writes the
/// rest.
void Func_080d5ff8(void) {
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


struct DisplayScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unkf04[4];
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
};

extern const s16 Data_080f08b0[];

void Func_080d607c(void) {
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
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
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
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
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
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
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
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
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

struct DmaFxWork {
  u8 unk000[0xf01];
  u8 kind;
  u8 unkf02[6];
  s32 unkf08;
  s32 unkf0c;
  s32 unkf10;
  s32 unkf14;
  s32 unkf18;
  s32 unkf1c;
};

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
  Task_Add(Func_080d607c, TASK_PASS_VBLANK);
  Task_Add(Func_080d5ff8, TASK_PASS_FRAME);
}

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  Task_Remove(Func_080d5ff8);
  Task_Remove(Func_080d607c);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}


struct GroundParticle {
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
  struct GroundParticle particles[64];
};

void Func_080140d8(struct GroundParticle *sprite, s32 depth);

extern const s16 Data_080f0ab0[];

void Func_080d64f0(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(358))
      particle->timer++;
    frame = &Data_080f0ab0[particle->timer * 5];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x800000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}
