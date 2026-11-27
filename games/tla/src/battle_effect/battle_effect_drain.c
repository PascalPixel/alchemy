// Break and Drain. Part of battle_effect.c.

// Break and Drain take the targets in turn, this many ticks apart.
#define DRAIN_TARGET_TICKS      20

#define BREAK_SHARDS_PER_TARGET 128
// Break runs DRAIN_TARGET_TICKS a target and then this many while the last shards fall.
#define BREAK_FALL_TICKS        56
#define BREAK_HIT_TICK          32
// The canvas row below which a falling shard goes out.
#define BREAK_SHARD_GROUND      112

// What btEffectRunDrainMotes draws, each named for the ability that plays it.
typedef enum DRAIN_MOTES_MODE
{
	DRAIN_MOTES_DRAIN,       // white and pale blue motes with long trails
	DRAIN_MOTES_PSY_DRAIN,   // golden motes with long trails
	DRAIN_MOTES_TRUE_COLLIDE // large violet orbs, dark at the heart
} DRAIN_MOTES_MODE;

// Ice shards burst up from each target in turn, tinting it, and fall away.
//
// Abilities: Break.
void btEffectRunBreak(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	DRAW_RECTANGLE       draw;
	Sint32               member;
	Uint8               *sheet;
	BATTLE_CAMERA       *camera;
	Sint32               point[3];
	SCREEN_POINT         screen;
	Sint32               frame;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet  = RamHeapSlots->battleSheet;
	camera = RamHeapSlots->cameraState;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	syWordsCopyEntry(
		(void *)BG_PLTT, syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET), SHEET_PALETTE_BYTES);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw = RamHeapSlots->ramCode;

	EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * BREAK_SHARDS_PER_TARGET);

	EFFECT_VIEW_FROM_CAMERA(camera);

	// Scatter the shards around the middle of each body.
	for (member = 0; member != args->targetCount; member++)
	{
		MOTION_OBJECT *object;
		Sint32         height;

		object   = btObjectGetStub(args->targets[member])->object;
		height   = btUnitGetEffectHeightStub(args->targets[member]) / 2;
		point[0] = object->x;
		point[1] = height;
		point[2] = object->z;
		btEffectPosProjectPoint(point, &screen);
		screen.x >>= 1;
		for (i = 0; i != BREAK_SHARDS_PER_TARGET; i++)
		{
			PARTICLE *shard = &PARTICLES[member * BREAK_SHARDS_PER_TARGET + i];
			Sint32    radius;
			Sint32    angle;
			Sint32    lift;

			radius           = syRandom() & 0xff;
			angle            = syRandom() & TRIG_ANGLE_MASK;
			shard->x         = ((sySin(angle) * radius) >> 7) + toFIXED(screen.x);
			shard->y         = ((syCos(angle) * radius) >> 3) + toFIXED(screen.y);
			// Thrown up at 2 to 6 pixels a tick, and up to a pixel a tick aside.
			shard->velocityX = (128 - (syRandom() & 0xff)) << 9;
			lift             = syRandom() & 0xff;
			shard->variant   = 0;
			shard->velocityY = (-lift - 128) << 10;
		}
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for (frame = 0; frame != args->targetCount * DRAIN_TARGET_TICKS + BREAK_FALL_TICKS; frame++)
	{
		if (frame == BREAK_HIT_TICK)
			EFFECT_SHOW_DAMAGE(0);
		for (member = 0; member != args->targetCount; member++)
		{
			if (frame == member * DRAIN_TARGET_TICKS)
			{
				sySoundPlayStub(SE_CRACK);
				btEffectTintBattler(
					args->targets[member], SPRITE_COLOR_WHITE, TINT_KEEP, member, 20);
			}
			// A shard falls and goes out below the ground.
			if (frame > member * DRAIN_TARGET_TICKS)
			{
				for (i = 0; i != BREAK_SHARDS_PER_TARGET; i++)
				{
					PARTICLE *shard = &PARTICLES[member * BREAK_SHARDS_PER_TARGET + i];

					if (shard->variant >= 0)
					{
						Sint32 size;

						size = i % 3 + 1;
						EFFECT_DRAW_MOTE(draw,
						                 canvas,
						                 sheet,
						                 FIXED_INTEGER_PART(shard->x),
						                 FIXED_INTEGER_PART(shard->y),
						                 size);
						btParticleAdvanceWithGravity2D(shard, 62, DrainGravities[i & 3]);
						shard->variant++;
						if (shard->velocityY > 0
						    && FIXED_INTEGER_PART(shard->y) > BREAK_SHARD_GROUND)
							shard->variant = -1;
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Motes of light burst from each target in turn, and are pulled into the centre of the user's body:
// white for the basic variant, golden for the stronger.
//
// Abilities: Drain, Psy Drain.
void btEffectRunDrain(BATTLE_EFFECT_PARM *args)
{
	Sint32 variant = args->variant;

	// Psy Drain's for any variant past the basic one.
	btEffectRunDrainMotes(args, variant != 0);
}

// Large violet orbs burst from each target in turn, and are pulled into the centre of the user's
// body.
//
// Abilities: True Collide.
void btEffectRunTrueCollide(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDrainMotes(args, DRAIN_MOTES_TRUE_COLLIDE);
}

#define DRAIN_MOTES_PER_TARGET 128
#define DRAIN_DRAWN            32 // motes drawn for each target
// Drain runs DRAIN_TARGET_TICKS a target and then this many; the user glows from DRAIN_GLOW_TICK
// until DRAIN_GLOW_END_TICKS before the end.
#define DRAIN_END_TICKS        72
#define DRAIN_GLOW_TICK        40
#define DRAIN_GLOW_END_TICKS   20
#define DRAIN_HIT_TICK         64
// A mote homes in on the user this many ticks after its target's turn, each mote a tick after the
// one before, gaining a 256th of the distance a tick.
#define DRAIN_PULL_DELAY       10
#define DRAIN_PULL(distance)   ((distance) >> 8)
// A mote goes out within this many pixels of the middle of the user, across and in depth.
#define DRAIN_ARRIVAL_DISTANCE 16
// The red and blue of True Collide's first colour, a dim violet.
#define TRUE_COLLIDE_VIOLET    21

// Motes burst out of each target in turn and are pulled into the centre of the user's body, which
// glows white as they arrive (DRAIN_MOTES_MODE).
void btEffectRunDrainMotes(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	DRAW_RECTANGLE       draw;
	Sint32               member;
	Uint8               *sheet;
	BATTLE_CAMERA       *camera;
	SCREEN_POINT         screen;
	Sint32               palette;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet  = RamHeapSlots->battleSheet;
	camera = RamHeapSlots->cameraState;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	if (mode == DRAIN_MOTES_TRUE_COLLIDE)
		btEffectDrawShadedCirclesMirrored(sheet, 10, toFIXED(1));
	else
		btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	if (mode == DRAIN_MOTES_DRAIN)
		palette = RESOURCE_ICE_BLOCK_SHEET;
	else if (mode == DRAIN_MOTES_PSY_DRAIN)
		palette = RESOURCE_BLAST_SHEET;
	else
		palette = RESOURCE_VIOLET_PALETTE_C;
	syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(palette), SHEET_PALETTE_BYTES);

	// The motes fly out of each target's body.
	EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_MOTES_PER_TARGET);

	for (member = 0; member != args->targetCount; member++)
	{
		MOTION_OBJECT *object;
		Sint32         halfHeight;

		object     = btObjectGetStub(args->targets[member])->object;
		halfHeight = btUnitGetEffectHeightStub(args->targets[member]) / 2;
		for (i = 0; i != DRAIN_MOTES_PER_TARGET; i++)
		{
			PARTICLE *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];
			Sint32    speed;

			SET_POSITION(mote, object->x, object->y + halfHeight, object->z);
			mote->velocityX = ((syRandom() & 0xff) - 128) << 10;
			mote->velocityY = ((syRandom() & 0xff) - 128) << 10;
			mote->velocityZ = (speed = (syRandom() & 0xff) - 128) << 10;
			// True Collide's fly out twice as fast.
			if (mode == DRAIN_MOTES_TRUE_COLLIDE)
			{
				mote->velocityX <<= 1;
				mote->velocityY <<= 1;
				mote->velocityZ   = speed * (toFIXED(1) / 32);
			}
			mote->variant = 0;
		}
	}

	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw = RamHeapSlots->ramCode;
	if (mode == DRAIN_MOTES_TRUE_COLLIDE)
	{
		// Colours 1 to 63 run from violet down to black, so an orb is dark where its level is
		// highest; the canvas is blended 16:6 and trails.
		Uint16 *color = &BG_PLTT_COLOR(0, 1);

		for (i = 1; i != SHEET_PALETTE_COLORS; i++)
		{
			Sint32 red = TRUE_COLLIDE_VIOLET - (i + 1) / 3;

			*color++   = RGB(red, 0, TRUE_COLLIDE_VIOLET - i / 3);
		}
		REG_BLDALPHA = BLEND_ALPHA(16, 6);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	}
	else
	{
		// Each transfer lowers every level by 4, leaving a trail.
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	sySoundPlayStub(SE_SWIRL);

	for (frame = 0; frame != args->targetCount * DRAIN_TARGET_TICKS + DRAIN_END_TICKS; frame++)
	{
		MOTION_OBJECT *user;
		Sint32         halfHeight;

		user       = btObjectGetStub(args->user)->object;
		halfHeight = btUnitGetEffectHeightStub(args->user) / 2;
		if (frame == DRAIN_HIT_TICK)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		EFFECT_VIEW_FROM_CAMERA(camera);
		if (frame == DRAIN_GLOW_TICK)
			btEffectTintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
		if (frame
		    == args->targetCount * DRAIN_TARGET_TICKS + DRAIN_END_TICKS - DRAIN_GLOW_END_TICKS)
			btEffectTintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
		for (member = 0; member != args->targetCount; member++)
		{
			if (frame == member * DRAIN_TARGET_TICKS)
				btEffectTintBattler(
					args->targets[member], SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 42);
			if (frame > member * DRAIN_TARGET_TICKS)
			{
				for (i = 0; i != DRAIN_DRAWN; i++)
				{
					PARTICLE *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

					if (mote->variant >= 0)
					{
						Uint8 size = DrainMotesSizes[mode];

						btEffectPosProjectPoint((Sint32 *)mote, &screen);
						screen.x >>= 1;
						if (mode == DRAIN_MOTES_TRUE_COLLIDE)
						{
							draw(canvas,
							     sheet,
							     screen.x - size / 2,
							     screen.y - size,
							     size,
							     size * 2);
							btParticleAdvanceWithGravity3D(mote, 61, 0);
						}
						else
						{
							EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
							btParticleAdvanceWithGravity3D(mote, 62, 0);
						}

						// After a moment a mote is pulled into the user.
						if (frame > member * DRAIN_TARGET_TICKS + i + DRAIN_PULL_DELAY)
						{
							Sint32 dx;
							Sint32 dy;
							Sint32 dz;

							dx               = DRAIN_PULL(user->x - mote->x);
							dy               = DRAIN_PULL(user->y + halfHeight - mote->y);
							dz               = DRAIN_PULL(user->z - mote->z);
							mote->velocityX += dx;
							mote->velocityY += dy;
							mote->velocityZ += dz;
							if ((dx > -DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE))
							     && dx < DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE)))
							    && (dz > -DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE))
							        && dz < DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE))))
								mote->variant = -1;
						}
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DRAIN_TARGET_TICKS
#undef BREAK_SHARDS_PER_TARGET
#undef BREAK_FALL_TICKS
#undef BREAK_HIT_TICK
#undef BREAK_SHARD_GROUND
#undef DRAIN_MOTES_PER_TARGET
#undef DRAIN_DRAWN
#undef DRAIN_END_TICKS
#undef DRAIN_GLOW_TICK
#undef DRAIN_GLOW_END_TICKS
#undef DRAIN_HIT_TICK
#undef DRAIN_PULL_DELAY
#undef DRAIN_PULL
#undef DRAIN_ARRIVAL_DISTANCE
#undef TRUE_COLLIDE_VIOLET
