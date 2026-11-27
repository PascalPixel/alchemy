/* The world map: its perspective view, its tiles, and drawing its metatiles
   into a BG map. Part of field.c. */

#include "battle/battle_camera.h"
#include "field/world_map.h"
#include "window/window.h"
#include "graphics/palette_buffers.h"

#define WORLD_MAP_BG_PALETTE ((Sint16 *)BG_PLTT)

/* A world map tile is 512 pixels square: WORLD_MAP_TILE gives the tile a 16.16
   coordinate lies in, as FIELD_METATILE a map grid metatile. The world map is
   32 by 32 tiles, wrapping, and each layer's tile numbers follow the 1024
   before it. */
#define WORLD_MAP_TILE(coordinate) ((coordinate) >> 25)
#define WORLD_MAP_TILES_ACROSS	   32
#define WORLD_MAP_LAYER_TILES	   1024

/* MAP_STATE.wideTiles for the world map (WORLD_MAP_VIEW.fadeStep): its tileset
   animations copy 64-byte tiles of 256 colours, into char block 2. */
#define WORLD_MAP_WIDE_TILES 128

/* The world map's four graphics sets, the rows of
   WorldMapGraphicsResources. */
typedef enum WORLD_MAP_GRAPHICS {
	WORLD_MAP_GRAPHICS_DEFAULT, /* the green lands, deserts and mountains */
	/* Snow and ice: Kalt Island, the Northern Reaches, Tundaria. */
	WORLD_MAP_GRAPHICS_SNOWFIELD,
	/* Palm-covered islands in the seas at the world's edges, the Apojii Islands
	   among them. */
	WORLD_MAP_GRAPHICS_TROPICAL,
	/* Prox and Mars Lighthouse, snow falling. */
	WORLD_MAP_GRAPHICS_BLIZZARD
} WORLD_MAP_GRAPHICS;

/* The world map's encounter regions come in groups of ten; three of the groups
   are drawn with a graphics set of their own, the rest with the default one. */
#define WORLD_MAP_REGION_GROUP 10

typedef enum WORLD_MAP_REGION_KIND {
	WORLD_MAP_REGIONS_TROPICAL = 2,
	WORLD_MAP_REGIONS_BLIZZARD = 3,
	WORLD_MAP_REGIONS_SNOWFIELD = 4
} WORLD_MAP_REGION_KIND;

/* The graphics set a group of world map regions is drawn with. */
static inline Sint32 fdWorldMapGetRegionGroupGraphics(Sint32 group)
{
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

/* A world map graphics set, a row of WorldMapGraphicsResources: its palette,
   tileset animation script, metatile tiles attributes and six tile sets. */
typedef enum WORLD_MAP_GRAPHICS_RESOURCE {
	WORLD_MAP_SET_PALETTE,
	WORLD_MAP_SET_ANIMATION,
	WORLD_MAP_SET_METATILE_ATTRIBUTES,
	WORLD_MAP_SET_TILES_1,
	WORLD_MAP_SET_TILES_2,
	WORLD_MAP_SET_TILES_3,
	WORLD_MAP_SET_TILES_4,
	WORLD_MAP_SET_TILES_5,
	WORLD_MAP_SET_TILES_6,
} WORLD_MAP_GRAPHICS_RESOURCE;

/* The still image on BG1 (fdWorldMapStartStillImage): 32 by 20 of the map
   screen's tiles, from its fifth row on. */
#define STILL_IMAGE_TILES_ACROSS 32
#define STILL_IMAGE_TILE_ROWS	 20
#define STILL_IMAGE_FIRST_TILE \
	(MAP_SCREEN_FIRST_TILE + 4 * STILL_IMAGE_TILES_ACROSS)
/* While the still image shows, the perspective's window lies below the screen,
   so no line is drawn in perspective. */
#define STILL_IMAGE_WINDOW_TOP	  200
#define STILL_IMAGE_WINDOW_BOTTOM 255

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
	Sint32		   *cameraPos;
	volatile Uint32 fill;
	FIXED			farPlane;
	Sint32			tileIndex;

	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	syScreenFadeFromBlack(0);
	perspective =
		syHeapSlotAllocate(HEAP_SLOT_MAP_GRID, sizeof(WORLD_MAP_VIEW));
	DMA_FILL32_CELL(fill, 0, perspective, sizeof(WORLD_MAP_VIEW));
	perspective->view[0] = 0;
	perspective->view[1] = 0;
	perspective->scaleX = toFIXED(32);
	perspective->scaleY = toFIXED(64);
	perspective->limitX = toFIXED(16352);
	perspective->limitY = toFIXED(16352);
	perspective->script = NULL;
	perspective->tiles = syResourceGetTableEntry(RESOURCE_PERSPECTIVE_DATA_A);
	syIOWrite16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
			BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
		&REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	REG_BLDY = 0;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_METATILES),
		MapGridMetatileBuffer);
	perspective->shownLayers = DISPCNT_BG0_ON | DISPCNT_BG1_ON |
		DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
	perspective->wideTiles = WORLD_MAP_WIDE_TILES;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_ENTRANCES), MapScene);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG3CNT);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG2CNT);
	syIOWrite16(
		BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5) | BGCNT_SIZE(2), &REG_BG1CNT);
	syIOWrite16(Q_8_8(1), &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	syIOWrite16(Q_8_8(1), &REG_BG2PD);
	REG_BG2X = 0;
	REG_BG2Y = 0;
	syIOWrite16(Q_8_8(1), &REG_BG3PA);
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	syIOWrite16(Q_8_8(1), &REG_BG3PD);
	REG_BG3X = 0;
	syIOWrite16(0, &REG_BG3Y);

	camera = syHeapSlotAllocateEWRAM(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	affineState = syHeapSlotAllocate(
		HEAP_SLOT_WORLD_MAP_AFFINE, sizeof(WORLD_MAP_AFFINE_STATE));
	affinePages = affineState->pages;
	farPlane = toFIXED(510);
	perspective->farPlane = farPlane;
	perspective->distance = farPlane;
	perspective->zoom = toFIXED(1);
	cameraPos = camera->focus;
	perspective->zoomTransitionFrame = 0;
	camera->followEye = 0;
	camera->followPos = 0;
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
		POINT eyeOffset;

		SET_VECTOR(eyeOffset, 0, 0, farPlane);
		syCalcPoint(eyeOffset, camera->eye);
		syInitMatrix();
		syLookAtLoad(camera->eye, cameraPos);
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE, fdWorldMapUpdateAffineLinesCode,
			fdWorldMapUpdateAffineLinesCodeSize);
		fdWorldMapBuildScanlineTable(
			syDivFX(syCos(perspective->pitch), sySin(perspective->pitch)),
			cameraPos, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
		((void (*)(void *, Sint32 *, void *, void *))RamHeapSlots->ramCode)(
			camera, cameraPos, affineState->scanlines,
			affinePages[TickCount & 1]);
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
		if(perspective->fixedEye == 0) {
			SET_VECTOR(eyeOffset, 0, 0, perspective->distance + toFIXED(1));
			syCalcPoint(eyeOffset, camera->eye);
		}
	}
	REG_MOSAIC = 0;
	syIOWrite16(DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	BgScrollWork[2].x = 0;
	BgScrollWork[2].y = 0;
	BgScrollWork[3].x = 0;
	BgScrollWork[3].y = 0;
	perspective->windowTop = 0;
	perspective->windowBottom = DISPLAY_HEIGHT - 1;
	sySetEvent(fdWorldMapUpdateView, EVENT_PASS_TICK + 5);
	sySetEvent(fdWorldMapApplyAffinePage, EVENT_PASS_VBLANK);
	/* Each grid metatile starts on the tile of its own number. */
	for(tileIndex = 0;
		tileIndex < WORLD_MAP_TILES_ACROSS * WORLD_MAP_TILES_ACROSS;
		tileIndex++)
		perspective->tileIds[tileIndex] = tileIndex;
}

/* Puts world map tile (x / 16, y / 16) at metatile (metatileX / 16, metatileY /
   16) of the tile id grid, and draws it at once when it is next to the view.
   metatileX and metatileY count 32-pixel world map blocks, so a grid metatile
   is a 512-pixel tile; it is next to the view when metatileX / 8 and metatileY
   / 8, in 256-pixel steps, are within one step of the followed point's. */
void fdWorldMapSetWindowMetatileTile(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	WORLD_MAP_VIEW *perspective;
	Sint32		   *followedPos;
	Sint32			originX;
	Sint32			originY;
	Sint32			tile;

	perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
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
	tile = (y << 5) + x;
	perspective
		->tileIds[(((metatileY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 5) +
			((metatileX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tile;

	if(ABS(originX - metatileX) <= 1 && ABS(originY - metatileY) <= 1) {
		fdWorldMapWriteLayerMetatileTile(
			0, metatileX / 2, metatileY / 2, tile, 1);
		fdWorldMapWriteLayerMetatileTile(
			1, metatileX / 2, metatileY / 2, tile + WORLD_MAP_LAYER_TILES, 1);
	}
}

/* fdWorldMapSetWindowMetatileTile without its view test: puts world map tile (x
   / 16, y / 16) at metatile (metatileX / 16, metatileY / 16) of the tile grid
   and draws it in both layers. */
void fdWorldMapSetWindowMetatileTileAndDraw(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY)
{
	WORLD_MAP_VIEW *perspective;
	Sint32		   *followedPos;
	Sint32			originX;
	Sint32			originY;
	Sint32			tile;

	perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	/* LEFTOVER(template): the followed point, taken and scaled exactly as
	 * fdWorldMapSetWindowMetatileTile above (and The Broken Seal's
	 * WorldMapSetGridTile) take it for their test of the view; this routine
	 * draws without that test, so nothing reads it; as the routine without
	 * these lines. */
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
	tile = (y << 5) + x;
	perspective
		->tileIds[(((metatileY / 2) & (WORLD_MAP_TILES_ACROSS - 1)) << 5) +
			((metatileX / 2) & (WORLD_MAP_TILES_ACROSS - 1))] = tile;
	fdWorldMapWriteLayerMetatileTile(0, metatileX / 2, metatileY / 2, tile, 1);
	fdWorldMapWriteLayerMetatileTile(
		1, metatileX / 2, metatileY / 2, tile + WORLD_MAP_LAYER_TILES, 1);
}

/* Loads the world map's palette and tiles for the area at (x, z): the set its
   region picks, its tint, and the plain BG maps the world map draws over. */
void fdWorldMapLoadGraphics(Sint32 x, Sint32 z)
{
	Uint8			   *buf;
	WORLD_MAP_VIEW	   *perspective;
	WORLD_MAP_METATILE *block;
	const Uint32	   *resources;
	Sint32				region;
	Sint16				backdropColor;
	Uint32			   *screenTiles;
	Uint32				tilePair;
	Sint32				row;
	Sint32				column;

	buf = syHeapAllocate(BG_PLTT_SIZE);
	perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	block = &((WORLD_MAP_METATILE *)
			syRamMapBlocks)[((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
								(WORLD_MAP_VIEW_METATILES - 1)) +
		(((z / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
			 (WORLD_MAP_VIEW_METATILES - 1)) *
			WORLD_MAP_VIEW_METATILES)];
	fdWorldMapUpdateRegionTint(block->region, 0);
	region = block->region;
	region /= WORLD_MAP_REGION_GROUP;
	resources =
		WorldMapGraphicsResources[fdWorldMapGetRegionGroupGraphics(region)];
	perspective->resources = resources;
	backdropColor = WORLD_MAP_BG_PALETTE[0];
	syDecompressByteLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_PALETTE]), buf);
	*(Sint16 *)buf = backdropColor;
	syDMAXCopy(buf, (void *)BG_PLTT, DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2),
		REG_DMA3);
	fdPaletteFadeLoadPaletteStub(buf);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_ANIMATION]),
		MAPGRID_ANIMATION_SCRIPT);
	fdTilesetAnimationStartAll((const Uint16 *)MAPGRID_ANIMATION_SCRIPT);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_METATILE_ATTRIBUTES]),
		RamMapCollision);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
		RamBgTileBuffer);
	syDMAXCopy(RamBgTileBuffer, BG_CHAR_BLOCK(2),
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
		WORLD_MAP_TILE_SET(1));
	syDMAXCopy(WORLD_MAP_TILE_SET(1),
		BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
		WORLD_MAP_TILE_SET(2));
	syDMAXCopy(WORLD_MAP_TILE_SET(2), BG_CHAR_BLOCK(3),
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
		WORLD_MAP_TILE_SET(3));
	syDMAXCopy(WORLD_MAP_TILE_SET(3),
		BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
		MapGridAnimationTiles);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
		&WorldMapStateWork);
	/* BG1's map (screen block 5) is filled with the window tiles' black tile,
	   and screen block 6 numbers the map screen image's tiles in rows, two a
	   word: the still image's, then on round the image. */
	DMA_FILL32(WINDOW_TILE_PAIR(WINDOW_TILE_COMMAND_PANEL), BG_SCREEN_BLOCK(5),
		24 * BG_SCREEN_WIDTH * sizeof(Uint16));
	screenTiles = (Uint32 *)BG_SCREEN_BLOCK(6);
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
	syHeapRelease(buf);
}

/* HBlank setup each VBlank on the world map: copies the first scanline's BG2
   and BG3 affine parameters, starts the HBlank DMA for the rest, and picks the
   BG mode from the window's top and bottom. */
void fdWorldMapApplyAffinePage(void)
{
	WORLD_MAP_AFFINE_PAIR(*affinePages)
	[DISPLAY_HEIGHT] =
		((WORLD_MAP_AFFINE_STATE *)RamHeapSlots->worldMapAffine)->pages;
	Sint32			displayControl = (Sint16)(REG_DISPCNT & ~DISPCNT_MODE_MASK);
	Uint32		   *affineRegisters = (Uint32 *)&REG_BG2PA;
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	volatile Uint16 *dma0 = REG_DMA0;
	Uint32			*affineSrc;
	Uint32			 mode;
	Uint32			 windowTop;
	Uint32			 nextDisplayControl;

	/* Stops the last VBlank's HBlank DMA, then loads line 0's eight words
	   (BG2PA to BG3Y) by hand; the DMA repeats eight words into the same
	   registers at each HBlank for the lines after it. */
	dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[5] &= ~DMA_ENABLE;
	(void)dma0[5];
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
					<< DMA_CONTROL_SHIFT |
				sizeof(WORLD_MAP_AFFINE_PAIR) / 4,
			(Uint32 *)dma0);
	}
	perspective->shownTop = perspective->windowTop;
	perspective->shownBottom = perspective->windowBottom;
	/* DISPCNT_MODE_2 (the affine ground) for the rows of the whole screen;
	   DISPCNT_MODE_0 for the still image's, which lie below it. */
	mode = DISPCNT_MODE_0;
	windowTop = perspective->shownTop;
	if(windowTop < STILL_IMAGE_WINDOW_TOP) {
		Uint16 bottom = perspective->shownBottom;

		mode = (bottom != 0) << 1;
		if(windowTop <= bottom) {
			mode = DISPCNT_MODE_0;
			if(windowTop == 0)
				mode = DISPCNT_MODE_2;
		}
	}
	displayControl |= mode;
	nextDisplayControl = (Uint16)displayControl;
	REG_DISPCNT = nextDisplayControl;
	perspective->windowPending = 0;
}

/* Copies one world map metatile's tiles to a tile map: its top two, then its
   bottom two on the row below. */
static __inline__ void fdWorldMapCopyMetatileTiles(
	Uint8 *metatileBytes, Uint8 *screenTile)
{
	Uint32	metatileTilesIndex = *(Uint16 *)metatileBytes;
	Uint16 *metatileTiles = (Uint16 *)RamMapMetatileBuffer;

	metatileTiles += metatileTilesIndex * 2;
	*(Uint16 *)screenTile = *metatileTiles++;
	*(Uint16 *)(screenTile + WORLD_MAP_TILE_MAP_WIDTH) = *metatileTiles;
}

/* Draws the metatiles of the world map's tile row tileRow into its tile maps:
   the lower layer's 32 metatiles, then the upper layer's. */
void fdWorldMapDrawMetatileRow(Uint32 tileRow)
{
	Uint8 *srcMetatile;
	Uint8 *screenTile;
	Uint32 metatilesCopied;

	srcMetatile = syRamMapBlocks +
		(((Sint32)tileRow / 2) & (WORLD_MAP_VIEW_METATILES - 1)) *
			WORLD_MAP_VIEW_METATILES * sizeof(WORLD_MAP_METATILE);
	screenTile = (Uint8 *)((Uint32)WORLD_MAP_LOWER_TILE_MAP +
		(tileRow & (WORLD_MAP_TILE_MAP_WIDTH - 2)) * WORLD_MAP_TILE_MAP_WIDTH);
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += 2;
		srcMetatile += sizeof(WORLD_MAP_METATILE);
	} while(metatilesCopied < WORLD_MAP_VIEW_METATILES);

	/* The upper layer's tile map and metatiles follow the lower layer's. */
	screenTile += WORLD_MAP_TILE_MAP_WIDTH * WORLD_MAP_TILE_MAP_WIDTH -
		WORLD_MAP_TILE_MAP_WIDTH;
	srcMetatile += WORLD_MAP_VIEW_LAYER_BYTES -
		WORLD_MAP_VIEW_METATILES * sizeof(WORLD_MAP_METATILE);
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += 2;
		srcMetatile += sizeof(WORLD_MAP_METATILE);
	} while(metatilesCopied < WORLD_MAP_VIEW_METATILES);
}

/* Draws the metatiles of the world map's tile column tileColumn into its tile
   maps: 64 metatiles down, the lower layer's 32 and then the upper layer's,
   which follow them in the tile maps and in the metatiles. */
void fdWorldMapDrawMetatileColumn(Uint32 tileColumn)
{
	Uint8 *srcMetatile;
	Uint8 *screenTile;
	Uint32 metatilesCopied;

	srcMetatile = syRamMapBlocks +
		(((Sint32)tileColumn / 2) & (WORLD_MAP_VIEW_METATILES - 1)) *
			sizeof(WORLD_MAP_METATILE);
	screenTile = (Uint8 *)((Uint32)WORLD_MAP_LOWER_TILE_MAP +
		(tileColumn & (WORLD_MAP_TILE_MAP_WIDTH - 2)));
	metatilesCopied = 0;
	do {
		fdWorldMapCopyMetatileTiles(srcMetatile, screenTile);
		metatilesCopied++;
		screenTile += 2 * WORLD_MAP_TILE_MAP_WIDTH;
		srcMetatile += WORLD_MAP_VIEW_METATILES * sizeof(WORLD_MAP_METATILE);
	} while(metatilesCopied < 2 * WORLD_MAP_VIEW_METATILES);
}

/* Each tick: follows the followed point with the screen shake, draws the rows
   and columns that came into view, and recomputes the projection and the ground
   plane for the current pitch and yaw; while the still image shows, only BG1
   shakes. */
void fdWorldMapUpdateView(void)
{
	BATTLE_CAMERA		   *camera = RamHeapSlots->cameraState;
	WORLD_MAP_AFFINE_STATE *affineState = RamHeapSlots->worldMapAffine;
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	Sint32		   *cameraPos = camera->focus;
	Sint32		   *followedPos = perspective->origin;
	WORLD_MAP_AFFINE_PAIR(*affinePages)[DISPLAY_HEIGHT] = affineState->pages;
	Sint32 distance = perspective->farPlane;
	Sint32 height = perspective->distance;
	Sint32 tileRow;
	FIXED  shakeOffsetX;
	FIXED  shakeOffsetY;
	POINT  eyeOffset;

	shakeOffsetX = 0;
	shakeOffsetY = 0;
	if(perspective->viewHeld != 0) {
		if(perspective->shakeX == 0 && perspective->shakeY == 0)
			return;
	} else {
		fdWorldMapPreloadCurrentTileBlock();
	}
	if(followedPos != NULL) {
		FIXED  x = followedPos[0];
		FIXED  y = followedPos[2];
		Sint32 tileColumn;

		if(perspective->shakeX != 0) {
			Sint32 shakeDifference = syRandom();

			shakeDifference -= syRandom();
			shakeOffsetX = syMulFX(perspective->shakeX, shakeDifference);
			x += shakeOffsetX;
			perspective->shakeX =
				syMulFX(perspective->shakeX, perspective->shakeDecay);
		}
		if(perspective->shakeY != 0) {
			Sint32 shakeDifference = syRandom();

			shakeDifference -= syRandom();
			shakeOffsetY = syMulFX(perspective->shakeY, shakeDifference);
			y += shakeOffsetY;
			perspective->shakeY =
				syMulFX(perspective->shakeY, perspective->shakeDecay);
		}
		tileColumn = x / toFIXED(16);
		tileRow = y / toFIXED(16);
		/* On crossing a 16-pixel line, draws the column 16 on that side, or the
		   row 12 on when y grows and 18 back when it falls. */
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
	cameraPos[0] = perspective->view[0];
	cameraPos[1] = 0;
	cameraPos[2] = perspective->view[1];
	if(perspective->stillImageShown != 0) {
		BgScrollWork[1].x = shakeOffsetX / toFIXED(1);
		BgScrollWork[1].y = shakeOffsetY / toFIXED(1);
		return;
	}
	ProjectionWork.centerX = DISPLAY_WIDTH / 2;
	ProjectionWork.centerY = 96;
	sySetScreenDist(distance, height / 2, height * 2);
	syInitMatrix();
	syMatrixTranslateVector(cameraPos);
	syRotY(perspective->yaw);
	syRotX(perspective->pitch);
	if(perspective->fixedEye == 0) {
		SET_VECTOR(eyeOffset, 0, 0, height + toFIXED(1));
		syCalcPoint(eyeOffset, camera->eye);
	}
	syInitMatrix();
	syLookAtLoad(camera->eye, cameraPos);
	if(ScanlinePitch != perspective->pitch) {
		FIXED cosine = syCos(perspective->pitch);
		FIXED sine = sySin(perspective->pitch);

		fdWorldMapBuildScanlineTable(
			syDivFX(cosine, sine), cameraPos, affineState->scanlines);
		ScanlineAngle = 0;
		ScanlinePitch = perspective->pitch;
	}
	((void (*)(void *, Sint32 *, void *, void *))RamHeapSlots->ramCode)(
		camera, cameraPos, affineState->scanlines, affinePages[TickCount & 1]);
}

/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlock(void)
{
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	Sint32			firstQuarterX = 0;
	Sint32			firstQuarterY = 0;
	Uint32			layer;
	Uint32			row;
	Uint32			column;
	Sint32			tile;

	if(perspective->origin != NULL) {
		Sint32 *followedPos = perspective->origin;

		firstQuarterX = *followedPos++;
		firstQuarterY = followedPos[1];
	}
	/* The top left of the 2x2 tiles around the point, 256 pixels left of it and
	   320 above. */
	firstQuarterX = WORLD_MAP_TILE(firstQuarterX - toFIXED(256));
	firstQuarterY = WORLD_MAP_TILE(firstQuarterY - toFIXED(320));
	for(layer = 0; layer < 2; layer++) {
		for(row = 0; row < 2; row++) {
			for(column = 0; column < 2; column++) {
				tile = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1))
						   << 5) +
					((firstQuarterX + column) & (WORLD_MAP_TILES_ACROSS - 1));
				tile = perspective->tileIds[tile];
				tile += layer * WORLD_MAP_LAYER_TILES;
				fdWorldMapWriteLayerMetatileTile(layer, firstQuarterX + column,
					firstQuarterY + row, tile, 1);
			}
		}
	}
}
