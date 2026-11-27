/* The Ring, the field Psynergy of the unused ability "Ma????" (named here for
   the ring of light it draws): its event, its burst of sparkles, and moving the
   view (opening the whole map to it, swinging it and giving the bounds back).
   Part of field_psynergy.c. */

#include "field/view_move.h"
#include "edition.h"

/* The sparkles that rise round the caster as the Ring is drawn. */
#define RING_SPARK_COUNT	64
/* The motes the Ring places, and the first 16 that twinkle on it. */
#define RING_MOTE_COUNT		32
#define RING_CIRCLING_MOTES 16
/* The ticks the Ring lasts once it is drawn, or cast again. */
#define RING_TICKS			600
/* The OBJ and BG palette the Ring's colours cycle in. */
#define RING_PALETTE		15
/* The ring's inner radius in pixels: its four bands run out to 3 more. */
#define RING_RADIUS			52
/* The colour of the ring's band n, 0 innermost: every fourth colour of the
   cycling palette, so the bands shimmer one after another. */
#define RING_BAND_COLOR(n)	(RING_PALETTE * PALETTE_COLORS + (n) * 4 + 3)
/* The arc the sparkles sweep, and the ring is drawn over, each tick: half a
   turn in 32 ticks, the picture's lower half being its upper half turned round
   (ParchPictureTiles). */
#define RING_ARC_STEP		(TRIG_TURN / 64)

/* The stages of drawing the Ring, 32, 32 and 16 ticks. */
typedef enum RING_DRAW_PHASE {
	/* The sparkles rise from two points sweeping half a turn round the
	   caster. */
	RING_SPARKS_CIRCLE = 0,
	/* They sweep the half turn again, and the ring is drawn behind them. */
	RING_DRAWN = 1,
	RING_SPARKS_DIE_OUT = 2 /* no new sparkles start; the last ones rise away */
} RING_DRAW_PHASE;

/* The Ring's state (heap slot battleEffect): the sparkle picture, the 64
   sparkles that rise round the caster as the ring is drawn and burst when it
   ends, the motes that circle it after, and what the per-tick event shows. */
typedef struct RING_STATE {
	/* the sparkle picture, then the motes', then the burst's */
	Uint8					tiles[4096];
	EFFECT_SPR				sparkSprites[RING_SPARK_COUNT];
	FIELD_PSYNERGY_PARTICLE sparks[RING_SPARK_COUNT];
	/* The motes, placed round the caster; the event draws the first 16. */
	EFFECT_SPR				sprites[RING_MOTE_COUNT];
	FIELD_PSYNERGY_PARTICLE motes[RING_MOTE_COUNT];
	Sint16					entry; /* the motes' VRAM cache entry */
	Uint16					tile;  /* the motes' first tile */
	/* the event draws RING_CIRCLING_MOTES motes round the caster */
	Sint8  sparkling;
	Sint8  orbiting; /* the ring sways round the caster as its time runs down */
	Sint8  cycling;	 /* the event cycles BG and OBJ palette RING_PALETTE */
	Uint8  unused;
	Uint16 phase; /* the colour cycle's step */
} RING_STATE;

/* Event of the Ring: its motes and colours each tick. */
void fdPsynergyUpdateRing(void)
{
	RING_STATE	*state = RamHeapSlots->battleEffect;
	FIELD_STATE *field = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32		 pos[3];
	FIELD_PSYNERGY_PARTICLE *entry;
	EFFECT_SPR				*sprite;
	Sint32					 x;
	Sint32					 y;
	Sint32					 i;
	Sint32					 timer;
	Uint16					*phase;
	Uint16					*tile;
	Sint32					 red;
	Sint32					 green;
	Sint32					 blue;
	FIXED					 sine;

	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed + toFIXED(16);
	pos[2] = actor->z.fixed;
	if(state->orbiting != 0)
		syVectorAddPolarOffset(
			toFIXED(2), -(field->ringFrame * (TRIG_TURN / 32)), pos);
	fdCameraWorldToScreen(pos);
	x = FIXEDtoINT(pos[0]);
	BgScrollWork[0].x = 64 - x;
	y = FIXEDtoINT(pos[2]);
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
		syIOWrite16(RGB(red, green, blue),
			&((Uint16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i]);
		((Uint16 *)&OBJ_PLTT_COLOR(RING_PALETTE, 0))[i] =
			((Uint16 *)&BG_PLTT_COLOR(RING_PALETTE, 0))[i];
		if(++i < PALETTE_COLORS)
			goto cycle;
		state->phase++;
	}

	if(state->sparkling != 0) {
		entry = state->motes;
		sprite = state->sprites;
		tile = &state->tile;
		for(i = 0; i < RING_CIRCLING_MOTES; i++, sprite++, entry++) {
			timer = entry->timer;
			if(timer == 0) {
				sprite->angle = syRandom();
				pos[2] = timer;
				pos[1] = timer;
				pos[0] = timer;
				syVectorAddPolarOffset(toFIXED(RING_RADIUS + 2),
					sprite->angle + DEGtoANG(45), pos);
				SET_POSITION(entry, pos[0], pos[1] + toFIXED(16), pos[2]);
				timer = entry->timer;
			}
			if(timer >= 0) {
				sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
				pos[0] = actor->x.fixed + entry->x;
				pos[1] = actor->y.fixed + entry->y;
				pos[2] = actor->z.fixed + entry->z;
				fdEffectSpriteDrawAtWorldPos(sprite, pos);
				timer = entry->timer;
			}
			if((entry->timer = timer + 1) == 16)
				entry->timer = 0;
		}
	}

	if(field->ringTimerRunning != 0 && field->ringFrame != 0)
		field->ringFrame--;
	if(field->ringFrame == 0)
		field->psynergy = ABILITY_MAGNET | ACT_PACKED;
}

/* The Ring: a ring of light is drawn round the caster among rising sparkles,
   then motes twinkle on it as it sways; fdPsynergyUpdateRing keeps it for
   RING_TICKS ticks, then sound ABILITY_MAGNET | ACT_PACKED ends it
   (fdPsynergyEndRing). Cast again while it lasts, it only starts its ticks
   again. */
void fdPsynergyRing(void)
{
	RING_STATE *state =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_EFFECT, sizeof(RING_STATE));
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffectState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	ACTOR					*caster = scene->caster;
	CANVAS_SPR_STATE		*canvas;
	Sint32					 entry;
	Sint32					 tile;
	Sint32					 arcStart;
	Sint32					 angle;
	Sint32					 repeat;
	Sint32					 drawing;
	Sint32					*posPtr;
	EFFECT_SPR				*sprite;
	FIELD_PSYNERGY_PARTICLE *mote;
	Sint32					 pos[3];
	Sint32					 phase;
	Sint32					 timer;
	Sint32					 arc;
	Sint32					 i;
	Uint8					*picture;

	arcStart = 0;
	angle = 0;
	repeat = 1;
	drawing = 0;
	if(field->ringActive == 0) {
		fdPsynergyFadeToCastColor();
		CpuLoadDisplayEnabled = 0;
		fdBattleEffectStartCanvasSprite();
		canvas = RamHeapSlots->canvasSprite;
		canvas->mode = 0;
		posPtr = pos;
		posPtr[0] = scene->caster->x.fixed;
		posPtr[1] = scene->caster->y.fixed + toFIXED(16);
		posPtr[2] = scene->caster->z.fixed;
		fdCameraWorldToScreen(posPtr);
		/* BG0's 128-pixel ring picture is centred on the caster's place on
		   screen, its whole pixels. */
		{
			Sint32 x = ((FIELD_COORDINATE *)posPtr)[0].part[0].integer;
			Sint32 y;

			BgScrollWork[0].x = 64 - x;
			y = ((FIELD_COORDINATE *)posPtr)[2].part[0].integer;
			BgScrollWork[0].y = 64 - y;
		}
		IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(4), 2048, 0);

		syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
		picture = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_CANVAS, 8192);
		IWRAM_FILL_WORDS(picture, 8192, 0);

		sySynch(1);
		syDMAXCopy(
			picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
		syIOWrite16(BGCNT_SCREEN_BASE(4) | BGCNT_256_COLORS, &REG_BG0CNT);
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_STAR_SPARKLE_FADE), state);
		entry = syVRAMCacheFindFreeEntry();
		tile = syVRAMCacheLoad(entry, 256, state);
		for(i = 0, mote = state->sparks; i < RING_SPARK_COUNT; i++, mote++) {
			sprite = &state->sparkSprites[i];
			fdEffectSpriteInit(sprite, 4, 4, 0, tile);
			sprite->oam.fullColor = 1;
			sprite->oam.palette = 0;
			sprite->oam.priority = 0;
			sprite->depth = OAM_DEPTH_WINDOW;
			mote->timer = -i / 4;
		}
		state->sparkling = 0;
		state->orbiting = 0;
		state->cycling = 1;
		state->phase = 0;
		field->ringActive = 1;
		field->ringTimerRunning = 1;
		field->ringFrame = 999;
		sySetEvent(fdPsynergyUpdateRing, EVENT_PASS_TICK);
		sySynch(1);
		state->cycling = 0;
		phase = RING_SPARKS_CIRCLE;
		timer = 0;
		do {
			if(drawing) {
				arc = arcStart;
				for(i = 0; i < 8; i++) {
					fdCanvasPlotPolarPixel(
						picture, toFIXED(RING_RADIUS), arc, RING_BAND_COLOR(0));
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
				sprite = &state->sparkSprites[i];
				if(mote->timer == 0) {
					Sint32 heading = angle + (i & 1) * DEGtoANG(180);

					SET_POSITION(mote, caster->x.fixed,
						caster->y.fixed + toFIXED(16), caster->z.fixed);
					syVectorAddPolarOffset(
						toFIXED(RING_RADIUS + 3), heading, &mote->x);
					syVectorAddPolarOffset(
						syRandom() * 6, syRandom(), &mote->x);
				}
				if((Uint32)mote->timer <= 15) {
					sprite->oam.index = tile + (mote->timer & 3) * 2;
					fdEffectSpriteDrawAtWorldPos(sprite, mote);
					mote->y += -toFIXED(1);
				}
				mote->timer++;
				if(repeat && mote->timer == 16)
					mote->timer = 0;
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

		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_RING_ARCS), picture);
		syDMAXCopy(
			picture, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_RING_WAVES), state);
		{
			Sint32 heading;
			Sint32 moteEntry = syVRAMCacheFindFreeEntry();
			Sint32 moteTile = syVRAMCacheLoad(moteEntry, 512, state);

			state->entry = moteEntry;
			state->tile = moteTile;
			state->sparkling = 1;
			state->orbiting = 1;
			for(i = 0, mote = state->motes; i < RING_MOTE_COUNT; i++) {
				heading = syRandom();
				sprite = &state->sprites[i];
				fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, moteTile);
				sprite->depth = OAM_DEPTH_WINDOW;
				sprite->oam.priority = 0;
				sprite->oam.fullColor = 0;
				sprite->oam.palette = RING_PALETTE;
				sprite->angle = heading + DEGtoANG(90);
				pos[0] = scene->caster->x.fixed;
				pos[1] = scene->caster->y.fixed + toFIXED(16);
				pos[2] = scene->caster->z.fixed;
				fdCameraWorldToScreen(pos);
				syVectorAddPolarOffset(toFIXED(64), heading, pos);
				mote->x = pos[0];
				mote->y = pos[2];
				mote->timer = -i;
				mote++;
			}
		}
		syVRAMCacheFreeEntry(entry);
		fdPsynergyFadeFromCastColor();
	}
	state->cycling = 1;
	field->ringActive = 1;
	field->ringTimerRunning = 1;
	field->ringFrame = RING_TICKS;
	sySetEvent(fdPsynergyUpdateRing, EVENT_PASS_TICK);
}

/* Ends the Ring, if one is up: the ring gives way to a burst of 64 sparkles
   from round the leader, four more setting off each tick, that fly out and
   fall for 40 ticks, and the window graphics come back to BG0. */
void fdPsynergyEndRing(void)
{
	RING_STATE	*state = RamHeapSlots->battleEffect;
	FIELD_STATE *field = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_PSYNERGY_PARTICLE *mote;
	EFFECT_SPR				*sprite;
	Sint32					 slot;
	Sint32					 tile;
	Sint32					 angle;
	Sint32					 i;
	Sint32					 frame;

	if(field->ringActive == 0)
		return;
	field->ringActive = 0;
	field->ringTimerRunning = 0;
	field->ringFrame = 0;
	syCloseEvent(fdPsynergyUpdateRing);
	sySynch(1);
	sySoundPlayStub(SE_MAP_TRANSITION);
	IWRAM_FILL_WORDS(
		(void *)BG_SCREEN_BLOCK(4), 1280, WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
	syWindowLoadGraphics();
	syIOWrite16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	CpuLoadDisplayEnabled = 1;
	sySynch(1);
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_SPARKLE_FADE), state);
	slot = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(slot, 256, state);

	i = 0;
	mote = state->sparks;
	sprite = state->sparkSprites;
	for(; i < RING_SPARK_COUNT; i++, sprite++, mote++) {
		angle = syRandom();
		SET_POSITION(
			mote, actor->x.fixed, actor->y.fixed + toFIXED(16), actor->z.fixed);
		syVectorAddPolarOffset(
			syRandom() * 4 + toFIXED(RING_RADIUS), angle, &mote->x);
		mote->velocityX = 0;
		mote->velocityY = -toFIXED(4);
		mote->velocityZ = 0;
		syVectorAddPolarOffset(
			syRandom() * 3 + toFIXED(2), angle, &mote->velocityX);
		mote->timer = -i / 4;
		fdEffectSpriteInit(sprite, 4, 4, 0, tile);
		sprite->oam.fullColor = 1;
		sprite->oam.palette = 0;
		sprite->oam.priority = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
	}
	sySynch(1);

	for(frame = 0; frame < 40; frame++) {
		EFFECT_SPR *spark;
		mote = state->sparks;
		spark = state->sparkSprites;
		for(i = 0; i < RING_SPARK_COUNT; i++, spark++, mote++) {
			if(mote->timer >= 0) {
				/* angle carries the spark's age here, as it carried each
				   spark's heading above. */
				angle = mote->timer;
				spark->oam.index = tile + (angle & 3) * 2;
				fdEffectSpriteDrawAtWorldPos(spark, mote);
				fdAdvanceWithGravity3D(&mote->x, 62, toFIXED(1) / 2);
			}
			mote->timer++;
		}
		sySynch(1);
	}
	syVRAMCacheFreeEntry(slot);
	syVRAMCacheFreeEntry(state->entry);
	fdBattleEffectEndCanvasSprite();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* How far the view stays from the edge it is beside, in pixels. */
#define VIEW_EDGE_MARGIN 12

/* A map's view bounds, in 8-pixel units. */
typedef struct MAP_BOUNDS {
	Sint32 minX;
	Sint32 maxX;
	Sint32 minZ;
	Sint32 maxZ;
} MAP_BOUNDS;

/* Writes a map's view bounds from its header, in 8-pixel units. */
void fdMapGetCameraBoundsStub(Sint32 map, MAP_BOUNDS *bounds);

/* Opens the whole map to the view: keeps the view centre's position and the
   map's view bounds for fdViewMoveEnd to restore, notes which edges the view is
   beside, and sets the bounds to the map's own. Where the view may not move (on
   the World Map, and outside the Japanese release on a map whose view keeps to
   its camera boxes), it shakes the screen along the asked direction instead. */
void fdViewMoveStart(Sint32 direction)
{
	FIELD_STATE		*field = RamHeapSlots->fieldState;
	ACTOR			*center = field->camera;
	MAP_STATE		*map = RamHeapSlots->mapGrid;
	Sint32			 viewX = FIXEDtoINT(map->view[0]);
	Sint32			 viewZ = FIXEDtoINT(map->view[1]);
	VIEW_MOVE_STATE *state = RamHeapSlots->battleEffect;
	MAP_BOUNDS		 bounds;
	Sint32			 minX;
	Sint32			 maxX;
	Sint32			 minZ;
	Sint32			 maxZ;
	Sint32			 left;
	Sint32			 right;
	Sint32			 top;
	Sint32			 bottom;

	/* Japanese does not check the map's rectangle list here. */
#if VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS
	if(map->cameraBoxes != NULL) {
		if(direction & VIEW_MOVE_X) {
			fdMapGridSetScreenShakeStub(toFIXED(3), 0, toFIXED(1));
			return;
		}
		if(direction & VIEW_MOVE_Z) {
			fdMapGridSetScreenShakeStub(0, toFIXED(3), toFIXED(1));
			return;
		}
	}
#endif
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(direction & VIEW_MOVE_X) {
			fdMapGridSetScreenShakeStub(toFIXED(3), 0, toFIXED(1));
			return;
		}
		if(direction & VIEW_MOVE_Z) {
			fdMapGridSetScreenShakeStub(0, toFIXED(3), toFIXED(1));
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

/* Swings the view centre away from where fdViewMoveStart found it, along the
   asked directions, by amplitude times the sine of the phase. The swing goes
   into the room: away from the edge the view is beside, or forward when it is
   beside neither. Where fdViewMoveStart shook the screen instead, nothing
   moves. */
void fdViewMoveSwing(Sint32 direction, Sint32 phase, FIXED amplitude)
{
	FIELD_STATE		*field = RamHeapSlots->fieldState;
	VIEW_MOVE_STATE *state = RamHeapSlots->battleEffect;
	ACTOR			*center = field->camera;
	FIXED			 x;
	FIXED			 z;
	Sint32			 edge;

	/* Japanese does not check the map's rectangle list here. */
#if VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS
	if(((MAP_STATE *)RamHeapSlots->mapGrid)->cameraBoxes != NULL)
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
	} else {
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
	} else {
		z = state->z;
	}

	SET_ACTOR_POSITION(center, x, state->y, z);
}

/* Ends a view move: gives the map back the view bounds fdViewMoveStart kept and
   lets the view follow the caster again. Where fdViewMoveStart shook the screen
   instead, it only stops the shake. */
void fdViewMoveEnd(void)
{
	VIEW_MOVE_STATE	   *state = RamHeapSlots->battleEffect;
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	MAP_STATE		   *map = RamHeapSlots->mapGrid;

#if VIEW_MOVE_SHAKES_ON_CAMERA_BOX_MAPS
	if(map->cameraBoxes != NULL || field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
		fdMapGridSetScreenShakeStub(0, 0, 0);
	} else {
		map->minX = state->savedMinX;
		map->maxX = state->savedMaxX;
		map->minZ = state->savedMinZ;
		map->maxZ = state->savedMaxZ;
		fdCameraFollowActor(scene->casterId, 1);
	}
}

/* Gathers up to eight actors near enough to the caster to shake
   (fdMeasurePsynergyReach), with each one's sprite rotation, for Tremor's sway
   to rock. */
void fdViewMoveSaveNearbySprites(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
	FIELD_STATE		   *state = RamHeapSlots->fieldState;
	VIEW_MOVE_STATE	   *group = RamHeapSlots->battleEffect;
	ACTOR			  **actors;
	ACTOR			   *actor;
	Sint32				i;

	group->count = 0;
	actors = state->actors;
	for(i = 0; i < FIELD_ACTOR_COUNT; i++) {
		actor = *actors++;
		if(actor == NULL || actor->script == NULL || actor == scene->caster)
			continue;
		if(fdMeasurePsynergyReach(
			   (Sint16)(scene->fieldPsynergy | REACH_SHAKEN_OBJECTS),
			   scene->casterId, i) == -1)
			continue;
		group->actors[group->count] = actor;
		group->spriteRotations[group->count] = ((SPR *)actor->sprite)->rotation;
		if(++group->count == ARRAY_COUNT(group->actors))
			break;
	}
}

/* Gives each gathered actor's sprite its saved rotation back. */
void fdViewMoveRestoreNearbySprites(void)
{
	VIEW_MOVE_STATE *group = RamHeapSlots->battleEffect;
	Sint32			 i;

	for(i = 0; i < group->count; i++)
		((SPR *)group->actors[i]->sprite)->rotation = group->spriteRotations[i];
}

#undef VIEW_EDGE_MARGIN
#undef SWING_SINE
#undef RING_RADIUS
#undef RING_BAND_COLOR
#undef RING_ARC_STEP
