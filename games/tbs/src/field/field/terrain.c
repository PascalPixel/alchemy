/// @file
/// Terrain: ground heights and the collision of cells and the world map.
/// Part of field.c, the field object, compiled only through it.

#include "../field_internal.h"

// A sample is in eight-pixel units. UB: signed samples may be negative, so
// their left shift is undefined. Multiplication fits s32 for both s8 and u8.
#ifdef UBFIX
#define CURVE_HEIGHT_FIXED(sample) ((sample) * (1 << CURVE_VALUE_SHIFT))
#else
#define CURVE_HEIGHT_FIXED(sample) ((sample) << CURVE_VALUE_SHIFT)
#endif

/// The terrain height functions (gMap_TerrainHeightFunctions): each takes a
/// cell's height samples after its kind byte and a pixel (x, y) within the
/// 16x16 cell, and returns the ground height there in 16.16 pixels. This one
/// is flat: the first sample everywhere.
/// Each is reached only through that table, by the low four bits of the
/// cell's kind byte: this one is kind 0, and the rest follow in order.
s32 Curve_GetFirstSampleA(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// A slope along x: from sample 0 at x = 0 towards sample 1 at x = 16.
/// Kind 1.
s32 Curve_LerpTwoSamples(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelX;
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

/// A slope along y: from sample 0 at y = 0 towards sample 1 at y = 16.
/// Kind 2.
s32 Curve_LerpTwoSamplesB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelY;
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

/// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
/// it and the higher of the two on it.
/// Kind 3.
s32 Curve_StepAtSummedPosition(const u8 *heightBytes, s32 x, s32 y) {
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
/// Kind 4.
s32 Curve_StepAtDifferencePosition(const u8 *heightBytes, s32 x, s32 y) {
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
/// Kind 5.
s32 Curve_LerpThreeSamplesByRatio(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 height;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples);
  lastHeight = CURVE_HEIGHT_FIXED(samples[1]);
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
/// Kind 6. It divides by 16 with a shift where kind 5 divides by 15, so its
/// slopes are 15/16 as steep and it never reaches sample 2 exactly.
s32 Curve_LerpThreeSamplesAtDifference(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 pixelDifference;
  s32 heightDelta;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(samples[0]);
  lastHeight = samples[1];
  pixelDifference = y - x;
  x = pixelDifference + CURVE_FULL_STEPS - 1;
  lastHeight = CURVE_HEIGHT_FIXED(lastHeight);

  if (x == CURVE_FULL_STEPS - 1)
    return middleHeight;

  if ((u32)x < CURVE_FULL_STEPS - 1) {
    heightDelta = middleHeight - firstHeight;
    pixelDifference = heightDelta * x;
    if (pixelDifference < 0)
      pixelDifference += CURVE_FULL_STEPS - 1;
    pixelDifference >>= 4;
    return firstHeight + pixelDifference;
  }
  pixelDifference *= lastHeight - middleHeight;
  if (pixelDifference < 0)
    pixelDifference += CURVE_FULL_STEPS - 1;
  pixelDifference >>= 4;
  return middleHeight + pixelDifference;
}

/// A raised or sunken rounded square: sample 1 where gCurve_LerpWeightTable is
/// 1, sample 0 in the corners.
/// Kind 7.
s32 Curve_LerpTwoSamplesByTable(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  return startHeight + ((CURVE_HEIGHT_FIXED(samples[1]) - startHeight) *
                        gCurve_LerpWeightTable[x + (y * CURVE_FULL_STEPS)]);
}

/// A step along x: sample 0 for x < 8, sample 1 from x = 8.
/// Kind 8.
s32 Curve_StepAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
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
/// Kind 9.
s32 Curve_StepAtHalfB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
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
/// Kind 10.
s32 Curve_LerpThreeSamplesAtHalf(const u8 *heightBytes, s32 pixelX,
                                 s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  s32 endpointHeight;
  s32 middleHeight;
  s32 heightDelta;

  endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples++);
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

/// A slope along y that bends at the middle, as kind 10 does along x.
/// Kind 11. Its samples are read unsigned: 128 to 255 are heights, where the
/// other kinds read them as -128 to -1.
s32 Curve_LerpThreeSamplesAtHalfB(const u8 *heightBytes, s32 pixelX,
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

/// The sample gCurve_SampleIndexTable picks for (x, y).
/// Kind 12.
s32 Curve_LookupSampleByTable(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  return CURVE_HEIGHT_FIXED(
      samples[gCurve_SampleIndexTable[cellX + (cellY << 4)]]);
}

/// The same, with the table mirrored left to right.
/// Kind 13.
s32 Curve_LookupSampleByTableReversed(const u8 *heightBytes, s32 pixelX,
                                      s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  return CURVE_HEIGHT_FIXED(
      samples[gCurve_SampleIndexTable[((cellY << 4) - cellX) +
                                      CURVE_FULL_STEPS - 1]]);
}

/// Flat, as Curve_GetFirstSampleA. Kind 14.
s32 Curve_GetFirstSampleB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

/// Flat, as Curve_GetFirstSampleA. Kind 15.
s32 Curve_GetFirstSampleC(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

#undef CURVE_HEIGHT_FIXED

/// Returns the ground height of a layer at (x, y) in 16.16 pixels: the
/// height function of the cell's terrain kind applied to its samples.
/// y is the map's down axis, an object's z.
///
/// Called each frame by Object_UpdateAllThumb for objects that follow the
/// terrain, by the player control, and as Map_GetTerrainHeightFar by the
/// field scene's objects, the Psynergy effects and about ten scene overlays.
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y) {
  struct MapState *work = gMapWork[0];
  struct MapCell *cell;
  u8 *heightBytes;

  x >>= 16;
  y >>= 16;
  cell = (struct MapCell *)Ram_MapCellBuffer;
  if (work != NULL) {
    cell = work->layers[layer & 3].cells;
  }
  // Cells are 16 pixels, 128 to a row. A cell's collision record is four
  // bytes: the kind, then up to three height samples.
  cell += x / 16 + (y / 16 << 7);
  heightBytes = Ram_MapCollision + cell->attributeB * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    return gMap_TerrainHeightFunctions[kind](heightBytes, x & 15, y & 15);
  }
#else
  return gMap_TerrainHeightFunctions[*heightBytes++ & 15](heightBytes, x & 15,
                                                          y & 15);
#endif
}

/// Returns the terrain kind (the low four bits of the first collision byte)
/// of a layer's cell at (x, y) in 16.16 pixels.
/// Called through Map_GetCellAttributeLowNibbleFar by the Psynergy Move
/// effect.
s32 Map_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y) {
  struct MapState *state = gMapWork[0];
  struct MapCell *cells;
  s32 cellX;
  s32 cellY;
  u32 heightAttributeIndex;

  x >>= 16;
  y >>= 16;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != 0) {
    cells = state->layers[layer & 3].cells;
  }
  cellX = x / 16;
  cellY = y / 16;
  cells += cellX + (cellY << 7);
  heightAttributeIndex = cells->attributeB;
  return *(Ram_MapCollision + heightAttributeIndex * 4) & 15;
}

/// Returns the collision code of a layer's cell at (x, y) in 16.16 pixels.
/// The shift by 20 takes 16.16 pixels straight to 16-pixel cells. Called
/// through GetMapCellCollisionFar by the Psynergy climbing and by scenes
/// (Altin Peak, Colosso, Imil, Crossbone Isle).
s32 GetMapCellCollision(s32 layer, s32 x, s32 y) {
  struct MapState *state;
  struct MapCell *cell;

  state = gMapWork[0];
  x >>= 20;
  y >>= 20;
  cell = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cell = state->layers[layer & 3].cells;
  }
  cell += x + (y << 7);
  return cell->collisionCode;
}

/// Sets the collision code of a layer's cell at (x, y) in 16.16 pixels.
/// Writes the cell in the map's grid; does nothing before the map is set up.
/// Called through SetMapCellCollisionFar by scenes
/// (Colosso, Altin Peak, Crossbone Isle, Venus Lighthouse, Tolbi, Sol
/// Sanctum).
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapState *state = gMapWork[0];

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cell = state->layers[layer & 3].cells;

    cell += x + (y << 7);
    cell->collisionCode = collisionCode;
  }
}

/// Returns the top two bits of the block word of the cell at pixel (x, y).
UNUSED s32 Map_GetCellHighFlags(s32 x, s32 y) {
  s32 cellX = x / 16;
  s32 cellY = y / 16;
  struct MapCell *mapCell =
      (struct MapCell *)gMapCellBuffer + (cellX + cellY * 128);

  return ((u8 *)&mapCell->metatileAndFlags)[1] >> 6;
}

/// Returns whether an object can step to position: 2 when the cell there is
/// a wall (collision code 0xff), 1 when the ground is more than 8 pixels
/// higher than the object's, -1 when it is more than 12 pixels lower, else 0.
///
/// Called by the player control and the script commands that pick a free
/// step, and as Map_CheckObjectStepFar by the Psynergy effects (Move, Carry,
/// Force, climbing). A terrainId above 2 reads the grid from its start.
s32 Map_CheckObjectStep(struct ObjectRuntime *object,
                        struct FieldPosition *position) {
  struct MapState *mapState;
  s32 pixelX;
  s32 pixelZ;
  struct MapCell *cell;
  u8 *heightBytes;
  s32 targetGround;
  s32 stepHeight;

  pixelX = ((union FieldCoordinate *)&position->x)->part.pixel;
  pixelZ = ((union FieldCoordinate *)&position->z)->part.pixel;
  mapState = gMapWork[0];
  if (mapState == NULL)
    return 0;
  if (object->terrainId <= 2)
    cell = mapState->layers[object->terrainId].cells;
  else
    cell = (struct MapCell *)Ram_MapCellBuffer;
  cell += pixelX / 16 + (pixelZ / 16 << 7);
  if (cell->collisionCode == 0xff)
    return 2;
  heightBytes = Ram_MapCollision + cell->attributeB * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    targetGround = gMap_TerrainHeightFunctions[kind](heightBytes, pixelX & 15,
                                                     pixelZ & 15);
  }
#else
  targetGround = gMap_TerrainHeightFunctions[*heightBytes++ & 15](
      heightBytes, pixelX & 15, pixelZ & 15);
#endif
  stepHeight = targetGround - object->terrainHeight;
  if (stepHeight > 0x80000)
    return 1;
  if (stepHeight < -0xc0000)
    return -1;
  return 0;
}

/// Returns 0 when the cell under position (its x, and y less its height) in
/// layer 2 is a wall (collision code 0xff), else -1.
/// The name says the opposite of what it returns: 0 is the blocked answer.
/// Called by the player control and, as CheckMapPositionCellOccupiedFar, by
/// the Psynergy climbing.
s32 CheckMapPositionCellOccupied(struct WorldPosition *position) {
  s32 pixelX;
  s32 pixelY;
  s32 cellX;
  s32 cellY;
  struct MapState *mapState;
  struct MapCell *cell;

  pixelX = position->x / 65536;
  pixelY = (position->y - position->height) / 65536;
  mapState = gMapWork[0];
  if (mapState == NULL)
    return 0;
  cell = mapState->layers[2].cells;
  cellX = pixelX / 16;
  cellY = pixelY / 16;
  cell += cellX + cellY * 128;
  return (cell->collisionCode != 0xff) - 1;
}

/// Returns the world map's collision value (1-15) at position: from the upper
/// collision layer where it has one, else the lower, else 7. Each layer is a
/// tile map of 4-bit collision pixels.
///
/// Called by the player control, CheckWorldMapWalkable and
/// GetWorldMapTerrainBehavior.
s32 GetWorldMapCollision(struct WorldPosition *position) {
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

  return 7;
}

/// Returns 0 when the world map at position can be walked on (collision 5 to
/// 12), else -1.
/// The first argument is not read; the player control passes its object.
/// Called by the player control as it probes the steps around the object it
/// moves.
s32 CheckWorldMapWalkable(s32 unused, struct WorldPosition *position) {
  if ((GetWorldMapCollision(position) - 5) <= 7U) {
    return 0;
  }
  return -1;
}

/// Returns the world map's terrain behaviour at position from
/// gWorldMap_TerrainBehaviorTable, and stores the block's terrain kind in
/// terrainKind.
/// Called through GetWorldMapTerrainBehaviorFar by the world map's
/// encounters (Encounter_ResolveWorldMapZoneAndBackdrop).
s32 GetWorldMapTerrainBehavior(struct WorldPosition *position,
                               s32 *terrainKind) {
  s32 collisionKind = GetWorldMapCollision(position);
  s32 x = position->x;
  s32 y;
  s32 behaviorBank = 0;
  u32 blockX;
  u32 index;
  u32 *blockWord;

  // Blocks are 32 pixels, 32 to a row; the bias rounds a negative coordinate
  // towards zero, as division would.
  if (x < 0)
    x += 0x1fffff;
  blockX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = blockX + (((y >> 21) & 31) << 5);
  blockWord = gMapBlocks + index;
  if (((u8 *)blockWord)[3] & 0x80)
    behaviorBank = 0x10;

  // The terrain kind is bits 24 to 30 of the block word.
  *terrainKind = (*blockWord << 1) >> 25;
  if (*terrainKind == 21)
    behaviorBank = 0x20;

  return gWorldMap_TerrainBehaviorTable[behaviorBank + collisionKind];
}
