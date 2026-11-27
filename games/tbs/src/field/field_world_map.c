/* The world map: its perspective view, its tiles, and the still image of the
   view BG1 shows while a menu or script runs. Part of field.c. */

#include "field_internal.h"

/* IO_WRITE16 as an inline function. */
static __inline__ void fdIOWrite16Inline(Sint32 value, volatile Uint16 *reg)
{
	*reg = value;
}

/* fdWorldMapUpdateAffineLines, run from RAM: works out each screen line's BG2
   and BG3 affine parameters for the camera into one of the two affine pages. */
typedef Sint32 (*WORLD_MAP_PLANE_FN)(
	void *camera, Sint32 *position, void *lines, void *out);

/* One 32-pixel metatile of the world map: its tile, its kind
   (WORLD_MAP_ENCOUNTER_REGION) and a flag. */
typedef struct WORLD_MAP_METATILE {
	/* low halfword: the metatile tiles fdWorldMapCopyMetatileTiles draws */
	Uint32 tile : 24;
	Uint32 kind : 7; /* the encounter region; encounters read it too */
	/* Selects the second row of WorldMapBattleEnvironments
	   (fdWorldMapGetBattleEnvironment); a WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
	   metatile takes the third row whatever this bit says. */
	Uint32 flg : 1;
} WORLD_MAP_METATILE;

#define WORLD_MAP_BG_PALETTE ((Sint16 *)BG_PLTT)

#define WORLD_MAP_METATILES ((WORLD_MAP_METATILE *)Ram_MetatileTiles)

/* A world map tile is 16 by 16 metatiles. */
#define WORLD_MAP_TILE_METATILES 16
/* The world map is 16 by 16 tiles, 512 pixels each, wrapping; each layer's tile
   numbers follow the 320 before it. */
#define WORLD_MAP_TILES_ACROSS	 16
#define WORLD_MAP_LAYER_TILES	 320
/* The tiles after the world's 256 in a layer: other versions of a few grid
   metatiles. */
#define WORLD_MAP_ALTERNATE_TILES \
	(WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS)
/* 16.16 pixels to whole 512-pixel tiles. */
#define WORLD_MAP_FIXED_TO_TILE(v)	 ((v) >> 25)
/* Each layer's metatile buffer holds the 2 by 2 tiles around the view: 32
   metatiles a side, 4 bytes a metatile. */
#define WORLD_MAP_BUFFER_METATILES	 32
#define WORLD_MAP_METATILE_ROW_BYTES (WORLD_MAP_BUFFER_METATILES * 4)
#define WORLD_MAP_LAYER_METATILE_BYTES \
	(WORLD_MAP_BUFFER_METATILES * WORLD_MAP_METATILE_ROW_BYTES)
/* An affine layer's BG map: 64 tiles a row, one byte each, two screen blocks in
   all. */
#define WORLD_MAP_SCREEN_WIDTH		   64
/* A map row as the halfwords that write it two tiles at a time. */
#define WORLD_MAP_SCREEN_ROW_HALFWORDS (WORLD_MAP_SCREEN_WIDTH / 2)
#define WORLD_MAP_LAYER_SCREEN_BYTES \
	(WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_WIDTH)
/* MAP_GRID_SCROLL_STATE's wideTiles, which the world map shares: any nonzero
   value has the tileset animations copy 64-byte tiles. */
#define WORLD_MAP_WIDE_TILES 128

/* The window rows fdWorldMapStartStillImage sets, below the screen: the tick
   update then picks mode 0 and BG1 shows the still image. */
#define WORLD_MAP_STILL_WINDOW_TOP	  200
#define WORLD_MAP_STILL_WINDOW_BOTTOM 255
#define WORLD_MAP_STILL_TILES		  (DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT)

/* Grid metatile (column, row) shows its alternate tile. */
#define WORLD_MAP_USE_ALTERNATE_TILE(perspective, column, row) \
	((perspective)->tileIds[(row) * WORLD_MAP_TILES_ACROSS + (column)] = \
			WORLD_MAP_ALTERNATE_TILES + (row) * WORLD_MAP_TILES_ACROSS + \
			(column))

/* Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's metatile
   picture buffer, decompressing it from the area's graphics, and with update
   set also draws it into the layer's BG map. */
Sint32 fdWorldMapWriteLayerMetatileTile(
	Sint32 layer, Sint32 x, Sint32 y, Sint32 tile, Sint32 update)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	Uint32		   *tileOffsets = perspective->tiles;
	Uint16		   *shownQuarter;
	Uint16		   *decodedTile;
	Uint16		   *srcTiles;
	Uint8		   *quarterMetatiles;
	Uint16		   *screenTiles;
	Uint32			rowIdx;
	Uint32			columnIdx;
	Uint32			metatileTilesId;

	x &= 1;
	y &= 1;
	shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
	if(update == 0 && tile == *shownQuarter)
		return 0;
	*shownQuarter = tile;
	decodedTile = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH,
		WORLD_MAP_TILE_METATILES * WORLD_MAP_TILE_METATILES * 4);
	syDecompressByteLZ((Uint8 *)tileOffsets + tileOffsets[tile], decodedTile);

	/* A tile is 16 by 16 metatiles of 4 bytes. */
	srcTiles = decodedTile;
	quarterMetatiles = Ram_MetatileTiles + (((layer * 2 + y) * 32 + x) << 6);
	for(rowIdx = 0; rowIdx < WORLD_MAP_TILE_METATILES; rowIdx++) {
		syDMAXCopy(srcTiles, quarterMetatiles,
			DMA_COPY_WORDS(WORLD_MAP_TILE_METATILES), REG_DMA3);
		srcTiles += WORLD_MAP_TILE_METATILES * 2;
		quarterMetatiles += WORLD_MAP_METATILE_ROW_BYTES;
	}

	/* The affine BG map has one byte a tile, 64 to a row: each metatile's first
	   halfword picks a metatile's tiles from Ram_MapGridMetatileBuffer, whose
	   two halfwords are its top two tiles and its bottom two, a row (64 bytes)
	   below. */
	if(update != 0) {
		screenTiles = (Uint16 *)(BG_SCREEN_BLOCK(8) +
			((((layer * 2 + y) << 6) + x) << 5));
		srcTiles = decodedTile;
		for(rowIdx = 0; rowIdx < WORLD_MAP_TILE_METATILES; rowIdx++) {
			for(columnIdx = 0; columnIdx < WORLD_MAP_TILE_METATILES;
				columnIdx++) {
				Uint16 *bottomTiles;

				metatileTilesId = *srcTiles;
				screenTiles[0] = *(
					Uint16 *)(Ram_MapGridMetatileBuffer + metatileTilesId * 4);
				bottomTiles = (Uint16 *)(Ram_MapGridMetatileBuffer + 2);
				bottomTiles =
					(Uint16 *)((Uint8 *)bottomTiles + metatileTilesId * 4);
				screenTiles[WORLD_MAP_SCREEN_ROW_HALFWORDS] = *bottomTiles;
				screenTiles++;
				srcTiles += 2;
			}
			/* A row of metatiles is two map rows. */
			screenTiles +=
				2 * WORLD_MAP_SCREEN_ROW_HALFWORDS - WORLD_MAP_TILE_METATILES;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return 1;
}

/* Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
   camera state, the projection, the per-scanline ground plane function in RAM
   (fdWorldMapUpdateAffineLines) and its first tick, and the per-tick view and
   HBlank updates. */
UNREAD_RESULT fdWorldMapInitPerspective(void)
{
	WORLD_MAP_VIEW		   *perspective;
	BATTLE_CAMERA		   *camera;
	WORLD_MAP_AFFINE_STATE *affineState;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT];
	Sint32 *cameraPos;
	Sint32 *distance;
	Sint16 *zoomTransitionTicks;
	FIXED	farPlane;
	Sint32	tileIdx;

	/* Hide the backgrounds and sprites while their memory is replaced. */
	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScreenFadeFromBlack(0);
	perspective = syHeapSlotAlloc(HEAP_SLOT_MAP_GRID, sizeof(WORLD_MAP_VIEW));
	DMA_FILL32(0, perspective, sizeof(WORLD_MAP_VIEW));
	perspective->view[0] = 0;
	perspective->view[1] = 0;
	perspective->scaleX = toFIXED(32);
	perspective->scaleY = toFIXED(64);
	perspective->limitX = toFIXED(8160);
	perspective->limitY = toFIXED(8160);
	perspective->script = NULL;
	perspective->tiles =
		syResourceGetTableEntry(RESOURCE_WORLD_MAP_PERSPECTIVE_TILES);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_TILE_ANIMATION),
		MAPGRID_ANIMATION_SCRIPT);
	fdTilesetAnimStartAll((const Uint16 *)MAPGRID_ANIMATION_SCRIPT);
	fdIOWrite16Inline(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
			BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
		&REG_BLDCNT);
	fdIOWrite16Inline(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	REG_BLDY = 0;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_GRID_METATILES),
		Ram_MapGridMetatileBuffer);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_METATILE_TILES_ATTRIBUTES),
		Ram_MapGridShapes);
	/* The ground is BG3 and BG2, two 512-pixel affine layers that wrap (char
	   blocks 2 and 3, screen blocks 8 and 10); BG1 is a plain layer on screen
	   block 5. */
	perspective->shownLayers = DISPCNT_BG0_ON | DISPCNT_BG1_ON |
		DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
	/* fadeStep is where a map keeps wideTiles, read by the tileset
	   animations. */
	perspective->wideTiles = WORLD_MAP_WIDE_TILES;
	fdIOWrite16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
			BGCNT_SCREEN_BASE(8) | BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG3CNT);
	fdIOWrite16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) |
			BGCNT_SCREEN_BASE(10) | BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG2CNT);
	fdIOWrite16Inline(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
	IO_WRITE16(Q_8_8(1), &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	IO_WRITE16(Q_8_8(1), &REG_BG2PD);
	REG_BG2X = 0;
	REG_BG2Y = 0;
	IO_WRITE16(Q_8_8(1), &REG_BG3PA);
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	IO_WRITE16(Q_8_8(1), &REG_BG3PD);
	REG_BG3X = 0;
	IO_WRITE16(0, &REG_BG3Y);

	camera = syHeapSlotAllocEWRAM(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	affineState = syHeapSlotAlloc(
		HEAP_SLOT_WORLD_MAP_AFFINE, sizeof(WORLD_MAP_AFFINE_STATE));
	cameraPos = camera->focus;
	affinePages = affineState->pages;
	farPlane = toFIXED(510);
	perspective->farPlane = farPlane;
	distance = &perspective->distance;
	*distance = farPlane;
	perspective->zoom = toFIXED(1);
	zoomTransitionTicks = &perspective->zoomTransitionFrm;
	camera->followEye = 0;
	camera->followPos = 0;
	*zoomTransitionTicks = 0;
	/* A first view from the origin at pitch and yaw 0, so the scanline table
	   and the first affine page exist before the first tick; the projection's
	   centre is 120 by 96, as on the layered maps. */
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(farPlane, farPlane >> 1, farPlane << 1);
	SET_VECTOR(cameraPos, 0, 0, 0);
	syInitMatrix();
	syMatrixTranslateVector(cameraPos);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	{
		POINT eyeOfs;

		eyeOfs[X] = 0;
		eyeOfs[Y] = 0;
		eyeOfs[Z] = farPlane;
		syCalcPoint(eyeOfs, (Sint32 *)camera);
		syInitMatrix();
		syLookAtLoad(camera->eye, cameraPos);
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE, fdWorldMapUpdateAffineLines,
			&fdWorldMapUpdateAffineLinesCodeSize);
		fdWorldMapBuildScanlineTable(
			syDivFX(syCos(perspective->pitch), sySin(perspective->pitch)),
			cameraPos, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
		((WORLD_MAP_PLANE_FN)HeapSlots[HEAP_SLOT_RAM_CODE])(camera, cameraPos,
			affineState->scanlines, affinePages[TickCount & 1]);
		/* Then the world map's own view: pitch TRIG_TURN * 7 / 8 (minus an
		   eighth of a turn) and yaw 0, with the eye one pixel beyond the
		   distance. */
		SET_VECTOR(cameraPos, 0, 0, 0);
		syInitMatrix();
		perspective->pitch = TRIG_TURN * 7 / 8;
		perspective->yaw = 0;
		syInitMatrix();
		syMatrixTranslateVector(cameraPos);
		syRotY(perspective->yaw);
		syRotX(perspective->pitch);
		eyeOfs[X] = 0;
		eyeOfs[Y] = 0;
		eyeOfs[Z] = *distance + toFIXED(1);
		syCalcPoint(eyeOfs, camera->eye);
	}
	REG_MOSAIC = 0;
	IO_WRITE16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	BgScrollWork[2].x = 0;
	BgScrollWork[2].y = 0;
	BgScrollWork[3].x = 0;
	BgScrollWork[3].y = 0;
	/* Rows 0 to 159: the whole screen shows the perspective view. */
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySetEvent(fdWorldMapUpdateView, EVENT_PASS_TICK + 5);
	sySetEvent(fdWorldMapApplyAffinePage, EVENT_PASS_VBLANK);
	/* Each of the 16 by 16 grid metatiles starts on the tile of its own
	   number. */
	for(tileIdx = 0; tileIdx < WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS;
		tileIdx++)
		perspective->tileIds[tileIdx] = tileIdx;
}

/* Puts world map tile (x / 16, y / 16) at metatile (metatileX / 16, metatileY /
   16) of the tile id grid, and draws it at once when it is next to the view.
   metatileX and metatileY count 32-pixel world map metatiles, so a grid
   metatile is a 512-pixel tile; it is next to the view when metatileX / 8 and
   metatileY / 8, in 256-pixel steps, are within one step of the followed
   point's. */
void fdWorldMapSetGridTile(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	WORLD_MAP_VIEW *perspective;
	Sint32		   *followedPos;
	Sint32			originX;
	Sint32			originY;
	Sint32			tileId;

	perspective = MapGridState[0];
	originX = 0;
	originY = 0;
	followedPos = perspective->origin;
	if(followedPos != 0) {
		originX = *followedPos++;
		originY = followedPos[1];
	}

	/* The followed point and metatileX, metatileY in 256-pixel steps. */
	originX >>= 24;
	originY >>= 24;
	x >>= 4;
	y >>= 4;
	metatileX >>= 3;
	metatileY >>= 3;
	tileId = (y << 4) + x;
	perspective
		->tileIds[(((metatileY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 4) +
			((metatileX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tileId;

	if(ABS(originX - metatileX) <= 1 && ABS(originY - metatileY) <= 1) {
		fdWorldMapWriteLayerMetatileTile(
			0, metatileX / 2, metatileY / 2, tileId, 1);
		fdWorldMapWriteLayerMetatileTile(
			1, metatileX / 2, metatileY / 2, tileId + WORLD_MAP_LAYER_TILES, 1);
	}
}

/* Loads the world map's palette and tiles for the area at (x, z): the second
   set where the ground there is WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD. */
void fdWorldMapLoadGraphics(Sint32 x, Sint32 z)
{
	WORLD_MAP_VIEW *perspective;
	Uint32		   *resources;
	Uint8		   *buf;
	Uint8		   *tiles;
	Uint32		   *screenTiles;
	Uint32			tilePair;
	Sint32			graphicsSet;
	Sint32			screenRow;
	Sint32			screenColumn;
	Sint16			backdropCol;

	graphicsSet = 0;
	buf = syHeapAlloc(512);
	perspective = MapGridState[0];
	/* The 32-pixel metatile under (x, z), 32 metatiles to a row. */
	if(WORLD_MAP_METATILES[((x / toFIXED(32)) & 31) +
		   (((z / toFIXED(32)) & 31) << 5)]
			.kind == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
		graphicsSet = 1;
	resources = WorldMapGraphicsResources[graphicsSet];
	perspective->resources = resources;
	backdropCol = WORLD_MAP_BG_PALETTE[0];
	/* Palettes 0 to 13, keeping the backdrop colour. */
	syDecompressByteLZ(syResourceGetTableEntry(resources[0]), buf);
	*(Sint16 *)buf = backdropCol;
	syDMAXCopy(buf, WORLD_MAP_BG_PALETTE,
		DMA_COPY_WORDS(14 * PALETTE_COLORS / 2), REG_DMA3);
	tiles = BgTileBuffer;
	syDecompressTaggedLZ(syResourceGetTableEntry(resources[1]), tiles);
	syDMAXCopy(tiles, BG_CHAR_BLOCK(2),
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(syResourceGetTableEntry(resources[2]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES,
		BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(syResourceGetTableEntry(resources[3]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2,
		BG_CHAR_BLOCK(3), DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4),
		REG_DMA3);
	syDecompressTaggedLZ(syResourceGetTableEntry(resources[4]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3,
		BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[5]), MapGridAnimTiles);
	/* BG1's map (screen block 5) is filled with tile 127 of WINDOW_PALETTE, and
	   screen block 6 numbers a 30 by 20 tile image from
	   WORLD_MAP_STILL_FIRST_TILE, two tile numbers a word, for the still image
	   fdWorldMapStartStillImage shows. */
	DMA_FILL32(WINDOW_TILE_PAIR(WINDOW_TILE(127)), BG_SCREEN_BLOCK(5), 1536);
	screenTiles = (Uint32 *)BG_SCREEN_BLOCK(6);
	tilePair = MAP_ENTRY_PAIR(
		WORLD_MAP_STILL_FIRST_TILE, WORLD_MAP_STILL_FIRST_TILE + 1);
	for(screenRow = 0; screenRow < DISPLAY_TILE_HEIGHT; screenRow++) {
		for(screenColumn = 0; screenColumn < DISPLAY_TILE_WIDTH / 2;
			screenColumn++) {
			*screenTiles++ = tilePair;
			tilePair += MAP_ENTRY_PAIR(2, 2);
		}
		screenTiles++;
	}
	/* On the snowfield set, six grid metatiles of the snowy north show their
	   own tiles. */
	if(graphicsSet == 1) {
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 1);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 1);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 1);
		fdWorldMapUpdateCurrentTileBlock();
	}
	syHeapRelease(buf);
}

/* HBlank setup each VBlank on the world map: copies the first scanline's BG2
   and BG3 affine parameters, starts the HBlank DMA for the rest, and picks the
   BG mode from the window's top and bottom. */
void fdWorldMapApplyAffinePage(void)
{
	WORLD_MAP_AFFINE_PAIR(*affinePages)
	[DISPLAY_HEIGHT] =
		((WORLD_MAP_AFFINE_STATE *)HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE))
			->pages;
	Sint32			displayControl = (Sint16)(REG_DISPCNT & ~DISPCNT_MODE_MASK);
	Uint32		   *affineRegisters = (Uint32 *)&REG_BG2PA;
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	volatile Uint16 *dma0 = REG_DMA0;
	Uint32			*affineSrc;
	Uint32			 mode;
	Uint32			 winTop;
	Uint32			 nextDisplayControl;

	/* Stop the last VBlank's HBlank DMA, then load line 0's eight words (BG2PA
	   to BG3Y) by hand; the DMA repeats eight words into the same registers at
	   each HBlank for the lines after it. */
	dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
	(void)dma0[DMA_CNT_H_INDEX];
	if(affinePages != NULL) {
		affineSrc = (Uint32 *)affinePages[TickCount & 1];
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters++ = *affineSrc++;
		*affineRegisters = *affineSrc++;
		syDMAXCopy(affineSrc, (void *)&REG_BG2PA,
			(DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT |
				DMA_DEST_RELOAD)
					<< 16 |
				8,
			(volatile Uint32 *)dma0);
	}
	perspective->shownTop = perspective->winTop;
	perspective->shownBottom = perspective->winBottom;
	/* DISPCNT_MODE_2 (the affine ground) for the load's rows 0 to 159;
	   DISPCNT_MODE_0 for fdWorldMapStartStillImage's 200 and 255, which show
	   the still image on BG1. */
	winTop = perspective->shownTop;
	mode = DISPCNT_MODE_0;
	if(winTop < WORLD_MAP_STILL_WINDOW_TOP) {
		Uint16 bottom = perspective->shownBottom;

		mode = (bottom != 0) << 1;
		if(winTop <= bottom) {
			mode = DISPCNT_MODE_0;
			if(winTop == 0)
				mode = DISPCNT_MODE_2;
		}
	}
	displayControl |= mode;
	nextDisplayControl = (Uint16)displayControl;
	REG_DISPCNT = nextDisplayControl;
	perspective->winPending = 0;
}

/* Copies one world map metatile's two tile entries (top and bottom) to a BG
   map. */
static __inline__ void fdWorldMapCopyMetatileTiles(
	Uint8 *metatileBytes, Uint8 *screenTile)
{
	Uint32	metatileTilesIdx = *(Uint16 *)metatileBytes;
	Uint16 *metatileTiles = (Uint16 *)Ram_MapGridMetatileBuffer;

	metatileTiles += metatileTilesIdx * 2;
	*(Uint16 *)screenTile = *metatileTiles++;
	*(Uint16 *)(screenTile + WORLD_MAP_SCREEN_WIDTH) = *metatileTiles;
}

/* Draws row tileRow of the metatile buffer into both layers' BG maps, 32
   metatiles in each: the row of the ground that scrolls into view. */
void fdWorldMapDrawMetatileRow(Uint32 tileRow)
{
	Uint8 *srcMetatile;
	Uint8 *screenTile;
	Uint32 metatilesCopied;

	srcMetatile = Ram_MetatileTiles + ((((Sint32)tileRow / 2) & 31) << 7);
	screenTile = (Uint8 *)((Uint32)BG_SCREEN_BLOCK(8) + ((tileRow & 62) << 6));
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += 2;
		srcMetatile += 4;
	} while(metatilesCopied < WORLD_MAP_BUFFER_METATILES);

	/* The same row of the second layer. */
	screenTile += WORLD_MAP_LAYER_SCREEN_BYTES - WORLD_MAP_BUFFER_METATILES * 2;
	srcMetatile +=
		WORLD_MAP_LAYER_METATILE_BYTES - WORLD_MAP_METATILE_ROW_BYTES;
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += 2;
		srcMetatile += 4;
	} while(metatilesCopied < WORLD_MAP_BUFFER_METATILES);
}

/* Draws column tileColumn of the metatile buffer into both layers' BG maps, 32
   metatiles down in each (the second layer's map and metatiles follow the
   first's):
   the column of the ground that scrolls into view. */
void fdWorldMapDrawMetatileColumn(Uint32 tileColumn)
{
	Uint8 *srcMetatile;
	Uint8 *screenTile;
	Uint32 metatilesCopied;

	srcMetatile = Ram_MetatileTiles + ((((Sint32)tileColumn / 2) & 31) << 2);
	screenTile = (Uint8 *)((Uint32)BG_SCREEN_BLOCK(8) + (tileColumn & 62));
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += WORLD_MAP_SCREEN_WIDTH * 2;
		srcMetatile += WORLD_MAP_METATILE_ROW_BYTES;
	} while(metatilesCopied < 2 * WORLD_MAP_BUFFER_METATILES);
}

/* Each tick on the world map: loads tiles as the view moves, shakes and
   follows the player, draws the BG row or column that scrolls into view, moves
   the camera, and builds the ground plane's scanline table. */
void fdWorldMapUpdateView(void)
{
	BATTLE_CAMERA		   *camera = BattleCameraWork;
	WORLD_MAP_AFFINE_STATE *affineState =
		HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	Sint32		   *cameraPos = camera->focus;
	Sint32		   *followedPos = perspective->origin;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT] = affineState->pages;
	Sint32 distance = perspective->farPlane;
	Sint32 height = perspective->distance;
	POINT  eyeOfs;

	fdWorldMapPreloadCurrentTileBlock();
	if(followedPos != NULL) {
		FIXED  x;
		FIXED  y;
		Sint32 tileColumn;
		Sint32 tileRow;

		y = followedPos[2];
		x = followedPos[0];
		if(perspective->shakeX != 0) {
			Sint32 shakeDifference = syRandom();
			Sint32 shakeAmplitude;

			shakeDifference -= syRandom();
			shakeAmplitude = perspective->shakeX;
			x += syMulFX(shakeAmplitude, shakeDifference);
			perspective->shakeX =
				syMulFX(shakeAmplitude, perspective->shakeDecay);
		}
		if(perspective->shakeY != 0) {
			Sint32 shakeDifference = syRandom();
			FIXED  shakeAmplitude;

			shakeDifference -= syRandom();
			shakeAmplitude = perspective->shakeY;
			y += syMulFX(shakeAmplitude, shakeDifference);
			perspective->shakeY =
				syMulFX(shakeAmplitude, perspective->shakeDecay);
		}
		/* On crossing a 16-pixel line, draw the column 16 on that side, or the
		   row 12 on when y grows and 18 back when it falls. */
		tileColumn = x / toFIXED(16);
		tileRow = y / toFIXED(16);
		if((perspective->view[0] ^ x) & toFIXED(16)) {
			if(perspective->view[0] < x)
				fdWorldMapDrawMetatileColumn(tileColumn + 16);
			else
				fdWorldMapDrawMetatileColumn(tileColumn - 16);
		}
		if((perspective->view[1] ^ y) & toFIXED(16)) {
			if(perspective->view[1] < y)
				fdWorldMapDrawMetatileRow(tileRow + 12);
			else
				fdWorldMapDrawMetatileRow(tileRow - 18);
		}
		perspective->view[0] = x;
		perspective->view[1] = y;
	}
	/* Rebuild the projection and look at the followed position from the current
	   pitch and yaw, the eye one pixel beyond the zoomed distance.
	   followedPos is read here without the NULL test above, which never
	   fails on the world map: fdModeRun's fdActorResetForLeader points it at
	   the camera actor before any tick passes, and nothing clears it. */
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(distance, height / 2, height * 2);
	SET_VECTOR(cameraPos, *followedPos++, 0, followedPos[1]);
	syInitMatrix();
	syMatrixTranslateVector(cameraPos);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	eyeOfs[X] = 0;
	eyeOfs[Y] = 0;
	eyeOfs[Z] = height + toFIXED(1);
	syCalcPoint(eyeOfs, (Sint32 *)camera);
	syInitMatrix();
	syLookAtLoad(camera->eye, cameraPos);
	/* The scanline table depends only on the pitch, so it is rebuilt only when
	   the pitch has changed. */
	if(ScanlinePitch != perspective->pitch) {
		FIXED cosine = syCos(perspective->pitch);
		FIXED sine = sySin(perspective->pitch);

		fdWorldMapBuildScanlineTable(
			syDivFX(cosine, sine), cameraPos, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
	}
	((WORLD_MAP_PLANE_FN)HeapSlots[HEAP_SLOT_RAM_CODE])(
		camera, cameraPos, affineState->scanlines, affinePages[TickCount & 1]);
}

/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlock(void)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	Sint32			firstQuarterX = 0;
	Sint32			firstQuarterY = 0;
	Uint32			layer;
	Uint32			row;
	Uint32			col;
	Sint32			layerTileBase;
	Sint32			tileId;

	if(perspective->origin != NULL) {
		Sint32 *followedPos = perspective->origin;

		firstQuarterX = *followedPos++;
		firstQuarterY = followedPos[1];
	}
	/* The top left of the 2x2 tiles around the point, 256 pixels left of it and
	   320 above. */
	firstQuarterX = WORLD_MAP_FIXED_TO_TILE(firstQuarterX - toFIXED(256));
	firstQuarterY = WORLD_MAP_FIXED_TO_TILE(firstQuarterY - toFIXED(320));
	layer = 0;
	layerTileBase = 0;
	for(; layer < 2; layer++) {
		for(row = 0; row < 2; row++) {
			for(col = 0; col < 2; col++) {
				tileId = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1))
							 << 4) +
					((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
				tileId = perspective->tileIds[tileId];
				tileId += layerTileBase;
				fdWorldMapWriteLayerMetatileTile(
					layer, firstQuarterX + col, firstQuarterY + row, tileId, 1);
			}
		}
		layerTileBase += WORLD_MAP_LAYER_TILES;
	}
}

/* Loads the first of the 2x2 world map tiles around the player that is not
   shown yet (at most one a tick), without drawing it. */
void fdWorldMapPreloadCurrentTileBlock(void)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	Sint32			firstQuarterX = 0;
	Sint32			firstQuarterY = 0;
	Uint32			layer;
	Uint32			row;
	Uint32			col;
	Sint32			tileId;

	if(perspective->origin != NULL) {
		Sint32 *followedPos = perspective->origin;

		firstQuarterX = *followedPos++;
		firstQuarterY = followedPos[1];
	}
	firstQuarterX = WORLD_MAP_FIXED_TO_TILE(firstQuarterX - toFIXED(256));
	firstQuarterY = WORLD_MAP_FIXED_TO_TILE(firstQuarterY - toFIXED(320));
	for(layer = 0; layer < 2; layer++) {
		for(row = 0; row < 2; row++) {
			for(col = 0; col < 2; col++) {
				tileId = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1))
							 << 4) +
					((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
				tileId = perspective->tileIds[tileId];
				tileId += layer * WORLD_MAP_LAYER_TILES;
				if(fdWorldMapWriteLayerMetatileTile(layer, firstQuarterX + col,
					   firstQuarterY + row, tileId, 0) != 0)
					return;
			}
		}
	}
}

/* VBlank: shows BG1 and copies the decoded picture's tiles into VRAM. */
void fdWorldMapPresentStillImage(void)
{
	IO_WRITE16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
		&REG_BG1CNT);
	syDMAXCopy(MapGridMetatileBuffer,
		BG_CHAR_BLOCK(0) + WORLD_MAP_STILL_FIRST_TILE * TILE_SIZE_8BPP,
		DMA_COPY_WORDS(WORLD_MAP_STILL_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
}

/* Freezes the world map's perspective view as a still image on BG1: saves BG1's
   tiles, draws the current affine frame into pixels and hands the window and
   VBlank to it. */
void fdWorldMapStartStillImage(void)
{
	WORLD_MAP_AFFINE_STATE *affineState =
		HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
	WORLD_MAP_VIEW *perspective = MapGridState[0];

	perspective->anmActive = 1;
	sySuspendEvent(fdTilesetAnimUpdate);
	syDMAXCopy(BG_CHAR_BLOCK(1), DecodeBuffer,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	sySynch(1);
	fdTileMapDrawRowsFromRam(
		affineState->pages[TickCount & 1], (Uint8 *)MapGridMetatileBuffer);
	perspective->winTop = WORLD_MAP_STILL_WINDOW_TOP;
	perspective->winBottom = WORLD_MAP_STILL_WINDOW_BOTTOM;
	VBlankCallback = fdWorldMapPresentStillImage;
}

/* VBlank at the end of the still image: puts BG1 back as the plain layer and
   copies the area's four tile sets back into char blocks 2 and 3. */
void fdWorldMapEndStillImage(void)
{
	IO_WRITE16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
	syDMAXCopy(BgTileBuffer, BG_CHAR_BLOCK(2),
		DMA_COPY_WORDS(4 * WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
}

/* Reloads the world map area's palette and tiles after the still image, puts
   the default metatiles back and returns to the perspective view. */
void fdWorldMapLoadAreaGraphics(void)
{
	WORLD_MAP_VIEW *perspective;
	Uint32		   *graphicsResources;
	Uint8		   *decodeBuf;
	Sint16			backdropCol;

	perspective = MapGridState[0];
	decodeBuf = (Uint8 *)MapGridMetatileBuffer;
	graphicsResources = perspective->resources;
	backdropCol = *(Sint16 *)BG_PLTT;
	syDecompressByteLZ(
		syResourceGetTableEntry(graphicsResources[0]), decodeBuf);
	*(Sint16 *)decodeBuf = backdropCol;
	syDMAXCopy(decodeBuf, (void *)BG_PLTT,
		DMA_COPY_WORDS(14 * PALETTE_COLORS / 2), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(graphicsResources[1]), BgTileBuffer);
	syDecompressTaggedLZ(syResourceGetTableEntry(graphicsResources[2]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
	syDecompressTaggedLZ(syResourceGetTableEntry(graphicsResources[3]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
	syDecompressTaggedLZ(syResourceGetTableEntry(graphicsResources[4]),
		Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
	/* The VBlank copies the tiles back and the next tick shows the ground;
	   only then is the still image's buffer refilled with the metatiles. */
	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	syDecompressTaggedLZ(
		syResourceGetTableEntry((Uint32)RESOURCE_DEFAULT_GRID_METATILES),
		decodeBuf);
	fdWorldMapUpdateCurrentTileBlock();
	perspective->anmActive = 0;
	syResumeEvent(fdTilesetAnimUpdate);
	sySynch(1);
}

/* Puts the default world map metatiles back after the still image and returns
   to the perspective view, without reloading the area's palette or tiles. */
void fdWorldMapLoadDefaultMetatilesAndUpdateBlock(void)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_GRID_METATILES),
		MapGridMetatileBuffer);
	fdWorldMapUpdateCurrentTileBlock();
	syResumeEvent(fdTilesetAnimUpdate);
	sySynch(1);
}

#undef WORLD_MAP_BG_PALETTE
#undef WORLD_MAP_METATILES
#undef WORLD_MAP_TILE_METATILES
#undef WORLD_MAP_TILES_ACROSS
#undef WORLD_MAP_LAYER_TILES
#undef WORLD_MAP_ALTERNATE_TILES
#undef WORLD_MAP_FIXED_TO_TILE
#undef WORLD_MAP_BUFFER_METATILES
#undef WORLD_MAP_METATILE_ROW_BYTES
#undef WORLD_MAP_LAYER_METATILE_BYTES
#undef WORLD_MAP_SCREEN_WIDTH
#undef WORLD_MAP_SCREEN_ROW_HALFWORDS
#undef WORLD_MAP_LAYER_SCREEN_BYTES
#undef WORLD_MAP_WIDE_TILES
#undef WORLD_MAP_STILL_WINDOW_TOP
#undef WORLD_MAP_STILL_WINDOW_BOTTOM
#undef WORLD_MAP_STILL_TILES
#undef WORLD_MAP_USE_ALTERNATE_TILE
