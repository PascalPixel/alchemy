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

/// Returns the world map's battle environment at position from
/// gWorldMap_TerrainBehaviorTable, by its collision value and its block's
/// terrain, and stores the block's terrain kind in terrainKind.
u8 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                 s32 *terrainKind) {
  s32 selector = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 flag = 0;
  u32 tileX;
  u32 index;
  u32 *tile;
  u32 kind;

  if (x < 0)
    x += toFIXED(32) - 1;
  tileX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += toFIXED(32) - 1;

  index = tileX + (((y >> 21) & 31) << 5);
  tile = gCellPictures + index;
  if (((u8 *)tile)[3] & 0x80)
    flag = 16;

  kind = (*tile << 2) >> 26;
  *terrainKind = kind;
  if (kind > 40)
    flag = 32;
  if (kind == 36)
    flag = 64;
  if (kind == 12 || kind == 15)
    flag = 48;

  return gWorldMap_TerrainBehaviorTable[flag + selector];
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

/// Draws the current affine page into the world map's still-picture pixels.
/// WorldMap_StartStillPicture supplies 160 pairs and a 240 by 160 byte output.
void TileMap_DrawRowsFromRam(s32 lines, s32 pixels) {
  void (*routine)(s32, s32, u8 *, u8 *);
  u8 *tiles = gDecodeBuffer;

  RamCode_Load(routine, Render_DrawMapCode, Render_DrawMapCodeSize);
  routine(lines, pixels, WORLD_MAP_TILE_SET(2), tiles + 4096);
  Heap_Release(routine);
}

/// Rebuilds the ground plane's scanline terms when the world map pitch changes.
void WorldMap_BuildScanlineTable(FIXED pitchSlope, s32 *position,
                                 struct WorldMapScanline *row) {
  s32 vector[3];
  s32 result[3];
  FIXED height;
  FIXED nearZ;
  s32 i;
  s32 z;
  FIXED ratio;
  s32 dx;
  FIXED sum;
  s32 zero;

  zero = 0;
  vector[0] = position[0];
  vector[1] = zero;
  vector[2] = position[2];
  Iwram_TransformVector(vector, result);
  height = result[1] - Iwram_MulFX(result[2], pitchSlope);
  nearZ = -gProjection.focal;
  for (i = 0; i < 160; i++) {
    FIXED distance;

    z = Iwram_RatioMulQ14(nearZ, (gProjection.centerY - i) * FIXED_ONE);
    distance = z - pitchSlope;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->scale = Iwram_RatioMulQ14(gProjection.focal,
                                     Iwram_MulFX(-ratio, FIXED_ONE / 2));
      z = Iwram_MulFX(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulFX(dx, dx);
      z >>= 4;
      sum += Iwram_MulFX(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->depth = Iwram_MulFX(length, FIXED_ONE / 2);
      }
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}
