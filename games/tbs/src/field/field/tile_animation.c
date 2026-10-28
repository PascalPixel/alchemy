/// @file
/// Timed map changes: tileset animations, the blend script, palette cycles.
// Part of field.c.

#include "../field_internal.h"

// A tileset animation script's commands, a halfword each where a source tile
// would stand (TilesetAnim_Update and TilesetAnim_StartAll).
#define TILESET_ANIM_LOOP 0xffff        ///< back to the start of the script
#define TILESET_ANIM_OPCODE_MASK 0xff00 ///< the high byte: the command
#define TILESET_ANIM_JUMP 0xfe00   ///< low byte: the halfword pair to jump to
#define TILESET_ANIM_STOP 0xff     ///< TILESET_ANIM_JUMP's low byte to stop
#define TILESET_ANIM_MARKER 0xfd00 ///< low four bits: the animation it starts
#define TILESET_ANIM_MARKER_PAUSED 0x80 ///< a marker's bit: start it paused
// The first source tile read from gMapGridAnimTiles rather than VRAM, for
// 32-byte and for 64-byte (wide) tiles: both bases put it at its start.
#define TILESET_ANIM_RAM_TILE 0x600
#define TILESET_ANIM_RAM_WIDE_TILE 0x200

// The blend script's commands (BlendScript_Run): as the tileset animations',
// and a BLDCNT value, told apart by its top four bits.
#define BLEND_SCRIPT_LOOP 0xffff
#define BLEND_SCRIPT_OPCODE_MASK 0xff00
#define BLEND_SCRIPT_JUMP 0xfe00
#define BLEND_SCRIPT_STOP 0xff
#define BLEND_SCRIPT_BLDCNT_MASK 0xf000
#define BLEND_SCRIPT_BLDCNT (BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD)

/// Each frame: runs the 16 tileset animations. A tileset animation's script is
/// groups of four halfwords (source tile, count, destination tile, frames to
/// wait) that copy tiles into VRAM; TILESET_ANIM_JUMP jumps to the halfword
/// pair its low byte names, with TILESET_ANIM_STOP it stops, and
/// TILESET_ANIM_LOOP loops to the start. TilesetAnim_StartAll adds it as a task
/// (priority TASK_PASS_FRAME); the world map's still view
/// (WorldMap_StartStillPicture) suspends it. Moves each tileset animation's
/// cursor and timer in the map work.
void TilesetAnim_Update(void) {
  // Each tileset animation is written (gridState->animations + channelIndex):
  // as gridState->animations[channelIndex], 78 of 119 instructions differ in
  // tbs-en, and a pointer to it swaps ip and lr. The retries stay gotos: as
  // an inner for (;;) loop with continue and break, loop.c reshapes the outer
  // loop.
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  u32 channelIndex;

  for (channelIndex = 0; channelIndex <= 15; channelIndex++) {
    const u16 *commands;
    u32 sourceTileOrOpcode;
    u32 tileCount;
    u32 destinationTile;

    // A tileset animation with no script, or paused by an event, keeps its
    // place.
    if ((gridState->animations + channelIndex)->start == NULL ||
        (gridState->animations + channelIndex)->paused != 0)
      continue;
    // While the wait lasts the tileset animation only counts down; when it is
    // over, commands run in this same frame until one sets a new wait or stops.
  next:
    if ((gridState->animations + channelIndex)->timer == 0) {
      commands = (gridState->animations + channelIndex)->cursor;
      sourceTileOrOpcode = *commands++;
      if (sourceTileOrOpcode == TILESET_ANIM_LOOP) {
        (gridState->animations + channelIndex)->cursor =
            (gridState->animations + channelIndex)->start;
        goto next;
      }
      // TILESET_ANIM_STOP leaves the cursor on itself, so the tileset animation
      // stays stopped until it is started again; TILESET_ANIM_JUMP jumps to
      // the halfword pair its low byte names.
      if ((sourceTileOrOpcode & TILESET_ANIM_OPCODE_MASK) ==
          TILESET_ANIM_JUMP) {
        if ((sourceTileOrOpcode & 0xff) == TILESET_ANIM_STOP)
          continue;
        (gridState->animations + channelIndex)->cursor =
            (gridState->animations + channelIndex)->start +
            (sourceTileOrOpcode & 0xff) * 2;
        goto next;
      }
      tileCount = *commands++;
      destinationTile = commands[0];
      (gridState->animations + channelIndex)->timer = commands[1];
      // A source tile from TILESET_ANIM_RAM_TILE (TILESET_ANIM_RAM_WIDE_TILE
      // for 64-byte tiles) is read from RAM, at the start of gMapGridAnimTiles.
      // A lower one is copied within VRAM. The count is in tiles, eight words
      // each (16 when wide).
      if (gridState->wideTiles == 0) {
        if (sourceTileOrOpcode >= TILESET_ANIM_RAM_TILE)
          Dma_Set(Ram_DecodeBuffer + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  DMA_COPY_WORDS(tileCount * 8), REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(1) + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  DMA_COPY_WORDS(tileCount * 8), REG_DMA3);
      } else {
        if (sourceTileOrOpcode >= TILESET_ANIM_RAM_WIDE_TILE)
          Dma_Set(Ram_CellPictures + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  DMA_COPY_WORDS(tileCount * 16), REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(2) + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  DMA_COPY_WORDS(tileCount * 16), REG_DMA3);
      }
      // A copy with a wait of 0 lets the next command run at once.
      (gridState->animations + channelIndex)->cursor += 4;
      goto next;
    } else {
      (gridState->animations + channelIndex)->timer--;
    }
  }
}

/// Resumes tileset animation channelIndex by clearing its paused byte.
/// Events and the Battle Arena (menu/battle_arena.c, tileset animation 5)
/// reach it through TilesetAnim_ResumeFar.
void TilesetAnim_Resume(u32 channelIndex) {
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  struct TilesetAnim *channel = &gridState->animations[channelIndex];

  channel->paused = 0;
}

/// Pauses tileset animation channelIndex by setting its paused byte.
/// Events and the Battle Arena (menu/battle_arena.c, tileset animation 5)
/// reach it through TilesetAnim_PauseFar.
void TilesetAnim_Pause(u32 channelIndex) {
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  struct TilesetAnim *channel = &gridState->animations[channelIndex];

  channel->paused = 1;
}

/// Starts the tileset animations a map's animation script lists: each
/// TILESET_ANIM_MARKER starts the tileset animation its low four bits name
/// there (paused with TILESET_ANIM_MARKER_PAUSED). Clears all 16 first, and
/// adds TilesetAnim_Update as a task when any starts. Called by MapGrid_LoadMap
/// with the map's own script, and by WorldMap_InitializePerspective with the
/// world map's default one.
void TilesetAnim_StartAll(const u16 *script) {
  struct MapGridScrollAnimView *gridState;
  s32 channelsStarted;
  u32 marker;

  gridState = gMapGridWork[0];
  channelsStarted = 0;
  Dma_Fill32(0, gridState->animations, sizeof(gridState->animations));
  // Each tileset animation's script starts just after its marker and runs on
  // through the rest of the stream; the scan steps over the halfwords between
  // markers. Only the low four bits pick the tileset animation, so a marker
  // for 16 or more reuses one of the 16.
  marker = *script++;
  // The stream ends at the TILESET_ANIM_LOOP every script after a marker runs
  // on to.
  while (marker != TILESET_ANIM_LOOP) {
    if ((marker & TILESET_ANIM_OPCODE_MASK) == TILESET_ANIM_MARKER) {
      struct TilesetAnim *channel;
      u32 channelIndex = marker & 15;
      u32 paused = 0;
      if (marker & TILESET_ANIM_MARKER_PAUSED)
        paused = 1;
      channel = &gridState->animations[channelIndex];
      channel->start = script;
      channel->cursor = script;
      channel->timer = 0;
      channel->paused = paused;
      channelsStarted++;
    }
    marker = *script++;
  }
  // A map with no tileset animations never adds the update.
  if (channelsStarted != 0)
    Task_Add(TilesetAnim_Update, TASK_PASS_FRAME);
}

/// Suspends the tileset animations, unless the world map's still picture is
/// shown. Menu_SuspendFieldTasks (menu/menu/shared.c) calls it through
/// TilesetAnim_SuspendAllFar.
void TilesetAnim_SuspendAll(void) {
  if (((struct MapGridScrollWork *)gMapGridWork[0])->animationActive == 0)
    Task_SuspendAll(TilesetAnim_Update);
}

/// Resumes the tileset animations, unless the world map's still picture is
/// shown. Menu_ResumeFieldTasks (menu/menu/shared.c) calls it through
/// TilesetAnim_ResumeAllFar.
void TilesetAnim_ResumeAll(void) {
  if (((struct MapGridScrollWork *)gMapGridWork[0])->animationActive == 0)
    Task_ResumeAll(TilesetAnim_Update);
}

/// Each frame: runs the map's blend script. A BLEND_SCRIPT_BLDCNT command sets
/// BLDCNT; any other sets BLDALPHA (or BLDY when not alpha blending) and waits
/// the halfword after it in frames; BLEND_SCRIPT_JUMP jumps, with
/// BLEND_SCRIPT_STOP it stops, and BLEND_SCRIPT_LOOP loops. BlendScript_Start
/// adds it as a task (priority TASK_PASS_FRAME). Writes REG_BLDCNT,
/// REG_BLDALPHA and REG_BLDY, and keeps the last BLDCNT in the map work's
/// blendControl.
void BlendScript_Run(void) {
  // The retries stay gotos: as a for (;;) loop with continue, loop.c hoists
  // blendControl's address and 118 bytes of tbs-en's code change.
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  struct BlendScriptState *blend;
  u16 *commands;
  u32 instruction;
  u32 delayOrJumpIndex;
  u16 delay;

  blend = &gridState->blend;
  if (blend->script == 0)
    return;
  if (blend->paused != 0)
    return;

  // As with the tileset animations, commands run in one frame until a level
  // sets a wait; a BLDCNT command and the jumps take no time.
again:
  delayOrJumpIndex = blend->delay;
  delay = delayOrJumpIndex;
  if (delay == 0) {
    commands = blend->cursor;
    instruction = *commands;
    commands++;
    if (instruction == BLEND_SCRIPT_LOOP) {
      blend->cursor = blend->script;
      goto again;
    }

    if ((instruction & BLEND_SCRIPT_OPCODE_MASK) == BLEND_SCRIPT_JUMP) {
      delayOrJumpIndex = instruction & 0xff;
      if (delayOrJumpIndex == BLEND_SCRIPT_STOP)
        return;
      blend->cursor = blend->script + delayOrJumpIndex * 2;
      goto again;
    }

    // A BLDCNT value is told apart by its top four bits being 3, its second
    // target's sprite and backdrop bits. A level cannot reach them: the
    // BLDALPHA coefficients and BLDY go to 16 at most.
    if ((instruction & BLEND_SCRIPT_BLDCNT_MASK) == BLEND_SCRIPT_BLDCNT) {
      REG_BLDCNT = instruction;
      gridState->blendControl = instruction;
      blend->cursor++;
      goto again;
    }

    // BLDCNT's effect bits (7..6) of 1 are alpha blending; 2 and 3 fade
    // to white or black by BLDY.
    if ((gridState->blendControl & 0xc0) == BLDCNT_EFFECT_BLEND)
      REG_BLDALPHA = instruction;
    else
      REG_BLDY = instruction;
    blend->delay = commands[0];
    blend->cursor += 2;
    goto again;
  }
  // Adding 0xffff to the halfword counts the wait down by one.
  blend->delay = delayOrJumpIndex + 0xffff;
}

/// Starts a map's blend script, unless it is empty. Clears the map work's
/// three-word blend state first, then adds BlendScript_Run as a task.
/// Called by MapGrid_LoadMap when the map's header has a blend script.
void BlendScript_Start(u16 *script) {
  struct BlendScriptState *blend;
  s32 hasScript;
  volatile u32 zero;

  // A fixed-source DMA of the zero word clears script, cursor, delay and
  // paused; a script that is only its end marker leaves the blend unset.
  hasScript = 0;
  zero = 0;
  blend = &((struct MapGridScrollAnimView *)gMapGridWork[0])->blend;
  Dma_Set((const void *)&zero, blend,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 3, REG_DMA3);
  if (*script != BLEND_SCRIPT_LOOP) {
    blend->script = script;
    blend->cursor = script;
    blend->delay = 0;
    blend->paused = 0;
    hasScript = 1;
  }
  if (hasScript)
    Task_Add(BlendScript_Run, TASK_PASS_FRAME);
}

/// Suspends the blend script. The lighthouse scenes and Vale's Babi event
/// reach it through BlendScript_SuspendFar.
void BlendScript_Suspend(void) { Task_SuspendAll(BlendScript_Run); }

/// Resumes the blend script, through BlendScript_ResumeFar.
void BlendScript_Resume(void) { Task_ResumeAll(BlendScript_Run); }

/// Unused: allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with
/// its four entries cleared. Nothing calls it.
UNUSED void PaletteCycle_Initialize(void) {
  struct PaletteCycleWork *paletteCycles;
  struct PaletteCycle *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_CYCLES,
                                      sizeof(struct PaletteCycleWork));
  cycle = paletteCycles->entries;
  // Four cycles of up to 16 saved colours each.
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

/// Unused: adds a palette cycle: colorCount colours of background palette
/// paletteNumber from firstColor, rotated by one every delay + 1 frames.
/// Returns 0, or -1 when all four cycles are taken. The cycle keeps a copy of
/// the colours as they were, which every step rotates from.
UNUSED s32 PaletteCycle_Add(s16 paletteNumber, s16 firstColor, s16 delay,
                            s16 colorCount) {
  struct PaletteCycleWork *paletteQueue;
  struct PaletteCycle *cycle;
  u16 *paletteColors;
  u32 cycleIndex;
  u16 colorCount16;

  paletteQueue = gPaletteCycleWork;
  cycleIndex = paletteQueue->count;
  if (cycleIndex > 3)
    return -1;
  // 16 colours to a palette; the cycle saves the colours as they are
  // now, one halfword each.
  cycle = &paletteQueue->entries[cycleIndex];
  paletteColors =
      (u16 *)BG_PLTT + (((u16)paletteNumber << 4) + (u16)firstColor);
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

/// Unused: stops the palette cycles and frees their queue
/// (HEAP_SLOT_PALETTE_CYCLES). Nothing calls it.
UNUSED void PaletteCycle_Stop(void) {
  Task_Remove(PaletteCycle_Step);
  HeapSlot_Release(HEAP_SLOT_PALETTE_CYCLES);
}

/// Unused: starts running the palette cycles each frame.
UNUSED void PaletteCycle_Start(void) {
  Task_Add(PaletteCycle_Step, TASK_PASS_FRAME);
}

/// Each frame: advances each palette cycle whose timer has run out by one
/// colour, writing the rotated colours back to the palette. Only
/// PaletteCycle_Start adds it as a task, and nothing calls that, so no palette
/// cycle runs in the game.
void PaletteCycle_Step(void) {
  struct PaletteCycleWork *paletteQueue = gPaletteCycleWork;
  u8 cycleIndex;
  // BUG: four queued cycles become zero here, so none advance.
  u16 activeCycles = paletteQueue->count & 3;
  u16 rotatedColors[16];

  for (cycleIndex = 0; cycleIndex < activeCycles; cycleIndex++) {
    struct PaletteCycle *cycle = &paletteQueue->entries[cycleIndex];
    if (cycle->timer == 0) {
      u16 rotationOffset = cycle->position;
      u16 colorCount = cycle->count;
      void *paletteColors = cycle->source;
      u16 *savedColors = cycle->colors;
      u8 colorIndex;

      // Each saved colour moves rotationOffset places down, the first ones
      // wrapping round to the end.
      for (colorIndex = colorCount - rotationOffset; colorIndex < colorCount;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      for (colorIndex = 0; colorIndex < colorCount - rotationOffset;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      Dma_Set(rotatedColors, paletteColors, DMA_COPY_HALFWORDS(colorCount),
              REG_DMA3);
      // The next step rotates one place further, back to none after a full
      // turn, and waits delay frames.
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

#undef TILESET_ANIM_LOOP
#undef TILESET_ANIM_OPCODE_MASK
#undef TILESET_ANIM_JUMP
#undef TILESET_ANIM_STOP
#undef TILESET_ANIM_MARKER
#undef TILESET_ANIM_MARKER_PAUSED
#undef TILESET_ANIM_RAM_TILE
#undef TILESET_ANIM_RAM_WIDE_TILE
#undef BLEND_SCRIPT_LOOP
#undef BLEND_SCRIPT_OPCODE_MASK
#undef BLEND_SCRIPT_JUMP
#undef BLEND_SCRIPT_STOP
#undef BLEND_SCRIPT_BLDCNT_MASK
#undef BLEND_SCRIPT_BLDCNT
