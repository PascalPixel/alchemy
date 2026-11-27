/* @file */
/* The Ring, the field Psynergy of the unused ability "Ma????" (named here for
 */
/* the ring of light it draws): its task, its burst of sparkles, and moving the
 */
/* view (opening the whole map to it, swinging it and giving the bounds back).
 */
/* Part of field_psynergy.c. */

#include "field/view_move.h"
#include "version.h"

/* The sparkles that rise round the caster as the Ring is drawn. */
#define RING_SPARK_COUNT 64
/* The motes the Ring places, and the first 16 that twinkle on it. */
#define RING_MOTE_COUNT		32
#define RING_CIRCLING_MOTES 16
/* The frames the Ring lasts once it is drawn, or cast again. */
#define RING_FRAMES 600
/* The OBJ and BG palette the Ring's colours cycle in. */
#define RING_PALETTE 15
/* The ring's inner radius in pixels: its four bands run out to 3 more. */
#define RING_RADIUS 52
/* The colour of the ring's band n, 0 innermost: every fourth colour of the */
/* cycling palette, so the bands shimmer one after another. */
#define RING_BAND_COLOR(n) (RING_PALETTE * PALETTE_COLORS + (n) * 4 + 3)
/* The arc the sparkles sweep, and the ring is drawn over, each frame: half */
/* a turn in 32 frames, the picture's lower half being its upper half turned */
/* round (Parch_PictureTiles). */
#define RING_ARC_STEP (TRIG_TURN / 64)

/* The stages of drawing the Ring, 32, 32 and 16 frames. */
typedef enum RING_DRAW_PHASE {
	/* The sparkles rise from two points sweeping half a turn round the caster.
	 */
	RING_SPARKS_CIRCLE = 0,
	/* They sweep the half turn again, and the ring is drawn behind them. */
	RING_DRAWN = 1,
	RING_SPARKS_DIE_OUT = 2 /* no new sparkles start; the last ones rise away */
} RING_DRAW_PHASE;

/* The Ring's state (heap slot battleEffect): the sparkle picture, the 64 */
/* sparkles that rise round the caster as the ring is drawn and burst when it */
/* ends, the motes that circle it after, and what the per-frame task shows. */
typedef struct RING_STATE {
	u8 tiles[4096]; /* the sparkle picture, then the motes', then the burst's */
	EFFECT_SPR sparkSprs[RING_SPARK_COUNT];
	FIELD_PSYNERGY_PARTICLE sparks[RING_SPARK_COUNT];
	/* The motes, placed round the caster; the task draws the first 16. */
	EFFECT_SPR sprs[RING_MOTE_COUNT];
	FIELD_PSYNERGY_PARTICLE motes[RING_MOTE_COUNT];
	s16 entry;	  /* the motes' VRAM cache entry */
	u16 tile;	  /* the motes' first tile */
	s8 sparkling; /* the task draws RING_CIRCLING_MOTES motes round the caster
				   */
	s8 orbiting;  /* the ring sways round the caster as its time runs down */
	s8 cycling;	  /* the task cycles BG and OBJ palette RING_PALETTE */
	u8 unused;
	u16 phase; /* the colour cycle's step */
} RING_STATE;

/* Task of the Ring: its motes and colours each frame. */
void fdPsynergyUpdateRing(void) {
	RING_STATE *state = Ram_HeapSlots->battleEff;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 position[3];
	FIELD_PSYNERGY_PARTICLE *entry;
	EFFECT_SPR *sprite;
	s32 x;
	s32 y;
	s32 i;
	s32 timer;
	u16 *phase;
	u16 *tile;
	s32 red;
	s32 green;
	s32 blue;
	FIXED sine;

	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed + toFIXED(16);
	position[2] = actor->z.fixed;
	if(state->orbiting != 0)
		syVectorAddPolarOffset(toFIXED(2), -(field->ringFrm * (TRIG_TURN / 32)),
							   position);
	fdCameraWorldToScr(position);
	x = FIXEDtoINT(position[0]);
	BgScrollWork[0].x = 64 - x;
	y = FIXEDtoINT(position[2]);
	BgScrollWork[0].y = 64 - y;
	fdBgMapDrawClipped16x16(x - 64, y - 64);

	if(state->cycling != 0) {
		i = 0;
		phase = &state->phase;
		red = GLOW_RED;
	cycle:
		sine = sySin(((*phase >> 1) + i) * (TRIG_TURN / 32));
		green = sine * GLOW_GREEN_SWING;
		sine =
			sySin(((*phase >> 2) + i) * (TRIG_TURN / 32) + TRIG_TURN * 9 / 32);
		blue = sine * GLOW_BLUE_SWING;
		green = FIXEDtoINT(green) + GLOW_GREEN;
		blue = FIXEDtoINT(blue) + GLOW_BLUE;
		syIoWrite16(RGB(red, green, blue),
					&((u16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i]);
		((u16 *)&OBJ_PLTT_COLOR(RING_PALETTE, 0))[i] =
			((u16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i];
		if(++i < PALETTE_COLORS)
			goto cycle;
		state->phase++;
	}

	if(state->sparkling != 0) {
		entry = state->motes;
		sprite = state->sprs;
		tile = &state->tile;
		for(i = 0; i < RING_CIRCLING_MOTES; i++, sprite++, entry++) {
			timer = entry->tmr;
			if(timer == 0) {
				sprite->ang = syRandom();
				position[2] = timer;
				position[1] = timer;
				position[0] = timer;
				syVectorAddPolarOffset(toFIXED(RING_RADIUS + 2),
									   sprite->ang + DEGtoANG(45), position);
				SET_POSITION(entry, position[0], position[1] + toFIXED(16),
							 position[2]);
				timer = entry->tmr;
			}
			if(timer >= 0) {
				sprite->oam.idx = *tile + ((timer >> 2) & 3) * 4;
				position[0] = actor->x.fixed + entry->x;
				position[1] = actor->y.fixed + entry->y;
				position[2] = actor->z.fixed + entry->z;
				fdEffectSprDrawAtWorldPos(sprite, position);
				timer = entry->tmr;
			}
			if((entry->tmr = timer + 1) == 16)
				entry->tmr = 0;
		}
	}

	if(field->ringTmrRunning != 0 && field->ringFrm != 0)
		field->ringFrm--;
	if(field->ringFrm == 0)
		field->psynergy = ABILITY_MAGNET | ACT_PACKED;
}

/* The Ring: a ring of light is drawn round the caster among rising */
/* sparkles, then motes twinkle on it as it sways; fdPsynergyUpdateRing */
/* keeps it for RING_FRAMES frames, then cue ABILITY_MAGNET | ACT_PACKED ends */
/* it (fdPsynergyEndRing). Cast again while it lasts, it only starts its */
/* frames again. */
void fdPsynergyRing(void) {
	RING_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof(RING_STATE));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *caster = scene->caster;
	CANVAS_SPR_STATE *canvas;
	s32 entry;
	s32 tile;
	s32 arcStart;
	s32 angle;
	s32 repeat;
	s32 drawing;
	s32 *pos;
	EFFECT_SPR *sprite;
	FIELD_PSYNERGY_PARTICLE *mote;
	s32 position[3];
	s32 phase;
	s32 timer;
	s32 arc;
	s32 i;
	u8 *picture;

	arcStart = 0;
	angle = 0;
	repeat = 1;
	drawing = 0;
	if(field->ringActive == 0) {
		fdPsynergyFadeToCastCol();
		CpuLoadDisplayEnabled = 0;
		fdBattleEffectStartCanvasSpr();
		canvas = Ram_HeapSlots->canvasSpr;
		canvas->mode = 0;
		pos = position;
		pos[0] = scene->caster->x.fixed;
		pos[1] = scene->caster->y.fixed + toFIXED(16);
		pos[2] = scene->caster->z.fixed;
		fdCameraWorldToScr(pos);
		/* BG0's 128-pixel ring picture is centred on the caster's place on */
		/* screen, its whole pixels. */
		{
			s32 x = ((FIELD_COORDINATE *)pos)[0].part[0].integer;
			s32 y;

			BgScrollWork[0].x = 64 - x;
			y = ((FIELD_COORDINATE *)pos)[2].part[0].integer;
			BgScrollWork[0].y = 64 - y;
		}
		IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 2048, 0);

		syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
		picture = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_CANVAS, 8192);
		IWRAM_FILL_WORDS(picture, 8192, 0);

		sySynch(1);
		syDMAXCopy(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048),
				   REG_DMA3);
		syIoWrite16(BGCNT_SCREEN_BASE(4) | BGCNT_256_COLORS, &REG_BG0CNT);
		syDecompressTaggedLz(
			syResourceGetTableEntry(RESOURCE_STAR_SPARKLE_FADE), state);
		entry = syVramCacheFindFreeEntry();
		tile = syVramCacheLoad(entry, 256, state);
		for(i = 0, mote = state->sparks; i < RING_SPARK_COUNT; i++, mote++) {
			sprite = &state->sparkSprs[i];
			fdEffectSprInit(sprite, 4, 4, 0, tile);
			sprite->oam.fullCol = 1;
			sprite->oam.pal = 0;
			sprite->oam.pri = 0;
			sprite->depth = OAM_DEPTH_WINDOW;
			mote->tmr = -i / 4;
		}
		state->sparkling = 0;
		state->orbiting = 0;
		state->cycling = 1;
		state->phase = 0;
		field->ringActive = 1;
		field->ringTmrRunning = 1;
		field->ringFrm = 999;
		sySetEvent(fdPsynergyUpdateRing, TASK_PASS_FRAME);
		sySynch(1);
		state->cycling = 0;
		phase = RING_SPARKS_CIRCLE;
		timer = 0;
		do {
			if(drawing) {
				arc = arcStart;
				for(i = 0; i < 8; i++) {
					fdCanvasPlotPolarPixel(picture, toFIXED(RING_RADIUS), arc,
										   RING_BAND_COLOR(0));
					fdCanvasPlotPolarPixel(picture, toFIXED(RING_RADIUS + 1),
										   arc, RING_BAND_COLOR(1));
					fdCanvasPlotPolarPixel(picture, toFIXED(RING_RADIUS + 2),
										   arc, RING_BAND_COLOR(2));
					fdCanvasPlotPolarPixel(picture, toFIXED(RING_RADIUS + 3),
										   arc, RING_BAND_COLOR(3));
					arc += RING_ARC_STEP / 8;
				}
				canvas->command = CANVAS_UPLOAD_TO_BG;
			}
			for(i = 0, mote = state->sparks; i < RING_SPARK_COUNT;
				i++, mote++) {
				sprite = &state->sparkSprs[i];
				if(mote->tmr == 0) {
					s32 heading = angle + (i & 1) * DEGtoANG(180);

					SET_POSITION(mote, caster->x.fixed,
								 caster->y.fixed + toFIXED(16),
								 caster->z.fixed);
					syVectorAddPolarOffset(toFIXED(RING_RADIUS + 3), heading,
										   &mote->x);
					syVectorAddPolarOffset(syRandom() * 6, syRandom(),
										   &mote->x);
				}
				if((u32)mote->tmr <= 15) {
					sprite->oam.idx = tile + (mote->tmr & 3) * 2;
					fdEffectSprDrawAtWorldPos(sprite, mote);
					mote->y += -toFIXED(1);
				}
				mote->tmr++;
				if(repeat && mote->tmr == 16)
					mote->tmr = 0;
			}
			switch(phase) {
			case RING_SPARKS_CIRCLE:
				angle = timer * RING_ARC_STEP;
				if(timer == 32) {
					phase = RING_DRAWN;
					timer = -1;
				}
				break;
			case RING_DRAWN:
				drawing = 1;
				angle = arcStart = timer * RING_ARC_STEP;
				if(timer == 32) {
					phase = RING_SPARKS_DIE_OUT;
					timer = -1;
				}
				break;
			case RING_SPARKS_DIE_OUT:
				repeat = 0;
				if(timer == 16)
					phase = FIELD_PSYNERGY_FINISHED;
				break;
			}
			sySynch(1);
			timer++;
		} while(phase != FIELD_PSYNERGY_FINISHED);

		syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_RING_ARCS),
							 picture);
		syDMAXCopy(picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048),
				   REG_DMA3);
		syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_RING_WAVES),
							 state);
		{
			s32 heading;
			s32 moteEntry = syVramCacheFindFreeEntry();
			s32 moteTile = syVramCacheLoad(moteEntry, 512, state);

			state->entry = moteEntry;
			state->tile = moteTile;
			state->sparkling = 1;
			state->orbiting = 1;
			for(i = 0, mote = state->motes; i < RING_MOTE_COUNT; i++) {
				heading = syRandom();
				sprite = &state->sprs[i];
				fdEffectSprInit(sprite, 8, 8, OBJ_SIZE_16X16, moteTile);
				sprite->depth = OAM_DEPTH_WINDOW;
				sprite->oam.pri = 0;
				sprite->oam.fullCol = 0;
				sprite->oam.pal = RING_PALETTE;
				sprite->ang = heading + DEGtoANG(90);
				position[0] = scene->caster->x.fixed;
				position[1] = scene->caster->y.fixed + toFIXED(16);
				position[2] = scene->caster->z.fixed;
				fdCameraWorldToScr(position);
				syVectorAddPolarOffset(toFIXED(64), heading, position);
				mote->x = position[0];
				mote->y = position[2];
				mote->tmr = -i;
				mote++;
			}
		}
		syVramCacheFreeEntry(entry);
		fdPsynergyFadeFromCastCol();
	}
	state->cycling = 1;
	field->ringActive = 1;
	field->ringTmrRunning = 1;
	field->ringFrm = RING_FRAMES;
	sySetEvent(fdPsynergyUpdateRing, TASK_PASS_FRAME);
}

/* Ends the Ring, if one is up: the ring gives way to a burst of 64 sparkles */
/* from round the leader, four more setting off each frame, that fly out and */
/* fall for 40 frames, and the window graphics come back to BG0. */
void fdPsynergyEndRing(void) {
	RING_STATE *state = Ram_HeapSlots->battleEff;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_PSYNERGY_PARTICLE *mote;
	EFFECT_SPR *sprite;
	s32 slot;
	s32 tile;
	s32 angle;
	s32 i;
	s32 frame;

	if(field->ringActive == 0)
		return;
	field->ringActive = 0;
	field->ringTmrRunning = 0;
	field->ringFrm = 0;
	syCloseEvent(fdPsynergyUpdateRing);
	sySynch(1);
	sySndPlayStub(SE_MAP_TRANSITION);
	IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 1280,
					 WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
	syWindowLoadGraphics();
	syIoWrite16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	CpuLoadDisplayEnabled = 1;
	sySynch(1);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_SPARKLE_FADE), state);
	slot = syVramCacheFindFreeEntry();
	tile = syVramCacheLoad(slot, 256, state);

	i = 0;
	mote = state->sparks;
	sprite = state->sparkSprs;
	for(; i < RING_SPARK_COUNT; i++, sprite++, mote++) {
		angle = syRandom();
		SET_POSITION(mote, actor->x.fixed, actor->y.fixed + toFIXED(16),
					 actor->z.fixed);
		syVectorAddPolarOffset(syRandom() * 4 + toFIXED(RING_RADIUS), angle,
							   &mote->x);
		mote->velX = 0;
		mote->velY = -toFIXED(4);
		mote->velZ = 0;
		syVectorAddPolarOffset(syRandom() * 3 + toFIXED(2), angle, &mote->velX);
		mote->tmr = -i / 4;
		fdEffectSprInit(sprite, 4, 4, 0, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
	}
	sySynch(1);

	for(frame = 0; frame < 40; frame++) {
		EFFECT_SPR *spark;
		mote = state->sparks;
		spark = state->sparkSprs;
		for(i = 0; i < RING_SPARK_COUNT; i++, spark++, mote++) {
			if(mote->tmr >= 0) {
				/* angle carries the spark's age here, as it carried each
				 * spark's */
				/* heading above. */
				angle = mote->tmr;
				spark->oam.idx = tile + (angle & 3) * 2;
				fdEffectSprDrawAtWorldPos(spark, mote);
				fdAdvanceWithGravity3D(&mote->x, 62, toFIXED(1) / 2);
			}
			mote->tmr++;
		}
		sySynch(1);
	}
	syVramCacheFreeEntry(slot);
	syVramCacheFreeEntry(state->entry);
	fdBattleEffectEndCanvasSpr();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* How far the view stays from the edge it is beside, in pixels. */
#define VIEW_EDGE_MARGIN 12

/* A map's view bounds, in 8-pixel units. */
typedef struct MAP_BOUNDS {
	s32 minX;
	s32 maxX;
	s32 minZ;
	s32 maxZ;
} MAP_BOUNDS;

/* Writes a map's view bounds from its header, in 8-pixel units. */
void fdMapGetCameraBoundsStub(s32 map, MAP_BOUNDS *bounds);

/* Opens the whole map to the view: keeps the view centre's position and the */
/* map's view bounds for fdViewMoveEnd to restore, notes which edges the view */
/* is beside, and sets the bounds to the map's own. Where the view may not */
/* move (on the World Map, and outside the Japanese release on a map whose */
/* view keeps to its camera boxes), it shakes the screen along the asked */
/* direction instead. */
void fdViewMoveStart(s32 direction) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *center = field->camera;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 viewX = FIXEDtoINT(map->view[0]);
	s32 viewZ = FIXEDtoINT(map->view[1]);
	VIEW_MOVE_STATE *state = Ram_HeapSlots->battleEff;
	MAP_BOUNDS bounds;
	s32 minX;
	s32 maxX;
	s32 minZ;
	s32 maxZ;
	s32 left;
	s32 right;
	s32 top;
	s32 bottom;

	/* Japanese does not check the map's rectangle list here. */
#if TLA_PATCH_1
	if(map->cameraBoxes != NULL) {
		if(direction & VIEW_MOVE_X) {
			fdMapGridSetScrShakeStub(toFIXED(3), 0, FIXED_ONE);
			return;
		}
		if(direction & VIEW_MOVE_Z) {
			fdMapGridSetScrShakeStub(0, toFIXED(3), FIXED_ONE);
			return;
		}
	}
#endif
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(direction & VIEW_MOVE_X) {
			fdMapGridSetScrShakeStub(toFIXED(3), 0, FIXED_ONE);
			return;
		}
		if(direction & VIEW_MOVE_Z) {
			fdMapGridSetScrShakeStub(0, toFIXED(3), FIXED_ONE);
			return;
		}
	}

	fdMapGetCameraBoundsStub(MainRoutineStateWork.mapId, &bounds);
	minX = bounds.minX;
	left = minX * 8;
	maxX = bounds.maxX;
	right = maxX * 8;
	minZ = bounds.minZ;
	top = minZ * 8;
	maxZ = bounds.maxZ;
	bottom = maxZ * 8;
	SET_POSITION(state, center->x.fixed, center->y.fixed, center->z.fixed);

	state->edgeX = 0;
	if(viewX < left + VIEW_EDGE_MARGIN)
		state->edgeX = -1;
	if(viewX + DISPLAY_WIDTH >= right - VIEW_EDGE_MARGIN)
		state->edgeX = 1;
	state->edgeZ = 0;
	if(viewZ < top + VIEW_EDGE_MARGIN)
		state->edgeZ = -1;
	if(viewZ + DISPLAY_HEIGHT >= bottom - VIEW_EDGE_MARGIN)
		state->edgeZ = 1;

	state->savedMinX = map->minX;
	state->savedMaxX = map->maxX;
	state->savedMinZ = map->minZ;
	state->savedMaxZ = map->maxZ;
	map->minX = toFIXED(minX * 8);
	map->maxX = toFIXED(maxX * 8);
	map->minZ = toFIXED(minZ * 8);
	map->maxZ = toFIXED(maxZ * 8);
	fdActorStartIdleStub(center);
}

/* The swing's size at an angle: its sine without the sign. */
#define SWING_SINE(angle) (sySin(angle) >= 0 ? sySin(angle) : -sySin(angle))

/* Swings the view centre away from where fdViewMoveStart found it, along the */
/* asked directions, by amplitude times the sine of the phase. The swing goes */
/* into the room: away from the edge the view is beside, or forward when it is
 */
/* beside neither. Where fdViewMoveStart shook the screen instead, nothing */
/* moves. */
void fdViewMoveSwing(s32 direction, s32 phase, FIXED amplitude) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	VIEW_MOVE_STATE *state = Ram_HeapSlots->battleEff;
	ACTOR *center = field->camera;
	FIXED x;
	FIXED z;
	s32 edge;

	/* Japanese does not check the map's rectangle list here. */
#if TLA_PATCH_1
	if(((MAP_STATE *)Ram_HeapSlots->mapGrid)->cameraBoxes != NULL)
		return;
#endif
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP)
		return;

	if(direction & VIEW_MOVE_X) {
		edge = state->edgeX;
		if(edge == -1)
			x = state->x +
				syMulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
		else if(edge == 1)
			x = state->x +
				syMulFX(-SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
		else
			x = state->x +
				syMulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
	}
	else {
		x = state->x;
	}

	if(direction & VIEW_MOVE_Z) {
		edge = state->edgeZ;
		if(edge == -1)
			z = state->z +
				syMulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
		else if(edge == 1)
			z = state->z +
				syMulFX(-SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
		else
			z = state->z +
				syMulFX(SWING_SINE(phase * (TRIG_TURN / 16)), amplitude);
	}
	else {
		z = state->z;
	}

	SET_ACTOR_POSITION(center, x, state->y, z);
}

/* Ends a view move: gives the map back the view bounds fdViewMoveStart kept */
/* and lets the view follow the caster again. Where fdViewMoveStart shook the */
/* screen instead, it only stops the shake. */
void fdViewMoveEnd(void) {
	VIEW_MOVE_STATE *state = Ram_HeapSlots->battleEff;
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;

#if TLA_PATCH_1
	if(map->cameraBoxes != NULL || field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
		fdMapGridSetScrShakeStub(0, 0, 0);
	}
	else {
		map->minX = state->savedMinX;
		map->maxX = state->savedMaxX;
		map->minZ = state->savedMinZ;
		map->maxZ = state->savedMaxZ;
		fdCameraFollowActor(scene->casterId, 1);
	}
}

/* Gathers up to eight actors near enough to the caster to shake */
/* (fdMeasurePsynergyReach), with each one's sprite rotation, for Tremor's */
/* sway to rock. */
void fdViewMoveSaveNearbySprs(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	VIEW_MOVE_STATE *group = Ram_HeapSlots->battleEff;
	ACTOR **actors;
	ACTOR *actor;
	s32 i;

	group->cnt = 0;
	actors = state->actors;
	for(i = 0; i < FIELD_ACTOR_COUNT; i++) {
		actor = *actors++;
		if(actor == NULL || actor->script == NULL || actor == scene->caster)
			continue;
		if(fdMeasurePsynergyReach(
			   (s16)(scene->fieldPsynergy | REACH_SHAKEN_OBJECTS),
			   scene->casterId, i) == -1)
			continue;
		group->actors[group->cnt] = actor;
		group->sprRotations[group->cnt] = ((SPR *)actor->spr)->rotation;
		if(++group->cnt == ARRAY_COUNT(group->actors))
			break;
	}
}

/* Gives each gathered actor's sprite its saved rotation back. */
void fdViewMoveRestoreNearbySprs(void) {
	VIEW_MOVE_STATE *group = Ram_HeapSlots->battleEff;
	s32 i;

	for(i = 0; i < group->cnt; i++)
		((SPR *)group->actors[i]->spr)->rotation = group->sprRotations[i];
}
#undef VIEW_EDGE_MARGIN
#undef SWING_SINE
#undef RING_RADIUS
#undef RING_BAND_COLOR
#undef RING_ARC_STEP
