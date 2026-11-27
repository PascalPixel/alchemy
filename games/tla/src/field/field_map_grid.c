/* Layered maps: decoding their tilemaps and palette, loading, drawing and
   scrolling, copying metatiles, and the edits flags apply. Part of field.c. */

/* A metatile word: its metatile tiles number in the low 12 bits, the rest its
   attributes. */
#define MAPGRID_METATILE_TILES_ID_MASK \
	0x00000fff								   /* the metatile tiles number */
#define MAPGRID_ATTRIBUTE_MASK		0xfffff000 /* the metatile's attributes */
/* The metatile tiles number the renderer draws, the low 11 bits: the copies
   that use MAPGRID_METATILE_TILES_ID_MASK carry the bit above it as well. */
#define MAPGRID_DRAWN_PICTURE_MASK	0x7ff
/* The shift that leaves a metatile word's drawn picture number on top, the bits
   above it shifted out. Shifted back down by one less it is the number times 2,
   the picture's offset in words in syRamMapBlocks (a word for each of its two
   rows of 2 tiles); by two less, times 4, its offset in halfwords. */
#define MAPGRID_DRAWN_PICTURE_SHIFT 21
/* The map's 128 by 128 metatile words. */
#define MAPGRID_METATILES			((Uint32 *)Ram_MapGridMetatileBuffer)
/* A layer's visible area in metatiles: the screen's 15 by 10, and the row and
   column partly scrolled in. */
#define MAPGRID_VISIBLE_WIDTH		16
#define MAPGRID_VISIBLE_HEIGHT		12
/* A layer's BG map in metatiles, 16 by 16 (32 by 32 tiles), and the bytes of
   one row of its tiles. */
#define MAPGRID_SCREEN_METATILES	16
#define MAPGRID_SCREEN_ROW_BYTES	64
/* A metatile's top tile row or left tile column in a layer's BG map, even,
   wrapping at the map's 32; its row as tile entries from the map's start. */
#define MAPGRID_SCREEN_TILE_MASK	(BG_SCREEN_WIDTH - 2)
#define MAPGRID_SCREEN_ROW_MASK		(MAPGRID_SCREEN_TILE_MASK * BG_SCREEN_WIDTH)
/* The 8-pixel tile a 16.16 layer coordinate lies in, as FIELD_METATILE the
   16-pixel metatile, and a map's width and height in those tiles. */
#define MAPGRID_TILE(coordinate)	((Uint32)(coordinate) >> 19)
#define MAPGRID_TILES				(MAPGRID_SIZE * MAPGRID_METATILE_PIXELS / 8)
/* A map's palette: the first 14 of the 16 BG palettes, 224 colours. */
#define MAP_PALETTE_BYTES			(14 * 16 * sizeof(Uint16))
/* The metatile shapes' buffer, four bytes a shape, cleared to -1 before a map's
   shapes are decoded into it; the map's script is decoded after it. */
#define MAPGRID_SHAPE_TABLE_BYTES	8192
/* A world map tile's 16 by 16 blocks, as fdWorldMapWriteLayerMetatileTile
   decodes one. */
#define WORLD_MAP_TILE_METATILES	16

/* The world map's two tile maps, of 64 by 64 one-byte tiles: BG3's, the lower
   layer, and BG2's, the upper, right after it (fdWorldMapEndStillImage). */
#define WORLD_MAP_LOWER_TILE_MAP BG_SCREEN_BLOCK(8)
#define WORLD_MAP_UPPER_TILE_MAP BG_SCREEN_BLOCK(10)
#define WORLD_MAP_TILE_MAP_WIDTH 64
/* The world map metatiles in view (syRamMapBlocks): WORLD_MAP_VIEW_METATILES
   square for the lower layer, then as many for the upper, each metatile 2 by 2
   tiles of its tile map. */
#define WORLD_MAP_VIEW_LAYER_BYTES \
	(WORLD_MAP_VIEW_METATILES * WORLD_MAP_VIEW_METATILES * \
		sizeof(WORLD_MAP_METATILE))

/* Remaps byteCount bytes in place through SpriteColorGreyTable. */
void fdSprColRemapPixels(Uint8 *bytes, Sint32 byteCount)
{
	Uint8 *remapTable = (Uint8 *)SpriteColorGreyTable;

	while(byteCount-- != 0) {
		*bytes = remapTable[*bytes];
		bytes++;
	}
}

/* Unpacks a map's metatile tiles, decoded into the map metatile buffer from its
   second byte, into MetatileTiles. The buffer's second byte is how they are
   packed (METATILE_TILES_PACKING) and the halfwords follow it; size counts from
   that byte, and an even size leaves nothing to unpack. */
void fdTileMapDecodeStagedBuffer(Sint32 size)
{
	Uint16 *dst;
	Uint16 *src;
	Sint32	n;
	Sint32	i;

	src = (Uint16 *)(Ram_MapMetatileBuffer + 2);
	dst = (Uint16 *)MetatileTiles;
	n = (size - 1) / 2;
	if(size & 1) {
		switch(((Uint8 *)MapGridMetatileBuffer)[1]) {
		case METATILE_TILES_PACKING_RAW:
			for(i = 0; i < n; i++)
				*dst++ = *src++;
			break;
		case METATILE_TILES_PACKING_SPLIT_DELTA: {
			Uint32 prev = 0;
			Uint8 *hi = Ram_MapMetatileBuffer + 2;
			Uint8 *lo = hi + n;

			for(i = 0; i < n; i++) {
				Uint32 v = ((*hi++ << 8) | *lo++) ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		case METATILE_TILES_PACKING_DELTA: {
			Sint32 prev = 0;

			for(i = 0; i < n; i++) {
				Sint32 v = *src++ ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		}
	}
}

/* A map's metatiles as they are decoded: two planes of a byte a metatile, the
   metatiles' attribute bytes, then a plane of their metatile tiles numbers, a
   halfword a metatile. */
#define MAPGRID_BYTE_PLANES_BYTES (2 * MAPGRID_SIZE * MAPGRID_SIZE)

/* Joins a decoded map's planes in place into its metatile words, with the ARM
   function copied to RAM, which reads the byte planes from a copy. */
void fdTileMapConvertBuffer(void)
{
	Uint8			   *saved;
	CONVERT_MAP_ROUTINE routine;
	Uint32				size;

	size = MAPGRID_BYTE_PLANES_BYTES;
	saved = syHeapAllocEWRAM(size);
	syWordsCopy(saved, Ram_MapMetatileBuffer, size);
	RAM_CODE_LOAD(routine, fdRenderConvertMapCode, fdRenderConvertMapCodeSize);
	routine(((Uint8 *)MapGridMetatileBuffer + MAPGRID_BYTE_PLANES_BYTES),
		Ram_MapMetatileBuffer, saved);
	syHeapRelease(routine);
	syHeapRelease(saved);
}

/* A layout's resource, counted in MapsWork from the empty map. */
#define MAPGRID_RESOURCE(offset) \
	syResourceGetTableEntry((offset) + RESOURCE_EMPTY_MAP)

/* Decodes the palette resource of a map's header row (its second halfword) and
   copies its 224 colors to destination, keeping the current backdrop color in
   the first entry. */
void fdMapCopyPalette(Sint32 map, void *destination)
{
	const MAP_RESOURCES *entry;
	Uint16				*buffer;
	Sint16				 backdrop;

	entry = &MapsWork[map];
	buffer = syHeapAlloc(BG_PLTT_SIZE);
	backdrop = *(Sint16 *)BG_PLTT;
	syDecompressTaggedLZ(MAPGRID_RESOURCE(entry->pal), buffer);
	buffer[0] = backdrop;
	syDMAXCopy(
		buffer, destination, DMA_COPY_WORDS(MAP_PALETTE_BYTES / 4), REG_DMA3);
	syHeapRelease(buffer);
}

/* Loads layout index from MapsWork: its metatile tiles, collision (cleared to
   -1 first), metatiles, shapes, tileset animations, blend script and script;
   sets up the three background layers' origins, parallax and wrap from the
   layout header; loads the palette and tile graphics unless
   FLAG_KEEP_LAYER_GRAPHICS says they are loaded; and starts the layer scrolling
   (fdMapGridUpdateLayerScroll). Returns 2, as The Broken Seal's does; no caller
   reads it. */
Sint32 fdMapGridLoadMap(Sint32 index)
{
	MAP_RESOURCES	 *sceneResources;
	MAP_STATE		 *map;
	MAP_HEADER		 *sceneHeader;
	MAP_LAYER		 *layer;
	MAP_HEADER_LAYER *layerSource;
	Sint32			 *viewX;
	Sint32			 *viewY;
	Sint32			  layerIndex;
	Uint8			 *graphicsBuffer;
	Sint32			  backdrop;
	Uint8			 *encodedData;
	Sint32			  displayFlags;

	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScreenFadeFromBlack(0);
	sceneResources = (MAP_RESOURCES *)&MapsWork[index];
	/* A field map's state ends after its three layers; the world map's goes
	   on. */
	map = syHeapSlotAllocEWRAM(HEAP_SLOT_MAP_GRID, MAPGRID_FIELD_STATE_SIZE);
	syWordsClearEntry(map, MAPGRID_FIELD_STATE_SIZE);
	sceneHeader = (MAP_HEADER *)MAPGRID_RESOURCE(sceneResources->header);
	encodedData = (Uint8 *)sceneHeader + sceneHeader->metatileTiles;
	fdTileMapDecodeStagedBuffer(
		syDecompressTaggedLZ(encodedData, Ram_MapMetatileBuffer + 1));
	DMA_FILL32(-1, Ram_MapCollision, MAPGRID_SHAPE_TABLE_BYTES);
	encodedData = (Uint8 *)sceneHeader + sceneHeader->collision;
	syDecompressTaggedLZ(encodedData, Ram_MapCollision);
	encodedData = (Uint8 *)sceneHeader + sceneHeader->metatiles;
	syDecompressTaggedLZ(encodedData, Ram_MapMetatileBuffer);
	fdTileMapConvertBuffer();
	encodedData = (Uint8 *)sceneHeader + sceneHeader->shapes;
	syDecompressTaggedLZ(encodedData, MapShapeGrid);
	if(sceneHeader->anm != 0) {
		encodedData = (Uint8 *)sceneHeader + sceneHeader->anm;
		syDecompressTaggedLZ(encodedData, MAPGRID_ANIMATION_SCRIPT);
		fdTilesetAnimStartAll((const Uint16 *)MAPGRID_ANIMATION_SCRIPT);
	}
	if(sceneHeader->blend != 0) {
		encodedData = (Uint8 *)sceneHeader + sceneHeader->blend;
		syDecompressTaggedLZ(encodedData, MAPGRID_BLEND_SCRIPT);
		fdBlendScriptStart((Uint16 *)MAPGRID_BLEND_SCRIPT);
	}
	encodedData = (Uint8 *)sceneHeader + sceneHeader->script;
	syDecompressTaggedLZ(
		encodedData, Ram_MapCollision + MAPGRID_SHAPE_TABLE_BYTES);
	map->minX = toFIXED(sceneHeader->cameraBounds[0] * 8);
	map->minZ = toFIXED(sceneHeader->cameraBounds[1] * 8);
	map->maxX = toFIXED(sceneHeader->cameraBounds[2] * 8);
	map->maxZ = toFIXED(sceneHeader->cameraBounds[3] * 8);
	viewX = &map->view[0];
	*viewX = 0;
	viewY = &map->view[1];
	*viewY = 0;
	map->enabled[0] = sceneHeader->pri[0];
	map->enabled[1] = sceneHeader->pri[1];
	map->enabled[2] = sceneHeader->pri[2];
	map->basePri = 2;
	if(map->maxX == 0)
		map->maxX = toFIXED(MAPGRID_SIZE * MAPGRID_METATILE_PIXELS);
	if(map->maxZ == 0)
		map->maxZ = toFIXED(MAPGRID_SIZE * MAPGRID_METATILE_PIXELS);
	layer = map->layers;
	layerSource = sceneHeader->layers;
	for(layerIndex = 0; layerIndex < 3; layerIndex++) {
		Uint32 x = layerSource->x;
		Uint32 y = layerSource->y;
		Sint32 scaleX;
		Sint32 metatile;
		Uint8 *shapeGrid;
		Uint32 maskY;

		layer->ofsX = toFIXED(x * 8);
		layer->ofsY = toFIXED(y * 8);
		/* Parallax and drift are signed sixteenths, made 16.16. */
		layer->scaleY = layerSource->scrollY * (toFIXED(1) / 16);
		layer->speedX = layerSource->speedX * (toFIXED(1) / 16);
		layer->speedY = layerSource->speedY * (toFIXED(1) / 16);
		layer->maskX = layerSource->periodX & (MAPGRID_SIZE - 1);
		maskY = layerSource->periodY & (MAPGRID_SIZE - 1);
		layer->phaseX = 0;
		layer->phaseY = 0;
		layer->maskY = maskY << 7;
		x >>= 1;
		y = (y >> 1) * MAPGRID_SIZE;
		layer->originY = y;
		scaleX = layerSource->scrollX * (toFIXED(1) / 16);
		metatile = y + x;
		shapeGrid = MapShapeGrid;
		layer->metatiles = (MAP_METATILE *)Ram_MapMetatileBuffer + metatile;
		layer->scaleX = scaleX;
		layer->originX = x;
		layer->terrainIds = shapeGrid + metatile;
		layer->x = syMulFX(*viewX, scaleX) + layer->ofsX;
		layer->y = syMulFX(*viewY, layer->scaleY) + layer->ofsY;
		layerSource++;
		layer++;
	}
	map->dispcnt = DISPCNT_OBJ_ON;
	if(map->enabled[0] != 0)
		map->dispcnt = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
	if(map->enabled[1] != 0)
		map->dispcnt |= DISPCNT_BG2_ON;
	if(map->enabled[2] != 0)
		map->dispcnt |= DISPCNT_BG1_ON;
	syIOWrite16(map->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
			BGCNT_SCREEN_BASE(5),
		&REG_BG3CNT);
	syIOWrite16(map->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
			BGCNT_SCREEN_BASE(6),
		&REG_BG2CNT);
	syIOWrite16(map->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
			BGCNT_SCREEN_BASE(7),
		&REG_BG1CNT);
	if(syGameFlagGet(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
		syGameFlagClear(FLAG_KEEP_LAYER_GRAPHICS);
	} else {
		graphicsBuffer = syHeapAlloc(BG_CHAR_SIZE);
		if(graphicsBuffer != NULL) {
			backdrop = *(Sint16 *)BG_PLTT;
			syDecompressTaggedLZ(
				MAPGRID_RESOURCE(sceneResources->pal), graphicsBuffer);
			*(Sint16 *)graphicsBuffer = backdrop;
			syWordsCopyEntry(
				(void *)BG_PLTT, graphicsBuffer, MAP_PALETTE_BYTES);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(sceneResources->tiles[0]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(1), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(sceneResources->tiles[1]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(2), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(sceneResources->tiles[2]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(3), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(sceneResources->anmTiles), MapGridAnimTiles);
			syHeapRelease(graphicsBuffer);
		}
	}
	displayFlags = 0;
	REG_MOSAIC = displayFlags;
	REG_BLDCNT = displayFlags;
	displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	syIOWrite16(displayFlags, &REG_DISPCNT);
	sySetEvent(fdMapGridUpdateLayerScroll, EVENT_PASS_FRAME + 5);
	return 2;
}

/* Centres the camera on the point the map follows (origin: x, height, z), or on
   (0, 0) when there is none, and scrolls the layers there. */
void fdMapGridCenterOnFollowedPoint(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   x = 0;
	Sint32	   height = 0;
	Sint32	   z = 0;
	Sint32	  *followedPosition = map->origin;

	if(followedPosition != NULL) {
		const Sint16 *rectList;
		const Sint16 *rect;

		x = *followedPosition++;
		height = *followedPosition++;
		z = *followedPosition++;
		rectList = map->cameraBoxes;
		if(rectList != NULL) {
			for(rect = rectList; rect[0] != -1; rect += 4) {
				FIXED left = toFIXED(rect[0]) - toFIXED(120);
				FIXED top = toFIXED(rect[1]) + height - toFIXED(64);
				FIXED right = toFIXED(rect[2]) + toFIXED(120);
				FIXED bottom = toFIXED(rect[3]) + height + toFIXED(96);

				if(x >= left && z >= top && x <= right && z <= bottom) {
					FIXED nearX = left;
					FIXED nearZ;

					if(ABS(nearX - x) > ABS(right - x))
						nearX = right;
					nearZ = top;
					if(ABS(nearZ - z) > ABS(bottom - z))
						nearZ = bottom;
					if(ABS(nearX - x) <= ABS(nearZ - z))
						x = nearX;
					else
						z = nearZ;
					break;
				}
			}
			followedPosition[-3] = x;
			followedPosition[-1] = z;
		}
	}
	fdMapGridSetCameraCenter(x, z - height);
	fdMapGridUpdateLayerScroll();
}

/* Draws the metatile tiles of a row of 16 of a layer's metatiles (32 tiles)
   into its BG map (screen block 5 + layer), starting at tile (tileX, tileY);
   the BG map wraps every 32 tiles and the metatiles wrap at the layer's window,
   which The Broken Seal's function does not have. Called by
   fdMapGridUpdateLayerScroll when a layer scrolls onto a new row of metatile
   pictures. */
void fdMapGridRenderMetatileTilesRow(Uint32 layer, Sint32 tileX, Sint32 tileY)
{
	Uint32	  *metatiles = (Uint32 *)Ram_MapMetatileBuffer;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *window = &map->layers[layer];
	Uint32	   originY = window->originY;
	Uint8	  *screenMap =
		(Uint8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	Uint32 maskY = window->maskY;
	Uint32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	Uint32 screenRowOffset =
		(tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	Uint32 sourceColumn;
	Uint32 screenColumnOffset;
	Uint32 maskX;
	Uint32 originX;
	Uint32 metatileTilesIndex;

	/* A window narrower than the 128 by 128 metatiles starts at its origin. */
	if(maskY != MAPGRID_ROW_MASK) {
		sourceRow = (sourceRow - originY) & maskY;
		metatiles += originY;
	}
	maskX = window->maskX;
	sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	originX = window->originX;
	screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	sourceColumn &= maskX;
	if(maskX != MAPGRID_SIZE - 1) {
		sourceColumn = (sourceColumn - originX) & maskX;
		metatiles += originX;
	}
	for(metatileTilesIndex = 0; metatileTilesIndex < MAPGRID_SCREEN_METATILES;
		metatileTilesIndex++) {
		Uint32 *sourceMetatile;
		Uint32 *metatileTilesRows;
		Uint8  *screenTile;
		Uint32	metatileTilesWordOffset;

		sourceMetatile = metatiles + sourceRow + sourceColumn;
		metatileTilesWordOffset =
			(*sourceMetatile << MAPGRID_DRAWN_PICTURE_SHIFT) >>
			(MAPGRID_DRAWN_PICTURE_SHIFT - 1);
		metatileTilesRows = (Uint32 *)syRamMapBlocks;
		metatileTilesRows += metatileTilesWordOffset;
		screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
		*(Uint32 *)screenTile = *metatileTilesRows;
		metatileTilesRows = (Uint32 *)(syRamMapBlocks + 4);
		metatileTilesRows += metatileTilesWordOffset;
		sourceColumn++;
		*(Uint32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *metatileTilesRows;
		sourceColumn &= maskX;
		screenColumnOffset =
			(screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
	}
}

/* Copies one column (tileHalf 0 or 1) of a metatile's 2 by 2 picture tiles into
   the screen map at the row and column offsets. */
static __inline__ void CopyMetatileTilesHalfColumn(Uint32 *metatile,
	Uint8 *screenMap, Uint32 screenRowOffset, Uint32 screenColumnOffset,
	Uint32 tileHalf)
{
	Uint16 *metatileTilesEntry;
	Uint8  *screenTile;
	Uint32	index = ((*metatile << MAPGRID_DRAWN_PICTURE_SHIFT) >>
						(MAPGRID_DRAWN_PICTURE_SHIFT - 2)) +
		tileHalf;

	metatileTilesEntry = (Uint16 *)syRamMapBlocks;
	metatileTilesEntry += index;
	screenTile =
		screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
	*(Uint16 *)screenTile = *metatileTilesEntry;
	metatileTilesEntry = (Uint16 *)(syRamMapBlocks + 4);
	metatileTilesEntry += index;
	*(Uint16 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *metatileTilesEntry;
}

/* Draws the metatile tiles of a column of 11 of a layer's metatiles, one tile
   wide, into its BG map from tile (tileX, tileY), as The Broken Seal's function
   of the same name; tileX's low bit picks the metatile tiles' left or right
   half. */
void fdMapGridRenderMetatileTilesColumn(
	Uint32 layer, Sint32 tileX, Sint32 tileY)
{
	Uint32	  *metatiles = (Uint32 *)Ram_MapMetatileBuffer;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *window = &map->layers[layer];
	Uint8	  *screenMap =
		(Uint8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	Uint32 maskY = window->maskY;
	Uint32 originY = window->originY;
	Uint32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	Uint32 screenRowOffset =
		(tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	Uint32 sourceColumn;
	Uint32 screenColumnOffset;
	Uint32 maskX;
	Uint32 originX;
	Uint32 tileHalf;
	Uint32 metatileTilesIndex;

	if(maskY != MAPGRID_ROW_MASK) {
		sourceRow = (sourceRow - originY) & maskY;
		metatiles = (Uint32 *)Ram_MapMetatileBuffer + originY;
	}
	maskX = window->maskX;
	originX = window->originX;
	sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	sourceColumn &= maskX;
	if(maskX != MAPGRID_SIZE - 1) {
		sourceColumn = (sourceColumn - originX) & maskX;
		metatiles += originX;
	}
	tileHalf = tileX & 1;
	for(metatileTilesIndex = 0;
		metatileTilesIndex <= DISPLAY_HEIGHT / MAPGRID_METATILE_PIXELS;
		metatileTilesIndex++) {
		CopyMetatileTilesHalfColumn(metatiles + sourceRow + sourceColumn,
			screenMap, screenRowOffset, screenColumnOffset, tileHalf);
		sourceRow = (sourceRow + MAPGRID_SIZE) & maskY;
		screenRowOffset =
			(screenRowOffset + 2 * BG_SCREEN_WIDTH) & MAPGRID_SCREEN_ROW_MASK;
	}
}

/* Each frame on a layered map: centres the view on the point the map follows
   (origin), clamped to the map's bounds and shaken by any decaying screen
   shake; then scrolls each of the three layers by its parallax scale and
   automatic scroll (wrapping at its period), draws the metatile tiles column or
   row that scrolls into view, and sets its BG scroll. */
void fdMapGridUpdateLayerScroll(void)
{
	MAP_STATE *map;
	MAP_LAYER *layer;
	Sint32	  *origin;
	Sint32	   x, y;
	Sint32	   minX, minY, maxX, maxY;
	Sint32	   baseX, baseY;
	Uint32	   layerIndex;

	map = Ram_HeapSlots->mapGrid;
	origin = map->origin;
	layer = map->layers;
	if(origin == 0)
		return;
	baseX = *origin++ - FIELD_VIEW_FOCUS_X;
	{
		Sint32 height = *origin++;

		baseY = *origin - height - FIELD_VIEW_FOCUS_Y;
	}
	minX = map->minX + map->shakeX;
	maxX = map->maxX - map->shakeX - FIELD_VIEW_WIDTH;
	minY = map->minZ + map->shakeY;
	maxY = map->maxZ - map->shakeY - FIELD_VIEW_HEIGHT;
	if(minX > maxX)
		maxX = minX;
	if(minY > maxY)
		maxY = minY;
	if(baseX < minX)
		baseX = minX;
	if(baseX > maxX)
		baseX = maxX;
	if(baseY < minY)
		baseY = minY;
	if(baseY > maxY)
		baseY = maxY;
	if(map->shakeX != 0) {
		Sint32 shakeRoll = syRandom();

		shakeRoll -= syRandom();
		baseX += syMulFX(map->shakeX, shakeRoll);
		map->shakeX = syMulFX(map->shakeX, map->shakeDecay);
	}
	if(map->shakeY != 0) {
		Sint32 shakeRoll = syRandom();

		shakeRoll -= syRandom();
		baseY += syMulFX(map->shakeY, shakeRoll);
		map->shakeY = syMulFX(map->shakeY, map->shakeDecay);
	}
	{
		Sint32 step = map->view[1] - baseY;

		if(step > toFIXED(16)) {
			baseY = map->view[1] - toFIXED(16);
			step = map->view[1] - baseY;
		}
		if(step < -toFIXED(16))
			baseY = map->view[1] + toFIXED(16);
	}
	map->view[0] = baseX;
	map->view[1] = baseY;
	for(layerIndex = 0; layerIndex <= 2; layerIndex++, layer++) {
		baseX = syMulFX(map->view[0], layer->scaleX);
		baseY = syMulFX(map->view[1], layer->scaleY);
		if(layer->speedX != 0) {
			layer->phaseX += layer->speedX;
			baseX += layer->phaseX;
		}
		if(layer->speedY != 0) {
			layer->phaseY += layer->speedY;
			baseY += layer->phaseY;
		}
		baseX += layer->ofsX;
		baseY += layer->ofsY;
		x = MAPGRID_TILE(baseX);
		y = MAPGRID_TILE(baseY);
		if((layer->x ^ baseX) & toFIXED(8)) {
			if(layer->x < baseX)
				fdMapGridRenderMetatileTilesColumn(
					layerIndex, x + DISPLAY_WIDTH / 8, y);
			else
				fdMapGridRenderMetatileTilesColumn(layerIndex, x, y);
		}
		if((layer->y ^ baseY) & toFIXED(16)) {
			if(layer->y < baseY)
				fdMapGridRenderMetatileTilesRow(
					layerIndex, x, y + DISPLAY_HEIGHT / 8);
			else
				fdMapGridRenderMetatileTilesRow(layerIndex, x, y);
		}
		layer->x = baseX;
		layer->y = baseY;
		BgScrollWork[3 - layerIndex].x = FIXEDtoINT(baseX);
		BgScrollWork[3 - layerIndex].y = FIXEDtoINT(baseY);
	}
}

/* Copies a metatile's 2 by 2 picture tiles into the screen map at the row and
   column offsets. */
static __inline__ void CopyMetatileTilesToScreen(Uint32 *metatile,
	Uint8 *screenMap, Uint32 screenRowOffset, Uint32 screenColumnOffset)
{
	Uint32 *metatileTilesRows;
	Uint8  *screenTile;
	Uint32	index = (*metatile << MAPGRID_DRAWN_PICTURE_SHIFT) >>
		(MAPGRID_DRAWN_PICTURE_SHIFT - 1);

	metatileTilesRows = (Uint32 *)syRamMapBlocks;
	metatileTilesRows += index;
	screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
	*(Uint32 *)screenTile = *metatileTilesRows;
	metatileTilesRows = (Uint32 *)(syRamMapBlocks + 4);
	metatileTilesRows += index;
	*(Uint32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *metatileTilesRows;
}

/* Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
   redraws every enabled layer's BG map around it. */
void fdMapGridSetCameraCenter(FIXED x, FIXED y)
{
	Uint32	   layerIndex;
	Uint8	  *screenMap;
	Uint32	  *metatiles;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer = map->layers;
	Uint32	   originX;

	x -= FIELD_VIEW_FOCUS_X;
	y -= FIELD_VIEW_FOCUS_Y;
	if(x < map->minX)
		x = map->minX;
	if(x > map->maxX - FIELD_VIEW_WIDTH)
		x = map->maxX - FIELD_VIEW_WIDTH;
	if(y < map->minZ)
		y = map->minZ;
	if(y > map->maxZ - FIELD_VIEW_HEIGHT)
		y = map->maxZ - FIELD_VIEW_HEIGHT;
	map->view[0] = x;
	map->view[1] = y;
	for(layerIndex = 0; layerIndex <= 2; layerIndex++) {
		if(map->enabled[layerIndex] != 0) {
			Uint32 maskX, maskY, originY;
			Uint32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
				metatileTilesRow, metatileTilesColumn;

			x = syMulFX(map->view[0], layer->scaleX);
			y = syMulFX(map->view[1], layer->scaleY);
			if(layer->speedX) {
				layer->phaseX += layer->speedX;
				x += layer->phaseX;
			}
			if(layer->speedY) {
				layer->phaseY += layer->speedY;
				y += layer->phaseY;
			}
			x += layer->ofsX;
			y += layer->ofsY;
			x /= toFIXED(8);
			y /= toFIXED(8);
			screenMap = (Uint8 *)BG_SCREEN_BLOCK(5) +
				layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
			maskY = layer->maskY;
			maskX = layer->maskX;
			originY = layer->originY;
			originX = layer->originX;
			metatiles = (Uint32 *)Ram_MapMetatileBuffer;
			if(maskX != MAPGRID_SIZE - 1)
				metatiles = (Uint32 *)Ram_MapMetatileBuffer + originX;
			if(maskY != MAPGRID_ROW_MASK)
				metatiles += originY;
			sourceRow = ((y / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
			screenRowOffset = (y & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
			if(maskY != MAPGRID_ROW_MASK)
				sourceRow = (sourceRow - originY) & maskY;
			for(metatileTilesRow = 0;
				metatileTilesRow <= DISPLAY_HEIGHT / MAPGRID_METATILE_PIXELS;
				metatileTilesRow++) {
				sourceColumn = (x / 2) & (MAPGRID_SIZE - 1);
				screenColumnOffset = x & MAPGRID_SCREEN_TILE_MASK;
				if(maskX != MAPGRID_SIZE - 1)
					sourceColumn = (sourceColumn - originX) & maskX;
				for(metatileTilesColumn = 0;
					metatileTilesColumn < MAPGRID_SCREEN_METATILES;
					metatileTilesColumn++) {
					CopyMetatileTilesToScreen(
						metatiles + sourceRow + sourceColumn, screenMap,
						screenRowOffset, screenColumnOffset);
					sourceColumn = (sourceColumn + 1) & maskX;
					screenColumnOffset =
						(screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
				}
				sourceRow = (sourceRow + MAPGRID_SIZE) & maskY;
				screenRowOffset = (screenRowOffset + 2 * BG_SCREEN_WIDTH) &
					MAPGRID_SCREEN_ROW_MASK;
			}
			layer++;
		}
	}
}

/* Copies the metatile tiles numbers of a rectangle of metatiles from (srcX,
   srcY) to (dstX, dstY), width by height metatiles, keeping each destination's
   attributes, and redraws the copied metatiles that are on screen. */
void fdMapGridCopyMetatileTilesIndicesRect(Sint32 srcX, Sint32 srcY,
	Uint32 dstX, Uint32 dstY, Uint32 width, Uint32 height)
{
	Uint32 *src =
		(Uint32 *)Ram_MapMetatileBuffer + (srcY * MAPGRID_SIZE + srcX);
	Uint32 *dst =
		(Uint32 *)Ram_MapMetatileBuffer + (dstY * MAPGRID_SIZE + dstX);
	MAP_LAYER		  *layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	MAP_GRID_TILE_POS  tile[3];
	Sint32			   i;
	MAP_GRID_TILE_POS *pos = tile;
	Uint32			   y;
	Uint32			   x;

	/* The three layers' visible areas: their scroll positions in metatiles. */
	for(i = 0; i < 3; i++) {
		pos->x = FIELD_METATILE(layer->x);
		pos->y = FIELD_METATILE(layer->y);
		pos++;
		layer++;
	}
	for(y = dstY; y < dstY + height; y++) {
		for(x = dstX; x < dstX + width; x++) {
			Uint32 val = *src++;
			Uint32 metatile = val & MAPGRID_METATILE_TILES_ID_MASK;
			Uint32 column, row, mapMetatile, offset, bytes;

			*dst = (*dst & MAPGRID_ATTRIBUTE_MASK) | metatile;
			column = x & (MAPGRID_SCREEN_METATILES - 1);
			dst++;
			i = 0;
			row = y & (MAPGRID_SCREEN_METATILES - 1);
			pos = tile;
			/* offset is the metatile's place in a 32x32 BG map: 128 bytes a
			   metatile row, 4 a metatile. */
			mapMetatile = row << 5;
			mapMetatile += column;
			offset = mapMetatile * 4;
			/* The first layer whose visible area (16x12 metatiles) holds the
			   metatile has the metatile tiles written to its BG map too, and
			   only that one (a map lays its layers out in different parts of
			   the grid); the layers' BG maps follow each other, 2,048 bytes
			   apart, from screen block 5. */
			for(; i < 3; i++) {
				if(pos->x <= (Sint32)x &&
					pos->x + MAPGRID_VISIBLE_WIDTH > (Sint32)x &&
					pos->y <= (Sint32)y &&
					pos->y + MAPGRID_VISIBLE_HEIGHT > (Sint32)y) {
					Uint32 screenAddress, metatileTilesAddress;
					Uint8 *screenTile, *metatileTilesRows;

					/* The metatile's tiles (8 bytes): its top two, and its
					   bottom two 64 bytes further on. */
					screenAddress = (Uint32)BG_SCREEN_BLOCK(5);
					screenTile = (Uint8 *)screenAddress;
					screenTile += offset;
					bytes = metatile * 8;
					metatileTilesAddress = (Uint32)syRamMapBlocks;
					metatileTilesRows = (Uint8 *)metatileTilesAddress;
					metatileTilesRows += bytes;
					*(Uint32 *)screenTile = *(Uint32 *)metatileTilesRows;

					metatileTilesAddress = (Uint32)syRamMapBlocks + 4;
					metatileTilesRows = (Uint8 *)metatileTilesAddress;
					metatileTilesRows += bytes;
					screenAddress =
						(Uint32)BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;
					screenTile = (Uint8 *)screenAddress;
					screenTile += offset;
					*(Uint32 *)screenTile = *(Uint32 *)metatileTilesRows;
					break;
				}
				/* bytes, reused for the step to the next layer's BG map. */
				bytes = MAPGRID_LAYER_SCREEN_BYTES;
				offset += bytes;
				pos++;
			}
		}
		src += MAPGRID_SIZE - width;
		dst += MAPGRID_SIZE - width;
	}
}

/* Plays a map edit (MAP_GRID_METATILE_ANIMATION_STEP entries, ended by a source
   of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of metatile
   picture numbers to (destinationX, destinationY) and waits its delay in
   frames. */
void fdMapGridPlayMapEdit(
	Uint16 *steps, Sint32 destinationX, Uint32 destinationY)
{
	/* Each entry is read in turn: its source, then its signed y, width, height
	   and delay (MAP_GRID_METATILE_ANIMATION_STEP). */
	while(*steps != MAPGRID_ANIMATION_STEPS_END) {
		Uint16 source = *steps++;
		Sint32 sourceY = (Sint16)*steps++;
		Sint32 width = (Sint16)*steps++;
		Sint32 height = (Sint16)*steps++;
		Sint32 delay = (Sint16)*steps++;

		fdMapGridCopyMetatileTilesIndicesRect(source, (Uint16)sourceY,
			destinationX, destinationY, (Uint16)width, (Uint16)height);
		sySynch((Uint16)delay);
	}
}

/* Copying rectangles of metatiles from one place of the map to another. */

/* Copies a rectangle of width by height metatiles from (srcX, srcY) to (dstX,
   dstY), their pictures and their attributes both. */
void fdMapCopyMetatilesRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 dstX, Sint32 dstY)
{
	fdMapGridCopyMetatileTilesIndicesRect(
		srcX, srcY, dstX, dstY, width, height);
	fdMapCopyMetatileAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

/* Copies the attributes and shapes of a rectangle of metatiles from (srcX,
   srcY) to (dstX, dstY), width by height metatiles, keeping each destination's
   picture. */
void fdMapCopyMetatileAttributeRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 dstX, Sint32 dstY)
{
	Uint32 *metatiles = (Uint32 *)Ram_MapMetatileBuffer;
	Sint32	dst = dstY * MAPGRID_SIZE + dstX;
	Uint8  *dstShapes = MapShapeGrid + dst;
	Sint32	src = srcY * MAPGRID_SIZE + srcX;
	Uint8  *srcShapes = MapShapeGrid + src;
	Uint32 *dstMetatiles = metatiles + dst;
	Uint16	row;
	Uint16	col;

	metatiles += src;
	for(row = 0; row < height; row++) {
		Uint32 *dstMetatile = dstMetatiles + row * MAPGRID_SIZE;
		Uint32 *srcMetatile = metatiles + row * MAPGRID_SIZE;
		Uint8  *dstShape = dstShapes + row * MAPGRID_SIZE;
		Uint8  *srcShape = srcShapes + row * MAPGRID_SIZE;

		for(col = 0; col < width; col++) {
			*dstMetatile = (*dstMetatile & MAPGRID_METATILE_TILES_ID_MASK) |
				(*srcMetatile++ & MAPGRID_ATTRIBUTE_MASK);
			dstMetatile++;
			*dstShape = *srcShape;
			dstShape++;
			srcShape++;
		}
	}
}

/* Copies the drawn metatile tiles numbers (MAPGRID_DRAWN_PICTURE_MASK) of a
   rectangle of metatiles from (srcX, srcY) to (dstX, dstY), width by height
   metatiles, keeping each destination's bits above them, and redraws the copied
   metatiles that are on screen. fdMapGridCopyMetatileTilesIndicesRect's loop
   with the narrower mask and the size before the destination: the ROM keeps
   both. */
void fdMapGridCopyMetatileTilesMetatilesRect(Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY)
{
	Sint32	y;
	Uint32 *src =
		(Uint32 *)MapGridMetatileBuffer + (srcY * MAPGRID_SIZE + srcX);
	Uint32 *dst =
		&((Uint32 *)MapGridMetatileBuffer)[dstY * MAPGRID_SIZE + dstX];
	MAP_LAYER		  *layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	MAP_GRID_TILE_POS  views[3];
	Sint32			   i;
	MAP_GRID_TILE_POS *view = views;
	Sint32			   x;

	for(i = 0; i < 3; i++) {
		view->x = FIELD_METATILE(layer->x);
		view->y = FIELD_METATILE(layer->y);
		view++;
		layer++;
	}
	for(y = dstY; y < dstY + height; y++) {
		{
			for(x = dstX; x < dstX + width; x++) {
				Uint32 metatile = *src++;
				Uint32 metatileTilesId;
				Uint32 offset;
				Uint32 row;
				Uint32 n;
				Uint32 column;
				Uint32 scaled;
				Uint32 merged;

				metatileTilesId = metatile & MAPGRID_METATILE_TILES_ID_MASK;
				/* The metatile takes the source's drawn metatile tiles number
				   and keeps its own bits above. */
				merged = metatile & MAPGRID_DRAWN_PICTURE_MASK;
				merged |= *dst & ~MAPGRID_DRAWN_PICTURE_MASK;
				*dst = merged;
				dst++;
				view = views;
				column = x & (MAPGRID_SCREEN_METATILES - 1);
				i = 0;
				row = y & (MAPGRID_SCREEN_METATILES - 1);
				n = row << 5;
				n += column;
				offset = n * 4;
				for(; i < 3; i += 1) {
					if(view->x <= x && view->x + MAPGRID_VISIBLE_WIDTH > x &&
						view->y <= y && view->y + MAPGRID_VISIBLE_HEIGHT > y) {
						Uint32 screenAddress, pictureAddress;
						Uint8 *screenTile, *pictureRows;

						screenAddress = (Uint32)BG_SCREEN_BLOCK(5);
						screenTile = (Uint8 *)screenAddress;
						screenTile += offset;
						scaled = metatileTilesId * 8;
						pictureAddress = (Uint32)syRamMapBlocks;
						pictureRows = (Uint8 *)pictureAddress;
						pictureRows += scaled;
						*(Uint32 *)screenTile = *(Uint32 *)pictureRows;

						pictureAddress = (Uint32)syRamMapBlocks + 4;
						pictureRows = (Uint8 *)pictureAddress;
						pictureRows += scaled;
						screenAddress = (Uint32)BG_SCREEN_BLOCK(5) +
							MAPGRID_SCREEN_ROW_BYTES;
						screenTile = (Uint8 *)screenAddress;
						screenTile += offset;
						*(Uint32 *)screenTile = *(Uint32 *)pictureRows;
						break;
					}
					/* scaled, reused for the step to the next layer's BG
					   map. */
					scaled = MAPGRID_LAYER_SCREEN_BYTES;
					offset += scaled;
					view++;
				}
			}
			src += MAPGRID_SIZE - width;
			dst += MAPGRID_SIZE - width;
		}
	}
}

/* Copies a rectangle of map metatiles as they are, with their shape grid bytes,
   from (srcX, srcY) to (dstX, dstY), width by height metatiles. */
void fdMapCopyRawMetatilesRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 dstX, Sint32 dstY)
{
	Uint32 *metatiles = (Uint32 *)Ram_MapMetatileBuffer;
	Sint32	dst = (dstY * MAPGRID_SIZE) + dstX;
	Uint8  *dstShapes = MapShapeGrid + dst;
	Sint32	src = (srcY * MAPGRID_SIZE) + srcX;
	Uint8  *srcShapes = MapShapeGrid + src;
	Uint32 *dstMetatiles = metatiles + dst;
	Uint16	row;
	Uint16	col;

	metatiles += src;
	for(row = 0; row < height; row++) {
		Uint32 *dstMetatile = dstMetatiles + row * MAPGRID_SIZE;
		Uint32 *srcMetatile = metatiles + row * MAPGRID_SIZE;
		Uint8  *dstShape = dstShapes + row * MAPGRID_SIZE;
		Uint8  *srcShape = srcShapes + row * MAPGRID_SIZE;

		for(col = 0; col < width; col++) {
			*dstMetatile++ = *srcMetatile++;
			*dstShape++ = *srcShape++;
		}
	}
}

/* Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
   REG_DISPCNT) from layerFlags, keeping its other flags. */
void fdMapGridSetShownLayers(Uint32 layerFlags)
{
	MAP_STATE *state = Ram_HeapSlots->mapGrid;
	Uint32	   shown =
		layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
	Uint32 dispcnt = state->dispcnt;

	dispcnt = (dispcnt &
				  (Uint16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		shown;
	state->dispcnt = dispcnt;
}

/* A map edit's flag word: the flag in its low 12 bits, and a bit that makes it
   copy the metatiles' attributes rather than their pictures. */
#define MAP_EDIT_FLAG_MASK	0xfff
#define MAP_EDIT_ATTRIBUTES 0x1000
/* The flag word of all ones that ends a map's edits, the one that one more
   wraps to 0. */
#define MAP_EDITS_END		0xffff

/* Plays every edit of the map whose flag is set, as fdMapApplyFlagEdits does,
   MAP_EDIT_ATTRIBUTES in an edit's flag word choosing attributes over pictures:
   the map as the story has left it, as the player enters. */
void fdMapApplySetFlagEdits(void)
{
	Uint8 *cursor = MapSceneEditsOffset;
	Uint32 entry;
	Sint32 srcX, srcY, dstX, dstY, width, height;

	cursor = MapScene + (Uint16)fdMapReadEditU16(&cursor);
	for(;;) {
		entry = (Uint16)fdMapReadEditU16(&cursor);
		if(((entry + 1) & MAP_EDITS_END) == 0)
			return;
		if(syGameFlagGet(entry & MAP_EDIT_FLAG_MASK) != 0) {
			srcX = *cursor++;
			srcY = *cursor++;
			dstX = *cursor++;
			dstY = *cursor++;
			width = *cursor++;
			height = *cursor++;
			if(entry & MAP_EDIT_ATTRIBUTES)
				fdMapCopyMetatileAttributeRect(
					srcX, srcY, width, height, dstX, dstY);
			else
				fdMapGridCopyMetatileTilesIndicesRect(
					srcX, srcY, dstX, dstY, width, height);
		} else {
			cursor += 6;
		}
	}
}

/* Reads the little-endian halfword at *cursor, which may be unaligned, and
   moves the cursor past it. */
Uint32 fdMapReadEditU16(Uint8 **cursor)
{
	Uint32 low = *(*cursor)++;
	Uint32 high = *(*cursor)++;

	return low | (high << 8);
}

/* Sets flag (its low 12 bits) and plays its map edits on the metatiles'
   pictures. */
void fdMapSetFlagAndApplyEdits(Sint32 flag)
{
	flag &= MAP_EDIT_FLAG_MASK;
	syGameFlagSet(flag);
	fdMapApplyFlagEdits(flag, 0);
}

/* Sets flag (its low 12 bits) and plays its map edits on the metatiles'
   attributes. */
void fdMapSetFlagAndApplyAttributeEdits(Sint32 flag)
{
	flag &= MAP_EDIT_FLAG_MASK;
	syGameFlagSet(flag);
	fdMapApplyFlagEdits(flag, 1);
}

/* Plays the map's edits for flag: each copies a rectangle of metatiles (their
   pictures, or with attributes their attributes and shapes) from one place of
   the map to another. The map's extra data lists the edits, each a flag then
   source x and y, destination x and y, width and height in bytes, ended by a
   flag halfword of all ones. */
void fdMapApplyFlagEdits(Sint32 flag, Sint32 attributes)
{
	Uint8 *cursor = MapSceneEditsOffset;
	Uint32 entry;
	Sint32 srcX, srcY, dstX, dstY, width, height;

	cursor = MapScene + (Uint16)fdMapReadEditU16(&cursor);
	for(;;) {
		entry = (Uint16)fdMapReadEditU16(&cursor);
		if(((entry + 1) & MAP_EDITS_END) == 0)
			return;
		if((entry & MAP_EDIT_FLAG_MASK) == flag) {
			srcX = *cursor++;
			srcY = *cursor++;
			dstX = *cursor++;
			dstY = *cursor++;
			width = *cursor++;
			height = *cursor++;
			if(attributes != 0)
				fdMapCopyMetatileAttributeRect(
					srcX, srcY, width, height, dstX, dstY);
			else
				fdMapGridCopyMetatileTilesIndicesRect(
					srcX, srcY, dstX, dstY, width, height);
		} else {
			cursor += 6;
		}
	}
}

/* Reads map's camera bounds from its header, in 8-pixel tiles: bounds[0] its
   left, bounds[1] its right, bounds[2] its top and bounds[3] its bottom, a
   right or bottom of 0 standing for the map's whole MAPGRID_TILES. */
void fdMapGetCameraBounds(Sint32 map, Sint32 *bounds)
{
	MAP_HEADER *header;

	header = MAPGRID_RESOURCE((MapsWork + map)->header);
	bounds[0] = header->cameraBounds[0];
	if(header->cameraBounds[2] == 0)
		bounds[1] = MAPGRID_TILES;
	else
		bounds[1] = header->cameraBounds[2];
	bounds[2] = header->cameraBounds[1];
	if(header->cameraBounds[3] == 0)
		bounds[3] = MAPGRID_TILES;
	else
		bounds[3] = header->cameraBounds[3];
}

/* Decodes the current map's metatiles into the map metatile buffer and converts
   them, then decodes its metatile shapes into MapShapeGrid. */
void fdMapLoadMetatilesAndShapes(void)
{
	Uint8 *source;
	/* MapsWork through a pointer to its rows. */
	MAP_HEADER *header = MAPGRID_RESOURCE(
		((const MAP_RESOURCES *)MapsWork)[MainRoutineStateWork.mapId].header);

	source = (Uint8 *)header + header->metatiles;
	syDecompressTaggedLZ(source, Ram_MapMetatileBuffer);
	fdTileMapConvertBuffer();
	source = (Uint8 *)header + header->shapes;
	syDecompressTaggedLZ(source, MapShapeGrid);
}

/* Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's metatile
   picture buffer, decompressing it from the area's graphics, and with update
   set also draws it into the layer's BG map. */
Sint32 fdWorldMapWriteLayerMetatileTile(
	Sint32 layer, Sint32 x, Sint32 y, Sint32 tile, Sint32 update)
{
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	Uint32		   *tileOffsets = perspective->tiles;
	Uint16		   *shownQuarter;
	Uint16		   *decodedTile;
	Uint16		   *sourceTiles;
	Uint8		   *quarterMetatiles;
	Uint16		   *screenTiles;
	Uint32			rowIndex;
	Uint32			columnIndex;
	Uint32			metatileTilesId;

	x &= 1;
	y &= 1;
	shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
	if(update == 0 && tile == *shownQuarter)
		return 0;
	*shownQuarter = tile;
	decodedTile = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH,
		WORLD_MAP_TILE_METATILES * WORLD_MAP_TILE_METATILES *
			sizeof(WORLD_MAP_METATILE));
	if(tileOffsets[tile] != 0)
		syDecompressByteLZ(
			(Uint8 *)tileOffsets + tileOffsets[tile], decodedTile);
	else
		syWordsClearEntry(decodedTile,
			WORLD_MAP_TILE_METATILES * WORLD_MAP_TILE_METATILES *
				sizeof(WORLD_MAP_METATILE));

	sourceTiles = decodedTile;
	quarterMetatiles = syRamMapBlocks + (((layer * 2 + y) * 32 + x) << 6);
	for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_METATILES; rowIndex++) {
		syDMAXCopy(sourceTiles, quarterMetatiles,
			DMA_COPY_WORDS(WORLD_MAP_TILE_METATILES), REG_DMA3);
		sourceTiles +=
			WORLD_MAP_TILE_METATILES * sizeof(WORLD_MAP_METATILE) / 2;
		quarterMetatiles +=
			WORLD_MAP_VIEW_METATILES * sizeof(WORLD_MAP_METATILE);
	}

	if(update != 0) {
		screenTiles = (Uint16 *)((Uint32)WORLD_MAP_LOWER_TILE_MAP +
			((((layer * 2 + y) << 6) + x) << 5));
		sourceTiles = decodedTile;
		for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_METATILES; rowIndex++) {
			for(columnIndex = 0; columnIndex < WORLD_MAP_TILE_METATILES;
				columnIndex++) {
				Uint16 *bottomTiles;

				metatileTilesId = *sourceTiles;
				screenTiles[0] =
					*(Uint16 *)(Ram_MapMetatileBuffer + metatileTilesId * 4);
				bottomTiles = (Uint16 *)(Ram_MapMetatileBuffer + 2);
				bottomTiles =
					(Uint16 *)((Uint8 *)bottomTiles + metatileTilesId * 4);
				screenTiles[WORLD_MAP_TILE_MAP_WIDTH / 2] = *bottomTiles;
				screenTiles++;
				sourceTiles += sizeof(WORLD_MAP_METATILE) / 2;
			}
			/* Down two rows of the tile map, of WORLD_MAP_TILE_MAP_WIDTH / 2
			   halfwords each, less the halfwords just written. */
			screenTiles +=
				2 * (WORLD_MAP_TILE_MAP_WIDTH / 2) - WORLD_MAP_TILE_METATILES;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return 1;
}

#undef WORLD_MAP_TILE_METATILES
