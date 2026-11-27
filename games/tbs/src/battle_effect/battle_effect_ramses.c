/* @file */
/* The Ramses summon. */
/* Part of battle_effect.c. */

#define RAMSES_FRAMES		320 /* A or B cuts it short */
#define RAMSES_STRIKE_FRAME 260 /* the fist strikes the targets */
#define RAMSES_ROCKS		6	/* rocks that fall and bounce */
#define RAMSES_FIRST_PUFF	6	/* the first puff in state->particles */
#define RAMSES_FIRST_ROCK_PUFF                                                 \
	30					  /* the first rock puff in state->particles           \
						   */
#define RAMSES_SPARKS 512 /* sparks of the strike */
/* The statue's own sprites, its fist's two after them, and the rocks' after */
/* those. */
#define RAMSES_STATUE_SPRITES	 7
#define RAMSES_FIST_SPRITE		 RAMSES_STATUE_SPRITES
#define RAMSES_FIRST_ROCK_SPRITE (RAMSES_FIST_SPRITE + 2)
/* A puff's life: four pictures, six frames each; a spent one keeps its */
/* variant past it. */
#define RAMSES_PUFF_FRAMES 24
#define RAMSES_PUFFS	   56 /* puffs drifted and drawn */
/* The fist's three slams, the first on the frame its circling ends, each */
/* RAMSES_SLAM_INTERVAL frames after the last. */
#define RAMSES_SLAMS		 3
#define RAMSES_FIRST_SLAM	 91
#define RAMSES_SLAM_INTERVAL 40

/* In a desert the giant statue of Ramses pounds the ground with its fist while
 */
/* rocks rain down, then launches the fist at the targets, which strikes them */
/* in a burst of sparks. */
/* */
/* Abilities: Ramses. */
void btEffectRunRamses(BATTLE_EFFECT_PARM *args) {
	static SPR_SCALE unitScale = {{{FIXED_ONE, FIXED_ONE}}};
	SPR_PLACEMENT position;
	DRAW_RECTANGLE draw[2];
	SPR_SCALE scale;
	void *canvas;
	BATTLE_EFFECT_STATE *state;
	s32 frame;
	u8 *motes;
	POINT2_D statue;
	POINT2_D fist;
	s32 i;
	s32 k;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT[1] = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);
	/* The statue, its fist and wrist, then six rocks. */
	btEffectCreateSprs(RAMSES_FIRST_ROCK_SPRITE, SPRITE_RAMSES, 2);
	for(i = 0; i != RAMSES_ROCKS; i++) {
		SPR *sprite = fdSprCreateStub(SPRITE_BATTLE_ROCKS);

		state->sprs[RAMSES_FIRST_ROCK_SPRITE + i] = sprite;
		if(sprite != 0) {
			sprite->flg = 0;
			fdSprStartAnimationStub(sprite, i % 3);
			state->sprs[RAMSES_FIRST_ROCK_SPRITE + i]->image.pri = 1;
		}
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
				WININ_WIN1_BG2 | WININ_WIN1_CLR;
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
	sySynch(1);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_DESERT_BACKDROP, 0);
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 1);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	statue.x = toFIXED(188);
	statue.y = toFIXED(92);
	fist.x = toFIXED(160);
	fist.y = toFIXED(92);
	for(i = 0; i != RAMSES_ROCKS; i++) {
		PARTICLE *rock = &state->particles[i];

		rock->x = INTtoFIXED(syRandom() & 127);
		rock->y = i * -toFIXED(16);
		rock->velX = 0;
		rock->velY = 0;
		rock->variant = 0;
	}
	for(i = 0; i != ARRAY_COUNT(state->particles) - RAMSES_FIRST_PUFF; i++)
		state->particles[RAMSES_FIRST_PUFF + i].variant = RAMSES_PUFF_FRAMES;
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
	state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
	state->fadeStep = 0;

	for(frame = 0; frame != RAMSES_FRAMES && !(KeysRepeat & (KEY_A | KEY_B));
		frame++) {
		if(frame == 94)
			sySndPlayStub(SE_SWITCH_MECHANISM);
		if(frame == 136)
			sySndPlayStub(SE_SWITCH_MECHANISM);
		if(frame == 178)
			sySndPlayStub(SE_SWITCH_MECHANISM);
		if(frame == RAMSES_STRIKE_FRAME)
			sySndPlayStub(SE_CRASH);
		scale = unitScale;
		if(frame >= 96 && frame <= 251)
			state->shakeFrm = 1;
		else if(frame >= RAMSES_STRIKE_FRAME && frame <= 263)
			state->shakeFrm = 1;
		position.groundHeight = 0;
		position.height = 0;
		for(i = 0; i != RAMSES_STATUE_SPRITES; i++) {
			position.x =
				INTtoFIXED(Ramses_StatueColumns[i]) + statue.x - toFIXED(32);
			position.y =
				INTtoFIXED(Ramses_StatueRows[i]) + statue.y - toFIXED(32);
			fdRenderApplyProjectedPlacementStub(state->sprs[i], &position,
												&scale, 0);
		}
		/* The fist circles. */
		if(frame < RAMSES_FIRST_SLAM) {
			fist.x = sySin(frame * (TRIG_TURN / 128)) * 16 + toFIXED(156);
			fist.y = syCos(frame * (TRIG_TURN / 128)) * 16 + toFIXED(92);
		}
		/* Then it slams down three times, throwing up dust. */
		if(frame <= 196) {
			s32 start;

			for(i = 0, start = RAMSES_FIRST_SLAM; i != RAMSES_SLAMS;
				start += RAMSES_SLAM_INTERVAL, i++) {
				if(frame >= start && frame < start + 4)
					fist.y += toFIXED(8);
				if(frame == start + 3) {
					for(k = 0; k != 4; k++) {
						PARTICLE *puff =
							&state->particles[RAMSES_FIRST_PUFF + i * 8 + k];

						puff->x = toFIXED(64);
						puff->y = toFIXED(96);
						puff->velX = ((syRandom() & 255) - 127) << 10;
						puff->velY = ((syRandom() & 255) - 127) << 10;
						puff->variant = syRandom() & 15;
					}
				}
				if(frame >= start + 20 && frame < start + 36)
					fist.y -= toFIXED(2);
			}
		}
		/* Then it flies off to the left at the targets. */
		if(frame >= 244 && frame <= 251)
			fist.x -= toFIXED(1);
		if(frame >= 252 && frame <= 275)
			fist.x -= INTtoFIXED(frame - 250);
		/* The fist's two sprites stand deep below the ground and as far up,
		 * which */
		/* cancel on the screen but put them in front. */
		if(frame <= 259) {
			position.x = fist.x;
			position.height = -toFIXED(256);
			position.y = fist.y - toFIXED(256);
			fdRenderApplyProjectedPlacementStub(state->sprs[RAMSES_FIST_SPRITE],
												&position, &scale, 0);
			position.x = fist.x + toFIXED(32);
			fdRenderApplyProjectedPlacementStub(
				state->sprs[RAMSES_FIST_SPRITE + 1], &position, &scale, 0);
		}
		position.height = 0;
		/* The rocks fall. */
		for(i = 0; i != RAMSES_ROCKS; i++) {
			PARTICLE *rock = &state->particles[i];

			if(rock->variant != 2) {
				position.x = rock->x;
				position.y = rock->y;
				fdRenderApplyProjectedPlacementStub(
					state->sprs[RAMSES_FIRST_ROCK_SPRITE + i], &position,
					&scale, 0);
				rock->x += rock->velX;
				rock->y += rock->velY;
				if(frame > 96)
					rock->velY += toFIXED(1) / 4;
				if(rock->y > toFIXED(120)) {
					rock->variant++;
					if(rock->variant == 1) {
						rock->velY = -rock->velY / 2;
						for(k = 0; k != 2; k++) {
							PARTICLE *puff =
								&state->particles[RAMSES_FIRST_ROCK_PUFF +
												  i * 2 + k];

							puff->x = rock->x / 2;
							puff->y = rock->y - toFIXED(32);
							puff->velX = ((syRandom() & 255) - 127) << 10;
							puff->velY = ((syRandom() & 255) - 127) << 10;
							puff->variant = syRandom() & 15;
						}
					}
					else if(frame <= 199) {
						rock->y = 0;
						rock->velY = 0;
						rock->variant = 0;
					}
				}
			}
		}
		/* Puffs of dust drift upwards. */
		for(i = 0; i != RAMSES_PUFFS; i++) {
			PARTICLE *puff = &state->particles[RAMSES_FIRST_PUFF + i];

			if(puff->variant >= 0) {
				if(puff->variant >= 0 && puff->variant < RAMSES_PUFF_FRAMES) {
					s32 picture = puff->variant / 6 + 3;

					draw[0](canvas, state->sheet + Ramses_PuffPictures[picture],
							FIXEDtoINT(puff->x) - Ramses_PuffSizes[picture] / 2,
							FIXEDtoINT(puff->y) - Ramses_PuffSizes[picture] / 2,
							Ramses_PuffSizes[picture],
							Ramses_PuffSizes[picture]);
				}
				btParticleAdvanceWithGravity2D(puff, 60, -toFIXED(1) / 4);
				puff->variant++;
			}
		}
		if(frame == RAMSES_STRIKE_FRAME) {
			/* The strike: every target recoils and flashes white. */
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				btMotionApplyKnockbackStub(state->args->tgts[i],
										   KNOCKBACK_HURL);
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_WHITE,
									TINT_KEEP, i, 8);
			}
			state->shakeFrm = 8;
		}
		/* DEAD: the strike frame tested a second time; the ROM keeps the second
		 */
		/* compare. */
		if(frame == RAMSES_STRIKE_FRAME) {
			/* Sparks fly from the strike. */
			for(i = 0; i != RAMSES_SPARKS; i++) {
				PARTICLE *spark = &PARTICLES[i];
				s32 speed;
				s32 angle;

				speed = (syRandom() & 0x3ff) + 32;
				angle = syRandom() & TRIG_ANGLE_MASK;
				spark->x = toFIXED(32);
				spark->y = toFIXED(92);
				spark->velX = sySin(angle) * speed >> 7;
				spark->velY = -(syCos(angle) * speed << 1) >> 7;
				spark->variant = (syRandom() & 15) + 32;
			}
		}
		/* Sparks shrink as they die. */
		for(i = 0; i != RAMSES_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			if(spark->variant >= 0) {
				s32 size = (spark->variant >> 3) + 1;

				EFFECT_DRAW_MOTE(draw[i & 1], canvas, motes,
								 FIXEDtoINT(spark->x), FIXEDtoINT(spark->y),
								 size);
				btParticleAdvanceWithGravity2D(spark, 62, toFIXED(1) / 16);
				spark->variant--;
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}

	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	btEffectSetupBlendedDisp();
	for(i = 0; i != RAMSES_FIRST_ROCK_SPRITE + RAMSES_ROCKS; i++)
		fdSprDestroyStub(state->sprs[i]);
	EFFECT_FINISH_CANVAS();
}

#undef RAMSES_FRAMES
#undef RAMSES_STRIKE_FRAME
#undef RAMSES_ROCKS
#undef RAMSES_FIRST_PUFF
#undef RAMSES_FIRST_ROCK_PUFF
#undef RAMSES_SPARKS
#undef RAMSES_STATUE_SPRITES
#undef RAMSES_FIST_SPRITE
#undef RAMSES_FIRST_ROCK_SPRITE
#undef RAMSES_PUFF_FRAMES
#undef RAMSES_PUFFS
#undef RAMSES_SLAMS
#undef RAMSES_FIRST_SLAM
#undef RAMSES_SLAM_INTERVAL
