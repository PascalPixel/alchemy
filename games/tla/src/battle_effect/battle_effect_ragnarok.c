// Ragnarok, and the great sword that falls onto the target in Titan Blade and Acheron's Grief. Part
// of battle_effect.c.

// The falling sword's modes (btEffectRunFallingSword).
typedef enum FALLING_SWORD_MODE
{
	FALLING_SWORD_TITAN_BLADE,
	FALLING_SWORD_ACHERONS_GRIEF
} FALLING_SWORD_MODE;

// A flash waits out its life, the ring's inner ones least, and in its last FLASH_BURN_TICKS burns
// down from FlashBurstSheet's fireball to embers, a picture every three ticks.
#define FLASH_LIFE              25
#define FLASH_BURN_TICKS        18
// The canvas line the sparks bounce on, and where the falling sword, its pillar of fire and its
// dust stop.
#define SWORD_GROUND_Y          108

// FlameBladeSheet's blade, then FlashBurstSheet's pictures.
#define RAGNAROK_BLADE_WIDTH    64
#define RAGNAROK_BLADE_HEIGHT   128
#define RAGNAROK_FLASH_PICTURES PICTURE_BYTES(RAGNAROK_BLADE)
// The blade comes down to RAGNAROK_BLADE_HOLD_TOP, and is not drawn below RAGNAROK_BLADE_FLOOR.
#define RAGNAROK_BLADE_HOLD_TOP 16
#define RAGNAROK_BLADE_FLOOR    104

#define RAGNAROK_COLUMNS        2   // columns that strike
#define RAGNAROK_SEEDED         3   // columns given particles; the third is unused
#define RAGNAROK_FLASHES        16  // flashes kept per column
#define RAGNAROK_RING           12  // of which drawn as the ring
#define RAGNAROK_SPARKS         340 // sparks kept per column
#define RAGNAROK_DRAWN_SPARKS   256 // of which flown

#define RAGNAROK_TICKS          80
#define BLADE_SWING_TICKS       10  // the blade comes down
#define BLADE_HOLD_TICK         20  // then holds, then creeps on
#define BLADE_LAST_TICK         39  // the last tick the blade shows
// each column strikes 8 ticks after the last
#define FIRST_STRIKE_TICK       16
#define RESULT_TICK             40 // the damage shows

// A huge flaming blade swings down over the targets and light bursts over them twice.
//
// Abilities: Ragnarok.
void btEffectRunRagnarok(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               column;
	Uint8               *cells;
	MOTION_OBJECT       *user;
	DRAW_RECTANGLE       draw[2];
	SCREEN_POINT         target;
	Sint32               baseX;
	Sint32               frame;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	cells  = RamHeapSlots->battleSheet;

	// Open the canvas, pose the user and place the columns.
	user   = btObjectGetStub(args->user)->object;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(args->side, draw);
	fdActorSelectAnimationStub(user, POSE_LEAP);
	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL * 3);
	btEffectPosProjectBattler(args->targets[0], &target);
	if (args->side == EFFECT_SIDE_ENEMIES)
		baseX = target.x / 2 - 24;
	else
		baseX = target.x / 2 - 92;

	btResourceLoadAndDecompress(RESOURCE_FLAME_BLADE_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_FLASH_BURST_SHEET, state->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, cells, 0, 0);

	// Each column's flashes (a ring, 2 pixels apart, living 25 to 32 ticks) and sparks (from y =
	// 88, at random speeds and angles, living 32 to 39).
	for (column = 0; column != RAGNAROK_SEEDED; column++)
	{
		for (i = 0; i != RAGNAROK_FLASHES; i++)
		{
			PARTICLE *flash  = &state->particles[column * RAGNAROK_FLASHES + i];
			Sint32    radius = i * 2;
			Sint32    angle  = syRandom() & TRIG_ANGLE_MASK;

			flash->x         = sySin(angle) * radius;
			flash->y         = -(syCos(angle) * radius);
			flash->variant   = i / 2 + FLASH_LIFE;
		}

		for (i = 0; i != RAGNAROK_SPARKS; i++)
		{
			PARTICLE *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];
			Sint32    speed;
			Sint32    angle;

			speed            = (syRandom() & 0x1ff) + 32;
			angle            = syRandom() & TRIG_ANGLE_MASK;
			spark->x         = toFIXED(RagnarokStrikeColumns[column + args->side * 3] + baseX);
			spark->y         = toFIXED(88);
			spark->velocityX = (sySin(angle) * speed) >> 6;
			spark->velocityY = -(syCos(angle) * speed * 2) >> 6;
			spark->variant   = (syRandom() & 7) + 32;
		}
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for (frame = 0; frame != RAGNAROK_TICKS; frame++)
	{
		if (frame == 4)
			sySoundPlayStub(SE_POWER_UP);
		if (frame == 8)
			state->shakeFrame = frame;
		if (frame == 18)
			sySoundPlayStub(SE_CRASH);
		if (frame == RESULT_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		// The blade: down 16 pixels a tick from above the canvas, held, then creeping on.
		if (frame <= BLADE_LAST_TICK)
		{
			Sint32 height;
			Sint32 x;
			Sint32 top;

			height = RAGNAROK_BLADE_HEIGHT;
			if (args->side == EFFECT_SIDE_PARTY)
			{
				if (frame < BLADE_SWING_TICKS)
				{
					x   = frame * 10 - 8;
					top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
				}
				else if (frame > BLADE_HOLD_TICK)
				{
					x   = frame + 62;
					top = frame * 2 - 24;
				}
				else
				{
					x   = 82;
					top = RAGNAROK_BLADE_HOLD_TOP;
				}
			}
			else
			{
				if (frame < BLADE_SWING_TICKS)
				{
					x   = 128 - frame * 10;
					top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
				}
				else if (frame > BLADE_HOLD_TICK)
				{
					x   = 58 - frame;
					top = frame * 2 - 24;
				}
				else
				{
					x   = 38;
					top = RAGNAROK_BLADE_HOLD_TOP;
				}
			}
			if (top + RAGNAROK_BLADE_HEIGHT > RAGNAROK_BLADE_FLOOR)
				height = height - top - (RAGNAROK_BLADE_HEIGHT - RAGNAROK_BLADE_FLOOR);
			if (height > 0)
				draw[0](canvas,
				        state->sheet,
				        x + baseX - RAGNAROK_BLADE_WIDTH / 2,
				        top,
				        RAGNAROK_BLADE_WIDTH,
				        height);
		}

		if (frame > FIRST_STRIKE_TICK)
			btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);

		for (column = 0; column != RAGNAROK_COLUMNS; column++)
		{
			Sint32 start = column * 8 + FIRST_STRIKE_TICK;

			// The strike: a shake, two ticks of the big flash, then the ring.
			if (frame == start)
				state->shakeFrame = 12;
			if (frame >= start)
			{
				if (frame < start + 2)
					draw[0](canvas,
					        state->sheet + RAGNAROK_FLASH_PICTURES,
					        RagnarokStrikeColumns[column + args->side * 3] + baseX
					            - FLASH_BURST_WIDTH / 2,
					        56,
					        FLASH_BURST_WIDTH,
					        FLASH_BURST_HEIGHT);
				for (i = 0; i != RAGNAROK_RING; i++)
				{
					PARTICLE *flash = &state->particles[column * RAGNAROK_FLASHES + i];
					Sint32    x;
					Sint32    life;
					Sint32    y;

					y = FIXED_INTEGER_PART(flash->y);
					x = FIXED_INTEGER_PART(flash->x)
					  + RagnarokStrikeColumns[column + args->side * 3] + baseX;
					life = flash->variant;
					if (life >= 0 && life < FLASH_BURN_TICKS)
						draw[0](canvas,
						        state->sheet
						            + RagnarokFlashPictures[life / 3] * PICTURE_BYTES(FLASH_BURST)
						            + RAGNAROK_FLASH_PICTURES,
						        x - FLASH_BURST_WIDTH / 2,
						        y + 56,
						        FLASH_BURST_WIDTH,
						        FLASH_BURST_HEIGHT);
					life           = flash->variant;
					flash->variant = life > 0 ? life - 1 : -1;
				}
			}

			// The sparks, from six ticks after the strike, bouncing on the ground.
			if (frame > start + 5)
			{
				for (i = 0; i != RAGNAROK_DRAWN_SPARKS; i++)
				{
					PARTICLE *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];

					if (spark->variant > 0)
					{
						Sint32 x;
						Sint32 y;
						Sint32 size;

						btParticleAdvanceWithGravity2D(
							spark, PARTICLE_DAMPING_NONE, toFIXED(1) / 16);
						spark->variant--;
						y = spark->y;
						if (y > toFIXED(SWORD_GROUND_Y))
						{
							spark->velocityY = -spark->velocityY / 2;
						}
						else
						{
							x = spark->x;
							if ((x >= 0 && x <= toFIXED(127) - 1) && y >= 0)
							{
								Uint8 *picture;
								Sint32 lane;
								Sint32 py  = FIXEDtoINT(y);
								Sint32 px  = FIXEDtoINT(x);

								size       = spark->variant / 5 + 1;
								lane       = i & 1;
								picture    = cells + BattleEffectMotePictures[size - 1];
								px        -= size / 2;
								py        -= size;
								draw[lane](canvas, picture, px, py, size, size * 2);
							}
						}
					}
				}
			}

			// The column's targets flash at the strike and are struck by its sparks.
			for (i = 0; i != args->targetCount; i++)
			{
				if (frame == start)
					btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 10);
				if (frame == start + 6)
				{
					EFFECT_STRIKE_TARGET(args, i, 10, KNOCKBACK_HURL);
				}
			}
		}

		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL);
	EFFECT_FINISH_CANVAS();
}

#undef RAGNAROK_BLADE_WIDTH
#undef RAGNAROK_BLADE_HEIGHT
#undef RAGNAROK_FLASH_PICTURES
#undef RAGNAROK_BLADE_HOLD_TOP
#undef RAGNAROK_BLADE_FLOOR
#undef RAGNAROK_COLUMNS
#undef RAGNAROK_SEEDED
#undef RAGNAROK_FLASHES
#undef RAGNAROK_RING
#undef RAGNAROK_SPARKS
#undef RAGNAROK_DRAWN_SPARKS
#undef RAGNAROK_TICKS
#undef BLADE_SWING_TICKS
#undef BLADE_HOLD_TICK
#undef BLADE_LAST_TICK
#undef FIRST_STRIKE_TICK
#undef RESULT_TICK

// Abilities: Titan Blade.
void btEffectRunTitanBlade(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFallingSword(args, FALLING_SWORD_TITAN_BLADE);
}

// Abilities: Acheron's Grief.
void btEffectRunAcheronsGrief(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFallingSword(args, FALLING_SWORD_ACHERONS_GRIEF);
}

// The sword's sparks, in the map cell buffer after the dust (PARTICLES).
#define TITAN_BLADE_SWORD_SPARKS                                                                   \
	((PARTICLE *)(RamMapMetatileBuffer + TITAN_BLADE_DUST * sizeof(PARTICLE)))

#define TITAN_BLADE_TICKS          102
#define TITAN_BLADE_SWORD_PICTURE  20000 // the sword picture in the sprite sheet
#define TITAN_BLADE_FLASHES        32    // flashes scattered
#define TITAN_BLADE_RING           16    // of which drawn as the ring
#define TITAN_BLADE_DUST           684   // dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST     256   // dust lit at once on tick 28
#define TITAN_BLADE_DRAWN_DUST     342   // dust moved and drawn
#define TITAN_BLADE_SPARKS         340   // sparks after the dust
// The flash pictures in the sprite sheet, after the 34x104 fire pillar.
#define TITAN_BLADE_FLASH_PICTURES PICTURE_BYTES(FIRE_PILLAR)
// SwordSheet's great sword, point down.
#define TITAN_BLADE_SWORD_WIDTH    36
#define TITAN_BLADE_SWORD_HEIGHT   120
// The sword comes down to TITAN_BLADE_SWORD_HOLD_TOP and holds there.
#define TITAN_BLADE_SWORD_HOLD_TOP 16

// From TITAN_BLADE_PILLAR_TICK, for TITAN_BLADE_PILLAR_TICKS, the pillar of fire rises and
// TITAN_BLADE_DUST_PER_TICK more dust spouts each tick.
#define TITAN_BLADE_PILLAR_TICK    32
#define TITAN_BLADE_PILLAR_TICKS   32
#define TITAN_BLADE_DUST_PER_TICK  16
// How far behind their climb the pillar's flames start, which sets the line of FirePillarSheetA's
// column they show first.
#define TITAN_BLADE_PILLAR_PHASE   256

// The sword strikes: the crash, the damage and the canvas filled bright.
#define TITAN_BLADE_STRIKE_TICK    60

// A great sword falls from the sky onto the target, a pillar of fire and dust spouting under it,
// and drives into the ground in a burst of flashes and sparks. The sword comes down fast, holds,
// sinks slowly while the pillar rises and then plunges; the canvas floods white as it arrives and
// as it strikes, and every target is struck a little after. Acheron's Grief shows the same sword in
// other colours.
void btEffectRunFallingSword(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	Sint32               frame;
	DRAW_RECTANGLE       draw[2];
	Uint8               *cells;
	SCREEN_POINT         target;
	Sint32               centerX;
	Sint32               step;
	FIXED                origin;
	Sint32               i;

	state  = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	cells  = RamHeapSlots->battleSheet;

	// Open the canvas on BG1 over the target and build the blitters.
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectPosProjectBattler(args->targets[0], &target);
	centerX = target.x / 2;

	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	draw[1] = RamHeapSlots->ramCodePaired;

	// No caller passes a third mode: both abilities draw the sword.
	if (mode != 2)
		btResourceLoadAndDecompress(
			RESOURCE_SWORD_SHEET, state->sheet + TITAN_BLADE_SWORD_PICTURE, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FLASH_BURST_SHEET, state->sheet + TITAN_BLADE_FLASH_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, cells, 0, 0);

	// Acheron's Grief turns the canvas's colours: red to blue, blue to green, green to red.
	if (mode == FALLING_SWORD_ACHERONS_GRIEF)
	{
		Uint16 *color = (Uint16 *)BG_PLTT;

		for (i = 0; i != CANVAS_LEVEL_BRIGHTEST + 1; i++)
		{
			Sint32 red   = RGB_RED(*color);
			Sint32 green = RGB_GREEN(*color);
			Sint32 blue  = RGB_BLUE(*color);

			*color++     = RGB(green, blue, red);
		}
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	// 32 flashes on rings, two pixels apart.
	for (i = 0; i != TITAN_BLADE_FLASHES; i++)
	{
		PARTICLE *flash = &state->particles[i];
		Sint32    scale = i * 2;
		Sint32    angle = syRandom() & TRIG_ANGLE_MASK;

		flash->x        = sySin(angle) * scale;
		flash->y        = -(syCos(angle) * scale);
		flash->variant  = i / 2 + FLASH_LIFE;
	}

	for (i = 0; i != TITAN_BLADE_DUST; i++)
	{
		PARTICLES[i].variant = -1;
	}

	// The sparks wait at the target's feet.
	i      = 0;
	origin = toFIXED(centerX);
	for (; i != TITAN_BLADE_SPARKS; i++)
	{
		PARTICLE *spark = &TITAN_BLADE_SWORD_SPARKS[i];
		Sint32    speed;
		Sint32    angle;

		speed            = (syRandom() & 0x1ff) + 32;
		angle            = syRandom() & TRIG_ANGLE_MASK;
		spark->x         = origin;
		spark->y         = toFIXED(88);
		spark->velocityX = (sySin(angle) * speed) >> 5;
		spark->velocityY = -(syCos(angle) * speed) >> 6;
		spark->variant   = (syRandom() & 7) + 32;
	}

	for (frame = 0; frame != TITAN_BLADE_TICKS; frame++)
	{
		if (mode == FALLING_SWORD_TITAN_BLADE)
		{
			if (frame >= 41 && frame <= 63)
				btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);
		}
		else if (mode == FALLING_SWORD_ACHERONS_GRIEF)
		{
			if (frame == 61)
			{
				syWordsCopyEntry((void *)BG_PLTT,
				                 syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
				                 SHEET_PALETTE_BYTES);
				btPaletteMirrorBgColors();
			}
		}

		if (frame == 8)
			state->shakeFrame = frame;
		if (frame == 48)
			state->shakeFrame = 8;
		if (frame == TITAN_BLADE_STRIKE_TICK)
			state->shakeFrame = 16;

		if (frame == 4)
			sySoundPlayStub(SE_POWER_UP);
		if (frame == TITAN_BLADE_PILLAR_TICK)
			sySoundPlayStub(SE_SCATTER);
		if (frame == TITAN_BLADE_STRIKE_TICK)
		{
			if (mode == FALLING_SWORD_ACHERONS_GRIEF)
				EFFECT_SHOW_DAMAGE(SE_CLATTER);
			else
				EFFECT_SHOW_DAMAGE(SE_CRASH);
		}

		// The flashes, from tick 56.
		if (frame > 55)
		{
			for (i = 0; i != TITAN_BLADE_RING; i++)
			{
				PARTICLE *flash = &state->particles[i];
				Sint32    x;
				Sint32    life;
				Sint32    y;

				y    = FIXED_INTEGER_PART(flash->y);
				x    = FIXED_INTEGER_PART(flash->x) + centerX;
				life = flash->variant;
				if (life >= 0 && life < FLASH_BURN_TICKS)
					draw[0](canvas,
					        state->sheet
					            + TitanBladeFlashPictures[life / 3] * PICTURE_BYTES(FLASH_BURST)
					            + TITAN_BLADE_FLASH_PICTURES,
					        x - FLASH_BURST_WIDTH / 2,
					        y + 48,
					        FLASH_BURST_WIDTH,
					        FLASH_BURST_HEIGHT);
				life           = flash->variant;
				flash->variant = life > 0 ? life - 1 : -1;
			}
		}

		// The dust: a fountain at tick 28, then more each tick while the pillar rises.
		if (frame == 28)
		{
			for (i = 0; i != TITAN_BLADE_FIRST_DUST; i++)
			{
				PARTICLE *dust = &PARTICLES[i];

				if (dust->variant == -1)
				{
					Sint32 speed;
					Sint32 angle;

					speed           = syRandom() & 63;
					angle           = syRandom() & TRIG_ANGLE_MASK;
					dust->x         = ((sySin(angle) * speed) >> 3) + origin;
					dust->y         = ((syCos(angle) * speed) >> 2) + toFIXED(96);
					dust->velocityX = ((syRandom() & 63) - 32) << 14;
					dust->velocityY = (-(syRandom() & 63) - 8) << 13;
					dust->variant   = 0;
				}
			}
		}

		step = frame - TITAN_BLADE_PILLAR_TICK;
		if (step >= 0 && step < TITAN_BLADE_PILLAR_TICKS)
		{
			Sint32 spawned;

			spawned = 0;
			for (i = 0; i != TITAN_BLADE_DUST; i++)
			{
				PARTICLE *dust = &PARTICLES[i];

				if (dust->variant == -1)
				{
					Sint32 speed;
					Sint32 angle;

					speed           = syRandom() & 63;
					angle           = syRandom() & TRIG_ANGLE_MASK;
					dust->x         = ((sySin(angle) * speed) >> 3) + origin;
					dust->y         = ((syCos(angle) * speed) >> 2) + toFIXED(96);
					dust->velocityX = ((syRandom() & 63) - 32) << 14;
					dust->velocityY = (-(syRandom() & 63) - 8) << 13;
					dust->variant   = 0;
					spawned++;
					if (spawned == TITAN_BLADE_DUST_PER_TICK)
						break;
				}
			}
		}

		// The pillar of fire, its flames climbing 16 pixels a tick.
		if (step >= 0 && step < TITAN_BLADE_PILLAR_TICKS)
		{
			Sint32         rise   = frame * 16;
			Sint32         height = FIRE_PILLAR_HEIGHT;
			Uint8         *src    = state->sheet;
			Sint32         x      = centerX - FIRE_PILLAR_WIDTH / 2;
			DRAW_RECTANGLE blit   = draw[0];
			Sint32         scroll = (rise - TITAN_BLADE_PILLAR_PHASE) % height;

			blit(canvas,
			     src,
			     x,
			     SWORD_GROUND_Y - FIRE_PILLAR_HEIGHT - scroll,
			     FIRE_PILLAR_WIDTH,
			     height);
			blit(canvas, src, x, SWORD_GROUND_Y - scroll, FIRE_PILLAR_WIDTH, scroll);
		}

		// The dust grows as it falls and toward the end. It stops at tick 71, so only the first two
		// of the later sizes, and never the lift from tick 80, are reached.
		if (frame <= 71)
		{
			for (i = 0; i != TITAN_BLADE_DRAWN_DUST; i++)
			{
				PARTICLE *dust = &PARTICLES[i];

				if (dust->variant >= 0)
				{
					Sint32 size;

					size = i % 3 + 2;
					if (dust->velocityY > 0)
						size += 2;
					if (frame > 68 && size <= 5)
						size = 6;
					if (frame > 70 && size <= 6)
						size = 7;
					if (frame > 72 && size <= 7)
						size = 8;
					if (frame > 74 && size <= 8)
						size = 9;
					if (frame > 76)
						size = MOTE_PICTURES;
					draw[0](canvas,
					        cells + BattleEffectMotePictures[size - 1],
					        FIXED_INTEGER_PART(dust->x) - size / 2,
					        FIXED_INTEGER_PART(dust->y) - size,
					        size,
					        size * 2);

					dust->x += dust->velocityX;
					dust->y += dust->velocityY;
					if (frame > 80)
						dust->velocityY -= toFIXED(1) / 2;
					else
						dust->velocityY += TitanBladeDustGravity[i & 3];
					dust->velocityX = dust->velocityX * 62 / DAMPING_SCALE;
					dust->velocityY = dust->velocityY * 62 / DAMPING_SCALE;
					dust->variant++;
					if (dust->velocityY > 0 && FIXED_INTEGER_PART(dust->y) > SWORD_GROUND_Y)
						dust->variant = -1;
				}
			}
		}

		// The sword: down 16 pixels a tick, held, sinking while the pillar rises, plunging after
		// the strike; it is not drawn below the ground.
		if (frame <= 95)
		{
			Sint32 x;
			Sint32 high;
			Sint32 top;

			x    = centerX - TITAN_BLADE_SWORD_WIDTH / 2;
			high = TITAN_BLADE_SWORD_HEIGHT;
			if (frame > TITAN_BLADE_STRIKE_TICK)
				top = frame * 8 - 450;
			else if (frame > TITAN_BLADE_PILLAR_TICK)
				top = step / 2 + TITAN_BLADE_SWORD_HOLD_TOP;
			else if (frame <= 9)
				top = frame * 16 - 128;
			else
				top = TITAN_BLADE_SWORD_HOLD_TOP;
			if (top + TITAN_BLADE_SWORD_HEIGHT > SWORD_GROUND_Y)
				high = high - top - (TITAN_BLADE_SWORD_HEIGHT - SWORD_GROUND_Y);
			if (high > 0)
				draw[1](canvas,
				        state->sheet + TITAN_BLADE_SWORD_PICTURE,
				        x,
				        top,
				        TITAN_BLADE_SWORD_WIDTH,
				        high);
		}

		// The sparks bounce on the ground.
		if (frame > 59)
		{
			for (i = 0; i != TITAN_BLADE_SPARKS; i++)
			{
				PARTICLE *spark = &TITAN_BLADE_SWORD_SPARKS[i];

				if (spark->variant > 0)
				{
					Sint32 x;
					Sint32 y;
					Sint32 size;

					btParticleAdvanceWithGravity2D(spark, PARTICLE_DAMPING_NONE, toFIXED(1) / 8);
					y = spark->y;
					spark->variant--;
					if (y > toFIXED(SWORD_GROUND_Y))
					{
						spark->velocityY = -spark->velocityY / 2;
					}
					else
					{
						x = spark->x;
						if ((x >= 0 && x <= toFIXED(127) - 1) && y >= 0)
						{
							Uint8 *cell;

							size  = spark->variant / 5 + 1;
							cell  = cells + BattleEffectMotePictures[size - 1];
							x     = FIXEDtoINT(x);
							y     = FIXEDtoINT(y);
							x    -= size / 2;
							y    -= size;
							draw[0](canvas, cell, x, y, size, size * 2);
						}
					}
				}
			}
		}

		// The hit, and the canvas flooded as the sword arrives and as it strikes.
		if (frame == 68)
		{
			for (i = 0; i != args->targetCount; i++)
			{
				EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_JOLT);
			}
		}

		if (frame == 9)
		{
			btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if (frame == TITAN_BLADE_STRIKE_TICK)
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));

		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef TITAN_BLADE_SWORD_SPARKS
#undef TITAN_BLADE_TICKS
#undef TITAN_BLADE_SWORD_PICTURE
#undef TITAN_BLADE_FLASHES
#undef TITAN_BLADE_RING
#undef TITAN_BLADE_DUST
#undef TITAN_BLADE_FIRST_DUST
#undef TITAN_BLADE_DRAWN_DUST
#undef TITAN_BLADE_SPARKS
#undef TITAN_BLADE_FLASH_PICTURES
#undef TITAN_BLADE_SWORD_WIDTH
#undef TITAN_BLADE_SWORD_HEIGHT
#undef TITAN_BLADE_SWORD_HOLD_TOP
#undef TITAN_BLADE_PILLAR_TICK
#undef TITAN_BLADE_PILLAR_TICKS
#undef TITAN_BLADE_DUST_PER_TICK
#undef TITAN_BLADE_PILLAR_PHASE
#undef TITAN_BLADE_STRIKE_TICK
#undef FLASH_LIFE
#undef FLASH_BURN_TICKS
#undef SWORD_GROUND_Y
