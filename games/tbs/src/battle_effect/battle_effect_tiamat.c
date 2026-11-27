/* Dragon Driver and the Tiamat summon. Part of battle_effect.c. */

/* Who breathes the dragon's fire (btEffectRunDragonFire). */
typedef enum DRAGON_FIRE_MODE {
	/* Tiamat lands beside the targets and breathes the fire, then bursts away
	   (btEffectRunImpactBurst) */
	DRAGON_FIRE_TIAMAT,
	/* no dragon is drawn: the breath starts just in front of the targets, in
	   the LightningBolt colours */
	DRAGON_FIRE_DRAGON_DRIVER
} DRAGON_FIRE_MODE;

/* Fireballs rain on the targets and a blast of fire sweeps across them, in the
   LightningBolt colours.

   Abilities: Dragon Driver, the Fusion Dragon's. */
void btEffectRunDragonDriver(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}

/* Tiamat lands while fireballs rain on the targets, breathes fire across them,
   and bursts away.

   Abilities: Tiamat. */
void btEffectRunTiamat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_TIAMAT);
}

#define DRAGON_FIRE_FRAMES			208
/* The breath of fire, then the flash that ends it. */
#define DRAGON_FIRE_BREATH_FRAME	128
#define DRAGON_FIRE_FLASH_FRAME		160
#define DRAGON_FIRE_FLASH_END_FRAME 176
#define DRAGON_FIRE_FIREBALLS		16 /* falling fireballs */
#define DRAGON_FIRE_FIRST_FIREBALL	24 /* the first in state->particles */
#define DRAGON_FIRE_FIRST_SPARK		40 /* the first spark in state->particles */
#define DRAGON_FIRE_EMBERS			512 /* embers in PARTICLES */
#define DRAGON_FIRE_BREATH			24 /* puffs of breath, state->particles 0 to 23 */
#define DRAGON_FIRE_SPARKS			24 /* sparks, state->particles 40 to 63 */
/* The flames a fireball burns with where it lands, in the sprite sheet. */
#define DRAGON_FIRE_FLAME_PICTURES	12288
/* A puff of breath shows the flash burst's six pictures, four frames each. */
#define DRAGON_FIRE_PUFF_PICTURES	6
#define DRAGON_FIRE_PUFF_FRAMES		4
/* A landed fireball's flame burns through its six pictures, two frames each,
   over and over. */
#define DRAGON_FIRE_BURN_PICTURES	6
#define DRAGON_FIRE_BURN_FRAMES		2
#define DRAGON_FIRE_SPARK_LIFE		48	/* frames a spark flies */
#define DRAGON_FIRE_EMBER_LIFE		256 /* frames an ember rises */
/* A falling fireball is the largest mote. */
#define DRAGON_FIRE_FIREBALL_SIZE	10
/* The sprites Tiamat is drawn from. */
#define TIAMAT_SPRITES				8

/* Tiamat's pieces (state->sprs), each showing its own picture of the sprite
   sheet: piece n shows animation n. As it breathes, its head, horns, jaw and
   throat show the fire-lit pictures of its open mouth. */
typedef enum TIAMAT_PIECE {
	TIAMAT_HEAD = 0,
	TIAMAT_HORNS = 1,
	TIAMAT_JAW = 3,
	TIAMAT_THROAT = 4,
} TIAMAT_PIECE;

/* The fire-lit pictures the pieces show as Tiamat breathes, by animation. */
typedef enum TIAMAT_ANIMATION {
	TIAMAT_HEAD_BREATHING = 8,
	TIAMAT_HORNS_BREATHING,
	TIAMAT_JAW_BREATHING,
	TIAMAT_THROAT_BREATHING,
} TIAMAT_ANIMATION;

/* Fireballs rain on the targets and burn where they land while embers rise from
   the ground; then a blast of fire sweeps across them and the screen flares
   white. */
void btEffectRunDragonFire(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 origin;
	void				*canvas;
	BATTLE_CAMERA		*camera;
	BATTLE_EFFECT_STATE *state;
	BLITTER_PAIR		 draw;
	Sint32				 frame;
	Uint8				*motes;
	POINT2_D			 tiamat;
	Sint32				 direction;
	Uint8				*resource;
	Sint32				 i;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

	resource = syResourceGetTableEntry(RESOURCE_FLASH_BURST_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	resource = syResourceGetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet + DRAGON_FIRE_FLAME_PICTURES);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);
	if(mode == DRAGON_FIRE_DRAGON_DRIVER)
		syWordsCopy(BG_PLTT,
			syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
			SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	if(mode == DRAGON_FIRE_TIAMAT)
		direction = 1;
	else if(state->args->side == EFFECT_SIDE_PARTY)
		direction = -1;
	else
		direction = 1;
	if(mode == DRAGON_FIRE_DRAGON_DRIVER) {
		btProjectionScaledUnit(state->args->user, &origin);
		/* DEAD: the halved x is replaced below; the ROM keeps the halving and
		   its store. */
		origin.x /= 2;
		origin.y = 66;
		if(state->args->side == EFFECT_SIDE_PARTY)
			origin.x = 76;
		else
			origin.x = 44;
	}

	tiamat.x = toFIXED(212);
	tiamat.y = -toFIXED(60);
	EFFECT_FREE_PARTICLES(i, state->particles, ARRAY_COUNT(state->particles));
	for(i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
		PARTICLE *fireball = &state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

		if(direction == 1)
			fireball->x = (syRandom() & 127) + 128;
		else
			fireball->x = (syRandom() & 127) - 128;
		fireball->y = (syRandom() & 7) - 72;
		fireball->variant = -(syRandom() & 31);
	}
	EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
	if(mode == DRAGON_FIRE_TIAMAT) {
		btEffectShowLivingTargets(state->args);
		sySynch(1);
		btEffectCreateSprs(TIAMAT_SPRITES, SPRITE_TIAMAT, 2);
	}

	for(frame = 0; frame != DRAGON_FIRE_FRAMES; frame++) {
		/* A or B skips to the flash. */
		if((PerRepeat & (KEY_A | KEY_B)) && frame > 48 &&
			frame < DRAGON_FIRE_FLASH_FRAME) {
			/* Tiamat's open mouth of the breath and the targets' reel of frame
			   138. */
			if(mode == DRAGON_FIRE_TIAMAT) {
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_HEAD], TIAMAT_HEAD_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_HORNS], TIAMAT_HORNS_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_JAW], TIAMAT_JAW_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_THROAT], TIAMAT_THROAT_BREATHING);
			}
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_RED_TINT,
					POSE_DAMAGE, TINT_KEEP, 0);
				btMotionApplyKnockbackStub(
					state->args->tgts[i], KNOCKBACK_HURL);
			}
			frame = DRAGON_FIRE_FLASH_FRAME;
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frame == 178)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		if(frame == DRAGON_FIRE_BREATH_FRAME)
			state->transferValue = CANVAS_FADE_HALF;
		if(frame == DRAGON_FIRE_FLASH_END_FRAME) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
			resource = syResourceGetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
			syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
		} else if(frame >= DRAGON_FIRE_FLASH_FRAME &&
			frame < DRAGON_FIRE_FLASH_END_FRAME) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
			if(frame > 173)
				state->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
			btPaletteBrightenBgEntries(2, 2, 2);
		}

		/* Embers rise from the ground. */
		if(frame >= 33 && frame < DRAGON_FIRE_FLASH_END_FRAME) {
			Sint32 spawned = 0;
			Sint32 limit = 1;

			if(frame > 103)
				limit = 8;
			for(i = 0; i != DRAGON_FIRE_EMBERS; i++) {
				PARTICLE *ember = &PARTICLES[i];

				if(ember->variant == -1) {
					ember->x = toFIXED((syRandom() & 255) - 32);
					ember->y = toFIXED(112);
					ember->velX =
						((syRandom() & 127) + Tiamat_EmberSpeeds[i & 3]) << 9;
					ember->velY =
						-((syRandom() & 127) + Tiamat_EmberSpeeds[i & 3]) << 11;
					ember->variant = 0;
					spawned++;
					if(spawned == limit)
						break;
				}
			}
		}
		/* Sparks fly up from Tiamat's head. */
		if(frame >= 41 && frame < DRAGON_FIRE_BREATH_FRAME) {
			Sint32 spawned = 0;

			if(frame & 1) {
				for(i = 0; i != DRAGON_FIRE_SPARKS; i++) {
					PARTICLE *spark =
						&state->particles[DRAGON_FIRE_FIRST_SPARK + i];

					if(spark->variant == -1) {
						Sint32 speed = (syRandom() & 255) + 128;
						Sint32 angle = (syRandom() & 0x1fff) + 20000;

						if(mode == DRAGON_FIRE_TIAMAT) {
							spark->x = INTtoFIXED((syRandom() & 7) + 78);
							spark->y = toFIXED(70);
						} else {
							spark->x = toFIXED((syRandom() & 7) + origin.x - 8);
							spark->y = toFIXED(origin.y);
						}
						spark->velX = speed * sySin(angle) >> 9;
						spark->velY = speed * syCos(angle) >> 9;
						spawned++;
						spark->variant = 0;
						if(spawned == 1)
							break;
					}
				}
			}
		}
		if(frame == 48)
			sySndPlayStub(SE_RUMBLE);
		if(frame == DRAGON_FIRE_BREATH_FRAME)
			sySndPlayStub(SE_CRASH);
		/* Breath: puffs of fire thrown across the targets. */
		if(frame > DRAGON_FIRE_BREATH_FRAME &&
			frame < DRAGON_FIRE_FLASH_END_FRAME) {
			Sint32 spawned = 0;

			for(i = 0; i != DRAGON_FIRE_BREATH; i++) {
				PARTICLE *puff = &state->particles[i];

				if(puff->variant == -1) {
					Sint32 speed = (syRandom() & 255) + 128;
					Sint32 angle = (syRandom() & 0x1fff) - 20000;

					if(mode == DRAGON_FIRE_TIAMAT) {
						puff->x = toFIXED(68);
						puff->y = toFIXED(64);
					} else {
						puff->x = toFIXED(origin.x);
						puff->y = toFIXED(origin.y);
					}
					puff->velX = speed * sySin(angle) >> 6;
					puff->velY = speed * syCos(angle) >> 6;
					puff->variant = 0;
					spawned++;
					if(spawned == 1)
						break;
				}
			}
		}

		if(frame < DRAGON_FIRE_FLASH_END_FRAME) {
			for(i = 0; i != DRAGON_FIRE_BREATH; i++) {
				PARTICLE *puff = &state->particles[i];

				if(puff->variant >= 0) {
					draw[0](canvas,
						state->sheet +
							(puff->variant >> 2) * PICTURE_BYTES(FLASH_BURST),
						FIXEDtoINT(puff->x) - FLASH_BURST_WIDTH / 2,
						FIXEDtoINT(puff->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					puff->x += puff->velX * direction;
					puff->y += puff->velY;
					puff->variant++;
					if(puff->variant ==
						DRAGON_FIRE_PUFF_PICTURES * DRAGON_FIRE_PUFF_FRAMES)
						puff->variant = -1;
				}
			}
		}

		for(i = 0; i != DRAGON_FIRE_SPARKS; i++) {
			PARTICLE *spark = &state->particles[DRAGON_FIRE_FIRST_SPARK + i];
			Sint32	  size = 1;

			if(spark->variant >= 0) {
				draw[1](canvas, Tiamat_SparkPixels, FIXEDtoINT(spark->x),
					FIXEDtoINT(spark->y) - size, size, size * 2);
				spark->x += spark->velX * direction;
				spark->y += spark->velY;
				spark->velY += -toFIXED(1) / 64;
				spark->variant++;
				if(spark->variant == DRAGON_FIRE_SPARK_LIFE)
					spark->variant = -1;
			}
		}

		/* Embers rise, then curve away. */
		if(frame < DRAGON_FIRE_FLASH_END_FRAME) {
			for(i = 0; i != DRAGON_FIRE_EMBERS; i++) {
				PARTICLE *ember = &PARTICLES[i];

				if(ember->variant >= 0) {
					Uint8 size = Tiamat_EmberSizes[i & 3];

					draw[0](canvas, motes + BattleEffect_MotePictures[size - 1],
						FIXEDtoINT(ember->x) - size / 2,
						FIXEDtoINT(ember->y) - size, size, size * 2);
					ember->x += ember->velX * direction;
					ember->y += ember->velY;
					if(frame > DRAGON_FIRE_BREATH_FRAME) {
						if(i & 1)
							ember->velX += -toFIXED(1) / 2;
						else
							ember->velX += -toFIXED(1) / 8;
					} else {
						ember->velY += -toFIXED(1) / 64;
					}
					ember->variant++;
					if(ember->variant == DRAGON_FIRE_EMBER_LIFE)
						ember->variant = -1;
				}
			}
		}

		if(frame == DRAGON_FIRE_BREATH_FRAME)
			state->shakeFrm = 48;
		if(mode == DRAGON_FIRE_TIAMAT && frame == 48)
			state->shakeFrm = 8;

		/* Tiamat swoops down. */
		if(frame >= 40 && frame <= 47) {
			tiamat.x += -toFIXED(8);
			tiamat.y += toFIXED(16);
		}
		if(mode == DRAGON_FIRE_TIAMAT) {
			if(frame == DRAGON_FIRE_BREATH_FRAME) {
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_HEAD], TIAMAT_HEAD_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_HORNS], TIAMAT_HORNS_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_JAW], TIAMAT_JAW_BREATHING);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_THROAT], TIAMAT_THROAT_BREATHING);
			}
			if(frame == DRAGON_FIRE_FLASH_END_FRAME) {
				fdSprStartAnimationStub(state->sprs[TIAMAT_HEAD], TIAMAT_HEAD);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_HORNS], TIAMAT_HORNS);
				fdSprStartAnimationStub(state->sprs[TIAMAT_JAW], TIAMAT_JAW);
				fdSprStartAnimationStub(
					state->sprs[TIAMAT_THROAT], TIAMAT_THROAT);
			}
			btEffectPlaceFormationSprs(
				SUMMON_FORMATION_TIAMAT, tiamat.x, tiamat.y);
		}
		if(frame == 138) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_RED_TINT,
					POSE_DAMAGE, TINT_KEEP, 0);
				btMotionApplyKnockbackStub(
					state->args->tgts[i], KNOCKBACK_HURL);
			}
		}

		if(frame < DRAGON_FIRE_FLASH_END_FRAME) {
			for(i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
				PARTICLE *fireball =
					&state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

				if(fireball->y > 55) {
					if(fireball->variant >= 0 &&
						fireball->variant < DRAGON_FIRE_BURN_PICTURES *
								DRAGON_FIRE_BURN_FRAMES) {
						Sint32 picture =
							fireball->variant / DRAGON_FIRE_BURN_FRAMES;

						draw[0](canvas,
							state->sheet + Tiamat_FlamePictures[picture] +
								DRAGON_FIRE_FLAME_PICTURES,
							fireball->x - Tiamat_FlameWidths[picture] / 2,
							fireball->y + Tiamat_FlameDrops[picture],
							Tiamat_FlameWidths[picture],
							Tiamat_FlameHeights[picture]);
					}
					fireball->variant++;
					if(fireball->variant ==
						DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_FRAMES)
						fireball->variant = 0;
				} else if(fireball->variant == 0) {
					Sint32 size = DRAGON_FIRE_FIREBALL_SIZE;

					fireball->x -= direction * 6;
					fireball->y += 6;
					if(frame <= 47 && fireball->y > 55)
						sySndPlayStub(SE_WHOOSH);
					draw[0](canvas, motes + BattleEffect_MotePictures[size - 1],
						fireball->x - size / 2, fireball->y + 30, size,
						size * 2);
				} else {
					fireball->variant++;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	if(mode == DRAGON_FIRE_TIAMAT) {
		btEffectRunImpactBurst(SUMMON_FORMATION_TIAMAT, tiamat.x, tiamat.y);
		for(i = 0; i != TIAMAT_SPRITES; i++)
			fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

#undef DRAGON_FIRE_FRAMES
#undef DRAGON_FIRE_BREATH_FRAME
#undef DRAGON_FIRE_FLASH_FRAME
#undef DRAGON_FIRE_FLASH_END_FRAME
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_PICTURES
#undef DRAGON_FIRE_PUFF_PICTURES
#undef DRAGON_FIRE_PUFF_FRAMES
#undef DRAGON_FIRE_BURN_PICTURES
#undef DRAGON_FIRE_BURN_FRAMES
#undef DRAGON_FIRE_SPARK_LIFE
#undef DRAGON_FIRE_EMBER_LIFE
#undef DRAGON_FIRE_FIREBALL_SIZE
#undef TIAMAT_SPRITES
