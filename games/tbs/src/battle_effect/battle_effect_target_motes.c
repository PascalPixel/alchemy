/* The status motes of Taint, Delude, Paralyze, Heartrender, Mystic Flame, Demon
   Fire and Drown, then Sleep's sheep and Curse's flames. Part of
   battle_effect.c. */

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
   variant. No effect table entry in The Broken Seal points here. */
UNUSED void btEffectRunUnusedPinkSmoke(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/* Cyan smoke rises around each target.

   Abilities: Delude, Confuse, Mist. */
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

   Abilities: Demon Fire. */
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

/* Where Paralyze keeps its glint images in state->sheet. */
enum { TARGET_MOTES_GLINT_PICTURES = 9600 };

#define TARGET_MOTES				  512 /* motes scattered at the start */
#define TARGET_MOTES_SLOTS			  64 /* particles reserved for each target */
#define TARGET_MOTES_CYCLE			  24 /* ticks of a mote's image cycle */
/* The motes about each target: TARGET_MOTES_PER_TARGET, or Demon Fire's orbs or
   Drown's drops. */
#define TARGET_MOTES_PER_TARGET		  16
#define DEMON_FIRE_ORBS				  24
#define DROWN_DROPS					  32
/* The ticks between one target's motes starting and the next's. */
#define TARGET_MOTES_STAGGER		  8
/* The ticks the effect runs on after the last target's motes start. */
#define TARGET_MOTES_RUN_ON			  32
#define TARGET_MOTES_WIDE_RUN_ON	  64
/* The ticks the effect runs on after the damage shows. Demon Fire's user sets
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
	Sint32				 frm;
	Sint32				 total;
	Sint32				 member;
	Sint32				 cnt;
	SCREEN_POINT		 screen;
	Sint32				 point[3];
	SCREEN_POINT		 base;
	BLITTER_PAIR		 draw;
	void				*palette;
	Sint32				*row;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	cnt = TARGET_MOTES_PER_TARGET;
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
	if(mode == TARGET_MOTES_TAINT) {
		palette = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	} else if(mode == TARGET_MOTES_PINK_SMOKE) {
		palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
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
		cnt = DEMON_FIRE_ORBS;
		btResourceLoadAndDecompress(
			RESOURCE_GLOW_ORB_SHEET, state->sheet, 1, 0);
		palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	} else {
		cnt = DROWN_DROPS;
		btResourceLoadAndDecompress(RESOURCE_VORTEX_SHEET, state->sheet, 1, 0);
		palette = syResourceGetTableEntry(RESOURCE_VORTEX_SHEET);
	}
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
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
		total = state->args->tgtCnt * TARGET_MOTES_STAGGER +
			TARGET_MOTES_WIDE_RUN_ON;
	else
		total =
			state->args->tgtCnt * TARGET_MOTES_STAGGER + TARGET_MOTES_RUN_ON;
	if(TARGET_MOTES_RIPPLE(mode))
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	sySndPlayStub(SE_SWIRL);

	for(frm = 0; frm != total; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		/* Demon Fire: the user steps up to the first target and strikes it. */
		if(mode == TARGET_MOTES_DEMON_FIRE) {
			if(frm ==
				total - TARGET_MOTES_DAMAGE_END_TICKS - DEMON_FIRE_LEAP_TICKS)
				btMotionApproachTargetStub(state->args->user,
					state->args->tgts[0], DEMON_FIRE_APPROACH_TICKS, 0);
			if(frm == total - TARGET_MOTES_DAMAGE_END_TICKS) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				btMotionApplyKnockbackStub(
					state->args->tgts[0], KNOCKBACK_HURL);
				state->shakeFrm = 8;
			}
		} else if(frm == total - TARGET_MOTES_DAMAGE_END_TICKS) {
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}
		row = state->bg2xLines;
		for(i = 0; i != DISPLAY_HEIGHT; i++)
			*row++ = CANVAS_WAVE_BG2X(toFIXED(4) -
				sySin(frm * (TRIG_TURN / 16) + i * (TRIG_TURN / 32)) * 4);

		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor = btObjectGetStub(state->args->tgts[member])->actor;

			/* Paralyze's glints sparkle near the target. */
			if(mode == TARGET_MOTES_PARALYZE &&
				frm > member * TARGET_MOTES_STAGGER &&
				frm < member * TARGET_MOTES_STAGGER + 32) {
				btProjectionScaledUnit(state->args->tgts[member], &base);
				for(i = 0; i != 2; i++) {
					Sint32 ang;
					Sint32 radius;
					Sint32 x;
					Sint32 y;

					ang = syRandom() & TRIG_ANGLE_MASK;
					radius = (syRandom() & 31) + 4;
					x = base.x / 2 + (FIXEDtoINT(sySin(ang) * radius) >> 1) -
						(BattleEffectGlintPictureWidths[frm & 3] >> 1);
					y = base.y - FIXEDtoINT(syCos(ang) * radius) -
						(BattleEffectGlintPictureHeights[frm & 3] >> 1);
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS |
							TargetMotesGlintFlips[syRandom() & 3],
						COMPOSITE_LIGHTEN);
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
						state->sheet + BattleEffectGlintPictures[frm & 3] +
							TARGET_MOTES_GLINT_PICTURES,
						x, y + 16, BattleEffectGlintPictureWidths[frm & 3],
						BattleEffectGlintPictureHeights[frm & 3]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
			draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, actor->x.fixed, toFIXED(20), actor->z.fixed);
			syMatrixTranslateVector(point);
			if(frm > member * TARGET_MOTES_STAGGER) {
				syRotY(frm * (TRIG_TURN / 128));
				if(mode <= TARGET_MOTES_PINK_SMOKE ||
					mode == TARGET_MOTES_HEARTRENDER)
					syRotX(frm * (TRIG_TURN / 128));
				for(i = 0; i != cnt; i++) {
					PARTICLE *mote =
						&PARTICLES[member * TARGET_MOTES_SLOTS + i];
					Sint32 distance;

					if(frm > member * TARGET_MOTES_STAGGER + i) {
						Sint32 xx = (mote->x >> 8) * (mote->x >> 8);
						Sint32 yy = (mote->y >> 8) * (mote->y >> 8);
						Sint32 zz = (mote->z >> 8) * (mote->z >> 8);

						distance = sySquartIWRAM(xx + yy + zz) >> 9;
						if(distance != 0 &&
							mote->variant < TARGET_MOTES_CYCLE) {
							Sint32 picture = mote->variant / 4;

							btProjectionPoint(mote, &screen);
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
			if(frm == member * TARGET_MOTES_STAGGER + 16) {
				Sint32 delay = total - frm;

				if(delay > 31)
					delay = 31;
				btEffectTintBattler(state->args->tgts[member],
					SPRITE_COLOR_WHITE, POSE_DAMAGE, member, delay);
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

#define SLEEP_TICKS			 148
#define SLEEP_SEEDED		 32 /* particles cleared at the start */
#define SLEEP_SHEEP			 12 /* sheep dropped */
/* The sheep drop SLEEP_SHEEP_STAGGER ticks apart; the targets fall asleep
   SLEEP_TARGET_STAGGER ticks apart from SLEEP_ASLEEP_TICK, and the damage
   shows at SLEEP_DAMAGE_TICK. */
#define SLEEP_SHEEP_STAGGER	 4
#define SLEEP_ASLEEP_TICK	 64
#define SLEEP_TARGET_STAGGER 16
#define SLEEP_DAMAGE_TICK	 80
/* The ground the sheep were meant to bounce off. */
#define SLEEP_GROUND_Y		 toFIXED(92)
/* SheepSheet: four pictures, then the four drawn flipped. */
#define SHEEP_WIDTH			 32
#define SHEEP_HEIGHT		 32
#define SHEEP_PICTURES		 4
/* A sheep's variant counts its tumble, z a tick: it turns to its next picture
   every SHEEP_TUMBLE_STEP. */
#define SHEEP_TUMBLE_STEP	 16

/* Twelve sheep drop onto the targets' side, tumbling, and each target falls
   asleep in turn.

   Abilities: Sleep. */
void btEffectRunSleep(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 anchor[3];
	PARTICLE			*current;
	PARTICLE			*sheep;
	Sint32				 particleIndex;
	Sint32				 tick;
	DRAW_RECTANGLE		 drawFlipped;
	DRAW_RECTANGLE		 drawUpright;
	Sint32				 targetIndex;

	sheep = PARTICLES;
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(0);
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_SHEEP_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	drawUpright = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_TRANSPARENT);
	drawFlipped = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

	for(particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
		current = &sheep[particleIndex];
		current->x = INTtoFIXED((syRandom() & 63) + 32);
		current->y = -toFIXED(32);
		current->velY = syRandom() & 0; /* a random number drawn for nothing */
		current->z = syRandom() & 3;
		current->variant = syRandom() & 255;
	}
	if(state->args->side == EFFECT_SIDE_PARTY)
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	sySndPlayStub(SE_SWIRL);
	for(tick = 0; tick != SLEEP_TICKS; tick++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		if(tick == SLEEP_DAMAGE_TICK)
			EFFECT_SHOW_DAMAGE(0);
		EFFECT_FOR_EACH_TARGET(targetIndex, state->args) {
			ACTOR *target;

			target = btObjectGetStub(state->args->tgts[targetIndex])->actor;
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(anchor, target->x.fixed, toFIXED(40), target->z.fixed);
			syMatrixTranslateVector(anchor);
			if(tick == targetIndex * SLEEP_TARGET_STAGGER + SLEEP_ASLEEP_TICK)
				btEffectTintBattler(state->args->tgts[targetIndex],
					SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP, 0);
		}
		for(particleIndex = 0; particleIndex != SLEEP_SHEEP; particleIndex++) {
			current = &sheep[particleIndex];
			if(tick > particleIndex * SLEEP_SHEEP_STAGGER &&
				current->y <= toFIXED(CANVAS_SIZE) - 1) {
				Sint32 picture;

				picture = (current->variant / SHEEP_TUMBLE_STEP) &
					(SHEEP_PICTURES * 2 - 1);
				if(picture < SHEEP_PICTURES)
					drawUpright(canvas,
						state->sheet + picture * PICTURE_BYTES(SHEEP),
						FIXEDtoINT(current->x) - SHEEP_WIDTH / 2,
						FIXEDtoINT(current->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
						SHEEP_HEIGHT);
				else
					drawFlipped(canvas,
						state->sheet + picture * PICTURE_BYTES(SHEEP) -
							SHEEP_PICTURES * PICTURE_BYTES(SHEEP),
						FIXEDtoINT(current->x) - SHEEP_WIDTH / 2,
						FIXEDtoINT(current->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
						SHEEP_HEIGHT);
				current->y += current->velY;
				current->velY += toFIXED(1) / 8;
				current->variant += current->z;
				/* BUG: meant to bounce a sheep off the ground, tumbling faster,
				   but it tests for a speed of exactly 0 just after gravity was
				   added, so the sheep almost always fall on through. BUGFIX
				   bounces any sheep that is falling. */
#if FIX_SLEEP_SHEEP_BOUNCE
				if(current->y > SLEEP_GROUND_Y && current->velY > 0) {
#else
				if(current->y > SLEEP_GROUND_Y && current->velY == 0) {
#endif
					Sint32 speed = current->velY + 1;
					current->y = SLEEP_GROUND_Y;
					current->z += 4;
					current->velY = -speed / 2;
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS();
}

#define CURSE_CANDLE_TICKS	72 /* how long each target's candle burns */
/* The ticks between one target's candle and the next's. */
#define CURSE_STAGGER		16
/* The flame flickers through its pictures, CURSE_FLICKER_TICKS a picture. */
#define CURSE_FLICKER_TICKS 6

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
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR draw;
	PARTICLE	*candle;
	Sint32		 i;
	Sint32		 frm;
	Sint32		 start;
	Sint32		 picture;
	Sint32		 y;
	SCREEN_POINT screenPos;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(
		RESOURCE_FLAME_COLUMN_SHEET, state->sheet, 1, 1);
	if(state->args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	state->transferMode = CANVAS_COPY_AND_CLEAR;
	state->transferValue = 0;
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* Each flame starts at a random point of its flicker. */
	EFFECT_FOR_EACH_TARGET(i, state->args) {
		state->particles[i].variant = syRandom() & 63;
	}
	for(frm = 0; frm != (state->args->tgtCnt << 5) + 32; frm++) {
		if(frm == 32) {
			EFFECT_SHOW_DAMAGE(0);
		}
		i = 0;
		if(state->args->tgtCnt != 0) {
			candle = state->particles;
			do {
				start = i * CURSE_STAGGER;
				if(frm == start) {
					sySndPlayStub(SE_CRACK);
				}
				if(frm >= start && frm < start + CURSE_CANDLE_TICKS) {
					btProjectionScaledUnit(state->args->tgts[i], &screenPos);
					if(state->args->side == EFFECT_SIDE_PARTY) {
						screenPos.x -= NARROW_CANVAS_PARTY_X;
					}
					y = screenPos.y;
					screenPos.y = y - 16;
					draw[0](canvas, state->sheet + CURSE_CANDLE_PICTURE,
						screenPos.x - CURSE_CANDLE_WIDTH / 2,
						y - CURSE_CANDLE_HEIGHT, CURSE_CANDLE_WIDTH,
						CURSE_CANDLE_HEIGHT);
					/* DEAD: always true here; the ROM keeps the test. */
					if(frm >= start) {
						picture = ((frm - start) + candle->variant) /
							CURSE_FLICKER_TICKS % CURSE_FLAME_PICTURES;
						draw[0](canvas,
							state->sheet + picture * PICTURE_BYTES(CURSE_FLAME),
							screenPos.x - CURSE_FLAME_WIDTH / 2,
							screenPos.y - 16, CURSE_FLAME_WIDTH,
							CURSE_FLAME_HEIGHT);
					}
				}
				candle++;
				i++;
			} while(i != state->args->tgtCnt);
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef TARGET_MOTES_START_WIDE
#undef TARGET_MOTES_CLOSE_IN
#undef TARGET_MOTES_RIPPLE
#undef TARGET_MOTES
#undef TARGET_MOTES_SLOTS
#undef TARGET_MOTES_CYCLE
#undef TARGET_MOTES_PER_TARGET
#undef DEMON_FIRE_ORBS
#undef DROWN_DROPS
#undef TARGET_MOTES_STAGGER
#undef TARGET_MOTES_RUN_ON
#undef TARGET_MOTES_WIDE_RUN_ON
#undef DROWN_DROP_WIDTH
#undef DROWN_DROP_HEIGHT
#undef DROWN_DROP_PICTURE
#undef SHEEP_PICTURES
#undef SHEEP_TUMBLE_STEP
#undef SLEEP_TICKS
#undef SLEEP_SEEDED
#undef SLEEP_SHEEP
#undef TARGET_MOTES_DAMAGE_END_TICKS
#undef DEMON_FIRE_LEAP_TICKS
#undef DEMON_FIRE_APPROACH_TICKS
#undef SLEEP_SHEEP_STAGGER
#undef SLEEP_ASLEEP_TICK
#undef SLEEP_TARGET_STAGGER
#undef SLEEP_DAMAGE_TICK
#undef SLEEP_GROUND_Y
#undef CURSE_CANDLE_TICKS
#undef CURSE_STAGGER
#undef CURSE_FLICKER_TICKS
#undef CURSE_FLAME_PICTURES
#undef CURSE_CANDLE_PICTURE
