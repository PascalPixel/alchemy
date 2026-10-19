/// @file
/// Layered maps: loading, drawing and scrolling, and copying cells.
/// Part of field.c, the field object, compiled only through it.

#include "../field_internal.h"

/// Remaps byteCount bytes in place through gRuntime_ByteRemapTable.
void Runtime_RemapBytesByTable(u8 *bytes, s32 byteCount) {
  s32 remaining;
  u8 *cursor;
  u8 *remapTable;

  cursor = bytes;
  remapTable = gRuntime_ByteRemapTable;
  remaining = byteCount - 1;
  if (remaining != -1) {
    do {
      remaining -= 1;
      *cursor = remapTable[*cursor];
      cursor += 1;
    } while (remaining != -1);
  }
}

/// Unpacks the staged block data into gMapBlocks. Byte 1 of the staged buffer
/// (Ram_MapCellBuffer) is the method, and the
/// 16-bit data follows from byte 2. size counts the method byte too (an even
/// size means no data). Methods:
///   0: copy as it is
///   1: all the high bytes, then all the low bytes; each value is XORed with
///      the one before
///   2: each value is XORed with the one before
void Tilemap_DecodeStagedBuffer(s32 size) {
  u16 *destinationWords;
  u16 *sourceWords;
  s32 wordCount;
  s32 i;

  sourceWords = (u16 *)(Ram_MapCellBuffer + 2);
  destinationWords = (u16 *)gMapBlocks;
  wordCount = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < wordCount; i++)
        *destinationWords++ = *sourceWords++;
      break;
    case 1: {
      u32 previousWord = 0;
      u8 *highBytes = Ram_MapCellBuffer + 2;
      u8 *lowBytes = highBytes + wordCount;

      for (i = 0; i < wordCount; i++) {
        u32 decodedWord = ((*highBytes++ << 8) | *lowBytes++) ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
      }
      break;
    }
    case 2: {
      s32 previousWord = 0;

      for (i = 0; i < wordCount; i++) {
        s32 decodedWord = *sourceWords++ ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
      }
      break;
    }
    }
  }
}

typedef void (*TilemapConvertFn)(void *dst, const void *src, const void *saved);

/// Gives each map cell whose block number is 0xfff (a placeholder) a number
/// of its own, counting up from 0 in map order.
UNUSED void Tilemap_NumberPlaceholderBlocks(void) {
  u32 *cell = (u32 *)gMapCellBuffer;
  u32 remaining = 128 << 7;
  u32 mask = 0xfff;
  s32 nextPlaceholderBlock = -1;

  do {
    u32 value = *cell++;
    u32 blockId = value & mask;

    if (blockId == mask) {
      if (nextPlaceholderBlock != blockId) {
        nextPlaceholderBlock++;
      }
      value = value + nextPlaceholderBlock - blockId;
      cell[-1] = value;
    }
    remaining--;
  } while (remaining != 0);
}

/// Converts the map's cells in place with Tile_ConvertMap, run from RAM, from
/// a saved copy of the staged buffer.
void Tilemap_ConvertBuffer(void) {
  u8 *savedCells;
  TilemapConvertFn convertMap;
  u32 savedBytes;

  savedBytes = 0x8000;
  savedCells = Runtime_BumpAllocateAlternatePool(savedBytes);
  Iwram_CopyWords(savedCells, Ram_MapCellBuffer, savedBytes);
  RamCode_Load(convertMap, Tile_ConvertMap, Tile_ConvertMapCodeSize);
  convertMap(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, savedCells);
  MemFree(convertMap);
  MemFree(savedCells);
}

/// Copies one tile column of a block (metatile) into a BG map: its top and
/// bottom tile entries, choosing the left or right half with tileHalf.
static __inline__ void Map_CopyMetatileHalfColumn(u32 *cell, u8 *screenMap,
                                                  u32 screenRowOffset,
                                                  u32 screenColumnOffset,
                                                  u32 tileHalf) {
  u16 *metatileEntry;
  u8 *screenTile;
  u32 index = ((*cell << 20) >> 18) + tileHalf;

  metatileEntry = (u16 *)Ram_MapBlocks;
  metatileEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *metatileEntry;
  metatileEntry = (u16 *)(Ram_MapBlocks + 4);
  metatileEntry += index;
  *(u16 *)(screenTile + 64) = *metatileEntry;
}

/// Copies a whole block (metatile) into a BG map: its top two tile entries
/// and its bottom two, a map row (64 bytes) below.
static __inline__ void Map_CopyMetatileToScreen(u32 *cell, u8 *screenMap,
                                                u32 screenRowOffset,
                                                u32 screenColumnOffset) {
  u32 *metatileRows;
  u8 *screenTile;
  u32 index = (*cell << 20) >> 19;

  metatileRows = (u32 *)Ram_MapBlocks;
  metatileRows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *metatileRows;
  metatileRows = (u32 *)(Ram_MapBlocks + 4);
  metatileRows += index;
  *(u32 *)(screenTile + 64) = *metatileRows;
}

struct LayeredSceneEntry {
  u16 resources[6];
};

extern struct LayeredSceneEntry gMap_LayeredScenes[];

struct LayeredSceneLayer {
  u8 x;
  u8 y;
  s8 scrollX;
  s8 scrollY;
  s8 speedX;
  s8 speedY;
  u8 periodX;
  u8 periodY;
};

struct LayeredSceneHeader {
  u8 origin[4];
  u8 priority[3];
  u8 charBase[3];
  u16 unk0a;
  struct LayeredSceneLayer layers[3];
  s32 tiles;
  s32 collision;
  s32 tilemap;
  s32 animation;
  s32 blend;
  s32 script;
};

/// Loads layered scene index from gMap_LayeredScenes: its blocks, collision,
/// cells, tile animations, blend script and event script; sets up the three
/// background layers' origins, parallax and wrap from the scene header; loads
/// the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says they are
/// loaded; and starts the layer scrolling. Returns 2.
s32 Map_LoadLayeredScene(s32 index) {
  struct LayeredSceneEntry *sceneResources;
  struct MapScrollWork *work;
  struct LayeredSceneHeader *sceneHeader;
  struct MapLayerScroll *mapLayer;
  struct LayeredSceneLayer *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  sceneResources = &gMap_LayeredScenes[index];
  work = Runtime_AllocateBlock(HEAP_SLOT_MAP, sizeof(struct MapScrollWork));
  Iwram_ClearWords(work, sizeof(struct MapScrollWork));
  sceneHeader = (struct LayeredSceneHeader *)Resource_GetTableEntry(
      sceneResources->resources[0] + (u32)&ResourceId_Map001);
  encodedData = (u8 *)sceneHeader + sceneHeader->tiles;
  Tilemap_DecodeStagedBuffer(
      Resource_DecodeType01(encodedData, Ram_MapCellBuffer + 1));
  encodedData = (u8 *)sceneHeader + sceneHeader->collision;
  Resource_DecodeType01(encodedData, Ram_MapCollision);
  encodedData = (u8 *)sceneHeader + sceneHeader->tilemap;
  Resource_DecodeType01(encodedData, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  // Optional scripts share the decoded scene's collision-work allocation.
  if (sceneHeader->animation != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->animation;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
  }
  if (sceneHeader->blend != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->blend;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1e00);
    DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
  }
  // Publish camera bounds before deriving the three layers' parallax origins.
  work->script = (u8 *)sceneHeader + sceneHeader->script;
  work->minX = sceneHeader->origin[0] << 19;
  work->minY = sceneHeader->origin[1] << 19;
  work->maxX = sceneHeader->origin[2] << 19;
  work->maxY = sceneHeader->origin[3] << 19;
  viewX = &work->viewX;
  *viewX = 0;
  viewY = &work->viewY;
  *viewY = 0;
  work->enabled[0] = sceneHeader->priority[0];
  work->enabled[1] = sceneHeader->priority[1];
  work->enabled[2] = sceneHeader->priority[2];
  mapLayer = work->layers;
  layerSource = sceneHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scrollX;
    s32 scrollY;
    s32 baseX;
    s32 baseY;

    mapLayer->offsetX = baseX = x << 19;
    mapLayer->offsetY = baseY = y << 19;
    // Signed byte parallax and velocity use twelve fractional bits.
    // Multiplication also handles negative coefficients without a signed shift.
    scrollX = layerSource->scrollX * 4096;
    scrollY = layerSource->scrollY * 4096;
    mapLayer->scaleX = scrollX;
    mapLayer->scaleY = scrollY;
    mapLayer->speedX = layerSource->speedX * 4096;
    mapLayer->speedY = layerSource->speedY * 4096;
    mapLayer->maskX = layerSource->periodX;
    mapLayer->maskY = layerSource->periodY;
    mapLayer->phaseX = 0;
    mapLayer->phaseY = 0;
    mapLayer->cells =
        (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
    mapLayer->x = Iwram_MulQ16(*viewX, scrollX) + baseX;
    mapLayer->y = Iwram_MulQ16(*viewY, scrollY) + baseY;
    layerSource++;
    mapLayer++;
  }
  // Match each enabled layer to BG3, BG2, then BG1.
  work->flags = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->flags |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->flags |= DISPCNT_BG1_ON;
  displayFlags = work->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5);
  REG_BG3CNT = displayFlags;
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
    GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
  } else {
    graphicsBuffer = MemAlloc(0x4000);
    if (graphicsBuffer != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Resource_DecodeType01(
          Resource_GetTableEntry(sceneResources->resources[1] +
                                 (u32)&ResourceId_Map001),
          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      /* FAKEMATCH(wrapper): these four copies call the fixed entry itself, past
       * the IWRAM call layer; as Iwram_CopyWords, 103 of 349 instructions
       * differ in tbs-en. */
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, 0x1c0);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[2] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[3] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[4] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[5] +
                                                  (u32)&ResourceId_Map001),
                           gMapLayerData);
      MemFree(graphicsBuffer);
    }
  }
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  REG_DISPCNT = displayFlags;
  // UB: the scheduler returns s32; call it through its declared type.
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Map_UpdateLayerScroll, 0xc85);
#else
  // Called through a cast as void (*)(void (*)(void), s32); called as its
  // prototype, 2 of 349 instructions differ in tbs-en.
  ((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)(
      Map_UpdateLayerScroll, 0xc85);
#endif
  return 2;
}

/// Centres the camera on the point the map follows (origin: x, height, z),
/// or on (0, 0) when there is none, and scrolls the layers there.
void Map_CenterOnFollowedPoint(void) {
  struct MapScrollWork *work = gMapWork[0];
  s32 x;
  s32 depth;
  s32 height;
  s32 *followedPosition;

  followedPosition = work->origin;
  x = 0;
  height = 0;
  depth = 0;
  if (followedPosition != NULL) {
    x = *followedPosition++;
    height = *followedPosition++;
    depth = *followedPosition;
  }
  Map_SetCameraCenter(x, depth - height);
  Map_UpdateLayerScroll();
}

/// Draws a row of 16 blocks (32 tiles) of a layer's cells into its BG map
/// (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
/// wraps every 32 tiles.
void Map_RenderMetatileRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7F) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 0x7F;
  u32 screenColumnOffset = tileX & 30;
  u32 metatileIndex;

  for (metatileIndex = 0; metatileIndex <= 15; metatileIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapCellBuffer;
    u32 *metatileRows;
    u8 *screenTile;
    u32 metatileWordOffset;

    sourceCell += sourceRow + sourceColumn;
    metatileWordOffset = (*sourceCell << 20) >> 19;
    metatileRows = (u32 *)Ram_MapBlocks;
    metatileRows += metatileWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *metatileRows;
    metatileRows = (u32 *)(Ram_MapBlocks + 4);
    metatileRows += metatileWordOffset;
    *(u32 *)(screenTile + 64) = *metatileRows;
    sourceColumn = (sourceColumn + 1) & 0x7F;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}

/// Draws a column of 11 blocks of a layer's cells, one tile wide, into its BG
/// map, starting at tile (tileX, tileY).
void Map_RenderMetatileColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7F) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 0x7F;
  u32 screenColumnOffset = tileX & 30;
  u32 tileHalf = tileX & 1;
  u32 metatileIndex;

  for (metatileIndex = 0; metatileIndex <= 10; metatileIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapCellBuffer;

    sourceCell += sourceRow + sourceColumn;
    Map_CopyMetatileHalfColumn(sourceCell, screenMap, screenRowOffset,
                               screenColumnOffset, tileHalf);

    sourceRow = (sourceRow + 128) & 0x3F80;
    screenRowOffset = (screenRowOffset + 64) & 0x3C0;
  }
}

/// Each frame on a layered map: centres the view on the point the map follows
/// (origin), clamped to the map's bounds and shaken by any decaying screen
/// shake; then scrolls each of the three layers by its parallax scale and
/// automatic scroll (wrapping at its period), draws the block column or row
/// that scrolls into view, and sets its BG scroll.
void Map_UpdateLayerScroll(void) {
  struct MapScrollWork *work;
  struct MapLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 layerIndex;

  work = gMapWork[0];
  origin = work->origin;
  layer = work->layers;
  if (origin == 0)
    return;

  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;
    baseY = *origin - height - 0x600000;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - 0xf00000;
  minY = work->minY + work->shakeY;
  maxY = work->maxY - work->shakeY - 0xa00000;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  CLAMP(baseX, minX, maxX);
  CLAMP(baseY, minY, maxY);

  if (work->shakeX != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = work->shakeX;
    baseX += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    work->shakeX = Iwram_MulQ16(shakeAmplitude, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = work->shakeY;
    baseY += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    work->shakeY = Iwram_MulQ16(shakeAmplitude, work->shakeDecay);
  }
  work->viewX = baseX;
  work->viewY = baseY;
  for (layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
    baseX = Iwram_MulQ16(work->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(work->viewY, layer->scaleY);
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX = (baseX + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY = (baseY + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    x = baseX / 0x80000;
    y = baseY / 0x80000;
    if ((layer->x ^ baseX) & 0x80000) {
      if (layer->x < baseX)
        Map_RenderMetatileColumn(layerIndex, x + 30, y);
      else
        Map_RenderMetatileColumn(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Map_RenderMetatileRow(layerIndex, x, y + 20);
      else
        Map_RenderMetatileRow(layerIndex, x, y);
    }
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
    layer->x = baseX;
    layer->y = baseY;
  }
}

/// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds,
/// and redraws every enabled layer's BG map around it.
void Map_SetCameraCenter(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
  struct MapScrollWork *work = gMapWork[0];
  struct MapLayerScroll *layer = work->layers;

  x -= 0x780000;
  y -= 0x600000;
  CLAMP(x, work->minX, work->maxX - 0xf00000);
  CLAMP(y, work->minY, work->maxY - 0xa00000);
  work->viewX = x;
  work->viewY = y;

  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    if (work->enabled[layerIndex]) {
      u32 rows = 22;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          metatileRow, metatileColumn;

      x = Iwram_MulQ16(work->viewX, layer->scaleX);
      y = Iwram_MulQ16(work->viewY, layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x = (x + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y = (y + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
        rows = 32;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      layer++;
      x /= 0x80000;
      y /= 0x80000;
      screenMap = BG_SCREEN_BLOCK(5) + (layerIndex << 11);
      sourceRow = ((y / 2) & 127) << 7;
      screenRowOffset = (y & 30) << 5;
      for (metatileRow = 0; metatileRow < rows / 2; metatileRow++) {
        sourceColumn = (x / 2) & 127;
        screenColumnOffset = x & 30;
        for (metatileColumn = 0; metatileColumn <= 15; metatileColumn++) {
          u32 *map = (u32 *)Ram_MapCellBuffer;
          map += sourceRow + sourceColumn;
          Map_CopyMetatileToScreen(map, screenMap, screenRowOffset,
                                   screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & 127;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + 128) & 0x3f80;
        screenRowOffset = (screenRowOffset + 64) & 0x3c0;
      }
    }
  }
}

/// The top left of a layer's visible area, in cells.
struct MapTilePos {
  s32 x, y;
};

/// Copies the block numbers of a rectangle of cells from (srcX, srcY) to
/// (dstX, dstY), width by height cells, keeping each destination's
/// attributes, and redraws the copied cells that are on screen.
void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                 u32 width, u32 height) {
  /* FAKEMATCH(temporary): the chains of temporaries set the registers and the
   * schedule of the cell loop; as a plain body (the offset computed once, two
   * word stores), 96 of 141 instructions differ in tbs-en. */
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
  struct MapTilePos visibleLayers[3];
  s32 i;
  struct MapTilePos *visibleLayer = visibleLayers;
  u32 y;
  u32 x;

  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 sourceCell = *sourceCells++;
      u32 metatileId = sourceCell & 0xfff;
      u32 offset;
      u32 tmp3;
      u32 n;
      u32 tmp6;
      u32 tmp9;
      u32 tmp12;
      u32 tmp11;
      u32 tmp13;
      u32 tmp15;
      tmp9 = *destinationCells;
      tmp3 = tmp9 & -0x1000;
      tmp12 = x & 15;
      destinationCells[0] = metatileId | tmp3;
      tmp6 = tmp12;
      destinationCells++;
      i = 0;
      tmp15 = y & 15;
      tmp3 = tmp15;
      visibleLayer = visibleLayers;
      tmp13 = tmp3 << 5;
      tmp12 = tmp13;
      n = tmp12;
      n += tmp6;
      tmp13 = 4 * n;
      tmp11 = tmp13;
      tmp6 = tmp11;
      offset = tmp6;
      for (; i < 3; i++) {
        if (visibleLayer->x <= (s32)x && 16 + visibleLayer->x > (s32)x &&
            visibleLayer->y <= (s32)y && visibleLayer->y + 12 > (s32)y) {
          u32 screenAddress, metatileAddress;
          u8 *screenTile, *metatileRows;

          screenAddress = 0x06002800;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          tmp11 = metatileId * 8;
          metatileAddress = (u32)Ram_MapBlocks;
          metatileRows = (u8 *)metatileAddress;
          metatileRows += tmp11;
          *(u32 *)screenTile = *(u32 *)metatileRows;

          metatileAddress = (u32)Ram_MapBlocks + 4;
          metatileRows = (u8 *)metatileAddress;
          metatileRows += tmp11;
          screenAddress = 0x06002840;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)metatileRows;
          break;
        }
        offset += 0x800;
        visibleLayer++;
      }
    }
    sourceCells += 128 - width;
    destinationCells += 128 - width;
  }
}

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

/// Plays a map edit sequence (struct MapCopyCommand records, ended by a
/// source of 0xffff): copies each record's rectangle of block numbers to
/// (destinationX, destinationY) and waits its delay in frames. Doors and
/// other changing scenery are drawn this way.
void Map_PlayMetatileCopySequence(u16 *sequence, s32 destinationX,
                                  u32 destinationY) {
  struct MapCopyCommand *copyCommand = (struct MapCopyCommand *)sequence;
  u32 sourceX = copyCommand->source;
  u32 endMarker = 0xffff;
  u32 firstSourceCopy = sourceX;

  /* FAKEMATCH(dead): the updates that cancel out emit nothing but end CSE
   * before the comparison, keeping its ip copy while source stays in r0;
   * without them, 2 of 53 instructions differ in tbs-en. */
  if (copyCommand) {
    sourceX ^= firstSourceCopy;
    sourceX ^= firstSourceCopy;
  }
  if (firstSourceCopy != endMarker) {
    u32 endMarkerCopy = endMarker;
    s16 *rectangleArgs = (s16 *)(sequence + 1);

    while (1) {
      s32 sourceY = rectangleArgs[0];
      s32 width = rectangleArgs[1];
      s32 height = rectangleArgs[2];
      s32 delay = rectangleArgs[3];
      rectangleArgs += 5;

      Map_CopyMetatileIndicesRect(sourceX, (u16)sourceY, destinationX,
                                  destinationY, (u16)width, (u16)height);
      copyCommand++;
      WaitFrames((u16)delay);
      sourceX = copyCommand->source;
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH(barrier): the same ip copy after the wait, which CSE's copy
         * swap or combine removes in every plain form tried; without the
         * barrier, 2 of 53 instructions differ in tbs-en. */
        __asm__("" : "+r"(nextSourceCopy));
        if (nextSourceCopy == endMarkerCopy)
          break;
      }
    }
  }
}

// Map: copying rectangles of cells.

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff      ///< the block (metatile) number
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 ///< the cell's attributes

/// Copies a rectangle of cells (16x16 pixels each) as they are, from
/// (srcX, srcY) to (dstX, dstY), width by height cells. Where a copied cell
/// lies in a layer's visible area (16x12 cells), its block's 2x2 tiles go to
/// that layer's BG map in VRAM too.
void Map_CopyMetatileCellsRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  s32 y = dstY;
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = &((u32 *)Ram_MapCellBuffer)[y * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct MapTilePos visibleLayers[3];
  s32 i;
  struct MapTilePos *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 metatileId;
        u32 offset;
        u32 tmp3;
        u32 *screenTile;
        u32 n;
        u32 *metatileRows;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        destinationCells[0] = sourceCell;
        metatileId = sourceCell & 0xfff;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        i = 0;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (0x800
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
            // The block's (8 bytes) top two tiles, and its bottom two 0x40
            // bytes further on.
            u32 *tmp;
            u32 tmp2;
            u32 tmp4;
            u32 *tmp7;
            u32 tmp10;
            u32 tmp14;
            u32 tmp16;
            /* FAKEMATCH(temporary): restoring the unscaled cell keeps loop.c's
             * shift inside the successful layer branch, and the copy chain
             * through tmp2 to tmp17 sets the registers and stack slots of the
             * native 36-byte frame; as two plain word stores, 65 of 137
             * instructions differ in tbs-en. */
            u32 bytes = metatileId * 8;
            u32 tmp8;
            u32 tmp17;

            metatileId = bytes / 8;
            tmp = (u32 *)Ram_MapBlocks;
            tmp8 = 0x06002800 + offset;
            metatileRows = tmp;
            tmp2 = tmp8;
            tmp = (u32 *)tmp2;
            screenTile = tmp;
            metatileRows = (u32 *)((u8 *)metatileRows + bytes);
            tmp16 = *metatileRows;
            tmp14 = tmp16;
            tmp17 = 0x06002840 + offset;
            tmp10 = tmp14;
            *screenTile = tmp10;
            tmp7 = (u32 *)(Ram_MapBlocks + 4);
            metatileRows = tmp7;
            metatileRows = (u32 *)((u8 *)metatileRows + bytes);
            tmp8 = tmp17;
            tmp14 = tmp8;
            tmp17 = metatileRows[0];
            tmp16 = tmp17;
            tmp10 = tmp16;
            tmp4 = tmp10;
            tmp2 = tmp4;
            tmp4 = tmp14;
            tmp7 = (u32 *)tmp4;
            screenTile = tmp7;
            *screenTile = tmp2;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells += (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

/// Copies only the attributes of a rectangle of cells, from (srcX, srcY) to
/// (dstX, dstY), width by height cells, keeping each destination cell's
/// block number.
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *destinationCells = MAP_CELLS;
  u32 *sourceCells = destinationCells;
  u16 rowIndex;
  u16 columnIndex;

  destinationCells += (dstY << 7) + dstX;
  sourceCells += (srcY << 7) + srcX;

  for (rowIndex = 0; rowIndex < height; rowIndex++) {
    u32 *destinationCell = destinationCells + (rowIndex << 7);
    u32 *sourceCell = sourceCells + (rowIndex << 7);

    for (columnIndex = 0; columnIndex < width; columnIndex++) {
      *destinationCell = (*destinationCell & MAP_CELL_TILE_MASK) |
                         (*sourceCell++ & MAP_CELL_ATTRIBUTE_MASK);
      destinationCell++;
    }
  }
}

/// Copies only the block numbers of a rectangle of cells, from (srcX, srcY)
/// to (dstX, dstY), width by height cells, keeping each destination cell's
/// attributes; cells in a visible area update the BG maps too.
void Map_CopyCellIndexRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                           s32 dstY) {
  /* FAKEMATCH(temporary): the word and index temporaries set the registers and
   * the 36-byte frame; as a plain body (the offset computed once, two word
   * stores), 94 of 142 instructions differ in tbs-en. */
  s32 y;
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = &((u32 *)Ram_MapCellBuffer)[dstY * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct MapTilePos visibleLayers[3];
  s32 i;
  struct MapTilePos *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  y = dstY;
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 metatileId;
        u32 offset;
        u32 tmp3;
        u32 n;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        metatileId = sourceCell & MAP_CELL_TILE_MASK;
        destinationCells[0] =
            (destinationCells[0] & MAP_CELL_ATTRIBUTE_MASK) | metatileId;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        i = 0;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (0x800
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
            // The block's (8 bytes) top two tiles, and its bottom two 0x40
            // bytes further on.
            u32 screenAddress, metatileAddress;
            u8 *screenTile, *metatileRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            tmp13 = metatileId * 8;
            metatileAddress = (u32)Ram_MapBlocks;
            metatileRows = (u8 *)metatileAddress;
            metatileRows += tmp13;
            *(u32 *)screenTile = *(u32 *)metatileRows;

            metatileAddress = (u32)Ram_MapBlocks + 4;
            metatileRows = (u8 *)metatileAddress;
            metatileRows += tmp13;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)metatileRows;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells = sourceCells + (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

/// Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
/// REG_DISPCNT) from layerFlags, keeping its other flags.
void Map_SetShownLayers(u32 layerFlags) {
  struct MapState *mapState = gMapWork[0];
  u32 mask = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
  u32 flags = mapState->flags;

  // A halfword mask: as ~(...) without the cast, the mask in the literal pool
  // is 0xfffff1ff, where the ROM has 0xf1ff.
  flags =
      (flags & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) | mask;
  mapState->flags = flags;
}

#undef MAP_CELLS
#undef MAP_CELL_TILE_MASK
#undef MAP_CELL_ATTRIBUTE_MASK
