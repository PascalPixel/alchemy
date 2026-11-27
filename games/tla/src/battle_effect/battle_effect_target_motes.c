/* The status motes of Taint, Delude, Paralyze, Heartrender, Mystic Flame,
   Demonfire and Drown, then Sleep's sheep, Curse's candles, an unused Mercury
   Djinni heal, and Balm, Tinder and Phoenix. Part of battle_effect.c. */

/* The target motes' styles (btEffectRunTargetMotes), each named for the effect
   that plays it. */
typedef enum TARGET_MOTES_MODE {
	TARGET_MOTES_TAINT, /* lime smoke closing in */
	/* pink smoke closing in; Taint's stronger variant */
	TARGET_MOTES_PINK_SMOKE,
	TARGET_MOTES_DELUDE,	   /* cyan smoke rising */
	TARGET_MOTES_PARALYZE,	   /* smoke rising, with glints */
	TARGET_MOTES_HEARTRENDER,  /* hearts closing in */
	TARGET_MOTES_MYSTIC_FLAME, /* glowing orbs closing in */
	TARGET_MOTES_DROWN,		   /* water drops closing in */
	/* orbs in Mars Djinn colours rising, then a strike */
	TARGET_MOTES_DEMON_FIRE
} TARGET_MOTES_MODE;

/* Swirling motes around each target, in eight styles of one runner: each mote
   plays its six pictures and starts again, a closing mote where it is and a
   rising one at a new point below. */

/* Lime smoke swirls in on each target, or pink smoke for a stronger variant.

   Abilities: Taint, Poison. */
void btEffectRunTaint(BATTLE_EFFECT_PARM *args)
{
	if(args->variant == EFFECT_VARIANT_BASIC) {
		btEffectRunTargetMotes(args, TARGET_MOTES_TAINT);
		return;
	}
	btEffectRunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/* Pink smoke swirls in on each target, as Taint shows it for a stronger
   variant.

   Abilities: Poison Gel. */
void btEffectRunPoisonGel(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/* Cyan smoke rises around each target.

   Abilities: Delude, Confuse, Mist, Mucous Gel. */
void btEffectRunDelude(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DELUDE);
}

/* Smoke rises around each target, and glints sparkle near it.

   Abilities: Paralyze. */
void btEffectRunParalyze(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_PARALYZE);
}

/* Hearts close in on each target, turning as they come.

   Abilities: Heartrender. */
void btEffectRunHeartrender(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_HEARTRENDER);
}

/* Glowing orbs spiral in on each target.

   Abilities: Mystic Flame. */
void btEffectRunMysticFlame(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_MYSTIC_FLAME);
}

/* Glowing orbs in Mars Djinn colours rise around each target, and then the user
   steps up to the first target and strikes it.

   Abilities: Demonfire. */
void btEffectRunDemonFire(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DEMON_FIRE);
}

/* Thirty-two water drops spiral in on each target.

   Abilities: Drown. */
void btEffectRunDrown(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DROWN);
}

/* Styles whose motes start in a wide ball, and run on TARGET_MOTES_WIDE_RUN_ON
   ticks after the last target's motes start. */
#define TARGET_MOTES_START_WIDE(mode) \
	((mode) <= TARGET_MOTES_PINK_SMOKE || \
		(mode) == TARGET_MOTES_HEARTRENDER || \
		(mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN || \
		(mode) == TARGET_MOTES_DEMON_FIRE)
/* Styles whose motes close in on the target; the others rise. */
#define TARGET_MOTES_CLOSE_IN(mode) \
	((mode) <= TARGET_MOTES_PINK_SMOKE || \
		(mode) == TARGET_MOTES_HEARTRENDER || \
		(mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN)
/* Every style but Taint's smokes and Paralyze ripples the canvas. */
#define TARGET_MOTES_RIPPLE(mode) \
	(!((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_PARALYZE))

/* Where Paralyze keeps its glint pictures in state->sheet. */
#define TARGET_MOTES_GLINT_PICTURES 9600

#define TARGET_MOTES				  512 /* motes scattered at the start */
#define TARGET_MOTES_SLOTS			  64 /* particles reserved for each target */
#define TARGET_MOTES_CYCLE			  24 /* ticks of a mote's image cycle */
/* The motes about each target: TARGET_MOTES_PER_TARGET, or Demonfire's orbs or
   Drown's drops. */
#define TARGET_MOTES_PER_TARGET		  16
#define DEMON_FIRE_ORBS				  24
#define DROWN_DROPS					  32
/* The ticks between one target's motes starting and the next's. */
#define TARGET_MOTES_STAGGER		  8
/* The ticks the effect runs on after the last target's motes start. */
#define TARGET_MOTES_RUN_ON			  32
#define TARGET_MOTES_WIDE_RUN_ON	  64
/* The ticks the effect runs on after the damage shows. Demonfire's user sets
   off DEMON_FIRE_LEAP_TICKS before then and reaches the first target over
   DEMON_FIRE_APPROACH_TICKS, striking it as the damage shows. */
#define TARGET_MOTES_DAMAGE_END_TICKS 32
#define DEMON_FIRE_LEAP_TICKS		  14
#define DEMON_FIRE_APPROACH_TICKS	  16
/* VortexSheet's water drop, after its three 16x64 swirls. */
#define DROWN_DROP_WIDTH			  12
#define DROWN_DROP_HEIGHT			  24
#define DROWN_DROP_PICTURE			  (3 * 16 * 64)

/* Around each target in turn, motes (smoke, hearts, orbs) close in on it or
   rise, and the target flashes. */
void btEffectRunTargetMotes(BATTLE_EFFECT_PARM *args, Uint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Sint32				 total;
	Sint32				 member;
	Sint32				 count;
	SCREEN_POINT		 screen;
	FIXED				 point[3];
	SCREEN_POINT		 base;
	DRAW_RECTANGLE		 draw[2];
	void				*palette;
	Sint32				*row;
	Sint32				 i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	count = TARGET_MOTES_PER_TARGET;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
	if(mode == TARGET_MOTES_TAINT) {
		palette = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	} else if(mode == TARGET_MOTES_PINK_SMOKE) {
		palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	} else if(mode == TARGET_MOTES_DELUDE) {
		palette = syResourceGetTableEntry(RESOURCE_CYAN_PALETTE);
	} else if(mode == TARGET_MOTES_PARALYZE) {
		palette = syResourceGetTableEntry(RESOURCE_BLAST_SHEET);
	} else if(mode == TARGET_MOTES_HEARTRENDER) {
		palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	} else if(mode == TARGET_MOTES_MYSTIC_FLAME) {
		btResourceLoadAndDecompress(
			RESOURCE_GLOW_ORB_SHEET, state->sheet, 1, 0);
		palette = syResourceGetTableEntry(RESOURCE_GLOW_ORB_SHEET);
	} else if(mode == TARGET_MOTES_DEMON_FIRE) {
		count = DEMON_FIRE_ORBS;
		btResourceLoadAndDecompress(
			RESOURCE_GLOW_ORB_SHEET, state->sheet, 1, 0);
		palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	} else {
		count = DROWN_DROPS;
		btResourceLoadAndDecompress(RESOURCE_VORTEX_SHEET, state->sheet, 1, 0);
		palette = syResourceGetTableEntry(RESOURCE_VORTEX_SHEET);
	}
	syWordsCopy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
	if(mode == TARGET_MOTES_HEARTRENDER)
		btResourceLoadAndDecompress(RESOURCE_HEART_SHEET, state->sheet, 1, 1);
	if(mode == TARGET_MOTES_PARALYZE)
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET,
			state->sheet + TARGET_MOTES_GLINT_PICTURES, 1, 0);

	for(i = 0; i != TARGET_MOTES; i++) {
		PARTICLE *mote = &PARTICLES[i];

		if(TARGET_MOTES_START_WIDE(mode)) {
			SET_POSITION(mote, ((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 14,
				((syRandom() & 0xff) - 127) << 15);
		} else {
			SET_POSITION(mote, ((syRandom() & 0xff) - 127) << 13,
				((syRandom() & 0xff) - 255) << 13,
				((syRandom() & 0xff) - 127) << 13);
		}
		mote->variant = 0;
	}

	if(TARGET_MOTES_START_WIDE(mode))
		total =
			args->targetCount * TARGET_MOTES_STAGGER + TARGET_MOTES_WIDE_RUN_ON;
	else
		total = args->targetCount * TARGET_MOTES_STAGGER + TARGET_MOTES_RUN_ON;
	if(TARGET_MOTES_RIPPLE(mode))
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySoundPlayStub(SE_SWIRL);

	for(frame = 0; frame != total; frame++) {
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

		/* Demonfire: the user steps up to the first target and strikes it. */
		if(mode == TARGET_MOTES_DEMON_FIRE) {
			if(frame ==
				total - TARGET_MOTES_DAMAGE_END_TICKS - DEMON_FIRE_LEAP_TICKS)
				btMotionApproachTargetStub(
					args->user, args->targets[0], DEMON_FIRE_APPROACH_TICKS, 0);
			if(frame == total - TARGET_MOTES_DAMAGE_END_TICKS) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_HURL);
				state->shakeFrame = 8;
			}
		} else if(frame == total - TARGET_MOTES_DAMAGE_END_TICKS) {
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}

		row = state->bg2xLines;
		for(i = 0; i != DISPLAY_HEIGHT; i++)
			*row++ = CANVAS_WAVE_BG2X(toFIXED(4) -
				sySin(frame * (TRIG_TURN / 16) + i * (TRIG_TURN / 32)) * 4);

		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *object =
				btObjectGetStub(args->targets[member])->object;

			/* Paralyze's glints sparkle near the target. */
			if(mode == TARGET_MOTES_PARALYZE &&
				frame > member * TARGET_MOTES_STAGGER &&
				frame < member * TARGET_MOTES_STAGGER + 32) {
				btEffectPosProjectScaledBattler(args->targets[member], &base);

				for(i = 0; i != 2; i++) {
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
					Sint32 picture = frame & 3;
					Sint32 radius = (syRandom() & 31) + 4;
					Sint32 x = base.x / 2 +
						(FIXEDtoINT(sySin(angle) * radius) >> 1) -
						(BattleEffectGlintPictureWidths[picture] >> 1);
					Sint32 y = base.y - FIXEDtoINT(syCos(angle) * radius) -
						(BattleEffectGlintPictureHeights[picture] >> 1);
					Uint32 flip = syRandom();

					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						TargetMotesGlintFlips[flip & 3] | COMPOSITE_CLIP_X |
							COMPOSITE_CLIP_Y,
						BLITTER_MAX);
					(RamHeapSlots->ramCodePaired)(canvas,
						state->sheet + BattleEffectGlintPictures[picture] +
							TARGET_MOTES_GLINT_PICTURES,
						x, y + 16, BattleEffectGlintPictureWidths[picture],
						BattleEffectGlintPictureHeights[picture]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}

			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
			draw[0] = RamHeapSlots->ramCode;
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
			draw[1] = RamHeapSlots->ramCodePaired;
			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);
			point[0] = object->x;
			point[1] = toFIXED(20);
			point[2] = object->z;
			syMatrixTranslateVector(point);

			if(frame > member * TARGET_MOTES_STAGGER) {
				syRotY(frame << 9);
				if(mode <= TARGET_MOTES_PINK_SMOKE ||
					mode == TARGET_MOTES_HEARTRENDER)
					syRotX(frame << 9);
				for(i = 0; i != count; i++) {
					PARTICLE *mote =
						&PARTICLES[member * TARGET_MOTES_SLOTS + i];
					Sint32 distance;

					if(frame > member * TARGET_MOTES_STAGGER + i) {
						Sint32 xx = (mote->x >> 8) * (mote->x >> 8);
						Sint32 yy = (mote->y >> 8) * (mote->y >> 8);
						Sint32 zz = (mote->z >> 8) * (mote->z >> 8);

						distance = sySquartIWRAM(xx + yy + zz) >> 9;
						if(distance != 0 &&
							mote->variant < TARGET_MOTES_CYCLE) {
							Sint32 picture = mote->variant / 4;

							btEffectPosProjectPoint((Sint32 *)mote, &screen);
							screen.x >>= 1;
							if(mode == TARGET_MOTES_MYSTIC_FLAME ||
								mode == TARGET_MOTES_DEMON_FIRE)
								draw[1](canvas,
									state->sheet +
										picture * PICTURE_BYTES(GLOW_ORB),
									screen.x - GLOW_ORB_WIDTH / 2,
									screen.y - GLOW_ORB_HEIGHT / 2,
									GLOW_ORB_WIDTH, GLOW_ORB_HEIGHT);
							else if(mode == TARGET_MOTES_DROWN)
								draw[1](canvas,
									state->sheet + DROWN_DROP_PICTURE,
									screen.x - DROWN_DROP_WIDTH / 2,
									screen.y - DROWN_DROP_HEIGHT / 2,
									DROWN_DROP_WIDTH, DROWN_DROP_HEIGHT);
							else if(mode == TARGET_MOTES_HEARTRENDER)
								draw[1](canvas, state->sheet,
									screen.x - HEART_WIDTH / 2,
									screen.y - HEART_HEIGHT / 2, HEART_WIDTH,
									HEART_HEIGHT);
							else
								draw[(i & 3) != 0](canvas,
									state->sheet +
										picture * PICTURE_BYTES(SMOKE_PUFF),
									screen.x - SMOKE_PUFF_WIDTH / 2,
									screen.y - SMOKE_PUFF_HEIGHT / 2,
									SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							if(TARGET_MOTES_CLOSE_IN(mode)) {
								mote->x -= mote->x / distance;
								mote->y -= mote->y / distance;
								mote->z -= mote->z / distance;
							} else {
								mote->y += toFIXED(1);
							}
							mote->variant++;
							if(mote->variant == TARGET_MOTES_CYCLE) {
								if(TARGET_MOTES_CLOSE_IN(mode)) {
									mote->variant = 0;
								} else {
									SET_POSITION(mote,
										((syRandom() & 0xff) - 127) << 13,
										((syRandom() & 0xff) - 255) << 12,
										((syRandom() & 0xff) - 127) << 13);
								}
							}
						}
					}
				}
			}
			EFFECT_RELEASE_BLITTERS();

			if(frame == member * TARGET_MOTES_STAGGER + 16) {
				Sint32 delay = total - frame;

				if(delay > 31)
					delay = 31;
				btEffectTintBattler(args->targets[member], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, member, delay);
			}
		}

		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	if(TARGET_MOTES_RIPPLE(mode))
		syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	btEffectEndCanvasLayer();
}

#define SLEEP_TICKS		  98
#define SLEEP_SEEDED	  32 /* particles cleared at the start */
#define SLEEP_SHEEP		  12 /* sheep dropped */
/* SheepCardSheet: the Sleep Card's picture, which Sleep does not draw, then
   four pictures of a tumbling sheep, drawn flipped for the next four. */
#define SHEEP_PICTURES	  4
/* A sheep's variant counts its tumble, z a tick: it turns to its next picture
   every SHEEP_TUMBLE_STEP. */
#define SHEEP_TUMBLE_STEP 16
/* The ground the sheep were meant to bounce off. */
#define GROUND_Y		  toFIXED(92)

/* Twelve sheep drop onto the targets' side, tumbling, and each target falls
   asleep in turn.

   Abilities: Sleep. */
void btEffectRunSleep(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	SCREEN_POINT		 middle;
	FIXED				 anchor[3];
	PARTICLE			*sheep;
	PARTICLE			*flock;
	Sint32				 i;
	Sint32				 tick;
	DRAW_RECTANGLE		 drawFlipped;
	DRAW_RECTANGLE		 drawUpright;
	Sint32				 scroll;
	Sint32				 member;

	flock = (PARTICLE *)RamMapMetatileBuffer;
	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	scroll = 0;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = scroll; /* still 0: no blending */
	btResourceLoadAndDecompress(RESOURCE_SHEEP_CARD_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	drawUpright = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_TRANSPARENT);
	drawFlipped = RamHeapSlots->ramCodePaired;

	/* The sheep wait above the screen with a random spin and starting
	   picture. */
	for(i = 0; i != SLEEP_SEEDED; i++) {
		sheep = &flock[i];
		sheep->x = INTtoFIXED((syRandom() & 63) + 32);
		sheep->y = -toFIXED(32);
		sheep->velocityY = syRandom() & 0;
		sheep->z = syRandom() & 3;
		sheep->variant = syRandom() & 255;
	}

	/* Centre the canvas on the targets; what BG2 cannot reach moves the
	   sheep. */
	btEffectGetTargetsMidpoint(&middle);
	middle.x -= CANVAS_SIZE / 2;
	if(middle.x < 0) {
		scroll = middle.x;
		middle.x = 0;
	} else if(middle.x > NARROW_CANVAS_PARTY_X) {
		scroll = middle.x - NARROW_CANVAS_PARTY_X;
		middle.x = NARROW_CANVAS_PARTY_X;
	}
	REG_BG2X = -middle.x << 8;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	sySoundPlayStub(SE_SWIRL);

	for(tick = 0; tick != SLEEP_TICKS; tick++) {
		BATTLE_CAMERA *camera;

		camera = RamHeapSlots->cameraState;
		if(tick == 80)
			EFFECT_SHOW_DAMAGE(0);

		/* Place each target, and from tick 64 restore its palette, sheep
		   target every 16 ticks. */
		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *object;

			object = btObjectGetStub(args->targets[member])->object;
			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);
			anchor[0] = object->x;
			anchor[1] = toFIXED(40);
			anchor[2] = object->z;
			syMatrixTranslateVector(anchor);
			if(tick == member * 16 + 64)
				btEffectTintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
					POSE_DAMAGE, TINT_KEEP, 0);
		}

		/* The sheep fall, each tumbling through its eight pictures: the first
		   four drawn as they are, the last four flipped. */
		sheep = flock;
		for(i = 0; i != SLEEP_SHEEP; i++, sheep++) {
			if(tick > i * 4 && sheep->y <= toFIXED(128) - 1) {
				Sint32 picture;

				picture = (sheep->variant / SHEEP_TUMBLE_STEP) & 7;
				if(picture < SHEEP_PICTURES)
					drawUpright(canvas,
						state->sheet + picture * PICTURE_BYTES(SHEEP) +
							PICTURE_BYTES(CARD),
						scroll + FIXEDtoINT(sheep->x) - SHEEP_WIDTH / 2,
						FIXEDtoINT(sheep->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
						SHEEP_HEIGHT);
				else
					drawFlipped(canvas,
						state->sheet + picture * PICTURE_BYTES(SHEEP) -
							SHEEP_PICTURES * PICTURE_BYTES(SHEEP) +
							PICTURE_BYTES(CARD),
						scroll + FIXEDtoINT(sheep->x) - SHEEP_WIDTH / 2,
						FIXEDtoINT(sheep->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
						SHEEP_HEIGHT);
				sheep->y += sheep->velocityY;
				sheep->velocityY += toFIXED(1) / 8;
				sheep->variant += sheep->z;
				/* BUG: meant to bounce a sheep off the ground, tumbling faster,
				   but it tests for a speed of exactly 0 just after gravity was
				   added, so the sheep fall on through. BUGFIX bounces any sheep
				   that is falling. */
#if FIX_SLEEP_SHEEP_BOUNCE
				if(sheep->y > GROUND_Y && sheep->velocityY > 0) {
#else
				if(sheep->y > GROUND_Y && sheep->velocityY == 0) {
#endif
					Sint32 speed = sheep->velocityY + 1;
					sheep->y = GROUND_Y;
					sheep->z += 4;
					sheep->velocityY = -speed / 2;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_FINISH_CANVAS();
}

#define CURSE_CANDLE_TICKS 72 /* how long each target's candle burns */
/* The ticks between one target's candle and the next's. */
#define CURSE_STAGGER	   16

/* FlameColumnSheet: the flame's nine pictures, then the candle. */
#define CURSE_FLAME_WIDTH	 16
#define CURSE_FLAME_HEIGHT	 12
#define CURSE_FLAME_PICTURES 9
#define CURSE_CANDLE_WIDTH	 16
#define CURSE_CANDLE_HEIGHT	 20
#define CURSE_CANDLE_PICTURE (CURSE_FLAME_PICTURES * PICTURE_BYTES(CURSE_FLAME))

/* A candle stands on each target's head in turn, a small flame burning on it.

   Abilities: Curse; Lucid Prophecy. */
void btEffectRunCurse(BATTLE_EFFECT_PARM *args)
{
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	DRAW_RECTANGLE		 draw[2];
	PARTICLE			*flame;
	Sint32				 i;
	Sint32				 frame;
	Sint32				 start;
	Sint32				 picture;
	Sint32				 y;
	SCREEN_POINT		 pos;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(
		RESOURCE_FLAME_COLUMN_SHEET, state->sheet, 1, 1);
	if(args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	state->transferMode = CANVAS_COPY_AND_CLEAR;
	draw[0] = RamHeapSlots->ramCode;
	state->transferValue = 0;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	/* Each flame starts at a random point of its flicker. */
	EFFECT_FOR_EACH_TARGET(i, args) {
		state->particles[i].variant = syRandom() & 63;
	}

	for(frame = 0; frame != (args->targetCount << 5) + 32; frame += 1) {
		if(frame == 32) {
			EFFECT_SHOW_DAMAGE(0);
		}
		i = 0;
		if(args->targetCount != 0) {
			for(; i != args->targetCount; i++) {
				start = i * CURSE_STAGGER;
				if(frame == start) {
					sySoundPlayStub(SE_CRACK);
				}
				if(frame >= start && frame < start + CURSE_CANDLE_TICKS) {
					/* The candle on the target's head, then its flame, six
					   ticks a picture. */
					flame = &state->particles[i];
					btEffectPosProjectScaledBattler(args->targets[i], &pos);
					if(args->side == EFFECT_SIDE_PARTY) {
						pos.x -= NARROW_CANVAS_PARTY_X;
					}
					y = pos.y;
					pos.y = y - 16;
					draw[0](canvas, state->sheet + CURSE_CANDLE_PICTURE,
						pos.x - CURSE_CANDLE_WIDTH / 2, y - CURSE_CANDLE_HEIGHT,
						CURSE_CANDLE_WIDTH, CURSE_CANDLE_HEIGHT);
					/* DEAD: always true here; the ROM keeps the test. */
					if(frame >= start) {
						picture = ((frame - start) + flame->variant) / 6 %
							CURSE_FLAME_PICTURES;
						draw[0](canvas,
							state->sheet + picture * PICTURE_BYTES(CURSE_FLAME),
							pos.x - CURSE_FLAME_WIDTH / 2, pos.y - 16,
							CURSE_FLAME_WIDTH, CURSE_FLAME_HEIGHT);
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define DJINN_HEAL_SPARKS			 512 /* sparks in PARTICLES, 64 for each target */
#define DJINN_HEAL_SPARKS_PER_TARGET 64

#define DJINN_HEAL_SWIRL_TICKS		  98
#define DJINN_HEAL_SWIRL_MOTES		  32 /* motes swirled in */
/* The motes set out to swirl, of which DJINN_HEAL_SWIRL_MOTES are drawn. */
#define DJINN_HEAL_SWIRL_SLOTS		  64
/* The ticks the swirl takes to travel from the user to above the first target,
   from tick 30. */
#define DJINN_HEAL_TRAVEL_TICKS		  40
/* A swirling mote is drawn at size DJINN_HEAL_MOTE_SIZE_MAX at depth
   DJINN_HEAL_MOTE_DEPTH_NEAR or nearer, one size smaller every
   DJINN_HEAL_MOTE_DEPTH_STEP farther, its depth held below
   DJINN_HEAL_MOTE_DEPTH_FAR. */
#define DJINN_HEAL_MOTE_SIZE_MAX	  9
#define DJINN_HEAL_MOTE_DEPTH_NEAR	  250
#define DJINN_HEAL_MOTE_DEPTH_STEP	  64
#define DJINN_HEAL_MOTE_DEPTH_FAR	  634
/* The depths the sparkles' idle size by depth is worked out over, as the
   healing sparkles' are: three sizes, DJINN_HEAL_SPARKLE_DEPTH_STEP apart. */
#define DJINN_HEAL_SPARKLE_DEPTH_NEAR 314
#define DJINN_HEAL_SPARKLE_DEPTH_STEP 128
#define DJINN_HEAL_SPARKLE_DEPTH_FAR  634
/* The angle mote `i` turns each tick about its axis: each later mote
   faster. */
#define DJINN_HEAL_MOTE_SPIN(i)		  ((i) * 32 + 256)
/* How far, in pixels, the Djinni sways each way over the targets. */
#define DJINN_HEAL_SWAY				  40

/* Unused: nothing reaches it. A small Mercury Djinni heals the party: motes
   swirl up from the user and over the first target, the Djinni appears where
   they gather, and sparkles spiral in on each target in turn. */
UNUSED void btEffectRunDjinnHeal(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 member;
	Uint8				*sheet;
	DRAW_RECTANGLE		*draw;
	MOTION_OBJECT		*actor;
	MOTION_OBJECT		*goal;
	Sint32				 frame;
	Sint32				 i;
	FIXED				 pos[3];
	FIXED				 target[3];
	FIXED				 delta[3];
	Sint32				 bob[3];
	SCREEN_POINT		 screen;
	FIXED				 point[3];
	SCREEN_POINT		 view;
	DRAW_RECTANGLE		 blitters[2];

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(
		RESOURCE_MERCURY_DJINN_SMALL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btEffectBuildCanvasBlitters(args->side ^ 1, draw = blitters);
	actor = btObjectGetStub(args->user)->object;
	goal = btObjectGetStub(args->targets[0])->object;

	/* The motes, thrown out level about the user. */
	for(i = 0; i != DJINN_HEAL_SWIRL_SLOTS; i++) {
		PARTICLE *spark = &PARTICLES[i];
		Sint32	  heading = syRandom() & TRIG_ANGLE_MASK;
		Sint32	  speed = (syRandom() & 0xff) + 128;

		SET_POSITION(spark, 0, INTtoFIXED((syRandom() & 31) + 20), 0);
		spark->velocityX = (sySin(heading) * speed) >> 5;
		spark->velocityY = 0;
		spark->velocityZ = (syCos(heading) * speed) >> 5;
		spark->variant = 0;
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The swirl travels from the user to above the first target. */
	pos[0] = actor->x;
	pos[1] = 0;
	pos[2] = actor->z;
	target[0] = goal->x;
	target[1] = toFIXED(90);
	target[2] = 0;
	delta[0] = (target[0] - pos[0]) / DJINN_HEAL_TRAVEL_TICKS;
	delta[1] = (target[1] - pos[1]) / DJINN_HEAL_TRAVEL_TICKS;
	delta[2] = (target[2] - pos[2]) / DJINN_HEAL_TRAVEL_TICKS;

	for(frame = 0; frame != DJINN_HEAL_SWIRL_TICKS; frame++) {
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

		if(frame == 8)
			sySoundPlayStub(SE_POWER_UP);
		if(frame == 80)
			sySoundPlayStub(SE_SWIRL);
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);
		if(frame >= 30 && frame < 30 + DJINN_HEAL_TRAVEL_TICKS) {
			pos[0] += delta[0];
			pos[1] += delta[1];
			pos[2] += delta[2];
		}
		syMatrixTranslateVector(pos);

		/* The user flashes for the first 24 ticks. */
		if(frame == 0)
			btEffectTintBattler(
				args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
		if(frame == 24)
			btEffectTintBattler(
				args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);

		/* Each mote spins about one of four axes, smaller farther away, and
		   after 30 ticks is pulled back to the swirl's centre. */
		for(i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
			PARTICLE *spark = &PARTICLES[i];

			if(frame > i && spark->variant == 0) {
				Sint32 size;

				syPushMatrix();
				switch(i & 3) {
				case 0:
					syRotY(frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case 1:
					syRotX(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case 2:
					syRotZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				case 3:
					syRotX(-frame * DJINN_HEAL_MOTE_SPIN(i));
					syRotZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
					break;
				}
				btEffectPosProjectPoint((Sint32 *)spark, &screen);
				screen.x >>= 1;
				syPopMatrix();
				CLAMP(screen.depth, DJINN_HEAL_MOTE_DEPTH_NEAR,
					DJINN_HEAL_MOTE_DEPTH_FAR);
				size = DJINN_HEAL_MOTE_SIZE_MAX -
					(screen.depth - DJINN_HEAL_MOTE_DEPTH_NEAR) /
						DJINN_HEAL_MOTE_DEPTH_STEP;
				EFFECT_DRAW_MOTE(
					blitters[0], canvas, sheet, screen.x, screen.y, size);
				btParticleAdvanceWithGravity3D(spark, 60, 0);
				if(frame > i + 30) {
					Sint32 dx = -spark->x >> 8;
					Sint32 dy = -spark->y >> 8;
					Sint32 dz = -spark->z >> 8;

					spark->velocityX += dx;
					spark->velocityY += dy;
					spark->velocityZ += dz;
				}
			}
		}

		/* The Djinni appears and bobs, for the last 15 ticks. */
		if(frame > 82) {
			bob[0] = 0;
			bob[1] = sySin(frame * (TRIG_TURN / 64)) * 4;
			bob[2] = 0;
			btEffectPosProjectPoint(bob, &screen);
			screen.x >>= 1;
			draw[1](canvas, state->sheet, screen.x - SMALL_DJINNI_WIDTH / 2,
				screen.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
				MERCURY_SMALL_DJINNI_HEIGHT);
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	/* The healing: DJINN_HEAL_SPARKS_PER_TARGET sparkles for each target. */
	for(i = 0; i != DJINN_HEAL_SPARKS; i++) {
		PARTICLE *spark = &PARTICLES[i];

		SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
			((syRandom() & 0x7f) + 64) << 15,
			((syRandom() & 0xff) - 127) << 15);
		spark->variant = 0;
	}
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, sheet, 0, 0);

	for(frame = 0; frame != args->targetCount * 8 + 72; frame++) {
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

		/* The Djinni sways over the targets, then rises away for the last 32
		   ticks. */
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);
		if(frame >= args->targetCount * 8 + 40)
			target[1] += toFIXED(4);
		point[0] = target[0];
		point[1] = target[1];
		point[2] =
			target[2] + sySin(frame * (TRIG_TURN / 32)) * DJINN_HEAL_SWAY;
		btEffectPosProjectPoint(point, &view);
		view.x >>= 1;
		blitters[0](canvas, state->sheet, view.x - SMALL_DJINNI_WIDTH / 2,
			view.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
			MERCURY_SMALL_DJINNI_HEIGHT);

		/* Each target in turn, eight ticks apart: its sparkles close in around
		   it, and it flashes from 40 to 64 ticks into its turn. */
		EFFECT_FOR_EACH_TARGET(member, args) {
			MOTION_OBJECT *object =
				btObjectGetStub(args->targets[member])->object;
			Sint32 start = member * 8;

			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);
			pos[0] = object->x;
			pos[1] = toFIXED(40);
			pos[2] = object->z;
			syMatrixTranslateVector(pos);
			if(frame == start + 30)
				sySoundPlayStub(SE_RECOVERY);
			if(frame == start + 40)
				btEffectTintBattler(args->targets[member], SPRITE_COLOR_WHITE,
					TINT_KEEP, TINT_KEEP, 0);
			if(frame == start + 64)
				btEffectTintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
					TINT_KEEP, TINT_KEEP, 0);
			if(frame > start) {
				syRotY((frame - start) << 9);
				for(i = 0; i != DJINN_HEAL_SPARKS_PER_TARGET; i++) {
					PARTICLE *spark =
						&PARTICLES[member * DJINN_HEAL_SPARKS_PER_TARGET + i];

					if(frame > start + i / 2) {
						Sint32 xx = (spark->x >> 8) * (spark->x >> 8);
						Sint32 yy = (spark->y >> 8) * (spark->y >> 8);
						Sint32 zz = (spark->z >> 8) * (spark->z >> 8);
						Sint32 distance = sySquartIWRAM(xx + yy + zz) >> 9;

						if(distance != 0) {
							Sint32 pictureIndex;

							btEffectPosProjectPoint((Sint32 *)spark, &view);
							view.x >>= 1;
							/* LEFTOVER(template): the clamp and size by depth,
							 * idle here, as btEffectRunHealingMotes
							 * (battle_effect_healing.c) clamps the depth and
							 * sizes its motes by it, overwriting the size only
							 * for Ply's, Spring's, Breath's and Salt's sparkles
							 * as this does; as both lines removed. */
							CLAMP(view.depth, DJINN_HEAL_SPARKLE_DEPTH_NEAR,
								DJINN_HEAL_SPARKLE_DEPTH_FAR);
							pictureIndex = 3 -
								(view.depth - DJINN_HEAL_SPARKLE_DEPTH_NEAR) /
									DJINN_HEAL_SPARKLE_DEPTH_STEP;
							pictureIndex = (i * 4 + frame) % 9;
							{
								Uint8 *picture = sheet +
									BattleEffectSparklePictures[pictureIndex];
								Uint32 edge =
									BattleEffectSparkleSizes[pictureIndex];
								Uint32 half = edge >> 1;

								draw[1](canvas, picture, view.x - half,
									view.y - half, edge, edge);
							}
							spark->x -= spark->x / distance;
							spark->y -= spark->y / distance;
							spark->z -= spark->z / distance;
						}
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* Abilities: Balm. */
void btEffectRunBalm(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTumblingQuads(args, TUMBLING_QUADS_BALM);
}

/* Abilities: Tinder. */
void btEffectRunTinder(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTumblingQuads(args, TUMBLING_QUADS_TINDER);
}

/* Abilities: Phoenix, Regen Dance. */
void btEffectRunPhoenix(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTumblingQuads(args, TUMBLING_QUADS_PHOENIX);
}

#undef TARGET_MOTES_START_WIDE
#undef TARGET_MOTES_CLOSE_IN
#undef TARGET_MOTES_RIPPLE
#undef TARGET_MOTES_GLINT_PICTURES
#undef TARGET_MOTES
#undef TARGET_MOTES_SLOTS
#undef TARGET_MOTES_CYCLE
#undef TARGET_MOTES_PER_TARGET
#undef TARGET_MOTES_DAMAGE_END_TICKS
#undef DEMON_FIRE_LEAP_TICKS
#undef DEMON_FIRE_APPROACH_TICKS
#undef DEMON_FIRE_ORBS
#undef DROWN_DROPS
#undef TARGET_MOTES_STAGGER
#undef TARGET_MOTES_RUN_ON
#undef TARGET_MOTES_WIDE_RUN_ON
#undef DROWN_DROP_WIDTH
#undef DROWN_DROP_HEIGHT
#undef DROWN_DROP_PICTURE
#undef SLEEP_TICKS
#undef SLEEP_SEEDED
#undef SLEEP_SHEEP
#undef SHEEP_PICTURES
#undef SHEEP_TUMBLE_STEP
#undef GROUND_Y
#undef CURSE_CANDLE_TICKS
#undef CURSE_STAGGER
#undef CURSE_FLAME_WIDTH
#undef CURSE_FLAME_HEIGHT
#undef CURSE_FLAME_PICTURES
#undef CURSE_CANDLE_WIDTH
#undef CURSE_CANDLE_HEIGHT
#undef CURSE_CANDLE_PICTURE
#undef DJINN_HEAL_SPARKS
#undef DJINN_HEAL_SPARKS_PER_TARGET
#undef DJINN_HEAL_SWIRL_TICKS
#undef DJINN_HEAL_SWIRL_MOTES
#undef DJINN_HEAL_SWIRL_SLOTS
#undef DJINN_HEAL_TRAVEL_TICKS
#undef DJINN_HEAL_MOTE_SIZE_MAX
#undef DJINN_HEAL_MOTE_DEPTH_NEAR
#undef DJINN_HEAL_MOTE_DEPTH_STEP
#undef DJINN_HEAL_MOTE_DEPTH_FAR
#undef DJINN_HEAL_SPARKLE_DEPTH_NEAR
#undef DJINN_HEAL_SPARKLE_DEPTH_STEP
#undef DJINN_HEAL_SPARKLE_DEPTH_FAR
#undef DJINN_HEAL_MOTE_SPIN
#undef DJINN_HEAL_SWAY
