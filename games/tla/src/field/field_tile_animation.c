/// @file
/// Timed map changes (the map animations, the blend script and the palette
/// queue), then the ground heights and the collision of cells and the world
/// map, as The Broken Seal's tile_animation.c and terrain.c have them.
// Part of field.c.

#include "field_internal.h"
#include "flags.h"
#include "field/world_map.h"

#include "field/map_grid.h"
#include "graphics/graphics.h"
#include "field/map.h"
#include "system/system.h"
#include "system/frame.h"
#include "graphics/palette_snapshots.h"
#include "system/io_write_queue.h"
#include "field/flag_call.h"
#include "field/curve.h"
#include "edition.h"

// A tileset animation script's commands, a halfword each where a source tile
// would stand (TilesetAnim_Update and TilesetAnim_StartAll).
#define TILESET_ANIM_LOOP 0xffff        ///< back to the start of the script
#define TILESET_ANIM_OPCODE_MASK 0xff00 ///< the high byte: the command
#define TILESET_ANIM_JUMP 0xfe00 ///< low byte: the halfword pair to jump to
#define TILESET_ANIM_STOP 0xff   ///< TILESET_ANIM_JUMP's low byte to stop
/// A layout with wide tiles animates 64-byte (256-colour) tiles in char block
/// 2; others animate 16-colour ones in char block 1.
#define TILESET_ANIM_WIDE_TILE_BYTES (TILE_SIZE_4BPP * 2)
#define TILESET_ANIM_MARKER 0xfd00 ///< low four bits: the animation it starts
#define TILESET_ANIM_MARKER_PAUSED 0x80 ///< a marker's bit: start it paused
// The first source tile read from gMapGridAnimTiles rather than VRAM, for
// 32-byte and for 64-byte (wide) tiles: both bases put it at its start.
#define TILESET_ANIM_RAM_TILE 1536
#define TILESET_ANIM_RAM_WIDE_TILE 512

// The blend script's commands (BlendScript_Run): as the tileset animations',
// and a BLDCNT value, told apart by its top four bits.
#define BLEND_SCRIPT_LOOP 0xffff
#define BLEND_SCRIPT_OPCODE_MASK 0xff00
#define BLEND_SCRIPT_JUMP 0xfe00
#define BLEND_SCRIPT_STOP 0xff
#define BLEND_SCRIPT_BLDCNT_MASK 0xf000
#define BLEND_SCRIPT_BLDCNT (BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD)

/// Loads the first of the 2x2 world map tiles around the player that is not
/// shown yet (at most one a frame), without drawing it. When the region under
/// the party picks another graphics set, reloads that set over the next 12
/// frames, one step a frame, unless a screen transition is running.
void WorldMap_PreloadCurrentTileBlock(void) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 x = 0;
  s32 y = 0;
  s32 x0;
  s32 y0;
  s32 layer;
  s32 row;
  s32 col;
  s32 tile;
  struct WorldMapCell *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 value;
  u8 *buffer;

  if (perspective->origin != NULL) {
    s32 *p = perspective->origin;

    x = *p++;
    y = p[1];
  }
  x0 = (x - 0x1000000) >> 25;
  y0 = (y - 0x1400000) >> 25;
  for (layer = 0; layer <= 1; layer++) {
    for (row = 0; row <= 1; row++) {
      for (col = 0; col <= 1; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = perspective->tileIds[tile];
        tile += layer << 10;
        if (WorldMap_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) !=
            0)
          return;
      }
    }
  }
  block = (struct WorldMapCell *)Ram_MapBlocks +
          (((x / px(32)) & 31) + (((y / px(32)) & 31) << 5));
  WorldMap_UpdateRegionTint(block->region, 1);
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
  resources = gWorldMap_GraphicsResources[graphicsSet];
  if (resources != perspective->resources) {
    perspective->resources = resources;
    perspective->reloadSteps = 12;
  }
  value = ScreenTransition_IsScanlineActiveFar();
  if (perspective->reloadSteps == 0)
    return;
  if (value != 0)
    return;
  buffer = Heap_Alloc(0x200);
  switch (perspective->reloadSteps) {
  case 12: {
    s16 backdropColor = *(s16 *)BG_PLTT;

    Decompress_ByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = backdropColor;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
    {
      u8 *anim = MAPGRID_ANIMATION_SCRIPT;
      PaletteFade_LoadPaletteFar(buffer);
      Decompress_TaggedLz(Resource_GetTableEntry(resources[1]), anim);
      TilesetAnim_StartAll((const u16 *)anim);
    }
    Decompress_TaggedLz(Resource_GetTableEntry(resources[2]), gMapGridShapes);
    break;
  }
  case 11:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gBgTileBuffer);
    break;
  case 10:
    Dma_Set(gBgTileBuffer, (void *)BG_CHAR_BLOCK(2),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 9:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                        WORLD_MAP_TILE_SET(1));
    break;
  case 8:
    Dma_Set(WORLD_MAP_TILE_SET(1),
            (void *)(BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 7:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[5]),
                        WORLD_MAP_TILE_SET(2));
    break;
  case 6:
    Dma_Set(WORLD_MAP_TILE_SET(2), (void *)BG_CHAR_BLOCK(3),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 5:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[6]),
                        WORLD_MAP_TILE_SET(3));
    break;
  case 4:
    Dma_Set(WORLD_MAP_TILE_SET(3),
            (void *)(BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 3:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[7]),
                        gMapGridAnimTiles);
    break;
  case 1:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), &gWorldMapWork);
    break;
  }
  perspective->reloadSteps--;
  Heap_Release(buffer);
}

/// VBlank: shows BG1 and copies the decoded picture's tiles into VRAM.
void WorldMap_PresentStillPicture(void) {
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG3CNT);
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
             &REG_BG1CNT);
  Dma_Set(Ram_MapCellBuffer, (void *)BG_SCREEN_BLOCK(12),
          DMA_COPY_WORDS(0xa000 / 4), REG_DMA3);
}

/// Freezes the world map's perspective view as a still picture on BG1: decodes
/// the area's tiles again where a field Psynergy used their buffer, saves BG1's
/// tiles, draws the current affine frame into pixels and hands the window and
/// VBlank to it.
void WorldMap_StartStillPicture(void) {
  u8 *pages = Ram_HeapSlots->worldMapAffine;
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  u32 *resources;
  s32 one;
  s8 *active;

  active = &perspective->animationActive;
  one = 1;
  *active = one;
  Task_SuspendAll(TilesetAnim_Update);
  perspective->viewHeld = one;
  resources = perspective->resources;
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gBgTileBuffer);
  if (perspective->tileBufferUsed != 0)
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                        WORLD_MAP_TILE_SET(1));
  perspective->tileBufferUsed = 0;
  Dma_Set((void *)BG_CHAR_BLOCK(1), gDecodeBuffer, DMA_COPY_WORDS(0x2000 / 4),
          REG_DMA3);
  wait(1);
  TileMap_DrawRowsFromRam(pages + (gFrameCount & one) * 0x1400 + 0xc80,
                          Ram_MapCellBuffer);
  perspective->windowTop = 200;
  perspective->windowBottom = 255;
  gVBlankCallback = WorldMap_PresentStillPicture;
  perspective->stillPictureShown = one;
}

/// VBlank: shows BG1 and copies the area's tiles back into VRAM.
void WorldMap_ShowBg1FromBuffer(void) {
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG3CNT);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
  Dma_Set(gBgTileBuffer, (void *)BG_CHAR_BLOCK(2), DMA_COPY_WORDS(0x8000 / 4),
          REG_DMA3);
}

/// Reloads the world map area's palette and tiles after the still picture, puts
/// the default cells back and returns to the perspective view.
s32 WorldMap_LoadAreaGraphics(void) {
  struct WorldMapView *perspective;
  u32 *resources;
  u8 *buffer;
  s16 value;

  perspective = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  buffer = Ram_MapCellBuffer;
  resources = perspective->resources;
  value = *(s16 *)BG_PLTT;
  Decompress_ByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gBgTileBuffer);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]),
                      WORLD_MAP_TILE_SET(1));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]),
                      WORLD_MAP_TILE_SET(2));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[6]),
                      WORLD_MAP_TILE_SET(3));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gMapGridAnimTiles);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), &gWorldMapWork);
  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      buffer);
  WorldMap_UpdateCurrentTileBlock();
  perspective->animationActive = 0;
  Task_ResumeAll(TilesetAnim_Update);
  perspective->viewHeld = 0;
  perspective->stillPictureShown = 0;
  WorldMap_UpdateRegionTint(perspective->tintRegion, 0);
  wait(1);
}

/// Puts the default world map cells back after the still picture and returns to
/// the perspective view, without reloading the area's palette or tiles.
void WorldMap_LoadDefaultCellsAndUpdateBlock(void) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;

  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 0x9f;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      Ram_MapCellBuffer);
  WorldMap_UpdateCurrentTileBlock();
  Task_ResumeAll(TilesetAnim_Update);
  wait(1);
}

/// Each frame: runs the 16 tileset animations.
void TilesetAnim_Update(void) {
  struct MapWork *gridState = Ram_HeapSlots->mapGrid;
  u32 channelIndex;

  for (channelIndex = 0; channelIndex <= 15; channelIndex++) {
    const u16 *commands;
    u32 sourceTileOrOpcode;
    u32 tileCount;
    u32 destinationTile;

    // A tileset animation with no script, or paused by an event, keeps its
    // place.
    if ((gridState->anim + channelIndex)->start == NULL ||
        (gridState->anim + channelIndex)->paused != 0)
      continue;
    // While the wait lasts the tileset animation only counts down; when it is
    // over, commands run in this same frame until one sets a new wait or stops.
  next:
    if ((gridState->anim + channelIndex)->timer == 0) {
      commands = (gridState->anim + channelIndex)->cursor;
      sourceTileOrOpcode = *commands++;
      if (sourceTileOrOpcode == TILESET_ANIM_LOOP) {
        (gridState->anim + channelIndex)->cursor =
            (gridState->anim + channelIndex)->start;
        goto next;
      }
      // TILESET_ANIM_STOP leaves the cursor on itself, so the tileset animation
      // stays stopped until it is started again; TILESET_ANIM_JUMP jumps to
      // the halfword pair its low byte names.
      if ((sourceTileOrOpcode & TILESET_ANIM_OPCODE_MASK) ==
          TILESET_ANIM_JUMP) {
        if ((sourceTileOrOpcode & 0xff) == TILESET_ANIM_STOP)
          continue;
        (gridState->anim + channelIndex)->cursor =
            (gridState->anim + channelIndex)->start +
            (sourceTileOrOpcode & 0xff) * 2;
        goto next;
      }
      tileCount = *commands++;
      destinationTile = commands[0];
      (gridState->anim + channelIndex)->timer = commands[1];
      // A source tile from TILESET_ANIM_RAM_TILE (TILESET_ANIM_RAM_WIDE_TILE
      // for 64-byte tiles) is read from RAM, at the start of gMapGridAnimTiles.
      if (gridState->wideTiles == 0) {
        if (sourceTileOrOpcode >= TILESET_ANIM_RAM_TILE)
          Dma_Set(Ram_DecodeBuffer + sourceTileOrOpcode * TILE_SIZE_4BPP,
                  BG_CHAR_BLOCK(1) + destinationTile * TILE_SIZE_4BPP,
                  DMA_COPY_WORDS(tileCount * (TILE_SIZE_4BPP / 4)), REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(1) + sourceTileOrOpcode * TILE_SIZE_4BPP,
                  BG_CHAR_BLOCK(1) + destinationTile * TILE_SIZE_4BPP,
                  DMA_COPY_WORDS(tileCount * (TILE_SIZE_4BPP / 4)), REG_DMA3);
      } else {
        if (sourceTileOrOpcode >= TILESET_ANIM_RAM_WIDE_TILE)
          Dma_Set(
              Ram_MapBlocks + sourceTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
              BG_CHAR_BLOCK(2) + destinationTile * TILESET_ANIM_WIDE_TILE_BYTES,
              DMA_COPY_WORDS(tileCount * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
              REG_DMA3);
        else
          Dma_Set(
              BG_CHAR_BLOCK(2) +
                  sourceTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
              BG_CHAR_BLOCK(2) + destinationTile * TILESET_ANIM_WIDE_TILE_BYTES,
              DMA_COPY_WORDS(tileCount * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
              REG_DMA3);
      }
      // A copy with a wait of 0 lets the next command run at once.
      (gridState->anim + channelIndex)->cursor += 4;
      goto next;
    } else {
      (gridState->anim + channelIndex)->timer--;
    }
  }
}

/// Resumes tileset animation no by clearing its paused byte.
void TilesetAnim_Resume(u32 channelIndex) {
  struct MapWork *gridState = Ram_HeapSlots->mapGrid;
  struct MapAnimation *channel = &gridState->anim[channelIndex];

  channel->paused = 0;
}

/// Pauses tileset animation no by setting its paused byte.
void TilesetAnim_Pause(u32 channelIndex) {
  struct MapWork *gridState = Ram_HeapSlots->mapGrid;
  struct MapAnimation *channel = &gridState->anim[channelIndex];

  channel->paused = 1;
}

/// Starts the tileset animations a map's animation script lists: each 0xfdNN
/// marker starts the tileset animation its low four bits name there, paused
/// when bit 7 is set.
void TilesetAnim_StartAll(const u16 *script) {
  struct MapWork *gridState = Ram_HeapSlots->mapGrid;
  s32 channelsStarted;
  u32 marker;

  channelsStarted = 0;
  Iwram_ClearWordsEntry(gridState->anim, sizeof(gridState->anim));
  // Each tileset animation's script starts just after its marker and runs on
  // through the rest of the stream; the scan steps over the halfwords between
  // markers.
  marker = *script++;
  // The stream ends at the TILESET_ANIM_LOOP every script after a marker runs
  // on to.
  while (marker != TILESET_ANIM_LOOP) {
    if ((marker & TILESET_ANIM_OPCODE_MASK) == TILESET_ANIM_MARKER) {
      struct MapAnimation *channel;
      u32 channelIndex = marker & 15;
      u32 paused = 0;

      if (marker & TILESET_ANIM_MARKER_PAUSED)
        paused = 1;
      channel = &gridState->anim[channelIndex];
      channel->start = script;
      channel->cursor = script;
      channel->timer = 0;
      channel->paused = paused;
      channelsStarted++;
    }
    marker = *script++;
  }
  // A map with no tileset animations never adds the update, nor adds it twice.
  if (channelsStarted != 0 && Task_Find(TilesetAnim_Update) == -1)
    Task_Add(TilesetAnim_Update, TASK_PASS_VBLANK);
}

/// Suspends the tileset animations, unless the world map's still picture is
/// shown.
void TilesetAnim_SuspendAll(void) {
  if (((struct MapWork *)Ram_HeapSlots->mapGrid)->animationActive == 0)
    Task_SuspendAll(TilesetAnim_Update);
}

/// Resumes the tileset animations, unless the world map's still picture is
/// shown.
void TilesetAnim_ResumeAll(void) {
  if (((struct MapWork *)Ram_HeapSlots->mapGrid)->animationActive == 0)
    Task_ResumeAll(TilesetAnim_Update);
}

/// Each frame: runs the map's blend script.
void BlendScript_Run(void) {
  struct MapWork *gridState = Ram_HeapSlots->mapGrid;
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
    // target's sprite and backdrop bits.
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
void BlendScript_Start(u16 *script) {
  struct BlendScriptState *state;
  s32 started;
  volatile u32 zero;
  u32 first;

  started = 0;
  zero = 0;
  state = &Ram_HeapSlots->mapGrid->blend;
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

/// Suspends the blend script; scenes reach it through BlendScript_SuspendFar.
void BlendScript_Suspend(void) { Task_SuspendAll(BlendScript_Run); }

/// Resumes the blend script, through BlendScript_ResumeFar.
void BlendScript_Resume(void) { Task_ResumeAll(BlendScript_Run); }

/// Unused: allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with
/// its four entries cleared.
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

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->paletteCycles)

#include "palqueue_add.c"

/// Stops the palette cycles: removes their task and frees their work.
void PaletteQueue_Stop(void) {
  Task_Remove(PaletteCycle_Step);
  HeapSlot_Release(HEAP_SLOT_PALETTE_CYCLES);
}

void PaletteCycle_Start(void) { Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK); }

void PaletteCycle_Step(void) {
  struct PaletteCycleWork *paletteQueue = Ram_HeapSlots->paletteCycles;
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

/// Tints the world map for the encounter region under the party: region 9
/// brightens it by 12 steps, region 18 darkens it by 6 until flag 0x8ef is
/// set, any other clears the tint. Gradually, the tint moves one step every 16
/// frames; at once, it jumps there. Flag 0x106 holds the tint as it is.
void WorldMap_UpdateRegionTint(s32 region, s32 gradual) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;

  if (perspective->tintRegion != region) {
    perspective->tintRegion = region;
    if (region == 9)
      perspective->tintTarget = 12;
    else if (region == 18 && !GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED))
      perspective->tintTarget = -6;
    else
      perspective->tintTarget = 0;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN))
    return;
  if (gradual == 0) {
    perspective->tint = perspective->tintTarget;
  } else {
    region = perspective->tintTarget - perspective->tint;
    if (region == 0)
      return;
    if (gFrameCount & 15)
      return;
    if (region < 0)
      region = -1;
    else
      region = 1;
    perspective->tint += region;
  }
  if (perspective->tint > 0) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                            BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, (u16)perspective->tint,
                        IO_WRITE_HALFWORD);
  } else {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                            BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, (u16)-perspective->tint,
                        IO_WRITE_HALFWORD);
  }
}

/// The ground height functions (gMapGrid_TerrainHeightFunctions): each takes a
/// cell's height samples after its kind byte and a pixel (x, y) within the
/// 16x16 cell, and returns the ground height there in 16.16 pixels.
s32 Terrain_GetFlatHeight(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

/// The height of a slope across x at position (0 to 15 of the cell): a straight
/// line from samples[0] to samples[1].
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

/// A slope along y: from sample 0 at the top to sample 1 at the bottom.
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

/// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
/// it and the higher of the two on it.
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

/// A step across the diagonal x = y: sample 0 where y < x, sample 1 where
/// y > x and the higher of the two on it.
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

/// A slope that bends on the diagonal x + y = 15: from sample 0 at the top
/// left corner to sample 1 on the diagonal to sample 2 at the bottom right.
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

/// A slope that bends on the diagonal x = y: from sample 0 at the top right
/// corner to sample 1 on the diagonal to sample 2 at the bottom left.
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

/// A raised or sunken rounded square: sample 1 where gTerrain_LerpWeightTable
/// is 1, sample 0 in the corners.
s32 Terrain_GetRoundedSquareHeight(const s8 *samples, s32 position, s32 row) {
  s32 start;

  start = samples[0] << CURVE_VALUE_SHIFT;
  // The weight is 0 or 1, so the result is one sample or the other, never
  // between.
  return start +
         (((samples[1] << CURVE_VALUE_SHIFT) - start) *
          gTerrain_LerpWeightTable[position + (row * CURVE_FULL_STEPS)]);
}

/// The height of a step across x at position (0 to 15 of the cell):
/// samples[0] on the cell's first half, samples[1] on its second.
s32 Terrain_GetStepXHeight(const s8 *samples, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

/// A step along y: sample 0 above the middle, sample 1 below it.
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
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  u8 *grid;
  u8 *code;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  grid += x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7);
  code = Ram_MapCollision + *grid * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15);
}

/// Returns the terrain kind of a layer's cell at (x, z) in 16.16 pixels, as
/// The Broken Seal's.
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / MAPGRID_CELL_PIXELS +
                               (z / MAPGRID_CELL_PIXELS << 7)] *
                          4] &
         15;
}

/// As MapGrid_GetCellAttributeLowNibble, but a rounded square whose two
/// samples differ (a real bump or hollow, not a flat cell) reads as 99.
s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  u8 *grid;
  u8 *code;
  s32 kind;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  code =
      &Ram_MapCollision
          [grid[x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7)] * 4];
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

/// Gives the cell at (x, z) of layer the ground height `height`, in 8-pixel
/// steps, by tagging its shape with terrain kind 0 at that height.
void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height) {
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  u8 *shape;

  shape = gMapShapeGrid +
          (work->layers[layer & 3].cells - (struct MapCell *)Ram_MapCellBuffer);
  shape += (FIELD_CELL(x)) + ((FIELD_CELL(z)) << 7);
  *shape = MapCollision_FindOrAddTagFar(layer, 0, height);
}

/// The ground height at (x, z) of layer, in 8-pixel steps.
s32 MapGrid_GetCellHeight(s32 x, s32 z, s32 layer) {
  return MapGrid_GetTerrainHeight(layer, x, z) >> 19;
}

/// As MapGrid_GetCellAttributeLowNibble, a second copy.
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapGrid;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / MAPGRID_CELL_PIXELS +
                               (z / MAPGRID_CELL_PIXELS << 7)] *
                          4] &
         15;
}

/// Returns the cell behavior of a layer's cell at (x, y) in 16.16 pixels. The
/// shift by 20 takes 16.16 pixels straight to 16-pixel cells.
u8 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 y) {
  struct MapWork *state;
  struct MapCell *cells;

  state = Ram_HeapSlots->mapGrid;
  x >>= 20;
  y >>= 20;
  cells = (struct MapCell *)gMapGridCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + (y << 7);
  return cells->collisionCode;
}

/// Sets the cell behavior of a layer's cell at (x, y) in 16.16 pixels. Writes
/// the cell in the map's grid; does nothing before the map is set up.
void MapGrid_SetCellBehaviorAt(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapWork *state = Ram_HeapSlots->mapGrid;

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
  s32 x = FIELD_PIXEL(px);
  s32 z = FIELD_PIXEL(pz);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *shapes;
  u8 *code;

  if (map == NULL)
    return 0;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return 0;
  if (layer <= 2)
    shapes = map->layers[layer].terrainIds;
  else
    shapes = gMapShapeGrid;
  code = Ram_MapCollision +
         shapes[x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7)] * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         base;
}

/// How the ground at (px, pz) (16.16 pixels) on the given layer stands to
/// height: 2 when the cell has no tile, 1 when the ground rises more than 8
/// pixels above it, -1 when it falls more than 12 below, else 0. A layer past
/// 2 reads the cell buffer and shape grid. With no map, or while the event
/// mode is 3, every point is level (0).
s32 MapGrid_CheckCollisionAt(s32 px, s32 pz, s32 layer, s32 height) {
  s32 x = FIELD_PIXEL(px);
  s32 z = FIELD_PIXEL(pz);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return 0;
  if (layer <= 2) {
    cells = (u8 *)map->layers[layer].cells;
    shapes = map->layers[layer].terrainIds;
  } else {
    cells = gMapGridCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == CELL_BEHAVIOR_WALL)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         height;
  if (rise > px(8))
    return 1;
  if (rise < -px(12))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollisionAt for the point position (16.16 pixels, read as
/// its whole-pixel halves) on the actor's layer, against its ground height.
s32 MapGrid_CheckCollision(struct Actor *actor,
                           struct FieldPosition *position) {
  s32 x = ((struct FieldCoordinate *)&position->x)->part[0].pixel;
  s32 z = ((struct FieldCoordinate *)&position->z)->part[0].pixel;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return 0;
  if (actor->layer <= 2) {
    cells = (u8 *)map->layers[actor->layer].cells;
    shapes = map->layers[actor->layer].terrainIds;
  } else {
    cells = gMapGridCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == CELL_BEHAVIOR_WALL)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         actor->groundHeight;
  if (rise > px(8))
    return 1;
  if (rise < -px(12))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollision, against the actor's height and with no -1: 2
/// when the cell has no tile, 1 when the ground rises more than 8 pixels above
/// it, else 0.
s32 MapGrid_CheckCollisionAbove(struct Actor *actor,
                                struct FieldPosition *position) {
  s32 x = ((struct FieldCoordinate *)&position->x)->part[0].pixel;
  s32 z = ((struct FieldCoordinate *)&position->z)->part[0].pixel;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;

  if (map == NULL)
    return 0;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return 0;
  if (actor->layer <= 2) {
    cells = (u8 *)map->layers[actor->layer].cells;
    shapes = map->layers[actor->layer].terrainIds;
  } else {
    cells = gMapGridCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == CELL_BEHAVIOR_WALL)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  if (gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
          actor->y.fixed >
      px(8))
    return 1;
  return 0;
}

/// Returns -1 when the cell under position (its x, and z less its height) in
/// the overhead layer is a wall (cell behavior 255), else 0; also 0 before the
/// map work exists.
s32 MapGrid_IsWallAt(s32 *position) {
  s32 x = position[0] / Q16_ONE;
  s32 z = (u32)((position[2] & FIELD_CELL_MASK) - position[1]) >> 16;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;

  if (map == NULL)
    return 0;
  cell = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
  cell += x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  return (cell->collisionCode != CELL_BEHAVIOR_WALL) - 1;
}

/* VIEW(f02c58c): variable gCellPictures as struct WorldMapBlock
 * gWorldMapBlocks[] under a name of its own; system/ram_buffer.h has u32
 * gCellPictures[]; read through it with a cast, the field object shares the
 * address with the other reads and the block read moves (4 lines in tla-en). */
extern struct WorldMapBlock gWorldMapBlocks[] __asm__("gCellPictures");

/// The place of the world map block under position (blocks 32 pixels square on
/// a 32 by 32 grid), WORLD_MAP_NO_PLACE where the map has none.
// XXX: what the byte numbers besides 255; the world map scene's events of
// kind 2 against the blocks in gCellPictures would settle it.
s32 WorldMap_GetBlockPlace(struct WorldPosition *position) {
  s32 z = position->y;
  s32 x = position->x;

  if (x < 0)
    x += 0x1fffff;
  x = x >> 21 & 31;
  if (z < 0)
    z += 0x1fffff;
  z = z >> 21 & 31;
  return gWorldMapBlocks[x + z * 32].place;
}

/// Returns the world map's collision value (1-15) at position: from the upper
/// collision layer where it has one, else the lower, else
/// WORLD_MAP_COLLISION_DEFAULT.
s32 WorldMap_GetCollision(struct WorldPosition *position) {
  s32 halfPixelX = position->x >> 17;
  s32 y = position->y;
  s32 halfPixelY = y >> 17;
  u32 collisionCell;
  u32 pixelOffset;
  u8 collisionTile;
  u8 packedCollisions;
  u32 result;

  // Coordinates in 2-pixel units.
  collisionCell = (((halfPixelY / 8) & 63) << 6) + ((halfPixelX / 8) & 63);
  pixelOffset = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

  // The upper layer: its tile map in screen block 10, its tiles 0x800 bytes
  // into the collision buffer.
  collisionTile = BG_SCREEN_BLOCK(10)[collisionCell];
  packedCollisions =
      *(Ram_MapCollision + 0x800 + (collisionTile << 3) + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 15;
    if (result != 0)
      return result;
  }

  // The lower layer: its tile map in screen block 8, its tiles at the start.
  collisionTile = BG_SCREEN_BLOCK(8)[collisionCell];
  packedCollisions = *(Ram_MapCollision + (collisionTile << 3) + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 15;
    if (result != 0)
      return result;
  }

  return WORLD_MAP_COLLISION_DEFAULT;
}

/// Returns 0 when the world map at position can be walked on (a place there,
/// and collision WORLD_MAP_COLLISION_WALKABLE_FIRST to
/// WORLD_MAP_COLLISION_WALKABLE_LAST), else -1.
s32 WorldMap_IsWalkable(void *object, struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != WORLD_MAP_NO_PLACE) {
    if ((u32)(WorldMap_GetCollision(position) -
              WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
        WORLD_MAP_COLLISION_WALKABLE_LAST - WORLD_MAP_COLLISION_WALKABLE_FIRST)
      return 0;
  }
  return -1;
}

/// Returns 0 where the ship can sail (the sea or shallow water), else -1.
s32 WorldMap_IsSailable(void *object, struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != WORLD_MAP_NO_PLACE) {
    s32 collision = WorldMap_GetCollision(position);
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    s32 mode;

    mode = work->terrainKind;
    if (mode == 0) {
      if ((u32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
              WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_SEA_FIRST ||
          collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
        return 0;
    } else if (mode == 1) {
      if ((u32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
              WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_SEA_FIRST ||
          collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
        return 0;
    } else if (mode == 2) {
      if ((u32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
              WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_SEA_FIRST ||
          collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
        return 0;
    }
  }
  return -1;
}

/// Returns 0 where the winged ship can go, else -1: low down, where it can
/// sail; higher, more of the ground by the kind it last flew over.
s32 WorldMap_IsFlyable(struct WorldMover *mover,
                       struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != WORLD_MAP_NO_PLACE) {
    s32 collision = WorldMap_GetCollision(position);
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    s32 height = mover->height;

    if (height < px(4)) {
      if ((u32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) >
              WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_SEA_FIRST &&
          collision != WORLD_MAP_COLLISION_SHALLOW_WATER)
        return -1;
      return 0;
    } else {
      s32 mode = work->terrainKind;

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
