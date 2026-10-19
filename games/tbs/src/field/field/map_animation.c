// Part of field.c, the field object, compiled only through it.
// Timed map effects: tile animation, the blend script, palette cycles.

#include "../field_internal.h"

// Each frame: runs the 16 tile animation channels. A channel script is
// groups of four halfwords (source tile, count, destination tile, frames to
// wait) that copy tiles into VRAM; 0xfeNN jumps to halfword offset NN * 2,
// 0xfeff stops, and 0xffff loops to the start.
// The retries stay gotos: as an inner for (;;) loop with continue and break,
// loop.c reshapes the channel loop.
void MapAnimation_Update(void) {
  // Each channel is written (mapState->anim + channelIndex): as
  // mapState->anim[channelIndex], 78 of 119 instructions differ in tbs-en, and
  // a channel pointer swaps ip and lr.
  struct MapState *mapState = gMapWork[0];
  u32 channelIndex;

  for (channelIndex = 0; channelIndex <= 15; channelIndex++) {
    const u16 *commands;
    u32 sourceTileOrOpcode;
    u32 tileCount;
    u32 destinationTile;

    if ((mapState->anim + channelIndex)->start == NULL ||
        (mapState->anim + channelIndex)->paused != 0)
      continue;
  next:
    if ((mapState->anim + channelIndex)->timer == 0) {
      commands = (mapState->anim + channelIndex)->cursor;
      sourceTileOrOpcode = *commands++;
      if (sourceTileOrOpcode == 0xffff) {
        (mapState->anim + channelIndex)->cursor =
            (mapState->anim + channelIndex)->start;
        goto next;
      }
      if ((sourceTileOrOpcode & 0xff00) == 0xfe00) {
        if ((sourceTileOrOpcode & 0xff) == 0xff)
          continue;
        (mapState->anim + channelIndex)->cursor =
            (mapState->anim + channelIndex)->start +
            (sourceTileOrOpcode & 0xff) * 2;
        goto next;
      }
      tileCount = *commands++;
      destinationTile = commands[0];
      (mapState->anim + channelIndex)->timer = commands[1];
      if (mapState->wideTiles == 0) {
        if (sourceTileOrOpcode >= 0x600)
          Dma_Set(Ram_DecodeBuffer + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  (DMA_ENABLE | DMA_32BIT) << 16 | tileCount * 8, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(1) + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  (DMA_ENABLE | DMA_32BIT) << 16 | tileCount * 8, REG_DMA3);
      } else {
        if (sourceTileOrOpcode >= 0x200)
          Dma_Set(Ram_MapBlocks + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  (DMA_ENABLE | DMA_32BIT) << 16 | tileCount * 16, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(2) + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  (DMA_ENABLE | DMA_32BIT) << 16 | tileCount * 16, REG_DMA3);
      }
      (mapState->anim + channelIndex)->cursor += 4;
      goto next;
    } else {
      (mapState->anim + channelIndex)->timer--;
    }
  }
}

// Resumes tile animation channel no.
void Map_ClearLayerEntryFlag(u32 channelIndex) {
  struct MapState *mapState = gMapWork[0];
  struct MapAnimation *channel = &mapState->anim[channelIndex];

  channel->paused = 0;
}

// Pauses tile animation channel no.
void Map_SetLayerEntryFlag(u32 channelIndex) {
  struct MapState *mapState = gMapWork[0];
  struct MapAnimation *channel = &mapState->anim[channelIndex];

  channel->paused = 1;
}

// Starts the tile animation channels a map's animation script lists: each
// 0xfdNN marker starts channel NN & 15 there (paused when bit 7 is set).
void MapAnimation_StartChannels(const u16 *script) {
  struct MapState *mapState;
  s32 channelsStarted;
  u32 marker;

  mapState = gMapWork[0];
  channelsStarted = 0;
  Dma_Fill32(0, mapState->anim, sizeof(mapState->anim));
  marker = *script++;
  while (marker != 0xffff) {
    if ((marker & 0xff00) == 0xfd00) {
      struct MapAnimation *channel;
      u32 channelIndex = marker & 15;
      u32 paused = 0;
      if (marker & 0x80)
        paused = 1;
      channel = &mapState->anim[channelIndex];
      channel->start = script;
      channel->cursor = script;
      channel->timer = 0;
      channel->paused = paused;
      channelsStarted++;
    }
    marker = *script++;
  }
  if (channelsStarted != 0)
    Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

// Suspends the tile animations, unless a BG1 animation is running.
void Map_EnableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_SuspendTasks(MapAnimation_Update);
}

// Resumes the tile animations, unless a BG1 animation is running.
void Map_DisableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_ResumeTasks(MapAnimation_Update);
}

// Each frame: runs the map's blend script. A 0x3NNN command sets BLDCNT;
// any other sets BLDALPHA (or BLDY when not alpha blending) and waits the
// halfword after it in frames; 0xfeNN jumps, 0xfeff stops and 0xffff loops.
// The retries stay gotos: as a for (;;) loop with continue, loop.c hoists
// blendControl's address and 118 bytes of tbs-en's code change.
void DisplayBlend_RunScript(void) {
  struct MapState *mapState = gMapWork[0];
  struct BlendScriptState *blend;
  u16 *commands;
  u32 instruction;
  u32 delayOrJumpIndex;
  u16 delay16;

  blend = &mapState->blend;
  if (blend->script == 0)
    return;
  if (blend->paused != 0)
    return;

again:
  delayOrJumpIndex = blend->delay;
  delay16 = delayOrJumpIndex;
  if (delay16 == 0) {
    commands = blend->cursor;
    instruction = *commands;
    commands++;
    if (instruction == 0xffff) {
      blend->cursor = blend->script;
      goto again;
    }

    if ((instruction & 0xff00) == 0xfe00) {
      delayOrJumpIndex = instruction & 0xff;
      if (delayOrJumpIndex == 0xff)
        return;
      blend->cursor = blend->script + delayOrJumpIndex * 2;
      goto again;
    }

    if ((instruction & 0xf000) == 0x3000) {
      REG_BLDCNT = instruction;
      mapState->blendControl = instruction;
      blend->cursor++;
      goto again;
    }

    if ((mapState->blendControl & 0xc0) == 0x40)
      REG_BLDALPHA = instruction;
    else
      REG_BLDY = instruction;
    blend->delay = commands[0];
    blend->cursor += 2;
    goto again;
  }
  blend->delay = delayOrJumpIndex + 0xffff;
}

// Starts a map's blend script, unless it is empty.
void DisplayBlend_StartScript(u16 *script) {
  struct BlendScriptState *blend;
  s32 hasScript;
  volatile u32 zero;

  hasScript = 0;
  zero = 0;
  blend = &((struct MapState *)gMapWork[0])->blend;
  Dma_Set((const void *)&zero, blend,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 3, REG_DMA3);
  if (*script != 0xffff) {
    blend->script = script;
    blend->cursor = script;
    blend->delay = 0;
    blend->paused = 0;
    hasScript = 1;
  }
  if (hasScript)
    Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

// Suspends the blend script.
void DisplayBlend_EnableRunScript(void) {
  Scheduler_SuspendTasks(DisplayBlend_RunScript);
}

// Resumes the blend script.
void DisplayBlend_DisableRunScript(void) {
  Scheduler_ResumeTasks(DisplayBlend_RunScript);
}

// Allocates the palette cycling queue (heap slot 28) with its four entries
// cleared.
UNUSED void PaletteQueue_Initialize(void) {
  struct PaletteSnapshotWork *paletteCycles;
  struct PaletteSnapshot *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
  cycle = paletteCycles->entries;
  for (cycleIndex = 0; cycleIndex != 4; cycleIndex++) {
    cycle->source = 0;
    cycle->position = 0;
    cycle->timer = 0;
    cycle->delay = 0;
    cycle->count = 0;
    for (colorIndex = 0; colorIndex != 16; colorIndex++) {
      cycle->colors[colorIndex] = 0;
    }
    cycle++;
  }
  paletteCycles->count = 0;
}

// Adds a palette cycle: colorCount colours of background palette paletteBank
// from firstColor, rotated by one every delay + 1 frames. Returns 0, or -1 when
// all four cycles are taken.
UNUSED s32 PaletteQueue_Add(s16 paletteBank, s16 firstColor, s16 delay,
                            s16 colorCount) {
  struct PaletteSnapshotWork *paletteQueue;
  struct PaletteSnapshot *cycle;
  u16 *paletteColors;
  u32 cycleIndex;
  u16 colorCount16;

  paletteQueue = gPaletteWork;
  cycleIndex = paletteQueue->count;
  if (cycleIndex > 3)
    return -1;
  cycle = &paletteQueue->entries[cycleIndex];
  paletteColors = (u16 *)BG_PLTT + (((u16)paletteBank << 4) + (u16)firstColor);
  colorCount16 = colorCount;
  cycle->position = 0;
  cycle->timer = 0;
  cycle->count = colorCount16;
  cycle->source = paletteColors;
  cycle->delay = delay;
  Dma_Set(paletteColors, cycle->colors, DMA_ENABLE << 16 | colorCount16,
          REG_DMA3);
  paletteQueue->count++;
  return 0;
}

// Stops the palette cycles and frees their queue (heap slot 28).
UNUSED void PaletteQueue_Stop(void) {
  Scheduler_RemoveCallback(PaletteQueue_StepCycles);
  Runtime_ReleaseHeapBlock(28);
}

// Starts running the palette cycles each frame.
UNUSED void PaletteQueue_Start(void) {
  Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

// Each frame: advances each palette cycle whose timer has run out by one
// colour, writing the rotated colours back to the palette.
void PaletteQueue_StepCycles(void) {
  struct PaletteSnapshotWork *paletteQueue = gPaletteWork;
  u8 cycleIndex;
  // BUG: four queued cycles become zero here, so none advance.
  u16 activeCycles = paletteQueue->count & 3;
  u16 rotatedColors[16];

  for (cycleIndex = 0; cycleIndex < activeCycles; cycleIndex++) {
    struct PaletteSnapshot *cycle = &paletteQueue->entries[cycleIndex];
    if (cycle->timer == 0) {
      u16 rotationOffset = cycle->position;
      u16 colorCount = cycle->count;
      void *paletteColors = cycle->source;
      u16 *savedColors = cycle->colors;
      u8 colorIndex;
      for (colorIndex = colorCount - rotationOffset; colorIndex < colorCount;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      for (colorIndex = 0; colorIndex < colorCount - rotationOffset;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      Dma_Set(rotatedColors, paletteColors, DMA_ENABLE << 16 | colorCount,
              REG_DMA3);
      rotationOffset++;
      if (rotationOffset >= colorCount)
        rotationOffset = 0;
      cycle->position = rotationOffset;
      cycle->timer = cycle->delay;
    } else {
      cycle->timer--;
    }
  }
}
