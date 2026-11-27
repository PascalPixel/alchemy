/* @file */
/* The healing motes of Ply, Wish, Cure, Cure Poison, Tonic and Psy Crystal. */
/* Part of battle_effect.c. */

#define HEALING_SPARKS 512 /* sparks in PARTICLES */

/* Where the Djinni's picture is decoded in the state's sprite sheet, in bytes.
 */
#define HEALING_DJINNI_PICTURE 4096
/* How far the Djinni sways to either side, in screen pixels. */
#define HEALING_DJINNI_SWAY 20
/* Each target's motes: HEALING_MOTES for the stronger variant, half that for */
/* the basic and twice for the strongest. A target's motes start HEALING_MOTES
 */
/* after the last target's in PARTICLES, so the strongest variant's run on into
 */
/* the next target's. */
#define HEALING_MOTES 64
/* The frames each target's ring of Cure motes lags the last target's in its */
/* turn. */
#define HEALING_CURE_RING_LAG 40
/* A sparkle or mote is drawn at one of three sizes by its depth, from */
/* HEALING_MOTE_DEPTH_NEAR, one size smaller every HEALING_MOTE_DEPTH_STEP */
/* farther, its depth held below HEALING_MOTE_DEPTH_FAR. */
#define HEALING_MOTE_DEPTH_NEAR 314
#define HEALING_MOTE_DEPTH_STEP 128
#define HEALING_MOTE_DEPTH_FAR	634
/* Eddy's bubble: BlueBeamSheet's first picture, a 24x24 blue bubble, unpacked
 */
/* at HEALING_BUBBLE_SHEET and copied to HEALING_BUBBLE at every second pixel,
 */
/* so that the canvas shown at twice its width draws it round. */
#define HEALING_BUBBLE_SHEET  2048
#define HEALING_BUBBLE		  8192
#define HEALING_BUBBLE_WIDTH  (BLUE_BUBBLE_WIDTH / 2)
#define HEALING_BUBBLE_HEIGHT BLUE_BUBBLE_HEIGHT

/* The healing motes' modes (btEffectRunHealingMotes), each named for the */
/* first ability that plays it. */
typedef enum HEALING_MOTES_MODE {
	HEALING_MODE_PLY,  /* swirling sparkles, the canvas at its own width */
	HEALING_MODE_WISH, /* motes tumbling in from all around */
	HEALING_MODE_CURE, /* a ring of motes below each target */
	HEALING_MODE_CURE_POISON, /* sparkles that go out one by one */
	HEALING_MODE_RESTORE,	  /* the same, turning on a second axis */
	HEALING_MODE_TONIC,		  /* played exactly as HEALING_MODE_RESTORE */
	HEALING_MODE_PSY_CRYSTAL, /* motes in orange */
	HEALING_MODE_SALT,	 /* sparkles thrown up from the feet, turning cyan */
	HEALING_MODE_SPRING, /* Ply's sparkles, twice as many */
	HEALING_MODE_BREATH, /* sparkles whirling fast in Jupiter's colours */
	HEALING_MODE_ETHER,	 /* motes in Jupiter's colours */
	HEALING_MODE_AURA,	 /* sparkles from a wide cloud, not turning */
	HEALING_MODE_EDDY,	 /* blue bubbles */
	HEALING_MODE_REVERSE_STAR /* sparkles turning on the view's axis */
} HEALING_MOTES_MODE;

/* Sparkles of light swirl around each target and close in on it, in the ice */
/* shard's or ice block's colours, with the canvas shown at its own width and */
/* centred on the first target. */
/* */
/* Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water,
 */
/* Pixie, Faery, Weird Nymph, Succubus, Earnest Ply, Fizz. */
void btEffectRunPly(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_PLY);
}

/* Healing motes tumble in on each target from all around it. */
/* */
/* Abilities: Wish, Wish Well, Pure Wish, Mist Potion, Elder Wood, Estre Wood,
 */
/* Regenerate, Reflect, Spritz. */
void btEffectRunWish(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_WISH);
}

/* A ring of healing motes below each target closes in on it, in the blast */
/* sprite sheet's colours. */
/* */
/* Abilities: Cure, Cure Well, Potent Cure, Corn, Flower, Worms. */
void btEffectRunCure(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_CURE);
}

/* After a Mercury Djinni appears, Ply's sparkles, twice as many, swirl in on */
/* each target. */
/* */
/* Abilities: Spring. */
void btEffectRunSpring(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_SPRING);
}

/* After a Jupiter Djinni appears, sparkles in the Jupiter Djinn's colours */
/* whirl fast around each target and close in on it. */
/* */
/* Abilities: Breath. */
void btEffectRunBreath(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_BREATH);
}

/* Sparkles twinkle around each target, four more a frame, and go out one by */
/* one instead of closing in. */
/* */
/* Abilities: Cure Poison, Restore. */
void btEffectRunCurePoison(BATTLE_EFFECT_PARM *args) {
	if(args->variant == EFFECT_VARIANT_BASIC) {
		btEffectRunHealingMotes(args, HEALING_MODE_CURE_POISON);
		return;
	}
	btEffectRunHealingMotes(args, HEALING_MODE_RESTORE);
}

/* Plays the healing motes as Restore shows them; no effect table entry reaches
 */
/* this one. */
UNUSED void btEffectRunUnusedRestoreMotes(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_RESTORE);
}

/* Sparkles twinkle around each target, turning on two axes, and go out one by
 */
/* one; the runner plays HEALING_MODE_TONIC exactly as HEALING_MODE_RESTORE. */
/* */
/* Abilities: Tonic. */
void btEffectRunTonic(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_TONIC);
}

/* After a Venus Djinni appears, sparkles burst up and out from each target's */
/* feet, their colours turning from the blast sprite sheet's to cyan, and go */
/* out one by one. */
/* */
/* Abilities: Salt. */
void btEffectRunSalt(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_SALT);
}

/* Healing motes gather around each target in orange; with a prelude, a Mars */
/* Djinni appears first. */
/* */
/* Abilities: Psy Crystal, Adept Ring, Ember. */
void btEffectRunPsyCrystal(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_PSY_CRYSTAL);
}

/* After a Jupiter Djinni appears, motes in the Jupiter Djinn's colours swirl */
/* in on each target. */
/* */
/* Abilities: Ether. */
void btEffectRunEther(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_ETHER);
}

/* Sparkles from a wide cloud around each target close in on it without */
/* turning, in orange. */
/* */
/* Abilities: Aura, Healing Aura, Cool Aura, Manticore. */
void btEffectRunAura(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_AURA);
}

/* After a Mercury Djinni appears, blue bubbles swirl in on each target. */
/* */
/* Abilities: Eddy. */
void btEffectRunEddy(BATTLE_EFFECT_PARM *args) {
	btEffectRunHealingMotes(args, HEALING_MODE_EDDY);
}

/* Motes of light scattered around each target swirl in towards it. */
/* */
/* Abilities: Reverse Star. */
void btEffectRunReverseStar(BATTLE_EFFECT_PARM *args) {
	args->prelude = 0;
	args->variant = EFFECT_VARIANT_STRONGEST;
	btEffectRunHealingMotes(args, HEALING_MODE_REVERSE_STAR);
}

/* Motes of light scattered around each target swirl in towards it, one target
 */
/* after another, eight frames apart; a healing chime sounds as they gather and
 */
/* the target glows. */
void btEffectRunHealingMotes(BATTLE_EFFECT_PARM *args, u32 mode) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 frameCount;
	s32 member;
	u8 *sprites;
	s32 motesPerTarget;
	s32 canvasShiftX;
	s32 doubleWidth;
	DRAW_RECTANGLE *draw;
	s32 frame;
	s32 i;
	u8 *resource;
	s32 origin[3];
	s32 target[3];
	SCR_POINT base;
	SCR_POINT screen;
	FIXED point[3];
	DRAW_RECTANGLE blit[2];

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	canvasShiftX = 0;
	btEffectStartCanvasLayer(0);
	if(mode == HEALING_MODE_REVERSE_STAR)
		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;

	/* The prelude: the Djinni appears; origin receives where. */
	if(args->prelude == 1) {
		s32 djinni;

		if(mode == HEALING_MODE_PSY_CRYSTAL)
			djinni = ELEMENT_MARS;
		else if(mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
			djinni = ELEMENT_VENUS;
		else if(mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
			djinni = ELEMENT_JUPITER;
		else
			djinni = ELEMENT_MERCURY;
		if(mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_PLY ||
		   mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
		   mode == HEALING_MODE_ETHER)
			btEffectRunDjinniEntrance(
				args,
				DJINNI_ENTRANCE(djinni, args->side, DJINNI_ANCHOR_OVER_TARGET),
				origin, target);
		else
			btEffectRunDjinniEntrance(
				args,
				DJINNI_ENTRANCE(djinni, args->side, DJINNI_ANCHOR_TARGET_X),
				origin, target);
		args->variant = EFFECT_VARIANT_BASIC;
	}

	/* Ply, Spring and Breath show the canvas at its own width, centred on the
	 */
	/* first target; the rest show it at twice its width. */
	if(mode == HEALING_MODE_PLY || mode == HEALING_MODE_SPRING ||
	   mode == HEALING_MODE_BREATH) {
		btEffectPosProjectScaledBattler(args->tgts[0], &base);
		canvasShiftX = CANVAS_SIZE / 2 - base.x;
		REG_BG2X = canvasShiftX << 8;
		REG_BG2PA = Q_8_8(1);
		doubleWidth = 0;
	}
	else {
		doubleWidth = 1;
	}

	/* The pictures, Eddy's bubble narrowed, and the colours. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, state->sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
								state->sheet + HEALING_BUBBLE_SHEET, 1, 0);
	for(i = 0; i != PICTURE_BYTES(HEALING_BUBBLE); i++)
		state->sheet[HEALING_BUBBLE + i] =
			state->sheet[HEALING_BUBBLE_SHEET + i * 2];
	if(mode <= HEALING_MODE_WISH || mode == HEALING_MODE_CURE_POISON ||
	   mode == HEALING_MODE_RESTORE || mode == HEALING_MODE_TONIC ||
	   mode == HEALING_MODE_SPRING || mode == HEALING_MODE_EDDY) {
		if(args->variant == EFFECT_VARIANT_BASIC)
			resource = syResourceGetTableEntry(RESOURCE_ICE_SHARD_SHEET);
		else
			resource = syResourceGetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
	}
	else if(mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER) {
		resource = syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
	}
	else if(mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
			mode == HEALING_MODE_REVERSE_STAR) {
		resource = syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B);
	}
	else {
		resource = syResourceGetTableEntry(RESOURCE_BLAST_SHEET);
	}
	syWordsCopy((void *)BG_PLTT, resource, SHEET_PALETTE_BYTES);

	/* The Djinni: its 40x40 picture on a canvas at its own width, the small */
	/* sprite sheet's 20x40 one on a canvas at twice its width. */
	if(doubleWidth == 0) {
		if(mode == HEALING_MODE_PSY_CRYSTAL)
			resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
		else if(mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
			resource = syResourceGetTableEntry(RESOURCE_VENUS_DJINN_SHEET);
		else if(mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
			resource = syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
		else
			resource = syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SHEET);
	}
	else {
		if(mode == HEALING_MODE_PSY_CRYSTAL || mode == HEALING_MODE_AURA ||
		   mode == HEALING_MODE_REVERSE_STAR)
			resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
		else if(mode == HEALING_MODE_CURE || mode == HEALING_MODE_SALT)
			resource =
				syResourceGetTableEntry(RESOURCE_VENUS_DJINN_SMALL_SHEET);
		else if(mode == HEALING_MODE_BREATH || mode == HEALING_MODE_ETHER)
			resource =
				syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SMALL_SHEET);
		else
			resource =
				syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
	}
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLz(resource, state->sheet + HEALING_DJINNI_PICTURE);
	/* blit[0] keeps the brighter pixel and blit[1] adds; draw points at them.
	 */
	btEffectBuildCanvasBlitters(args->side, draw = blit);

	/* Scatter the motes; the mode decides how, and how long the effect is. */
	if(mode == HEALING_MODE_PLY || mode == HEALING_MODE_PSY_CRYSTAL ||
	   mode == HEALING_MODE_SPRING || mode == HEALING_MODE_BREATH ||
	   mode == HEALING_MODE_ETHER) {
		/* Across and deep 64 pixels each way, 32-95 pixels up. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 127) + 64) << 15,
						 ((syRandom() & 255) - 127) << 15);
			spark->variant = 0;
		}
		if(mode == HEALING_MODE_BREATH)
			frameCount = 54;
		else if(mode == HEALING_MODE_SPRING)
			frameCount = 76;
		else
			frameCount = args->tgtCnt * 8 + 58;
	}
	else if(mode == HEALING_MODE_SALT) {
		/* From the feet, 48 pixels up, thrown out and up. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			spark->y = toFIXED(48);
			spark->x = 0;
			spark->z = 0;
			spark->velX = ((syRandom() & 255) - 127) << 8;
			spark->velY = -((syRandom() & 127) + 64) << 9;
			spark->velZ = ((syRandom() & 255) - 127) << 8;
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}
	else if(mode == HEALING_MODE_EDDY) {
		/* As Ply's. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 127) + 64) << 15,
						 ((syRandom() & 255) - 127) << 15);
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}
	else if(mode == HEALING_MODE_WISH || mode == HEALING_MODE_REVERSE_STAR) {
		/* 64 pixels each way. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 255) - 127) << 15);
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}
	else if(mode == HEALING_MODE_AURA) {
		/* A wide cloud, 100 pixels each way. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, (syRandom() % 200 - 100) << 15,
						 (syRandom() % 200 - 100) << 15,
						 (syRandom() % 200 - 100) << 15);
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 46;
	}
	else if(mode == HEALING_MODE_CURE) {
		/* A ring 32-95 pixels across, 50 pixels down, rising. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];
			s32 heading = syRandom() & TRIG_ANGLE_MASK;
			s32 radius = (syRandom() & 63) + 32;

			SET_POSITION(spark, sySin(heading) * radius, -toFIXED(50),
						 syCos(heading) * radius);
			spark->velY = ((syRandom() & 31) + 32) << 13;
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}
	else if(mode == HEALING_MODE_CURE_POISON) {
		/* 64 pixels across and deep, 32 up and down. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 255) - 127) << 14,
						 ((syRandom() & 255) - 127) << 15);
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}
	else {
		/* 64 pixels each way. */
		for(i = 0; i != HEALING_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			SET_POSITION(spark, ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 255) - 127) << 15,
						 ((syRandom() & 255) - 127) << 15);
			spark->variant = 0;
		}
		frameCount = args->tgtCnt * 8 + 58;
	}

	/* Motes a target: half as many for the basic variant and twice for the */
	/* strongest; Spring always takes twice and Eddy a quarter. */
	motesPerTarget = HEALING_MOTES;
	if(args->variant == EFFECT_VARIANT_BASIC)
		motesPerTarget = HEALING_MOTES / 2;
	else if(args->variant == EFFECT_VARIANT_STRONGEST)
		motesPerTarget = HEALING_MOTES * 2;
	if(mode == HEALING_MODE_SPRING)
		motesPerTarget = HEALING_MOTES * 2;
	if(mode == HEALING_MODE_EDDY)
		motesPerTarget = HEALING_MOTES / 4;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != frameCount; frame++) {
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;

		if(frame == 40)
			EFFECT_SHOW_DAMAGE(0);
		if(mode == HEALING_MODE_SALT && (frame & 3) == 0)
			btEffectStepPaletteToResource(RESOURCE_CYAN_PALETTE);

		/* The Djinni sways, then from frame 32 rises two pixels a frame; for
		 * its */
		/* first four frames it is added over itself as well. */
		if(args->prelude == 1) {
			if(doubleWidth == 0) {
				s32 x = FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) *
								   HEALING_DJINNI_SWAY) +
						origin[0] + canvasShiftX - DJINNI_WIDTH / 2;
				s32 y = FIXEDtoINT(syCos(frame * (TRIG_TURN / 32)) * 4) +
						origin[1] - 24;

				if(frame > 32)
					y = y - frame * 2 + 64;
				blit[0](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
						DJINNI_WIDTH, DJINNI_HEIGHT);
				if(frame <= 3)
					draw[1](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
							DJINNI_WIDTH, DJINNI_HEIGHT);
			}
			else {
				s32 x = FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) *
								   (HEALING_DJINNI_SWAY / 2)) +
						origin[0] / 2 - SMALL_DJINNI_WIDTH / 2;
				s32 y = FIXEDtoINT(syCos(frame * (TRIG_TURN / 32)) * 4) +
						origin[1] - 24;

				if(frame > 32)
					y = y - frame * 2 + 64;
				blit[0](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
						SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				if(frame <= 3)
					draw[1](canvas, state->sheet + HEALING_DJINNI_PICTURE, x, y,
							SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			}
		}

		for(member = 0; member != args->tgtCnt; member++) {
			MOTION_OBJECT *object = btObjectGetStub(args->tgts[member])->obj;
			s32 start = member * 8;

			/* The motes turn about a point 40 pixels above the target. */
			syInitMatrix();
			syMatrixSetLookAt(camera->eye, camera->focus);
			point[0] = object->x;
			point[1] = toFIXED(40);
			point[2] = object->z;
			syMatrixTranslateVector(point);
			if(frame == start + 20)
				sySndPlayStub(SE_RECOVERY);
			if(mode == HEALING_MODE_SPRING) {
				if(frame == start + 41)
					btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
										TINT_KEEP, member, 32);
			}
			else if(mode == HEALING_MODE_AURA) {
				if(frame == start + 24)
					btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
										TINT_KEEP, member, 28);
			}
			else if(frame == start + 36) {
				btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
									TINT_KEEP, member, 28);
			}
			if(frame > start) {
				if(mode == HEALING_MODE_PLY ||
				   mode == HEALING_MODE_PSY_CRYSTAL ||
				   mode == HEALING_MODE_SALT || mode == HEALING_MODE_SPRING ||
				   mode == HEALING_MODE_ETHER || mode == HEALING_MODE_EDDY) {
					syRotY((frame - start) * (TRIG_TURN / 128));
				}
				else if(mode == HEALING_MODE_BREATH) {
					syRotY((frame - start) * (TRIG_TURN / 32));
				}
				else if(mode == HEALING_MODE_AURA) {
					/* Aura's motes close in without turning. */
				}
				else if(mode == HEALING_MODE_REVERSE_STAR) {
					syRotZ(frame * (TRIG_TURN / 128));
				}
				else if(mode == HEALING_MODE_WISH) {
					syRotX(frame * (TRIG_TURN / 128));
					syRotZ(frame * (TRIG_TURN / 128));
				}
				else if(mode == HEALING_MODE_CURE ||
						mode == HEALING_MODE_EDDY) {
					syRotY((frame - member * HEALING_CURE_RING_LAG) *
						   (TRIG_TURN / 128));
				}
				else if(mode == HEALING_MODE_CURE_POISON) {
					syRotY((frame - start) * (TRIG_TURN / 128));
				}
				else {
					syRotY((frame - start) * (TRIG_TURN / 128));
					syRotX((frame - start) * (TRIG_TURN / 128));
				}

				/* Four motes appear each frame; Cure Poison's, Restore's and
				 * Tonic's */
				/* go out one by one from 32 frames on, Salt's from 64. */
				for(i = 0; i != motesPerTarget; i++) {
					PARTICLE *spark = &PARTICLES[member * HEALING_MOTES + i];
					s32 endFrame;

					if(mode == HEALING_MODE_CURE_POISON ||
					   mode == HEALING_MODE_RESTORE ||
					   mode == HEALING_MODE_TONIC)
						endFrame = start + i / 2 + 32;
					else if(mode == HEALING_MODE_SALT)
						endFrame = start + i / 2 + 64;
					else
						endFrame = 65536;
					if(frame > start + i / 4 && frame < endFrame) {
						s32 xx = (spark->x >> 8) * (spark->x >> 8);
						s32 yy = (spark->y >> 8) * (spark->y >> 8);
						s32 zz = (spark->z >> 8) * (spark->z >> 8);
						s32 distance = sySquart(xx + yy + zz) >> 9;

						/* Measured at three quarters, so the motes close in a
						 * third */
						/* faster than 1 / distance of the way. */
						distance = distance * 48 / 64;
						if(distance != 0) {
							s32 size;

							btEffectPosProjectPoint((s32 *)spark, &screen);
							if(mode == HEALING_MODE_PLY ||
							   mode == HEALING_MODE_SPRING ||
							   mode == HEALING_MODE_BREATH)
								screen.x += canvasShiftX;
							else
								screen.x >>= 1;
							screen.y += 16;

							/* Aura's and Reverse Star's sizes run one larger;
							 * Ply's, */
							/* Spring's, Breath's and Salt's sparkles turn
							 * through the nine */
							/* sparkle pictures instead. */
							CLAMP(screen.depth, HEALING_MOTE_DEPTH_NEAR,
								  HEALING_MOTE_DEPTH_FAR);
							size =
								3 - (screen.depth - HEALING_MOTE_DEPTH_NEAR) /
										HEALING_MOTE_DEPTH_STEP;
							if(mode == HEALING_MODE_PLY ||
							   mode == HEALING_MODE_SPRING ||
							   mode == HEALING_MODE_BREATH)
								size = (i * 4 + frame) % 9;
							else if(mode == HEALING_MODE_AURA ||
									mode == HEALING_MODE_REVERSE_STAR)
								size = 4 - (screen.depth -
											HEALING_MOTE_DEPTH_NEAR) /
											   HEALING_MOTE_DEPTH_STEP;
							else if(mode == HEALING_MODE_SALT)
								size = (i * 4 + frame) % 9;
							if(mode == HEALING_MODE_PLY ||
							   mode == HEALING_MODE_CURE_POISON ||
							   mode == HEALING_MODE_RESTORE ||
							   mode == HEALING_MODE_TONIC ||
							   mode == HEALING_MODE_SALT ||
							   mode == HEALING_MODE_SPRING ||
							   mode == HEALING_MODE_BREATH ||
							   mode == HEALING_MODE_AURA ||
							   mode == HEALING_MODE_REVERSE_STAR) {
								u8 *cell = state->sheet +
										   BattleEffect_SparklePictures[size];
								u32 edge = BattleEffect_SparkleSizes[size];
								u32 half = edge >> 1;

								draw[1](canvas, cell, screen.x - half,
										screen.y - half, edge, edge);
							}
							else if(mode == HEALING_MODE_EDDY) {
								blit[0](canvas, state->sheet + HEALING_BUBBLE,
										screen.x - HEALING_BUBBLE_WIDTH / 2,
										screen.y - HEALING_BUBBLE_HEIGHT / 2,
										HEALING_BUBBLE_WIDTH,
										HEALING_BUBBLE_HEIGHT);
							}
							else {
								EFFECT_DRAW_MOTE(draw[1], canvas, sprites,
												 screen.x, screen.y, size);
							}

							/* Close in by 1 / distance of the way a frame. Cure
							 * Poison's, */
							/* Restore's and Tonic's stay where they are until
							 * they go out, */
							/* and Salt's fly on. */
							if(mode <= HEALING_MODE_CURE ||
							   mode == HEALING_MODE_PSY_CRYSTAL ||
							   mode == HEALING_MODE_SPRING ||
							   mode == HEALING_MODE_BREATH ||
							   mode == HEALING_MODE_ETHER ||
							   mode == HEALING_MODE_AURA ||
							   mode == HEALING_MODE_REVERSE_STAR ||
							   mode == HEALING_MODE_EDDY) {
								spark->x -= spark->x / distance;
								spark->y -= spark->y / distance;
								spark->z -= spark->z / distance;
							}
							if(mode == HEALING_MODE_SALT) {
								spark->x += spark->velX;
								spark->y += spark->velY;
								spark->z += spark->velZ;
							}
						}
					}
				}
			}
		}
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS();
}
