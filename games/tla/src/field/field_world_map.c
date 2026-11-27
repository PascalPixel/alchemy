/* @file */
/* The world map: its perspective view, its tiles, and drawing its cells into */
/* a BG map. */
/* Part of field.c. */

#include "battle/battle_camera.h"
#include "field/world_map.h"
#include "window/window.h"
#include "graphics/palette_buffers.h"

#define WORLD_MAP_BG_PALETTE ((s16 *)BG_PLTT)

/* A world map tile is 512 pixels square: WORLD_MAP_TILE gives the tile a 16.16
 */
/* coordinate lies in, as FIELD_CELL a map grid cell. The world map is 32 by 32
 */
/* tiles, wrapping, and each layer's tile numbers follow the 1024 before it. */
#define WORLD_MAP_TILE(coordinate) ((coordinate) >> 25)
#define WORLD_MAP_TILES_ACROSS	   32
#define WORLD_MAP_LAYER_TILES	   1024

/* MAP_STATE.wideTiles for the world map (WORLD_MAP_VIEW.fadeStep): its tileset
 */
/* animations copy 64-byte tiles of 256 colours, into char block 2. */
#define WORLD_MAP_WIDE_TILES 128

/* The world map's four graphics sets, the rows of WorldMap_GraphicsResources.
 */
typedef enum WORLD_MAP_GRAPHICS {
	WORLD_MAP_GRAPHICS_DEFAULT, /* the green lands, deserts and mountains */
	/* Snow and ice: Kalt Island, the Northern Reaches, Tundaria. */
	WORLD_MAP_GRAPHICS_SNOWFIELD,
	/* Palm-covered islands in the seas at the world's edges, the Apojii */
	/* Islands among them. */
	WORLD_MAP_GRAPHICS_TROPICAL,
	/* Prox and Mars Lighthouse, snow falling. */
	WORLD_MAP_GRAPHICS_BLIZZARD
} WORLD_MAP_GRAPHICS;

/* The world map's encounter regions come in groups of ten; three of the groups
 */
/* are drawn with a graphics set of their own, the rest with the default one. */
#define WORLD_MAP_REGION_GROUP 10
typedef enum WORLD_MAP_REGION_KIND {
	WORLD_MAP_REGIONS_TROPICAL = 2,
	WORLD_MAP_REGIONS_BLIZZARD = 3,
	WORLD_MAP_REGIONS_SNOWFIELD = 4
} WORLD_MAP_REGION_KIND;

/* The graphics set a group of world map regions is drawn with. */
static inline s32 fdWorldMapGetRegionGroupGraphics(s32 group) {
	switch(group) {
	case WORLD_MAP_REGIONS_BLIZZARD:
		return WORLD_MAP_GRAPHICS_BLIZZARD;
	case WORLD_MAP_REGIONS_TROPICAL:
		return WORLD_MAP_GRAPHICS_TROPICAL;
	case WORLD_MAP_REGIONS_SNOWFIELD:
		return WORLD_MAP_GRAPHICS_SNOWFIELD;
	default:
		return WORLD_MAP_GRAPHICS_DEFAULT;
	}
}

/* A world map graphics set, a row of WorldMap_GraphicsResources: its */
/* palette, tileset animation script, cell tiles attributes and six tile */
/* sets. */
typedef enum WORLD_MAP_GRAPHICS_RESOURCE {
	WORLD_MAP_SET_PALETTE,
	WORLD_MAP_SET_ANIMATION,
	WORLD_MAP_SET_CELL_ATTRIBUTES,
	WORLD_MAP_SET_TILES_1,
	WORLD_MAP_SET_TILES_2,
	WORLD_MAP_SET_TILES_3,
	WORLD_MAP_SET_TILES_4,
	WORLD_MAP_SET_TILES_5,
	WORLD_MAP_SET_TILES_6,
} WORLD_MAP_GRAPHICS_RESOURCE;

/* The still image on BG1 (fdWorldMapStartStillImage): 32 by 20 of the */
/* map screen's tiles, from its fifth row on. */
#define STILL_IMAGE_TILES_ACROSS 32
#define STILL_IMAGE_TILE_ROWS	 20
#define STILL_IMAGE_FIRST_TILE                                                 \
	(MAP_SCREEN_FIRST_TILE + 4 * STILL_IMAGE_TILES_ACROSS)
/* While the still image shows, the perspective's window lies below the */
/* screen, so no line is drawn in perspective. */
#define STILL_IMAGE_WINDOW_TOP	  200
#define STILL_IMAGE_WINDOW_BOTTOM 255

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
	volatile u32 fill;
	FIXED farPlane;
	s32 tileIndex;

	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
					 DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScrFadeFromBlack(0);
	perspective = syHeapSlotAlloc(HEAP_SLOT_MAP_GRID, sizeof(WORLD_MAP_VIEW));
	btDmaFill32Cell(fill, 0, perspective, sizeof(WORLD_MAP_VIEW));
	perspective->view[0] = 0;
	perspective->view[1] = 0;
	perspective->scaleX = FIXED_ONE * 32;
	perspective->scaleY = FIXED_ONE * 64;
	perspective->limitX = toFIXED(16352);
	perspective->limitY = toFIXED(16352);
	perspective->script = NULL;
	perspective->tiles = syResourceGetTableEntry(RESOURCE_PERSPECTIVE_DATA_A);
	syIoWrite16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
					BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
				&REG_BLDCNT);
	syIoWrite16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	REG_BLDY = 0;
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_CELLS),
						 MapGridCellBuffer);
	perspective->shownLayers = DISPCNT_BG0_ON | DISPCNT_BG1_ON |
							   DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
	perspective->wideTiles = WORLD_MAP_WIDE_TILES;
	syDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_ENTRANCES), MapScene);
	syIoWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
					BGCNT_WRAP | BGCNT_SIZE(2),
				&REG_BG3CNT);
	syIoWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
					BGCNT_WRAP | BGCNT_SIZE(2),
				&REG_BG2CNT);
	syIoWrite16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5) | BGCNT_SIZE(2),
				&REG_BG1CNT);
	syIoWrite16(Q_8_8(1), &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	syIoWrite16(Q_8_8(1), &REG_BG2PD);
	REG_BG2X = 0;
	REG_BG2Y = 0;
	syIoWrite16(Q_8_8(1), &REG_BG3PA);
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	syIoWrite16(Q_8_8(1), &REG_BG3PD);
	REG_BG3X = 0;
	syIoWrite16(0, &REG_BG3Y);

	camera = syHeapSlotAllocEwram(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	affineState = syHeapSlotAlloc(HEAP_SLOT_WORLD_MAP_AFFINE,
								  sizeof(WORLD_MAP_AFFINE_STATE));
	affinePages = affineState->pages;
	farPlane = toFIXED(510);
	perspective->farPlane = farPlane;
	perspective->distance = farPlane;
	perspective->zoom = FIXED_ONE;
	cameraPosition = camera->focus;
	perspective->zoomTransitionFrm = 0;
	camera->followEye = 0;
	camera->followPos = 0;
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

		SET_VECTOR(eyeOffset, 0, 0, farPlane);
		syCalcPoint(eyeOffset, camera->eye);
		syInitMatrix();
		syMatrixSetLookAt(camera->eye, cameraPosition);
		syRamCodeLoadSlot(HEAP_SLOT_RAM_CODE, fdWorldMapUpdateAffineLinesCode,
						  fdWorldMapUpdateAffineLinesCodeSize);
		fdWorldMapBuildScanlineTable(
			syDivFX(syCos(perspective->pitch), sySin(perspective->pitch)),
			cameraPosition, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
		((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->ramCode)(
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
		if(perspective->fixedEye == 0) {
			SET_VECTOR(eyeOffset, 0, 0, perspective->distance + FIXED_ONE);
			syCalcPoint(eyeOffset, camera->eye);
		}
	}
	REG_MOSAIC = 0;
	syIoWrite16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	BgScrollWork[2].x = 0;
	BgScrollWork[2].y = 0;
	BgScrollWork[3].x = 0;
	BgScrollWork[3].y = 0;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySetEvent(fdWorldMapUpdateView, TASK_PASS_FRAME + 5);
	sySetEvent(fdWorldMapApplyAffinePage, TASK_PASS_VBLANK);
	/* Each grid cell starts on the tile of its own number. */
	for(tileIndex = 0;
		tileIndex < WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS;
		tileIndex++)
		perspective->tileIds[tileIndex] = tileIndex;
}

/* Puts world map tile (x / 16, y / 16) at cell (cellX / 16, cellY / 16) of the
 */
/* tile id grid, and draws it at once when it is next to the view. cellX and */
/* cellY count 32-pixel world map blocks, so a grid cell is a 512-pixel tile; */
/* it is next to the view when cellX / 8 and cellY / 8, in 256-pixel steps, */
/* are within one step of the followed point's. */
void fdWorldMapSetWindowCellTile(s32 x, s32 y, s32 cellX, s32 cellY) {
	WORLD_MAP_VIEW *perspective;
	s32 *followedPosition;
	s32 originX;
	s32 originY;
	s32 tile;

	perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
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
	tile = (y << 5) + x;
	perspective->tileIds[(((cellY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 5) +
						 ((cellX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tile;

	if(ABS(originX - cellX) <= 1 && ABS(originY - cellY) <= 1) {
		fdWorldMapWriteLayerCellTile(0, cellX / 2, cellY / 2, tile, 1);
		fdWorldMapWriteLayerCellTile(1, cellX / 2, cellY / 2,
									 tile + WORLD_MAP_LAYER_TILES, 1);
	}
}

/* fdWorldMapSetWindowCellTile without its view test: puts world map tile */
/* (x / 16, y / 16) at cell (cellX / 16, cellY / 16) of the tile grid and draws
 */
/* it in both layers. */
void fdWorldMapSetWindowCellTileAndDraw(s32 x, s32 y, s32 cellX, s32 cellY) {
	WORLD_MAP_VIEW *perspective;
	s32 *followedPosition;
	s32 originX;
	s32 originY;
	s32 tile;

	perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	/* LEFTOVER(template): the followed point, taken and scaled exactly as
	 * fdWorldMapSetWindowCellTile above (and The Broken Seal's
	 * WorldMap_SetGridTile) take it for their test of the view; this routine
	 * draws without that test, so nothing reads it; as the routine without
	 * these lines. */
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
	tile = (y << 5) + x;
	perspective->tileIds[(((cellY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 5) +
						 ((cellX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tile;
	fdWorldMapWriteLayerCellTile(0, cellX / 2, cellY / 2, tile, 1);
	fdWorldMapWriteLayerCellTile(1, cellX / 2, cellY / 2,
								 tile + WORLD_MAP_LAYER_TILES, 1);
}

/* Loads the world map's palette and tiles for the area at (x, z): the set */
/* its region picks, its tint, and the plain BG maps the world map draws over.
 */
void fdWorldMapLoadGraphics(s32 x, s32 z) {
	u8 *buffer;
	WORLD_MAP_VIEW *perspective;
	WORLD_MAP_CELL *block;
	const u32 *resources;
	s32 region;
	s16 backdropColor;
	u32 *screenTiles;
	u32 tilePair;
	s32 row;
	s32 column;

	buffer = syHeapAlloc(BG_PLTT_SIZE);
	perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	block = &((WORLD_MAP_CELL *)
				  syRamMapBlocks)[((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
								   (WORLD_MAP_VIEW_CELLS - 1)) +
								  (((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
									(WORLD_MAP_VIEW_CELLS - 1)) *
								   WORLD_MAP_VIEW_CELLS)];
	fdWorldMapUpdateRegionTint(block->region, 0);
	region = block->region;
	region /= WORLD_MAP_REGION_GROUP;
	resources =
		WorldMap_GraphicsResources[fdWorldMapGetRegionGroupGraphics(region)];
	perspective->resources = resources;
	backdropColor = WORLD_MAP_BG_PALETTE[0];
	syDecompressByteLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_PALETTE]), buffer);
	*(s16 *)buffer = backdropColor;
	syDMAXCopy(buffer, (void *)BG_PLTT,
			   DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
	fdPaletteFadeLoadPaletteStub(buffer);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_ANIMATION]),
		MAPGRID_ANIMATION_SCRIPT);
	fdTilesetAnimStartAll((const u16 *)MAPGRID_ANIMATION_SCRIPT);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_CELL_ATTRIBUTES]),
		Ram_MapCollision);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
		Ram_BgTileBuffer);
	syDMAXCopy(Ram_BgTileBuffer, BG_CHAR_BLOCK(2),
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
		WORLD_MAP_TILE_SET(1));
	syDMAXCopy(WORLD_MAP_TILE_SET(1),
			   BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
		WORLD_MAP_TILE_SET(2));
	syDMAXCopy(WORLD_MAP_TILE_SET(2), BG_CHAR_BLOCK(3),
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
		WORLD_MAP_TILE_SET(3));
	syDMAXCopy(WORLD_MAP_TILE_SET(3),
			   BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
			   DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
		MapGridAnimTiles);
	syDecompressTaggedLz(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
		&WorldMapStateWork);
	/* BG1's map (screen block 5) is filled with the window tiles' black tile,
	 */
	/* and screen block 6 numbers the map screen image's tiles in rows, two a */
	/* word: the still image's, then on round the image. */
	btDmaFill32(WINDOW_TILE_PAIR(WINDOW_TILE_COMMAND_PANEL), BG_SCREEN_BLOCK(5),
				24 * BG_SCREEN_WIDTH * sizeof(u16));
	screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
	tilePair = TILE_PAIR(STILL_IMAGE_FIRST_TILE);
	for(row = 0; row < STILL_IMAGE_TILE_ROWS; row++) {
		for(column = 0; column < STILL_IMAGE_TILES_ACROSS / 2; column++) {
			*screenTiles++ = tilePair;
			tilePair += TILE_PAIR_STEP;
		}
	}
	tilePair = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
	for(row = 0; row < MAP_SCREEN_TILE_ROWS; row++) {
		for(column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
			*screenTiles++ = tilePair;
			tilePair += TILE_PAIR_STEP;
		}
	}
	tilePair = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
	for(row = 0; row < 4; row++) {
		for(column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
			*screenTiles++ = tilePair;
			tilePair += TILE_PAIR_STEP;
		}
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
		((WORLD_MAP_AFFINE_STATE *)Ram_HeapSlots->worldMapAffine)->pages;
	s32 displayControl = (s16)(REG_DISPCNT & ~DISPCNT_MODE_MASK);
	u32 *affineRegisters = (u32 *)&REG_BG2PA;
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	volatile u16 *dma0 = REG_DMA0;
	u32 *affineSource;
	u32 mode;
	u32 windowTop;
	u32 nextDisplayControl;

	/* Stops the last VBlank's HBlank DMA, then loads line 0's eight words
	 * (BG2PA */
	/* to BG3Y) by hand; the DMA repeats eight words into the same registers at
	 */
	/* each HBlank for the lines after it. */
	dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[5] &= ~DMA_ENABLE;
	(void)dma0[5];
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
						   << DMA_CONTROL_SHIFT |
					   sizeof(WORLD_MAP_AFFINE_PAIR) / 4,
				   (u32 *)dma0);
	}
	perspective->shownTop = perspective->winTop;
	perspective->shownBottom = perspective->winBottom;
	/* DISPCNT_MODE_2 (the affine ground) for the rows of the whole screen; */
	/* DISPCNT_MODE_0 for the still image's, which lie below it. */
	mode = DISPCNT_MODE_0;
	windowTop = perspective->shownTop;
	if(windowTop < STILL_IMAGE_WINDOW_TOP) {
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

/* Copies one world map cell's tiles to a tile map: its top two, then its */
/* bottom two on the row below. */
static __inline__ void fdWorldMapCopyCellTiles(u8 *cellBytes, u8 *screenTile) {
	u32 cellTilesIndex = *(u16 *)cellBytes;
	u16 *cellTiles = (u16 *)Ram_MapCellBuffer;

	cellTiles += cellTilesIndex * 2;
	*(u16 *)screenTile = *cellTiles++;
	*(u16 *)(screenTile + WORLD_MAP_TILE_MAP_WIDTH) = *cellTiles;
}

/* Draws the cells of the world map's tile row tileRow into its tile maps: the
 */
/* lower layer's 32 cells, then the upper layer's. */
void fdWorldMapDrawCellRow(u32 tileRow) {
	u8 *sourceCell;
	u8 *screenTile;
	u32 cellsCopied;

	sourceCell =
		syRamMapBlocks + (((s32)tileRow / 2) & (WORLD_MAP_VIEW_CELLS - 1)) *
							 WORLD_MAP_VIEW_CELLS * sizeof(WORLD_MAP_CELL);
	screenTile = (u8 *)((u32)WORLD_MAP_LOWER_TILE_MAP +
						(tileRow & (WORLD_MAP_TILE_MAP_WIDTH - 2)) *
							WORLD_MAP_TILE_MAP_WIDTH);
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += 2;
		sourceCell += sizeof(WORLD_MAP_CELL);
	} while(cellsCopied < WORLD_MAP_VIEW_CELLS);

	/* The upper layer's tile map and cells follow the lower layer's. */
	screenTile += WORLD_MAP_TILE_MAP_WIDTH * WORLD_MAP_TILE_MAP_WIDTH -
				  WORLD_MAP_TILE_MAP_WIDTH;
	sourceCell += WORLD_MAP_VIEW_LAYER_BYTES -
				  WORLD_MAP_VIEW_CELLS * sizeof(WORLD_MAP_CELL);
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += 2;
		sourceCell += sizeof(WORLD_MAP_CELL);
	} while(cellsCopied < WORLD_MAP_VIEW_CELLS);
}

/* Draws the cells of the world map's tile column tileColumn into its tile */
/* maps: 64 cells down, the lower layer's 32 and then the upper layer's, which
 */
/* follow them in the tile maps and in the cells. */
void fdWorldMapDrawCellColumn(u32 tileColumn) {
	u8 *sourceCell;
	u8 *screenTile;
	u32 cellsCopied;

	sourceCell =
		syRamMapBlocks + (((s32)tileColumn / 2) & (WORLD_MAP_VIEW_CELLS - 1)) *
							 sizeof(WORLD_MAP_CELL);
	screenTile = (u8 *)((u32)WORLD_MAP_LOWER_TILE_MAP +
						(tileColumn & (WORLD_MAP_TILE_MAP_WIDTH - 2)));
	cellsCopied = 0;
	do {
		fdWorldMapCopyCellTiles(sourceCell, screenTile);
		cellsCopied++;
		screenTile += 2 * WORLD_MAP_TILE_MAP_WIDTH;
		sourceCell += WORLD_MAP_VIEW_CELLS * sizeof(WORLD_MAP_CELL);
	} while(cellsCopied < 2 * WORLD_MAP_VIEW_CELLS);
}

/* Each frame: follows the followed point with the screen shake, draws the rows
 */
/* and columns that came into view, and recomputes the projection and the */
/* ground plane for the current pitch and yaw; while the still image shows, */
/* only BG1 shakes. */
void fdWorldMapUpdateView(void) {
	BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;
	WORLD_MAP_AFFINE_STATE *affineState = Ram_HeapSlots->worldMapAffine;
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	s32 *cameraPosition = camera->focus;
	s32 *followedPosition = perspective->origin;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT] = affineState->pages;
	s32 distance = perspective->farPlane;
	s32 height = perspective->distance;
	s32 tileRow;
	FIXED shakeOffsetX;
	FIXED shakeOffsetY;
	POINT eyeOffset;

	shakeOffsetX = 0;
	shakeOffsetY = 0;
	if(perspective->viewHeld != 0) {
		if(perspective->shakeX == 0 && perspective->shakeY == 0)
			return;
	}
	else {
		fdWorldMapPreloadCurrentTileBlock();
	}
	if(followedPosition != NULL) {
		FIXED x = followedPosition[0];
		FIXED y = followedPosition[2];
		s32 tileColumn;

		if(perspective->shakeX != 0) {
			s32 shakeDifference = syRandom();

			shakeDifference -= syRandom();
			shakeOffsetX = syMulFX(perspective->shakeX, shakeDifference);
			x += shakeOffsetX;
			perspective->shakeX =
				syMulFX(perspective->shakeX, perspective->shakeDecay);
		}
		if(perspective->shakeY != 0) {
			s32 shakeDifference = syRandom();

			shakeDifference -= syRandom();
			shakeOffsetY = syMulFX(perspective->shakeY, shakeDifference);
			y += shakeOffsetY;
			perspective->shakeY =
				syMulFX(perspective->shakeY, perspective->shakeDecay);
		}
		tileColumn = x / toFIXED(16);
		tileRow = y / toFIXED(16);
		/* On crossing a 16-pixel line, draws the column 16 on that side, or the
		 */
		/* row 12 on when y grows and 18 back when it falls. */
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
	cameraPosition[0] = perspective->view[0];
	cameraPosition[1] = 0;
	cameraPosition[2] = perspective->view[1];
	if(perspective->stillImageShown != 0) {
		BgScrollWork[1].x = shakeOffsetX / FIXED_ONE;
		BgScrollWork[1].y = shakeOffsetY / FIXED_ONE;
		return;
	}
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(distance, height / 2, height * 2);
	syInitMatrix();
	syMatrixTranslateVector(cameraPosition);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	if(perspective->fixedEye == 0) {
		SET_VECTOR(eyeOffset, 0, 0, height + toFIXED(1));
		syCalcPoint(eyeOffset, camera->eye);
	}
	syInitMatrix();
	syMatrixSetLookAt(camera->eye, cameraPosition);
	if(ScanlinePitch != perspective->pitch) {
		FIXED cosine = syCos(perspective->pitch);
		FIXED sine = sySin(perspective->pitch);

		fdWorldMapBuildScanlineTable(syDivFX(cosine, sine), cameraPosition,
									 affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
	}
	((void (*)(void *, s32 *, void *, void *))Ram_HeapSlots->ramCode)(
		camera, cameraPosition, affineState->scanlines,
		affinePages[FrameCount & 1]);
}

/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlock(void) {
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	s32 firstQuarterX = 0;
	s32 firstQuarterY = 0;
	u32 layer;
	u32 row;
	u32 col;
	s32 tile;

	if(perspective->origin != NULL) {
		s32 *followedPosition = perspective->origin;

		firstQuarterX = *followedPosition++;
		firstQuarterY = followedPosition[1];
	}
	/* The top left of the 2x2 tiles around the point, 256 pixels left of it and
	 */
	/* 320 above. */
	firstQuarterX = WORLD_MAP_TILE(firstQuarterX - toFIXED(256));
	firstQuarterY = WORLD_MAP_TILE(firstQuarterY - toFIXED(320));
	for(layer = 0; layer < 2; layer++) {
		for(row = 0; row < 2; row++) {
			for(col = 0; col < 2; col++) {
				tile = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1))
						<< 5) +
					   ((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
				tile = perspective->tileIds[tile];
				tile += layer * WORLD_MAP_LAYER_TILES;
				fdWorldMapWriteLayerCellTile(layer, firstQuarterX + col,
											 firstQuarterY + row, tile, 1);
			}
		}
	}
}
