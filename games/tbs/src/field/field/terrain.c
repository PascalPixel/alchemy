/// @file
/// Ground heights, and the collision of cells and the world map.
// Part of field.c.

#include "../field_internal.h"

// A sample is in eight-pixel units. UB: signed samples may be negative, so
// their left shift is undefined. Multiplication fits s32 for both s8 and u8.
#ifdef UBFIX
#define CURVE_HEIGHT_FIXED(sample) ((sample) * (1 << CURVE_VALUE_SHIFT))
#else
#define CURVE_HEIGHT_FIXED(sample) ((sample) << CURVE_VALUE_SHIFT)
#endif

/// The ground height functions (gMapGrid_TerrainHeightFunctions): each takes a
/// cell's height samples after its kind byte and a pixel (x, y) within the
/// 16x16 cell, and returns the ground height there in 16.16 pixels. This one
/// is flat: the first sample everywhere.
/// Each is reached only through that table, by the cell's enum TerrainKind:
/// this one is TERRAIN_FLAT, and the rest follow in order.
s32 Terrain_GetFirstSampleA(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// A slope along x: from sample 0 at x = 0 towards sample 1 at x = 16.
s32 Terrain_LerpTwoSamples(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelX;
  // Round towards zero, as dividing by 16 would.
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

/// A slope along y: from sample 0 at y = 0 towards sample 1 at y = 16.
s32 Terrain_LerpTwoSamplesB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelY;
  // Round towards zero, as dividing by 16 would.
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

/// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
/// it and the higher of the two on it.
s32 Terrain_StepAtSummedPosition(const u8 *heightBytes, s32 x, s32 y) {
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
s32 Terrain_StepAtDifferencePosition(const u8 *heightBytes, s32 x, s32 y) {
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
s32 Terrain_LerpThreeSamplesByRatio(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 height;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples);
  lastHeight = CURVE_HEIGHT_FIXED(samples[1]);
  // x + y from 0 to 30: 15 is the diagonal, below it the first half and
  // above it the second. Each half divides by 15, so it reaches its end
  // sample.
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
/// It divides by 16 with a shift where TERRAIN_BEND_SUM_DIAGONAL divides by
/// 15, so its
/// slopes are 15/16 as steep and it never reaches sample 2 exactly.
s32 Terrain_LerpThreeSamplesAtDifference(const u8 *heightBytes, s32 x, s32 y) {
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
s32 Terrain_LerpTwoSamplesByTable(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  // The weight is 0 or 1, so the result is one sample or the other, never
  // between.
  return startHeight + ((CURVE_HEIGHT_FIXED(samples[1]) - startHeight) *
                        gTerrain_LerpWeightTable[x + (y * CURVE_FULL_STEPS)]);
}

/// A step along x: sample 0 for x < 8, sample 1 from x = 8.
s32 Terrain_StepAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  s32 heightSample;
  if (cellX < CURVE_HALF_STEPS)
    heightSample = samples[0];
  else
    heightSample = samples[1];
  return CURVE_HEIGHT_FIXED(heightSample);
}

/// A step along y: sample 0 for y < 8, sample 1 from y = 8.
s32 Terrain_StepAtHalfB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellY = pixelY;
  s32 heightSample;
  if (cellY < CURVE_HALF_STEPS)
    heightSample = samples[0];
  else
    heightSample = samples[1];
  return CURVE_HEIGHT_FIXED(heightSample);
}

/// A slope along x that bends at the middle: sample 0 at x = 0, sample 1 at
/// x = 8, towards sample 2 at x = 16.
s32 Terrain_LerpThreeSamplesAtHalf(const u8 *heightBytes, s32 pixelX,
                                   s32 pixelY) {
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
/// Its samples are read unsigned: 128 to 255 are heights, where the
/// other kinds read them as -128 to -1.
s32 Terrain_LerpThreeSamplesAtHalfB(const u8 *heightBytes, s32 pixelX,
                                    s32 pixelY) {
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
s32 Terrain_LookupSampleByTable(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  // The table holds a byte per pixel of the cell, row by row.
  return CURVE_HEIGHT_FIXED(
      samples[gTerrain_SampleIndexTable[cellX + (cellY << 4)]]);
}

/// The same, with the table mirrored left to right.
s32 Terrain_LookupSampleByTableReversed(const u8 *heightBytes, s32 pixelX,
                                        s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  // Row y read from its right end: (y << 4) + 15 - x.
  return CURVE_HEIGHT_FIXED(
      samples[gTerrain_SampleIndexTable[((cellY << 4) - cellX) +
                                        CURVE_FULL_STEPS - 1]]);
}

/// Flat, as Terrain_GetFirstSampleA.
s32 Terrain_GetFirstSampleB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// Flat, as Terrain_GetFirstSampleA.
s32 Terrain_GetFirstSampleC(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

#undef CURVE_HEIGHT_FIXED

/// Returns the ground height of a layer at (x, y) in 16.16 pixels: the
/// height function of the cell's terrain kind applied to its samples.
/// y is the map's down axis, an object's z.
///
/// Called each frame by Object_UpdateAllThumb for objects that follow the
/// ground, by the player control, and as MapGrid_GetTerrainHeightFar by the
/// field map's objects, the Psynergy effects and about ten map overlays.
s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 y) {
  struct MapGridScrollAnimView *work = gMapGridWork[0];
  struct MapGridCell *cell;
  u8 *heightBytes;

  x >>= 16;
  y >>= 16;
  // Before the map work exists, the grid is read from its start.
  cell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  if (work != NULL) {
    cell = work->layers[layer & 3].cells;
  }
  // Cells are 16 pixels, 128 to a row. A cell's shape is four
  // bytes: the kind, then up to three height samples.
  cell += x / 16 + (y / 16 << 7);
  heightBytes = Ram_MapGridShapes + cell->shape * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    return gMapGrid_TerrainHeightFunctions[kind](heightBytes, x & 15, y & 15);
  }
#else
  return gMapGrid_TerrainHeightFunctions[*heightBytes++ & 15](heightBytes,
                                                              x & 15, y & 15);
#endif
}

/// Returns the terrain kind of a layer's cell at (x, y) in 16.16 pixels: the
/// low four bits of the first byte of the cell's shape.
/// Called through MapGrid_GetCellAttributeLowNibbleFar by the Psynergy Move
/// effect.
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y) {
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];
  struct MapGridCell *cells;
  s32 cellX;
  s32 cellY;
  u32 heightAttributeIndex;

  x >>= 16;
  y >>= 16;
  cells = (struct MapGridCell *)gMapGridCellBuffer;
  if (gridState != 0) {
    cells = gridState->layers[layer & 3].cells;
  }
  cellX = x / 16;
  cellY = y / 16;
  cells += cellX + (cellY << 7);
  // The kind byte of the cell's four-byte shape.
  heightAttributeIndex = cells->shape;
  return *(Ram_MapGridShapes + heightAttributeIndex * 4) & 15;
}

/// Returns the metatile behavior of a layer's cell at (x, y) in 16.16 pixels.
/// The shift by 20 takes 16.16 pixels straight to 16-pixel cells. Called
/// through MapGrid_GetMetatileBehaviorAtFar by the Psynergy climbing and by
/// maps (Altin Peak, Colosso, Imil, Crossbone Isle).
s32 MapGrid_GetMetatileBehaviorAt(s32 layer, s32 x, s32 y) {
  struct MapGridScrollAnimView *gridState;
  struct MapGridCell *cell;

  gridState = gMapGridWork[0];
  x >>= 20;
  y >>= 20;
  // Before the map work exists, the grid is read from its start.
  cell = (struct MapGridCell *)gMapGridCellBuffer;
  if (gridState != NULL) {
    cell = gridState->layers[layer & 3].cells;
  }
  cell += x + (y << 7);
  return cell->behavior;
}

/// Sets the metatile behavior of a layer's cell at (x, y) in 16.16 pixels.
/// Writes the cell in the map's grid; does nothing before the map is set up.
/// Called through MapGrid_SetMetatileBehaviorAtFar by maps
/// (Colosso, Altin Peak, Crossbone Isle, Venus Lighthouse, Tolbi, Sol
/// Sanctum).
void MapGrid_SetMetatileBehaviorAt(u32 layer, s32 x, s32 y, u32 behavior) {
  struct MapGridScrollAnimView *gridState = gMapGridWork[0];

  x >>= 20;
  y >>= 20;
  // Without the map work there is no layer to write to.
  if (gridState != NULL) {
    struct MapGridCell *cell = gridState->layers[layer & 3].cells;

    cell += x + (y << 7);
    cell->behavior = behavior;
  }
}

/// Unused: returns the top two bits of the metatile halfword of the cell at
/// pixel (x, y). Nothing calls it.
UNUSED s32 MapGrid_GetCellHighFlags(s32 x, s32 y) {
  s32 cellX = x / 16;
  s32 cellY = y / 16;
  struct MapGridCell *cell =
      (struct MapGridCell *)gMapGridCellBuffer + (cellX + cellY * 128);

  // The high byte of the metatile halfword; its top two bits are flags.
  return ((u8 *)&cell->metatileAndFlags)[1] >> 6;
}

/// Returns whether an object can step to position, an enum Collision:
/// whether the cell there is a wall, or the ground too high or too low for a
/// step from the object's.
///
/// Called by the player control and the script commands that pick a free
/// step, and as MapGrid_CheckCollisionFar by the Psynergy effects (Move,
/// Carry, Force, climbing). A layer past MAPGRID_LAYER_OVERHEAD reads the
/// grid from its start.
s32 MapGrid_CheckCollision(struct Object *object,
                           struct FieldPosition *position) {
  struct MapGridScrollAnimView *gridState;
  s32 pixelX;
  s32 pixelZ;
  struct MapGridCell *cell;
  u8 *heightBytes;
  s32 targetGround;
  s32 stepHeight;

  pixelX = ((union FieldCoordinate *)&position->x)->part.pixel;
  pixelZ = ((union FieldCoordinate *)&position->z)->part.pixel;
  gridState = gMapGridWork[0];
  if (gridState == NULL)
    return COLLISION_NONE;
  // The three layers have their windows into the grid; any other layer
  // reads the grid from its start.
  if (object->layer <= MAPGRID_LAYER_OVERHEAD)
    cell = gridState->layers[object->layer].cells;
  else
    cell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  cell += pixelX / 16 + (pixelZ / 16 << 7);
  // A wall blocks whatever the heights.
  if (cell->behavior == MB_WALL)
    return COLLISION_IMPASSABLE;
  heightBytes = Ram_MapGridShapes + cell->shape * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    targetGround = gMapGrid_TerrainHeightFunctions[kind](
        heightBytes, pixelX & 15, pixelZ & 15);
  }
#else
  targetGround = gMapGrid_TerrainHeightFunctions[*heightBytes++ & 15](
      heightBytes, pixelX & 15, pixelZ & 15);
#endif
  // Up to 8 pixels up or 12 down is a step the object can take.
  stepHeight = targetGround - object->groundHeight;
  if (stepHeight > 0x80000)
    return COLLISION_TOO_HIGH;
  if (stepHeight < -0xc0000)
    return COLLISION_TOO_LOW;
  return COLLISION_NONE;
}

/// Returns -1 when the cell under position (its x, and y less its height) in
/// MAPGRID_LAYER_OVERHEAD is a wall (MB_WALL), else 0; also 0 before the map
/// work exists. Called by the player control and, as
/// MapGrid_IsWallAtFar, by the Psynergy climbing.
s32 MapGrid_IsWallAt(struct WorldPosition *position) {
  s32 pixelX;
  s32 pixelY;
  s32 cellX;
  s32 cellY;
  struct MapGridScrollAnimView *gridState;
  struct MapGridCell *cell;

  pixelX = position->x / 65536;
  // The point on the ground below the position.
  pixelY = (position->y - position->height) / 65536;
  gridState = gMapGridWork[0];
  if (gridState == NULL)
    return 0;
  // Always the overhead layer, the one on BG1.
  cell = gridState->layers[MAPGRID_LAYER_OVERHEAD].cells;
  cellX = pixelX / 16;
  cellY = pixelY / 16;
  cell += cellX + cellY * 128;
  // 1 - 1 for open ground, 0 - 1 for a wall: nonzero means walled.
  return (cell->behavior != MB_WALL) - 1;
}

/// Returns the world map's collision value (1-15) at position: from the upper
/// collision layer where it has one, else the lower, else
/// WORLD_MAP_COLLISION_DEFAULT. Each layer is a
/// tile map of 4-bit collision pixels.
///
/// Called by the player control, WorldMap_IsWalkable and
/// WorldMap_GetBattleEnvironment.
s32 WorldMap_GetCollision(struct WorldPosition *position) {
  s32 halfPixelX = position->x >> 17;
  s32 y = position->y;
  s32 halfPixelY = y >> 17;
  u32 collisionCell;
  u32 pixelOffset;
  u8 collisionTile;
  u8 packedCollisions;
  u32 result;

  // Coordinates in 2-pixel units. A collision tile covers 16x16 pixels as
  // 4x4 values of 4 bits, two to a byte, 8 bytes a tile; the maps of tile
  // numbers are 64 tiles square and wrap.
  collisionCell = (((halfPixelY / 8) & 63) << 6) + ((halfPixelX / 8) & 63);
  pixelOffset = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

  // The upper layer: its tile map in screen block 10, its tiles 0x800 bytes
  // into the collision buffer.
  collisionTile = BG_SCREEN_BLOCK(10)[collisionCell];
  packedCollisions =
      *(Ram_MapGridShapes + 0x800 + (collisionTile << 3) + pixelOffset);
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
  packedCollisions = *(Ram_MapGridShapes + (collisionTile << 3) + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 15;
    if (result != 0)
      return result;
  }

  // Neither layer has a value: the default, inside the walkable range.
  return WORLD_MAP_COLLISION_DEFAULT;
}

/// Returns 0 when the world map at position can be walked on (collision
/// WORLD_MAP_COLLISION_WALKABLE_FIRST to WORLD_MAP_COLLISION_WALKABLE_LAST),
/// else -1.
/// The first argument is not read; the player control passes its object.
/// Called by the player control as it probes the steps around the object it
/// moves.
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
/// terrainKind.
/// Called through WorldMap_GetBattleEnvironmentFar by the world map's
/// encounters (Encounter_ResolveWorldMapZoneAndBackdrop).
s32 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                  s32 *terrainKind) {
  s32 collisionKind = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 behaviorBank = 0;
  u32 metatileX;
  u32 index;
  u32 *metatileWord;

  // Blocks are 32 pixels, 32 to a row; the bias rounds a negative coordinate
  // towards zero, as division would.
  if (x < 0)
    x += 0x1fffff;
  metatileX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = metatileX + (((y >> 21) & 31) << 5);
  metatileWord = gMetatiles + index;
  // Bit 31 of the block word selects the second bank of 16; the snowfield
  // the third, whatever bit 31 says.
  if (((u8 *)metatileWord)[3] & 0x80)
    behaviorBank = 0x10;

  // The encounter region is bits 24 to 30 of the block word.
  *terrainKind = (*metatileWord << 1) >> 25;
  if (*terrainKind == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
    behaviorBank = 0x20;

  return gWorldMap_BattleEnvironments[behaviorBank + collisionKind];
}
