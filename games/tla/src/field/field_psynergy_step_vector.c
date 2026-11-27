/* The canvas a field effect draws into and its darkening, an actor appearing in
   a whirl of sparks, and the field particles that fly to their targets. Part of
   field_psynergy.c. */

/* Moves a 2D point by its velocity, damped and pulled by gravity
   (fdAdvanceWithGravity2D). */
void fdBattleEffectStepVector2(Sint32 *vector, Sint32 damping, Sint32 gravity)
{
	fdAdvanceWithGravity2D(vector, damping, gravity);
}

/* Moves a 3D point by its velocity, damped and pulled by gravity
   (fdAdvanceWithGravity3D). */
void fdBattleEffectStepVector3(Sint32 *vector, Sint32 damping, Sint32 gravity)
{
	fdAdvanceWithGravity3D(vector, damping, gravity);
}

/* Plots a pixel of colour color at (x, y) on the 64-pixel canvas. */
void fdBattleEffectPlotHalfCanvasPixel(Sint32 x, Sint32 y, Sint32 col)
{
	Uint16 *map = Ram_HeapSlots->canvasMap;
	Uint8  *canvas;

	canvas = Ram_HeapSlots->battleCanvas;
	canvas[map[(y << 6) + x]] = col;
}

/* Plots a pixel of colour color at (x, y) on the 128-pixel-wide canvas pair: x
   from 64 lands on the second canvas. */
void fdBattleEffectPlotCanvasPixel(Sint32 x, Sint32 y, Sint32 col)
{
	Uint16 *map = Ram_HeapSlots->canvasMap;
	Uint8  *canvas = Ram_HeapSlots->battleCanvas;

	if(x > 63) {
		canvas += CANVAS_SPRITE_BYTES;
		x -= 64;
	}
	canvas[map[(y << 6) + x]] = col;
}

/* Draws a line of half canvas pixels from x0, y0 to x1, y1, Bresenham's way:
   one pixel per step along the longer axis. */
void fdBattleEffectDrawHalfCanvasLine(
	Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 col)
{
	Sint32 dx = x1 - x0;
	Sint32 dy;
	Sint32 sx;
	Sint32 sy;
	Sint32 err = 0;
	Sint32 x;
	Sint32 y;
	Sint32 i;

	if(dx < 0) {
		sx = -1;
		dx = -dx;
	} else {
		sx = 1;
	}
	dy = y1 - y0;
	if(dy < 0) {
		sy = -1;
		dy = -dy;
	} else {
		sy = 1;
	}
	x = x0;
	y = y0;
	if(dx > dy) {
		for(i = 0; i < dx; i++) {
			fdBattleEffectPlotHalfCanvasPixel(x, y, col);
			err += dy;
			if(err >= dx) {
				err -= dx;
				y += sy;
			}
			x += sx;
		}
	} else {
		for(i = 0; i < dy; i++) {
			fdBattleEffectPlotHalfCanvasPixel(x, y, col);
			err += dx;
			if(err >= dy) {
				err -= dy;
				x += sx;
			}
			y += sy;
		}
	}
}

/* Draws a circle of the given radius round (cx, cy) on the half canvas, by the
   midpoint method: each step plots the eight symmetric points. */
void fdBattleEffectDrawHalfCanvasCircle(
	Sint32 cx, Sint32 cy, Sint32 radius, Sint32 col)
{
	Sint32 x = radius;
	Sint32 y;

	for(y = 0; x >= y; y++) {
		fdBattleEffectPlotHalfCanvasPixel(cx + x, cy + y, col);
		fdBattleEffectPlotHalfCanvasPixel(cx + x, cy - y, col);
		fdBattleEffectPlotHalfCanvasPixel(cx - x, cy + y, col);
		fdBattleEffectPlotHalfCanvasPixel(cx - x, cy - y, col);
		fdBattleEffectPlotHalfCanvasPixel(cx + y, cy + x, col);
		fdBattleEffectPlotHalfCanvasPixel(cx + y, cy - x, col);
		fdBattleEffectPlotHalfCanvasPixel(cx - y, cy + x, col);
		fdBattleEffectPlotHalfCanvasPixel(cx - y, cy - x, col);
		radius = radius - y * 2 + 1;
		if(radius < 0) {
			Sint32 next = x - 1;

			radius += next * 2;
			x = next;
		}
	}
}

/* Copies a width by height block of colour indices onto the battle canvas
   through its metatile map at x, y, skipping zeros; nothing is drawn when the
   block starts off the map or its far edge passes metatile 63. */
void fdBattleEffectDrawCanvasBlock(
	const Uint8 *src, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	Uint16 *map = Ram_HeapSlots->canvasMap;
	Uint8  *canvas = Ram_HeapSlots->battleCanvas;
	Sint32	row;
	Sint32	n;

	if(x < 0 || x + width > 63 || y < 0 || y + height > 63)
		return;
	for(row = 0; row < height; row++) {
		for(n = 0; n < width; n++) {
			if(*src != 0)
				canvas[map[((y + row) << 6) + x + n]] = *src;
			src++;
		}
	}
}

/* The canvas's ramp of colours, from colour 224 (palette 14 of the 256-colour
   canvas), which the darkening maps index by a colour's place in it. */
#define CANVAS_RAMP_COLORS 32

/* Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
   ColorBufferDarkenOneLevelMap, one level darker down the ramp of colours 224
   to 254. */
void fdColBufferDarkenOneLevel(Uint8 *buf, Sint32 cnt)
{
	if(cnt > 0) {
		const Uint8 *tbl = ColorBufferDarkenOneLevelMap;

		do {
			if(*buf != 0)
				*buf = tbl[*buf & (CANVAS_RAMP_COLORS - 1)];
			cnt--;
			buf++;
		} while(cnt != 0);
	}
}

/* Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
   ColorBufferDarkenTwoLevelsMap, two levels darker. */
void fdColBufferDarkenTwoLevels(Uint8 *buf, Sint32 cnt)
{
	if(cnt > 0) {
		const Uint8 *tbl = ColorBufferDarkenTwoLevelsMap;

		do {
			if(*buf != 0)
				*buf = tbl[*buf & (CANVAS_RAMP_COLORS - 1)];
			cnt--;
			buf++;
		} while(cnt != 0);
	}
}

/* Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
   ColorBufferDarkenFourLevelsMap, four levels darker. */
void fdColBufferDarkenFourLevels(Uint8 *buf, Sint32 cnt)
{
	if(cnt > 0) {
		const Uint8 *tbl = ColorBufferDarkenFourLevelsMap;

		do {
			if(*buf != 0)
				*buf = tbl[*buf & (CANVAS_RAMP_COLORS - 1)];
			cnt--;
			buf++;
		} while(cnt != 0);
	}
}

/* The depth the canvas sprite is drawn at, between OAM_DEPTH_WINDOW and
   OAM_DEPTH_FRONT. */
#define CANVAS_SPRITE_DEPTH 250
#define BG_VRAM				((void *)BG_CHAR_BLOCK(0))
#define TILE_SIZE			32

/* The canvas in words, as one 32-bit DMA. */
#define CANVAS_DMA DMA_COPY_WORDS(CANVAS_SPRITE_BYTES / 4)

/* The canvas's tick callback: counts the tick and carries out the command the
   effect left for it, copying the canvas into the sprite's tiles (or to the
   start of BG VRAM), and clearing or fading it after the copy. */
void fdBattleEffectUploadCanvas(void)
{
	CANVAS_SPR_STATE *state = Ram_HeapSlots->canvasSpr;
	Uint8			 *canvas = Ram_HeapSlots->battleCanvas;
	Uint8			 *tiles = OBJ_VRAM + state->tile * TILE_SIZE;

	state->frm++;
	switch(state->command) {
	case CANVAS_UPLOAD:
		syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
		break;
	case CANVAS_UPLOAD_AND_CLEAR:
		syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
		syWordsFill(canvas, CANVAS_SPRITE_BYTES, 0);
		break;
	case CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL:
		syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
		fdColBufferDarkenOneLevel(canvas, CANVAS_SPRITE_BYTES);
		break;
	case CANVAS_UPLOAD_AND_DARKEN_TWO_LEVELS:
		syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
		fdColBufferDarkenTwoLevels(canvas, CANVAS_SPRITE_BYTES);
		break;
	case CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS:
		syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
		fdColBufferDarkenFourLevels(canvas, CANVAS_SPRITE_BYTES);
		break;
	case CANVAS_UPLOAD_TO_BG:
		syDMAXCopy(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SPRITE_BYTES * 2 / 4),
			REG_DMA3);
		break;
	}
	state->command = 0;
}

/* The canvas sprite's tick callback: places the sprite at the state's position
   (whole pixels), scale and angle, and draws it. A sprite scaled to nothing is
   not drawn. */
void fdBattleEffectDrawCanvasSpr(void)
{
	CANVAS_SPR_STATE *state = Ram_HeapSlots->canvasSpr;
	Sint32			  x = toFIXED(state->x);
	Sint32			  y = toFIXED(state->y);
	Sint32			  scaleX = state->scaleX;
	Sint32			  scaleY = state->scaleY;

	if(scaleX != 0 && scaleY != 0) {
		if(state->mode == CANVAS_SPRITE_16_COLORS) {
			fdEffectSprInit(&state->spr, 32, 32, OBJ_SIZE_64X64, state->tile);
			state->spr.oam.fullCol = 0;
			state->spr.oam.pal = CANVAS_PALETTE;
			state->spr.oam.pri = state->pri;
			state->spr.depth = state->depth;
			state->spr.x = x;
			state->spr.y = y;
			state->spr.scaleX = scaleX;
			state->spr.scaleY = scaleY;
			state->spr.ang = state->ang;
			fdEffectSprDraw(&state->spr);
		}
		if(state->mode == CANVAS_SPRITE_256_COLORS) {
			fdEffectSprInit(&state->spr, 32, 32, OBJ_SIZE_64X64, state->tile);
			state->spr.oam.fullCol = 1;
			state->spr.oam.pal = 0;
			state->spr.oam.pri = state->pri;
			state->spr.depth = state->depth;
			state->spr.x = x;
			state->spr.y = y;
			state->spr.scaleX = scaleX;
			state->spr.scaleY = scaleY;
			state->spr.ang = state->ang;
			fdEffectSprDraw(&state->spr);
		}
		if(state->mode == CANVAS_SPRITE_KEPT) {
			state->spr.oam.pri = state->pri;
			state->spr.depth = state->depth;
			state->spr.x = x;
			state->spr.y = y;
			state->spr.scaleX = scaleX;
			state->spr.scaleY = scaleY;
			state->spr.ang = state->ang;
			fdEffectSprDraw(&state->spr);
		}
	}
	state->drawnFrm++;
}

/* The canvas sprite of the field Psynergy scenes. */

/* Starts the canvas sprite of the field Psynergy scenes: its state, a cleared
   64x64 canvas copied into the sprite's tiles, and a map from each canvas pixel
   (row by row) to its place in the tiles. fdBattleEffectDrawCanvasSpr draws the
   sprite and fdBattleEffectUploadCanvas uploads the canvas each tick;
   fdBattleEffectEndCanvasSpr ends it. */
void fdBattleEffectStartCanvasSpr(void)
{
	WIN_STATE		 *winState = (WIN_STATE *)Ram_HeapSlots->winState;
	CANVAS_SPR_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_CANVAS_SPRITE, sizeof(*state));
	Uint8 *canvas =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_SPRITE_BYTES);
	Uint16 *map;
	Sint32	y;
	Sint32	x;

	IWRAM_FILL_WORDS(state, sizeof(*state), 0);
	state->tile = winState->glyphTiles;
	state->mode = CANVAS_SPRITE_16_COLORS;
	state->command = 0;
	state->x = 16;
	state->y = 0;
	state->scaleX = toFIXED(1);
	state->scaleY = toFIXED(1);
	state->pri = 1;
	state->depth = CANVAS_SPRITE_DEPTH;
	syWordsFillEntry(canvas, CANVAS_SPRITE_BYTES, 0);
	syDMAXCopy(canvas, OBJ_VRAM + state->tile * TILE_SIZE,
		DMA_COPY_WORDS(CANVAS_SPRITE_BYTES / 4), REG_DMA3);

	/* Where each pixel of the canvas sits in its 8x8 tiles. */
	map = syHeapSlotAllocEWRAM(
		HEAP_SLOT_CANVAS_MAP, CANVAS_WIDTH * CANVAS_WIDTH * sizeof(Uint16));
	for(y = 0; y < CANVAS_WIDTH; y++) {
		for(x = 0; x < CANVAS_WIDTH; x++) {
			Sint32 row = y & 7;
			Sint32 column = x & 7;
			*map++ = ((((y >> 3) * 8 + (x >> 3)) * 8) + row) * 8 + column;
		}
	}

	sySetEvent(fdBattleEffectDrawCanvasSpr, EVENT_PASS_TICK);
	sySetEvent(fdBattleEffectUploadCanvas, EVENT_PASS_VBLANK - 20);
}

/* Ends the canvas sprite: stops its draw and upload callbacks, waits a tick
   and frees its sprite state, canvas and pixel map. */
void fdBattleEffectEndCanvasSpr(void)
{
	syCloseEvent(fdBattleEffectDrawCanvasSpr);
	syCloseEvent(fdBattleEffectUploadCanvas);
	sySynch(1);
	syHeapSlotRelease(HEAP_SLOT_CANVAS_MAP);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_CANVAS_SPRITE);
}

/* The sparks that circle an actor as it appears. */
#define APPEAR_SPARK_COUNT 64

/* fdActorAppearInSparks's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the spark
   picture, and the sparks that circle the actor and their sprites. */
typedef struct APPEAR_SPARKS_STATE {
	Uint8		picture[1024];
	WHIRL_SPARK sparks[APPEAR_SPARK_COUNT];
	EFFECT_SPR	sprs[APPEAR_SPARK_COUNT];
} APPEAR_SPARKS_STATE;

#define SPARK_LIFE		   50
#define PHASE_DONE		   999 /* the appearance's phases are over */
/* RESOURCE_SPARKLE_FADE's sparkles, each smaller and fainter than the last. */
#define SPARKLE_PICTURES   4
/* The ticks the sparks keep coming, and the ticks the actor then takes to
   widen from nothing to full width. */
#define APPEAR_SPAWN_TICKS 10
#define APPEAR_WIDEN_TICKS 64

/* An actor appears in a whirl of sparks, as Alex does on Venus Lighthouse:
   APPEAR_SPARK_COUNT sparkles (RESOURCE_SPARKLE_FADE) circle round where it
   stands, each rising and closing in for SPARK_LIFE ticks. The actor spins and
   widens from nothing to full width over APPEAR_WIDEN_TICKS, when its shadow
   shows again, and the sparks stop coming APPEAR_SPAWN_TICKS later. */
void fdActorAppearInSparks(Sint32 actorId)
{
	APPEAR_SPARKS_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(APPEAR_SPARKS_STATE));
	Sint32		 phase;
	Sint32		 entry;
	Sint32		 tiles;
	Sint32		 spawning;
	ACTOR		*actor = fdActorLookup(actorId);
	Sint32		 depth = fdActorGetSprDepth(actor);
	Sint32		 ang = (Uint16)actor->facing;
	WHIRL_SPARK *spark;
	EFFECT_SPR	*spr;
	Sint32		 i;
	Sint32		 next;
	Sint32		 frm;
	Sint32		 pos[3];

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_SPARKLE_FADE), state);
	entry = syVRAMCacheFindFreeEntry();
	tiles = syVRAMCacheLoad(entry, sizeof(state->picture), state);

	i = 0;
	spark = state->sparks;
	for(; i < APPEAR_SPARK_COUNT; i++, spark++) {
		spr = &state->sprs[i];
		/* Set up as Teleport's sparks are; each draw picks its sparkle
		   again. */
		fdEffectSprInit(spr, 4, 4, 0,
			tiles + (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->depth = fdActorGetSprDepth(actor);
		spr->oam.pri = fdActorGetSprPriority(actor);
		spark->age = -1;
	}
	actor->scaleX = 0;
	actor->scaleY = toFIXED(1);
	fdActorSetSprFlagsStub(actor, 0);
	spawning = 1;
	phase = 0;
	next = 0;
	frm = 0;
	do {
		if(spawning != 0) {
			for(i = spawning; i != 0; i--) {
				Sint32 sparkAng = syRandom();
				Sint32 radius = syRandom() * 6 + toFIXED(12);

				spark = &state->sparks[next];

				spark->age = 0;
				spark->x = actor->x.fixed;
				spark->y = actor->y.fixed + toFIXED(20);
				spark->z = actor->z.fixed;
				syVectorAddPolarOffset(syRandom(), sparkAng, &spark->x);
				next++;
				spark->ang = sparkAng;
				spark->radius = radius;
				next %= APPEAR_SPARK_COUNT;
			}
		}
		spark = state->sparks;
		for(i = 0; i < APPEAR_SPARK_COUNT; i++, spark++) {
			if(spark->age >= 0) {
				spr = &state->sprs[i];
				if((Uint32)spark->age < SPARK_LIFE) {
					Sint32 sparkAng = spark->ang;
					FIXED  radius = spark->radius;

					pos[0] = spark->x;
					pos[1] = spark->y;
					pos[2] = spark->z;
					syVectorAddPolarOffset(radius, sparkAng, pos);
					if(pos[2] > actor->z.fixed)
						spr->depth = depth + 2;
					else
						spr->depth = depth - 2;
					fdCameraWorldToScreen(pos);
					spr->x = pos[0];
					spr->y = pos[2];
					spr->oam.idx = tiles +
						(spark->age & (SPARKLE_PICTURES - 1)) *
							OBJ_TILES_PER_8BPP_TILE;
					fdEffectSprDraw(spr);
					spark->ang += TRIG_TURN / 32;
					spark->radius -= toFIXED(1) / 5;
					spark->y -= toFIXED(1) / 4;
				}
				if(++spark->age == SPARK_LIFE)
					spark->age = -1;
			}
		}
		switch(phase) {
		case 0:
			if(frm == APPEAR_SPAWN_TICKS) {
				phase = 1;
				frm = -1;
			}
			break;
		case 1:
			if(frm < APPEAR_WIDEN_TICKS) {
				actor->facing -= FACING_STEP;
				actor->scaleX = frm * (toFIXED(1) / APPEAR_WIDEN_TICKS);
				if(actor->scaleX > toFIXED(1))
					actor->scaleX = toFIXED(1);
			}
			if(frm == APPEAR_WIDEN_TICKS) {
				actor->facing = ang;
				fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
			}
			if(frm == APPEAR_SPAWN_TICKS)
				spawning = 0;
			if(frm == APPEAR_WIDEN_TICKS + APPEAR_SPAWN_TICKS)
				phase = PHASE_DONE;
			break;
		}
		sySynch(1);
		frm++;
	} while(phase != PHASE_DONE);
	syVRAMCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Updates one field effect sprite: counts its age, runs its callback (after any
   delay), moves it and draws it. */
void fdParticleUpdate(FIELD_PARTICLE *effect)
{
	if(effect->active != 0) {
		effect->age++;
		if(effect->funcDelay != 0)
			effect->funcDelay--;
		else if(effect->func != 0)
			effect->func(effect);
		if(effect->active != 0) {
			if(effect->updateMotion != 0)
				fdParticleUpdateMotion(effect);
			if(effect->render != 0)
				fdEffectDrawScaledActor(effect);
		}
	}
}

/* Draws a field effect sprite at its screen position and scale, when it is near
   the screen: x from -32 to 272 pixels and z from -32 to 224, both
   exclusive. */
void fdEffectDrawScaledActor(FIELD_PARTICLE *object)
{
	SPR	  *spr;
	Sint32 ofs;
	FIXED  scale[2];
	Sint32 pos[4];

	spr = object->spr;
	ofs = 0;
	if(object->flg & FIELD_PARTICLE_LIFTED_BY_Z)
		ofs = toFIXED(508) - object->z;
	scale[0] = syMulFX(object->scaleX, spr->scale);
	scale[1] = syMulFX(object->scaleY, spr->scale);
	SET_VECTOR(pos, object->x, ofs, object->z + ofs);
	pos[3] = 0;
	if(object->x > -toFIXED(32) && object->x < toFIXED(272) &&
		object->z > -toFIXED(32) && object->z < toFIXED(224))
		fdRenderApplyProjectedPlacementStub(spr, pos, scale, 0);
}

/* Moves a field effect sprite toward its target, turning at most turnStepMax a
   tick when turnStepLimited is set and speeding up by acceleration to
   speedMax. One that stops at its target lands on it once within 8 pixels. */
void fdParticleUpdateMotion(FIELD_PARTICLE *effect)
{
	Sint32 dx;
	Sint32 dz;
	FIXED  distance;
	Sint32 ix;
	Sint32 iz;
	Sint32 speed;
	Sint16 ang;
	Sint32 heading;
	Sint32 facing;
	Sint16 turn;

	if(effect->tgtX == EFFECT_NO_TARGET)
		return;
	dx = effect->tgtX - effect->x;
	dz = effect->tgtZ - effect->z;
	if(effect->stopAtTgt != 0) {
		ix = dx / toFIXED(1);
		iz = dz / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(ix * ix + iz * iz));
		if(distance < toFIXED(128))
			distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
		if(distance <= toFIXED(8)) {
			fdParticleSetPos(effect, effect->tgtX, effect->tgtZ);
			return;
		}
	}
	ang = syAtan(dz, dx);
	if(effect->turnStepLimited != 0) {
		heading = effect->heading;
		turn = ang - heading;
		if((turn >= 0 ? turn : -turn) >= effect->turnStepMax) {
			if(turn < 0) {
				if(-turn > effect->turnStepMax)
					turn = -effect->turnStepMax;
			} else if(turn > effect->turnStepMax) {
				turn = effect->turnStepMax;
			}
			ang = turn + heading;
		}
	}
	facing = (Uint16)ang;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if(speed > effect->speedMax)
		speed = effect->speedMax;
	ang = (Sint16)facing;
	effect->speed = speed;
	effect->x += syMulFX(syCos(ang), speed);
	effect->z += syMulFX(sySin(ang), speed);
}

/* Reports an unfinished target only for effects that stop on arrival. */
Uint32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect)
{
	if(effect->stopAtTgt == 0)
		return 0;
	if(effect->tgtX != EFFECT_NO_TARGET)
		return 1;
	return 0;
}

/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z)
{
	effect->tgtX = EFFECT_NO_TARGET;
	effect->tgtZ = EFFECT_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode)
{
	fdSprStartAnimationStub(effect->spr, mode);
}

/* Gives a field effect sprite its per-tick callback, starting at state 0. */
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK func)
{
	effect->func = func;
	effect->funcDelay = 0;
	effect->age = 0;
	effect->state = 0;
}

/* Starts a field particle with a sprite of the given kind at (x, z), at full
   size and in front of everything: it will fly to its target at up to 2 pixels
   a tick, turning a limited step at a time, and stop there. */
void fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, Sint32 z)
{
	SPR *anm;

	DMA_FILL32(0, effect, sizeof *effect);
	anm = fdSprCreateStub(kind);
	effect->spr = anm;
	if(anm != NULL)
		anm->image.pri = 0;
	fdParticleSetPos(effect, x, z);
	effect->scaleX = toFIXED(1);
	effect->speedMax = toFIXED(2);
	effect->scaleY = toFIXED(1);
	effect->acceleration = toFIXED(1);
	effect->originX = x;
	effect->originZ = z;
	/* BUG: fdSprCreateStub can return NULL, but this and the animation setup
	   below still require a sprite. Callers also immediately use it, so
	   guarding this store alone would not establish allocation-failure
	   recovery. */
	{
		SPR *spr = effect->spr;
		spr->flg = 0;
	}
	effect->stopAtTgt = 1;
	effect->turnStepLimited = 1;
	effect->updateMotion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->randomValue = syRandom();
	effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
	fdParticleSetAnimation(effect, 1);
}

/* Frees a field effect sprite and its sprite, clearing it to zero
   (inactive). */
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect)
{
	if(effect->spr)
		fdSprDestroyStub(effect->spr);
	DMA_FILL32(0, effect, sizeof *effect);
}
