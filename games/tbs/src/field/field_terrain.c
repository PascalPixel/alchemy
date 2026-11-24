/// @file
/// Ground heights, and the collision of cells and the world map.
// Part of field.c.

#include "field_internal.h"

/// A sample is in eight-pixel units.
#define CURVE_HEIGHT_FIXED(sample) ((sample) * (1 << CURVE_VALUE_SHIFT))

/// The ground height functions (gMapGrid_TerrainHeightFunctions): each takes a
/// cell's height samples after its kind byte and a pixel (x, y) within the
/// 16x16 cell, and returns the ground height there in 16.16 pixels.
FIXED Terrain_GetFlatHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// The slopes along one axis: from sample 0 where `along`, the pixel's
/// coordinate on that axis, is 0, towards sample 1 where it is 16, rounded
/// towards zero as dividing by 16 would.
#define TERRAIN_DEFINE_SLOPE(name, along)                                      \
  s32 name(const u8 *heightBytes, s32 pixelX, s32 pixelY) {                    \
    const s8 *samples = (const s8 *)heightBytes;                               \
    s32 startHeight;                                                           \
    s32 heightDelta;                                                           \
                                                                               \
    startHeight = CURVE_HEIGHT_FIXED(samples[0]);                              \
    heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * (along);    \
    if (heightDelta < 0) {                                                     \
      heightDelta += CURVE_FULL_STEPS - 1;                                     \
    }                                                                          \
    return startHeight + (heightDelta >> 4);                                   \
  }

/// A slope along x.
TERRAIN_DEFINE_SLOPE(Terrain_GetSlopeXHeight, pixelX)
/// A slope along y.
TERRAIN_DEFINE_SLOPE(Terrain_GetSlopeYHeight, pixelY)

/// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
/// it and the higher of the two on it.
FIXED Terrain_GetStepSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 secondHeight;
  s32 higherHeight;
  u32 summedPixel;

  firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
  secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
  higherHeight = firstHeight;
  if (secondHeight > firstHeight) {
    higherHeight = secondHeight;
  }
  // x + y runs from 0 to 30; 15 is the diagonal.
  summedPixel = x + y;
  if (summedPixel == CURVE_FULL_STEPS - 1) {
    return higherHeight;
  }
  if (summedPixel < CURVE_FULL_STEPS - 1) {
    secondHeight = firstHeight;
  }
  return secondHeight;
}

/// A step across the diagonal x = y: sample 0 where y < x, sample 1 where
/// y > x and the higher of the two on it.
FIXED Terrain_GetStepDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
                                              s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 secondHeight;
  s32 higherHeight;
  s32 pixelDifference;
  u32 biasedDifference = x;

  firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
  secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
  higherHeight = firstHeight;
  if (secondHeight > firstHeight) {
    higherHeight = secondHeight;
  }
  // y - x biased by 15 runs from 0 to 30, so the diagonal is 15 and one
  // unsigned compare tells the sides apart, as in TERRAIN_STEP_SUM_DIAGONAL.
  pixelDifference = y - biasedDifference;
  biasedDifference = pixelDifference + CURVE_FULL_STEPS - 1;
  if (biasedDifference == CURVE_FULL_STEPS - 1) {
    return higherHeight;
  }
  if (biasedDifference < CURVE_FULL_STEPS - 1) {
    secondHeight = firstHeight;
  }
  return secondHeight;
}

/// A slope that bends on the diagonal x + y = 15: from sample 0 at the top
/// left corner to sample 1 on the diagonal to sample 2 at the bottom right.
FIXED Terrain_GetBendSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 height;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples);
  lastHeight = CURVE_HEIGHT_FIXED(samples[1]);
  // x + y from 0 to 30: 15 is the diagonal, below it the first half and above
  // it the second.
  x += y;
  if (x == CURVE_FULL_STEPS - 1) {
    height = middleHeight;
  } else if ((u32)x < CURVE_FULL_STEPS - 1) {
    height =
        firstHeight + (middleHeight - firstHeight) * x / (CURVE_FULL_STEPS - 1);
  } else {
    x -= (CURVE_FULL_STEPS - 1);
    height =
        middleHeight + (lastHeight - middleHeight) * x / (CURVE_FULL_STEPS - 1);
  }
  return height;
}

/// A slope that bends on the diagonal x = y: from sample 0 at the top right
/// corner to sample 1 on the diagonal to sample 2 at the bottom left.
FIXED Terrain_GetBendDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
                                              s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 pixelDifference;
  s32 heightDelta;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(samples[0]);
  lastHeight = samples[1];
  // As TERRAIN_BEND_SUM_DIAGONAL, on y - x biased by 15.
  pixelDifference = y - x;
  x = pixelDifference + CURVE_FULL_STEPS - 1;
  lastHeight = CURVE_HEIGHT_FIXED(lastHeight);

  if (x == CURVE_FULL_STEPS - 1)
    return middleHeight;

  if ((u32)x < CURVE_FULL_STEPS - 1) {
    heightDelta = middleHeight - firstHeight;
    pixelDifference = heightDelta * x;
    if (pixelDifference < 0)
      // Round towards zero, as dividing by 16 would.
      pixelDifference += CURVE_FULL_STEPS - 1;
    pixelDifference >>= 4;
    return firstHeight + pixelDifference;
  }
  // The second half measures from the diagonal by y - x itself, 1 to 15.
  pixelDifference *= lastHeight - middleHeight;
  if (pixelDifference < 0)
    pixelDifference += CURVE_FULL_STEPS - 1;
  pixelDifference >>= 4;
  return middleHeight + pixelDifference;
}

/// A raised or sunken rounded square: sample 1 where gTerrain_LerpWeightTable
/// is 1, sample 0 in the corners.
FIXED Terrain_GetRoundedSquareHeight(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  // The weight is 0 or 1, so the result is one sample or the other, never
  // between.
  return startHeight + ((CURVE_HEIGHT_FIXED(samples[1]) - startHeight) *
                        gTerrain_LerpWeightTable[x + (y * CURVE_FULL_STEPS)]);
}

/// The steps along one axis: sample 0 where `along`, the pixel's coordinate
/// on that axis, is below 8, sample 1 from 8.
#define TERRAIN_DEFINE_STEP(name, along)                                       \
  s32 name(const u8 *heightBytes, s32 pixelX, s32 pixelY) {                    \
    const s8 *samples = (const s8 *)heightBytes;                               \
    s32 heightSample;                                                          \
    if ((u32)(along) < CURVE_HALF_STEPS)                                       \
      heightSample = samples[0];                                               \
    else                                                                       \
      heightSample = samples[1];                                               \
    return CURVE_HEIGHT_FIXED(heightSample);                                   \
  }

/// A step along x.
TERRAIN_DEFINE_STEP(Terrain_GetStepXHeight, pixelX)
/// A step along y.
TERRAIN_DEFINE_STEP(Terrain_GetStepYHeight, pixelY)

/// A slope along x that bends at the middle: sample 0 at x = 0, sample 1 at
/// x = 8, towards sample 2 at x = 16.
FIXED Terrain_GetBendXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  s32 endpointHeight;
  s32 middleHeight;
  s32 heightDelta;

  endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples++);
  // Each half is 8 pixels: the shift by 3 divides by 8, and the bias of 7
  // rounds towards zero.
  if (cellX < CURVE_HALF_STEPS) {
    heightDelta = (middleHeight - endpointHeight) * cellX;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return endpointHeight + (heightDelta >> 3);
  }

  {
    s32 offsetPastMiddle;

    endpointHeight = CURVE_HEIGHT_FIXED(*samples);
    offsetPastMiddle = cellX;
    endpointHeight -= middleHeight;
    offsetPastMiddle -= CURVE_HALF_STEPS;
    heightDelta = endpointHeight * offsetPastMiddle;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return middleHeight + (heightDelta >> 3);
  }
}

/// A slope along y that bends at the middle, as TERRAIN_BEND_X does along x.
FIXED Terrain_GetBendYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const u8 *samples = heightBytes;
  u32 cellY = pixelY;
  s32 endpointHeight;
  s32 middleHeight;
  s32 offsetPastMiddle;
  s32 heightDelta;
  s32 tailDelta;

  endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples++);
  // As TERRAIN_BEND_X: halves of 8 pixels, rounded towards zero.
  if (cellY < CURVE_HALF_STEPS) {
    heightDelta = (middleHeight - endpointHeight) * cellY;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return endpointHeight + (heightDelta >> 3);
  }

  endpointHeight = CURVE_HEIGHT_FIXED(*samples);
  offsetPastMiddle = cellY;
  endpointHeight -= middleHeight;
  offsetPastMiddle -= CURVE_HALF_STEPS;
  tailDelta = endpointHeight * offsetPastMiddle;
  if (tailDelta < 0) {
    tailDelta += CURVE_HALF_STEPS - 1;
  }
  return middleHeight + (tailDelta >> 3);
}

/// The sample gTerrain_SampleIndexTable picks for (x, y).
FIXED Terrain_GetSampleTableHeight(const u8 *heightBytes, s32 pixelX,
                                   s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  // The table holds a byte per pixel of the cell, row by row.
  return CURVE_HEIGHT_FIXED(
      samples[gTerrain_SampleIndexTable[pixelX + (pixelY << 4)]]);
}

/// The same, with the table mirrored left to right.
FIXED Terrain_GetSampleTableMirroredHeight(const u8 *heightBytes, s32 pixelX,
                                           s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  // Row y read from its right end: (y << 4) + 15 - x.
  return CURVE_HEIGHT_FIXED(
      samples[gTerrain_SampleIndexTable[((pixelY << 4) - pixelX) +
                                        CURVE_FULL_STEPS - 1]]);
}

/// Flat, as Terrain_GetFlatHeight, for TERRAIN_FLAT_B.
FIXED Terrain_GetFlatBHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// Flat, as Terrain_GetFlatHeight, for TERRAIN_FLAT_C.
FIXED Terrain_GetFlatCHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

#undef TERRAIN_DEFINE_SLOPE
#undef TERRAIN_DEFINE_STEP
#undef CURVE_HEIGHT_FIXED

/// Returns the ground height of a layer at (x, y) in 16.16 pixels: the
/// height function of the cell's terrain kind applied to its samples.
/// y is the map's down axis, an actor's z.
FIXED MapGrid_GetTerrainHeight(s32 layer, FIXED x, FIXED y) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct MapGridCell *cell;
  u8 *heightBytes;

  x = FIXEDtoINT(x);
  y = FIXEDtoINT(y);
  // Before the map state exists, the grid is read from its start.
  cell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  if (grid != NULL) {
    cell = grid->layers[layer & 3].cells;
  }
  // Cells are 16 pixels, 128 to a row.
  cell += x / MAPGRID_CELL_PIXELS + (y / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  heightBytes = Ram_MapGridShapes + cell->shape * 4;
  // UB: read the kind byte before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & MAPGRID_SHAPE_KIND_MASK;

    return gMapGrid_TerrainHeightFunctions[kind](heightBytes,
                                                 x & (MAPGRID_CELL_PIXELS - 1),
                                                 y & (MAPGRID_CELL_PIXELS - 1));
  }
#else
  return gMapGrid_TerrainHeightFunctions[*heightBytes++ &
                                         MAPGRID_SHAPE_KIND_MASK](
      heightBytes, x & (MAPGRID_CELL_PIXELS - 1),
      y & (MAPGRID_CELL_PIXELS - 1));
#endif
}

/// Returns the terrain kind (enum TerrainKind) of a layer's cell at (x, y) in
/// 16.16 pixels.
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, FIXED x, FIXED y) {
  struct MapGridScrollState *gridState = gMapGridState[0];
  struct MapGridCell *cells;
  s32 cellX;
  s32 cellY;
  u32 shape;

  x = FIXEDtoINT(x);
  y = FIXEDtoINT(y);
  cells = (struct MapGridCell *)gMapGridCellBuffer;
  if (gridState != 0) {
    cells = gridState->layers[layer & 3].cells;
  }
  cellX = x / MAPGRID_CELL_PIXELS;
  cellY = y / MAPGRID_CELL_PIXELS;
  cells += cellX + cellY * MAPGRID_SIZE;
  // The kind byte of the cell's four-byte shape.
  shape = cells->shape;
  return *(Ram_MapGridShapes + shape * 4) & MAPGRID_SHAPE_KIND_MASK;
}

/// Returns the cell behavior of a layer's cell at (x, y) in 16.16 pixels.
s32 MapGrid_GetCellBehaviorAt(s32 layer, FIXED x, FIXED y) {
  struct MapGridScrollState *gridState;
  struct MapGridCell *cell;

  gridState = gMapGridState[0];
  x = FIELD_CELL(x);
  y = FIELD_CELL(y);
  // Before the map state exists, the grid is read from its start.
  cell = (struct MapGridCell *)gMapGridCellBuffer;
  if (gridState != NULL) {
    cell = gridState->layers[layer & 3].cells;
  }
  cell += x + y * MAPGRID_SIZE;
  return cell->behavior;
}

/// Sets the cell behavior of a layer's cell at (x, y) in 16.16 pixels. Writes
/// the cell in the map's grid; does nothing before the map is set up.
void MapGrid_SetCellBehaviorAt(u32 layer, FIXED x, FIXED y, u32 behavior) {
  struct MapGridScrollState *gridState = gMapGridState[0];

  x = FIELD_CELL(x);
  y = FIELD_CELL(y);
  // Without the map state there is no layer to write to.
  if (gridState != NULL) {
    struct MapGridCell *cell = gridState->layers[layer & 3].cells;

    cell += x + y * MAPGRID_SIZE;
    cell->behavior = behavior;
  }
}

/// Unused: returns the top two bits of the cell tiles halfword of the cell at
/// pixel (x, y).
UNUSED s32 MapGrid_GetCellHighFlags(s32 x, s32 y) {
  s32 cellX = x / MAPGRID_CELL_PIXELS;
  s32 cellY = y / MAPGRID_CELL_PIXELS;
  struct MapGridCell *cell =
      (struct MapGridCell *)gMapGridCellBuffer + (cellX + cellY * MAPGRID_SIZE);

  // The high byte of the cell tiles halfword; its top two bits are flags.
  return ((u8 *)&cell->cellTilesAndFlags)[1] >> 6;
}

/// Returns whether an actor can step to position, an enum Collision: whether
/// the cell there is a wall, or the ground too high or too low for a step from
/// the actor's.
s32 MapGrid_CheckCollision(struct Actor *actor,
                           struct FieldPosition *position) {
  struct MapGridScrollState *gridState;
  s32 pixelX;
  s32 pixelZ;
  struct MapGridCell *cell;
  u8 *heightBytes;
  s32 targetGround;
  s32 stepHeight;

  pixelX = ((struct FieldCoordinate *)&position->x)->part[0].integer;
  pixelZ = ((struct FieldCoordinate *)&position->z)->part[0].integer;
  gridState = gMapGridState[0];
  if (gridState == NULL)
    return COLLISION_NONE;
  // The three layers have their windows into the grid; any other layer
  // reads the grid from its start.
  if (actor->layer <= MAPGRID_LAYER_OVERHEAD)
    cell = gridState->layers[actor->layer].cells;
  else
    cell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  cell += pixelX / MAPGRID_CELL_PIXELS +
          (pixelZ / MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  // A wall blocks whatever the heights.
  if (cell->behavior == CELL_BEHAVIOR_WALL)
    return COLLISION_IMPASSABLE;
  heightBytes = Ram_MapGridShapes + cell->shape * 4;
  // UB: read the kind byte before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & MAPGRID_SHAPE_KIND_MASK;

    targetGround = gMapGrid_TerrainHeightFunctions[kind](
        heightBytes, pixelX & (MAPGRID_CELL_PIXELS - 1),
        pixelZ & (MAPGRID_CELL_PIXELS - 1));
  }
#else
  targetGround =
      gMapGrid_TerrainHeightFunctions[*heightBytes++ & MAPGRID_SHAPE_KIND_MASK](
          heightBytes, pixelX & (MAPGRID_CELL_PIXELS - 1),
          pixelZ & (MAPGRID_CELL_PIXELS - 1));
#endif
  // Up to 8 pixels up or 12 down is a step the actor can take.
  stepHeight = targetGround - actor->groundHeight;
  if (stepHeight > toFIXED(8))
    return COLLISION_TOO_HIGH;
  if (stepHeight < -toFIXED(12))
    return COLLISION_TOO_LOW;
  return COLLISION_NONE;
}

/// Returns -1 when the cell under position (its x, and y less its height) in
/// MAPGRID_LAYER_OVERHEAD is a wall (CELL_BEHAVIOR_WALL), else 0; also 0 before
/// the map state exists.
s32 MapGrid_IsWallAt(struct WorldPosition *position) {
  s32 pixelX;
  s32 pixelY;
  s32 cellX;
  s32 cellY;
  struct MapGridScrollState *gridState;
  struct MapGridCell *cell;

  pixelX = position->x / FIXED_ONE;
  // The point on the ground below the position.
  pixelY = (position->y - position->height) / FIXED_ONE;
  gridState = gMapGridState[0];
  if (gridState == NULL)
    return 0;
  // Always the overhead layer, the one on BG1.
  cell = gridState->layers[MAPGRID_LAYER_OVERHEAD].cells;
  cellX = pixelX / MAPGRID_CELL_PIXELS;
  cellY = pixelY / MAPGRID_CELL_PIXELS;
  cell += cellX + cellY * MAPGRID_SIZE;
  // 1 - 1 for open ground, 0 - 1 for a wall: nonzero means walled.
  return (cell->behavior != CELL_BEHAVIOR_WALL) - 1;
}

/// The world map's collision: each tile of a terrain layer's map has a tile of
/// 4-bit collision values, 8 bytes, two values a byte. The lower layer's map
/// is in screen block 8, the upper's in screen block 10, and the upper's
/// collision tiles follow the lower's 256 in the buffer.
#define WORLD_MAP_LOWER_COLLISION_MAP BG_SCREEN_BLOCK(8)
#define WORLD_MAP_UPPER_COLLISION_MAP BG_SCREEN_BLOCK(10)
#define WORLD_MAP_COLLISION_TILE_BYTES 8
#define WORLD_MAP_UPPER_COLLISION_TILES (256 * WORLD_MAP_COLLISION_TILE_BYTES)
#define WORLD_MAP_COLLISION_BITS 4
#define WORLD_MAP_COLLISION_MASK ((1 << WORLD_MAP_COLLISION_BITS) - 1)

/// A block word's bit 31, in its top byte: the second bank of battle
/// environments.
#define WORLD_MAP_BLOCK_SECOND_BANK (1 << (31 - 24))
/// A block word's encounter region, its bits 24 to 30.
#define WORLD_MAP_BLOCK_REGION(word) (((word) << 1) >> 25)
/// The battle environments of gWorldMap_BattleEnvironments come in banks of
/// one for each collision value.
#define WORLD_MAP_ENVIRONMENT_BANK 16

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

  // Coordinates in 2-pixel units.
  collisionCell = (((halfPixelY / 8) & 63) << 6) + ((halfPixelX / 8) & 63);
  pixelOffset = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

  // The upper layer first.
  collisionTile = WORLD_MAP_UPPER_COLLISION_MAP[collisionCell];
  packedCollisions =
      *(Ram_MapGridShapes + WORLD_MAP_UPPER_COLLISION_TILES +
        collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> WORLD_MAP_COLLISION_BITS;
    else
      result = packedCollisions & WORLD_MAP_COLLISION_MASK;
    if (result != 0)
      return result;
  }

  // Then the lower layer.
  collisionTile = WORLD_MAP_LOWER_COLLISION_MAP[collisionCell];
  packedCollisions =
      *(Ram_MapGridShapes + collisionTile * WORLD_MAP_COLLISION_TILE_BYTES +
        pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> WORLD_MAP_COLLISION_BITS;
    else
      result = packedCollisions & WORLD_MAP_COLLISION_MASK;
    if (result != 0)
      return result;
  }

  // Neither layer has a value: the default, inside the walkable range.
  return WORLD_MAP_COLLISION_DEFAULT;
}

/// Returns 0 when the world map at position can be walked on (collision
/// WORLD_MAP_COLLISION_WALKABLE_FIRST to WORLD_MAP_COLLISION_WALKABLE_LAST),
/// else -1.
s32 WorldMap_IsWalkable(s32 unused, struct WorldPosition *position) {
  // One unsigned compare tests the walkable range.
  if ((WorldMap_GetCollision(position) - WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
      (u32)(WORLD_MAP_COLLISION_WALKABLE_LAST -
            WORLD_MAP_COLLISION_WALKABLE_FIRST)) {
    return 0;
  }
  return -1;
}

/// Returns the world map's battle environment at position from
/// gWorldMap_BattleEnvironments, and stores the block's encounter region in
/// *region.
s32 WorldMap_GetBattleEnvironment(struct WorldPosition *position, s32 *region) {
  s32 collisionKind = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 environmentBank = 0;
  u32 blockX;
  u32 index;
  u32 *blockWord;

  // The bias rounds a negative coordinate towards zero, as division would.
  if (x < 0)
    x += toFIXED(WORLD_MAP_BLOCK_PIXELS) - 1;
  blockX = (x >> WORLD_MAP_BLOCK_SHIFT) & (WORLD_MAP_BLOCKS_ACROSS - 1);
  y = position->y;
  if (y < 0)
    y += toFIXED(WORLD_MAP_BLOCK_PIXELS) - 1;

  index =
      blockX + ((y >> WORLD_MAP_BLOCK_SHIFT) & (WORLD_MAP_BLOCKS_ACROSS - 1)) *
                   WORLD_MAP_BLOCKS_ACROSS;
  blockWord = gCellTiles + index;
  // Bit 31 of the block word selects the second bank; the snowfield the
  // third, whatever bit 31 says.
  if (((u8 *)blockWord)[3] & WORLD_MAP_BLOCK_SECOND_BANK)
    environmentBank = WORLD_MAP_ENVIRONMENT_BANK;

  *region = WORLD_MAP_BLOCK_REGION(*blockWord);
  if (*region == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
    environmentBank = 2 * WORLD_MAP_ENVIRONMENT_BANK;

  return gWorldMap_BattleEnvironments[environmentBank + collisionKind];
}

#undef WORLD_MAP_LOWER_COLLISION_MAP
#undef WORLD_MAP_UPPER_COLLISION_MAP
#undef WORLD_MAP_COLLISION_TILE_BYTES
#undef WORLD_MAP_UPPER_COLLISION_TILES
#undef WORLD_MAP_COLLISION_BITS
#undef WORLD_MAP_COLLISION_MASK
#undef WORLD_MAP_BLOCK_SECOND_BANK
#undef WORLD_MAP_BLOCK_REGION
#undef WORLD_MAP_ENVIRONMENT_BANK
