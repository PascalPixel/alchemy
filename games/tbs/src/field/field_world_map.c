/* @file */
/* The world map: its perspective view, its tiles, and the still image of */
/* the view BG1 shows while a menu or event runs. */
/* Part of field.c. */

#include "field_internal.h"

/* scIoWrite16 as an inline function. */
static __inline__ void fdIoWrite16Inline(s32 value, volatile u16 *reg) {
	*reg = value;
}

/* fdWorldMapUpdateAffineLines, run from RAM: works out each screen line's BG2
 */
/* and BG3 affine parameters for the camera into one of the two affine pages. */
typedef s32 (*WORLD_MAP_PLANE_FN)(void *camera, s32 *position, void *lines,
								  void *out);

/* One 32-pixel cell of the world map: its tile, its kind */
/* (WORLD_MAP_ENCOUNTER_REGION) and a flag. */
typedef struct WORLD_MAP_CELL {
	u32 tile : 24; /* low halfword: the cell tiles fdWorldMapCopyCellTiles draws
					*/
	u32 kind : 7;  /* the encounter region; encounters read it too */
	/* Selects the second row of WorldMap_BattleEnvironments */
	/* (fdWorldMapGetBattleEnvironment); a WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD
	 */
	/* cell takes the third row whatever this bit says. */
	u32 flg : 1;
} WORLD_MAP_CELL;

#define WORLD_MAP_BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_MAP_CELLS ((WORLD_MAP_CELL *)Ram_CellTiles)

/* A world map tile is 16 by 16 cells. */
#define WORLD_MAP_TILE_CELLS 16
/* The world map is 16 by 16 tiles, 512 pixels each, wrapping; each layer's */
/* tile numbers follow the 320 before it. */
#define WORLD_MAP_TILES_ACROSS 16
#define WORLD_MAP_LAYER_TILES  320
/* The tiles after the world's 256 in a layer: other versions of a few grid */
/* cells. */
#define WORLD_MAP_ALTERNATE_TILES                                              \
	(WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS)
/* 16.16 pixels to whole 512-pixel tiles. */
#define WORLD_MAP_FIXED_TO_TILE(v) ((v) >> 25)
/* Each layer's cell buffer holds the 2 by 2 tiles around the view: 32 cells */
/* a side, 4 bytes a cell. */
#define WORLD_MAP_BUFFER_CELLS	 32
#define WORLD_MAP_CELL_ROW_BYTES (WORLD_MAP_BUFFER_CELLS * 4)
#define WORLD_MAP_LAYER_CELL_BYTES                                             \
	(WORLD_MAP_BUFFER_CELLS * WORLD_MAP_CELL_ROW_BYTES)
/* An affine layer's BG map: 64 tiles a row, one byte each, two screen blocks */
/* in all. */
#define WORLD_MAP_SCREEN_WIDTH 64
/* A map row as the halfwords that write it two tiles at a time. */
#define WORLD_MAP_SCREEN_ROW_HALFWORDS (WORLD_MAP_SCREEN_WIDTH / 2)
#define WORLD_MAP_LAYER_SCREEN_BYTES                                           \
	(WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_WIDTH)
/* MAP_GRID_SCROLL_STATE's wideTiles, which the world map shares: any nonzero */
/* value has the tileset animations copy 64-byte tiles. */
#define WORLD_MAP_WIDE_TILES 128

/* The window rows fdWorldMapStartStillImage sets, below the screen: the */
/* frame update then picks mode 0 and BG1 shows the still image. */
#define WORLD_MAP_STILL_WINDOW_TOP	  200
#define WORLD_MAP_STILL_WINDOW_BOTTOM 255
#define WORLD_MAP_STILL_TILES		  (DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT)

/* Grid cell (column, row) shows its alternate tile. */
#define WORLD_MAP_USE_ALTERNATE_TILE(perspective, column, row)                 \
	((perspective)->tileIds[(row) * WORLD_MAP_TILES_ACROSS + (column)] =       \
		 WORLD_MAP_ALTERNATE_TILES + (row) * WORLD_MAP_TILES_ACROSS +          \
		 (column))

/* Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's cell */
/* picture buffer, decompressing it from the area's graphics, and with update */
/* set also draws it into the layer's BG map. */
s32 fdWorldMapWriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile,
								 s32 update) {
	WORLD_MAP_VIEW *perspective = MapGridState[0];
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
	decodedTile =
		syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH,
						WORLD_MAP_TILE_CELLS * WORLD_MAP_TILE_CELLS * 4);
	fdDecompressByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);

	/* A tile is 16 by 16 cells of 4 bytes. */
	sourceTiles = decodedTile;
	quarterCells = Ram_CellTiles + (((layer * 2 + y) * 32 + x) << 6);
	for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
		syDMAXCopy(sourceTiles, quarterCells,
				   DMA_COPY_WORDS(WORLD_MAP_TILE_CELLS), REG_DMA3);
		sourceTiles += WORLD_MAP_TILE_CELLS * 2;
		quarterCells += WORLD_MAP_CELL_ROW_BYTES;
	}

	/* The affine BG map has one byte a tile, 64 to a row: each cell's first */
	/* halfword picks a cell's tiles from Ram_MapGridCellBuffer, whose two */
	/* halfwords are its top two tiles and its bottom two, a row (64 bytes)
	 * below. */
	if(update != 0) {
		screenTiles =
			(u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
		sourceTiles = decodedTile;
		for(rowIndex = 0; rowIndex < WORLD_MAP_TILE_CELLS; rowIndex++) {
			for(columnIndex = 0; columnIndex < WORLD_MAP_TILE_CELLS;
				columnIndex++) {
				u16 *bottomTiles;

				cellTilesId = *sourceTiles;
				screenTiles[0] =
					*(u16 *)(Ram_MapGridCellBuffer + cellTilesId * 4);
				bottomTiles = (u16 *)(Ram_MapGridCellBuffer + 2);
				bottomTiles = (u16 *)((u8 *)bottomTiles + cellTilesId * 4);
				screenTiles[WORLD_MAP_SCREEN_ROW_HALFWORDS] = *bottomTiles;
				screenTiles++;
				sourceTiles += 2;
			}
			/* A row of cells is two map rows. */
			screenTiles +=
				2 * WORLD_MAP_SCREEN_ROW_HALFWORDS - WORLD_MAP_TILE_CELLS;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return 1;
}

/* Sets up the world map's perspective view: BG2 and BG3 in affine mode, the */
/* camera state, the projection, the per-scanline ground plane function in RAM
 */
/* (fdWorldMapUpdateAffineLines) and its first frame, and the per-frame view */
/* and HBlank updates. */
UNREAD_RESULT fdWorldMapInitPerspective(void) {
	WORLD_MAP_VIEW *perspective;
	BATTLE_CAMERA *camera;
	WORLD_MAP_AFFINE_STATE *affineState;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT];
	s32 *cameraPosition;
	s32 *distance;
	s16 *zoomTransitionFrames;
	FIXED farPlane;
	s32 tileIndex;

	/* Hide the backgrounds and sprites while their memory is replaced. */
	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
					 DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScrFadeFromBlack(0);
	perspective = syHeapSlotAlloc(HEAP_SLOT_MAP_GRID, sizeof(WORLD_MAP_VIEW));
	btDmaFill32(0, perspective, sizeof(WORLD_MAP_VIEW));
	perspective->view[0] = 0;
	perspective->view[1] = 0;
	perspective->scaleX = FIXED_ONE * 32;
	perspective->scaleY = FIXED_ONE * 64;
	perspective->limitX = toFIXED(8160);
	perspective->limitY = toFIXED(8160);
	perspective->script = NULL;
	perspective->tiles =
		syResourceGetTableEntry(RESOURCE_WORLD_MAP_PERSPECTIVE_TILES);
	fdDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_DEFAULT_TILE_ANIMATION),
		MAPGRID_ANIMATION_SCRIPT);
	fdTilesetAnimStartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
	fdIoWrite16Inline(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
						  BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN |
						  BLDCNT_TGT2_ALL,
					  &REG_BLDCNT);
	fdIoWrite16Inline(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	REG_BLDY = 0;
	fdDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_DEFAULT_GRID_CELLS),
						 Ram_MapGridCellBuffer);
	fdDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_DEFAULT_CELL_TILES_ATTRIBUTES),
		Ram_MapGridShapes);
	/* The ground is BG3 and BG2, two 512-pixel affine layers that wrap (char */
	/* blocks 2 and 3, screen blocks 8 and 10); BG1 is a plain layer on screen
	 */
	/* block 5. */
	perspective->shownLayers = DISPCNT_BG0_ON | DISPCNT_BG1_ON |
							   DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
	/* fadeStep is where a map keeps wideTiles, read by the tileset animations.
	 */
	perspective->wideTiles = WORLD_MAP_WIDE_TILES;
	fdIoWrite16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
						  BGCNT_SCREEN_BASE(8) | BGCNT_WRAP | BGCNT_SIZE(2),
					  &REG_BG3CNT);
	fdIoWrite16Inline(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) |
						  BGCNT_SCREEN_BASE(10) | BGCNT_WRAP | BGCNT_SIZE(2),
					  &REG_BG2CNT);
	fdIoWrite16Inline(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
	scIoWrite16(Q_8_8(1), &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	scIoWrite16(Q_8_8(1), &REG_BG2PD);
	REG_BG2X = 0;
	REG_BG2Y = 0;
	scIoWrite16(Q_8_8(1), &REG_BG3PA);
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	scIoWrite16(Q_8_8(1), &REG_BG3PD);
	REG_BG3X = 0;
	scIoWrite16(0, &REG_BG3Y);

	camera = syHeapSlotAllocEwram(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	affineState = syHeapSlotAlloc(HEAP_SLOT_WORLD_MAP_AFFINE,
								  sizeof(WORLD_MAP_AFFINE_STATE));
	cameraPosition = camera->focus;
	affinePages = affineState->pages;
	farPlane = toFIXED(510);
	perspective->farPlane = farPlane;
	distance = &perspective->distance;
	*distance = farPlane;
	perspective->zoom = FIXED_ONE;
	zoomTransitionFrames = &perspective->zoomTransitionFrm;
	camera->followEye = 0;
	camera->followPos = 0;
	*zoomTransitionFrames = 0;
	/* A first view from the origin at pitch and yaw 0, so the scanline */
	/* table and the first affine page exist before the first frame; the */
	/* projection's centre is 120 by 96, as on the layered maps. */
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(farPlane, farPlane >> 1, farPlane << 1);
	SET_VECTOR(cameraPosition, 0, 0, 0);
	syInitMatrix();
	syMatrixTranslateVector(cameraPosition);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	{
		POINT eyeOffset;

		eyeOffset[X] = 0;
		eyeOffset[Y] = 0;
		eyeOffset[Z] = farPlane;
		syCalcPoint(eyeOffset, (s32 *)camera);
		syInitMatrix();
		syMatrixSetLookAt(camera->eye, cameraPosition);
		syRamCodeLoadSlot(HEAP_SLOT_RAM_CODE, fdWorldMapUpdateAffineLines,
						  &fdWorldMapUpdateAffineLinesCodeSize);
		fdWorldMapBuildScanlineTable(
			syDivFX(syCos(perspective->pitch), sySin(perspective->pitch)),
			cameraPosition, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
		((WORLD_MAP_PLANE_FN)HeapSlots[HEAP_SLOT_RAM_CODE])(
			camera, cameraPosition, affineState->scanlines,
			affinePages[FrameCount & 1]);
		/* Then the world map's own view: pitch TRIG_TURN * 7 / 8 (minus an
		 * eighth */
		/* of a turn) and yaw 0, with the eye one pixel beyond the distance. */
		SET_VECTOR(cameraPosition, 0, 0, 0);
		syInitMatrix();
		perspective->pitch = TRIG_TURN * 7 / 8;
		perspective->yaw = 0;
		syInitMatrix();
		syMatrixTranslateVector(cameraPosition);
		syRotY(perspective->yaw);
		syRotX(perspective->pitch);
		eyeOffset[X] = 0;
		eyeOffset[Y] = 0;
		eyeOffset[Z] = *distance + FIXED_ONE;
		syCalcPoint(eyeOffset, camera->eye);
	}
	REG_MOSAIC = 0;
	scIoWrite16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	BgScrollWork[2].x = 0;
	BgScrollWork[2].y = 0;
	BgScrollWork[3].x = 0;
	BgScrollWork[3].y = 0;
	/* Rows 0 to 159: the whole screen shows the perspective view. */
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySetEvent(fdWorldMapUpdateView, TASK_PASS_FRAME + 5);
	sySetEvent(fdWorldMapApplyAffinePage, TASK_PASS_VBLANK);
	/* Each of the 16 by 16 grid cells starts on the tile of its own number. */
	for(tileIndex = 0;
		tileIndex < WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS;
		tileIndex++)
		perspective->tileIds[tileIndex] = tileIndex;
}

/* Puts world map tile (x / 16, y / 16) at cell (cellX / 16, cellY / 16) of the
 */
/* tile id grid, and draws it at once when it is next to the view. cellX and */
/* cellY count 32-pixel world map cells, so a grid cell is a 512-pixel tile; it
 */
/* is next to the view when cellX / 8 and cellY / 8, in 256-pixel steps, are */
/* within one step of the followed point's. */
void fdWorldMapSetGridTile(s32 x, s32 y, s32 cellX, s32 cellY) {
	WORLD_MAP_VIEW *perspective;
	s32 *followedPosition;
	s32 originX;
	s32 originY;
	s32 tileId;

	perspective = MapGridState[0];
	originX = 0;
	originY = 0;
	followedPosition = perspective->origin;
	if(followedPosition != 0) {
		originX = *followedPosition++;
		originY = followedPosition[1];
	}

	/* The followed point and cellX, cellY in 256-pixel steps. */
	originX >>= 24;
	originY >>= 24;
	x >>= 4;
	y >>= 4;
	cellX >>= 3;
	cellY >>= 3;
	tileId = (y << 4) + x;
	perspective->tileIds[(((cellY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 4) +
						 ((cellX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tileId;

	if(ABS(originX - cellX) <= 1 && ABS(originY - cellY) <= 1) {
		fdWorldMapWriteLayerCellTile(0, cellX / 2, cellY / 2, tileId, 1);
		fdWorldMapWriteLayerCellTile(1, cellX / 2, cellY / 2,
									 tileId + WORLD_MAP_LAYER_TILES, 1);
	}
}

/* Loads the world map's palette and tiles for the area at (x, z): the second */
/* set where the ground there is WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD. */
void fdWorldMapLoadGraphics(s32 x, s32 z) {
	WORLD_MAP_VIEW *perspective;
	u32 *resources;
	u8 *buffer;
	u8 *tiles;
	u32 *screenTiles;
	u32 tilePair;
	s32 graphicsSet;
	s32 screenRow;
	s32 screenColumn;
	s16 backdropColor;

	graphicsSet = 0;
	buffer = syHeapAlloc(512);
	perspective = MapGridState[0];
	/* The 32-pixel cell under (x, z), 32 cells to a row. */
	if(WORLD_MAP_CELLS[((x / toFIXED(32)) & 31) +
					   (((z / toFIXED(32)) & 31) << 5)]
		   .kind == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
		graphicsSet = 1;
	resources = WorldMap_GraphicsResources[graphicsSet];
	perspective->resources = resources;
	backdropColor = WORLD_MAP_BG_PALETTE[0];
	/* Palettes 0 to 13, keeping the backdrop colour. */
	fdDecompressByteLz(syResourceGetTableEntry(resources[0]), buffer);
	*(s16 *)buffer = backdropColor;
	syDMAXCopy(buffer, WORLD_MAP_BG_PALETTE,
			   DMA_COPY_WORDS(14 * PALETTE_COLORS / 2), REG_DMA3);
	tiles = BgTileBuffer;
	fdDecompressTaggedLz(syResourceGetTableEntry(resources[1]), tiles);
	syDMAXCopy(tiles, BG_CHAR_BLOCK(2),
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	fdDecompressTaggedLz(syResourceGetTableEntry(resources[2]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES,
			   BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	fdDecompressTaggedLz(syResourceGetTableEntry(resources[3]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2,
			   BG_CHAR_BLOCK(3), DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4),
			   REG_DMA3);
	fdDecompressTaggedLz(syResourceGetTableEntry(resources[4]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
	syDMAXCopy(Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3,
			   BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	fdDecompressTaggedLz(syResourceGetTableEntry(resources[5]),
						 MapGridAnimTiles);
	/* BG1's map (screen block 5) is filled with tile 127 of WINDOW_PALETTE, and
	 */
	/* screen block 6 numbers a 30 by 20 tile image from */
	/* WORLD_MAP_STILL_FIRST_TILE, two tile numbers a word, for the still */
	/* image fdWorldMapStartStillImage shows. */
	btDmaFill32(WINDOW_TILE_PAIR(WINDOW_TILE(127)), BG_SCREEN_BLOCK(5), 1536);
	screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
	tilePair = MAP_ENTRY_PAIR(WORLD_MAP_STILL_FIRST_TILE,
							  WORLD_MAP_STILL_FIRST_TILE + 1);
	for(screenRow = 0; screenRow < DISPLAY_TILE_HEIGHT; screenRow++) {
		for(screenColumn = 0; screenColumn < DISPLAY_TILE_WIDTH / 2;
			screenColumn++) {
			*screenTiles++ = tilePair;
			tilePair += MAP_ENTRY_PAIR(2, 2);
		}
		screenTiles++;
	}
	/* On the snowfield set, six grid cells of the snowy north show their own */
	/* tiles. */
	if(graphicsSet == 1) {
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 0);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 10, 1);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 11, 1);
		WORLD_MAP_USE_ALTERNATE_TILE(perspective, 12, 1);
		fdWorldMapUpdateCurrentTileBlock();
	}
	syHeapRelease(buffer);
}

/* HBlank setup each VBlank on the world map: copies the first scanline's BG2 */
/* and BG3 affine parameters, starts the HBlank DMA for the rest, and picks the
 */
/* BG mode from the window's top and bottom. */
void fdWorldMapApplyAffinePage(void) {
	WORLD_MAP_AFFINE_PAIR(*affinePages)
	[DISPLAY_HEIGHT] =
		((WORLD_MAP_AFFINE_STATE *)HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE))
			->pages;
	s32 displayControl = (s16)(REG_DISPCNT & ~DISPCNT_MODE_MASK);
	u32 *affineRegisters = (u32 *)&REG_BG2PA;
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	volatile u16 *dma0 = REG_DMA0;
	u32 *affineSource;
	u32 mode;
	u32 windowTop;
	u32 nextDisplayControl;

	/* Stop the last VBlank's HBlank DMA, then load line 0's eight words (BG2PA
	 * to */
	/* BG3Y) by hand; the DMA repeats eight words into the same registers at */
	/* each HBlank for the lines after it. */
	dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
	(void)dma0[DMA_CNT_H_INDEX];
	if(affinePages != NULL) {
		affineSource = (u32 *)affinePages[FrameCount & 1];
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters++ = *affineSource++;
		*affineRegisters = *affineSource++;
		syDMAXCopy(affineSource, (void *)&REG_BG2PA,
				   (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT |
					DMA_DEST_RELOAD)
						   << 16 |
					   8,
				   (volatile u32 *)dma0);
	}
	perspective->shownTop = perspective->winTop;
	perspective->shownBottom = perspective->winBottom;
	/* DISPCNT_MODE_2 (the affine ground) for the load's rows 0 to 159; */
	/* DISPCNT_MODE_0 for fdWorldMapStartStillImage's 200 and 255, which show
	 * the */
	/* still image on BG1. */
	windowTop = perspective->shownTop;
	mode = DISPCNT_MODE_0;
	if(windowTop < WORLD_MAP_STILL_WINDOW_TOP) {
		u16 bottom = perspective->shownBottom;

		mode = (bottom != 0) << 1;
		if(windowTop <= bottom) {
			mode = DISPCNT_MODE_0;
			if(windowTop == 0)
				mode = DISPCNT_MODE_2;
		}
	}
	displayControl |= mode;
	nextDisplayControl = (u16)displayControl;
	REG_DISPCNT = nextDisplayControl;
	perspective->winPending = 0;
}

/* Copies one world map cell's two tile entries (top and bottom) to a BG map. */
static __inline__ void fdWorldMapCopyCellTiles(u8 *cellBytes, u8 *screenTile) {
	u32 cellTilesIndex = *(u16 *)cellBytes;
	u16 *cellTiles = (u16 *)Ram_MapGridCellBuffer;

	cellTiles += cellTilesIndex * 2;
	*(u16 *)screenTile = *cellTiles++;
	*(u16 *)(screenTile + WORLD_MAP_SCREEN_WIDTH) = *cellTiles;
}

/* Draws row tileRow of the cell buffer into both layers' BG maps, 32 cells in
 */
/* each: the row of the ground that scrolls into view. */
void fdWorldMapDrawCellRow(u32 tileRow) {
	u8 *sourceCell;
	u8 *screenTile;
	u32 cellsCopied;

	sourceCell = Ram_CellTiles + ((((s32)tileRow / 2) & 31) << 7);
	screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + ((tileRow & 62) << 6));
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += 2;
		sourceCell += 4;
	} while(cellsCopied < WORLD_MAP_BUFFER_CELLS);

	/* The same row of the second layer. */
	screenTile += WORLD_MAP_LAYER_SCREEN_BYTES - WORLD_MAP_BUFFER_CELLS * 2;
	sourceCell += WORLD_MAP_LAYER_CELL_BYTES - WORLD_MAP_CELL_ROW_BYTES;
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += 2;
		sourceCell += 4;
	} while(cellsCopied < WORLD_MAP_BUFFER_CELLS);
}

/* Draws column tileColumn of the cell buffer into both layers' BG maps, 32 */
/* cells down in each (the second layer's map and cells follow the first's): */
/* the column of the ground that scrolls into view. */
void fdWorldMapDrawCellColumn(u32 tileColumn) {
	u8 *sourceCell;
	u8 *screenTile;
	u32 cellsCopied;

	sourceCell = Ram_CellTiles + ((((s32)tileColumn / 2) & 31) << 2);
	screenTile = (u8 *)((u32)BG_SCREEN_BLOCK(8) + (tileColumn & 62));
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += WORLD_MAP_SCREEN_WIDTH * 2;
		sourceCell += WORLD_MAP_CELL_ROW_BYTES;
	} while(cellsCopied < 2 * WORLD_MAP_BUFFER_CELLS);
}

/* Each frame on the world map: loads tiles as the view moves, shakes and */
/* follows the player, draws the BG row or column that scrolls into view, moves
 */
/* the camera, and builds the ground plane's scanline table. */
void fdWorldMapUpdateView(void) {
	BATTLE_CAMERA *camera = BattleCameraWork;
	WORLD_MAP_AFFINE_STATE *affineState =
		HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	s32 *cameraPosition = camera->focus;
	s32 *followedPosition = perspective->origin;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT] = affineState->pages;
	s32 distance = perspective->farPlane;
	s32 height = perspective->distance;
	POINT eyeOffset;

	fdWorldMapPreloadCurrentTileBlock();
	if(followedPosition != NULL) {
		FIXED x;
		FIXED y;
		s32 tileColumn;
		s32 tileRow;

		y = followedPosition[2];
		x = followedPosition[0];
		if(perspective->shakeX != 0) {
			s32 shakeDifference = syRandom();
			s32 shakeAmplitude;

			shakeDifference -= syRandom();
			shakeAmplitude = perspective->shakeX;
			x += syMulFX(shakeAmplitude, shakeDifference);
			perspective->shakeX =
				syMulFX(shakeAmplitude, perspective->shakeDecay);
		}
		if(perspective->shakeY != 0) {
			s32 shakeDifference = syRandom();
			FIXED shakeAmplitude;

			shakeDifference -= syRandom();
			shakeAmplitude = perspective->shakeY;
			y += syMulFX(shakeAmplitude, shakeDifference);
			perspective->shakeY =
				syMulFX(shakeAmplitude, perspective->shakeDecay);
		}
		/* On crossing a 16-pixel line, draw the column 16 on that side, or the
		 */
		/* row 12 on when y grows and 18 back when it falls. */
		tileColumn = x / toFIXED(16);
		tileRow = y / toFIXED(16);
		if((perspective->view[0] ^ x) & toFIXED(16)) {
			if(perspective->view[0] < x)
				fdWorldMapDrawCellColumn(tileColumn + 16);
			else
				fdWorldMapDrawCellColumn(tileColumn - 16);
		}
		if((perspective->view[1] ^ y) & toFIXED(16)) {
			if(perspective->view[1] < y)
				fdWorldMapDrawCellRow(tileRow + 12);
			else
				fdWorldMapDrawCellRow(tileRow - 18);
		}
		perspective->view[0] = x;
		perspective->view[1] = y;
	}
	/* Rebuild the projection and look at the followed position from the */
	/* current pitch and yaw, the eye one pixel beyond the zoomed distance. */
	/* followedPosition is read here without the NULL test above, which never */
	/* fails on the world map: fdSequenceRun's fdActorResetForLeader */
	/* points it at the camera actor before any frame passes, and nothing clears
	 */
	/* it. */
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(distance, height / 2, height * 2);
	SET_VECTOR(cameraPosition, *followedPosition++, 0, followedPosition[1]);
	syInitMatrix();
	syMatrixTranslateVector(cameraPosition);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	eyeOffset[X] = 0;
	eyeOffset[Y] = 0;
	eyeOffset[Z] = height + FIXED_ONE;
	syCalcPoint(eyeOffset, (s32 *)camera);
	syInitMatrix();
	syMatrixSetLookAt(camera->eye, cameraPosition);
	/* The scanline table depends only on the pitch, so it is rebuilt only when
	 */
	/* the pitch has changed. */
	if(ScanlinePitch != perspective->pitch) {
		FIXED cosine = syCos(perspective->pitch);
		FIXED sine = sySin(perspective->pitch);

		fdWorldMapBuildScanlineTable(syDivFX(cosine, sine), cameraPosition,
									 affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
	}
	((WORLD_MAP_PLANE_FN)HeapSlots[HEAP_SLOT_RAM_CODE])(
		camera, cameraPosition, affineState->scanlines,
		affinePages[FrameCount & 1]);
}

/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlock(void) {
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	s32 firstQuarterX = 0;
	s32 firstQuarterY = 0;
	u32 layer;
	u32 row;
	u32 col;
	s32 layerTileBase;
	s32 tileId;

	if(perspective->origin != NULL) {
		s32 *followedPosition = perspective->origin;

		firstQuarterX = *followedPosition++;
		firstQuarterY = followedPosition[1];
	}
	/* The top left of the 2x2 tiles around the point, 256 pixels left of it */
	/* and 320 above. */
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
				fdWorldMapWriteLayerCellTile(layer, firstQuarterX + col,
											 firstQuarterY + row, tileId, 1);
			}
		}
		layerTileBase += WORLD_MAP_LAYER_TILES;
	}
}

/* Loads the first of the 2x2 world map tiles around the player that is not */
/* shown yet (at most one a frame), without drawing it. */
void fdWorldMapPreloadCurrentTileBlock(void) {
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	s32 firstQuarterX = 0;
	s32 firstQuarterY = 0;
	u32 layer;
	u32 row;
	u32 col;
	s32 tileId;

	if(perspective->origin != NULL) {
		s32 *followedPosition = perspective->origin;

		firstQuarterX = *followedPosition++;
		firstQuarterY = followedPosition[1];
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
				if(fdWorldMapWriteLayerCellTile(layer, firstQuarterX + col,
												firstQuarterY + row, tileId,
												0) != 0)
					return;
			}
		}
	}
}

/* VBlank: shows BG1 and copies the decoded picture's tiles into VRAM. */
void fdWorldMapPresentStillImage(void) {
	scIoWrite16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
				&REG_BG1CNT);
	syDMAXCopy(MapGridCellBuffer,
			   BG_CHAR_BLOCK(0) + WORLD_MAP_STILL_FIRST_TILE * TILE_SIZE_8BPP,
			   DMA_COPY_WORDS(WORLD_MAP_STILL_TILES * TILE_SIZE_8BPP / 4),
			   REG_DMA3);
}

/* Freezes the world map's perspective view as a still image on BG1: saves */
/* BG1's tiles, draws the current affine frame into pixels and hands the window
 */
/* and VBlank to it. */
void fdWorldMapStartStillImage(void) {
	WORLD_MAP_AFFINE_STATE *affineState =
		HEAP_SLOT_BLOCK(HEAP_SLOT_WORLD_MAP_AFFINE);
	WORLD_MAP_VIEW *perspective = MapGridState[0];

	perspective->anmActive = 1;
	syTaskSuspend(fdTilesetAnimUpdate);
	syDMAXCopy(BG_CHAR_BLOCK(1), DecodeBuffer,
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	sySynch(1);
	fdTileMapDrawRowsFromRam(affineState->pages[FrameCount & 1],
							 (u8 *)MapGridCellBuffer);
	perspective->winTop = WORLD_MAP_STILL_WINDOW_TOP;
	perspective->winBottom = WORLD_MAP_STILL_WINDOW_BOTTOM;
	VBlankCallback = fdWorldMapPresentStillImage;
}

/* VBlank at the end of the still image: puts BG1 back as the plain layer */
/* and copies the area's four tile sets back into char blocks 2 and 3. */
void fdWorldMapEndStillImage(void) {
	scIoWrite16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
	syDMAXCopy(BgTileBuffer, BG_CHAR_BLOCK(2),
			   DMA_COPY_WORDS(4 * WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
}

/* Reloads the world map area's palette and tiles after the still image, puts */
/* the default cells back and returns to the perspective view. */
void fdWorldMapLoadAreaGraphics(void) {
	WORLD_MAP_VIEW *perspective;
	u32 *graphicsResources;
	u8 *decodeBuffer;
	s16 backdropColor;

	perspective = MapGridState[0];
	decodeBuffer = (u8 *)MapGridCellBuffer;
	graphicsResources = perspective->resources;
	backdropColor = *(s16 *)BG_PLTT;
	fdDecompressByteLz(syResourceGetTableEntry(graphicsResources[0]),
					   decodeBuffer);
	*(s16 *)decodeBuffer = backdropColor;
	syDMAXCopy(decodeBuffer, (void *)BG_PLTT,
			   DMA_COPY_WORDS(14 * PALETTE_COLORS / 2), REG_DMA3);
	fdDecompressTaggedLz(syResourceGetTableEntry(graphicsResources[1]),
						 BgTileBuffer);
	fdDecompressTaggedLz(syResourceGetTableEntry(graphicsResources[2]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES);
	fdDecompressTaggedLz(syResourceGetTableEntry(graphicsResources[3]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 2);
	fdDecompressTaggedLz(syResourceGetTableEntry(graphicsResources[4]),
						 Ram_BgTileBuffer + WORLD_MAP_TILE_SET_BYTES * 3);
	/* The VBlank copies the tiles back and the next frame shows the ground; */
	/* only then is the still image's buffer refilled with the cells. */
	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	fdDecompressTaggedLz(
		syResourceGetTableEntry((u32)RESOURCE_DEFAULT_GRID_CELLS),
		decodeBuffer);
	fdWorldMapUpdateCurrentTileBlock();
	perspective->anmActive = 0;
	syTaskResume(fdTilesetAnimUpdate);
	sySynch(1);
}

/* Puts the default world map cells back after the still image and returns to */
/* the perspective view, without reloading the area's palette or tiles. */
void fdWorldMapLoadDefaultCellsAndUpdateBlock(void) {
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	fdDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_DEFAULT_GRID_CELLS),
						 MapGridCellBuffer);
	fdWorldMapUpdateCurrentTileBlock();
	syTaskResume(fdTilesetAnimUpdate);
	sySynch(1);
}

#undef WORLD_MAP_BG_PALETTE
#undef WORLD_MAP_CELLS
#undef WORLD_MAP_TILE_CELLS
#undef WORLD_MAP_TILES_ACROSS
#undef WORLD_MAP_LAYER_TILES
#undef WORLD_MAP_ALTERNATE_TILES
#undef WORLD_MAP_FIXED_TO_TILE
#undef WORLD_MAP_BUFFER_CELLS
#undef WORLD_MAP_CELL_ROW_BYTES
#undef WORLD_MAP_LAYER_CELL_BYTES
#undef WORLD_MAP_SCREEN_WIDTH
#undef WORLD_MAP_SCREEN_ROW_HALFWORDS
#undef WORLD_MAP_LAYER_SCREEN_BYTES
#undef WORLD_MAP_WIDE_TILES
#undef WORLD_MAP_STILL_WINDOW_TOP
#undef WORLD_MAP_STILL_WINDOW_BOTTOM
#undef WORLD_MAP_STILL_TILES
#undef WORLD_MAP_USE_ALTERNATE_TILE
