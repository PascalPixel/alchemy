/// @file
/// Timed map changes (the map animations, the blend script and the palette
/// queue), then the ground heights and the collision of cells and the world
/// map, as The Broken Seal's tile_animation.c and terrain.c have them.

#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "heap_release.h"
#include "resource.h"
#include "graphics.h"
#include "module/field/field.h"
#include "module/field/map.h"
#include "module/field/object.h"

extern u8 gWorldMapWork[];

void WorldMap_PreloadCurrentTileBlock(void) {
  struct MapTileWindow *window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 y = 0;
  s32 x0;
  s32 y0;
  s32 layer;
  s32 row;
  s32 col;
  s32 tile;
  struct Data02020000 *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 value;
  u8 *buffer;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    // origin[0] and origin[2], read through a stepped pointer: the only
    // spelling that gives the native's post-increment load (4 of 277 as
    // indexes).
    x = *p++;
    y = p[1];
  }
  x0 = (x - 0x1000000) >> 25;
  y0 = (y - 0x1400000) >> 25;
  for (layer = 0; layer <= 1; layer++) {
    for (row = 0; row <= 1; row++) {
      for (col = 0; col <= 1; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = window->tileIds[tile];
        tile += layer << 10;
        if (WorldMap_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) !=
            0)
          return;
      }
    }
  }
  block = (struct Data02020000 *)Ram_MapBlocks +
          (((x / 0x200000) & 31) + (((y / 0x200000) & 31) << 5));
  Func_0802d088(block->region, 1);
  // One local holds the region, then Func_080c8950's result: set twice, it
  // reads the region into r3 for the divide, as the original does.
  value = block->region;
  switch (value / 10) {
  case 3:
    graphicsSet = 3;
    break;
  case 2:
    graphicsSet = 2;
    break;
  case 4:
    graphicsSet = 1;
    break;
  default:
    graphicsSet = 0;
    break;
  }
  resources = gData_0802ed34[graphicsSet];
  if (resources != window->resources) {
    window->resources = resources;
    window->reloadSteps = 12;
  }
  value = Func_080c8950();
  if (window->reloadSteps == 0)
    return;
  if (value != 0)
    return;
  buffer = Heap_Alloc(0x200);
  switch (window->reloadSteps) {
  case 12: {
    s16 backdropColor = *(s16 *)0x05000000;

    Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = backdropColor;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
    {
      u8 *anim = Data_0202d000;
      BattleEffect_LoadBufferPaletteFar(buffer);
      Decompress_TaggedLz(Resource_GetTableEntry(resources[1]), anim);
      TilesetAnim_StartAll((const u16 *)anim);
    }
    Decompress_TaggedLz(Resource_GetTableEntry(resources[2]), gMapCollision);
    break;
  }
  case 11:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
    break;
  case 10:
    Dma_Set(gData_02038000, (void *)0x06008000, 0x84000800, REG_DMA3);
    break;
  case 9:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
    break;
  case 8:
    Dma_Set(gData_0203a000, (void *)0x0600a000, 0x84000800, REG_DMA3);
    break;
  case 7:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[5]), gData_0203c000);
    break;
  case 6:
    Dma_Set(gData_0203c000, (void *)0x0600c000, 0x84000800, REG_DMA3);
    break;
  case 5:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[6]), gData_0203e000);
    break;
  case 4:
    Dma_Set(gData_0203e000, (void *)0x0600e000, 0x84000800, REG_DMA3);
    break;
  case 3:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gData_02028000);
    break;
  case 1:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), gWorldMapWork);
    break;
  }
  window->reloadSteps--;
  Heap_Release(buffer);
}

#include "system.h"
#include "frame.h"
#include "task.h"

void MapAnimation_PresentPicture(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80b;
  REG_BG3CNT = control;
  control = 0xaa0f;
  REG_BG2CNT = control;
  control = 0x682;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH(barrier): an empty asm on dma, which loads the DMA channel's
   * address before the transfer's operands; without it, 2 of 24 instructions
   * differ in tla-en. */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(Ram_MapCellBuffer, (void *)0x06006000, DMA_COPY_WORDS(0xa000 / 4),
          dma);
}

void TileMap_DrawRowsFromRam(void *source, void *destination);

void MapAnimation_Start(void) {
  u8 *pages = Ram_HeapSlots->unk1c[0];
  struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  u32 *resources;
  s32 one;
  s8 *active;

  active = &work->animationActive;
  one = 1;
  *active = one;
  Task_SuspendAll(TilesetAnim_Update);
  work->picturePending = one;
  resources = work->resources;
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
  if (work->extraTiles != 0)
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
  work->extraTiles = 0;
  Dma_Set((void *)0x06004000, gData_0201c000, DMA_COPY_WORDS(0x2000 / 4),
          REG_DMA3);
  wait(1);
  TileMap_DrawRowsFromRam(pages + (gFrameCount & one) * 0x1400 + 0xc80,
                          Ram_MapCellBuffer);
  work->windowTop = 200;
  work->windowBottom = 255;
  gData_030011f8 = MapAnimation_PresentPicture;
  work->pictureShown = one;
}

void Map_ShowBg1FromBuffer(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80a;
  REG_BG3CNT = control;
  control = 0xaa0e;
  REG_BG2CNT = control;
  control = 0x501;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH(barrier): an empty asm on dma, which loads the DMA channel's
   * address before the transfer's operands; without it, 2 of 24 instructions
   * differ in tla-en. */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(gData_02038000, (void *)0x06008000, DMA_COPY_WORDS(0x8000 / 4), dma);
}

s32 WorldMap_LoadAreaGraphics(void) {
  struct MapViewWork *state;
  u32 *resources;
  u8 *buffer;
  s16 value;

  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  buffer = Ram_MapCellBuffer;
  resources = state->resources;
  value = *(s16 *)0x05000000;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]), gData_0203c000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[6]), gData_0203e000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gData_02028000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), gWorldMapWork);
  gData_030011f8 = Map_ShowBg1FromBuffer;
  state->windowTop = 0;
  state->windowBottom = 159;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      buffer);
  WorldMap_UpdateCurrentTileBlock();
  state->animationActive = 0;
  Task_ResumeAll(TilesetAnim_Update);
  state->picturePending = 0;
  state->pictureShown = 0;
  Func_0802d088(state->area, 0);
  wait(1);
}

void WorldMap_LoadDefaultCellsAndUpdateBlock(void) {
  struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;

  gData_030011f8 = Map_ShowBg1FromBuffer;
  work->windowTop = 0;
  work->windowBottom = 0x9f;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      Ram_MapCellBuffer);
  WorldMap_UpdateCurrentTileBlock();
  Task_ResumeAll(TilesetAnim_Update);
  wait(1);
}

void TilesetAnim_Update(void) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  u32 i;

  for (i = 0; i <= 15; i++) {
    const u16 *script;
    u32 op;
    u32 count;
    u32 dst;

    if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
      continue;
  next:
    if ((state->anim + i)->timer == 0) {
      script = (state->anim + i)->cursor;
      op = *script++;
      if (op == 0xffff) {
        (state->anim + i)->cursor = (state->anim + i)->start;
        goto next;
      }
      if ((op & 0xff00) == 0xfe00) {
        if ((op & 0xff) == 0xff)
          continue;
        (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
        goto next;
      }
      count = *script++;
      dst = script[0];
      (state->anim + i)->timer = script[1];
      if (state->wideTiles == 0) {
        if (op >= 0x600)
          Dma_Set(Ram_DecodeBuffer + op * 32, (void *)(dst * 32 + 0x06004000),
                  DMA_COPY_WORDS(count * 8), REG_DMA3);
        else
          Dma_Set((void *)(op * 32 + 0x06004000),
                  (void *)(dst * 32 + 0x06004000), DMA_COPY_WORDS(count * 8),
                  REG_DMA3);
      } else {
        if (op >= 0x200)
          Dma_Set(Ram_MapBlocks + op * 64, (void *)(dst * 64 + 0x06008000),
                  DMA_COPY_WORDS(count * 16), REG_DMA3);
        else
          Dma_Set((void *)(op * 64 + 0x06008000),
                  (void *)(dst * 64 + 0x06008000), DMA_COPY_WORDS(count * 16),
                  REG_DMA3);
      }
      (state->anim + i)->cursor += 4;
      goto next;
    } else {
      (state->anim + i)->timer--;
    }
  }
}

void Map_ClearLayerEntryFlag(u32 no) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 1;
}

#include "iwram_call.h"

void TilesetAnim_StartAll(const u16 *script) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  s32 count;
  u32 command;

  count = 0;
  Iwram_ClearWordsEntry(state->anim, sizeof(state->anim));
  command = *script++;
  while (command != 0xffff) {
    if ((command & 0xff00) == 0xfd00) {
      struct MapAnimation *anim;
      u32 idx = command & 15;
      u32 flag = 0;

      if (command & 0x80)
        flag = 1;
      anim = &state->anim[idx];
      anim->start = script;
      anim->cursor = script;
      anim->timer = 0;
      anim->paused = flag;
      count++;
    }
    command = *script++;
  }
  if (count != 0 && Task_Find(TilesetAnim_Update) == -1)
    Task_Add(TilesetAnim_Update, TASK_PASS_VBLANK);
}

void TilesetAnim_SuspendAll(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_SuspendAll(TilesetAnim_Update);
}

void TilesetAnim_ResumeAll(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_ResumeAll(TilesetAnim_Update);
}

void BlendScript_Run(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct BlendScriptState *state;
  u16 *cursor;
  u32 command;
  u32 value;
  u16 test;

  state = &work->blend;
  if (state->script == 0)
    return;
  if (state->paused != 0)
    return;

again:
  value = state->delay;
  test = value;
  if (test != 0)
    goto count_down;

  cursor = state->cursor;
  command = *cursor;
  cursor++;
  if (command == 0xffff) {
    state->cursor = state->script;
    goto again;
  }

  if ((command & 0xff00) == 0xfe00) {
    value = command & 0xff;
    if (value == 0xff)
      return;
    state->cursor = state->script + value * 2;
    goto again;
  }

  if ((command & 0xf000) == 0x3000) {
    REG_BLDCNT = command;
    work->blendControl = command;
    state->cursor++;
    goto again;
  }

  if ((work->blendControl & 0xc0) == 0x40)
    REG_BLDALPHA = command;
  else
    REG_BLDY = command;
  state->delay = cursor[0];
  state->cursor += 2;
  goto again;

count_down:
  state->delay = value + 0xffff;
}

void BlendScript_Start(u16 *script) {
  struct BlendScriptState *state;
  s32 started;
  volatile u32 zero;
  u32 first;

  started = 0;
  zero = 0;
  state = &Ram_HeapSlots->mapWork->blend;
  Dma_Set((const void *)&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 3, REG_DMA3);
  first = *script;
  if (first != 0xffff) {
    state->script = script;
    state->cursor = script;
    state->delay = 0;
    state->paused = 0;
    started = 1;
  }
  if (started)
    Task_Add((s32)(BlendScript_Run), TASK_PASS_VBLANK);
}

void BlendScript_Suspend(void) { Task_SuspendAll((u32)BlendScript_Run); }

void BlendScript_Resume(void) { Task_ResumeAll((u32)BlendScript_Run); }

#include "palette_snapshots.h"

void PaletteCycle_Initialize(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  u16 i;
  u16 j;

  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
  for (i = 0; i != 4; i++) {
    entry->source = 0;
    entry->position = 0;
    entry->timer = 0;
    entry->delay = 0;
    entry->count = 0;
    for (j = 0; j != 16; j++) {
      entry->colors[j] = 0;
    }
    entry++;
  }
  queue->count = 0;
}

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unk70[0])

#include "../palqueue_add.c"

void PaletteQueue_Stop(void) {
  Task_Remove(PaletteCycle_Step);
  HeapSlot_Release(112);
}

void PaletteCycle_Start(void) { Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK); }

void PaletteCycle_Step(void) {
  struct PaletteSnapshotWork *work = gPaletteWork;
  u8 i;
  u16 count = work->count & 3;
  u16 buf[16];

  for (i = 0; i < count; i++) {
    struct PaletteSnapshot *cycle = &work->entries[i];
    if (cycle->timer == 0) {
      u16 start = cycle->position;
      u16 len = cycle->count;
      void *dest = cycle->source;
      u16 *src = cycle->colors;
      u8 j;
      for (j = len - start; j < len; j++)
        buf[j] = *src++;
      for (j = 0; j < len - start; j++)
        buf[j] = *src++;
      Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
      start++;
      if (start >= len)
        start = 0;
      cycle->position = start;
      cycle->timer = cycle->delay;
    } else {
      cycle->timer--;
    }
  }
}

#include "io_write_queue.h"
#include "flag_call.h"

void Func_0802d088(s32 region, s32 gradual) {
  struct WorldTint *work = (struct WorldTint *)Ram_HeapSlots->mapWork;

  if (work->region != region) {
    work->region = region;
    if (region == 9)
      work->target = 12;
    else if (region == 18 && !GameFlag_Test(0x8ef))
      work->target = -6;
    else
      work->target = 0;
  }
  if (GameFlag_Test(0x106))
    return;
  if (gradual == 0) {
    work->tint = work->target;
  } else {
    region = work->target - work->tint;
    if (region == 0)
      return;
    if (gFrameCount & 15)
      return;
    if (region < 0)
      region = -1;
    else
      region = 1;
    work->tint += region;
  }
  if (work->tint > 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    {
      struct IoWriteQueue *q = &gIoWriteQueue;
      u32 saved;
      s32 count;

      IRQ_LOCK(saved);
      count = q->count;
      if (count <= 31) {
        u32 *entry;
        /* FAKEMATCH(temporary): the entry's byte offset held in offset and
         * scaled after the count store; as u32 *entry = q->entries[count]
         * before the store, 21 of 210 instructions differ in tla-en. */
        s32 offset = count * 3;

        q->count = count + 1;
        offset *= sizeof(u32);
        /* FAKEMATCH(barrier): an empty asm keeps the tint's load after the
         * offset's scaling; without it, 2 of 210 instructions differ in
         * tla-en. */
        asm("");
        entry = (u32 *)((u8 *)q->entries + offset);
        *entry++ = (u16)work->tint;
        *entry++ = 0x4000054;
        *entry = 0x20000;
      }
      IRQ_UNLOCK(saved);
    }
  } else {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)-work->tint, 0x20000);
  }
}

#include "curve.h"

s32 Terrain_GetFlatHeight(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Terrain_GetSlopeXHeight(const s8 *samples, s32 position) {
  s32 start;
  s32 delta;

  start = samples[0] << CURVE_VALUE_SHIFT;
  delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * position;
  if (delta < 0) {
    delta += CURVE_FULL_STEPS - 1;
  }
  return start + (delta >> 4);
}

s32 Terrain_GetSlopeYHeight(const s8 *samples, s32 unused, s32 position) {
  s32 base;
  s32 offset;

  base = samples[0] << CURVE_VALUE_SHIFT;
  offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * position;
  if (offset < 0) {
    offset += CURVE_FULL_STEPS - 1;
  }
  return base + (offset >> 4);
}

s32 Terrain_GetStepSumDiagonalHeight(const s8 *samples, s32 start, s32 end) {
  s32 first;
  s32 second;
  s32 maximum;
  u32 position;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  position = start + end;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

s32 Terrain_GetStepDifferenceDiagonalHeight(const s8 *samples, s32 start,
                                            s32 end) {
  s32 first;
  s32 second;
  s32 maximum;
  s32 difference;
  u32 position = start;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  difference = end - position;
  position = difference + CURVE_FULL_STEPS - 1;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

s32 Terrain_GetBendSumDiagonalHeight(const s8 *samples, s32 start, s32 end) {
  s32 first;
  s32 middle;
  s32 last;
  s32 result;

  first = *samples++ << CURVE_VALUE_SHIFT;
  middle = *samples << CURVE_VALUE_SHIFT;
  last = samples[1] << CURVE_VALUE_SHIFT;
  start = start + end;
  if ((u32)start == CURVE_FULL_STEPS - 1) {
    result = middle;
  } else if ((u32)start < CURVE_FULL_STEPS - 1) {
    result = first + (middle - first) * start / (CURVE_FULL_STEPS - 1);
  } else {
    start = start - (CURVE_FULL_STEPS - 1);
    result = middle + (last - middle) * start / (CURVE_FULL_STEPS - 1);
  }
  return result;
}

#include "fixed_math.h"

s32 Terrain_GetBendDifferenceDiagonalHeight(const s8 *samples, s32 start,
                                            s32 end) {
  s32 first;
  s32 second;
  s32 third;
  s32 difference;
  s32 delta;

  first = *samples++ << CURVE_VALUE_SHIFT;
  second = samples[0] << CURVE_VALUE_SHIFT;
  third = samples[1];
  difference = end - start;
  start = difference + CURVE_FULL_STEPS - 1;
  third <<= CURVE_VALUE_SHIFT;

  if (start == CURVE_FULL_STEPS - 1)
    return second;

  if ((u32)start < CURVE_FULL_STEPS - 1) {
    delta = second - first;
    difference = delta * start;
    if (difference < 0)
      difference += CURVE_FULL_STEPS - 1;
    difference >>= 4;
    return first + difference;
  } else {
    difference *= third - second;
    if (difference < 0)
      difference += CURVE_FULL_STEPS - 1;
    difference >>= 4;
    return second + difference;
  }
}

s32 Terrain_GetRoundedSquareHeight(const s8 *samples, s32 position, s32 row) {
  s32 start;
  s32 end;
  s32 weight;
  u8 *table;

  start = samples[0];
  end = samples[1];
  table = gTerrain_LerpWeightTable;
  /* FAKEMATCH(barrier): an empty asm reading the table's address keeps its
   * load after both samples' and before the row's scaling; without it, 15 of
   * 15 instructions differ in tla-en. */
  __asm__("" : : "r"(table));
  position += row * CURVE_FULL_STEPS;
  weight = table[position];
  start <<= CURVE_VALUE_SHIFT;
  return start + (((end << CURVE_VALUE_SHIFT) - start) * weight);
}

s32 Terrain_GetStepXHeight(const s8 *samples, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

s32 Terrain_GetStepYHeight(const s8 *samples, u32 unused, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableHeight with x and y swapped.
s32 Terrain_GetSampleTableTransposedHeight(const s8 *samples, u32 pixelX,
                                           u32 pixelY) {
  return samples[gTerrain_SampleIndexTable[pixelY + (pixelX << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableMirroredHeight with x and y swapped.
s32 Terrain_GetSampleTableTransposedMirroredHeight(const s8 *samples, u32 row,
                                                   u32 position) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// The sample gTerrain_SampleIndexTable picks for (x, y): a byte per pixel
/// of the cell, row by row.
s32 Terrain_GetSampleTableHeight(const s8 *samples, u32 position, u32 row) {
  return samples[gTerrain_SampleIndexTable[position + (row << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// The same, with the table mirrored left to right.
s32 Terrain_GetSampleTableMirroredHeight(const s8 *samples, u32 position,
                                         u32 row) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// Above y 8 a step along x from the first sample to the second; below, the
/// third.
s32 Terrain_GetSplitTopStepHeight(const s8 *samples, u32 column, u32 row) {
  s32 value;

  if (row < CURVE_HALF_STEPS) {
    if (column < CURVE_HALF_STEPS)
      value = samples[0];
    else
      value = samples[1];
  } else {
    value = samples[2];
  }
  return value << CURVE_VALUE_SHIFT;
}

/// Above y 8 the first sample; below, a step along x from the second to the
/// third.
s32 Terrain_GetSplitBottomStepHeight(const s8 *samples, u32 column, u32 row) {
  s32 value;

  if (row < CURVE_HALF_STEPS)
    value = samples[0];
  else if (column < CURVE_HALF_STEPS)
    value = samples[1];
  else
    value = samples[2];
  return value << CURVE_VALUE_SHIFT;
}

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 z);

extern TerrainHeightFn gMapGrid_TerrainHeightFunctions[16];

/// Returns the ground height of a layer at (x, z) in 16.16 pixels, as The
/// Broken Seal's: the height function of the cell's terrain kind applied to
/// its samples.
s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  grid += x / 16 + (z / 16 << 7);
  code = Ram_MapCollision + *grid * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15);
}

#include "edition.h"

extern u8 gMapCellBuffer[];

/// Returns the terrain kind of a layer's cell at (x, z) in 16.16 pixels, as
/// The Broken Seal's.
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

/// As MapGrid_GetCellAttributeLowNibble, but a rounded square whose two
/// samples differ (a real bump or hollow, not a flat cell) reads as 99.
s32 Func_0802d530(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;
  s32 kind;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  code = &Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4];
  kind = code[0];
  if (kind == 7 && code[1] != code[2])
    kind = 99;
  return kind;
}

/// The highest ground at (x, z) and at the four points `reach` away from it
/// along x and z, all in 16.16 pixels.
s32 MapGrid_GetHighestTerrainHeightAround(s32 layer, s32 x, s32 z, s32 reach) {
  s32 best;
  s32 height;

  best = MapGrid_GetTerrainHeight(layer, x, z);
  height = MapGrid_GetTerrainHeight(layer, x - reach, z);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x + reach, z);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x, z - reach);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x, z + reach);
  if (best < height)
    best = height;
  return best;
}

void Func_0802d600(s32 x, s32 z, s32 layer, s32 value) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *shape;

  shape = gMapShapeGrid +
          (work->layers[layer & 3].cells - (struct MapCell *)Ram_MapCellBuffer);
  shape += (x >> 20) + ((z >> 20) << 7);
  *shape = Func_080c8730(layer, 0, value);
}

s32 Func_0802d644(s32 a, s32 b, s32 c) {
  return MapGrid_GetTerrainHeight(c, a, b) >> 19;
}

/// As MapGrid_GetCellAttributeLowNibble, a second copy.
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

u8 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 y) {
  struct MapWork *state;
  struct MapCell *cells;

  state = Ram_HeapSlots->mapWork;
  x >>= 20;
  y >>= 20;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + (y << 7);
  return cells->collisionCode;
}

void MapGrid_SetCellBehaviorAt(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapWork *state = Ram_HeapSlots->mapWork;

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cells = state->layers[layer & 3].cells;

    cells += x + (y << 7);
    cells->collisionCode = collisionCode;
  }
}

/// The height of the ground at (px, pz) (16.16 pixels) on the given layer,
/// less base; a layer past 2 reads the shape grid. With no map, or while the
/// event mode is 3, it is 0.
s32 MapGrid_GetTerrainHeightAbove(s32 px, s32 pz, s32 layer, s32 base) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *shapes;
  u8 *code;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (layer <= 2)
    shapes = map->layers[layer].terrainIds;
  else
    shapes = gMapShapeGrid;
  code = Ram_MapCollision + shapes[x / 16 + ((z / 16) << 7)] * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         base;
}

/// How the ground at (px, pz) (16.16 pixels) on the given layer stands to
/// height: 2 when the cell has no tile, 1 when the ground rises more than 8
/// pixels above it, -1 when it falls more than 12 below, else 0. A layer past
/// 2 reads the cell buffer and shape grid. With no map, or while the event
/// mode is 3, every point is level (0).
s32 MapGrid_CheckCollisionAt(s32 px, s32 pz, s32 layer, s32 height) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (layer <= 2) {
    cells = (u8 *)map->layers[layer].cells;
    shapes = map->layers[layer].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         height;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollisionAt for the point position (16.16 pixels, read as
/// its whole-pixel halves) on the actor's layer, against its ground height.
s32 MapGrid_CheckCollision(struct Actor *actor, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (actor->terrainId <= 2) {
    cells = (u8 *)map->layers[actor->terrainId].cells;
    shapes = map->layers[actor->terrainId].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         actor->terrainHeight;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollision, against the actor's height and with no -1: 2
/// when the cell has no tile, 1 when the ground rises more than 8 pixels above
/// it, else 0.
s32 MapGrid_CheckCollisionAbove(struct Actor *actor, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (actor->terrainId <= 2) {
    cells = (u8 *)map->layers[actor->terrainId].cells;
    shapes = map->layers[actor->terrainId].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  if (gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
          actor->y >
      8 << 16)
    return 1;
  return 0;
}

s32 MapGrid_IsWallAt(s32 *position) {
  s32 x = position[0] / 65536;
  s32 z = (u32)((position[2] & 0xfff00000) - position[1]) >> 16;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  cell = *(u8 **)(map + 0x1a8);
  cell += (x / 16 + (z / 16) * 128) * 4;
  return (cell[2] != 255) - 1;
}

/* VIEW(f02c58c): variable gMapBlocks as struct WorldMapBlock gMapBlocks[];
 * module/tla.h has u32 gMapBlocks[]; tried as the rest of the object declares
 * it, it does not compile in tla-ja:
 * games/tla/src/field/map/tile_animation.c:1131: request for member `unk2' in
 * something not a structure or union. */
extern struct WorldMapBlock gMapBlocks__f02c58c[] __asm__("gMapBlocks");

/// Reads byte 2 of the world map block under position: blocks are 32 pixels
/// square on a 32 by 32 grid, positions are Q16 pixels.
s32 Func_0802da88(struct WorldPosition *position) {
  s32 z = position->y;
  s32 x = position->x;

  if (x < 0)
    x += 0x1fffff;
  x = x >> 21 & 31;
  if (z < 0)
    z += 0x1fffff;
  z = z >> 21 & 31;
  return gMapBlocks__f02c58c[x + z * 32].unk2;
}

s32 WorldMap_GetCollision(struct WorldPosition *position) {
  s32 x_step = position->x >> 17;
  s32 y = position->y;
  s32 y_step = y >> 17;
  u32 cell;
  u32 pixel_offset;
  u8 tile;
  u8 packed_pixels;
  u32 result;

  cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
  pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

  tile = BG_SCREEN_BLOCK(10)[cell];
  packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  tile = BG_SCREEN_BLOCK(8)[cell];
  packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  return 7;
}

s32 WorldMap_IsWalkable(void *object, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    if ((u32)(WorldMap_GetCollision(position) - 5) <= 7)
      return 0;
  }
  return -1;
}

s32 Func_0802db88(void *object, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    s32 collision = WorldMap_GetCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;
    s32 mode;

    mode = *(s8 *)(work + 0xd3a);
    if (mode == 0) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 1) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 2) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    }
  }
  return -1;
}

s32 Func_0802dbd0(struct WorldMover *mover, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    s32 collision = WorldMap_GetCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;

    s32 height = mover->height;

    if (height < 0x40000) {
      if ((u32)(collision - 1) > 3 && collision != 6)
        return -1;
      return 0;
    } else {
      s32 mode = *(s8 *)(work + 0xd3a);

      if (mode == 0) {
        if ((u32)(collision - 1) <= 5)
          return 0;
      } else if (mode == 1) {
        if ((u32)(collision - 1) <= 7)
          return 0;
      } else {
        if (mode == 2) {
          if ((u32)(collision - 1) <= 7)
            return 0;
        } else {
          if ((u32)(collision - 4) <= 4)
            return 0;
        }
        if (collision == 10)
          return 0;
      }
    }
  }
  return -1;
}
