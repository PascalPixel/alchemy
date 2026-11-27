/* The Kirin summon, and the two line functions that set BG2's horizontal scale.
   Part of battle_effect.c. */

extern const SPR_SCALE KirinSweepScale;
extern const SPR_SCALE KirinChargeScale;

#define KIRIN_CHARGE_TICKS 96  /* ticks of the charge and the wall of fire */
#define KIRIN_SWEEP_TICKS  120 /* ticks of Kirin's bound through the hoop */

/* The bytes of one of the sprite sheet's four fire blobs. */
#define KIRIN_BLOB_BYTES PICTURE_BYTES(FIRE_BLOB)

/* The sprites Kirin is drawn from, each at its KirinColumns and KirinRows. */
#define KIRIN_SPRITES				 9
/* The forest's scroll speeds up a pixel a tick to this, in pixels a tick. */
#define KIRIN_FOREST_SPEED_MAX		 24
/* How high Kirin rises in each bound, in its last leap away, and in the leap
   that opens its charge, in pixels. */
#define KIRIN_BOUND_HEIGHT			 32
#define KIRIN_LEAP_AWAY_HEIGHT		 96
#define KIRIN_CHARGE_LEAP_HEIGHT	 64
/* The hoop's motes, spaced evenly round it. */
#define KIRIN_HOOP_MOTES			 64
/* The hoop's half-width and half-height for motes of `size` before the cap: an
   ellipse six times as tall as it is wide. */
#define KIRIN_HOOP_HALF_WIDTH(size)	 ((size) * 2)
#define KIRIN_HOOP_HALF_HEIGHT(size) ((size) * 12)
/* The fire trail: the first of state->particles. */
#define KIRIN_TRAIL_MOTES			 48
/* How far the column of fire blobs slides in, in pixels. */
#define KIRIN_BLOB_SLIDE_MAX		 40
/* The motes a target bursts into, at PARTICLES[target * KIRIN_BURST_MOTES]. */
#define KIRIN_BURST_MOTES			 32
/* The charge frees the motes of ten bursts and draws those of the first six
   targets. */
#define KIRIN_BURSTS_FREED			 10
#define KIRIN_BURSTS_DRAWN			 6

/* Over a scrolling forest Kirin bounds in through a hoop of light, trailing
   fire; then it charges in from the right across the targets, each bursting
   into motes as it passes, and a wall of fire sweeps over them and leaves them
   black.

   Abilities: Kirin. */
void btEffectRunKirin(BATTLE_EFFECT_PARM *args)
{
	SPR_PLACEMENT			 place;
	SCREEN_POINT			 unit;
	Uint8					 jitterRow[16];
	Uint8					 hitRow[14];
	SPR_PLACEMENT			 chargePlace;
	void					*canvas;
	BATTLE_EFFECT_STATE		*state;
	DRAW_RECTANGLE			 blit;
	Uint8					*sheet;
	Sint32					 baseX;
	Sint32					 grow;
	Sint32					 savedX;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 shift;
	Sint32					 wave;
	Sint32					 frm;
	Sint32					 i;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEff;
	sheet = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = RGB(24, 0, 0);
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* Setup: wipe the battle away in dark red, swap in the forest and wipe
	   back. */
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	btEffectCreateSprs(KIRIN_SPRITES, SPRITE_KIRIN, 1);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectShowLivingTargets(state->args);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_BG2 | WININ_WIN1_CLR;
	REG_WIN0H = WIN_RANGE(0, 202);
	sySynch(1);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_FOREST_BACKDROP, 0);
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_BLOB_SHEET, state->sheet, 1, 1);
	EFFECT_SHOW_WIDE_CANVAS(CANVAS_BLEND_LEVEL);

	baseX = 0;
	grow = 0;
	savedX = BgScrollWork[1].x;
	view = RamHeapSlots->battleBg;
	shift = 0;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	view->busy = 1;

	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		PARTICLE *fire = &state->particles[i];

		fire->x = (syRandom() & 31) + 16;
		fire->y = INTtoFIXED((syRandom() & 31) + 48);
		fire->velY = toFIXED((syRandom() & 31) - 16);
		fire->variant = syRandom() % 48 + 2;
	}

	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit = RamHeapSlots->ramCode;
	REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);

	/* The sweep: Kirin bounds in over the forest (A or B ends it). */
	for(frm = 0; frm != KIRIN_SWEEP_TICKS; frm++) {
		wave = 0;
		if(frm == 0)
			sySndPlayStub(SE_WHOOSH);
		if(frm == 26)
			sySndPlayStub(SE_RUMBLE);
		if(frm == 40)
			sySndPlayStub(SE_CAST);
		if(frm == 72)
			sySndPlayStub(SE_CAST);
		if(frm == 104)
			sySndPlayStub(SE_CAST);
		if((PerWork.repeat & (KEY_A | KEY_B)) != 0 && frm > 16)
			break;

		if(frm >= 24 && frm <= 55)
			shift++;
		if(shift > KIRIN_FOREST_SPEED_MAX)
			shift = KIRIN_FOREST_SPEED_MAX;
		/* DEAD: tick <= 135 here and tick <= 149 below always hold in the
		   sweep's ticks; the ROM keeps both compares. */
		if(frm <= 135) {
			BgScrollWork[1].x -= shift;
			grow += shift;
		}

		/* Kirin bounds in, the screen shaking at each landing, then lifts away
		   to the left. */
		if(frm <= 149) {
			SPR_SCALE pair = KirinSweepScale;
			Sint32	  lift;

			lift = 0;
			if(frm > 103)
				lift = (frm - 104) * 16;
			if(frm >= 8 && frm <= 31)
				baseX = baseX + shift - 8;
			if(frm > 7) {
				Sint32 height;
				Sint32 ang;

				height = KIRIN_LEAP_AWAY_HEIGHT;
				if(frm <= 104)
					height = KIRIN_BOUND_HEIGHT;
				/* A bound every 32 ticks: half a sine wave, repeated. */
				ang = (frm * (TRIG_TURN / 64) - DEGtoANG(45)) & TRIG_ANGLE_MASK;
				if(ang > DEGtoANG(180))
					ang -= DEGtoANG(180);
				wave = FIXEDtoINT(sySin(ang) * height);
				if((frm & 31) == 8)
					state->shakeFrm = 4;
			}
			place.groundHeight = 0;
			place.height = toFIXED(255);
			for(i = 0; i != KIRIN_SPRITES; i++) {
				place.x =
					toFIXED(baseX + KirinColumns[i] - lift) + toFIXED(224);
				place.y = toFIXED(KirinRows[i] - wave) + toFIXED(72);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[i], &place.x, &pair.x, 0);
			}
		}

		/* A hoop of motes opens in front of Kirin. */
		if(frm <= 26) {
			Sint32 size = grow + 4;
			Sint32 cnt = frm * 8;

			if(size > MOTE_SIZE_MAX)
				size = MOTE_SIZE_MAX;
			if(cnt > KIRIN_HOOP_MOTES)
				cnt = KIRIN_HOOP_MOTES;
			/* The hoop swells and moves right as the forest starts to
			   scroll. */
			for(i = 0; i != cnt; i++) {
				Sint32 ang = i * (TRIG_TURN / KIRIN_HOOP_MOTES);
				Sint32 x =
					FIXEDtoINT(sySin(ang) * KIRIN_HOOP_HALF_WIDTH(grow + 4)) +
					grow + 96;
				Sint32 y =
					FIXEDtoINT(syCos(ang) * KIRIN_HOOP_HALF_HEIGHT(grow + 4)) +
					64;

				EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
			}
		}

		if(frm == 24) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frm == 28)
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

		/* Fire trails Kirin. */
		if(frm > 17) {
			for(i = 0; i != KIRIN_TRAIL_MOTES; i++) {
				PARTICLE *fire = &state->particles[i];

				if(fire->variant == 0) {
					Sint32 size = i % 3 + 1;

					blit(canvas, sheet + BattleEffectMotePictures[size - 1],
						fire->x, FIXED_INTEGER_PART(fire->y) - size, size,
						size * 2);
					fire->x += 2;
					fire->y += fire->velY;
					fire->velY = fire->velY * 48 / DAMPING_SCALE;
				} else {
					fire->variant--;
				}
				if(fire->x > 128 || fire->variant == 1) {
					fire->x = (syRandom() & 31) + baseX + 172;
					fire->y = INTtoFIXED((syRandom() & 31) - wave + 56);
					fire->velY = ((syRandom() & 31) - 16) * (toFIXED(1) / 2);
				}
			}
		}

		/* A column of fire blobs slides left. */
		if(frm > 31) {
			Sint32 slide = (frm - 32) / 2;

			if(slide > KIRIN_BLOB_SLIDE_MAX)
				slide = KIRIN_BLOB_SLIDE_MAX;
			for(i = 0; i != 6; i++)
				blit(canvas, state->sheet + (syRandom() & 3) * KIRIN_BLOB_BYTES,
					120 - slide, i * 18, FIRE_BLOB_WIDTH, FIRE_BLOB_HEIGHT);
		}

		if(state->shakeFrm > 0) {
			state->shakeFrm--;
			BgScrollWork[1].y =
				(syRandom() & 7) + BATTLE_BACKGROUND_SCROLL_Y - 4;
		} else {
			BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	/* The charge: Kirin runs across the targets behind the battle
	   background. */
	BgScrollWork[1].x = savedX;
	view->busy = 0;
	btEffectSetupBlendedDisp();
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);

	for(i = 0; i != KIRIN_SPRITES; i++) {
		SPR *spr = state->sprs[i];

		spr->image.pri = 3;
	}

	{
		Sint32 rowX;

		rowX = 224;
		for(i = 0; i != ARRAY_COUNT(hitRow); i++)
			hitRow[i] = 0;
		for(i = 0; i != ARRAY_COUNT(jitterRow); i++)
			jitterRow[i] = syRandom() & 31;
		for(i = 0; i != KIRIN_BURSTS_FREED * KIRIN_BURST_MOTES; i++)
			PARTICLES[i].variant = 0;

		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
		REG_BG2CNT =
			BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		REG_BLDALPHA = EFFECT_BLEND_ADD;

		for(frm = 0; frm != KIRIN_CHARGE_TICKS; frm++) {
			if(frm <= 23) {
				SPR_SCALE chargeScale = KirinChargeScale;
				Sint32	  ang;
				Sint32	  rise;

				rowX -= 16;
				/* A bound every 16 ticks, the first higher. */
				if(frm <= 8) {
					ang = frm * (TRIG_TURN / 32) + DEGtoANG(90);
					if(ang > DEGtoANG(180))
						ang -= DEGtoANG(180);
					rise = FIXEDtoINT(sySin(ang) * KIRIN_CHARGE_LEAP_HEIGHT);
				} else {
					ang = frm * (TRIG_TURN / 32) + DEGtoANG(90);
					if(ang > DEGtoANG(180))
						ang -= DEGtoANG(180);
					rise = FIXEDtoINT(sySin(ang) * KIRIN_BOUND_HEIGHT);
				}
				chargePlace.groundHeight = 0;
				chargePlace.height = toFIXED(255);
				for(i = 0; i != KIRIN_SPRITES; i++) {
					chargePlace.x = toFIXED(rowX + KirinColumns[i]);
					chargePlace.y = toFIXED(KirinRows[i] - rise) + toFIXED(72);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], &chargePlace.x, &chargeScale.x, 0);
				}
			}

			if(frm == 8) {
				state->shakeFrm = 8;
				sySndPlayStub(SE_CRASH);
			}
			if(frm == 11)
				sySndPlayStub(SE_CRASH);
			if(frm == 46)
				sySndPlayStub(SE_KIRIN);

			EFFECT_FOR_EACH_TARGET(i, args) {
				/* A target Kirin passes bursts into motes and reels. */
				if(hitRow[i] == 0) {
					btEffectPosProjectBattler(args->tgts[i], &unit);
					if(unit.x > rowX) {
						Sint32 k;

						hitRow[i] = 1;
						for(k = 0; k != KIRIN_BURST_MOTES; k++) {
							PARTICLE *burst =
								&PARTICLES[i * KIRIN_BURST_MOTES + k];
							Sint32 up;

							/* From the target's place on the wide canvas, 16
							   pixels up. */
							burst->x = unit.x * (toFIXED(1) / 2);
							burst->y = INTtoFIXED(unit.y - 16);
							burst->velX =
								((syRandom() & 255) - 128) * (toFIXED(1) / 64);
							up = (syRandom() & 255) - 192;
							burst->velY = up * (toFIXED(1) / 32);
							burst->x += burst->velX * 4;
							burst->y += up * (toFIXED(1) / 8);
							burst->variant = (syRandom() & 15) + 8;
						}
						btMotionApplyKnockbackStub(
							args->tgts[i], KNOCKBACK_HOP);
						sySndPlayStub(SE_HEAVY_IMPACT);
					}
				}
			}

			for(i = 0; i != KIRIN_BURSTS_DRAWN * KIRIN_BURST_MOTES; i++) {
				PARTICLE *burst = &PARTICLES[i];

				if(burst->variant > 0) {
					Sint32 size = 3;

					EFFECT_DRAW_MOTE(blit, canvas, sheet,
						FIXED_INTEGER_PART(burst->x),
						FIXED_INTEGER_PART(burst->y), size);
					burst->x += burst->velX;
					burst->y += burst->velY;
					burst->variant--;
				}
			}

			if(frm == 48)
				sySndPlayStub(SE_WHOOSH);
			/* The wall of fire slides off to the left and its smear dies
			   away. */
			if(frm > 40) {
				Sint32 slide = (frm - 40) * 12;

				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
				for(i = 0; i != ARRAY_COUNT(jitterRow); i++)
					blit(canvas,
						state->sheet + (syRandom() & 3) * KIRIN_BLOB_BYTES,
						jitterRow[i] - slide + 120, i * 8 - 8, FIRE_BLOB_WIDTH,
						FIRE_BLOB_HEIGHT);
			}
			if(frm > 64)
				state->transferMode = CANVAS_COPY_AND_FADE;
			/* Every target, burnt by the wall of fire, turns black. */
			if(frm == 58) {
				EFFECT_FOR_EACH_TARGET(i, args)
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_BLACK,
						POSE_DAMAGE, TINT_KEEP, 0);
			}

			btCameraShake(8, 8);
			EFFECT_END_TICK(state);
		}
	}

	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	for(i = 0; i != KIRIN_SPRITES; i++)
		fdSprDestroyStub(state->sprs[i]);
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef KIRIN_CHARGE_TICKS
#undef KIRIN_SWEEP_TICKS
#undef KIRIN_BLOB_BYTES
#undef KIRIN_SPRITES
#undef KIRIN_FOREST_SPEED_MAX
#undef KIRIN_BOUND_HEIGHT
#undef KIRIN_LEAP_AWAY_HEIGHT
#undef KIRIN_CHARGE_LEAP_HEIGHT
#undef KIRIN_HOOP_MOTES
#undef KIRIN_HOOP_HALF_WIDTH
#undef KIRIN_HOOP_HALF_HEIGHT
#undef KIRIN_TRAIL_MOTES
#undef KIRIN_BLOB_SLIDE_MAX
#undef KIRIN_BURST_MOTES
#undef KIRIN_BURSTS_FREED
#undef KIRIN_BURSTS_DRAWN

/* Sets BG2's horizontal scale back to one texel a pixel above the stretched
   lines. */
void btEffectResetBg2Pa(void)
{
	REG_BG2PA = Q_8_8(1);
}

/* Stretches BG2 to twice its width from the current line down. */
void btEffectSetBg2AffineScaleHalf(void)
{
	REG_BG2PA = Q_8_8(0.5);
}
