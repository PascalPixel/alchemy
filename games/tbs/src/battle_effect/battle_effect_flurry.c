/* Gust, Flying Attack and the rush strikes. Part of battle_effect.c. */

/* The motes in PARTICLES, waiting above the target. */
#define FLURRY_WAITING_MOTES	768
/* The motes the strikes draw, from the first. */
#define FLURRY_DRAWN_MOTES		512
/* Each strike's run of PARTICLES, from the strike's number times this. */
#define FLURRY_MOTES_PER_STRIKE 32

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

/* The bytes of a FlurryStyles row, one row a mode. */
typedef enum FLURRY_COLUMN {
	FLURRY_ARC_SHEET, /* the arcs (FLURRY_ARC) */
	FLURRY_PALETTE,	  /* whose palette (FLURRY_PAL) */
	FLURRY_STRIKES,
	/* motes thrown by each strike, at most FLURRY_MOTES_PER_STRIKE */
	FLURRY_MOTES,
	FLURRY_INTERVAL, /* ticks between strikes */
	FLURRY_TICKS,	 /* ticks in the whole effect */
	FLURRY_FLASH	 /* 1 fills the canvas with level 47 on each hit */
} FLURRY_COLUMN;

/* The flash pictures, packed to half width after the arcs. */
#define FLURRY_ORB_PICTURES	 20736
/* The wave pictures, in the map grid buffer past the motes. */
#define FLURRY_WAVE_PICTURES (RamMapGridMetatileBuffer + 24064)

/* The user closes on the first target and strikes it again and again, each
   strike an arc or a wave with a flash and a spray of motes; the last knocks
   the target back. */
void btEffectRunFlurry(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 userPos;
	SCREEN_POINT		 tgtPos;
	SCREEN_POINT		 screen;
	Sint32				 screenX;
	Sint32				 screenY;
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Uint8				*sprs;
	BATTLE_CAMERA		*camera;
	ACTOR				*tgt;
	Uint8				*palette;
	Sint32				 paletteId;
	Sint32				 row;
	Sint32				 col;
	Sint32				 i;
	Sint32				 j;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sprs = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(state->args->prelude == 1)
		btEffectRunDjinniEntrance(args, ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG,
			state->args->side, DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);

	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, state->sheet, 1, 0);
	for(row = 0; row != YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT; row++) {
		for(col = 0; col != YELLOW_ORB_WIDTH; col++) {
			Sint32 src = row * YELLOW_ORB_WIDTH + col;
			Sint32 dst =
				row * YELLOW_ORB_PACKED_WIDTH + col / 2 + FLURRY_ORB_PICTURES;

			state->sheet[dst] = state->sheet[src];
		}
	}
	if(FlurryStyles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS_AND_WAVES)
		btResourceLoadAndDecompress(
			RESOURCE_BLUE_ARC_SHEET_A, state->sheet, 1, 1);
	else
		btResourceLoadAndDecompress(
			RESOURCE_BLUE_ARC_SHEET_B, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_WAVE_SHEET, FLURRY_WAVE_PICTURES, 1, 0);
	switch(FlurryStyles[mode][FLURRY_PALETTE]) {
	case FLURRY_PALETTE_MARS_DJINNI:
		paletteId = RESOURCE_MARS_DJINN_SHEET;
		break;
	case FLURRY_PALETTE_LIME:
		paletteId = RESOURCE_LIME_PALETTE;
		break;
	case FLURRY_PALETTE_BLUE_ARC:
		paletteId = RESOURCE_BLUE_ARC_SHEET_B;
		break;
	default:
		paletteId = RESOURCE_EMBER_STREAK_SHEET;
		break;
	}
	palette = syResourceGetTableEntry(paletteId);
	syWordsCopy(BG_PLTT, palette, SHEET_PALETTE_BYTES);
	sySynch(1);

	btProjectionScaledUnit(state->args->tgts[0], &tgtPos);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* Motes wait above the target until their strike releases them. */
	tgt = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0; i != FLURRY_WAITING_MOTES; i++) {
		PARTICLE *mote = &PARTICLES[i];

		SET_POSITION(
			mote, tgt->x.fixed, tgt->y.fixed + toFIXED(25), tgt->z.fixed);
		SET_VELOCITY(mote, (syRandom() & 255) << 12,
			((syRandom() & 255) - 127) << 12, ((syRandom() & 255) - 127) << 12);
		if(mote->x > 0)
			mote->velX = -mote->velX;
		mote->variant = -1; /* ticks left (negative: not out yet) */
	}
	btMotionApproachTargetStub(state->args->user, state->args->tgts[0], 4, 0);

	for(frm = 0; frm != FlurryStyles[mode][FLURRY_TICKS]; frm++) {
		Sint32 strikes = FlurryStyles[mode][FLURRY_STRIKES];

		btProjectionScaledUnit(state->args->user, &userPos);
		userPos.x /= 2;

		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
		}
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		for(i = 0; i != strikes; i++) {
			Sint32 start = i * FlurryStyles[mode][FLURRY_INTERVAL];

			if(frm >= start && frm < start + 6) {
				Sint32 step = frm - start;

				if((i & 3) <= 1 ||
					FlurryStyles[mode][FLURRY_ARC_SHEET] == FLURRY_ARCS) {
					if(state->args->side == EFFECT_SIDE_ENEMIES)
						draw[i & 1](canvas,
							state->sheet + step * PICTURE_BYTES(BLUE_ARC),
							tgtPos.x / 2 - 16, tgtPos.y - 40, BLUE_ARC_WIDTH,
							BLUE_ARC_HEIGHT);
					else
						draw[i & 1](canvas,
							state->sheet + step * PICTURE_BYTES(BLUE_ARC),
							tgtPos.x / 2 - 32, tgtPos.y - 40, BLUE_ARC_WIDTH,
							BLUE_ARC_HEIGHT);
				} else {
					if(state->args->side == EFFECT_SIDE_ENEMIES)
						draw[i & 1](canvas,
							FLURRY_WAVE_PICTURES + step * PICTURE_BYTES(WAVE),
							tgtPos.x / 2 - 16, userPos.y - WAVE_HEIGHT / 2,
							WAVE_WIDTH, WAVE_HEIGHT);
					else
						draw[i & 1](canvas,
							FLURRY_WAVE_PICTURES + step * PICTURE_BYTES(WAVE),
							tgtPos.x / 2 - 32, userPos.y - WAVE_HEIGHT / 2,
							WAVE_WIDTH, WAVE_HEIGHT);
				}
			}
			if(frm == start + 2) {
				if(FlurryStyles[mode][FLURRY_FLASH] == 1)
					syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, 0, 4);
				if(i == strikes - 1) {
					btMotionApplyKnockbackStub(
						state->args->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 8;
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				} else {
					if(i & 1)
						btMotionApplyKnockbackStub(
							state->args->tgts[0], KNOCKBACK_JOLT);
					state->shakeFrm = 4;
					sySndPlayStub(SE_HEAVY_IMPACT);
				}
				for(j = 0; j != FlurryStyles[mode][FLURRY_MOTES]; j++)
					PARTICLES[i * FLURRY_MOTES_PER_STRIKE + j].variant =
						(syRandom() & 7) + 15;
			}
			if(frm >= start + 2 && frm < start + 14)
				draw[0](canvas,
					state->sheet +
						((frm - start - 2) / 2) *
							PICTURE_BYTES(YELLOW_ORB_PACKED) +
						FLURRY_ORB_PICTURES,
					tgtPos.x / 2 - YELLOW_ORB_PACKED_WIDTH / 2,
					tgtPos.y - YELLOW_ORB_PACKED_HEIGHT / 2,
					YELLOW_ORB_PACKED_WIDTH, YELLOW_ORB_PACKED_HEIGHT);
		}
		EFFECT_RELEASE_BLITTERS();

		/* Released motes fly and shrink as they die. */
		EFFECT_VIEW_FROM_CAMERA(camera);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		for(i = 0; i != FLURRY_DRAWN_MOTES; i++) {
			PARTICLE *mote = &PARTICLES[i];

			Sint32 size = mote->variant;

			if(size > 0) {
				btProjectionPoint(&mote->x, &screen);
				screen.x /= 2;
				size >>= 3;
				size++;
				EFFECT_DRAW_MOTE(
					draw[(i / 2) & 1], canvas, sprs, screen.x, screen.y, size);
				btParticleAdvanceWithGravity3D(mote, 60, -toFIXED(1) / 64);
				if(mote->y < toFIXED(8))
					mote->velY = -mote->velY / 2;
				mote->variant--;
			}
		}
		EFFECT_RELEASE_BLITTERS();
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FLURRY_WAITING_MOTES
#undef FLURRY_DRAWN_MOTES
#undef FLURRY_MOTES_PER_STRIKE
#undef FLURRY_ORB_PICTURES
#undef FLURRY_WAVE_PICTURES
#undef FLURRY_DEFINE_BY_VARIANT
