// The canvas a field effect draws into and its darkening, an actor appearing in a whirl of sparks,
// and the field particles that fly to their targets. Part of field_psynergy.c.

// Moves a 2D point by its velocity, damped and pulled by gravity (fdAdvanceWithGravity2D).
void fdBattleEffectStepVector2(Sint32 *vector, Sint32 damping, Sint32 gravity)
{
	fdAdvanceWithGravity2D(vector, damping, gravity);
}

// Moves a 3D point by its velocity, damped and pulled by gravity (fdAdvanceWithGravity3D).
void fdBattleEffectStepVector3(Sint32 *vector, Sint32 damping, Sint32 gravity)
{
	fdAdvanceWithGravity3D(vector, damping, gravity);
}

// Plots a pixel of colour color at (x, y) on the 64-pixel canvas.
void fdBattleEffectPlotHalfCanvasPixel(Sint32 x, Sint32 y, Sint32 color)
{
	Uint16 *map = RamHeapSlots->canvasMap;
	Uint8  *canvas;

	canvas                    = RamHeapSlots->battleCanvas;
	canvas[map[(y << 6) + x]] = color;
}

// Plots a pixel of colour color at (x, y) on the 128-pixel-wide canvas pair: x from 64 lands on the
// second canvas.
void fdBattleEffectPlotCanvasPixel(Sint32 x, Sint32 y, Sint32 color)
{
	Uint16 *map    = RamHeapSlots->canvasMap;
	Uint8  *canvas = RamHeapSlots->battleCanvas;

	if (x > 63)
	{
		canvas += CANVAS_SPRITE_BYTES;
		x      -= 64;
	}
	canvas[map[(y << 6) + x]] = color;
}

// Draws a line of half canvas pixels from x0, y0 to x1, y1, Bresenham's way: one pixel per step
// along the longer axis.
void fdBattleEffectDrawHalfCanvasLine(Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 color)
{
	Sint32 dx = x1 - x0;
	Sint32 dy;
	Sint32 sx;
	Sint32 sy;
	Sint32 err = 0;
	Sint32 x;
	Sint32 y;
	Sint32 i;

	if (dx < 0)
	{
		sx = -1;
		dx = -dx;
	}
	else
	{
		sx = 1;
	}
	dy = y1 - y0;
	if (dy < 0)
	{
		sy = -1;
		dy = -dy;
	}
	else
	{
		sy = 1;
	}
	x = x0;
	y = y0;
	if (dx > dy)
	{
		for (i = 0; i < dx; i++)
		{
			fdBattleEffectPlotHalfCanvasPixel(x, y, color);
			err += dy;
			if (err >= dx)
			{
				err -= dx;
				y   += sy;
			}
			x += sx;
		}
	}
	else
	{
		for (i = 0; i < dy; i++)
		{
			fdBattleEffectPlotHalfCanvasPixel(x, y, color);
			err += dx;
			if (err >= dy)
			{
				err -= dy;
				x   += sx;
			}
			y += sy;
		}
	}
}

// Draws a circle of the given radius round (cx, cy) on the half canvas, by the midpoint method:
// each step plots the eight symmetric points.
void fdBattleEffectDrawHalfCanvasCircle(Sint32 cx, Sint32 cy, Sint32 radius, Sint32 color)
{
	Sint32 x = radius;
	Sint32 y;

	for (y = 0; x >= y; y++)
	{
		fdBattleEffectPlotHalfCanvasPixel(cx + x, cy + y, color);
		fdBattleEffectPlotHalfCanvasPixel(cx + x, cy - y, color);
		fdBattleEffectPlotHalfCanvasPixel(cx - x, cy + y, color);
		fdBattleEffectPlotHalfCanvasPixel(cx - x, cy - y, color);
		fdBattleEffectPlotHalfCanvasPixel(cx + y, cy + x, color);
		fdBattleEffectPlotHalfCanvasPixel(cx + y, cy - x, color);
		fdBattleEffectPlotHalfCanvasPixel(cx - y, cy + x, color);
		fdBattleEffectPlotHalfCanvasPixel(cx - y, cy - x, color);
		radius = radius - y * 2 + 1;
		if (radius < 0)
		{
			Sint32 next  = x - 1;

			radius      += next * 2;
			x            = next;
		}
	}
}

// Copies a width by height block of colour indices onto the battle canvas through its metatile map
// at x, y, skipping zeros; nothing is drawn when the block starts off the map or its far edge
// passes metatile 63.
void fdBattleEffectDrawCanvasBlock(
	const Uint8 *src, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	Uint16 *map    = RamHeapSlots->canvasMap;
	Uint8  *canvas = RamHeapSlots->battleCanvas;
	Sint32  row;
	Sint32  n;

	if (x < 0 || x + width > 63 || y < 0 || y + height > 63)
		return;
	for (row = 0; row < height; row++)
	{
		for (n = 0; n < width; n++)
		{
			if (*src != 0)
				canvas[map[((y + row) << 6) + x + n]] = *src;
			src++;
		}
	}
}

// The canvas's ramp of colours, from colour 224 (palette 14 of the 256-colour canvas), which the
// darkening maps index by a colour's place in it.
#define CANVAS_RAMP_COLORS 32

// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
// ColorBufferDarkenOneLevelMap, one level darker down the ramp of colours 224 to 254.
void fdColorBufferDarkenOneLevel(Uint8 *buf, Sint32 count)
{
	if (count > 0)
	{
		const Uint8 *table = ColorBufferDarkenOneLevelMap;

		do
		{
			if (*buf != 0)
				*buf = table[*buf & (CANVAS_RAMP_COLORS - 1)];
			count--;
			buf++;
		} while (count != 0);
	}
}

// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
// ColorBufferDarkenTwoLevelsMap, two levels darker.
void fdColorBufferDarkenTwoLevels(Uint8 *buf, Sint32 count)
{
	if (count > 0)
	{
		const Uint8 *table = ColorBufferDarkenTwoLevelsMap;

		do
		{
			if (*buf != 0)
				*buf = table[*buf & (CANVAS_RAMP_COLORS - 1)];
			count--;
			buf++;
		} while (count != 0);
	}
}

// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
// ColorBufferDarkenFourLevelsMap, four levels darker.
void fdColorBufferDarkenFourLevels(Uint8 *buf, Sint32 count)
{
	if (count > 0)
	{
		const Uint8 *table = ColorBufferDarkenFourLevelsMap;

		do
		{
			if (*buf != 0)
				*buf = table[*buf & (CANVAS_RAMP_COLORS - 1)];
			count--;
			buf++;
		} while (count != 0);
	}
}

// The depth the canvas sprite is drawn at, between OAM_DEPTH_WINDOW and OAM_DEPTH_FRONT.
#define CANVAS_SPRITE_DEPTH 250
#define BG_VRAM             ((void *)BG_CHAR_BLOCK(0))
#define TILE_SIZE           32

// The canvas in words, as one 32-bit DMA.
#define CANVAS_DMA          DMA_COPY_WORDS(CANVAS_SPRITE_BYTES / 4)

// The canvas's tick callback: counts the tick and carries out the command the effect left for it,
// copying the canvas into the sprite's tiles (or to the start of BG VRAM), and clearing or fading
// it after the copy.
void fdBattleEffectUploadCanvas(void)
{
	CANVAS_SPR_STATE *state  = RamHeapSlots->canvasSprite;
	Uint8            *canvas = RamHeapSlots->battleCanvas;
	Uint8            *tiles  = OBJ_VRAM + state->tile * TILE_SIZE;

	state->frame++;
	switch (state->command)
	{
		case CANVAS_UPLOAD: syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3); break;
		case CANVAS_UPLOAD_AND_CLEAR:
			syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
			syWordsFill(canvas, CANVAS_SPRITE_BYTES, 0);
			break;
		case CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL:
			syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
			fdColorBufferDarkenOneLevel(canvas, CANVAS_SPRITE_BYTES);
			break;
		case CANVAS_UPLOAD_AND_DARKEN_TWO_LEVELS:
			syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
			fdColorBufferDarkenTwoLevels(canvas, CANVAS_SPRITE_BYTES);
			break;
		case CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS:
			syDMAXCopy(canvas, tiles, CANVAS_DMA, REG_DMA3);
			fdColorBufferDarkenFourLevels(canvas, CANVAS_SPRITE_BYTES);
			break;
		case CANVAS_UPLOAD_TO_BG:
			syDMAXCopy(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SPRITE_BYTES * 2 / 4), REG_DMA3);
			break;
	}
	state->command = 0;
}

// The canvas sprite's tick callback: places the sprite at the state's position (whole pixels),
// scale and angle, and draws it. A sprite scaled to nothing is not drawn.
void fdBattleEffectDrawCanvasSprite(void)
{
	CANVAS_SPR_STATE *state  = RamHeapSlots->canvasSprite;
	Sint32            x      = toFIXED(state->x);
	Sint32            y      = toFIXED(state->y);
	Sint32            scaleX = state->scaleX;
	Sint32            scaleY = state->scaleY;

	if (scaleX != 0 && scaleY != 0)
	{
		if (state->mode == CANVAS_SPRITE_16_COLORS)
		{
			fdEffectSpriteInit(&state->sprite, 32, 32, OBJ_SIZE_64X64, state->tile);
			state->sprite.oam.fullColor = 0;
			state->sprite.oam.palette   = CANVAS_PALETTE;
			state->sprite.oam.priority  = state->priority;
			state->sprite.depth         = state->depth;
			state->sprite.x             = x;
			state->sprite.y             = y;
			state->sprite.scaleX        = scaleX;
			state->sprite.scaleY        = scaleY;
			state->sprite.angle         = state->angle;
			fdEffectSpriteDraw(&state->sprite);
		}
		if (state->mode == CANVAS_SPRITE_256_COLORS)
		{
			fdEffectSpriteInit(&state->sprite, 32, 32, OBJ_SIZE_64X64, state->tile);
			state->sprite.oam.fullColor = 1;
			state->sprite.oam.palette   = 0;
			state->sprite.oam.priority  = state->priority;
			state->sprite.depth         = state->depth;
			state->sprite.x             = x;
			state->sprite.y             = y;
			state->sprite.scaleX        = scaleX;
			state->sprite.scaleY        = scaleY;
			state->sprite.angle         = state->angle;
			fdEffectSpriteDraw(&state->sprite);
		}
		if (state->mode == CANVAS_SPRITE_KEPT)
		{
			state->sprite.oam.priority = state->priority;
			state->sprite.depth        = state->depth;
			state->sprite.x            = x;
			state->sprite.y            = y;
			state->sprite.scaleX       = scaleX;
			state->sprite.scaleY       = scaleY;
			state->sprite.angle        = state->angle;
			fdEffectSpriteDraw(&state->sprite);
		}
	}
	state->drawnFrame++;
}

// The canvas sprite of the field Psynergy scenes.

// Starts the canvas sprite of the field Psynergy scenes: its state, a cleared 64x64 canvas copied
// into the sprite's tiles, and a map from each canvas pixel (row by row) to its place in the tiles.
// fdBattleEffectDrawCanvasSprite draws the sprite and fdBattleEffectUploadCanvas uploads the canvas
// each tick; fdBattleEffectEndCanvasSprite ends it.
void fdBattleEffectStartCanvasSprite(void)
{
	WIN_STATE        *windowState = (WIN_STATE *)RamHeapSlots->windowState;
	CANVAS_SPR_STATE *state       = syHeapSlotAllocate(HEAP_SLOT_CANVAS_SPRITE, sizeof(*state));
	Uint8            *canvas = syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_SPRITE_BYTES);
	Uint16           *map;
	Sint32            y;
	Sint32            x;

	IWRAM_FILL_WORDS(state, sizeof(*state), 0);
	state->tile     = windowState->glyphTiles;
	state->mode     = CANVAS_SPRITE_16_COLORS;
	state->command  = 0;
	state->x        = 16;
	state->y        = 0;
	state->scaleX   = toFIXED(1);
	state->scaleY   = toFIXED(1);
	state->priority = 1;
	state->depth    = CANVAS_SPRITE_DEPTH;
	syWordsFillEntry(canvas, CANVAS_SPRITE_BYTES, 0);
	syDMAXCopy(canvas,
	           OBJ_VRAM + state->tile * TILE_SIZE,
	           DMA_COPY_WORDS(CANVAS_SPRITE_BYTES / 4),
	           REG_DMA3);

	// Where each pixel of the canvas sits in its 8x8 tiles.
	map =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_CANVAS_MAP, CANVAS_WIDTH * CANVAS_WIDTH * sizeof(Uint16));
	for (y = 0; y < CANVAS_WIDTH; y++)
	{
		for (x = 0; x < CANVAS_WIDTH; x++)
		{
			Sint32 row    = y & 7;
			Sint32 column = x & 7;
			*map++        = ((((y >> 3) * 8 + (x >> 3)) * 8) + row) * 8 + column;
		}
	}

	sySetEvent(fdBattleEffectDrawCanvasSprite, EVENT_PASS_TICK);
	sySetEvent(fdBattleEffectUploadCanvas, EVENT_PASS_VBLANK - 20);
}

// Ends the canvas sprite: stops its draw and upload callbacks, waits a tick and frees its sprite
// state, canvas and pixel map.
void fdBattleEffectEndCanvasSprite(void)
{
	syCloseEvent(fdBattleEffectDrawCanvasSprite);
	syCloseEvent(fdBattleEffectUploadCanvas);
	sySynch(1);
	syHeapSlotRelease(HEAP_SLOT_CANVAS_MAP);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_CANVAS_SPRITE);
}

// The sparks that circle an actor as it appears.
#define APPEAR_SPARK_COUNT 64

// fdActorAppearInSparks's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the spark picture, and the
// sparks that circle the actor and their sprites.
typedef struct APPEAR_SPARKS_STATE
{
	Uint8       picture[1024];
	WHIRL_SPARK sparks[APPEAR_SPARK_COUNT];
	EFFECT_SPR  sprites[APPEAR_SPARK_COUNT];
} APPEAR_SPARKS_STATE;

#define SPARK_LIFE         50
#define PHASE_DONE         999 // the appearance's phases are over
// RESOURCE_SPARKLE_FADE's sparkles, each smaller and fainter than the last.
#define SPARKLE_PICTURES   4
// The ticks the sparks keep coming, and the ticks the actor then takes to widen from nothing to
// full width.
#define APPEAR_SPAWN_TICKS 10
#define APPEAR_WIDEN_TICKS 64

// An actor appears in a whirl of sparks, as Alex does on Venus Lighthouse: APPEAR_SPARK_COUNT
// sparkles (RESOURCE_SPARKLE_FADE) circle round where it stands, each rising and closing in for
// SPARK_LIFE ticks. The actor spins and widens from nothing to full width over APPEAR_WIDEN_TICKS,
// when its shadow shows again, and the sparks stop coming APPEAR_SPAWN_TICKS later.
void fdActorAppearInSparks(Sint32 actorId)
{
	APPEAR_SPARKS_STATE *state =
		syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(APPEAR_SPARKS_STATE));
	Sint32       phase;
	Sint32       entry;
	Sint32       tiles;
	Sint32       spawning;
	ACTOR       *actor = fdActorLookup(actorId);
	Sint32       depth = fdActorGetSpriteDepth(actor);
	Sint32       angle = (Uint16)actor->facing;
	WHIRL_SPARK *spark;
	EFFECT_SPR  *sprite;
	Sint32       i;
	Sint32       next;
	Sint32       frame;
	Sint32       pos[3];

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_SPARKLE_FADE), state);
	entry = syVRAMCacheFindFreeEntry();
	tiles = syVRAMCacheLoad(entry, sizeof(state->picture), state);

	i     = 0;
	spark = state->sparks;
	for (; i < APPEAR_SPARK_COUNT; i++, spark++)
	{
		sprite = &state->sprites[i];
		// Set up as Teleport's sparks are; each draw picks its sparkle again.
		fdEffectSpriteInit(
			sprite, 4, 4, 0, tiles + (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		sprite->oam.fullColor = 1;
		sprite->oam.palette   = 0;
		sprite->depth         = fdActorGetSpriteDepth(actor);
		sprite->oam.priority  = fdActorGetSpritePriority(actor);
		spark->age            = -1;
	}
	actor->scaleX = 0;
	actor->scaleY = toFIXED(1);
	fdActorSetSpriteFlagsStub(actor, 0);
	spawning = 1;
	phase    = 0;
	next     = 0;
	frame    = 0;
	do
	{
		if (spawning != 0)
		{
			for (i = spawning; i != 0; i--)
			{
				Sint32 sparkAngle = syRandom();
				Sint32 radius     = syRandom() * 6 + toFIXED(12);

				spark             = &state->sparks[next];

				spark->age        = 0;
				spark->x          = actor->x.fixed;
				spark->y          = actor->y.fixed + toFIXED(20);
				spark->z          = actor->z.fixed;
				syVectorAddPolarOffset(syRandom(), sparkAngle, &spark->x);
				next++;
				spark->angle   = sparkAngle;
				spark->radius  = radius;
				next          %= APPEAR_SPARK_COUNT;
			}
		}
		spark = state->sparks;
		for (i = 0; i < APPEAR_SPARK_COUNT; i++, spark++)
		{
			if (spark->age >= 0)
			{
				sprite = &state->sprites[i];
				if ((Uint32)spark->age < SPARK_LIFE)
				{
					Sint32 sparkAngle = spark->angle;
					FIXED  radius     = spark->radius;

					pos[0]            = spark->x;
					pos[1]            = spark->y;
					pos[2]            = spark->z;
					syVectorAddPolarOffset(radius, sparkAngle, pos);
					if (pos[2] > actor->z.fixed)
						sprite->depth = depth + 2;
					else
						sprite->depth = depth - 2;
					fdCameraWorldToScreen(pos);
					sprite->x = pos[0];
					sprite->y = pos[2];
					sprite->oam.index =
						tiles + (spark->age & (SPARKLE_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE;
					fdEffectSpriteDraw(sprite);
					spark->angle  += TRIG_TURN / 32;
					spark->radius -= toFIXED(1) / 5;
					spark->y      -= toFIXED(1) / 4;
				}
				if (++spark->age == SPARK_LIFE)
					spark->age = -1;
			}
		}
		switch (phase)
		{
			case 0:
				if (frame == APPEAR_SPAWN_TICKS)
				{
					phase = 1;
					frame = -1;
				}
				break;
			case 1:
				if (frame < APPEAR_WIDEN_TICKS)
				{
					actor->facing -= FACING_STEP;
					actor->scaleX  = frame * (toFIXED(1) / APPEAR_WIDEN_TICKS);
					if (actor->scaleX > toFIXED(1))
						actor->scaleX = toFIXED(1);
				}
				if (frame == APPEAR_WIDEN_TICKS)
				{
					actor->facing = angle;
					fdActorSetSpriteFlagsStub(actor, SPRITE_FLAG_SHADOW);
				}
				if (frame == APPEAR_SPAWN_TICKS)
					spawning = 0;
				if (frame == APPEAR_WIDEN_TICKS + APPEAR_SPAWN_TICKS)
					phase = PHASE_DONE;
				break;
		}
		sySynch(1);
		frame++;
	} while (phase != PHASE_DONE);
	syVRAMCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

// Updates one field effect sprite: counts its age, runs its callback (after any delay), moves it
// and draws it.
void fdParticleUpdate(FIELD_PARTICLE *effect)
{
	if (effect->active != 0)
	{
		effect->age++;
		if (effect->callbackDelay != 0)
			effect->callbackDelay--;
		else if (effect->callback != 0)
			effect->callback(effect);
		if (effect->active != 0)
		{
			if (effect->updateMotion != 0)
				fdParticleUpdateMotion(effect);
			if (effect->render != 0)
				fdEffectDrawScaledActor(effect);
		}
	}
}

// Draws a field effect sprite at its screen position and scale, when it is near the screen: x from
// -32 to 272 pixels and z from -32 to 224, both exclusive.
void fdEffectDrawScaledActor(FIELD_PARTICLE *object)
{
	SPR   *sprite;
	Sint32 offset;
	FIXED  scale[2];
	Sint32 pos[4];

	sprite = object->sprite;
	offset = 0;
	if (object->flags & FIELD_PARTICLE_LIFTED_BY_Z)
		offset = toFIXED(508) - object->z;
	scale[0] = syMulFX(object->scaleX, sprite->scale);
	scale[1] = syMulFX(object->scaleY, sprite->scale);
	SET_VECTOR(pos, object->x, offset, object->z + offset);
	pos[3] = 0;
	if (object->x > -toFIXED(32) && object->x < toFIXED(272) && object->z > -toFIXED(32)
	    && object->z < toFIXED(224))
		fdRenderApplyProjectedPlacementStub(sprite, pos, scale, 0);
}

// Moves a field effect sprite toward its target, turning at most turnStepMax a tick when
// turnStepLimited is set and speeding up by acceleration to speedMax. One that stops at its target
// lands on it once within 8 pixels.
void fdParticleUpdateMotion(FIELD_PARTICLE *effect)
{
	Sint32 dx;
	Sint32 dz;
	FIXED  distance;
	Sint32 ix;
	Sint32 iz;
	Sint32 speed;
	Sint16 angle;
	Sint32 heading;
	Sint32 facing;
	Sint16 turn;

	if (effect->targetX == EFFECT_NO_TARGET)
		return;
	dx = effect->targetX - effect->x;
	dz = effect->targetZ - effect->z;
	if (effect->stopAtTarget != 0)
	{
		ix       = dx / toFIXED(1);
		iz       = dz / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(ix * ix + iz * iz));
		if (distance < toFIXED(128))
			distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
		if (distance <= toFIXED(8))
		{
			fdParticleSetPos(effect, effect->targetX, effect->targetZ);
			return;
		}
	}
	angle = syAtan(dz, dx);
	if (effect->turnStepLimited != 0)
	{
		heading = effect->heading;
		turn    = angle - heading;
		if ((turn >= 0 ? turn : -turn) >= effect->turnStepMax)
		{
			if (turn < 0)
			{
				if (-turn > effect->turnStepMax)
					turn = -effect->turnStepMax;
			}
			else if (turn > effect->turnStepMax)
			{
				turn = effect->turnStepMax;
			}
			angle = turn + heading;
		}
	}
	facing          = (Uint16)angle;
	effect->heading = facing;
	speed           = effect->speed + effect->acceleration;
	if (speed > effect->speedMax)
		speed = effect->speedMax;
	angle          = (Sint16)facing;
	effect->speed  = speed;
	effect->x     += syMulFX(syCos(angle), speed);
	effect->z     += syMulFX(sySin(angle), speed);
}

// Reports an unfinished target only for effects that stop on arrival.
Uint32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect)
{
	if (effect->stopAtTarget == 0)
		return 0;
	if (effect->targetX != EFFECT_NO_TARGET)
		return 1;
	return 0;
}

// Places a field effect sprite at (x, z), at rest with no target.
void fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z)
{
	effect->targetX = EFFECT_NO_TARGET;
	effect->targetZ = EFFECT_NO_TARGET;
	effect->x       = x;
	effect->z       = z;
	effect->speed   = 0;
}

// Sets the animation of a field effect sprite.
void fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode)
{
	fdSpriteStartAnimationStub(effect->sprite, mode);
}

// Gives a field effect sprite its per-tick callback, starting at state 0.
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK callback)
{
	effect->callback      = callback;
	effect->callbackDelay = 0;
	effect->age           = 0;
	effect->state         = 0;
}

// Starts a field particle with a sprite of the given kind at (x, z), at full size and in front of
// everything: it will fly to its target at up to 2 pixels a tick, turning a limited step at a time,
// and stop there.
void fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, Sint32 z)
{
	SPR *animation;

	DMA_FILL32(0, effect, sizeof *effect);
	animation      = fdSpriteCreateStub(kind);
	effect->sprite = animation;
	if (animation != NULL)
		animation->image.priority = 0;
	fdParticleSetPos(effect, x, z);
	effect->scaleX       = toFIXED(1);
	effect->speedMax     = toFIXED(2);
	effect->scaleY       = toFIXED(1);
	effect->acceleration = toFIXED(1);
	effect->originX      = x;
	effect->originZ      = z;
	// BUG: fdSpriteCreateStub can return NULL, but this and the animation setup below still require
	// a sprite. Callers also immediately use it, so guarding this store alone would not establish
	// allocation-failure recovery.
	{
		SPR *sprite   = effect->sprite;
		sprite->flags = 0;
	}
	effect->stopAtTarget    = 1;
	effect->turnStepLimited = 1;
	effect->updateMotion    = 1;
	effect->render          = 1;
	effect->active          = 1;
	effect->randomValue     = syRandom();
	effect->flags           = FIELD_PARTICLE_LIFTED_BY_Z;
	fdParticleSetAnimation(effect, 1);
}

// Frees a field effect sprite and its sprite, clearing it to zero (inactive).
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect)
{
	if (effect->sprite)
		fdSpriteDestroyStub(effect->sprite);
	DMA_FILL32(0, effect, sizeof *effect);
}
