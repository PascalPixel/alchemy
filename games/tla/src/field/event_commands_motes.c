/// @file
/// The floating motes and sparkles on the field, and the blends they fade by.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "field/event_commands.h"
#include "field/field_bank.h"
#include "field/object.h"
#include "field/ground_mask.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

/// Starts the 64 floating motes (FieldMotes_Update): loads their tiles and puts
/// each under the leader with staggered timers, drawn semi-transparent.
void FieldMotes_Start(void) {
  struct WeatherWork *work;
  struct GroundParticle *mote;
  u8 *tiles;
  u32 i;
  u32 size;
  volatile u32 zero;
  volatile u16 *reg;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6. */
  register s32 clear asm("r6");

  size = sizeof(struct WeatherWork);
  work = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, size);
  mote = work->particles;
  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x205, REG_DMA3);
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  Resource_DecodeByteLzInRam(gFieldEffect_MoteTiles, tiles);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++) {
    struct MapWork *view = slots->mapGrid;
    s32 *pos;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and the
     * height's x. */
    u32 p;
    s32 z;

    p = (u32)mote;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    pos = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    mote->timer = (i & 15) + 1;
    mote++;
  }
  /* FAKEMATCH(dead): the blend writes in a once-only block keep the task
   * routine's address ahead of the call's other argument. */
  do {
    /* FAKEMATCH(temporary): the values through v and the registers through one
     * stepped pointer. */
    s32 v;
    v = 0x3f00;
    reg = &REG_BLDCNT;
    *reg = v;
    v = 0x1008;
    reg++;
    *reg = v;
    reg++;
    *reg = clear;
  } while (0);
  Task_Add(FieldMotes_Update, TASK_PASS_VBLANK);
}

#undef Resource_DecodeByteLzInRam

/// Starts Weather_UpdateRain's particles: allocates and clears heap slot 0x74,
/// decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
/// words and places it on the ground at the map's focus with a timer of
/// (i & 11) + 1, turns on alpha blending of all layers at 16/16, schedules the
/// update and, when asked, plays SE_KIRIN (Scene_QueueMusic).
void Weather_StartRain(s32 withCue) {
  struct WeatherWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct WeatherWork));
  struct GroundParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r1, built after the copy of work.
   */
  register u32 zero asm("r1");
  u8 *tiles;
  u32 i;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6. */
  register s32 clear asm("r6");

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, sizeof(struct WeatherWork));
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  Resource_DecodeByteLzInRam(gFieldEffect_StreakTiles, tiles);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++, particle++) {
    struct MapWork *view = slots->mapGrid;
    s32 *focus;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and the
     * height's x. */
    u32 p;
    s32 z;

    p = (u32)particle;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    focus = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  Io_Write16(clear, &REG_BLDY);
  Task_Add(Weather_UpdateRain, TASK_PASS_VBLANK);
  if (withCue != 0)
    Scene_QueueMusic(SE_KIRIN);
}

/// Stops Weather_StartRain's particles. Unused: nothing calls
/// Weather_StopRainFar.
void Weather_StopRain(void) { Task_Remove(Weather_UpdateRain); }

/// Task: the sparkles.
void FieldEffect_UpdateSparkles(void) {
  struct WeatherWork *work;
  u32 spawned;
  u32 i;
  struct MapWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct GroundParticle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = Ram_HeapSlots->weather;
  spawned = 0;
  map = Ram_HeapSlots->mapGrid;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->particles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Get(FLAG_WEATHER_HELD) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gFieldEffect_SparklePictures[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->posY - cameraZ) / Q16_ONE +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this reads a u32 as unsigned long; UBFIX reads the declared type.
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = sy;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Oam_AddSpriteAtDepthClamped(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 63);
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

/// Starts the sparkles (FieldEffect_UpdateSparkles): loads their tiles and
/// clears the 32 sparkles.
void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct WeatherWork);
  struct WeatherWork *work = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, size);
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *slots;

  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set(&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(struct WeatherWork) / 4),
          REG_DMA3);
  buf = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  Resource_DecodeByteLzInRam(gFieldEffect_SparkleTiles, buf);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x200, buf);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);

  i = 0;
  slots = Ram_HeapSlots;
  clear = 0;
  do {
    struct MapWork *view = slots->mapGrid;
    s32 *leader;
    /* FAKEMATCH(temporary): one integer cursor for the stores and the x. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(u32);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(u32);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = MapGrid_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & 15) + 1;
    i++;
    p++;
  } while (i < 64);
  Task_Add(FieldEffect_UpdateSparkles, TASK_PASS_VBLANK);
}

/// Starts the storm's lightning (Weather_UpdateLightning): builds the dark
/// (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
/// fade, and arms the first burst to end in 600 frames, repeating.
void Weather_StartLightning(void) {
  struct FieldBlendWork *work;
  struct PaletteFade *buffers;
  volatile u32 zero;

  work =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->paletteFade;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  PaletteFade_Build(0x10003, (u16 *)buffers, (u16 *)work, 1);
  PaletteFade_Build(0x10005, (u16 *)buffers, (u16 *)work->to, 1);
  PaletteFade_CalculateSteps(work->to, work->from, work->delta, 12);
  PaletteFade_Build((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Task_Add(Weather_UpdateLightning, TASK_PASS_VBLANK);
}

/// Stops the lightning: no flash for 0x7fff frames, and none after.
void Weather_StopLightning(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 0x7fff;
  state->enabled = 0;
}

/// One last burst of lightning, its first flash two frames from now (timer 12;
/// Weather_UpdateLightning flashes at 11), then no more.
void Weather_LastLightning(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

/// Restarts the lightning: the next burst ends in 128 frames, and bursts
/// repeat.
void Weather_ResumeLightning(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

/// Flashes the palette with the storm's task: one burst of three flashes of
/// palette to, each fading back to palette from (PaletteFade_Build modes) on
/// the BG colours, ending 120 frames from now, and no more after.
void Weather_StartLightningFlash(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct PaletteFade *buffers;
  volatile u32 zero;
  s16 *target;

  work =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->paletteFade;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  PaletteFade_Build(from, (u16 *)buffers, (u16 *)work, 1);
  target = work->to;
  PaletteFade_Build(to, (u16 *)buffers, (u16 *)target, 1);
  PaletteFade_CalculateSteps(target, work->from, work->delta, 12);
  PaletteFade_Build((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Task_Add(Weather_UpdateLightning, TASK_PASS_VBLANK);
}
