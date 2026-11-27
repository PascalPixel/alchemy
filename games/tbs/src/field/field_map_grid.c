/* Layered maps: loading, drawing and scrolling, and copying metatiles. Part of
   field.c. */

#include "field_internal.h"

/* A metatile word's attributes, the bits above the number of its metatile tiles
   (MAPGRID_METATILE_TILES_ID_MASK). */
#define MAPGRID_ATTRIBUTE_MASK		 0xfffff000
/* The shift that leaves a metatile word's metatile tiles number on top, its
   attributes shifted out. Shifted back down by one less it is the number times
   2, the metatile tiles' offset in words in MetatileTiles (a word for each of
   its two rows of 2 tiles); by two less, times 4, its offset in halfwords. */
#define MAPGRID_METATILE_TILES_SHIFT 20
/* The map's 128 by 128 metatiles, each read whole as its word (a struct
   MAP_GRID_METATILE's four bytes). */
#define MAPGRID_METATILE_WORDS		 ((Uint32 *)RamMapGridMetatileBuffer)
/* A layer's visible area in metatiles: the screen's 15 by 10, and the row and
   column partly scrolled in. */
#define MAPGRID_VISIBLE_WIDTH		 16
#define MAPGRID_VISIBLE_HEIGHT		 12
/* The bytes between one layer's BG map and the next, from screen block 5. */
#define MAPGRID_LAYER_SCREEN_BYTES	 2048
/* A layer's BG map in metatiles, 16 by 16 (32 by 32 tiles), and the bytes of
   one row of its tiles. */
#define MAPGRID_SCREEN_METATILES	 16
#define MAPGRID_SCREEN_ROW_BYTES	 64
/* A metatile's top tile row or left tile column in a layer's BG map, even,
   wrapping at the map's 32; its row as tile entries from the map's start. */
#define MAPGRID_SCREEN_TILE_MASK	 (BG_SCREEN_WIDTH - 2)
#define MAPGRID_SCREEN_ROW_MASK		 (MAPGRID_SCREEN_TILE_MASK * BG_SCREEN_WIDTH)
/* The metatile tiles drawn down a column as the view scrolls sideways: the
   screen's 10 rows of metatiles and the one partly scrolled in. */
#define MAPGRID_COLUMN_METATILES	 11
/* A map's metatiles as they are decoded: two planes of a byte a metatile, the
   metatiles' attribute bytes, then a plane of the numbers of their metatile
   tiles, a halfword a metatile. */
#define MAPGRID_BYTE_PLANES_BYTES	 (2 * MAPGRID_SIZE * MAPGRID_SIZE)
/* A map's palette: the first 14 of the 16 BG palettes, 224 colours. */
#define MAP_PALETTE_BYTES			 (14 * PALETTE_COLORS * sizeof(Uint16))

/* Remaps byteCount bytes in place through SpriteColorGreyTable. */
void fdSpriteColorRemapPixels(Uint8 *bytes, Sint32 byteCount)
{
	Uint8 *remapTable = (Uint8 *)SpriteColorGreyTable;

	while(byteCount-- != 0) {
		*bytes = remapTable[*bytes];
		bytes++;
	}
}

/* Unpacks the staged metatile tiles into MetatileTiles. */
void fdTileMapDecodeStagedBuffer(Sint32 size)
{
	Uint16 *destWords;
	Uint16 *srcWords;
	Sint32	wordCount;
	Sint32	i;

	srcWords = (Uint16 *)(RamMapGridMetatileBuffer + 2);
	destWords = (Uint16 *)MetatileTiles;
	wordCount = (size - 1) / 2;
	if(size & 1) {
		switch(((Uint8 *)MapGridMetatileBuffer)[1]) {
		case METATILE_TILES_PACKING_RAW:
			for(i = 0; i < wordCount; i++)
				*destWords++ = *srcWords++;
			break;
		case METATILE_TILES_PACKING_SPLIT_DELTA: {
			/* The high bytes, then the low bytes, each word XORed with the
			   last. */
			Uint32 prevWord = 0;
			Uint8 *highBytes = RamMapGridMetatileBuffer + 2;
			Uint8 *lowBytes = highBytes + wordCount;

			for(i = 0; i < wordCount; i++) {
				Uint32 decodedWord =
					((*highBytes++ << 8) | *lowBytes++) ^ prevWord;
				*destWords++ = decodedWord;
				prevWord = decodedWord;
			}
			break;
		}
		case METATILE_TILES_PACKING_DELTA: {
			Sint32 prevWord = 0;

			for(i = 0; i < wordCount; i++) {
				Sint32 decodedWord = *srcWords++ ^ prevWord;
				*destWords++ = decodedWord;
				prevWord = decodedWord;
			}
			break;
		}
		}
	}
}

typedef void (*TILEMAP_CONVERT_FN)(
	void *dest, const void *src, const void *saved);

/* Unused: gives each map metatile whose metatile tiles are number 0xfff (a
   placeholder) a number of its own, counting up from 0 in map order. */
UNUSED void fdTileMapNumDummyMetatileTiles(void)
{
	Uint32 *metatile = (Uint32 *)MapGridMetatileBuffer;
	Uint32	remaining = MAPGRID_SIZE * MAPGRID_SIZE;
	Uint32	mask = MAPGRID_METATILE_TILES_ID_MASK;
	Sint32	nextPlaceholder = -1;

	for(; remaining != 0; remaining--) {
		Uint32 value = *metatile++;
		Uint32 metatileTilesId = value & mask;

		if(metatileTilesId == mask) {
			if(nextPlaceholder != metatileTilesId)
				nextPlaceholder++;
			value = value + nextPlaceholder - metatileTilesId;
			metatile[-1] = value;
		}
	}
}

/* Joins a decoded map's planes in place into its metatile words, with
   fdTileConvertMapGrid copied to RAM, which reads the byte planes from a
   copy. */
void fdTileMapConvertBuffer(void)
{
	Uint8			  *savedMetatiles;
	TILEMAP_CONVERT_FN convertGrid;

	savedMetatiles = syHeapAllocateEWRAM(MAPGRID_BYTE_PLANES_BYTES);
	syWordsCopy(
		savedMetatiles, RamMapGridMetatileBuffer, MAPGRID_BYTE_PLANES_BYTES);
	RAM_CODE_LOAD(
		convertGrid, fdTileConvertMapGrid, fdTileConvertMapGridCodeSize);
	convertGrid((Uint8 *)MapGridMetatileBuffer + MAPGRID_BYTE_PLANES_BYTES,
		RamMapGridMetatileBuffer, savedMetatiles);
	syHeapRelease(convertGrid);
	syHeapRelease(savedMetatiles);
}

/* Copies one tile column of a metatile's tiles into a BG map: its top and
   bottom tile entries, choosing the left or right half with tileHalf. */
static __inline__ void fdMapGridCopyMetatileTilesHalfColumn(Uint32 *metatile,
	Uint8 *screenMap, Uint32 screenRowOffset, Uint32 screenColumnOffset,
	Uint32 tileHalf)
{
	Uint16 *entry;
	Uint8  *screenTile;
	Uint32	index = ((*metatile << MAPGRID_METATILE_TILES_SHIFT) >>
						(MAPGRID_METATILE_TILES_SHIFT - 2)) +
		tileHalf;

	entry = (Uint16 *)RamMetatileTiles + index;
	screenTile =
		screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
	*(Uint16 *)screenTile = *entry;
	entry = (Uint16 *)(RamMetatileTiles + 4) + index;
	*(Uint16 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *entry;
}

/* Copies all four of a metatile's tiles into a BG map: its top two tile entries
   and its bottom two, a map row (64 bytes) below. */
static __inline__ void fdMapGridCopyMetatileTilesToScreen(Uint32 *metatile,
	Uint8 *screenMap, Uint32 screenRowOffset, Uint32 screenColumnOffset)
{
	Uint32 *rows;
	Uint8  *screenTile;
	Uint32	index = (*metatile << MAPGRID_METATILE_TILES_SHIFT) >>
		(MAPGRID_METATILE_TILES_SHIFT - 1);

	rows = (Uint32 *)RamMetatileTiles + index;
	screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
	*(Uint32 *)screenTile = *rows;
	rows = (Uint32 *)(RamMetatileTiles + 4) + index;
	*(Uint32 *)(screenTile + MAPGRID_SCREEN_ROW_BYTES) = *rows;
}

/* One background layer's setup in a layout's header: its origin, its parallax
   and drift, and where it wraps. */
typedef struct MAP_LAYER {
	Uint8 x;	   /* 8-pixel tiles */
	Uint8 y;	   /* 8-pixel tiles */
	Sint8 scrollX; /* sixteenths of the view's motion */
	Sint8 scrollY; /* sixteenths of the view's motion */
	Sint8 speedX;  /* 1/16 pixels per tick */
	Sint8 speedY;  /* 1/16 pixels per tick */
	Uint8 periodX; /* 8-pixel tiles */
	Uint8 periodY; /* 8-pixel tiles */
} MAP_LAYER;

/* A layout's header: the camera's bounds, the layers' priorities, tile bases
   and setup, and the offsets of its packed tiles, shapes, tilemap, tileset
   animation, blend script and treasure spots. */
typedef struct MAP_HEADER {
	/* The camera's bounds in 8-pixel tiles: left, top, right and bottom. */
	Uint8 cameraBounds[4];
	/* Each layer's BG priority, 1 to 3; 0 leaves the layer off. */
	Uint8	  priority[3];
	Uint8	  charBase[3]; /* each layer's BG char block */
	Uint16	  reserved; /* 0 in every layout of both games; nothing reads it */
	MAP_LAYER layers[3];
	/* Offsets from the header's start, of data packed for syDecompressTaggedLZ
	   but for the treasure spots, which are read in place. the metatile tiles,
	   staged and unpacked into MetatileTiles */
	Sint32 metatileTiles;
	Sint32 shapes;	  /* the shapes, into RamMapGridShapes */
	Sint32 metatiles; /* the metatiles, into RamMapGridMetatileBuffer */
	Sint32 animation; /* the tileset animation script, or 0 for none */
	Sint32 blend;	  /* the blend script, or 0 for none */
	Sint32 markers; /* the chests and Psynergy stones (fdPlaceTreasureSpots) */
} MAP_HEADER;

/* One of a layout's resources, from its row of MapsWork. */
#define MAPGRID_RESOURCE(offset) \
	syResourceGetTableEntry((offset) + (Uint32)RESOURCE_EMPTY_MAP)

/* Loads layout index from MapsWork: its metatile tiles, shapes, metatiles,
   tileset animations, blend script and treasure spots; sets up the three
   background layers' origins, parallax and wrap from the layout header; loads
   the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says they are
   loaded; and starts the layer scrolling. */
Sint32 fdMapGridLoadMap(Sint32 index)
{
	MAP_RESOURCES		  *mapResources;
	MAP_GRID_SCROLL_STATE *grid;
	MAP_HEADER			  *mapHeader;
	MAP_GRID_LAYER_SCROLL *layer;
	MAP_LAYER			  *layerSrc;
	Sint32				   layerIndex;
	Uint8				  *graphicsBuf;
	Sint32				   backdrop;
	Uint8				  *packed;
	Sint32				  *viewX;
	Sint32				  *viewY;
	Uint32				   control;

	/* Hide the map layers and sprites while their memory is replaced. */
	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScreenFadeFromBlack(0);
	mapResources = (MAP_RESOURCES *)&MapsWork[index];
	grid = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_MAP_GRID, sizeof(MAP_GRID_SCROLL_STATE));
	syWordsClear(grid, sizeof(MAP_GRID_SCROLL_STATE));
	mapHeader = MAPGRID_RESOURCE(mapResources->header);
	/* The header's offsets count from the header itself. */
	packed = (Uint8 *)mapHeader + mapHeader->metatileTiles;
	fdTileMapDecodeStagedBuffer(
		syDecompressTaggedLZ(packed, RamMapGridMetatileBuffer + 1));
	packed = (Uint8 *)mapHeader + mapHeader->shapes;
	syDecompressTaggedLZ(packed, RamMapGridShapes);
	packed = (Uint8 *)mapHeader + mapHeader->metatiles;
	syDecompressTaggedLZ(packed, RamMapGridMetatileBuffer);
	fdTileMapConvertBuffer();
	/* The tileset animation and blend scripts are optional (offset 0). */
	if(mapHeader->animation != 0) {
		packed = (Uint8 *)mapHeader + mapHeader->animation;
		syDecompressTaggedLZ(packed, MAPGRID_ANIMATION_SCRIPT);
		fdTilesetAnimationStartAll((const Uint16 *)MAPGRID_ANIMATION_SCRIPT);
	}
	if(mapHeader->blend != 0) {
		packed = (Uint8 *)mapHeader + mapHeader->blend;
		syDecompressTaggedLZ(packed, MAPGRID_BLEND_SCRIPT);
		fdBlendScriptStart((Uint16 *)MAPGRID_BLEND_SCRIPT);
	}
	/* The treasure spot list is not decoded: fdPlaceTreasureSpots reads it in
	   place through this pointer. */
	grid->markers = (Uint8 *)mapHeader + mapHeader->markers;
	/* The header's bounds and origins are in 8-pixel tiles. */
	grid->minX = toFIXED(mapHeader->cameraBounds[0] * 8);
	grid->minY = toFIXED(mapHeader->cameraBounds[1] * 8);
	grid->maxX = toFIXED(mapHeader->cameraBounds[2] * 8);
	grid->maxY = toFIXED(mapHeader->cameraBounds[3] * 8);
	viewX = &grid->view[0];
	*viewX = 0;
	viewY = &grid->view[1];
	*viewY = 0;
	grid->enabled[0] = mapHeader->priority[0];
	grid->enabled[1] = mapHeader->priority[1];
	grid->enabled[2] = mapHeader->priority[2];
	layer = grid->layers;
	layerSrc = mapHeader->layers;
	for(layerIndex = 0; layerIndex < 3; layerIndex++) {
		Uint32 x = layerSrc->x;
		Uint32 y = layerSrc->y;
		Sint32 scaleX, scaleY;
		FIXED  offsetX, offsetY;

		layer->offsetX = offsetX = toFIXED(x * 8);
		layer->offsetY = offsetY = toFIXED(y * 8);
		/* Parallax and drift are signed sixteenths. */
		scaleX = layerSrc->scrollX * (toFIXED(1) / 16);
		scaleY = layerSrc->scrollY * (toFIXED(1) / 16);
		layer->scaleX = scaleX;
		layer->scaleY = scaleY;
		layer->speedX = layerSrc->speedX * (toFIXED(1) / 16);
		layer->speedY = layerSrc->speedY * (toFIXED(1) / 16);
		layer->maskX = layerSrc->periodX;
		layer->maskY = layerSrc->periodY;
		layer->phaseX = 0;
		layer->phaseY = 0;
		/* The layer's window starts at the metatile under its origin tile (a
		   metatile is 2x2 tiles, 128 metatiles to a row). */
		layer->metatiles = (MAP_GRID_METATILE *)RamMapGridMetatileBuffer +
			(y / 2) * MAPGRID_SIZE + x / 2;
		layer->x = syMulFX(*viewX, scaleX) + offsetX;
		layer->y = syMulFX(*viewY, scaleY) + offsetY;
		layerSrc++;
		layer++;
	}
	/* The ground, middle and overhead layers show on BG3, BG2 and BG1, a layer
	   only where its priority is not zero. */
	grid->flags = DISPCNT_OBJ_ON;
	if(grid->enabled[0] != 0)
		grid->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
	if(grid->enabled[1] != 0)
		grid->flags |= DISPCNT_BG2_ON;
	if(grid->enabled[2] != 0)
		grid->flags |= DISPCNT_BG1_ON;
	control = grid->enabled[0] | BGCNT_CHAR_BASE(mapHeader->charBase[0]) |
		BGCNT_SCREEN_BASE(5);
	REG_BG3CNT = control;
	IO_WRITE16(grid->enabled[1] | BGCNT_CHAR_BASE(mapHeader->charBase[1]) |
			BGCNT_SCREEN_BASE(6),
		&REG_BG2CNT);
	IO_WRITE16(grid->enabled[2] | BGCNT_CHAR_BASE(mapHeader->charBase[2]) |
			BGCNT_SCREEN_BASE(7),
		&REG_BG1CNT);
	/* A set FLAG_KEEP_LAYER_GRAPHICS skips loading the graphics once, and is
	   cleared. */
	if(gmFlagGetStub(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
		gmFlagClearStub(FLAG_KEEP_LAYER_GRAPHICS);
	} else {
		/* One 16 KB buffer takes each resource in turn: the palette, the three
		   16 KB tilesets, and last the tileset animations' tiles, which decode
		   straight to MapGridAnimationTiles. */
		graphicsBuf = syHeapAllocate(BG_CHAR_SIZE);
		if(graphicsBuf != NULL) {
			/* Colour 0 of BG palette 0 is the backdrop; it survives the new
			   palette. */
			backdrop = *(Sint16 *)PLTT;
			syDecompressTaggedLZ(
				MAPGRID_RESOURCE(mapResources->palette), graphicsBuf);
			*(Sint16 *)graphicsBuf = backdrop;
			/* These four copies call the IWRAM function at its fixed address,
			   not through syWordsCopy. */
			syWordsCopyEntry((void *)BG_PLTT, graphicsBuf, MAP_PALETTE_BYTES);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(mapResources->tiles[0]), graphicsBuf);
			syWordsCopyEntry(BG_CHAR_BLOCK(1), graphicsBuf, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(mapResources->tiles[1]), graphicsBuf);
			syWordsCopyEntry(BG_CHAR_BLOCK(2), graphicsBuf, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(mapResources->tiles[2]), graphicsBuf);
			syWordsCopyEntry(BG_CHAR_BLOCK(3), graphicsBuf, BG_CHAR_SIZE);
			syDecompressMoveToFrontLZ(
				MAPGRID_RESOURCE(mapResources->animationTiles),
				MapGridAnimationTiles);
			syHeapRelease(graphicsBuf);
		}
	}
	/* Only BG0, the windows, is shown here; fdScreenTransitionStart ORs
	   map->flags into REG_DISPCNT when the map fades in. */
	control = 0;
	REG_MOSAIC = control;
	REG_BLDCNT = control;
	control = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	IO_WRITE16(control, &REG_DISPCNT);
	sySetEvent(fdMapGridUpdateLayerScroll, EVENT_PASS_TICK + 5);
	return 2;
}

/* Centres the camera on the point the map follows (origin: x, height, z), or on
   (0, 0) when there is none, and scrolls the layers there. */
void fdMapGridCenterOnFollowedPoint(void)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	Sint32				   x;
	Sint32				   depth;
	Sint32				   height;
	Sint32				  *followedPos;

	followedPos = grid->origin;
	x = 0;
	height = 0;
	depth = 0;
	if(followedPos != NULL) {
		x = *followedPos++;
		height = *followedPos++;
		depth = *followedPos;
	}
	fdMapGridSetCameraCenter(x, depth - height);
	fdMapGridUpdateLayerScroll();
}

/* Draws the metatile tiles of a row of 16 of a layer's metatiles (32 tiles)
   into its BG map (screen block 5 + layer), starting at tile (tileX, tileY);
   the BG map wraps every 32 tiles. */
void fdMapGridRenderMetatileTilesRow(Uint32 layer, Sint32 tileX, Sint32 tileY)
{
	Uint8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	Uint32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	Uint32 screenRowOffset =
		(tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	Uint32 srcColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	Uint32 screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	Uint32 metatileTilesIndex;

	/* A metatile's tiles are 2x2, so the BG map's tile row and column are kept
	   even and wrap at 32; the grid wraps at 128 metatiles. */
	for(metatileTilesIndex = 0; metatileTilesIndex < MAPGRID_SCREEN_METATILES;
		metatileTilesIndex++) {
		Uint32 *metatile = MAPGRID_METATILE_WORDS;

		metatile += sourceRow + srcColumn;
		fdMapGridCopyMetatileTilesToScreen(
			metatile, screenMap, screenRowOffset, screenColumnOffset);
		srcColumn = (srcColumn + 1) & (MAPGRID_SIZE - 1);
		screenColumnOffset =
			(screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
	}
}

/* Draws the metatile tiles of a column of 11 of a layer's metatiles, one tile
   wide, into its BG map, starting at tile (tileX, tileY). */
void fdMapGridRenderMetatileTilesColumn(
	Uint32 layer, Sint32 tileX, Sint32 tileY)
{
	Uint8 *screenMap = BG_SCREEN_BLOCK(5) + layer * MAPGRID_LAYER_SCREEN_BYTES;
	Uint32 sourceRow = ((tileY / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
	Uint32 screenRowOffset =
		(tileY & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
	Uint32 srcColumn = (tileX / 2) & (MAPGRID_SIZE - 1);
	Uint32 screenColumnOffset = tileX & MAPGRID_SCREEN_TILE_MASK;
	Uint32 tileHalf = tileX & 1;
	Uint32 metatileTilesIndex;

	/* Both the metatiles and the BG map wrap. */
	for(metatileTilesIndex = 0; metatileTilesIndex < MAPGRID_COLUMN_METATILES;
		metatileTilesIndex++) {
		Uint32 *metatile = MAPGRID_METATILE_WORDS;

		metatile += sourceRow + srcColumn;
		fdMapGridCopyMetatileTilesHalfColumn(
			metatile, screenMap, screenRowOffset, screenColumnOffset, tileHalf);

		sourceRow =
			(sourceRow + MAPGRID_SIZE) & ((MAPGRID_SIZE - 1) * MAPGRID_SIZE);
		screenRowOffset =
			(screenRowOffset + 2 * BG_SCREEN_WIDTH) & MAPGRID_SCREEN_ROW_MASK;
	}
}

/* Each tick on a layered map: centres the view on the point the map follows
   (origin), clamped to the map's bounds and shaken by any decaying screen
   shake; then scrolls each of the three layers by its parallax scale and
   automatic scroll (wrapping at its period), draws the column or row of
   metatile tiles that scrolls into view, and sets its BG scroll. */
void fdMapGridUpdateLayerScroll(void)
{
	MAP_GRID_SCROLL_STATE *grid;
	MAP_GRID_LAYER_SCROLL *layer;
	Sint32				  *origin;
	Sint32				   x, y;
	Sint32				   minX, minY, maxX, maxY;
	Sint32				   baseX, baseY;
	Uint32				   layerIndex;

	grid = MapGridState[0];
	origin = grid->origin;
	layer = grid->layers;
	if(origin == 0)
		return;

	/* Height projects the followed point above its ground position. */
	baseX = *origin++ - FIELD_VIEW_FOCUS_X;
	{
		Sint32 height = *origin++;
		baseY = *origin - height - FIELD_VIEW_FOCUS_Y;
	}
	/* The bounds shrink by the shake so a shaken view stays on the map; the
	   right and bottom bounds lose the 240x160 screen. */
	minX = grid->minX + grid->shakeX;
	maxX = grid->maxX - grid->shakeX - FIELD_VIEW_WIDTH;
	minY = grid->minY + grid->shakeY;
	maxY = grid->maxY - grid->shakeY - FIELD_VIEW_HEIGHT;
	if(minX > maxX)
		maxX = minX;
	if(minY > maxY)
		maxY = minY;
	CLAMP(baseX, minX, maxX);
	CLAMP(baseY, minY, maxY);

	/* A shake offsets the view by the amplitude times the difference of two
	   random fractions (between -1 and 1), then scales the amplitude by
	   shakeDecay for the next tick. */
	if(grid->shakeX != 0) {
		Sint32 shakeRollA = syRandom();
		Sint32 shakeRollB = syRandom();
		Sint32 shakeAmplitude = grid->shakeX;
		baseX += syMulFX(shakeAmplitude, shakeRollA - shakeRollB);
		grid->shakeX = syMulFX(shakeAmplitude, grid->shakeDecay);
	}
	if(grid->shakeY != 0) {
		Sint32 shakeRollA = syRandom();
		Sint32 shakeRollB = syRandom();
		FIXED  shakeAmplitude = grid->shakeY;
		baseY += syMulFX(shakeAmplitude, shakeRollA - shakeRollB);
		grid->shakeY = syMulFX(shakeAmplitude, grid->shakeDecay);
	}
	grid->view[0] = baseX;
	grid->view[1] = baseY;
	for(layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
		baseX = syMulFX(grid->view[0], layer->scaleX);
		baseY = syMulFX(grid->view[1], layer->scaleY);
		/* A drifting layer wraps at its period: maskX and maskY are in 8-pixel
		   tiles, and the mask keeps the pixel and fraction bits below them. */
		if(layer->speedX != 0) {
			layer->phaseX += layer->speedX;
			baseX = (baseX + layer->phaseX) &
				(layer->maskX * toFIXED(8) | (toFIXED(8) - 1));
		}
		if(layer->speedY != 0) {
			layer->phaseY += layer->speedY;
			baseY = (baseY + layer->phaseY) &
				(layer->maskY * toFIXED(8) | (toFIXED(8) - 1));
		}
		baseX += layer->offsetX;
		baseY += layer->offsetY;
		/* x and y are in 8-pixel tiles. */
		x = baseX / toFIXED(8);
		y = baseY / toFIXED(8);
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
		/* MAPGRID_LAYER_GROUND scrolls BG3, MAPGRID_LAYER_OVERHEAD BG1. */
		BgScrollWork[3 - layerIndex].x = FIXEDtoINT(baseX);
		BgScrollWork[3 - layerIndex].y = FIXEDtoINT(baseY);
		layer->x = baseX;
		layer->y = baseY;
	}
}

/* Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
   redraws every enabled layer's BG map around it. */
void fdMapGridSetCameraCenter(FIXED x, FIXED y)
{
	Uint32				   layerIndex;
	Uint8				  *screenMap;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_LAYER_SCROLL *layer = grid->layers;

	x -= FIELD_VIEW_FOCUS_X;
	y -= FIELD_VIEW_FOCUS_Y;
	CLAMP(x, grid->minX, grid->maxX - FIELD_VIEW_WIDTH);
	CLAMP(y, grid->minY, grid->maxY - FIELD_VIEW_HEIGHT);
	grid->view[0] = x;
	grid->view[1] = y;

	for(layerIndex = 0; layerIndex < 3; layerIndex++) {
		if(grid->enabled[layerIndex]) {
			Uint32 rows = 2 * MAPGRID_COLUMN_METATILES;
			Uint32 srcRow, srcColumn, screenRowOffset, screenColumnOffset,
				metatileTilesRow, metatileTilesColumn;

			x = syMulFX(grid->view[0], layer->scaleX);
			y = syMulFX(grid->view[1], layer->scaleY);
			if(layer->speedX) {
				layer->phaseX += layer->speedX;
				x = (x + layer->phaseX) &
					(layer->maskX * toFIXED(8) | (toFIXED(8) - 1));
			}
			if(layer->speedY) {
				layer->phaseY += layer->speedY;
				y = (y + layer->phaseY) &
					(layer->maskY * toFIXED(8) | (toFIXED(8) - 1));
				/* A layer drifting vertically gets its whole BG map, others the
				   rows a scrolling column draws. */
				rows = 2 * MAPGRID_SCREEN_METATILES;
			}
			x += layer->offsetX;
			y += layer->offsetY;
			layer++;
			/* Divided by 8 pixels in 16.16, x and y become tiles. */
			x /= toFIXED(8);
			y /= toFIXED(8);
			screenMap =
				BG_SCREEN_BLOCK(5) + layerIndex * MAPGRID_LAYER_SCREEN_BYTES;
			srcRow = ((y / 2) & (MAPGRID_SIZE - 1)) * MAPGRID_SIZE;
			screenRowOffset = (y & MAPGRID_SCREEN_TILE_MASK) * BG_SCREEN_WIDTH;
			for(metatileTilesRow = 0; metatileTilesRow < rows / 2;
				metatileTilesRow++) {
				srcColumn = (x / 2) & (MAPGRID_SIZE - 1);
				screenColumnOffset = x & MAPGRID_SCREEN_TILE_MASK;
				for(metatileTilesColumn = 0;
					metatileTilesColumn < MAPGRID_SCREEN_METATILES;
					metatileTilesColumn++) {
					Uint32 *metatile = MAPGRID_METATILE_WORDS;

					metatile += srcRow + srcColumn;
					fdMapGridCopyMetatileTilesToScreen(metatile, screenMap,
						screenRowOffset, screenColumnOffset);
					srcColumn = (srcColumn + 1) & (MAPGRID_SIZE - 1);
					screenColumnOffset =
						(screenColumnOffset + 2) & MAPGRID_SCREEN_TILE_MASK;
				}
				srcRow = (srcRow + MAPGRID_SIZE) &
					((MAPGRID_SIZE - 1) * MAPGRID_SIZE);
				screenRowOffset = (screenRowOffset + 2 * BG_SCREEN_WIDTH) &
					MAPGRID_SCREEN_ROW_MASK;
			}
		}
	}
}

/* The top left of a layer's visible area, in metatiles. */
typedef struct MAP_GRID_TILE_POS {
	Sint32 x, y;
} MAP_GRID_TILE_POS;

/* The redraw the three metatile copies below share, for the metatile at
   (metatileX, metatileY) just copied: the first layer whose visible area (16 by
   12 metatiles from its scroll position) holds it, and that one only, has the
   metatile's top two tile entries and its bottom two (a tile row, 64 bytes,
   further on) written to its BG map. The layers' BG maps follow each other,
   2,048 bytes apart, from screen block 5, and in each a row of metatiles is 128
   bytes and a metatile
   4. It uses the copy's own locals: row and column (the metatile's place in the
   BG map), metatileTilesId, visibleLayers and the scratch place, offset, bytes,
   i and visibleLayer. */
#define MAPGRID_REDRAW_COPIED_METATILE(metatileX, metatileY) \
	visibleLayer = visibleLayers; \
	place = row << 5; \
	place += column; \
	offset = place * 4; \
	do { \
		if(visibleLayer->x <= (metatileX) && \
			visibleLayer->x + MAPGRID_VISIBLE_WIDTH > (metatileX) && \
			visibleLayer->y <= (metatileY) && \
			visibleLayer->y + MAPGRID_VISIBLE_HEIGHT > (metatileY)) { \
			Uint8 *screen, *metatileTiles; \
\
			screen = BG_SCREEN_BLOCK(5); \
			screen += offset; \
			bytes = metatileTilesId * 8; \
			metatileTiles = RamMetatileTiles; \
			metatileTiles += bytes; \
			*(Uint32 *)screen = *(Uint32 *)metatileTiles; \
			metatileTiles = RamMetatileTiles + 4; \
			metatileTiles += bytes; \
			screen = BG_SCREEN_BLOCK(5) + MAPGRID_SCREEN_ROW_BYTES; \
			screen += offset; \
			*(Uint32 *)screen = *(Uint32 *)metatileTiles; \
			break; \
		} \
		bytes = MAPGRID_LAYER_SCREEN_BYTES; \
		offset += bytes; \
		visibleLayer++; \
		i++; \
	} while(i < 3)

/* Copies the metatile tiles numbers of a rectangle of metatiles from (srcX,
   srcY) to (destX, destY), width by height metatiles, keeping each
   destination's attributes, and redraws the copied metatiles that are on
   screen. */
void fdMapGridCopyMetatileTilesIndicesRect(Sint32 srcX, Sint32 srcY,
	Uint32 destX, Uint32 destY, Uint32 width, Uint32 height)
{
	Uint32	y;
	Uint32 *srcMetatiles =
		MAPGRID_METATILE_WORDS + (srcY * MAPGRID_SIZE + srcX);
	Uint32 *destMetatiles =
		MAPGRID_METATILE_WORDS + (destY * MAPGRID_SIZE + destX);
	MAP_GRID_LAYER_SCROLL *layer =
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers;
	MAP_GRID_TILE_POS  visibleLayers[3];
	Sint32			   i;
	MAP_GRID_TILE_POS *visibleLayer = visibleLayers;
	Uint32			   x;

	for(i = 0; i < 3; i++) {
		visibleLayer->x = FIELD_METATILE(layer->x);
		visibleLayer->y = FIELD_METATILE(layer->y);
		visibleLayer++;
		layer++;
	}
	for(y = destY; y < destY + height; y++) {
		for(x = destX; x < destX + width; x++) {
			Uint32 metatile = *srcMetatiles++;
			Uint32 metatileTilesId = metatile & MAPGRID_METATILE_TILES_ID_MASK;
			Uint32 column, row, place, offset, bytes;

			*destMetatiles =
				(*destMetatiles & MAPGRID_ATTRIBUTE_MASK) | metatileTilesId;
			column = x & (MAPGRID_SCREEN_METATILES - 1);
			destMetatiles++;
			i = 0;
			row = y & (MAPGRID_SCREEN_METATILES - 1);
			MAPGRID_REDRAW_COPIED_METATILE((Sint32)x, (Sint32)y);
		}
		srcMetatiles += MAPGRID_SIZE - width;
		destMetatiles += MAPGRID_SIZE - width;
	}
}

/* One step of a map edit: the rectangle of metatiles to copy from and the wait
   after it. */
typedef struct MAP_GRID_METATILE_ANIMATION_STEP {
	/* the source rectangle's x in metatiles, or MAPGRID_ANIMATION_STEPS_END */
	Uint16 src;
	/* The source rectangle's y, width and height in metatiles, and the ticks
	   to wait after the copy. */
	Sint16 y, width, height, delay;
} __attribute__((packed, aligned(2))) MAP_GRID_METATILE_ANIMATION_STEP;

STATIC_ASSERT(sizeof(MAP_GRID_METATILE_ANIMATION_STEP) == 10);

/* Plays a map edit (MAP_GRID_METATILE_ANIMATION_STEP entries, ended by a source
   of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of metatile
   tiles numbers to (destX, destY) and waits its delay in
   ticks. */
void fdMapGridPlayMapEdit(Uint16 *steps, Sint32 destX, Uint32 destY)
{
	/* Each entry is read in turn: its source, then its signed y, width, height
	   and delay (MAP_GRID_METATILE_ANIMATION_STEP). */
	while(*steps != MAPGRID_ANIMATION_STEPS_END) {
		Uint16 src = *steps++;
		Sint32 srcY = (Sint16)*steps++;
		Sint32 width = (Sint16)*steps++;
		Sint32 height = (Sint16)*steps++;
		Sint32 delay = (Sint16)*steps++;

		fdMapGridCopyMetatileTilesIndicesRect(
			src, (Uint16)srcY, destX, destY, (Uint16)width, (Uint16)height);
		sySynch((Uint16)delay);
	}
}

/* Copies a rectangle of metatiles (16x16 pixels each) as they are, from
   (srcX, srcY) to (destX, destY), width by height metatiles. */
void fdMapGridCopyMetatileTilesMetatilesRect(Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
	Sint32	y;
	Uint32 *srcMetatiles =
		MAPGRID_METATILE_WORDS + (srcY * MAPGRID_SIZE + srcX);
	Uint32 *destMetatiles =
		MAPGRID_METATILE_WORDS + (destY * MAPGRID_SIZE + destX);
	MAP_GRID_LAYER_SCROLL *layer =
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers;
	MAP_GRID_TILE_POS  visibleLayers[3];
	Sint32			   i;
	MAP_GRID_TILE_POS *visibleLayer = visibleLayers;
	Sint32			   x;

	for(i = 0; i < 3; i++) {
		visibleLayer->x = FIELD_METATILE(layer->x);
		visibleLayer->y = FIELD_METATILE(layer->y);
		visibleLayer++;
		layer++;
	}
	for(y = destY; y < destY + height; y++) {
		for(x = destX; x < destX + width; x++) {
			Uint32 metatile = *srcMetatiles++;
			Uint32 metatileTilesId;
			Uint32 column, row, place, offset, bytes;

			*destMetatiles = metatile;
			metatileTilesId = metatile & MAPGRID_METATILE_TILES_ID_MASK;
			column = x & (MAPGRID_SCREEN_METATILES - 1);
			destMetatiles++;
			i = 0;
			row = y & (MAPGRID_SCREEN_METATILES - 1);
			MAPGRID_REDRAW_COPIED_METATILE(x, y);
		}
		srcMetatiles += MAPGRID_SIZE - width;
		destMetatiles += MAPGRID_SIZE - width;
	}
}

/* Copies only the attributes of a rectangle of metatiles, from (srcX,
   srcY) to (destX, destY), width by height metatiles, keeping each destination
   metatile's metatile tiles number. */
void fdMapGridCopyMetatileAttributeRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY)
{
	Uint32 *destMetatiles = MAPGRID_METATILE_WORDS;
	Uint32 *srcMetatiles = destMetatiles;
	Uint16	row;
	Uint16	column;

	destMetatiles += destY * MAPGRID_SIZE + destX;
	srcMetatiles += srcY * MAPGRID_SIZE + srcX;

	for(row = 0; row < height; row++) {
		Uint32 *destinationMetatile = destMetatiles + row * MAPGRID_SIZE;
		Uint32 *sourceMetatile = srcMetatiles + row * MAPGRID_SIZE;

		for(column = 0; column < width; column++) {
			*destinationMetatile =
				(*destinationMetatile & MAPGRID_METATILE_TILES_ID_MASK) |
				(*sourceMetatile++ & MAPGRID_ATTRIBUTE_MASK);
			destinationMetatile++;
		}
	}
}

/* Copies only the metatile tiles numbers of a rectangle of metatiles, from
   (srcX, srcY) to (destX, destY), width by height metatiles, keeping each
   destination metatile's attributes; metatiles in a visible area update the BG
   maps too.

   The same copy as fdMapGridCopyMetatileTilesIndicesRect with the size before
   the destination and signed metatiles: the game has both, and scripts call
   each. */
void fdMapGridCopyMetatileIndexRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY)
{
	Sint32	y;
	Uint32 *srcMetatiles =
		MAPGRID_METATILE_WORDS + (srcY * MAPGRID_SIZE + srcX);
	Uint32 *destMetatiles =
		MAPGRID_METATILE_WORDS + (destY * MAPGRID_SIZE + destX);
	MAP_GRID_LAYER_SCROLL *layer =
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers;
	MAP_GRID_TILE_POS  visibleLayers[3];
	Sint32			   i;
	MAP_GRID_TILE_POS *visibleLayer = visibleLayers;
	Sint32			   x;

	for(i = 0; i < 3; i++) {
		visibleLayer->x = FIELD_METATILE(layer->x);
		visibleLayer->y = FIELD_METATILE(layer->y);
		visibleLayer++;
		layer++;
	}
	for(y = destY; y < destY + height; y++) {
		for(x = destX; x < destX + width; x++) {
			Uint32 metatile = *srcMetatiles++;
			Uint32 metatileTilesId = metatile & MAPGRID_METATILE_TILES_ID_MASK;
			Uint32 column, row, place, offset, bytes;

			*destMetatiles =
				(*destMetatiles & MAPGRID_ATTRIBUTE_MASK) | metatileTilesId;
			column = x & (MAPGRID_SCREEN_METATILES - 1);
			destMetatiles++;
			i = 0;
			row = y & (MAPGRID_SCREEN_METATILES - 1);
			MAPGRID_REDRAW_COPIED_METATILE(x, y);
		}
		srcMetatiles += MAPGRID_SIZE - width;
		destMetatiles += MAPGRID_SIZE - width;
	}
}

/* Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
   REG_DISPCNT) from layerFlags, keeping its other flags. */
void fdMapGridSetShownLayers(Uint32 layerFlags)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	Uint32				   shown =
		layerFlags & (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
	Uint32 flags = grid->flags;

	flags =
		(flags & (Uint16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		shown;
	grid->flags = flags;
}

#undef MAPGRID_METATILE_WORDS
#undef MAPGRID_ATTRIBUTE_MASK
#undef MAPGRID_METATILE_TILES_SHIFT
#undef MAPGRID_VISIBLE_WIDTH
#undef MAPGRID_VISIBLE_HEIGHT
#undef MAPGRID_LAYER_SCREEN_BYTES
#undef MAPGRID_SCREEN_METATILES
#undef MAPGRID_SCREEN_ROW_BYTES
#undef MAPGRID_RESOURCE
#undef MAPGRID_REDRAW_COPIED_METATILE
