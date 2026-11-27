/* Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt). Part of
   battle_effect.c. */

/* Packs the six yellow orb pictures to half width, keeping every second byte.
   Its rows are always the orbs' own; rowCnt is not read. */
void btEffectPackYellowOrbs(
	Uint8 *src, Uint8 *dst, Sint32 stride, Sint32 rowCnt)
{
	Sint32 row = 0;
	Sint32 rows = YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT;
	Sint32 ofs = 0;

	for(; row != rows; row++) {
		Sint32 x;

		for(x = 0; x != YELLOW_ORB_WIDTH; x++) {
			dst[ofs / 2 + x / 2] = src[ofs + x];
		}
		ofs += stride;
	}
}

#define RUSHING_STRIKE_SPARK_COUNT 64
/* The first pair of sparks' ticks of life; each pair after lives a tick
   longer. */
#define RUSHING_STRIKE_SPARK_TICKS 16
/* One ring picture: a yellow orb packed to half width, in the map grid
   buffer. */
#define RUSHING_STRIKE_RING_BYTES  PICTURE_BYTES(YELLOW_ORB_PACKED)

/* The rushing strike's sparks' palettes (btEffectRunRushingStrike), each named
   for the effect that plays it. */
typedef enum RUSHING_STRIKE_MODE {
	RUSHING_STRIKE_BONE_CHARGE, /* the fire pillar's */
	RUSHING_STRIKE_BRUTE_FORCE, /* the ice block's */
	RUSHING_STRIKE_ONSLAUGHT,	/* the pink burst's */
	RUSHING_STRIKE_HEADBUTT		/* the blast's */
} RUSHING_STRIKE_MODE;

/* Rushes the target and scatters sparks in the fire pillar palette.

   Abilities: Bone Charge. */
void btEffectRunBoneCharge(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BONE_CHARGE);
}

/* Rushes the target and scatters sparks in the ice-block palette.

   Abilities: Brute Force. */
void btEffectRunBruteForce(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}

/* Rushes the target and scatters sparks in the pink-burst palette.

   Abilities: Onslaught. */
void btEffectRunOnslaught(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}

/* Rushes the target and scatters sparks in the blast palette.

   Abilities: Headbutt. */
void btEffectRunHeadbutt(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}

/* BlueBurstSheet's one picture, the top half of a burst: drawn as it is and
   mirrored top to bottom beneath, a 40x64 burst that follows the user in. */
#define BLUE_BURST_WIDTH		   40
#define BLUE_BURST_HEIGHT		   32
#define RUSHING_STRIKE_TICKS	   60
#define RUSHING_STRIKE_BURST_TICKS 15 /* the burst follows the user in */
#define RUSHING_STRIKE_RING_TICK   8  /* the ring flares at the target */
#define RUSHING_STRIKE_RING_TICKS  12 /* its pictures, two ticks each */
#define RUSHING_STRIKE_TICK		   10 /* the user strikes */

/* The user rushes at the first target; a ring flares where it hits and sparks
   scatter. */
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	BATTLE_CAMERA		*camera;
	Uint8				*motes;
	PARTICLE			*spark;
	PARTICLE			*newSpark;
	SCREEN_POINT		 origin;
	SCREEN_POINT		 screen;
	SCREEN_POINT		 sparkScreen;
	ACTOR				*tgt;
	Sint32				 frm;
	Sint32				 i;
	Sint32				 size;
	Sint32				 ringFrm;
	Sint32				 paletteId;
	Uint8				*rings;

	rings = RamMapGridMetatileBuffer;
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);

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

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, state->sheet, 1, 0);
	btEffectPackYellowOrbs(state->sheet, (Uint8 *)rings, YELLOW_ORB_WIDTH,
		YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT);
	btResourceLoadAndDecompress(RESOURCE_BLUE_BURST_SHEET, state->sheet, 1, 1);
	switch(mode) {
	case RUSHING_STRIKE_BONE_CHARGE:
		paletteId = RESOURCE_FIRE_PILLAR_SHEET_B;
		break;
	case RUSHING_STRIKE_BRUTE_FORCE:
		paletteId = RESOURCE_ICE_BLOCK_SHEET;
		break;
	case RUSHING_STRIKE_ONSLAUGHT:
		paletteId = RESOURCE_PINK_BURST_SHEET;
		break;
	default:
		paletteId = RESOURCE_BLAST_SHEET;
		break;
	}
	syWordsCopy(
		BG_PLTT, syResourceGetTableEntry(paletteId), SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* Send the user in, and ready the sparks at the target. */
	btEffectSetApproachMotion(state->args->user, state->args->tgts[0], 10);
	tgt = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
		newSpark = &state->particles[i];
		newSpark->x = tgt->x.fixed;
		newSpark->y = tgt->y.fixed + toFIXED(10);
		newSpark->z = tgt->z.fixed;
		SET_VELOCITY(newSpark, (syRandom() & 0x1ff) << 11,
			((syRandom() & 255) - 64) << 11, ((syRandom() & 255) - 128) << 11);
		if(newSpark->x > 0)
			newSpark->velX = -newSpark->velX;
		newSpark->variant = i / 2 + RUSHING_STRIKE_SPARK_TICKS;
	}
	btProjectionUnit(state->args->tgts[0], &origin);

	for(frm = 0; frm != RUSHING_STRIKE_TICKS; frm++) {
		/* The burst at the user as it rushes in. */
		if(frm < RUSHING_STRIKE_BURST_TICKS) {
			btProjectionUnit(state->args->user, &screen);
			draw[0](canvas, state->sheet, screen.x / 2 - 16,
				screen.y - 16 - BLUE_BURST_HEIGHT, BLUE_BURST_WIDTH,
				BLUE_BURST_HEIGHT);
			draw[1](canvas, state->sheet, screen.x / 2 - 16, screen.y - 16,
				BLUE_BURST_WIDTH, BLUE_BURST_HEIGHT);
		}
		if(frm == RUSHING_STRIKE_TICK) {
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_HURL);
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			state->shakeFrm = 8;
		}
		/* The ring where it hits. */
		ringFrm = frm - RUSHING_STRIKE_RING_TICK;
		if(ringFrm >= 0 && ringFrm < RUSHING_STRIKE_RING_TICKS) {
			Sint32 ring = ringFrm / 2;
			draw[0](canvas, rings + ring * RUSHING_STRIKE_RING_BYTES,
				origin.x / 2 - 16, screen.y - 40, YELLOW_ORB_PACKED_WIDTH,
				YELLOW_ORB_PACKED_HEIGHT);
		}
		/* The sparks fly and shrink. */
		if(ringFrm >= 0 && ringFrm <= 55) {
			EFFECT_VIEW_FROM_CAMERA(camera);
			for(i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
				spark = &state->particles[i];
				size = spark->variant;
				if(size > 0) {
					btProjectionPoint(&spark->x, &sparkScreen);
					size >>= 4;
					size += 2;
					sparkScreen.x >>= 1;
					EFFECT_DRAW_MOTE(draw[0], canvas, motes, sparkScreen.x,
						sparkScreen.y, size);
					btParticleAdvanceWithGravity3D(
						spark, 60, -toFIXED(1) / 128);
					spark->variant--;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_SPARK_TICKS
#undef RUSHING_STRIKE_BURST_TICKS
#undef RUSHING_STRIKE_RING_TICK
#undef RUSHING_STRIKE_RING_TICKS
#undef RUSHING_STRIKE_RING_BYTES
#undef RUSHING_STRIKE_TICKS
#undef RUSHING_STRIKE_TICK
#undef BLUE_BURST_WIDTH
#undef BLUE_BURST_HEIGHT
