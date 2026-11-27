/* @file */
/* Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt). */
/* Part of battle_effect.c. */

/* Packs the six yellow orb pictures to half width, keeping every second */
/* byte. Its rows are always the orbs' own; rowCount is not read. */
void btEffectPackYellowOrbs(u8 *src, u8 *dst, s32 stride, s32 rowCount) {
	s32 row = 0;
	s32 rows = YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT;
	s32 offset = 0;

	for(; row != rows; row++) {
		s32 x;

		for(x = 0; x != YELLOW_ORB_WIDTH; x++) {
			dst[offset / 2 + x / 2] = src[offset + x];
		}
		offset += stride;
	}
}

#define RUSHING_STRIKE_SPARK_COUNT 64
/* The first pair of sparks' frames of life; each pair after lives a frame */
/* longer. */
#define RUSHING_STRIKE_SPARK_FRAMES 16
/* One ring picture: a yellow orb packed to half width, in the map grid */
/* buffer. */
#define RUSHING_STRIKE_RING_BYTES PICTURE_BYTES(YELLOW_ORB_PACKED)

/* The rushing strike's sparks' palettes (btEffectRunRushingStrike), each */
/* named for the effect that plays it. */
typedef enum RUSHING_STRIKE_MODE {
	RUSHING_STRIKE_BONE_CHARGE, /* the fire pillar's */
	RUSHING_STRIKE_BRUTE_FORCE, /* the ice block's */
	RUSHING_STRIKE_ONSLAUGHT,	/* the pink burst's */
	RUSHING_STRIKE_HEADBUTT		/* the blast's */
} RUSHING_STRIKE_MODE;

/* Rushes the target and scatters sparks in the fire pillar palette. */
/* */
/* Abilities: Bone Charge. */
void btEffectRunBoneCharge(BATTLE_EFFECT_PARM *args) {
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BONE_CHARGE);
}
/* Rushes the target and scatters sparks in the ice-block palette. */
/* */
/* Abilities: Brute Force. */
void btEffectRunBruteForce(BATTLE_EFFECT_PARM *args) {
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}
/* Rushes the target and scatters sparks in the pink-burst palette. */
/* */
/* Abilities: Onslaught. */
void btEffectRunOnslaught(BATTLE_EFFECT_PARM *args) {
	btEffectRunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}
/* Rushes the target and scatters sparks in the blast palette. */
/* */
/* Abilities: Headbutt. */
void btEffectRunHeadbutt(BATTLE_EFFECT_PARM *args) {
	btEffectRunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}
/* BlueBurstSheet's one picture, the top half of a burst: drawn as it is and */
/* mirrored top to bottom beneath, a 40x64 burst that follows the user in. */
#define BLUE_BURST_WIDTH			40
#define BLUE_BURST_HEIGHT			32
#define RUSHING_STRIKE_FRAMES		60
#define RUSHING_STRIKE_BURST_FRAMES 15 /* the burst follows the user in */
#define RUSHING_STRIKE_RING_FRAME	8  /* the ring flares at the target */
#define RUSHING_STRIKE_RING_FRAMES	12 /* its pictures, two frames each */
#define RUSHING_STRIKE_FRAME		10 /* the user strikes */

/* The user rushes at the first target; a ring flares where it hits and sparks
 */
/* scatter. */
void btEffectRunRushingStrike(BATTLE_EFFECT_PARM *args, s32 mode) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	BLITTER_PAIR draw;
	BATTLE_CAMERA *camera;
	u8 *motes;
	PARTICLE *spark;
	PARTICLE *newSpark;
	SCR_POINT origin;
	SCR_POINT screen;
	SCR_POINT sparkScreen;
	ACTOR *target;
	s32 frame;
	s32 i;
	s32 size;
	s32 ringFrame;
	s32 paletteId;
	u8 *rings;

	rings = Ram_MapGridCellBuffer;
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);

	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
							COMPOSITE_LIGHTEN);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
							COMPOSITE_LIGHTEN);
	}
	else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
							COMPOSITE_LIGHTEN);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
							COMPOSITE_LIGHTEN);
	}
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, state->sheet, 1, 0);
	btEffectPackYellowOrbs(state->sheet, (u8 *)rings, YELLOW_ORB_WIDTH,
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
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(paletteId),
				SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	/* Send the user in, and ready the sparks at the target. */
	btEffectSetApproachMotion(state->args->user, state->args->tgts[0], 10);
	target = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
		newSpark = &state->particles[i];
		newSpark->x = target->x.fixed;
		newSpark->y = target->y.fixed + toFIXED(10);
		newSpark->z = target->z.fixed;
		SET_VELOCITY(newSpark, (syRandom() & 0x1ff) << 11,
					 ((syRandom() & 255) - 64) << 11,
					 ((syRandom() & 255) - 128) << 11);
		if(newSpark->x > 0)
			newSpark->velX = -newSpark->velX;
		newSpark->variant = i / 2 + RUSHING_STRIKE_SPARK_FRAMES;
	}
	btProjectionUnit(state->args->tgts[0], &origin);

	for(frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
		/* The burst at the user as it rushes in. */
		if(frame < RUSHING_STRIKE_BURST_FRAMES) {
			btProjectionUnit(state->args->user, &screen);
			draw[0](canvas, state->sheet, screen.x / 2 - 16,
					screen.y - 16 - BLUE_BURST_HEIGHT, BLUE_BURST_WIDTH,
					BLUE_BURST_HEIGHT);
			draw[1](canvas, state->sheet, screen.x / 2 - 16, screen.y - 16,
					BLUE_BURST_WIDTH, BLUE_BURST_HEIGHT);
		}
		if(frame == RUSHING_STRIKE_FRAME) {
			btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
								POSE_DAMAGE, 0, 8);
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_HURL);
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			state->shakeFrm = 8;
		}
		/* The ring where it hits. */
		ringFrame = frame - RUSHING_STRIKE_RING_FRAME;
		if(ringFrame >= 0 && ringFrame < RUSHING_STRIKE_RING_FRAMES) {
			s32 ring = ringFrame / 2;
			draw[0](canvas, rings + ring * RUSHING_STRIKE_RING_BYTES,
					origin.x / 2 - 16, screen.y - 40, YELLOW_ORB_PACKED_WIDTH,
					YELLOW_ORB_PACKED_HEIGHT);
		}
		/* The sparks fly and shrink. */
		if(ringFrame >= 0 && ringFrame <= 55) {
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
					btParticleAdvanceWithGravity3D(spark, 60,
												   -toFIXED(1) / 128);
					spark->variant--;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_SPARK_FRAMES
#undef RUSHING_STRIKE_BURST_FRAMES
#undef RUSHING_STRIKE_RING_FRAME
#undef RUSHING_STRIKE_RING_FRAMES
#undef RUSHING_STRIKE_RING_BYTES
#undef RUSHING_STRIKE_FRAMES
#undef RUSHING_STRIKE_FRAME
#undef BLUE_BURST_WIDTH
#undef BLUE_BURST_HEIGHT
