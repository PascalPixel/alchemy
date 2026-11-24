/// @file
/// Timed map changes (the map animations, the blend script and the palette
/// queue), then the ground heights and the collision of cells and the world
/// map, as The Broken Seal's field_tile_animation.c and field_terrain.c have
/// them.
// Part of field.c.

#include "field_internal.h"
#include "flags.h"
#include "field/world_map.h"

#include "field/map_grid.h"
#include "graphics/graphics.h"
#include "field/map.h"
#include "system/system.h"
#include "system/frame.h"
#include "graphics/palette_cycles.h"
#include "graphics/palette_buffers.h"
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
#define TILESET_ANIM_ARGUMENT_MASK                                             \
  0xff ///< the low byte: the command's argument
/// A layout with wide tiles animates 64-byte (256-colour) tiles in char block
/// 2; others animate 16-colour ones in char block 1.
#define TILESET_ANIM_WIDE_TILE_BYTES (TILE_SIZE_4BPP * 2)
#define TILESET_ANIM_MARKER 0xfd00 ///< low four bits: the animation it starts
#define TILESET_ANIM_MARKER_PAUSED 0x80 ///< a marker's bit: start it paused
#define TILESET_ANIM_MARKER_ANIMATION_MASK 0xf ///< a marker's animation
/// The first source tile read from gMapGridAnimTiles rather than VRAM, for
/// 32-byte and for 64-byte (wide) tiles: both bases put it at its start.
#define TILESET_ANIM_RAM_TILE 1536
#define TILESET_ANIM_RAM_WIDE_TILE 512

/// The blend script's other commands (BlendScript_Run; BLEND_SCRIPT_LOOP and
/// BLEND_SCRIPT_JUMP are map_grid.h's): as the tileset animations', and a
/// BLDCNT value, told apart by its top four bits.
#define BLEND_SCRIPT_OPCODE_MASK 0xff00
#define BLEND_SCRIPT_STOP 0xff
#define BLEND_SCRIPT_ARGUMENT_MASK 0xff
#define BLEND_SCRIPT_BLDCNT_MASK 0xf000
#define BLEND_SCRIPT_BLDCNT (BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD)

/// A world map collision tile: 4 by 4 values of 4 bits, each for 4 by 4 pixels,
/// in 8 bytes. The collision buffer holds the 256 tiles of the lower layer's,
/// then the upper layer's.
#define WORLD_MAP_COLLISION_TILE_BYTES 8
#define WORLD_MAP_COLLISION_TILES 256

/// The frames a world map graphics set takes to reload, one step a frame,
/// counted down from this: its palette, animations and shapes, then each tile
/// set decoded and copied to VRAM.
#define WORLD_MAP_RELOAD_STEPS 12

/// Loads the first of the 2x2 world map tiles around the player that is not
/// shown yet (at most one a frame), without drawing it. When the region under
/// the party picks another graphics set, reloads that set over the next
/// WORLD_MAP_RELOAD_STEPS frames, unless a screen transition is running.
void WorldMap_PreloadCurrentTileBlock(void) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 x = 0;
  s32 y = 0;
  s32 firstQuarterX;
  s32 firstQuarterY;
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
  firstQuarterX = WORLD_MAP_TILE(x - toFIXED(256));
  firstQuarterY = WORLD_MAP_TILE(y - toFIXED(320));
  for (layer = 0; layer <= 1; layer++) {
    for (row = 0; row <= 1; row++) {
      for (col = 0; col <= 1; col++) {
        tile = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1)) << 5) +
               ((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
        tile = perspective->tileIds[tile];
        tile += layer * WORLD_MAP_LAYER_TILES;
        if (WorldMap_WriteLayerCellTile(layer, firstQuarterX + col,
                                        firstQuarterY + row, tile, 0) != 0)
          return;
      }
    }
  }
  block = (struct WorldMapCell *)Ram_MapBlocks +
          (((x / toFIXED(32)) & 31) + (((y / toFIXED(32)) & 31) << 5));
  WorldMap_UpdateRegionTint(block->region, 1);
  value = block->region;
  switch (value / WORLD_MAP_REGION_GROUP) {
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
    perspective->reloadSteps = WORLD_MAP_RELOAD_STEPS;
  }
  value = ScreenTransition_IsScanlineActiveFar();
  if (perspective->reloadSteps == 0)
    return;
  if (value != 0)
    return;
  buffer = Heap_Alloc(BG_PLTT_SIZE);
  switch (perspective->reloadSteps) {
  case WORLD_MAP_RELOAD_STEPS: {
    s16 backdropColor = *(s16 *)BG_PLTT;

    Decompress_ByteLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_PALETTE]),
                      buffer);
    *(s16 *)buffer = backdropColor;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2),
            REG_DMA3);
    {
      u8 *anim = MAPGRID_ANIMATION_SCRIPT;
      PaletteFade_LoadPaletteFar(buffer);
      Decompress_TaggedLz(
          Resource_GetTableEntry(resources[WORLD_MAP_SET_ANIMATION]), anim);
      TilesetAnim_StartAll((const u16 *)anim);
    }
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_CELL_ATTRIBUTES]),
        gMapGridShapes);
    break;
  }
  case 11:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
        gBgTileBuffer);
    break;
  case 10:
    Dma_Set(gBgTileBuffer, (void *)BG_CHAR_BLOCK(2),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 9:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
        WORLD_MAP_TILE_SET(1));
    break;
  case 8:
    Dma_Set(WORLD_MAP_TILE_SET(1),
            (void *)(BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 7:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
        WORLD_MAP_TILE_SET(2));
    break;
  case 6:
    Dma_Set(WORLD_MAP_TILE_SET(2), (void *)BG_CHAR_BLOCK(3),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 5:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
        WORLD_MAP_TILE_SET(3));
    break;
  case 4:
    Dma_Set(WORLD_MAP_TILE_SET(3),
            (void *)(BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES),
            DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
    break;
  case 3:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
        gMapGridAnimTiles);
    break;
  case 1:
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
        &gWorldMapState);
    break;
  }
  perspective->reloadSteps--;
  Heap_Release(buffer);
}

/// VBlank: shows BG1 and copies the decoded picture's tiles into VRAM.
void WorldMap_PresentStillImage(void) {
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG3CNT);
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
                 BGCNT_WRAP | BGCNT_SIZE(2),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
             &REG_BG1CNT);
  Dma_Set(Ram_MapCellBuffer,
          BG_CHAR_BLOCK(0) + STILL_IMAGE_FIRST_TILE * TILE_SIZE_8BPP,
          DMA_COPY_WORDS(STILL_IMAGE_TILES_ACROSS * STILL_IMAGE_TILE_ROWS *
                         TILE_SIZE_8BPP / 4),
          REG_DMA3);
}

/// Freezes the world map's perspective view as a still image on BG1: decodes
/// the area's tiles again where a field Psynergy used their buffer, saves BG1's
/// tiles, draws the current affine frame into pixels and hands the window and
/// VBlank to it.
void WorldMap_StartStillImage(void) {
  struct WorldMapAffineState *affineState = Ram_HeapSlots->worldMapAffine;
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  const u32 *resources;

  perspective->animationActive = 1;
  Task_Suspend(TilesetAnim_Update);
  perspective->viewHeld = 1;
  resources = perspective->resources;
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
                      gBgTileBuffer);
  if (perspective->tileBufferUsed != 0)
    Decompress_TaggedLz(
        Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
        WORLD_MAP_TILE_SET(1));
  perspective->tileBufferUsed = 0;
  Dma_Set((void *)BG_CHAR_BLOCK(1), gDecodeBuffer,
          DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
  wait(1);
  TileMap_DrawRowsFromRam(affineState->pages[gFrameCount & 1],
                          Ram_MapCellBuffer);
  perspective->windowTop = STILL_IMAGE_WINDOW_TOP;
  perspective->windowBottom = STILL_IMAGE_WINDOW_BOTTOM;
  gVBlankCallback = WorldMap_PresentStillImage;
  perspective->stillImageShown = 1;
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
  Dma_Set(gBgTileBuffer, (void *)BG_CHAR_BLOCK(2),
          DMA_COPY_WORDS(4 * WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
}

/// Reloads the world map area's palette and tiles after the still image, puts
/// the default cells back and returns to the perspective view.
s32 WorldMap_LoadAreaGraphics(void) {
  struct WorldMapView *perspective;
  const u32 *resources;
  u8 *buffer;
  s16 backdropColor;

  perspective = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  buffer = Ram_MapCellBuffer;
  resources = perspective->resources;
  backdropColor = *(s16 *)BG_PLTT;
  Decompress_ByteLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_PALETTE]),
                    buffer);
  *(s16 *)buffer = backdropColor;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2),
          REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
                      gBgTileBuffer);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
                      WORLD_MAP_TILE_SET(1));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
                      WORLD_MAP_TILE_SET(2));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
                      WORLD_MAP_TILE_SET(3));
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
                      gMapGridAnimTiles);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
                      &gWorldMapState);
  // The VBlank copies the tiles back and the next frame shows the ground;
  // only then is the still image's buffer refilled with the cells.
  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DEFAULT_MAP_CELLS),
                      buffer);
  WorldMap_UpdateCurrentTileBlock();
  perspective->animationActive = 0;
  Task_Resume(TilesetAnim_Update);
  perspective->viewHeld = 0;
  perspective->stillImageShown = 0;
  WorldMap_UpdateRegionTint(perspective->tintRegion, 0);
  wait(1);
}

/// Puts the default world map cells back after the still image and returns to
/// the perspective view, without reloading the area's palette or tiles.
void WorldMap_LoadDefaultCellsAndUpdateBlock(void) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;

  gVBlankCallback = WorldMap_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = DISPLAY_HEIGHT - 1;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DEFAULT_MAP_CELLS),
                      Ram_MapCellBuffer);
  WorldMap_UpdateCurrentTileBlock();
  Task_Resume(TilesetAnim_Update);
  wait(1);
}

/// Each frame: runs the 16 tileset animations.
void TilesetAnim_Update(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u32 channelIndex;

  for (channelIndex = 0; channelIndex < ARRAY_COUNT(map->anim);
       channelIndex++) {
    const u16 *commands;
    u32 sourceTileOrOpcode;
    u32 tileCount;
    u32 destinationTile;

    // A tileset animation with no script, or paused by an event, keeps its
    // place.
    if ((map->anim + channelIndex)->start == NULL ||
        (map->anim + channelIndex)->paused != 0)
      continue;
    // While the wait lasts the tileset animation only counts down; when it is
    // over, commands run in this same frame until one sets a new wait or stops.
  next:
    if ((map->anim + channelIndex)->timer == 0) {
      commands = (map->anim + channelIndex)->cursor;
      sourceTileOrOpcode = *commands++;
      if (sourceTileOrOpcode == TILESET_ANIM_LOOP) {
        (map->anim + channelIndex)->cursor = (map->anim + channelIndex)->start;
        goto next;
      }
      // TILESET_ANIM_STOP leaves the cursor on itself, so the tileset animation
      // stays stopped until it is started again; TILESET_ANIM_JUMP jumps to
      // the halfword pair its low byte names.
      if ((sourceTileOrOpcode & TILESET_ANIM_OPCODE_MASK) ==
          TILESET_ANIM_JUMP) {
        if ((sourceTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) ==
            TILESET_ANIM_STOP)
          continue;
        (map->anim + channelIndex)->cursor =
            (map->anim + channelIndex)->start +
            (sourceTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) * 2;
        goto next;
      }
      tileCount = *commands++;
      destinationTile = commands[0];
      (map->anim + channelIndex)->timer = commands[1];
      // A source tile from TILESET_ANIM_RAM_TILE (TILESET_ANIM_RAM_WIDE_TILE
      // for 64-byte tiles) is read from RAM, at the start of gMapGridAnimTiles.
      if (map->wideTiles == 0) {
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
      (map->anim + channelIndex)->cursor += 4;
      goto next;
    } else {
      (map->anim + channelIndex)->timer--;
    }
  }
}

/// Resumes tileset animation channelIndex.
void TilesetAnim_Resume(u32 channelIndex) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapAnimation *channel = &map->anim[channelIndex];

  channel->paused = 0;
}

/// Pauses tileset animation channelIndex.
void TilesetAnim_Pause(u32 channelIndex) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapAnimation *channel = &map->anim[channelIndex];

  channel->paused = 1;
}

/// Starts the tileset animations a map's animation script lists: each
/// TILESET_ANIM_MARKER starts the tileset animation its low four bits name
/// there (paused with TILESET_ANIM_MARKER_PAUSED).
void TilesetAnim_StartAll(const u16 *script) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 channelsStarted;
  u32 marker;

  channelsStarted = 0;
  Iwram_ClearWordsEntry(map->anim, sizeof(map->anim));
  // Each tileset animation's script starts just after its marker and runs on
  // through the rest of the stream; the scan steps over the halfwords between
  // markers.
  marker = *script++;
  // The stream ends at the TILESET_ANIM_LOOP every script after a marker runs
  // on to.
  while (marker != TILESET_ANIM_LOOP) {
    if ((marker & TILESET_ANIM_OPCODE_MASK) == TILESET_ANIM_MARKER) {
      struct MapAnimation *channel;
      u32 channelIndex = marker & TILESET_ANIM_MARKER_ANIMATION_MASK;
      u32 paused = 0;

      if (marker & TILESET_ANIM_MARKER_PAUSED)
        paused = 1;
      channel = &map->anim[channelIndex];
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
    Task_Add(TilesetAnim_Update, TASK_PASS_FRAME);
}

/// Suspends the tileset animations, unless the world map's still image is
/// shown.
void TilesetAnim_SuspendAll(void) {
  if (((struct MapState *)Ram_HeapSlots->mapGrid)->animationActive == 0)
    Task_Suspend(TilesetAnim_Update);
}

/// Resumes the tileset animations, unless the world map's still image is
/// shown.
void TilesetAnim_ResumeAll(void) {
  if (((struct MapState *)Ram_HeapSlots->mapGrid)->animationActive == 0)
    Task_Resume(TilesetAnim_Update);
}

/// Each frame: runs the map's blend script.
void BlendScript_Run(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct BlendScriptState *blend;
  u16 *commands;
  u32 instruction;
  u32 delayOrJumpIndex;
  u16 delay;

  blend = &map->blend;
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
      delayOrJumpIndex = instruction & BLEND_SCRIPT_ARGUMENT_MASK;
      if (delayOrJumpIndex == BLEND_SCRIPT_STOP)
        return;
      blend->cursor = blend->script + delayOrJumpIndex * 2;
      goto again;
    }

    // A BLDCNT value is told apart by its top four bits being 3, its second
    // target's sprite and backdrop bits.
    if ((instruction & BLEND_SCRIPT_BLDCNT_MASK) == BLEND_SCRIPT_BLDCNT) {
      REG_BLDCNT = instruction;
      map->blendControl = instruction;
      blend->cursor++;
      goto again;
    }

    // An alpha blend takes the level as BLDALPHA; a fade to white or black
    // takes it as BLDY.
    if ((map->blendControl & BLDCNT_EFFECT_MASK) == BLDCNT_EFFECT_BLEND)
      REG_BLDALPHA = instruction;
    else
      REG_BLDY = instruction;
    blend->delay = commands[0];
    blend->cursor += 2;
    goto again;
  }
  // Counts the wait down by one.
  blend->delay = delayOrJumpIndex - 1;
}

/// Starts a map's blend script, unless it is empty. Clears the map state's
/// three-word blend state first, then adds BlendScript_Run as a task.
void BlendScript_Start(u16 *script) {
  struct BlendScriptState *blend;
  s32 hasScript;
  volatile u32 zero;

  // A fixed-source DMA of the zero word clears script, cursor, delay and
  // paused; a script that is only its end marker leaves the blend unset.
  hasScript = 0;
  zero = 0;
  blend = &((struct MapState *)Ram_HeapSlots->mapGrid)->blend;
  Dma_Set((const void *)&zero, blend, DMA_FILL_WORDS(sizeof(*blend) / 4),
          REG_DMA3);
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

/// Suspends the blend script; scenes reach it through BlendScript_SuspendFar.
void BlendScript_Suspend(void) { Task_Suspend(BlendScript_Run); }

/// Resumes the blend script, through BlendScript_ResumeFar.
void BlendScript_Resume(void) { Task_Resume(BlendScript_Run); }

#include "palette_cycle_initialize.c"

#include "palette_cycle_add.c"

/// Unused: stops the palette cycles and frees their queue
/// (HEAP_SLOT_PALETTE_CYCLES).
UNUSED void PaletteQueue_Stop(void) {
  Task_Remove(PaletteCycle_Step);
  HeapSlot_Release(HEAP_SLOT_PALETTE_CYCLES);
}

/// Unused: starts running the palette cycles each frame.
UNUSED void PaletteCycle_Start(void) {
  Task_Add(PaletteCycle_Step, TASK_PASS_FRAME);
}

/// Each frame: advances each palette cycle whose timer has run out by one
/// colour, writing the rotated colours back to the palette.
void PaletteCycle_Step(void) {
  struct PaletteCycleState *paletteQueue = Ram_HeapSlots->paletteCycles;
  u8 cycleIndex;
  // BUG: four queued cycles become zero here, so none advance.
  u16 activeCycles = paletteQueue->count & 3;
  u16 rotatedColors[PALETTE_COLORS];

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

/// The encounter regions the world map is tinted in, and their tints in BLDY's
/// sixteenths: one brightened, one darkened until the Gabomba ceremony ends.
#define TINT_BRIGHT_REGION 9
#define TINT_BRIGHT 12
#define TINT_DARK_REGION 18
#define TINT_DARK 6

/// Tints the world map for the encounter region under the party, any region but
/// TINT_BRIGHT_REGION and TINT_DARK_REGION clearing the tint. Gradually, the
/// tint moves one step every 16 frames; at once, it jumps there. While a menu
/// is open the tint stays as it is.
void WorldMap_UpdateRegionTint(s32 region, s32 gradual) {
  struct WorldMapView *perspective =
      (struct WorldMapView *)Ram_HeapSlots->mapGrid;

  if (perspective->tintRegion != region) {
    perspective->tintRegion = region;
    if (region == TINT_BRIGHT_REGION)
      perspective->tintTarget = TINT_BRIGHT;
    else if (region == TINT_DARK_REGION &&
             !GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED))
      perspective->tintTarget = -TINT_DARK;
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
FIXED Terrain_GetFlatHeight(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

/// The height of a slope across x at position (0 to 15 of the cell): a straight
/// line from samples[0] to samples[1].
FIXED Terrain_GetSlopeXHeight(const s8 *samples, s32 position) {
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
FIXED Terrain_GetSlopeYHeight(const s8 *samples, s32 unused, s32 position) {
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
FIXED Terrain_GetStepSumDiagonalHeight(const s8 *samples, s32 x, s32 y) {
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
  position = x + y;
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
FIXED Terrain_GetStepDifferenceDiagonalHeight(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 second;
  s32 maximum;
  s32 difference;
  u32 position = x;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  difference = y - position;
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
FIXED Terrain_GetBendSumDiagonalHeight(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 middle;
  s32 last;
  s32 result;

  first = *samples++ << CURVE_VALUE_SHIFT;
  middle = *samples << CURVE_VALUE_SHIFT;
  last = samples[1] << CURVE_VALUE_SHIFT;
  // x now holds x + y, which is 15 on the diagonal.
  x = x + y;
  if ((u32)x == CURVE_FULL_STEPS - 1) {
    result = middle;
  } else if ((u32)x < CURVE_FULL_STEPS - 1) {
    result = first + (middle - first) * x / (CURVE_FULL_STEPS - 1);
  } else {
    x = x - (CURVE_FULL_STEPS - 1);
    result = middle + (last - middle) * x / (CURVE_FULL_STEPS - 1);
  }
  return result;
}

/// A slope that bends on the diagonal x = y: from sample 0 at the top right
/// corner to sample 1 on the diagonal to sample 2 at the bottom left.
FIXED Terrain_GetBendDifferenceDiagonalHeight(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 second;
  s32 third;
  s32 difference;
  s32 delta;

  first = *samples++ << CURVE_VALUE_SHIFT;
  second = samples[0] << CURVE_VALUE_SHIFT;
  third = samples[1];
  // x now holds y - x + 15, which is 15 on the diagonal.
  difference = y - x;
  x = difference + CURVE_FULL_STEPS - 1;
  third <<= CURVE_VALUE_SHIFT;

  if (x == CURVE_FULL_STEPS - 1)
    return second;

  if ((u32)x < CURVE_FULL_STEPS - 1) {
    delta = second - first;
    difference = delta * x;
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

/// A raised or sunken rounded square: sample 1 where gTerrain_RoundedSquareMask
/// is 1, sample 0 in the corners.
FIXED Terrain_GetRoundedSquareHeight(const s8 *samples, s32 position, s32 row) {
  s32 start;

  start = samples[0] << CURVE_VALUE_SHIFT;
  // The weight is 0 or 1, so the result is one sample or the other, never
  // between.
  return start +
         (((samples[1] << CURVE_VALUE_SHIFT) - start) *
          gTerrain_RoundedSquareMask[position + (row * CURVE_FULL_STEPS)]);
}

/// The height of a step across x at position (0 to 15 of the cell):
/// samples[0] on the cell's first half, samples[1] on its second.
FIXED Terrain_GetStepXHeight(const s8 *samples, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

/// A step along y: sample 0 above the middle, sample 1 below it.
FIXED Terrain_GetStepYHeight(const s8 *samples, u32 unused, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableHeight with x and y swapped.
FIXED Terrain_GetSampleTableTransposedHeight(const s8 *samples, u32 pixelX,
                                             u32 pixelY) {
  return samples[gTerrain_SampleIndexTable[pixelY + (pixelX << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableMirroredHeight with x and y swapped.
FIXED Terrain_GetSampleTableTransposedMirroredHeight(const s8 *samples, u32 row,
                                                     u32 position) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// The sample gTerrain_SampleIndexTable picks for (x, y): a byte per pixel
/// of the cell, row by row.
FIXED Terrain_GetSampleTableHeight(const s8 *samples, u32 position, u32 row) {
  return samples[gTerrain_SampleIndexTable[position + (row << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// The same, with the table mirrored left to right.
FIXED Terrain_GetSampleTableMirroredHeight(const s8 *samples, u32 position,
                                           u32 row) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// Above y 8 a step along x from the first sample to the second; below, the
/// third.
FIXED Terrain_GetSplitTopStepHeight(const s8 *samples, u32 column, u32 row) {
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
FIXED Terrain_GetSplitBottomStepHeight(const s8 *samples, u32 column, u32 row) {
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

/// The ground height the shape at code gives at pixel (x, z) of its cell, in
/// 16.16 pixels: its terrain kind's height function on its samples.
// UB: the kind byte is read, and the samples after it passed, in one
// unsequenced expression.
#ifdef UBFIX
#define MapGrid_GetShapeHeight(code, x, z)                                     \
  gMapGrid_TerrainHeightFunctions[(code)[0] & MAPGRID_SHAPE_KIND_MASK](        \
      (code) + 1, (x) & (MAPGRID_CELL_PIXELS - 1),                             \
      (z) & (MAPGRID_CELL_PIXELS - 1))
#else
#define MapGrid_GetShapeHeight(code, x, z)                                     \
  gMapGrid_TerrainHeightFunctions[*(code)++ & MAPGRID_SHAPE_KIND_MASK](        \
      (code), (x) & (MAPGRID_CELL_PIXELS - 1),                                 \
      (z) & (MAPGRID_CELL_PIXELS - 1))
#endif

/// Returns the ground height of a layer at (x, z) in 16.16 pixels, as The
/// Broken Seal's: the height function of the cell's terrain kind applied to
/// its samples.
FIXED MapGrid_GetTerrainHeight(s32 layer, FIXED x, FIXED z) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *grid;
  u8 *code;

  x >>= 16;
  z >>= 16;
  if (map != NULL)
    grid = map->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  // A row of the grid is MAPGRID_SIZE cells, 1 << 7; a shape is 4 bytes.
  grid += x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7);
  code = Ram_MapCollision + *grid * 4;
  return MapGrid_GetShapeHeight(code, x, z);
}

/// Returns the terrain kind of a layer's cell at (x, z) in 16.16 pixels, as
/// The Broken Seal's.
s32 MapGrid_GetTerrainKindAt(s32 layer, FIXED x, FIXED z) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (map != NULL)
    grid = map->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / MAPGRID_CELL_PIXELS +
                               (z / MAPGRID_CELL_PIXELS << 7)] *
                          4] &
         MAPGRID_SHAPE_KIND_MASK;
}

/// As MapGrid_GetTerrainKindAt, but a rounded square whose two
/// samples differ reads as TERRAIN_BUMP.
s32 MapGrid_GetTerrainKindOrBumpAt(s32 layer, s32 x, s32 z) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *grid;
  u8 *code;
  s32 kind;

  x >>= 16;
  z >>= 16;
  if (map != NULL)
    grid = map->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  code =
      &Ram_MapCollision
          [grid[x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7)] * 4];
  kind = code[0];
  if (kind == TERRAIN_ROUNDED_SQUARE && code[1] != code[2])
    kind = TERRAIN_BUMP;
  return kind;
}

/// The highest ground at (x, z) and at the four points `reach` away from it
/// along x and z, all in 16.16 pixels.
FIXED MapGrid_GetHighestTerrainHeightAround(s32 layer, FIXED x, FIXED z,
                                            FIXED reach) {
  FIXED best;
  FIXED height;

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
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *shape;

  shape = gMapShapeGrid +
          (map->layers[layer & 3].cells - (struct MapCell *)Ram_MapCellBuffer);
  shape += (FIELD_CELL(x)) + FIELD_CELL(z) * MAPGRID_SIZE;
  *shape = MapCollision_FindOrAddTagFar(layer, 0, height);
}

/// The ground height at (x, z) of layer, in 8-pixel steps.
s32 MapGrid_GetCellHeight(FIXED x, FIXED z, s32 layer) {
  return MapGrid_GetTerrainHeight(layer, x, z) >> CURVE_VALUE_SHIFT;
}

/// As MapGrid_GetTerrainKindAt, a second copy.
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (map != NULL)
    grid = map->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / MAPGRID_CELL_PIXELS +
                               (z / MAPGRID_CELL_PIXELS << 7)] *
                          4] &
         MAPGRID_SHAPE_KIND_MASK;
}

/// Returns the cell behavior of a layer's cell at (x, y) in 16.16 pixels.
u8 MapGrid_GetCellBehaviorAt(s32 layer, FIXED x, FIXED y) {
  struct MapState *state;
  struct MapCell *cells;

  state = Ram_HeapSlots->mapGrid;
  x = FIELD_CELL(x);
  y = FIELD_CELL(y);
  cells = (struct MapCell *)gMapGridCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + y * MAPGRID_SIZE;
  return cells->collisionCode;
}

/// Sets the cell behavior of a layer's cell at (x, y) in 16.16 pixels. Writes
/// the cell in the map's grid; does nothing before the map is set up.
void MapGrid_SetCellBehaviorAt(u32 layer, FIXED x, FIXED y, u32 collisionCode) {
  struct MapState *state = Ram_HeapSlots->mapGrid;

  x = FIELD_CELL(x);
  y = FIELD_CELL(y);
  if (state != NULL) {
    struct MapCell *cells = state->layers[layer & 3].cells;

    cells += x + y * MAPGRID_SIZE;
    cells->collisionCode = collisionCode;
  }
}

/// The height of the ground at (px, pz) (16.16 pixels) on the given layer,
/// less base; a layer past 2 reads the shape grid. With no map, or on the
/// World Map, it is 0.
FIXED MapGrid_GetTerrainHeightAbove(FIXED px, FIXED pz, s32 layer, FIXED base) {
  s32 x = FIXEDtoINT(px);
  s32 z = FIXEDtoINT(pz);
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u8 *shapes;
  u8 *code;

  if (map == NULL)
    return 0;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
    return 0;
  if (layer <= 2)
    shapes = map->layers[layer].terrainIds;
  else
    shapes = gMapShapeGrid;
  code = Ram_MapCollision +
         shapes[x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS << 7)] * 4;
  return MapGrid_GetShapeHeight(code, x, z) - base;
}

/// How the ground at (px, pz) (16.16 pixels) on the given layer stands to
/// height: 2 on a wall, 1 when the ground rises more than 8 pixels above it,
/// -1 when it falls more than 12 below, else 0. A layer past 2 reads the cell
/// buffer and shape grid. With no map, or on the World Map, every point is
/// level (0).
s32 MapGrid_CheckCollisionAt(FIXED px, FIXED pz, s32 layer, s32 height) {
  s32 x = FIXEDtoINT(px);
  s32 z = FIXEDtoINT(pz);
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
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
  rise = MapGrid_GetShapeHeight(code, x, z) - height;
  if (rise > toFIXED(8))
    return 1;
  if (rise < -toFIXED(12))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollisionAt for the point position (16.16 pixels, read as
/// its whole-pixel halves) on the actor's layer, against its ground height.
s32 MapGrid_CheckCollision(struct Actor *actor,
                           struct FieldPosition *position) {
  s32 x = ((struct FieldCoordinate *)&position->x)->part[0].integer;
  s32 z = ((struct FieldCoordinate *)&position->z)->part[0].integer;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
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
  rise = MapGrid_GetShapeHeight(code, x, z) - actor->groundHeight;
  if (rise > toFIXED(8))
    return 1;
  if (rise < -toFIXED(12))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollision, against the actor's height and with no -1: 2 on
/// a wall, 1 when the ground rises more than 8 pixels above it, else 0.
s32 MapGrid_CheckCollisionAbove(struct Actor *actor,
                                struct FieldPosition *position) {
  s32 x = ((struct FieldCoordinate *)&position->x)->part[0].integer;
  s32 z = ((struct FieldCoordinate *)&position->z)->part[0].integer;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;

  if (map == NULL)
    return 0;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
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
  if (MapGrid_GetShapeHeight(code, x, z) - actor->y.fixed > toFIXED(8))
    return 1;
  return 0;
}

/// Returns -1 when the cell under position (its x, and z less its height) in
/// the overhead layer is a wall, else 0; also 0 before the map state exists.
s32 MapGrid_IsWallAt(s32 *position) {
  s32 x = position[0] / FIXED_ONE;
  s32 z = (u32)((position[2] & FIELD_CELL_MASK) - position[1]) / FIXED_ONE;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;

  if (map == NULL)
    return 0;
  cell = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
  cell += x / MAPGRID_CELL_PIXELS + (z / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  return (cell->collisionCode != CELL_BEHAVIOR_WALL) - 1;
}

/// The cell behavior of the world map block under position (blocks 32 pixels
/// square on a 32 by 32 grid), which the World Map's step triggers answer
/// (world_map.c: STEP_TRIGGER_AREA(AREA_DAILA) enters Daila,
/// CELL_BEHAVIOR_WORLD_MAP_IRON meets Iron), or CELL_BEHAVIOR_WALL where the
/// map has no block.
s32 WorldMap_GetBlockCellBehavior(struct WorldPosition *position) {
  s32 z = position->y;
  s32 x = position->x;

  // Rounded towards zero, as a division would.
  if (x < 0)
    x += toFIXED(32) - 1;
  x = WORLD_MAP_BLOCK(x) & 31;
  if (z < 0)
    z += toFIXED(32) - 1;
  z = WORLD_MAP_BLOCK(z) & 31;
  return ((struct WorldMapBlocks *)gCellTiles)->blocks[x + z * 32].place;
}

/// Returns the world map's collision value (1-15) at position: from the upper
/// collision layer where it has one, else the lower, else
/// WORLD_MAP_COLLISION_DEFAULT.
s32 WorldMap_GetCollision(struct WorldPosition *position) {
  s32 halfPixelX = FIXEDtoINT(position->x) >> 1;
  s32 y = position->y;
  s32 halfPixelY = FIXEDtoINT(y) >> 1;
  u32 collisionCell;
  u32 pixelOffset;
  u8 collisionTile;
  u8 packedCollisions;
  u32 result;

  // Coordinates in 2-pixel units, a tile map's tile 8 of them across.
  collisionCell = ((halfPixelY / 8) & (WORLD_MAP_TILE_MAP_WIDTH - 1)) *
                      WORLD_MAP_TILE_MAP_WIDTH +
                  ((halfPixelX / 8) & (WORLD_MAP_TILE_MAP_WIDTH - 1));
  pixelOffset = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

  // The upper layer's collision tiles follow the lower layer's.
  collisionTile = WORLD_MAP_UPPER_TILE_MAP[collisionCell];
  packedCollisions =
      *(Ram_MapCollision +
        WORLD_MAP_COLLISION_TILES * WORLD_MAP_COLLISION_TILE_BYTES +
        collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 0xf;
    if (result != 0)
      return result;
  }

  // The lower layer's, at the start of the buffer.
  collisionTile = WORLD_MAP_LOWER_TILE_MAP[collisionCell];
  packedCollisions =
      *(Ram_MapCollision + collisionTile * WORLD_MAP_COLLISION_TILE_BYTES +
        pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 0xf;
    if (result != 0)
      return result;
  }

  return WORLD_MAP_COLLISION_DEFAULT;
}

/// Returns 0 when the world map at position can be walked on (a place there,
/// and collision WORLD_MAP_COLLISION_WALKABLE_FIRST to
/// WORLD_MAP_COLLISION_WALKABLE_LAST), else -1.
s32 WorldMap_IsWalkable(void *mover, struct WorldPosition *position) {
  if (WorldMap_GetBlockCellBehavior(position) != CELL_BEHAVIOR_WALL) {
    if ((u32)(WorldMap_GetCollision(position) -
              WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
        WORLD_MAP_COLLISION_WALKABLE_LAST - WORLD_MAP_COLLISION_WALKABLE_FIRST)
      return 0;
  }
  return -1;
}

/// Returns 0 where the ship can sail (the sea or shallow water), else -1.
s32 WorldMap_IsSailable(void *mover, struct WorldPosition *position) {
  if (WorldMap_GetBlockCellBehavior(position) != CELL_BEHAVIOR_WALL) {
    s32 collision = WorldMap_GetCollision(position);
    struct FieldState *state = Ram_HeapSlots->fieldState;
    s32 mode;

    // Each of the three kinds of ground last flown over sails the same water.
    mode = state->terrainKind;
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

/// Returns 0 where the flying ship can go, else -1: low down, where it can
/// sail; higher, more of the ground by the kind it last flew over.
s32 WorldMap_IsFlyable(struct Actor *mover, struct WorldPosition *position) {
  if (WorldMap_GetBlockCellBehavior(position) != CELL_BEHAVIOR_WALL) {
    s32 collision = WorldMap_GetCollision(position);
    struct FieldState *state = Ram_HeapSlots->fieldState;

    s32 height = mover->y.fixed;

    if (height < toFIXED(4)) {
      if ((u32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) >
              WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_SEA_FIRST &&
          collision != WORLD_MAP_COLLISION_SHALLOW_WATER)
        return -1;
      return 0;
    } else {
      s32 mode = state->terrainKind;

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
        // XXX (no evidence): what ground collision 10 is, which a battle
        // counts as grassland as it does 7, 8, 11 and 12; nothing in reach
        // shows the World Map's collision under a point, and no text names it.
        if (collision == 10)
          return 0;
      }
    }
  }
  return -1;
}
