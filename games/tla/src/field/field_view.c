/// @file
/// Screen shake, the copied BG row drawer and the ground plane scanlines.
// Part of field.c.

#include "field_internal.h"

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/ram_code.h"
#include "field/resource_slot.h"

#define MAPGRID_SHAKE_SETTLE_FRAMES 300
#define MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE (toFIXED(1) / 256 - 1)
/// One of the world map's two tile maps of one-byte tiles, BG3's and then
/// BG2's, as WorldMap_StartStillPicture copies them into gDecodeBuffer.
#define TILEMAP_MAP_BANK_BYTES                                                 \
  (WORLD_MAP_TILE_MAP_WIDTH * WORLD_MAP_TILE_MAP_WIDTH)

/// The bit of a world map block's top byte, its word's bit 31, that makes it a
/// beach.
#define WORLD_MAP_BLOCK_BEACH (1 << 7)

/// The banks of gWorldMap_TerrainBehaviorTable, 16 collision values each,
/// which differ in the battles their walkable ground and forest give.
enum TerrainBehaviorBank {
  TERRAIN_BANK_PLAIN = 0,
  TERRAIN_BANK_BEACH = 16, ///< walkable ground fights on the beach
  TERRAIN_BANK_SNOWFIELD = 32,
  TERRAIN_BANK_DESERT = 48, ///< walkable ground fights in the desert
  TERRAIN_BANK_BLIZZARD = 64
};

/// The encounter regions (WorldMapCell.region) whose blocks take a bank of
/// their own.
enum {
  AIRS_ROCK_DESERT_REGION = 12,     ///< the desert round Air's Rock
  SHAMAN_VILLAGE_SANDS_REGION = 15, ///< the sandy shore north of Shaman Village
  PROX_BLIZZARD_REGION = 36,        ///< the snow round Prox
  SNOWFIELD_REGION_FIRST = 41       ///< and every region after it
};

/// Returns the world map's battle environment at position from
/// gWorldMap_TerrainBehaviorTable, by its collision value and its block's bank,
/// and stores the block's encounter region in terrainKind.
u8 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                 s32 *terrainKind) {
  s32 collisionKind = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 bank = TERRAIN_BANK_PLAIN;
  u32 blockX;
  u32 index;
  u32 *block;
  u32 region;

  // The bias rounds a negative coordinate towards zero, as division would.
  if (x < 0)
    x += toFIXED(32) - 1;
  blockX = WORLD_MAP_BLOCK(x) & (WORLD_MAP_VIEW_CELLS - 1);
  y = position->y;
  if (y < 0)
    y += toFIXED(32) - 1;

  index = blockX + ((WORLD_MAP_BLOCK(y) & (WORLD_MAP_VIEW_CELLS - 1)) << 5);
  block = gCellPictures + index;
  if (((u8 *)block)[3] & WORLD_MAP_BLOCK_BEACH)
    bank = TERRAIN_BANK_BEACH;

  region = ((struct WorldMapCell *)block)->region;
  *terrainKind = region;
  if (region >= SNOWFIELD_REGION_FIRST)
    bank = TERRAIN_BANK_SNOWFIELD;
  if (region == PROX_BLIZZARD_REGION)
    bank = TERRAIN_BANK_BLIZZARD;
  if (region == AIRS_ROCK_DESERT_REGION ||
      region == SHAMAN_VILLAGE_SANDS_REGION)
    bank = TERRAIN_BANK_DESERT;

  return gWorldMap_TerrainBehaviorTable[bank + collisionKind];
}

/// Changes the shake without restarting an axis whose argument is negative.
/// Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per frame.
void MapGrid_SetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay) {
  struct MapWork *grid;

  // A negative argument leaves that value alone, so an event can change the
  // decay without restarting the shake, or start one axis only.
  grid = Ram_HeapSlots->mapGrid;
  if (shakeX >= 0) {
    grid->shakeX = shakeX;
  }
  if (shakeY >= 0) {
    grid->shakeY = shakeY;
  }
  if (shakeDecay >= 0) {
    grid->shakeDecay = shakeDecay;
  }
}

/// Waits until both shake amplitudes are below 1/256 pixel, with a 300-frame
/// limit for a shake whose decay never settles.
void MapGrid_WaitForShakeToSettle(void) {
  struct MapWork *grid;
  s32 waitedFrames;

  grid = Ram_HeapSlots->mapGrid;
  waitedFrames = 0;
  // 300 frames is five seconds; 255 in 16.16 is just under 1/256 pixel.
  // A decay of 1.0 or more never settles, so the limit ends the wait.
  while (waitedFrames < MAPGRID_SHAKE_SETTLE_FRAMES &&
         (grid->shakeX > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE ||
          grid->shakeY > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE)) {
    wait(1);
    waitedFrames++;
  }
  // A zero decay makes the next view update's shake its last.
  grid->shakeDecay = 0;
}

/// The copied ARM kernel reads the BG2 affine row from each 32-byte pair and
/// writes 240 by 160 pixels as 256-colour tiles; it returns the address past
/// the last pair, where its row cursor stops.
typedef const struct WorldMapAffinePair *(*TileMapRowDrawer)(
    const struct WorldMapAffinePair *lines, u8 *pixels, const u8 *tiles,
    const u8 *map);

/// Draws the current affine page into the world map's still-picture pixels.
/// WorldMap_StartStillPicture supplies 160 pairs and a 240 by 160 byte output.
void TileMap_DrawRowsFromRam(const struct WorldMapAffinePair *lines,
                             u8 *pixels) {
  TileMapRowDrawer routine;
  u8 *maps = gDecodeBuffer;

  // The second tile set and tile map are BG2's.
  RamCode_Load(routine, Render_DrawMapCode, Render_DrawMapCodeSize);
  routine(lines, pixels, WORLD_MAP_TILE_SET(2), maps + TILEMAP_MAP_BANK_BYTES);
  Heap_Release(routine);
}

/// Rebuilds the ground plane's scanline terms when the world map pitch changes.
void WorldMap_BuildScanlineTable(FIXED pitchSlope, s32 *position,
                                 struct WorldMapScanline *scanline) {
  s32 groundPoint[3];
  s32 cameraPoint[3];
  FIXED horizon;
  FIXED focus;
  s32 line;
  s32 distance;
  FIXED scale;
  s32 horizontal;
  FIXED squaredSum;
  s32 groundHeight;

  // The followed point on the ground (height 0), in camera space; horizon is
  // its height less its depth times the pitch slope.
  groundHeight = 0;
  groundPoint[0] = position[0];
  groundPoint[1] = groundHeight;
  groundPoint[2] = position[2];
  Iwram_TransformVector(groundPoint, cameraPoint);
  horizon = cameraPoint[1] - Iwram_MulFX(cameraPoint[2], pitchSlope);
  focus = -gProjection.focal;
  for (line = 0; line < DISPLAY_HEIGHT; line++) {
    FIXED difference;

    // The line's ray slope: its offset from the projection's centre line over
    // the focal length.
    distance =
        Iwram_RatioMulQ14(focus, (gProjection.centerY - line) * FIXED_ONE);
    // A ray parallel to the ground would divide by zero.
    difference = distance - pitchSlope;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    // A negative scale is a line below the horizon, whose ray meets the
    // ground; the lines above it are sky and get zeros.
    if (scale < 0) {
      // MulFX by FIXED_ONE / 2 halves: the width scale is half the line's scale
      // over the focal length, and the distance below is stored halved.
      scanline->scale = Iwram_RatioMulQ14(gProjection.focal,
                                          Iwram_MulFX(-scale, FIXED_ONE / 2));
      // The distance is the root of the two squared terms; distance becomes
      // the vertical one.
      distance = Iwram_MulFX(scale, distance);
      horizontal = (cameraPoint[2] - scale) >> 4;
      distance -= cameraPoint[1];
      squaredSum = Iwram_MulFX(horizontal, horizontal);
      distance >>= 4;
      squaredSum += Iwram_MulFX(distance, distance);
      {
        // The root of a 16.16 square comes back in 8.8, and the terms were
        // taken in sixteenths.
        s32 groundDistance = Iwram_Sqrt(squaredSum) << (8 + 4);

        // The ground distance takes the sign of the vertical term.
        if (distance < 0)
          groundDistance = -groundDistance;
        scanline->depth = Iwram_MulFX(groundDistance, FIXED_ONE / 2);
      }
    } else {
      scanline->scale = 0;
      scanline->depth = 0;
    }
    scanline->offsetX = 0;
    scanline->offsetY = 0;
    scanline++;
  }
}
