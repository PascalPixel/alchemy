/* @file */
/* Layered maps: decoding their tilemaps and palette, loading, drawing and */
/* scrolling, copying cells, and the edits flags apply. */
/* Part of field.c. */

/* A cell word: its cell tiles number in the low 12 bits, the rest its */
/* attributes. */
#define MAPGRID_CELL_TILES_ID_MASK 0x00000fff /* the cell tiles number */
#define MAPGRID_ATTRIBUTE_MASK	   0xfffff000 /* the cell's attributes */
/* The cell tiles number the renderer draws, the low 11 bits: the copies */
/* that use MAPGRID_CELL_TILES_ID_MASK carry the bit above it as well. */
#define MAPGRID_DRAWN_PICTURE_MASK 0x7ff
/* The shift that leaves a cell word's drawn picture number on top, the bits */
/* above it shifted out. Shifted back down by one less it is the number times */
/* 2, the picture's offset in words in syRamMapBlocks (a word for each of its */
/* two rows of 2 tiles); by two less, times 4, its offset in halfwords. */
#define MAPGRID_DRAWN_PICTURE_SHIFT 21
/* The map's 128 by 128 cell words. */
#define MAPGRID_CELLS ((u32 *)Ram_MapGridCellBuffer)
/* A layer's visible area in cells: the screen's 15 by 10, and the row and */
/* column partly scrolled in. */
#define MAPGRID_VISIBLE_WIDTH  16
#define MAPGRID_VISIBLE_HEIGHT 12
/* A layer's BG map in cells, 16 by 16 (32 by 32 tiles), and the bytes of one */
/* row of its tiles. */
#define MAPGRID_SCREEN_CELLS	 16
#define MAPGRID_SCREEN_ROW_BYTES 64
/* A cell's top tile row or left tile column in a layer's BG map, even, */
/* wrapping at the map's 32; its row as tile entries from the map's start. */
#define MAPGRID_SCREEN_TILE_MASK (BG_SCREEN_WIDTH - 2)
#define MAPGRID_SCREEN_ROW_MASK	 (MAPGRID_SCREEN_TILE_MASK * BG_SCREEN_WIDTH)
/* The 8-pixel tile a 16.16 layer coordinate lies in, as FIELD_CELL the */
/* 16-pixel cell, and a map's width and height in those tiles. */
#define MAPGRID_TILE(coordinate) ((u32)(coordinate) >> 19)
#define MAPGRID_TILES			 (MAPGRID_SIZE * MAPGRID_CELL_PIXELS / 8)
/* A map's palette: the first 14 of the 16 BG palettes, 224 colours. */
#define MAP_PALETTE_BYTES (14 * 16 * sizeof(u16))
/* The cell shapes' buffer, four bytes a shape, cleared to -1 before a map's */
/* shapes are decoded into it; the map's script is decoded after it. */
#define MAPGRID_SHAPE_TABLE_BYTES 8192
/* A world map tile's 16 by 16 blocks, as fdWorldMapWriteLayerCellTile decodes
 */
/* one. */
#define WORLD_MAP_TILE_CELLS 16

/* The world map's two tile maps, of 64 by 64 one-byte tiles: BG3's, the lower
 */
/* layer, and BG2's, the upper, right after it (fdWorldMapEndStillImage). */
#define WORLD_MAP_LOWER_TILE_MAP BG_SCREEN_BLOCK(8)
#define WORLD_MAP_UPPER_TILE_MAP BG_SCREEN_BLOCK(10)
#define WORLD_MAP_TILE_MAP_WIDTH 64
/* The world map cells in view (syRamMapBlocks): WORLD_MAP_VIEW_CELLS square */
/* for the lower layer, then as many for the upper, each cell 2 by 2 tiles of */
/* its tile map. */
#define WORLD_MAP_VIEW_LAYER_BYTES                                             \
	(WORLD_MAP_VIEW_CELLS * WORLD_MAP_VIEW_CELLS * sizeof(WORLD_MAP_CELL))

/* Remaps byteCount bytes in place through SpriteColorGreyTable. */
void fdSprColRemapPixels(u8 *bytes, s32 byteCount) {
	u8 *remapTable = (u8 *)SpriteColorGreyTable;

	while(byteCount-- != 0) {
		*bytes = remapTable[*bytes];
		bytes++;
	}
}

/* Unpacks a map's cell tiles, decoded into the map cell buffer from its */
/* second byte, into CellTiles. The buffer's second byte is how they are */
/* packed (CELL_TILES_PACKING) and the halfwords follow it; size counts */
/* from that byte, and an even size leaves nothing to unpack. */
void fdTileMapDecodeStagedBuffer(s32 size) {
	u16 *dst;
	u16 *src;
	s32 n;
	s32 i;

	src = (u16 *)(Ram_MapCellBuffer + 2);
	dst = (u16 *)CellTiles;
	n = (size - 1) / 2;
	if(size & 1) {
		switch(((u8 *)MapGridCellBuffer)[1]) {
		case CELL_TILES_PACKING_RAW:
			for(i = 0; i < n; i++)
				*dst++ = *src++;
			break;
		case CELL_TILES_PACKING_SPLIT_DELTA: {
			u32 prev = 0;
			u8 *hi = Ram_MapCellBuffer + 2;
			u8 *lo = hi + n;

			for(i = 0; i < n; i++) {
				u32 v = ((*hi++ << 8) | *lo++) ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		case CELL_TILES_PACKING_DELTA: {
			s32 prev = 0;

			for(i = 0; i < n; i++) {
				s32 v = *src++ ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		}
	}
}

/* A map's cells as they are decoded: two planes of a byte a cell, the cells' */
/* attribute bytes, then a plane of their cell tiles numbers, a halfword a */
/* cell. */
#define MAPGRID_BYTE_PLANES_BYTES (2 * MAPGRID_SIZE * MAPGRID_SIZE)

/* Joins a decoded map's planes in place into its cell words, with the ARM */
/* function copied to RAM, which reads the byte planes from a copy. */
void fdTileMapConvertBuffer(void) {
	u8 *saved;
	CONVERT_MAP_ROUTINE routine;
	u32 size;

	size = MAPGRID_BYTE_PLANES_BYTES;
	saved = syHeapAllocEwram(size);
	syWordsCopy(saved, Ram_MapCellBuffer, size);
	syRamCodeLoad(routine, fdRenderConvertMapCode, Render_ConvertMapCodeSize);
	routine(((u8 *)MapGridCellBuffer + MAPGRID_BYTE_PLANES_BYTES),
			Ram_MapCellBuffer, saved);
	syHeapRelease(routine);
	syHeapRelease(saved);
}

/* A layout's resource, counted in MapsWork from the empty map. */
#define MAPGRID_RESOURCE(offset)                                               \
	syResourceGetTableEntry((offset) + RESOURCE_EMPTY_MAP)

/* Decodes the palette resource of a map's header row (its second halfword) */
/* and copies its 224 colors to destination, keeping the current backdrop */
/* color in the first entry. */
void fdMapCopyPalette(s32 map, void *destination) {
	const MAP_RESOURCES *entry;
	u16 *buffer;
	s16 backdrop;

	entry = &MapsWork[map];
	buffer = syHeapAlloc(BG_PLTT_SIZE);
	backdrop = *(s16 *)BG_PLTT;
	syDecompressTaggedLz(MAPGRID_RESOURCE(entry->pal), buffer);
	buffer[0] = backdrop;
	syDMAXCopy(buffer, destination, DMA_COPY_WORDS(MAP_PALETTE_BYTES / 4),
			   REG_DMA3);
	syHeapRelease(buffer);
}

/* Loads layout index from MapsWork: its cell tiles, collision (cleared to -1 */
/* first), cells, shapes, tileset animations, blend script and script; sets up
 */
/* the three background layers' origins, parallax and wrap from the layout */
/* header; loads the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS
 */
/* says they are loaded; and starts the layer scrolling */
/* (fdMapGridUpdateLayerScroll). Returns 2, as The Broken Seal's does; no */
/* caller reads it. */
s32 fdMapGridLoadMap(s32 index) {
	MAP_RESOURCES *sceneResources;
	MAP_STATE *map;
	MAP_HEADER *sceneHeader;
	MAP_LAYER *layer;
	MAP_HEADER_LAYER *layerSource;
	s32 *viewX;
	s32 *viewY;
	s32 layerIndex;
	u8 *graphicsBuffer;
	s32 backdrop;
	u8 *encodedData;
	s32 displayFlags;

	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
					 DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScrFadeFromBlack(0);
	sceneResources = (MAP_RESOURCES *)&MapsWork[index];
	/* A field map's state ends after its three layers; the world map's goes on.
	 */
	map = syHeapSlotAllocEwram(HEAP_SLOT_MAP_GRID, MAPGRID_FIELD_STATE_SIZE);
	syWordsClearEntry(map, MAPGRID_FIELD_STATE_SIZE);
	sceneHeader = (MAP_HEADER *)MAPGRID_RESOURCE(sceneResources->header);
	encodedData = (u8 *)sceneHeader + sceneHeader->cellTiles;
	fdTileMapDecodeStagedBuffer(
		syDecompressTaggedLz(encodedData, Ram_MapCellBuffer + 1));
	btDmaFill32(-1, Ram_MapCollision, MAPGRID_SHAPE_TABLE_BYTES);
	encodedData = (u8 *)sceneHeader + sceneHeader->collision;
	syDecompressTaggedLz(encodedData, Ram_MapCollision);
	encodedData = (u8 *)sceneHeader + sceneHeader->cells;
	syDecompressTaggedLz(encodedData, Ram_MapCellBuffer);
	fdTileMapConvertBuffer();
	encodedData = (u8 *)sceneHeader + sceneHeader->shapes;
	syDecompressTaggedLz(encodedData, MapShapeGrid);
	if(sceneHeader->anm != 0) {
		encodedData = (u8 *)sceneHeader + sceneHeader->anm;
		syDecompressTaggedLz(encodedData, MAPGRID_ANIMATION_SCRIPT);
		fdTilesetAnimStartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
	}
	if(sceneHeader->blend != 0) {
		encodedData = (u8 *)sceneHeader + sceneHeader->blend;
		syDecompressTaggedLz(encodedData, MAPGRID_BLEND_SCRIPT);
		fdBlendScriptStart((u16 *)MAPGRID_BLEND_SCRIPT);
	}
	encodedData = (u8 *)sceneHeader + sceneHeader->script;
	syDecompressTaggedLz(encodedData,
						 Ram_MapCollision + MAPGRID_SHAPE_TABLE_BYTES);
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
		map->maxX = toFIXED(MAPGRID_SIZE * MAPGRID_CELL_PIXELS);
	if(map->maxZ == 0)
		map->maxZ = toFIXED(MAPGRID_SIZE * MAPGRID_CELL_PIXELS);
	layer = map->layers;
	layerSource = sceneHeader->layers;
	for(layerIndex = 0; layerIndex < 3; layerIndex++) {
		u32 x = layerSource->x;
		u32 y = layerSource->y;
		s32 scaleX;
		s32 cell;
		u8 *shapeGrid;
		u32 maskY;

		layer->ofsX = toFIXED(x * 8);
		layer->ofsY = toFIXED(y * 8);
		/* Parallax and drift are signed sixteenths, made 16.16. */
		layer->scaleY = layerSource->scrollY * (FIXED_ONE / 16);
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
		scaleX = layerSource->scrollX * (FIXED_ONE / 16);
		cell = y + x;
		shapeGrid = MapShapeGrid;
		layer->cells = (MAP_CELL *)Ram_MapCellBuffer + cell;
		layer->scaleX = scaleX;
		layer->originX = x;
		layer->terrainIds = shapeGrid + cell;
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
	syIoWrite16(map->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
					BGCNT_SCREEN_BASE(5),
				&REG_BG3CNT);
	syIoWrite16(map->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
					BGCNT_SCREEN_BASE(6),
				&REG_BG2CNT);
	syIoWrite16(map->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
					BGCNT_SCREEN_BASE(7),
				&REG_BG1CNT);
	if(syGameFlagGet(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
		syGameFlagClear(FLAG_KEEP_LAYER_GRAPHICS);
	}
	else {
		graphicsBuffer = syHeapAlloc(BG_CHAR_SIZE);
		if(graphicsBuffer != NULL) {
			backdrop = *(s16 *)BG_PLTT;
			syDecompressTaggedLz(MAPGRID_RESOURCE(sceneResources->pal),
								 graphicsBuffer);
			*(s16 *)graphicsBuffer = backdrop;
			syWordsCopyEntry((void *)BG_PLTT, graphicsBuffer,
							 MAP_PALETTE_BYTES);
			syDecompressMoveToFrontLz(
				MAPGRID_RESOURCE(sceneResources->tiles[0]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(1), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLz(
				MAPGRID_RESOURCE(sceneResources->tiles[1]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(2), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLz(
				MAPGRID_RESOURCE(sceneResources->tiles[2]), graphicsBuffer);
			syWordsCopyEntry(BG_CHAR_BLOCK(3), graphicsBuffer, BG_CHAR_SIZE);
			syDecompressMoveToFrontLz(
				MAPGRID_RESOURCE(sceneResources->anmTiles), MapGridAnimTiles);
			syHeapRelease(graphicsBuffer);
		}
	}
	displayFlags = 0;
	REG_MOSAIC = displayFlags;
	REG_BLDCNT = displayFlags;
	displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	syIoWrite16(displayFlags, &REG_DISPCNT);
	sySetEvent(fdMapGridUpdateLayerScroll, TASK_PASS_FRAME + 5);
	return 2;
}

/* Centres the camera on the point the map follows (origin: x, height, z), or */
/* on (0, 0) when there is none, and scrolls the layers there. */
void fdMapGridCenterOnFollowedPoint(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 x = 0;
	s32 height = 0;
	s32 z = 0;
	s32 *followedPosition = map->origin;

	if(followedPosition != NULL) {
		const s16 *rectList;
		const s16 *rect;

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

/* Draws the cell tiles of a row of 16 of a layer's cells (32 tiles) into its */
/* BG map (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
 */
/* wraps every 32 tiles and the cells wrap at the layer's window, which The */
/* Broken Seal's function does not have. Called by */
/* fdMapGridUpdateLayerScroll when a layer scrolls onto a new row of cell */
/* pictures. */
void fdMapGridRenderCellTilesRow(u32 layer, s32 tileX, s32 tileY) {
	u32 *cells = (u32 *)Ram_MapCellBuffer;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *window = &map->layers[layer];
	u32 originY = window->originY;
	u8 *screenMap =
		(u8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	u32 maskY = window->maskY;
	u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	u32 screenRowOffset = (tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	u32 sourceColumn;
	u32 screenColumnOffset;
	u32 maskX;
	u32 originX;
	u32 cellTilesIndex;

	/* A window narrower than the 128 by 128 cells starts at its origin. */
	if(maskY != MAPGRID_ROW_MASK) {
		sourceRow = (sourceRow - originY) & maskY;
		cells += originY;
	}
	maskX = window->maskX;
	sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	originX = window->originX;
	screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	sourceColumn &= maskX;
	if(maskX != MAPGRID_SIZE - 1) {
		sourceColumn = (sourceColumn - originX) & maskX;
		cells += originX;
	}
	for(cellTilesIndex = 0; cellTilesIndex < MAPGRID_SCREEN_CELLS;
		cellTilesIndex++) {
		u32 *sourceCell;
		u32 *cellTilesRows;
		u8 *screenTile;
		u32 cellTilesWordOffset;

		sourceCell = cells + sourceRow + sourceColumn;
		cellTilesWordOffset = (*sourceCell << MAPGRID_DRAWN_PICTURE_SHIFT) >>
							  (MAPGRID_DRAWN_PICTURE_SHIFT - 1);
		cellTilesRows = (u32 *)syRamMapBlocks;
		cellTilesRows += cellTilesWordOffset;
		screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
		*(u32 *)screenTile = *cellTilesRows;
		cellTilesRows = (u32 *)(syRamMapBlocks + 4);
		cellTilesRows += cellTilesWordOffset;
		sourceColumn++;
		*(u32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *cellTilesRows;
		sourceColumn &= maskX;
		screenColumnOffset =
			(screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
	}
}

/* Copies one column (tileHalf 0 or 1) of a cell's 2 by 2 picture tiles into */
/* the screen map at the row and column offsets. */
static __inline__ void CopyCellTilesHalfColumn(u32 *cell, u8 *screenMap,
											   u32 screenRowOffset,
											   u32 screenColumnOffset,
											   u32 tileHalf) {
	u16 *cellTilesEntry;
	u8 *screenTile;
	u32 index = ((*cell << MAPGRID_DRAWN_PICTURE_SHIFT) >>
				 (MAPGRID_DRAWN_PICTURE_SHIFT - 2)) +
				tileHalf;

	cellTilesEntry = (u16 *)syRamMapBlocks;
	cellTilesEntry += index;
	screenTile =
		screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
	*(u16 *)screenTile = *cellTilesEntry;
	cellTilesEntry = (u16 *)(syRamMapBlocks + 4);
	cellTilesEntry += index;
	*(u16 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *cellTilesEntry;
}

/* Draws the cell tiles of a column of 11 of a layer's cells, one tile wide, */
/* into its BG map from tile (tileX, tileY), as The Broken Seal's function of */
/* the same name; tileX's low bit picks the cell tiles' left or right half. */
void fdMapGridRenderCellTilesColumn(u32 layer, s32 tileX, s32 tileY) {
	u32 *cells = (u32 *)Ram_MapCellBuffer;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *window = &map->layers[layer];
	u8 *screenMap =
		(u8 *)BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	u32 maskY = window->maskY;
	u32 originY = window->originY;
	u32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	u32 screenRowOffset = (tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	u32 sourceColumn;
	u32 screenColumnOffset;
	u32 maskX;
	u32 originX;
	u32 tileHalf;
	u32 cellTilesIndex;

	if(maskY != MAPGRID_ROW_MASK) {
		sourceRow = (sourceRow - originY) & maskY;
		cells = (u32 *)Ram_MapCellBuffer + originY;
	}
	maskX = window->maskX;
	originX = window->originX;
	sourceColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	sourceColumn &= maskX;
	if(maskX != MAPGRID_SIZE - 1) {
		sourceColumn = (sourceColumn - originX) & maskX;
		cells += originX;
	}
	tileHalf = tileX & 1;
	for(cellTilesIndex = 0;
		cellTilesIndex <= DISPLAY_HEIGHT / MAPGRID_CELL_PIXELS;
		cellTilesIndex++) {
		CopyCellTilesHalfColumn(cells + sourceRow + sourceColumn, screenMap,
								screenRowOffset, screenColumnOffset, tileHalf);
		sourceRow = (sourceRow + MAPGRID_SIZE) & maskY;
		screenRowOffset =
			(screenRowOffset + 2 * BG_SCREEN_WIDTH) & MAPGRID_SCREEN_ROW_MASK;
	}
}

/* Each frame on a layered map: centres the view on the point the map follows */
/* (origin), clamped to the map's bounds and shaken by any decaying screen */
/* shake; then scrolls each of the three layers by its parallax scale and */
/* automatic scroll (wrapping at its period), draws the cell tiles column or */
/* row that scrolls into view, and sets its BG scroll. */
void fdMapGridUpdateLayerScroll(void) {
	MAP_STATE *map;
	MAP_LAYER *layer;
	s32 *origin;
	s32 x, y;
	s32 minX, minY, maxX, maxY;
	s32 baseX, baseY;
	u32 layerIndex;

	map = Ram_HeapSlots->mapGrid;
	origin = map->origin;
	layer = map->layers;
	if(origin == 0)
		return;
	baseX = *origin++ - FIELD_VIEW_FOCUS_X;
	{
		s32 height = *origin++;

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
		s32 shakeRoll = syRandom();

		shakeRoll -= syRandom();
		baseX += syMulFX(map->shakeX, shakeRoll);
		map->shakeX = syMulFX(map->shakeX, map->shakeDecay);
	}
	if(map->shakeY != 0) {
		s32 shakeRoll = syRandom();

		shakeRoll -= syRandom();
		baseY += syMulFX(map->shakeY, shakeRoll);
		map->shakeY = syMulFX(map->shakeY, map->shakeDecay);
	}
	{
		s32 step = map->view[1] - baseY;

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
				fdMapGridRenderCellTilesColumn(layerIndex,
											   x + DISPLAY_WIDTH / 8, y);
			else
				fdMapGridRenderCellTilesColumn(layerIndex, x, y);
		}
		if((layer->y ^ baseY) & toFIXED(16)) {
			if(layer->y < baseY)
				fdMapGridRenderCellTilesRow(layerIndex, x,
											y + DISPLAY_HEIGHT / 8);
			else
				fdMapGridRenderCellTilesRow(layerIndex, x, y);
		}
		layer->x = baseX;
		layer->y = baseY;
		BgScrollWork[3 - layerIndex].x = FIXEDtoINT(baseX);
		BgScrollWork[3 - layerIndex].y = FIXEDtoINT(baseY);
	}
}

/* Copies a cell's 2 by 2 picture tiles into the screen map at the row and */
/* column offsets. */
static __inline__ void CopyCellTilesToScreen(u32 *cell, u8 *screenMap,
											 u32 screenRowOffset,
											 u32 screenColumnOffset) {
	u32 *cellTilesRows;
	u8 *screenTile;
	u32 index = (*cell << MAPGRID_DRAWN_PICTURE_SHIFT) >>
				(MAPGRID_DRAWN_PICTURE_SHIFT - 1);

	cellTilesRows = (u32 *)syRamMapBlocks;
	cellTilesRows += index;
	screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
	*(u32 *)screenTile = *cellTilesRows;
	cellTilesRows = (u32 *)(syRamMapBlocks + 4);
	cellTilesRows += index;
	*(u32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *cellTilesRows;
}

/* Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
 */
/* redraws every enabled layer's BG map around it. */
void fdMapGridSetCameraCenter(FIXED x, FIXED y) {
	u32 layerIndex;
	u8 *screenMap;
	u32 *cells;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer = map->layers;
	u32 originX;

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
			u32 maskX, maskY, originY;
			u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
				cellTilesRow, cellTilesColumn;

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
			screenMap = (u8 *)BG_SCREEN_BLOCK(5) +
						layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
			maskY = layer->maskY;
			maskX = layer->maskX;
			originY = layer->originY;
			originX = layer->originX;
			cells = (u32 *)Ram_MapCellBuffer;
			if(maskX != MAPGRID_SIZE - 1)
				cells = (u32 *)Ram_MapCellBuffer + originX;
			if(maskY != MAPGRID_ROW_MASK)
				cells += originY;
			sourceRow = ((y / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
			screenRowOffset = (y & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
			if(maskY != MAPGRID_ROW_MASK)
				sourceRow = (sourceRow - originY) & maskY;
			for(cellTilesRow = 0;
				cellTilesRow <= DISPLAY_HEIGHT / MAPGRID_CELL_PIXELS;
				cellTilesRow++) {
				sourceColumn = (x / 2) & (MAPGRID_SIZE - 1);
				screenColumnOffset = x & MAPGRID_SCREEN_TILE_MASK;
				if(maskX != MAPGRID_SIZE - 1)
					sourceColumn = (sourceColumn - originX) & maskX;
				for(cellTilesColumn = 0; cellTilesColumn < MAPGRID_SCREEN_CELLS;
					cellTilesColumn++) {
					CopyCellTilesToScreen(cells + sourceRow + sourceColumn,
										  screenMap, screenRowOffset,
										  screenColumnOffset);
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

/* Copies the cell tiles numbers of a rectangle of cells from (srcX, srcY) to */
/* (dstX, dstY), width by height cells, keeping each destination's attributes,
 */
/* and redraws the copied cells that are on screen. */
void fdMapGridCopyCellTilesIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
									   u32 width, u32 height) {
	u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * MAPGRID_SIZE + srcX);
	u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * MAPGRID_SIZE + dstX);
	MAP_LAYER *layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	MAP_GRID_TILE_POS tile[3];
	s32 i;
	MAP_GRID_TILE_POS *pos = tile;
	u32 y;
	u32 x;

	/* The three layers' visible areas: their scroll positions in cells. */
	for(i = 0; i < 3; i++) {
		pos->x = FIELD_CELL(layer->x);
		pos->y = FIELD_CELL(layer->y);
		pos++;
		layer++;
	}
	for(y = dstY; y < dstY + height; y++) {
		for(x = dstX; x < dstX + width; x++) {
			u32 val = *src++;
			u32 cell = val & MAPGRID_CELL_TILES_ID_MASK;
			u32 column, row, mapCell, offset, bytes;

			*dst = (*dst & MAPGRID_ATTRIBUTE_MASK) | cell;
			column = x & (MAPGRID_SCREEN_CELLS - 1);
			dst++;
			i = 0;
			row = y & (MAPGRID_SCREEN_CELLS - 1);
			pos = tile;
			/* offset is the cell's place in a 32x32 BG map: 128 bytes a cell
			 * row, */
			/* 4 a cell. */
			mapCell = row << 5;
			mapCell += column;
			offset = mapCell * 4;
			/* The first layer whose visible area (16x12 cells) holds the cell
			 * has */
			/* the cell tiles written to its BG map too, and only that one (a
			 * map */
			/* lays its layers out in different parts of the grid); the layers'
			 * BG */
			/* maps follow each other, 2,048 bytes apart, from screen block 5.
			 */
			for(; i < 3; i++) {
				if(pos->x <= (s32)x &&
				   pos->x + MAPGRID_VISIBLE_WIDTH > (s32)x &&
				   pos->y <= (s32)y &&
				   pos->y + MAPGRID_VISIBLE_HEIGHT > (s32)y) {
					u32 screenAddress, cellTilesAddress;
					u8 *screenTile, *cellTilesRows;

					/* The cell's tiles (8 bytes): its top two, and its bottom
					 * two */
					/* 64 bytes further on. */
					screenAddress = (u32)BG_SCREEN_BLOCK(5);
					screenTile = (u8 *)screenAddress;
					screenTile += offset;
					bytes = cell * 8;
					cellTilesAddress = (u32)syRamMapBlocks;
					cellTilesRows = (u8 *)cellTilesAddress;
					cellTilesRows += bytes;
					*(u32 *)screenTile = *(u32 *)cellTilesRows;

					cellTilesAddress = (u32)syRamMapBlocks + 4;
					cellTilesRows = (u8 *)cellTilesAddress;
					cellTilesRows += bytes;
					screenAddress =
						(u32)BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;
					screenTile = (u8 *)screenAddress;
					screenTile += offset;
					*(u32 *)screenTile = *(u32 *)cellTilesRows;
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

/* Plays a map edit (MAP_GRID_CELL_ANIMATION_STEP entries, ended by a source
 */
/* of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of cell */
/* picture numbers to (destinationX, destinationY) and waits its delay in */
/* frames. */
void fdMapGridPlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY) {
	/* Each entry is read in turn: its source, then its signed y, width, height
	 */
	/* and delay (MAP_GRID_CELL_ANIMATION_STEP). */
	while(*steps != MAPGRID_ANIMATION_STEPS_END) {
		u16 source = *steps++;
		s32 sourceY = (s16)*steps++;
		s32 width = (s16)*steps++;
		s32 height = (s16)*steps++;
		s32 delay = (s16)*steps++;

		fdMapGridCopyCellTilesIndicesRect(source, (u16)sourceY, destinationX,
										  destinationY, (u16)width,
										  (u16)height);
		sySynch((u16)delay);
	}
}

/* Copying rectangles of cells from one place of the map to another. */

/* Copies a rectangle of width by height cells from (srcX, srcY) to (dstX, */
/* dstY), their pictures and their attributes both. */
void fdMapCopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
						s32 dstY) {
	fdMapGridCopyCellTilesIndicesRect(srcX, srcY, dstX, dstY, width, height);
	fdMapCopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

/* Copies the attributes and shapes of a rectangle of cells from (srcX, srcY) */
/* to (dstX, dstY), width by height cells, keeping each destination's */
/* picture. */
void fdMapCopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
								s32 dstX, s32 dstY) {
	u32 *cells = (u32 *)Ram_MapCellBuffer;
	s32 dst = dstY * MAPGRID_SIZE + dstX;
	u8 *dstShapes = MapShapeGrid + dst;
	s32 src = srcY * MAPGRID_SIZE + srcX;
	u8 *srcShapes = MapShapeGrid + src;
	u32 *dstCells = cells + dst;
	u16 row;
	u16 col;

	cells += src;
	for(row = 0; row < height; row++) {
		u32 *dstCell = dstCells + row * MAPGRID_SIZE;
		u32 *srcCell = cells + row * MAPGRID_SIZE;
		u8 *dstShape = dstShapes + row * MAPGRID_SIZE;
		u8 *srcShape = srcShapes + row * MAPGRID_SIZE;

		for(col = 0; col < width; col++) {
			*dstCell = (*dstCell & MAPGRID_CELL_TILES_ID_MASK) |
					   (*srcCell++ & MAPGRID_ATTRIBUTE_MASK);
			dstCell++;
			*dstShape = *srcShape;
			dstShape++;
			srcShape++;
		}
	}
}

/* Copies the drawn cell tiles numbers (MAPGRID_DRAWN_PICTURE_MASK) of a */
/* rectangle of cells from (srcX, srcY) to (dstX, dstY), width by height */
/* cells, keeping each destination's bits above them, and redraws the copied */
/* cells that are on screen. fdMapGridCopyCellTilesIndicesRect's loop with the
 */
/* narrower mask and the size before the destination: the ROM keeps both. */
void fdMapGridCopyCellTilesCellsRect(s32 srcX, s32 srcY, s32 width, s32 height,
									 s32 dstX, s32 dstY) {
	s32 y;
	u32 *src = (u32 *)MapGridCellBuffer + (srcY * MAPGRID_SIZE + srcX);
	u32 *dst = &((u32 *)MapGridCellBuffer)[dstY * MAPGRID_SIZE + dstX];
	MAP_LAYER *layer = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers;
	MAP_GRID_TILE_POS views[3];
	s32 i;
	MAP_GRID_TILE_POS *view = views;
	s32 x;

	for(i = 0; i < 3; i++) {
		view->x = FIELD_CELL(layer->x);
		view->y = FIELD_CELL(layer->y);
		view++;
		layer++;
	}
	for(y = dstY; y < dstY + height; y++) {
		{
			for(x = dstX; x < dstX + width; x++) {
				u32 cell = *src++;
				u32 cellTilesId;
				u32 offset;
				u32 row;
				u32 n;
				u32 column;
				u32 scaled;
				u32 merged;

				cellTilesId = cell & MAPGRID_CELL_TILES_ID_MASK;
				/* The cell takes the source's drawn cell tiles number and keeps
				 * its */
				/* own bits above. */
				merged = cell & MAPGRID_DRAWN_PICTURE_MASK;
				merged |= *dst & ~MAPGRID_DRAWN_PICTURE_MASK;
				*dst = merged;
				dst++;
				view = views;
				column = x & (MAPGRID_SCREEN_CELLS - 1);
				i = 0;
				row = y & (MAPGRID_SCREEN_CELLS - 1);
				n = row << 5;
				n += column;
				offset = n * 4;
				for(; i < 3; i += 1) {
					if(view->x <= x && view->x + MAPGRID_VISIBLE_WIDTH > x &&
					   view->y <= y && view->y + MAPGRID_VISIBLE_HEIGHT > y) {
						u32 screenAddress, pictureAddress;
						u8 *screenTile, *pictureRows;

						screenAddress = (u32)BG_SCREEN_BLOCK(5);
						screenTile = (u8 *)screenAddress;
						screenTile += offset;
						scaled = cellTilesId * 8;
						pictureAddress = (u32)syRamMapBlocks;
						pictureRows = (u8 *)pictureAddress;
						pictureRows += scaled;
						*(u32 *)screenTile = *(u32 *)pictureRows;

						pictureAddress = (u32)syRamMapBlocks + 4;
						pictureRows = (u8 *)pictureAddress;
						pictureRows += scaled;
						screenAddress =
							(u32)BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES;
						screenTile = (u8 *)screenAddress;
						screenTile += offset;
						*(u32 *)screenTile = *(u32 *)pictureRows;
						break;
					}
					/* scaled, reused for the step to the next layer's BG map.
					 */
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

/* Copies a rectangle of map cells as they are, with their shape grid bytes, */
/* from (srcX, srcY) to (dstX, dstY), width by height cells. */
void fdMapCopyRawCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
						   s32 dstY) {
	u32 *cells = (u32 *)Ram_MapCellBuffer;
	s32 dst = (dstY * MAPGRID_SIZE) + dstX;
	u8 *dstShapes = MapShapeGrid + dst;
	s32 src = (srcY * MAPGRID_SIZE) + srcX;
	u8 *srcShapes = MapShapeGrid + src;
	u32 *dstCells = cells + dst;
	u16 row;
	u16 col;

	cells += src;
	for(row = 0; row < height; row++) {
		u32 *dstCell = dstCells + row * MAPGRID_SIZE;
		u32 *srcCell = cells + row * MAPGRID_SIZE;
		u8 *dstShape = dstShapes + row * MAPGRID_SIZE;
		u8 *srcShape = srcShapes + row * MAPGRID_SIZE;

		for(col = 0; col < width; col++) {
			*dstCell++ = *srcCell++;
			*dstShape++ = *srcShape++;
		}
	}
}

/* Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in */
/* REG_DISPCNT) from layerFlags, keeping its other flags. */
void fdMapGridSetShownLayers(u32 layerFlags) {
	MAP_STATE *state = Ram_HeapSlots->mapGrid;
	u32 shown = layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
	u32 dispcnt = state->dispcnt;

	dispcnt =
		(dispcnt & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		shown;
	state->dispcnt = dispcnt;
}

/* A map edit's flag word: the flag in its low 12 bits, and a bit that makes it
 */
/* copy the cells' attributes rather than their pictures. */
#define MAP_EDIT_FLAG_MASK	0xfff
#define MAP_EDIT_ATTRIBUTES 0x1000
/* The flag word of all ones that ends a map's edits, the one that one more */
/* wraps to 0. */
#define MAP_EDITS_END 0xffff

/* Plays every edit of the map whose flag is set, as fdMapApplyFlagEdits does,
 */
/* MAP_EDIT_ATTRIBUTES in an edit's flag word choosing attributes over */
/* pictures: the map as the story has left it, as the player enters. */
void fdMapApplySetFlagEdits(void) {
	u8 *cursor = MapSceneEditsOffset;
	u32 entry;
	s32 srcX, srcY, dstX, dstY, width, height;

	cursor = MapScene + (u16)fdMapReadEditU16(&cursor);
	for(;;) {
		entry = (u16)fdMapReadEditU16(&cursor);
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
				fdMapCopyCellAttributeRect(srcX, srcY, width, height, dstX,
										   dstY);
			else
				fdMapGridCopyCellTilesIndicesRect(srcX, srcY, dstX, dstY, width,
												  height);
		}
		else {
			cursor += 6;
		}
	}
}

/* Reads the little-endian halfword at *cursor, which may be unaligned, and */
/* moves the cursor past it. */
u32 fdMapReadEditU16(u8 **cursor) {
	u32 low = *(*cursor)++;
	u32 high = *(*cursor)++;

	return low | (high << 8);
}

/* Sets flag (its low 12 bits) and plays its map edits on the cells' pictures.
 */
void fdMapSetFlagAndApplyEdits(s32 flag) {
	flag &= MAP_EDIT_FLAG_MASK;
	syGameFlagSet(flag);
	fdMapApplyFlagEdits(flag, 0);
}

/* Sets flag (its low 12 bits) and plays its map edits on the cells' */
/* attributes. */
void fdMapSetFlagAndApplyAttributeEdits(s32 flag) {
	flag &= MAP_EDIT_FLAG_MASK;
	syGameFlagSet(flag);
	fdMapApplyFlagEdits(flag, 1);
}

/* Plays the map's edits for flag: each copies a rectangle of cells (their */
/* pictures, or with attributes their attributes and shapes) from one place */
/* of the map to another. The map's extra data lists the edits, each a flag */
/* then source x and y, destination x and y, width and height in bytes, ended */
/* by a flag halfword of all ones. */
void fdMapApplyFlagEdits(s32 flag, s32 attributes) {
	u8 *cursor = MapSceneEditsOffset;
	u32 entry;
	s32 srcX, srcY, dstX, dstY, width, height;

	cursor = MapScene + (u16)fdMapReadEditU16(&cursor);
	for(;;) {
		entry = (u16)fdMapReadEditU16(&cursor);
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
				fdMapCopyCellAttributeRect(srcX, srcY, width, height, dstX,
										   dstY);
			else
				fdMapGridCopyCellTilesIndicesRect(srcX, srcY, dstX, dstY, width,
												  height);
		}
		else {
			cursor += 6;
		}
	}
}

/* Reads map's camera bounds from its header, in 8-pixel tiles: bounds[0] its */
/* left, bounds[1] its right, bounds[2] its top and bounds[3] its bottom, a */
/* right or bottom of 0 standing for the map's whole MAPGRID_TILES. */
void fdMapGetCameraBounds(s32 map, s32 *bounds) {
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

/* Decodes the current map's cells into the map cell buffer and converts them,
 */
/* then decodes its cell shapes into MapShapeGrid. */
void fdMapLoadCellsAndShapes(void) {
	u8 *source;
	/* MapsWork through a pointer to its rows. */
	MAP_HEADER *header = MAPGRID_RESOURCE(
		((const MAP_RESOURCES *)MapsWork)[MainRoutineStateWork.mapId].header);

	source = (u8 *)header + header->cells;
	syDecompressTaggedLz(source, Ram_MapCellBuffer);
	fdTileMapConvertBuffer();
	source = (u8 *)header + header->shapes;
	syDecompressTaggedLz(source, MapShapeGrid);
}

/* Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's cell */
/* picture buffer, decompressing it from the area's graphics, and with update */
/* set also draws it into the layer's BG map. */
s32 fdWorldMapWriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile,
								 s32 update) {
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	u32 *tileOffsets = perspective->tiles;
	u16 *shownQuarter;
	u16 *decodedTile;
	u16 *sourceTiles;
	u8 *quarterCells;
	u16 *screenTiles;
	u32 rowIndex;
	u32 columnIndex;
	u32 cellTilesId;

	x &= 1;
	y &= 1;
	shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
	if(update == 0 && tile == *shownQuarter)
		return 0;
	*shownQuarter = tile;
	decodedTile = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH,
								  WORLD_MAP_TILE_CELLS * WORLD_MAP_TILE_CELLS *
									  sizeof(WORLD_MAP_CELL));
	if(tileOffsets[tile] != 0)
		syDecompressByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);
	else
		syWordsClearEntry(decodedTile, WORLD_MAP_TILE_CELLS *
										   WORLD_MAP_TILE_CELLS *
										   sizeof(WORLD_MAP_CELL));

	sourceTiles = decodedTile;
	quarterCells = syRamMapBlocks + (((layer * 2 + y) * 32 + x) << 6);
	for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
		syDMAXCopy(sourceTiles, quarterCells,
				   DMA_COPY_WORDS(WORLD_MAP_TILE_CELLS), REG_DMA3);
		sourceTiles += WORLD_MAP_TILE_CELLS * sizeof(WORLD_MAP_CELL) / 2;
		quarterCells += WORLD_MAP_VIEW_CELLS * sizeof(WORLD_MAP_CELL);
	}

	if(update != 0) {
		screenTiles = (u16 *)((u32)WORLD_MAP_LOWER_TILE_MAP +
							  ((((layer * 2 + y) << 6) + x) << 5));
		sourceTiles = decodedTile;
		for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
			for(columnIndex = 0; columnIndex < WORLD_MAP_TILE_CELLS;
				columnIndex++) {
				u16 *bottomTiles;

				cellTilesId = *sourceTiles;
				screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + cellTilesId * 4);
				bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
				bottomTiles = (u16 *)((u8 *)bottomTiles + cellTilesId * 4);
				screenTiles[WORLD_MAP_TILE_MAP_WIDTH / 2] = *bottomTiles;
				screenTiles++;
				sourceTiles += sizeof(WORLD_MAP_CELL) / 2;
			}
			/* Down two rows of the tile map, of WORLD_MAP_TILE_MAP_WIDTH / 2 */
			/* halfwords each, less the halfwords just written. */
			screenTiles +=
				2 * (WORLD_MAP_TILE_MAP_WIDTH / 2) - WORLD_MAP_TILE_CELLS;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return 1;
}
#undef WORLD_MAP_TILE_CELLS
