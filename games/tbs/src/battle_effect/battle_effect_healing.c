/* The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal.
   Part of battle_effect.c. */

#define HEALING_SPARKS 512 /* sparks in PARTICLES */

/* Healing sparkles come in seven modes of one runner, the wrappers naming the
   mode of each effect. Each mote keeps its place relative to its target, so the
   swirl is one turn of the whole group. */

/* the Djinni picture in the sprite sheet */
#define HEALING_DJINNI_PICTURE	4096
/* How far the Djinni sways to either side, in screen pixels. */
#define HEALING_DJINNI_SWAY		20
/* Each target's motes: HEALING_MOTES for the stronger variant, half that for
   the basic and twice for the strongest. A target's motes start HEALING_MOTES
   after the last target's in PARTICLES, so the strongest variant's run on into
   the next target's. */
#define HEALING_MOTES			64
/* The ticks each target's ring of Cure motes lags the last target's in its
   turn. */
#define HEALING_CURE_RING_LAG	40
/* A sparkle or mote is drawn at one of three sizes by its depth, from
   HEALING_MOTE_DEPTH_NEAR, one size smaller every HEALING_MOTE_DEPTH_STEP
   farther, its depth held below HEALING_MOTE_DEPTH_FAR. */
#define HEALING_MOTE_DEPTH_NEAR 314
#define HEALING_MOTE_DEPTH_STEP 128
#define HEALING_MOTE_DEPTH_FAR	634

/* The healing motes' modes (btEffectRunHealingMotes), each named for the first
   ability that plays it. */
typedef enum HEALING_MOTES_MODE {
	HEALING_MODE_PLY,  /* swirling sparkles, the canvas at its own width */
	HEALING_MODE_WISH, /* motes tumbling in from all around */
	HEALING_MODE_CURE, /* a ring of motes below each target */
	HEALING_MODE_CURE_POISON, /* sparkles that go out one by one */
	HEALING_MODE_RESTORE,	  /* the same, turning on a second axis */
	HEALING_MODE_TONIC,		  /* played exactly as HEALING_MODE_RESTORE */
	HEALING_MODE_PSY_CRYSTAL  /* motes in the Mars Djinni palette */
} HEALING_MOTES_MODE;

#include "battle_effect/effect_healing_motes.c"

#include "battle_effect/effect_healing_cure_poison.c"

#include "battle_effect/effect_psy_crystal.c"

/* Motes of light scattered around each target swirl in towards it, one target
   after another. */
void btEffectRunHealingMotes(BATTLE_EFFECT_PARM *args, Uint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frmCnt;
	Sint32				 member;
	Uint8				*sprs;
	Sint32				 motesPerTgt;
	Sint32				 canvasShiftX;
	Sint32				 doubleWidth;
	Sint32				 frm;
	Sint32				 i;
	Uint8				*resource;
	SCREEN_POINT		 base;
	SCREEN_POINT		 screen;
	Sint32				 point[3];
	Sint32				 originX;
	Sint32				 originY;
	BLITTER_PAIR		 blit;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sprs = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	canvasShiftX = 0;
	state->args = args;
	btEffectStartCanvasLayer(0);

	if(state->args->prelude == 1) {
		Sint32 element =
			mode == HEALING_MODE_PSY_CRYSTAL ? ELEMENT_MARS : ELEMENT_MERCURY;

		if(mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_PLY)
			btEffectRunDjinniEntrance(args, element, state->args->side,
				DJINNI_ANCHOR_OVER_TARGET, &originX, &originY);
		else
			btEffectRunDjinniEntrance(args, element, state->args->side,
				DJINNI_ANCHOR_TARGET_X, &originX, &originY);
		state->args->variant = EFFECT_VARIANT_BASIC;
	}

	if(mode == HEALING_MODE_PLY) {
		btProjectionScaledUnit(state->args->tgts[0], &base);
		canvasShiftX = CANVAS_SIZE / 2 - base.x;
		REG_BG2X = canvasShiftX << 8;
		REG_BG2PA = Q_8_8(1);
		doubleWidth = 0;
	} else {
		doubleWidth = 1;
	}

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, state->sheet, 0, 0);
	if(mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
		mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC) {
		if(state->args->variant == EFFECT_VARIANT_BASIC)
			resource = syResourceGetTableEntry(RESOURCE_ICE_SHARD_SHEET);
		else
			resource = syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
	} else if(mode == HEALING_MODE_PSY_CRYSTAL) {
		resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	} else {
		resource = syResourceGetTableEntry(RESOURCE_BLAST_SHEET);
	}
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);

	if(doubleWidth == 0) {
		if(mode == HEALING_MODE_PSY_CRYSTAL)
			resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
		else
			resource = syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
	} else {
		if(mode == HEALING_MODE_PSY_CRYSTAL)
			resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
		else
			resource =
				syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
	}
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet + HEALING_DJINNI_PICTURE);
	btEffectBuildCanvasBlitters(state->args->side, blit);

	/* Scatter the motes around each target's centre. */
	if(mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL) {
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0x7f) + 64) << 15,
				((syRandom() & 0xff) - 127) << 15);
			spark->variant = 0;
		}
		frmCnt = state->args->tgtCnt * 8 + 88;
	} else if(mode == HEALING_MODE_WISH) {
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 15);
			spark->variant = 0;
		}
		frmCnt = state->args->tgtCnt * 8 + 88;
	} else if(mode == HEALING_MODE_CURE) {
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];
			Sint32	  heading = syRandom() & TRIG_ANGLE_MASK;
			Sint32	  radius = (syRandom() & 63) + 32;

			SET_POSITION(spark, sySin(heading) * radius, -toFIXED(50),
				syCos(heading) * radius);
			spark->velY = ((syRandom() & 31) + 32) << 13;
			spark->variant = 0;
		}
		frmCnt = state->args->tgtCnt * 8 + 88;
	} else if(mode == HEALING_MODE_CURE_POISON) {
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 14,
				((syRandom() & 0xff) - 127) << 15);
			spark->variant = 0;
		}
		frmCnt = state->args->tgtCnt * 8 + 72;
	} else {
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 15,
				((syRandom() & 0xff) - 127) << 15);
			spark->variant = 0;
		}
		frmCnt = state->args->tgtCnt * 8 + 72;
	}

	motesPerTgt = HEALING_MOTES;
	if(state->args->variant == EFFECT_VARIANT_BASIC)
		motesPerTgt = HEALING_MOTES / 2;
	else if(state->args->variant == EFFECT_VARIANT_STRONGEST)
		motesPerTgt = HEALING_MOTES * 2;
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	for(frm = 0; frm != frmCnt; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		if(frm == 40)
			EFFECT_SHOW_DAMAGE(0);

		/* The Djinni sways, then rises away. */
		if(state->args->prelude == 1) {
			if(doubleWidth == 0) {
				Sint32 x = FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) *
							   HEALING_DJINNI_SWAY) +
					originX + canvasShiftX - DJINNI_WIDTH / 2;
				Sint32 y = FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 4) +
					originY - 24;

				if(frm > 32)
					y = y - frm * 2 + 64;
				blit[0](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
					DJINNI_WIDTH, DJINNI_HEIGHT);
				if(frm <= 3)
					blit[1](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
						DJINNI_WIDTH, DJINNI_HEIGHT);
			} else {
				Sint32 x = FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) *
							   (HEALING_DJINNI_SWAY / 2)) +
					originX / 2 - SMALL_DJINNI_WIDTH / 2;
				Sint32 y = FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 4) +
					originY - 24;

				if(frm > 32)
					y = y - frm * 2 + 64;
				blit[0](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				if(frm <= 3)
					blit[1](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
						SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			}
		}

		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor = btObjectGetStub(state->args->tgts[member])->actor;
			Sint32 start = member * 8;

			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, actor->x.fixed, toFIXED(40), actor->z.fixed);
			syMatrixTranslateVector(point);
			if(frm == start + 20)
				sySndPlayStub(SE_RECOVERY);
			if(frm == start + 36)
				btEffectTintBattler(state->args->tgts[member],
					SPRITE_COLOR_WHITE, TINT_KEEP, member, 28);

			/* Each target's motes swirl round it. */
			if(frm > start) {
				if(mode == HEALING_MODE_PLY ||
					mode == HEALING_MODE_PSY_CRYSTAL) {
					syRotY((frm - start) * (TRIG_TURN / 128));
				} else if(mode == HEALING_MODE_WISH) {
					syRotX(frm * (TRIG_TURN / 128));
					syRotZ(frm * (TRIG_TURN / 128));
				} else if(mode == HEALING_MODE_CURE) {
					syRotY((frm - member * HEALING_CURE_RING_LAG) *
						(TRIG_TURN / 128));
				} else if(mode == HEALING_MODE_CURE_POISON) {
					syRotY((frm - start) * (TRIG_TURN / 128));
				} else {
					syRotY((frm - start) * (TRIG_TURN / 128));
					syRotX((frm - start) * (TRIG_TURN / 128));
				}

				/* Four motes appear each tick. */
				for(i = 0; i != motesPerTgt; i++) {
					PARTICLE *spark = &PARTICLES[member * HEALING_MOTES + i];
					Sint32	  endFrm;

					if(mode == HEALING_MODE_CURE_POISON ||
						mode == HEALING_MODE_RESTORE ||
						mode == HEALING_MODE_TONIC)
						endFrm = start + i / 2 + 32;
					else
						endFrm = 65536;
					if(frm > start + i / 4 && frm < endFrm) {
						Sint32 xx = (spark->x >> 8) * (spark->x >> 8);
						Sint32 yy = (spark->y >> 8) * (spark->y >> 8);
						Sint32 zz = (spark->z >> 8) * (spark->z >> 8);
						Sint32 distance = sySquartIWRAM(xx + yy + zz) >> 9;

						if(distance != 0) {
							Sint32 size;

							btProjectionPoint(spark, &screen);
							if(mode == HEALING_MODE_PLY)
								screen.x += canvasShiftX;
							else
								screen.x >>= 1;
							screen.y += 16;

							CLAMP(screen.depth, HEALING_MOTE_DEPTH_NEAR,
								HEALING_MOTE_DEPTH_FAR);
							size = 3 -
								(screen.depth - HEALING_MOTE_DEPTH_NEAR) /
									HEALING_MOTE_DEPTH_STEP;
							if(mode == HEALING_MODE_PLY)
								size = (i * 4 + frm) % 9;
							if(mode == HEALING_MODE_PLY ||
								mode == HEALING_MODE_CURE_POISON ||
								mode == HEALING_MODE_RESTORE ||
								mode == HEALING_MODE_TONIC) {
								Uint8 *picture = state->sheet +
									BattleEffectSparklePictures[size];
								Uint32 edge = BattleEffectSparkleSizes[size];
								Uint32 half = edge >> 1;

								blit[1](canvas, picture, screen.x - half,
									screen.y - half, edge, edge);
							} else {
								EFFECT_DRAW_MOTE(blit[1], canvas, sprs,
									screen.x, screen.y, size);
							}

							/* Ply, Wish, Cure and Psy Crystal draw the motes
							   in; the others leave them where they are until
							   they go out. */
							if(mode <= HEALING_MODE_CURE ||
								mode == HEALING_MODE_PSY_CRYSTAL) {
								spark->x -= spark->x / distance;
								spark->y -= spark->y / distance;
								spark->z -= spark->z / distance;
							}
						}
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef HEALING_DJINNI_PICTURE
#undef HEALING_DJINNI_SWAY
#undef HEALING_MOTES
#undef HEALING_CURE_RING_LAG
#undef HEALING_MOTE_DEPTH_NEAR
#undef HEALING_MOTE_DEPTH_STEP
#undef HEALING_MOTE_DEPTH_FAR
