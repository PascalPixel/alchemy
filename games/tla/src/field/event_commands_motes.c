/// @file
/// Weather drawn in sprites on the field, and the storm's lightning: floating
/// motes, rain, rising sparkles, and the flashes of the palettes.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "field/ground_mask.h"

/// The frames the first motes and sparkles are spread over as they start.
#define FIRST_SPAWN_FRAMES 16

#define Decompress_ByteLz ((void (*)(const void *, void *))Decompress_ByteLz)

/// Unused: nothing calls FieldMotes_StartFar. Starts the 64 floating motes
/// (FieldMotes_Update): loads their tiles and puts each under the leader with
/// staggered timers, drawn semi-transparent.
void FieldMotes_Start(void) {
  struct WeatherState *state;
  struct GroundParticle *mote;
  u8 *tiles;
  u32 i;
  u32 size;
  struct HeapSlots *heap;

  size = sizeof(struct WeatherState);
  state = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, size);
  mote = state->particles;
  size = 0;
  Dma_Fill32(size, state, sizeof(*state));
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_MoteTiles, tiles);
  state->entry = VramCache_FindFreeEntry();
  state->tileBase = VramCache_Load(state->entry, 768, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  i = 0;
  heap = Ram_HeapSlots;
  for (; i < WEATHER_PARTICLE_COUNT; i++) {
    struct MapState *view = heap->mapGrid;
    s32 *pos;
    // p walks the sprite's attribute words, then holds the x, as size carries
    // the fill's 0 above.
    u32 p;
    s32 z;

    p = (u32)mote;
    *(s32 *)p = 0;
    p += sizeof(s32);
    pos = view->origin;
    *(s32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    p += sizeof(s32);
    *(s32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    mote->timer = (i & (FIRST_SPAWN_FRAMES - 1)) + 1;
    mote++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(FieldMotes_Update, TASK_PASS_FRAME);
}

#undef Decompress_ByteLz

/// Starts Weather_UpdateRain's raindrops: allocates and clears
/// HEAP_SLOT_WEATHER, decodes the streak tiles into VRAM, sets each of the 64
/// drops' sprite words and places it on the ground at the map's focus with its
/// first fall staggered, turns on alpha blending of all layers at 16/16,
/// schedules the update and, when asked, plays SE_KIRIN (Scene_QueueMusic).
void Weather_StartRain(s32 withCue) {
  struct WeatherState *state;
  struct GroundParticle *particle;
  u8 *tiles;
  u32 i;
  u32 size;
  struct HeapSlots *heap;

  size = sizeof(struct WeatherState);
  state = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, size);
  particle = state->particles;
  size = 0;
  Dma_Fill32(size, state, sizeof(*state));
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_StreakTiles, tiles);
  state->entry = VramCache_FindFreeEntry();
  state->tileBase = VramCache_Load(state->entry, 768, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  i = 0;
  heap = Ram_HeapSlots;
  for (; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
    struct MapState *view = heap->mapGrid;
    s32 *focus;
    // p walks the sprite's attribute words, then holds the x, as size carries
    // the fill's 0 above.
    u32 p;
    s32 z;

    p = (u32)particle;
    *(s32 *)p = 0;
    p += sizeof(s32);
    focus = view->origin;
    *(s32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    p += sizeof(s32);
    *(s32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    // Timers 1 to 4 and 9 to 12, by the drop's index.
    particle->timer = (i & 0xb) + 1;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(Weather_UpdateRain, TASK_PASS_FRAME);
  if (withCue != 0)
    Scene_QueueMusic(SE_KIRIN);
}

/// Stops Weather_StartRain's particles. Unused: nothing calls
/// Weather_StopRainFar.
void Weather_StopRain(void) { Task_Remove(Weather_UpdateRain); }

/// The sparkles each frame: now and then a burst of four rises from a random
/// spot within 128 pixels of the map's focus, each a frame of
/// gFieldEffect_SparklePictures every two frames and started 4 frames after the
/// one before; FLAG_WEATHER_HELD holds them where they are.
void FieldEffect_UpdateSparkles(void) {
  struct WeatherState *state;
  u32 spawned;
  u32 i;
  struct MapState *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct GroundParticle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 screenX;
  s32 screenY;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  state = Ram_HeapSlots->weather;
  spawned = 0;
  map = Ram_HeapSlots->mapGrid;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = state->particles;
  delay = 0;
  screenX = 0;
  screenY = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Get(FLAG_WEATHER_HELD) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gFieldEffect_SparklePictures[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = FIXEDtoINT(sparkle->posX - cameraX) + (((a & 1) + (b & 1)) >> 1);
      screenX = x - 1;
      screenY = (sparkle->posZ - sparkle->posY - cameraZ) / FIXED_ONE +
                *(const s16 *)frame;
      frame++;
      if (x >= -15 && x <= DISPLAY_WIDTH && screenY >= -32 &&
          screenY < DISPLAY_HEIGHT) {
        sparkle->tile = state->tileBase + *frame;
        sparkle->x = screenX;
        sparkle->shape = 0;
        sparkle->size = 1;
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this reads a u32 as unsigned long; UBFIX reads the declared type.
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = screenY;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Oam_AddSpriteAtDepthClamped(sparkle, OAM_DEPTH_WINDOW);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && state->stopped == 0) {
      if (delay != 0) {
        // The rest of a burst, on the first sparkle's spot.
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        // BUG: the ground's height is read at the last sparkle drawn's screen
        // place taken as a 16.16 position, near the map's top left corner,
        // rather than under the burst; the same below.
#ifdef BUGFIX
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, spawnX, spawnZ);
#else
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, screenX, screenY);
#endif
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        // A burst starts at a random spot.
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - toFIXED(128);
        spawnZ = leader[2] + (Random16() << 8) - toFIXED(128);
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
#ifdef BUGFIX
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, spawnX, spawnZ);
#else
        sparkle->posY = MapGrid_GetTerrainHeightFar(0, screenX, screenY);
#endif
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i < WEATHER_PARTICLE_COUNT);
}

#define Decompress_ByteLz ((void (*)(const void *, void *))Decompress_ByteLz)

/// Unused: nothing calls FieldEffect_InitializeSparklesFar; Aqua Rock and the
/// Apojii Islands keep their own copy (shared/ground_particles_start.h). Starts
/// the sparkles (FieldEffect_UpdateSparkles): loads their tiles and lays every
/// sparkle at the map's top left corner with a staggered timer.
void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct WeatherState);
  struct WeatherState *state = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, size);
  struct GroundParticle *p = state->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *heap;

  size = 0;
  Dma_Fill32Cell(zero, size, state, sizeof(struct WeatherState));
  buf = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_SparkleTiles, buf);
  state->entry = VramCache_FindFreeEntry();
  state->tileBase = VramCache_Load(state->entry, 512, buf);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);

  i = 0;
  heap = Ram_HeapSlots;
  clear = 0;
  do {
    struct MapState *view = heap->mapGrid;
    s32 *leader;
    // attr walks the sprite's attribute words, then holds the x.
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(u32);
    leader = view->origin;
    *(u32 *)attr = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    attr += sizeof(u32);
    *(u32 *)attr = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = MapGrid_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & (FIRST_SPAWN_FRAMES - 1)) + 1;
    i++;
    p++;
  } while (i < WEATHER_PARTICLE_COUNT);
  Task_Add(FieldEffect_UpdateSparkles, TASK_PASS_FRAME);
}

/// Starts the storm's lightning (Weather_UpdateLightning): builds the dark
/// (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
/// fade, and arms the first burst to end in 600 frames, repeating.
void Weather_StartLightning(void) {
  struct FieldBlendState *state;
  struct PaletteFade *buffers;
  volatile u32 zero;

  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND,
                              sizeof(struct FieldBlendState));
  buffers = Ram_HeapSlots->paletteFade;
  Dma_Fill32Cell(zero, 0, state, sizeof(*state));
  PaletteFade_Build(PALETTE_TINT_WARM, (u16 *)buffers, (u16 *)state, 1);
  PaletteFade_Build(PALETTE_TINT_PALE, (u16 *)buffers, (u16 *)state->to, 1);
  PaletteFade_CalculateSteps(state->to, state->from, state->delta, 12);
  PaletteFade_Build((s32)state, 0, buffers->target, 1);
  state->timer = 600;
  state->enabled = 1;
  Task_Add(Weather_UpdateLightning, TASK_PASS_FRAME);
}

/// Stops the lightning: no flash for 32767 frames, and none after.
void Weather_StopLightning(void) {
  struct FieldBlendState *state = HeapSlot_AllocEwram(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendState));
  state->timer = 32767;
  state->enabled = 0;
}

/// One last burst of lightning, its first flash two frames from now (timer 12;
/// Weather_UpdateLightning flashes at 11), then no more.
void Weather_LastLightning(void) {
  struct FieldBlendState *state = HeapSlot_AllocEwram(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendState));
  state->timer = 12;
  state->enabled = 0;
}

/// Restarts the lightning: the next burst ends in 128 frames, and bursts
/// repeat.
void Weather_ResumeLightning(void) {
  struct FieldBlendState *state = HeapSlot_AllocEwram(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendState));
  state->timer = 128;
  state->enabled = 1;
}

/// Flashes the palette with the storm's task: one burst of three flashes of
/// palette to, each fading back to palette from (PaletteFade_Build modes) on
/// the BG colours, ending 120 frames from now, and no more after.
void Weather_StartLightningFlash(s32 from, s32 to) {
  struct FieldBlendState *state;
  struct PaletteFade *buffers;
  volatile u32 zero;
  s16 *target;

  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD_BLEND,
                              sizeof(struct FieldBlendState));
  buffers = Ram_HeapSlots->paletteFade;
  Dma_Fill32Cell(zero, 0, state, sizeof(*state));
  PaletteFade_Build(from, (u16 *)buffers, (u16 *)state, 1);
  target = state->to;
  PaletteFade_Build(to, (u16 *)buffers, (u16 *)target, 1);
  PaletteFade_CalculateSteps(target, state->from, state->delta, 12);
  PaletteFade_Build((s32)state, 0, buffers->target, 1);
  state->timer = 120;
  state->enabled = 0;
  Task_Add(Weather_UpdateLightning, TASK_PASS_FRAME);
}

#undef FIRST_SPAWN_FRAMES
