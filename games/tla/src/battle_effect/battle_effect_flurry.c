/* Gust, Flying Attack, the rush bursts and the claws: the flurry of strikes
   they all play. Part of battle_effect.c. */

/* Each plays the flurry mode `basic`, `stronger` or `strongest` as the effect's
   variant (EFFECT_VARIANT) says. */
#define FLURRY_DEFINE_BY_VARIANT(name, basic, stronger, strongest) \
	void name(BATTLE_EFFECT_PARM *args) \
	{ \
		if(args->variant == EFFECT_VARIANT_BASIC) { \
			btEffectRunFlurry(args, basic); \
			return; \
		} \
		if(args->variant == EFFECT_VARIANT_STRONGER) { \
			btEffectRunFlurry(args, stronger); \
			return; \
		} \
		btEffectRunFlurry(args, strongest); \
	}

/* The user strikes the first target once, twice or three times as the variant
   grows with a double or triple blow, and each strike fills the canvas with
   light.

   Abilities: Gust; Rapid Smash, Sonic Smash, Slice. */
FLURRY_DEFINE_BY_VARIANT(
	btEffectRunGust, FLURRY_GUST, FLURRY_GUST_DOUBLE, FLURRY_GUST_TRIPLE)

/* The user strikes the first target five times.

   Abilities: Bear Claw; Counterstrike. */
void btEffectRunBearClaw(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BEAR_CLAW);
}

/* Ransack plays the same five strikes as Bear Claw.

   Abilities: Ransack; Mad Dash. */
void btEffectRunRansack(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BEAR_CLAW);
}

/* Seven strikes land on the first target in the lime palette.

   Abilities: Bacteria Rush. */
void btEffectRunBacteriaRush(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BACTERIA_RUSH);
}

/* Nine strikes land on the first target in the blue palette.

   Abilities: Freebite Rush. */
void btEffectRunFreebiteRush(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_FREEBITE_RUSH);
}

/* One, two or three strikes land on the first target as the variant grows with
   a double or triple blow.

   Abilities: Flying Attack; Swift Strike. */
FLURRY_DEFINE_BY_VARIANT(btEffectRunFlyingAttack, FLURRY_FLYING_ATTACK,
	FLURRY_FLYING_ATTACK_DOUBLE, FLURRY_FLYING_ATTACK_TRIPLE)

/* Three strikes land on the first target in the Mars Djinni's colours.

   Abilities: Ur Flash. */
void btEffectRunUrFlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_UR_FLASH);
}

/* The user lunges further at the first target and strikes once, in the embers'
   colours.

   Abilities: Claw Attack. */
void btEffectRunClawAttack(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_CLAW_ATTACK);
}

/* One strike lands on the first target while glints flicker around it.

   Abilities: Crusher Grip. */
void btEffectRunCrusherGrip(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_CRUSHER_GRIP);
}

/* One strike lands on the first target while the battle ripples and smoke rises
   from its feet.

   Abilities: Vanish Claw. */
void btEffectRunVanishClaw(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_VANISH_CLAW);
}

/* Doublestep plays Gust's one, two or three strikes as the variant grows.

   Abilities: Doublestep. */
FLURRY_DEFINE_BY_VARIANT(
	btEffectRunDoublestep, FLURRY_GUST, FLURRY_GUST_DOUBLE, FLURRY_GUST_TRIPLE)

/* The motes in PARTICLES, waiting above the target; the wave, smoke and orb
   pictures follow them in the map cell buffer. */
#define FLURRY_MOTE_COUNT		 256
/* Each strike's own run of the motes, of which it throws FlurryStyles'
   FLURRY_MOTES. */
#define FLURRY_STRIKE_MOTES		 32
/* The wave pictures, past the motes, where the effect's own pictures begin. */
#define FLURRY_WAVE_PICTURES	 (Ram_MapMetatileBuffer + 9216)
/* The smoke pictures, past the waves: an offset into the effect's pictures. */
#define FLURRY_SMOKE_PICTURES	 5000
/* Where BlueArcSheetA or BlueArcSheetB is decoded in the effect's pictures. */
#define FLURRY_BLUE_ARC_PICTURES 3200
/* The yellow orb's pictures, packed to half width past the smoke: an offset
   into the effect's pictures. */
#define FLURRY_ORB_PICTURES		 11912
/* The level the canvas fills with on each hit, where the style flashes. */
#define FLURRY_FLASH_LEVEL		 47

/* Vanish Claw's smoke: sixteen puffs, each through six pictures four ticks
   apart. */
#define FLURRY_SMOKE_PUFFS		   16
#define FLURRY_SMOKE_PICTURE_TICKS 4
#define FLURRY_SMOKE_TICKS		   (6 * FLURRY_SMOKE_PICTURE_TICKS)

/* The bytes of a FlurryStyles row, one row a mode. */
typedef enum FLURRY_COLUMN {
	FLURRY_ARC_SHEET, /* the arcs (FLURRY_ARC) */
	FLURRY_PALETTE,	  /* whose palette (FLURRY_PAL) */
	FLURRY_STRIKES,
	/* motes thrown by each strike, at most its run of FLURRY_STRIKE_MOTES */
	FLURRY_MOTES,
	FLURRY_INTERVAL, /* ticks between strikes */
	FLURRY_TICKS,	 /* ticks in the whole effect */
	FLURRY_FLASH, /* 1 fills the canvas with FLURRY_FLASH_LEVEL on each hit */
	FLURRY_EXTRA  /* what else plays around the target (FLURRY_EXTRA_KIND) */
} FLURRY_COLUMN;

/* The user closes on the first target and strikes it again and again, each
   strike an arc or a wave with a flash and a spray of motes; the last knocks
   the target back. */
void btEffectRunFlurry(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 actorPos;
	SCREEN_POINT		 tgtPos;
	Sint32				 screen[3];
	Sint32				 otherPoint[3];
	SCREEN_POINT		 pos;
	SCREEN_POINT		 spot;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Uint8				*sheet;
	BATTLE_CAMERA		*camera;
	MOTION_OBJECT		*tgt;
	Sint32				 resource;
	Uint8				*pictures = FLURRY_WAVE_PICTURES;
	Sint32				 row;
	Sint32				 col;
	Sint32				 i;
	Sint32				 j;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sheet = Ram_HeapSlots->battleSheet;
	camera = Ram_HeapSlots->cameraState;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(args->prelude == 1)
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side,
				DJINNI_ANCHOR_OVER_USER),
			screen, otherPoint);

	/* The pictures and the palette. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	/* The orb's pictures, packed to half width: the canvas is shown twice as
	   wide. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, state->sheet, 1, 0);
	for(row = 0; row != YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT; row++) {
		for(col = 0; col != YELLOW_ORB_WIDTH; col++) {
			Sint32 dst = row * YELLOW_ORB_PACKED_WIDTH + col / 2;
			Sint32 src = row * YELLOW_ORB_WIDTH + col;

			pictures[dst + FLURRY_ORB_PICTURES] = state->sheet[src];
		}
	}
	/* The glints' sprite sheet takes the orb's place. */
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
	if(FlurryStyles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS_AND_WAVES)
		btResourceLoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_A,
			state->sheet + FLURRY_BLUE_ARC_PICTURES, 1, 1);
	else
		btResourceLoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B,
			state->sheet + FLURRY_BLUE_ARC_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_WAVE_SHEET, pictures, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_SMOKE_SHEET, pictures + FLURRY_SMOKE_PICTURES, 1, 0);
	switch(FlurryStyles[mode][FLURRY_PALETTE]) {
	case FLURRY_PALETTE_MARS_DJINNI:
		resource = RESOURCE_MARS_DJINN_SHEET;
		break;
	case FLURRY_PALETTE_LIME:
		resource = RESOURCE_LIME_PALETTE;
		break;
	case FLURRY_PALETTE_BLUE_ARC:
		resource = RESOURCE_BLUE_ARC_SHEET_B;
		break;
	default:
		resource = RESOURCE_EMBER_STREAK_SHEET;
		break;
	}
	syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(resource),
		SHEET_PALETTE_BYTES);

	/* The user closes in; Claw Attack lunges further and waits. */
	if(mode == FLURRY_CLAW_ATTACK) {
		btMotionApproachTargetStub(args->user, args->tgts[0], 8, toFIXED(6));
		sySynch(7);
	} else {
		btMotionApproachTargetStub(args->user, args->tgts[0], 4, 0);
	}
	sySynch(1);

	btEffectPosProjectScaledBattler(args->tgts[0], &tgtPos);
	/* Each transfer leaves 3/4 of the last tick behind as an afterimage. */
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	if(FlurryStyles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);

	/* The motes wait 25 pixels above the target, aimed away from the caster;
	   none is out yet. */
	tgt = btObjectGetStub(args->tgts[0])->obj;
	for(i = 0; i != FLURRY_MOTE_COUNT; i++) {
		PARTICLE *mote = &PARTICLES[i];

		SET_POSITION(mote, tgt->x, tgt->y + toFIXED(25), tgt->z);
		SET_VELOCITY(mote, (syRandom() & 255) << 12,
			((syRandom() & 255) - 127) << 12, ((syRandom() & 255) - 127) << 12);
		if(mote->x > 0)
			mote->velX = -mote->velX;
		mote->variant = -1;
	}
	/* The smoke puffs start at the target's feet, one a tick. */
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		PARTICLE *puff = &state->particles[i];

		SET_POSITION(puff, toFIXED(tgtPos.x / 2), toFIXED(88), 0);
		puff->velX = ((syRandom() & 255) - 128) << 9;
		puff->velY = ((syRandom() & 255) - 128) << 9;
		puff->variant = -i;
		puff->velZ = 0;
	}

	for(frm = 0; frm != FlurryStyles[mode][FLURRY_TICKS]; frm++) {
		Sint32 *lines;
		Sint32	strikes;

		/* The ripple's line table: each line of BG2 swings up to six pixels on
		   a sine wave. */
		lines = state->bg2xLines;
		for(i = 0; i != DISPLAY_HEIGHT; i++)
			*lines++ = CANVAS_WAVE_BG2X(
				toFIXED(6) - sySin((frm + i) * (TRIG_TURN / 32)) * 6);
		if(mode == FLURRY_CRUSHER_GRIP && frm == 8)
			sySndPlayStub(SE_RISE_WITH_SPARKS);
		strikes = FlurryStyles[mode][FLURRY_STRIKES];
		btEffectPosProjectScaledBattler(args->user, &actorPos);
		actorPos.x /= 2;

		/* The arcs and waves, mirrored on odd strikes. */
		if(args->side == EFFECT_SIDE_ENEMIES) {
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
		}
		draw[0] = Ram_HeapSlots->ramCode;
		draw[1] = Ram_HeapSlots->ramCodePaired;
		for(i = 0; i != strikes; i++) {
			Sint32 start = i * FlurryStyles[mode][FLURRY_INTERVAL];

			if(frm >= start && frm < start + 6) {
				Sint32 step = frm - start;

				if((i & 3) <= 1 ||
					FlurryStyles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS) {
					/* The arc's six pictures. */
					if(args->side == EFFECT_SIDE_ENEMIES)
						draw[i & 1](canvas,
							state->sheet + FLURRY_BLUE_ARC_PICTURES +
								step * PICTURE_BYTES(BLUE_ARC),
							tgtPos.x / 2 - 16, tgtPos.y - 40, BLUE_ARC_WIDTH,
							BLUE_ARC_HEIGHT);
					else
						draw[i & 1](canvas,
							state->sheet + FLURRY_BLUE_ARC_PICTURES +
								step * PICTURE_BYTES(BLUE_ARC),
							tgtPos.x / 2 - 32, tgtPos.y - 40, BLUE_ARC_WIDTH,
							BLUE_ARC_HEIGHT);
				} else {
					/* The wave's six pictures. */
					if(args->side == EFFECT_SIDE_ENEMIES)
						draw[i & 1](canvas,
							pictures + step * PICTURE_BYTES(WAVE),
							tgtPos.x / 2 - 16, actorPos.y - 8, WAVE_WIDTH,
							WAVE_HEIGHT);
					else
						draw[i & 1](canvas,
							pictures + step * PICTURE_BYTES(WAVE),
							tgtPos.x / 2 - 32, actorPos.y - 8, WAVE_WIDTH,
							WAVE_HEIGHT);
				}
			}
			/* The hit; the last strike knocks the target back. */
			if(frm == start + 2) {
				if(FlurryStyles[mode][FLURRY_FLASH] == 1)
					syWordsFillEntry(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(FLURRY_FLASH_LEVEL));
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
				if(i == strikes - 1) {
					btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 8;
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				} else {
					if(i & 1)
						btMotionApplyKnockbackStub(
							args->tgts[0], KNOCKBACK_JOLT);
					state->shakeFrm = 4;
					sySndPlayStub(SE_HEAVY_IMPACT);
				}
				for(j = 0; j != FlurryStyles[mode][FLURRY_MOTES]; j++)
					PARTICLES[i * FLURRY_STRIKE_MOTES + j].variant =
						(syRandom() & 7) + 15;
			}
			/* The orb's six pictures, two ticks each. */
			if(frm >= start + 2 && frm < start + 14)
				draw[0](canvas,
					pictures + FLURRY_ORB_PICTURES +
						((frm - start - 2) / 2) *
							PICTURE_BYTES(YELLOW_ORB_PACKED),
					tgtPos.x / 2 - YELLOW_ORB_PACKED_WIDTH / 2,
					tgtPos.y - YELLOW_ORB_PACKED_HEIGHT / 2,
					YELLOW_ORB_PACKED_WIDTH, YELLOW_ORB_PACKED_HEIGHT);
		}
		EFFECT_RELEASE_BLITTERS();

		/* The motes, until their life runs out, bouncing off the ground. */
		EFFECT_VIEW_FROM_CAMERA(camera);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		draw[0] = Ram_HeapSlots->ramCode;
		draw[1] = Ram_HeapSlots->ramCodePaired;
		for(i = 0; i != FLURRY_MOTE_COUNT; i++) {
			PARTICLE *mote = &PARTICLES[i];
			Sint32	  size = mote->variant;

			if(size > 0) {
				btEffectPosProjectPoint(&mote->x, &pos);
				pos.x /= 2;
				size >>= 3;
				size++;
				EFFECT_DRAW_MOTE(
					draw[(i / 2) & 1], canvas, sheet, pos.x, pos.y, size);
				btParticleAdvanceWithGravity3D(mote, 60, -toFIXED(1) / 64);
				if(mote->y < toFIXED(8))
					mote->velY = -mote->velY / 2;
				mote->variant--;
			}
		}
		EFFECT_RELEASE_BLITTERS();

		if(frm > 3) {
			/* The glints: three a tick around the target, each with its own
			   random flip, until tick 31. */
			if(FlurryStyles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_GLINTS &&
				frm <= 31) {
				btEffectPosProjectScaledBattler(args->tgts[0], &spot);
				for(i = 0; i != 3; i++) {
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
					Sint32 picture = frm & 3;
					Sint32 radius = (syRandom() & 31) + 4;
					Sint32 x = spot.x / 2 +
						(FIXEDtoINT(sySin(angle) * radius) >> 1) -
						BattleEffectGlintPictureWidths[picture] / 2;
					Sint32 y = spot.y -
						(FIXEDtoINT(syCos(angle) * radius) >> 1) -
						BattleEffectGlintPictureHeights[picture] / 2;
					Uint32 flip = syRandom();
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						FlurryGlintFlips[flip & 3] | COMPOSITE_CLIP_X |
							COMPOSITE_CLIP_Y,
						BLITTER_ADDITIVE);
					(Ram_HeapSlots->ramCodePaired)(canvas,
						state->sheet + BattleEffectGlintPictures[picture], x, y,
						BattleEffectGlintPictureWidths[picture],
						BattleEffectGlintPictureHeights[picture]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
			/* The smoke: the puffs rise from the target's feet. */
			if(FlurryStyles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE) {
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
				draw[0] = Ram_HeapSlots->ramCode;
				for(i = 0; i != FLURRY_SMOKE_PUFFS; i++) {
					PARTICLE *puff = &state->particles[i];

					if(puff->variant >= 0 &&
						puff->variant < FLURRY_SMOKE_TICKS) {
						draw[0](canvas,
							pictures + FLURRY_SMOKE_PICTURES +
								(puff->variant / FLURRY_SMOKE_PICTURE_TICKS) *
									PICTURE_BYTES(SMOKE_PUFF),
							FIXED_INTEGER_PART(puff->x) - SMOKE_PUFF_WIDTH / 2,
							FIXED_INTEGER_PART(puff->y) - SMOKE_PUFF_HEIGHT / 2,
							SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
						btParticleAdvanceWithGravity2D(
							puff, 62, -toFIXED(1) / 16);
					}
					puff->variant++;
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	if(FlurryStyles[mode][FLURRY_EXTRA] == FLURRY_EXTRA_RIPPLE)
		syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FLURRY_DEFINE_BY_VARIANT
#undef FLURRY_MOTE_COUNT
#undef FLURRY_STRIKE_MOTES
#undef FLURRY_WAVE_PICTURES
#undef FLURRY_SMOKE_PICTURES
#undef FLURRY_BLUE_ARC_PICTURES
#undef FLURRY_ORB_PICTURES
#undef FLURRY_FLASH_LEVEL
#undef FLURRY_SMOKE_PUFFS
#undef FLURRY_SMOKE_PICTURE_TICKS
#undef FLURRY_SMOKE_TICKS
