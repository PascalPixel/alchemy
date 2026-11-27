/* The Ramses summon. Part of battle_effect.c. */

#define RAMSES_TICKS			 320 /* A or B cuts it short */
#define RAMSES_STRIKE_TICK		 260 /* the fist strikes the targets */
#define RAMSES_ROCKS			 6	 /* rocks that fall and bounce */
#define RAMSES_FIRST_PUFF		 6	 /* the first puff in state->particles */
/* the first rock puff in state->particles */
#define RAMSES_FIRST_ROCK_PUFF	 30
#define RAMSES_SPARKS			 512 /* sparks of the strike */
/* The statue's own sprites, its fist's two after them, and the rocks' after
   those. */
#define RAMSES_STATUE_SPRITES	 7
#define RAMSES_FIST_SPRITE		 RAMSES_STATUE_SPRITES
#define RAMSES_FIRST_ROCK_SPRITE (RAMSES_FIST_SPRITE + 2)
/* A puff's life: four pictures, six ticks each; a spent one keeps its variant
   past it. */
#define RAMSES_PUFF_TICKS		 24
#define RAMSES_PUFFS			 56 /* puffs drifted and drawn */
/* The fist's three slams, the first on the tick its circling ends, each
   RAMSES_SLAM_INTERVAL ticks after the last. */
#define RAMSES_SLAMS			 3
#define RAMSES_FIRST_SLAM		 91
#define RAMSES_SLAM_INTERVAL	 40

/* In a desert the giant statue of Ramses pounds the ground with its fist while
   rocks rain down, then launches the fist at the targets, which strikes them in
   a burst of sparks.

   Abilities: Ramses. */
void btEffectRunRamses(BATTLE_EFFECT_PARM *args)
{
	static SPR_SCALE	 unitScale = {{{toFIXED(1), toFIXED(1)}}};
	SPR_PLACEMENT		 pos;
	DRAW_RECTANGLE		 draw[2];
	SPR_SCALE			 scale;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frame;
	Uint8				*motes;
	POINT2_D			 statue;
	POINT2_D			 fist;
	Sint32				 i;
	Sint32				 k;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	BG_PLTT[1] = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);
	/* The statue, its fist and wrist, then six rocks. */
	btEffectCreateSprites(RAMSES_FIRST_ROCK_SPRITE, SPRITE_RAMSES, 2);
	for(i = 0; i != RAMSES_ROCKS; i++) {
		SPR *sprite = fdSpriteCreateStub(SPRITE_BATTLE_ROCKS);

		state->sprites[RAMSES_FIRST_ROCK_SPRITE + i] = sprite;
		if(sprite != 0) {
			sprite->flags = 0;
			fdSpriteStartAnimationStub(sprite, i % 3);
			state->sprites[RAMSES_FIRST_ROCK_SPRITE + i]->image.priority = 1;
		}
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
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

		rock->x = toFIXED(syRandom() & 127);
		rock->y = i * -toFIXED(16);
		rock->velocityX = 0;
		rock->velocityY = 0;
		rock->variant = 0;
	}
	for(i = 0; i != ARRAY_COUNT(state->particles) - RAMSES_FIRST_PUFF; i++)
		state->particles[RAMSES_FIRST_PUFF + i].variant = RAMSES_PUFF_TICKS;
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
	state->fadeFrame = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;

	for(frame = 0; frame != RAMSES_TICKS && !(PerRepeat & (KEY_A | KEY_B));
		frame++) {
		if(frame == 94)
			sySoundPlayStub(SE_SWITCH_MECHANISM);
		if(frame == 136)
			sySoundPlayStub(SE_SWITCH_MECHANISM);
		if(frame == 178)
			sySoundPlayStub(SE_SWITCH_MECHANISM);
		if(frame == RAMSES_STRIKE_TICK)
			sySoundPlayStub(SE_CRASH);
		scale = unitScale;
		if(frame >= 96 && frame <= 251)
			state->shakeFrame = 1;
		else if(frame >= RAMSES_STRIKE_TICK && frame <= 263)
			state->shakeFrame = 1;
		pos.groundHeight = 0;
		pos.height = 0;
		for(i = 0; i != RAMSES_STATUE_SPRITES; i++) {
			pos.x = toFIXED(RamsesStatueColumns[i]) + statue.x - toFIXED(32);
			pos.y = toFIXED(RamsesStatueRows[i]) + statue.y - toFIXED(32);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], &pos, &scale, 0);
		}
		/* The fist circles. */
		if(frame < RAMSES_FIRST_SLAM) {
			fist.x = sySin(frame * (TRIG_TURN / 128)) * 16 + toFIXED(156);
			fist.y = syCos(frame * (TRIG_TURN / 128)) * 16 + toFIXED(92);
		}
		/* Then it slams down three times, throwing up dust. */
		if(frame <= 196) {
			Sint32 start;

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
						puff->velocityX = ((syRandom() & 255) - 127) << 10;
						puff->velocityY = ((syRandom() & 255) - 127) << 10;
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
		   which cancel on the screen but put them in front. */
		if(frame <= 259) {
			pos.x = fist.x;
			pos.height = -toFIXED(256);
			pos.y = fist.y - toFIXED(256);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[RAMSES_FIST_SPRITE], &pos, &scale, 0);
			pos.x = fist.x + toFIXED(32);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[RAMSES_FIST_SPRITE + 1], &pos, &scale, 0);
		}
		pos.height = 0;
		/* The rocks fall. */
		for(i = 0; i != RAMSES_ROCKS; i++) {
			PARTICLE *rock = &state->particles[i];

			if(rock->variant != 2) {
				pos.x = rock->x;
				pos.y = rock->y;
				fdRenderApplyProjectedPlacementStub(
					state->sprites[RAMSES_FIRST_ROCK_SPRITE + i], &pos, &scale,
					0);
				rock->x += rock->velocityX;
				rock->y += rock->velocityY;
				if(frame > 96)
					rock->velocityY += toFIXED(1) / 4;
				if(rock->y > toFIXED(120)) {
					rock->variant++;
					if(rock->variant == 1) {
						rock->velocityY = -rock->velocityY / 2;
						for(k = 0; k != 2; k++) {
							PARTICLE *puff =
								&state->particles[RAMSES_FIRST_ROCK_PUFF +
									i * 2 + k];

							puff->x = rock->x / 2;
							puff->y = rock->y - toFIXED(32);
							puff->velocityX = ((syRandom() & 255) - 127) << 10;
							puff->velocityY = ((syRandom() & 255) - 127) << 10;
							puff->variant = syRandom() & 15;
						}
					} else if(frame <= 199) {
						rock->y = 0;
						rock->velocityY = 0;
						rock->variant = 0;
					}
				}
			}
		}
		/* Puffs of dust drift upwards. */
		for(i = 0; i != RAMSES_PUFFS; i++) {
			PARTICLE *puff = &state->particles[RAMSES_FIRST_PUFF + i];

			if(puff->variant >= 0) {
				if(puff->variant >= 0 && puff->variant < RAMSES_PUFF_TICKS) {
					Sint32 picture = puff->variant / 6 + 3;

					draw[0](canvas, state->sheet + RamsesPuffPictures[picture],
						FIXEDtoINT(puff->x) - RamsesPuffSizes[picture] / 2,
						FIXEDtoINT(puff->y) - RamsesPuffSizes[picture] / 2,
						RamsesPuffSizes[picture], RamsesPuffSizes[picture]);
				}
				btParticleAdvanceWithGravity2D(puff, 60, -toFIXED(1) / 4);
				puff->variant++;
			}
		}
		if(frame == RAMSES_STRIKE_TICK) {
			/* The strike: every target recoils and flashes white. */
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				btMotionApplyKnockbackStub(
					state->args->targets[i], KNOCKBACK_HURL);
				btEffectTintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
					TINT_KEEP, i, 8);
			}
			state->shakeFrame = 8;
		}
		/* DEAD: the strike tick tested a second time; the ROM keeps the second
		   compare. */
		if(frame == RAMSES_STRIKE_TICK) {
			/* Sparks fly from the strike. */
			for(i = 0; i != RAMSES_SPARKS; i++) {
				PARTICLE *spark = &PARTICLES[i];
				Sint32	  speed;
				Sint32	  angle;

				speed = (syRandom() & 0x3ff) + 32;
				angle = syRandom() & TRIG_ANGLE_MASK;
				spark->x = toFIXED(32);
				spark->y = toFIXED(92);
				spark->velocityX = sySin(angle) * speed >> 7;
				spark->velocityY = -(syCos(angle) * speed << 1) >> 7;
				spark->variant = (syRandom() & 15) + 32;
			}
		}
		/* Sparks shrink as they die. */
		for(i = 0; i != RAMSES_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[i];

			if(spark->variant >= 0) {
				Sint32 size = (spark->variant >> 3) + 1;

				EFFECT_DRAW_MOTE(draw[i & 1], canvas, motes,
					FIXEDtoINT(spark->x), FIXEDtoINT(spark->y), size);
				btParticleAdvanceWithGravity2D(spark, 62, toFIXED(1) / 16);
				spark->variant--;
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	btEffectSetupBlendedDisplay();
	for(i = 0; i != RAMSES_FIRST_ROCK_SPRITE + RAMSES_ROCKS; i++)
		fdSpriteDestroyStub(state->sprites[i]);
	EFFECT_FINISH_CANVAS();
}

#undef RAMSES_TICKS
#undef RAMSES_STRIKE_TICK
#undef RAMSES_ROCKS
#undef RAMSES_FIRST_PUFF
#undef RAMSES_FIRST_ROCK_PUFF
#undef RAMSES_SPARKS
#undef RAMSES_STATUE_SPRITES
#undef RAMSES_FIST_SPRITE
#undef RAMSES_FIRST_ROCK_SPRITE
#undef RAMSES_PUFF_TICKS
#undef RAMSES_PUFFS
#undef RAMSES_SLAMS
#undef RAMSES_FIRST_SLAM
#undef RAMSES_SLAM_INTERVAL
