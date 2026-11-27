/* The staff credits, played over the world map after the farewell at Prox: each
   entrance is one stretch of the credits. Between them the camera flies to each
   of the four lighthouses and shows its beacon's light
   (scStaffCreditShowBeacon), and last the four beams meet over Mt. Aleph and a
   light falls from where they meet; then the scene goes on to the summit.

   The overlay links the world map's flight, gauge and placed actors as well;
   what of them the credits never call is marked UNUSED. */
#include "macros.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_mode_script.h"
#include "field/field_sprite.h"
#include "field/perspective.h"
#include "game/game.h"
#include "scene/scene.h"
#include "scene/staff_credit.h"
#include "window/window.h"

#include "shared/ship_flight.h"

/* The beacon's buffer: the glow's or the beams' tiles, then from BEACON_MAPS
   their maps. */
#define BEACON_BUFFER_SIZE 24576
#define BEACON_MAPS		   8192
/* The glow is drawn as a 64-pixel square, one byte a pixel, in levels of light
   1 to 15, each a colour of the picture palette. */
#define GLOW_GRID_SIZE	   64
#define GLOW_LEVEL_MAX	   (PALETTE_COLORS - 1)
/* Two pixels of colour 1, the glow's darkest: the tiles' blank rows. */
#define GLOW_BLANK_PAIR	   ((Uint8)PIXELS_4BPP(1))
/* The beams' brighter colours, in the palette after the picture's. */
#define BEAM_LIGHT_PALETTE (PICTURE_PALETTE + 1)
/* The meeting beams' tiles, loaded from tile 0 of character block 0. */
#define BEAM_TILES		   256
/* The picture layer's tiles the glow clears before it is drawn. */
#define GLOW_CLEARED_TILES 256
/* Where the four beams meet, in pixels from where the last flight ends: the
   light there stands west and north of it. */
#define BEAM_MEET_OFFSET_X (-50)
#define BEAM_MEET_OFFSET_Z (-30)
/* The camera's turn back each tick of the flight home, and each tick of the
   flight out until the turn ends. */
#define FLIGHT_TURN_STEP   (TRIG_TURN / 2048)

/* The beams' palette when they light up. */
extern Sint32 StaffCreditBeamsLightPalette[];

Uint16 StaffCreditBlendLevel  BSS_DATA;
Uint16 StaffCreditBlendSteady BSS_DATA;
/* The actors the World Map has placed (world_map_objects.c). */
ACTOR *WorldMapPlacedActors[128] BSS_DATA;

Uint16 StaffCreditTransitionBlend BSS_DATA;
Sint16 GaugeShown BSS_DATA		  ALIGNED(4);
Sint16 GaugeVram				  BSS_DATA;
Sint16 GaugeCountdown			  BSS_DATA;
/* The Psynergy gauge's sprite, its tiles as drawn, and the pictures they are
   drawn from (world_map_gauge.c). */
Uint32 GaugeSprite[3] BSS_DATA;
Uint8  GaugeTiles[8][32] BSS_DATA;
Uint8  GaugeSourceTiles[12][32] BSS_DATA;
/* The beacon scene's pictures: the beacons' palettes, the beams' palette, their
   maps and their tiles. */
extern Uint8 StaffCreditBeaconPalettes[];
extern Uint8 StaffCreditBeamsPalette[];
extern Uint8 StaffCreditBeamsMaps[];
extern Uint8 StaffCreditBeamsTiles[];
/* Scrolls the credits from the page CREDITS_FROM_PAGE gives (mnCreditsRun,
   through the overlay's own import row). */
void scStaffCreditRunCredits(Sint32 mode);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets the layer blend, switching its alpha every two ticks unless
   StaffCreditBlendSteady is set. */
void scStaffCreditBlendBeacon(void)
{
	scQueueIOWrite16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	if((TickCount & 2) && StaffCreditBlendSteady == 0) {
		scQueueIOWrite16((Uint32)&REG_BLDALPHA,
			StaffCreditBlendLevel | (BLEND_WEIGHT_MAX - 1));
	} else {
		scQueueIOWrite16(
			(Uint32)&REG_BLDALPHA, StaffCreditBlendLevel | BLEND_WEIGHT_MAX);
	}
}

/* Adds a red, green and blue step to each colour of a palette, clamped to 0 to
   31. */
void scStaffCreditStepPaletteCols(
	Uint16 *cols, Sint32 cnt, Sint32 red, Sint32 green, Sint32 blue)
{
	Sint32 i;

	for(i = 0; i != cnt; i++) {
		Sint32 r = RGB_RED(*cols) + red;
		Sint32 g = RGB_GREEN(*cols) + green;
		Sint32 b = RGB_BLUE(*cols) + blue;

		if(r > RGB_LEVEL_MAX)
			r = RGB_LEVEL_MAX;
		if(r < 0)
			r = 0;
		if(g > RGB_LEVEL_MAX)
			g = RGB_LEVEL_MAX;
		if(g < 0)
			g = 0;
		if(b > RGB_LEVEL_MAX)
			b = RGB_LEVEL_MAX;
		if(b < 0)
			b = 0;
		*cols++ = RGB(r, g, b);
	}
}

/* Draws the beacon's glow as rings of light about a corner, brightest at the
   middle. */
void scStaffCreditDrawGlowRings(Uint8 *grid, Sint32 cnt, Sint32 scale)
{
	Sint32 i = 0;
	/* The corner the rings are centred on. */
	Sint32 cornerColumn = GLOW_GRID_SIZE - 1;
	Sint32 cornerRow = GLOW_GRID_SIZE - 1;

	syWordsFill(grid, GLOW_GRID_SIZE * GLOW_GRID_SIZE, PIXELS_8BPP(1));
	if(cnt == 1)
		return;
	do {
		Sint32 y = 0;
		Sint32 last = cnt - 1;
		Sint32 level;
		Sint32 x = i;
		Sint32 error = i;

		level = FIXEDtoINT(syCos(i * (TRIG_TURN / 4) / last) * GLOW_LEVEL_MAX);
		level = level * scale / toFIXED(1);
		if(level > GLOW_LEVEL_MAX)
			level = GLOW_LEVEL_MAX;
		if(level <= 0)
			level = 1;
		for(; x >= y; y++) {
			Sint32 lastColumn = GLOW_GRID_SIZE - 1;
			Sint32 column = cornerColumn - x;
			Sint32 row = cornerRow - y;
			Uint8 *metatile;
			metatile = &grid[(row << 6) + column];
			metatile[0] = level;
			if(column < lastColumn)
				metatile[1] = level;
			/* The same point mirrored across the diagonal. */
			column = row;
			row = cornerRow - x;
			metatile = &grid[(row << 6) + column];
			metatile[0] = level;
			if(column < lastColumn)
				metatile[1] = level;
			error = error - y * 2 - 1;
			if(error < 0) {
				error = error + x * 2 - 2;
				x--;
			}
		}
		i++;
	} while(i != cnt - 1);
}

extern Sint16 StaffCreditFlightStarts[][2];
extern Sint16 StaffCreditFlightEnds[][2];
extern Sint16 StaffCreditFlightReturns[][2];
extern Sint32 StaffCreditStartZooms[];
extern Sint32 StaffCreditEndZooms[];
extern Uint8  StaffCreditFlightTicks[];
extern Uint8  StaffCreditFlightTurns[];
extern Uint16 StaffCreditGlowFadeIn[];
/* The glow layer's scroll as each stage starts, which sets it over its
   lighthouse. */
extern Sint8 StaffCreditGlowPositions[][2];

/* The lighthouses whose beacons the credits fly to, in the order they are
   shown, and last the four beams meeting over Mt. Aleph. */
typedef enum BEACON_STAGE {
	BEACON_MARS,
	BEACON_MERCURY,
	BEACON_VENUS,
	BEACON_JUPITER,
	BEACON_BEAMS_MEET
} BEACON_STAGE;

/* The staff credits' one placed actor, a beacon's glow (SPRITE_MARS_BEACON)
   placed atop Mars Lighthouse; scStaffCreditShowBeacon puts it out of sight,
   and in the last stage grows it where the four beams meet. */
enum { ACTOR_BEACON = ACTOR_FIRST_PLACED };

/* Flies the camera to a lit lighthouse and shows its beacon's glow, then flies
   back; BEACON_BEAMS_MEET is the four beams meeting over Mt. Aleph. */
void scStaffCreditShowBeacon(Sint32 stage)
{
	WORLD_MAP_VIEW *map = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	Sint32			palette;
	Uint8		   *buf;
	Uint8		   *grid;
	Sint32			x;
	Sint32			y;
	Sint32			i;
	Sint32			frm;

	fdActorSetPos(ACTOR_BEACON, 0, 0);
	StaffCreditBlendSteady = 0;
	sySynch(1);
	fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION);
	FieldStateWork->transition = 0;
	palette = stage;
	buf = syHeapAllocEWRAM(BEACON_BUFFER_SIZE);
	fdActorSetSprFlags(fdActorGetById(ACTOR_BEACON), 0);
	if(stage == BEACON_BEAMS_MEET)
		palette = 0;
	BgScrollWork[0].y = 0;
	BgScrollWork[0].x = 0;
	fdCameraMoveToFX(toFIXED(StaffCreditFlightStarts[stage][0]), CAMERA_KEEP,
		toFIXED(StaffCreditFlightStarts[stage][1]), 0);
	fdWorldMapUpdateCurrentTileBlock();
	syFieldWorldMapStartZoom(StaffCreditStartZooms[stage], 1);
	map->yaw = TRIG_TURN / 16;
	map->pitch = TRIG_TURN * 13 / 16;
	syWordsClearEntry(
		(void *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0), PALETTE_COLORS * 2);
	sySynch(1);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transitionFrm = 64;
	fdScriptStartScreenTransition();
	fdCameraMoveToFX(toFIXED(StaffCreditFlightEnds[stage][0]), CAMERA_KEEP,
		toFIXED(StaffCreditFlightEnds[stage][1]), 1);
	for(frm = 0; frm != StaffCreditFlightTicks[stage]; frm++) {
		{
			Sint32 length = StaffCreditFlightTicks[stage];

			map->yaw = (length - frm) * FLIGHT_TURN_STEP;
			if(frm == length - 59)
				syFieldWorldMapStartZoom(StaffCreditEndZooms[stage], 60);
		}
		if(frm >= StaffCreditFlightTicks[stage] - 64) {
			Uint16 *turn = &map->pitch;
			Uint8  *step = StaffCreditFlightTurns + 1;

			*turn += step[stage];
		}
		sySynch(1);
	}
	fdWorldMapFreeze();
	if(stage == BEACON_BEAMS_MEET) {
		Uint8 *ticks = buf + BEACON_MAPS;

		syDecompressTaggedLZ(StaffCreditBeamsTiles, buf);
		syDecompressTaggedLZ(StaffCreditBeamsMaps, ticks);
		IO_WRITE_QUEUE_PUSH(
			VRAM, (Uint32)buf, DMA_COPY_WORDS(BEAM_TILES * TILE_SIZE_4BPP / 4));
		IO_WRITE_QUEUE_PUSH(
			PICTURE_MAP, (Uint32)ticks, DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
		grid = ticks;
		sySynch(1);
	} else {
		Uint16 *tiles = (Uint16 *)buf;

		IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
			(Uint32)(StaffCreditBeaconPalettes + palette * PALETTE_COLORS * 2),
			DMA_COPY_HALFWORDS(PALETTE_COLORS));
		syWordsClear(
			(void *)PICTURE_TILES, GLOW_CLEARED_TILES * TILE_SIZE_4BPP);
		sySynch(1);
		/* The glow's map: its quarter drawn four times, mirrored across and
		   down into a whole disc, and blank rows below. */
		for(y = 0; y != 8; y++) {
			for(x = 0; x != 16; x++) {
				Sint32 tile = y * 16 + x + PICTURE_FIRST_TILE;

				tiles[y * 32 + x] =
					tile | (Sint16)BG_TILE_PALETTE(PICTURE_PALETTE);
				tiles[y * 32 - x + 31] = tile |
					(Sint16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_HFLIP);
				tiles[(15 - y) * 32 + x] = tile |
					(Sint16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_VFLIP);
				tiles[(15 - y) * 32 - x + 31] = tile |
					(Sint16)(BG_TILE_PALETTE(PICTURE_PALETTE) | TILE_HFLIP |
						TILE_VFLIP);
			}
		}
		for(y = 0; y != 16; y++) {
			for(x = 0; x != 32; x++)
				tiles[y * 32 + x + 16 * BG_SCREEN_WIDTH] =
					BG_TILE_PALETTE(PICTURE_PALETTE) | PICTURE_FIRST_TILE;
		}
		IO_WRITE_QUEUE_PUSH(PICTURE_MAP, (Uint32)buf,
			DMA_COPY_HALFWORDS(BG_SCREEN_WIDTH * BG_SCREEN_WIDTH));
		sySynch(1);
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySetEvent(scStaffCreditBlendBeacon, EVENT_PASS_TICK);
	fdScriptPlaySnd(SE_GLITTER);
	for(frm = 0; frm != 172; frm++) {
		if(frm == 0) {
			if(stage == BEACON_BEAMS_MEET) {
				BgScrollWork[0].x = 8;
				BgScrollWork[0].y = frm;
				IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
					(Uint32)StaffCreditBeamsPalette,
					DMA_COPY_HALFWORDS(PALETTE_COLORS));
				IO_WRITE_QUEUE_PUSH(
					(Uint32)&BG_PLTT_COLOR(BEAM_LIGHT_PALETTE, 0),
					(Uint32)StaffCreditBeamsLightPalette,
					DMA_COPY_HALFWORDS(PALETTE_COLORS));
			} else {
				BgScrollWork[0].x = StaffCreditGlowPositions[stage][0];
				BgScrollWork[0].y = StaffCreditGlowPositions[stage][1];
				IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
					(Uint32)(StaffCreditBeaconPalettes +
						palette * PALETTE_COLORS * 2),
					DMA_COPY_HALFWORDS(PALETTE_COLORS));
			}
		}
		if(stage == BEACON_BEAMS_MEET) {

			{
				Sint16 *flightEnds = (Sint16 *)StaffCreditFlightEnds;
				Sint32	endX;
				Sint32	endZ;
				FIXED	ofsZ;
				FIXED	ofsX;
				endX = flightEnds[BEACON_BEAMS_MEET * 2];
				endZ = flightEnds[BEACON_BEAMS_MEET * 2 + 1];
				ofsZ = toFIXED(BEAM_MEET_OFFSET_Z);
				ofsX = toFIXED(BEAM_MEET_OFFSET_X);

				fdActorSetPosFX(
					ACTOR_BEACON, toFIXED(endX) + ofsX, toFIXED(endZ) + ofsZ);
			}
			fdActorSetBlendMode(fdActorGetById(ACTOR_BEACON), 1);
			fdActorSetAnimation(ACTOR_BEACON, 0);
			fdActorGetById(ACTOR_BEACON)->scaleX = 0;
			fdActorGetById(ACTOR_BEACON)->scaleY = 0;
			if(frm > 22) {
				Sint32 scale = (frm - 21) * (toFIXED(1) / 64);

				if(scale > toFIXED(1))
					scale = toFIXED(1);
				fdActorGetById(ACTOR_BEACON)->scaleX = scale;
				fdActorGetById(ACTOR_BEACON)->scaleY = scale;
			}
			if(frm <= 31) {
				Sint32 step = frm / 4;

				if(step <= 6)
					IO_WRITE_QUEUE_PUSH(PICTURE_MAP,
						(Uint32)(buf + step * PICTURE_MAP_BYTES + BEACON_MAPS),
						DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
				StaffCreditBlendLevel = BLEND_ALPHA(0, 10);
			}
			if(frm == 32) {
				StaffCreditBlendSteady = 1;
				StaffCreditBlendLevel = BLEND_ALPHA(0, 10);
			}
			if(frm == 36)
				StaffCreditBlendLevel = BLEND_ALPHA(0, 11);
			if(frm == 40)
				StaffCreditBlendLevel = BLEND_ALPHA(0, 12);
			if(frm == 44)
				StaffCreditBlendLevel = BLEND_ALPHA(0, 13);
			if(frm == 48)
				StaffCreditBlendLevel = BLEND_ALPHA(0, 14);
			if(frm == 52)
				StaffCreditBlendLevel = BLEND_ALPHA(0, 15);
			if(frm == 56)
				StaffCreditBlendLevel = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
			if(frm == 70)
				IO_WRITE_QUEUE_PUSH(PICTURE_MAP,
					(Uint32)(buf + BEACON_MAPS + 7 * PICTURE_MAP_BYTES),
					DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
			if(frm == 72)
				IO_WRITE_QUEUE_PUSH(PICTURE_MAP,
					(Uint32)(buf + BEACON_MAPS + 8 * PICTURE_MAP_BYTES),
					DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
			if(frm == 74)
				IO_WRITE_QUEUE_PUSH(PICTURE_MAP,
					(Uint32)(buf + BEACON_MAPS + 9 * PICTURE_MAP_BYTES),
					DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
			if(frm == 76)
				IO_WRITE_QUEUE_PUSH(PICTURE_MAP,
					(Uint32)(buf + BEACON_MAPS + 10 * PICTURE_MAP_BYTES),
					DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4));
			if(frm > 153) {
				StaffCreditBlendLevel = 0;
				scStaffCreditStepPaletteCols(
					(Uint16 *)&BG_PLTT_COLOR(0, 0), 256, -2, -2, -2);
				scStaffCreditStepPaletteCols(
					(Uint16 *)&OBJ_PLTT_COLOR(0, 0), 256, -2, -2, -2);
			} else if(frm > 79 && (frm & 1)) {
				scStaffCreditStepPaletteCols(
					(Uint16 *)&BG_PLTT_COLOR(0, 0), 256, 1, 1, 1);
				scStaffCreditStepPaletteCols(
					(Uint16 *)&OBJ_PLTT_COLOR(0, 0), 256, 1, 1, 1);
			}
		} else if(frm >= 0) {
			grid = buf + BEACON_MAPS;

			if(frm + 8 <= 63) {
				Sint32 ofs = 0;
				Sint32 by, bx, row;
				Sint32 rowBase;
				Sint32 base;
				Uint8 *out;
				Uint8 *fill;

				scStaffCreditDrawGlowRings(grid, frm + 8, toFIXED(1));
				/* The 64-pixel grid into tiles, one row of tiles at a time
				   (by): eight blank tiles, then the row's eight tiles of the
				   glow (bx), each eight lines of four bytes, two pixels to a
				   byte. */
				by = 0;
				rowBase = 0;
				goto blockRowTest;
			blockRow:
				bx = 0;
				fill = (Uint8 *)ofs + (Sint32)buf;
				do {
					*fill = GLOW_BLANK_PAIR;
					bx++;
					ofs++;
					fill++;
				} while(bx != 8 * TILE_SIZE_4BPP);
				bx = 0;
				base = rowBase * 8;
				goto blockTest;
			block:
				row = 0;
				i = 0;
				do {
					x = 0;
					out = (Uint8 *)ofs + (Sint32)buf;
					do {
						Uint8 *pair = grid + (x * 2 + (base + i));

						x++;
						*out = pair[1] << 4;
						*out |= pair[0];
						ofs++;
						out++;
					} while(x != 4);
					row++;
					i += GLOW_GRID_SIZE;
				} while(row != 8);
				base += 8;
				bx++;
			blockTest:
				if(bx != 8)
					goto block;
				rowBase += 64;
				by++;
			blockRowTest:
				if(by != 8)
					goto blockRow;
				IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (Uint32)buf,
					DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
			}
		}
		if(stage != BEACON_BEAMS_MEET) {
			Uint16 *alpha = &StaffCreditBlendLevel;
			Uint32	scrollTicks = frm - 32;

			/* DEAD: the fade-in's level, at once overwritten with full
			   strength; the ROM keeps the table read and its store. */
			*alpha = frm <= 6 ? StaffCreditGlowFadeIn[frm % 6]
							  : BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
			StaffCreditBlendLevel = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
			if(frm == 138)
				StaffCreditBlendSteady = 1;
			if(frm > 64 && (frm & 1))
				scStaffCreditStepPaletteCols(
					(Uint16 *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0), 16, 1, 1, 1);
			if(frm > 140)
				scStaffCreditStepPaletteCols(
					(Uint16 *)&BG_PLTT_COLOR(PICTURE_PALETTE, 0), 16, -2, -2,
					-2);
			if(scrollTicks <= 95)
				BgScrollWork[0].y++;
		}
		sySynch(1);
	}
	syFieldCloseEvent(scStaffCreditBlendBeacon);
	if(stage != BEACON_BEAMS_MEET) {
		fdCameraSetSpeed(6);
		fdWorldMapReloadPendingAreaGraphics();
		fdCameraMoveToFX(toFIXED(StaffCreditFlightReturns[stage][0]),
			CAMERA_KEEP, toFIXED(StaffCreditFlightReturns[stage][1]), 1);
		syFieldWorldMapStartZoom(toFIXED(7) / 10, 60);
		for(frm = 0; frm != 124;) {
			map->yaw -= FLIGHT_TURN_STEP;
			if(frm == 60) {
				FieldStateWork->transitionFrm = 64;
				fdScriptFinishScreenTransition();
			}
			frm++;
			sySynch(1);
		}
		fdScriptWaitTransitionTicks();
	} else {
		BgScrollWork[0].x = 0;
		BgScrollWork[0].y = 0;
	}
	syHeapRelease(buf);
	syPrintClear();
	scQueueIOWrite16((Uint32)&REG_BLDALPHA, StaffCreditBlendLevel);
	scQueueIOWrite16((Uint32)&REG_BLDCNT, 0);
	fdScriptSetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/* The staff credits' entrances, each a stretch of the credits: the summit
   enters the first two, and each beacon's stretch enters the next. */
typedef enum STAFF_CREDIT_ENTRANCE {
	STAFF_CREDIT_OPENING,	 /* then back to the summit */
	STAFF_CREDIT_TO_BEACONS, /* then on to the beacons */
	STAFF_CREDIT_MARS_BEACON,
	STAFF_CREDIT_MERCURY_BEACON,
	STAFF_CREDIT_VENUS_BEACON,
	STAFF_CREDIT_JUPITER_BEACON,
	STAFF_CREDIT_BEAMS_MEET /* the four beams over Mt. Aleph, then the summit */
} STAFF_CREDIT_ENTRANCE;

/* mnCreditsRun's mode for the scrolling credits from the given page (mode 1 is
   the title's slide show). */
#define CREDITS_FROM_PAGE(page) ((page) + 2)

/* The summit's entrances the credits return to (mt_aleph_summit.c): Alex sees
   the Golden Sun begin to form, and Alex takes the Golden Sun's power. */
enum { SUMMIT_GOLDEN_SUN_FORMS = 2, SUMMIT_ALEX_TAKES_THE_POWER = 3 };

/* Chooses where the credits go next by the entrance they were entered from:
   each beacon's entrance first shows its lighthouse's beacon, the last the
   beams meeting, and each but the last runs the next stretch of credits. */
Sint32 scInit(void)
{
	FIELD_STATE *state;

	fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
	state = RamHeapSlots->fieldState;
	state->transition = 0;
	state->transitionFrm = 16;
	syWait(1);
	fdCameraSetSpeed(8);
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	scWorldMapStartPlacedActors();
	switch(MainRoutineStateWork.entrance) {
	case STAFF_CREDIT_OPENING:
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(0));
		fdScriptSetDestination(SCENE_MT_ALEPH_SUMMIT, SUMMIT_GOLDEN_SUN_FORMS);
		break;
	case STAFF_CREDIT_TO_BEACONS:
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(1));
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_MARS_BEACON);
		break;
	case STAFF_CREDIT_MARS_BEACON:
		scStaffCreditShowBeacon(BEACON_MARS);
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(2));
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_MERCURY_BEACON);
		break;
	case STAFF_CREDIT_MERCURY_BEACON:
		scStaffCreditShowBeacon(BEACON_MERCURY);
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(3));
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_VENUS_BEACON);
		break;
	case STAFF_CREDIT_VENUS_BEACON:
		scStaffCreditShowBeacon(BEACON_VENUS);
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(4));
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_JUPITER_BEACON);
		break;
	case STAFF_CREDIT_JUPITER_BEACON:
		scStaffCreditShowBeacon(BEACON_JUPITER);
		syResourceReservedNoOperation(RESOURCE_CREDITS_STUBS);
		scStaffCreditRunCredits(CREDITS_FROM_PAGE(5));
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_BEAMS_MEET);
		break;
	case STAFF_CREDIT_BEAMS_MEET:
		scStaffCreditShowBeacon(BEACON_BEAMS_MEET);
		fdScriptSetDestination(
			SCENE_MT_ALEPH_SUMMIT, SUMMIT_ALEX_TAKES_THE_POWER);
		break;
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#include "shared/tile_sprite_reset.h"

#include "shared/world_map_objects.c"

/* The ship's flicker and its sparks while it powers up (flight mode 1). */
void scStaffCreditFlashPoweringUp(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & 15) == 0) {
		scWorldMapSpawnSparkPair(actor);
	}
}

/* The step through StaffCreditGlowColorModes, every other tick. */
extern Sint32 StaffCreditGlowColorStep;

/* The ship's glow and its trail in flight (flight mode 2). */
void scStaffCreditGlowInFlight(ACTOR *actor)
{
	if(TickCount & 1) {
		fdActorSetSprColMode(
			actor, StaffCreditGlowColorModes[StaffCreditGlowColorStep]);
		StaffCreditGlowColorStep = (StaffCreditGlowColorStep + 1) & 3;
	}
	if(!(TickCount & 3)) {
		scWorldMapSpawnParticle(actor);
	}
}

/* Sets the ship's look by flight mode (SHIP_FLIGHT_MODE). */
void scActorSetFlightMode(ACTOR *actor, Sint32 mode)
{
	if(mode == SHIP_LANDED) {
		actor->update = 0;
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
		fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	} else if(mode == SHIP_POWERING_UP) {
		actor->update = (OBJECT_UPDATE)scStaffCreditFlashPoweringUp;
	} else {
		actor->update = (OBJECT_UPDATE)scStaffCreditGlowInFlight;
		fdScriptPlaySnd(SE_TAKE_FLIGHT);
	}
}

#include "shared/ship_take_flight.h"

/* Unused: the leader's ship lands, plain again. */
UNUSED void scStaffCreditEndFlight(void)
{
	scActorSetFlightMode(fdActorGet(syFieldPartyGetLeaderActor()), SHIP_LANDED);
}

/* The copy of the world map's scWorldMapUpdateTransitionBlend, which
   scStaffCreditPlayLighthouseTransition runs each tick: every other two ticks
   the blend alternates between two strengths. */
void scStaffCreditUpdateTransitionBlend(void)
{
	scQueueIOWrite16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	if(TickCount & 2) {
		scQueueIOWrite16((Uint32)&REG_BLDALPHA,
			StaffCreditTransitionBlend | (BLEND_WEIGHT_MAX - 4));
	} else {
		scQueueIOWrite16((Uint32)&REG_BLDALPHA,
			StaffCreditTransitionBlend | BLEND_WEIGHT_MAX);
	}
}

#include "shared/camera_jitter.h"

/* Unused: the copy of the world map's scWorldMapPlayLighthouseTransition, the
   view of a lighthouse lit, which the credits never call. */
UNUSED void scStaffCreditPlayLighthouseTransition(Sint32 mode)
{
	Uint8 *buf;

	syWait(1);
	fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION);
	FieldStateWork->transition = 0;
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transitionFrm = 16;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	if(mode == 2) {
		fdMapGridSetShake(2, 3, 1);
		fdScriptPlaySnd(SE_RUMBLE);
	}
	fdWorldMapFreeze();
	buf = syHeapAllocEWRAM(16384);
	syDecompressTaggedLZ(WorldMapPackedTiles, buf);
	syDecompressTaggedLZ(
		WorldMapPackedPictures, buf + PICTURE_FIRST_TILE * TILE_SIZE_4BPP);
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
		(Uint32)(WorldMapPalettes + mode * PALETTE_COLORS * 2),
		DMA_COPY_HALFWORDS(PALETTE_COLORS));
	{
		IO_WRITE_QUEUE *q = &IoWriteQueueWork;
		Uint32			saved;
		Sint32			cnt;

		IRQ_LOCK(saved);
		cnt = q->cnt;
		if(cnt < (Sint32)ARRAY_COUNT(q->entries)) {
			Uint32 *entry = q->entries[cnt];

			*entry++ = (Uint32)buf;
			q->cnt = cnt + 1;
			*entry++ = PICTURE_TILES;
			*entry = DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4);
		}
		IRQ_UNLOCK(saved);
	}
	sySetTickEvent(scStaffCreditUpdateTransitionBlend, EVENT_PASS_TICK);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_GLITTER);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 14);
	PUSH_PICTURE(6);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 13);
	PUSH_PICTURE(5);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 12);
	PUSH_PICTURE(4);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 11);
	PUSH_PICTURE(3);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 10);
	PUSH_PICTURE(2);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 9);
	PUSH_PICTURE(1);
	syFieldScriptWait(2);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 8);
	PUSH_PICTURE(0);
	syFieldScriptWait(140);
	PUSH_PICTURE(1);
	syFieldScriptWait(4);
	PUSH_PICTURE(2);
	syFieldScriptWait(4);
	PUSH_PICTURE(3);
	syFieldScriptWait(4);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 9);
	PUSH_PICTURE(4);
	syFieldScriptWait(4);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 10);
	PUSH_PICTURE(5);
	syFieldScriptWait(4);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 11);
	PUSH_PICTURE(6);
	syFieldScriptWait(4);
	StaffCreditTransitionBlend = BLEND_ALPHA(0, 12);
	PUSH_PICTURE(7);
	if(mode == 1) {
		fdMapGridSetShake(2, 2, 1);
		fdScriptPlaySnd(SE_RUMBLE);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 13);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 14);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 15);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		syFieldScriptWait(45);
		syFieldCloseEvent(scStaffCreditUpdateTransitionBlend);
		syPrintClear();
		scQueueIOWrite16((Uint32)&REG_BLDALPHA, StaffCreditTransitionBlend);
		scQueueIOWrite16((Uint32)&REG_BLDCNT, 0);
	} else {
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 13);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 14);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, 15);
		syFieldScriptWait(4);
		StaffCreditTransitionBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		syFieldScriptWait(45);
		syFieldCloseEvent(scStaffCreditUpdateTransitionBlend);
		syPrintClear();
		scQueueIOWrite16((Uint32)&REG_BLDALPHA, StaffCreditTransitionBlend);
		scQueueIOWrite16((Uint32)&REG_BLDCNT, 0);
	}
	syHeapRelease(buf);
	fdScriptSetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/* Unused: the copy of the world map's scWorldMapFlashWithSparks: the actor
   flashes white, with a pair of sparks every sixteenth tick. */
UNUSED void scStaffCreditFlashWithSparks(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & 15) == 0) {
		scStaffCreditSpawnGrowingPair(actor);
	}
}

/* Cycles an actor's colour mode, sending out a growing pair of sparks every
   sixteenth tick. */
void scActorAnimateAndEmit(ACTOR *actor)
{
	Uint32 frm = TickCount;

	if(frm & 1)
		fdActorSetSprColMode(actor, (frm >> 1) % 6);
	if((TickCount & 15) == 0)
		scStaffCreditSpawnGrowingPair(actor);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

/* Adds a pair of sparks under an actor, one growing and one shrinking. */
void scStaffCreditSpawnGrowingPair(ACTOR *parent)
{
	ACTOR				*pair[2];
	ACTOR				*child;
	SPR					*spr;
	LINKED_EFFECT_STATE *state = RamHeapSlots->fieldEffState;
	Sint32				 i;

	for(i = 0; i < 2; i++) {
		child = (scFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
			parent->y.fixed, parent->z.fixed));
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			spr = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (union FIELD_OBJECT *)parent;
			if(spr != NULL) {
				fdSprStartAnimation(spr, 0);
				spr->flg = 0;
				syVRAMCacheFreeEntry(spr->slot);
				spr->slot = state->vramBlock;
				spr->retainCacheEntry = 1;
				spr->image.tile =
					VramCacheEntries[spr->slot].ofs / TILE_SIZE_4BPP &
					OBJ_TILE_MASK;
				spr->image.fullCol = 0;
				spr->image.shape = 1;
				spr->image.size = 2;
				spr->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR	  *p = pair[0];
		FIELD_SPR *sp = p->spr;

		p->update = (void *)scActorGrowDown;
		sp->pri = 1;
	}
	{
		ACTOR	  *p = pair[1];
		FIELD_SPR *sp = p->spr;

		sp->pri = 1;
		p->update = (void *)scActorGrowUp;
		p->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}

#include "shared/world_map_gauge.c"

Sint32 WorldMapSparkScript[] = {
	SC_CALL((Sint32)scWorldMapRiseFromAnchor),
	SC_RELEASE,
};

/* clang-format off */
#include "shared/world_map_particle_script.c"

/* The colour modes the ship's glow steps through, one every other tick. */
Uint32 StaffCreditGlowColorModes[] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_BLUE_TINT, SPRITE_COLOR_NORMAL, SPRITE_COLOR_GREY,
};
/* clang-format on */

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 10150, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{3, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{4, CONDITION_ALWAYS, 10137, 0, 8602, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{5, CONDITION_ALWAYS, 10423, 0, 8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{6, CONDITION_ALWAYS, 10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{7, CONDITION_ALWAYS, 10578, 0, 8952, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{8, CONDITION_ALWAYS, 10635, 0, 9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{9, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{10, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{11, CONDITION_ALWAYS, 10271, 0, 9420, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{12, CONDITION_ALWAYS, 10380, 0, 9370, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{13, CONDITION_ALWAYS, 10460, 0, 9400, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{14, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{15, CONDITION_ALWAYS, 12070, 0, 10470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{16, CONDITION_ALWAYS, 11880, 0, 10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{17, CONDITION_ALWAYS, 10976, 0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{18, CONDITION_ALWAYS, 11736, 0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{19, CONDITION_ALWAYS, 11910, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{20, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{21, CONDITION_ALWAYS, 9240, 0, 9532, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{22, CONDITION_ALWAYS, 9344, 0, 9530, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{23, CONDITION_ALWAYS, 9148, 0, 9048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{24, CONDITION_ALWAYS, 9148, 0, 8988, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{25, CONDITION_ALWAYS, 9200, 0, 8440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{26, CONDITION_ALWAYS, 9060, 0, 8290, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{27, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{28, CONDITION_ALWAYS, 9546, 0, 8316, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{29, CONDITION_ALWAYS, 12770, 0, 11570, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{30, CONDITION_ALWAYS, 11724, 0, 8064, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{31, CONDITION_ALWAYS, 11680, 0, 6678, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{32, CONDITION_ALWAYS, 13486, 0, 7482, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{33, CONDITION_ALWAYS, 12921, 0, 9639, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{34, CONDITION_ALWAYS, 12921, 0, 9752, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{35, CONDITION_ALWAYS, 12770, 0, 10220, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{36, CONDITION_ALWAYS, 14478, 0, 8410, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{37, CONDITION_ALWAYS, 14250, 0, 8728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{38, CONDITION_ALWAYS, 13370, 0, 6070, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{39, CONDITION_ALWAYS, 13452, 0, 6204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{40, CONDITION_ALWAYS, 10898, 0, 6300, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{41, CONDITION_ALWAYS, 11198, 0, 6132, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{42, CONDITION_ALWAYS, 12472, 0, 7592, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{43, CONDITION_ALWAYS, 12090, 0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{44, CONDITION_ALWAYS, 12972, 0, 1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{45, CONDITION_ALWAYS, 13088, 0, 1090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{46, CONDITION_ALWAYS, 9490, 0, 10960, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{47, CONDITION_ALWAYS, 5836, 0, 10120, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{48, CONDITION_ALWAYS, 8582, 0, 7420, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{49, CONDITION_ALWAYS, 6314, 0, 6223, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{50, CONDITION_ALWAYS, 8848, 0, 4650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{51, CONDITION_ALWAYS, 8970, 0, 7500, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{52, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{53, CONDITION_ALWAYS, 7280, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{54, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{55, CONDITION_ALWAYS, 7275, 0, 7365, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{56, CONDITION_ALWAYS, 7383, 0, 5753, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{57, CONDITION_ALWAYS, 7504, 0, 5650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{58, CONDITION_ALWAYS, 7662, 0, 5558, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{59, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{60, CONDITION_ALWAYS, 8933, 0, 3800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{61, CONDITION_ALWAYS, 8916, 0, 1640, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{62, CONDITION_ALWAYS, 9000, 0, 1307, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{63, CONDITION_ALWAYS, 9040, 0, 1245, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{64, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{65, CONDITION_ALWAYS, 2032, 0, 7600, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{67, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{68, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{69, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{71, CONDITION_ALWAYS, 10720, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{72, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{80, CONDITION_ALWAYS, 11754, 0, 8030, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{81, CONDITION_ALWAYS, 11700, 0, 8034, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{82, CONDITION_ALWAYS, 11596, 0, 10204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{83, CONDITION_ALWAYS, 10240, 0, 9142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{84, CONDITION_ALWAYS, 9020, 0, 882, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{85, CONDITION_ALWAYS, 9288, 0, 9434, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{86, CONDITION_ALWAYS, 9288, 0, 9640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{87, CONDITION_ALWAYS, 12640, 0, 4390, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{88, CONDITION_ALWAYS, 7868, 0, 9120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{89, CONDITION_ALWAYS, 9479, 0, 5060, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{98, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{99, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{100, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{101, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{102, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{103, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MARS_BEACON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		9020, 0, 882, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

LATE_DATA CUE SceneCues[] = {
	{CUE_TABLE_END},
};

/* Where each beacon flight's camera starts, in pixels
   (scStaffCreditShowBeacon). */
/* clang-format off */
LATE_DATA Sint16 StaffCreditFlightStarts[][2] = {
	{9276, 1782}, {10754, 3137}, {10160, 6268}, {8243, 7334}, {8926, 5100},
};

/* Where each flight ends, over its lighthouse, in pixels. */
LATE_DATA Sint16 StaffCreditFlightEnds[][2] = {
	{9020, 782}, {10101, 4284}, {10179, 7270}, {7255, 7305}, {9801, 5100},
};

/* Where the camera flies back to from each beacon, in pixels. */
LATE_DATA Sint16 StaffCreditFlightReturns[][2] = {
	{9570, 2573}, {12179, 4639}, {8727, 7317}, {9751, 6829}, {9751, 5220},
};

/* Each flight's zoom as it starts, as a 16.16 ratio. */
LATE_DATA Sint32 StaffCreditStartZooms[] = {
	toFIXED(1) / 2, toFIXED(4) / 5, toFIXED(3) / 2,
	toFIXED(3) / 5, toFIXED(9) / 10,
};

/* Each flight's zoom over its beacon, as a 16.16 ratio. */
LATE_DATA Sint32 StaffCreditEndZooms[] = {
	toFIXED(1), toFIXED(4) / 5, toFIXED(7) / 10,
	toFIXED(13) / 10, toFIXED(3) / 5,
};

/* Each flight's length, in ticks. */
LATE_DATA Uint8 StaffCreditFlightTicks[] = {
	130, 170, 120, 126,
};

/* How far the camera's pitch turns on each of a flight's last 64 ticks, */
/* read from the second entry on by stage. */
LATE_DATA Uint8 StaffCreditFlightTurns[] = {
	107, 108, 64, 108, 108, 108,
};

/* The beacon glow's blend weights as it fades in. */
LATE_DATA Uint16 StaffCreditGlowFadeIn[] = {
	BLEND_ALPHA(0, 14), BLEND_ALPHA(0, 13), BLEND_ALPHA(0, 12), BLEND_ALPHA(0, 11),
	BLEND_ALPHA(0, 10), BLEND_ALPHA(0, 9),  BLEND_ALPHA(0, 8),
};
/* clang-format on */

LATE_DATA Sint8 StaffCreditGlowPositions[][2] = {
	{22, -4},
	{8, -8},
	{2, -28},
	{14, 12},
	{8, 8},
	{0, 0},
};

LATE_DATA Sint32 WorldMapPlacedActorScript[] = {
	SC_CALL(scTileSprReset),
	SC_END,
	0,
};

LATE_DATA Sint32 StaffCreditGlowColorStep = 0;
