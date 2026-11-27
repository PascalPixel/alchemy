/* The Cybele summon. Part of battle_effect.c. */

#define CYBELE_TICKS		366 /* ticks of Cybele's whole run */
/* The first target lands on its spreading ring, sixteen ticks after it is
   dropped; the others follow. */
#define CYBELE_LANDING_TICK 302

/* Where the blast pictures start in state->sheet, after the yellow ring's. */
#define CYBELE_BLAST_PICTURES	 5760
#define CYBELE_VINES			 16 /* vines, from state->particles[0] */
#define CYBELE_SPARKS			 16 /* sparks, from CYBELE_FIRST_SPARK */
#define CYBELE_THORNS			 16 /* thorns, from CYBELE_FIRST_THORN */
#define CYBELE_CHIPS			 48 /* chips, from CYBELE_FIRST_CHIP */
#define CYBELE_FIRST_SPARK		 16 /* the first spark in state->particles */
#define CYBELE_FIRST_THORN		 32 /* the first thorn in state->particles */
#define CYBELE_FIRST_CHIP		 16 /* the first chip in state->particles */
#define CYBELE_DUST_PICTURE		 6	/* the dust cloud in CybeleBlastPictures */
#define CYBELE_SPRITES			 8	/* the sprites Cybele is drawn from */
/* The two sprites of her face (CYBELE_POSE_FACE_LEFT and _RIGHT). */
#define CYBELE_FACE_LEFT_SPRITE	 3
#define CYBELE_FACE_RIGHT_SPRITE 4
/* How far right of the screen's left edge the canvas shows until the chips fly,
   in pixels. */
#define CYBELE_CANVAS_X			 32
/* The order the canvas dissolves in: shuffled runs of the canvas's columns, row
   y taking run y % CYBELE_DISSOLVE_RUNS. */
#define CYBELE_DISSOLVE_RUNS	 8
/* The dissolve clears CYBELE_DISSOLVE_PIXELS pixels a tick from each of the
   CYBELE_DISSOLVE_BAND rows it moves down the canvas. */
#define CYBELE_DISSOLVE_BAND	 32
#define CYBELE_DISSOLVE_PIXELS	 4
/* The vines are drawn for CYBELE_VINE_TICKS ticks from the chips' first,
   growing for the first CYBELE_VINE_GROW_TICKS. */
#define CYBELE_VINE_TICKS		 96
#define CYBELE_VINE_GROW_TICKS	 80
/* The vines drawn, of the CYBELE_VINES set out. */
#define CYBELE_VINES_DRAWN		 10
/* The chips that fly, from the first after the sparks. */
#define CYBELE_FLYING_CHIPS		 32
#define CYBELE_FIRST_FLYING_CHIP (CYBELE_FIRST_SPARK + CYBELE_SPARKS)
/* The sparks round each target, from CYBELE_FIRST_SPARK, flashing from the
   vines' first tick to CYBELE_SPARKS_END ticks after it. */
#define CYBELE_TARGET_SPARKS	 6
#define CYBELE_SPARKS_END		 172
/* Each target's dust puffs, from state->particles[target * CYBELE_TARGET_DUST]:
   set out a fifth of a turn apart (rounded up), the first
   CYBELE_TARGET_DUST_DRAWN drawn until their CYBELE_DUST_LAST_TICK. */
#define CYBELE_TARGET_DUST		 10
#define CYBELE_TARGET_DUST_DRAWN 5
#define CYBELE_DUST_SPREAD		 ((TRIG_TURN + 4) / 5)
#define CYBELE_DUST_LAST_TICK	 26

/* The vine pictures in the thorn vine sprite sheet, below a tip at 0; the
   flowers, thorns and sparks follow. */
#define CYBELE_VINE_WIDTH		24
#define CYBELE_VINE_TIP_HEIGHT	8
#define CYBELE_VINE_STEM_HEIGHT 64
/* YellowRingSheet's ring, drawn as two mirrored halves. */
#define CYBELE_RING_HALF_WIDTH	20
#define CYBELE_RING_HALF_HEIGHT 24

/* Where the vine stems begin in the thorn vine sprite sheet, in bytes. */
typedef enum THORN_VINE_SHEET_PICTURE {
	CYBELE_VINE_STEM = CYBELE_VINE_WIDTH * CYBELE_VINE_TIP_HEIGHT,
	/* a thinner vine: its tip, then its stem */
	CYBELE_THIN_VINE =
		CYBELE_VINE_STEM + CYBELE_VINE_WIDTH * CYBELE_VINE_STEM_HEIGHT
} THORN_VINE_SHEET_PICTURE;

/* Cybele drops in at the right and thorns fly from her mouth; flowering vines
   grow out of the ground, lifting the targets while sparks flash around them;
   then the canvas greys and dissolves pixel by pixel, and the targets drop,
   each landing on a spreading ring in a cloud of dust.

   Abilities: Cybele. */
void btEffectRunCybele(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	Uint8				*canvas;
	Sint32				 frm;
	FIXED				 height;
	FIXED				 speed;
	Sint32				 phase;
	Sint32				 rise;
	Sint32				 i;
	Sint32				 j;
	Uint8				*order;
	Sint32				 point[3];
	SCREEN_POINT		 screen;
	BLITTER_PAIR		 draw;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectCreateSprs(CYBELE_SPRITES, SPRITE_CYBELE, 1);

	order = RamMapGridMetatileBuffer;
	for(i = 0; i != CYBELE_DISSOLVE_RUNS * CANVAS_SIZE; i++)
		order[i] = i & (CANVAS_SIZE - 1);
	for(i = 0; i != CYBELE_DISSOLVE_RUNS; i++) {
		for(j = 0; j != CANVAS_SIZE; j++) {
			Sint32 a = syRandom() & (CANVAS_SIZE - 1);
			Sint32 b = syRandom() & (CANVAS_SIZE - 1);
			Uint8 *to = &order[i * CANVAS_SIZE + b];
			Uint8 *from = &order[i * CANVAS_SIZE + a];
			Uint8  t = *to;

			*to = *from;
			*from = t;
		}
	}

	EFFECT_START_NARROW_CANVAS(0);
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(RESOURCE_THORN_VINE_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	REG_BG2X = -(CYBELE_CANVAS_X << 8);

	for(i = 0; i != CYBELE_VINES; i++) {
		PARTICLE *vine = &state->particles[i];

		if(i <= 4) {
			vine->x = i * 20;
			vine->y = (syRandom() & 7) + 104;
		} else {
			vine->x = i * 20 - 90;
			vine->y = (syRandom() & 7) + 108;
		}
		vine->velY = (syRandom() & 7) + 4;
		vine->variant = (syRandom() & 15) + 16;
	}
	for(i = 0; i != CYBELE_SPARKS; i++) {
		PARTICLE *spark = &state->particles[CYBELE_FIRST_SPARK + i];

		spark->x = (syRandom() & 63) + 32;
		spark->y = (syRandom() & 31) + 64;
		spark->variant = -(syRandom() & 7) - 8;
	}
	for(i = 0; i != CYBELE_THORNS; i++) {
		PARTICLE *thorn = &state->particles[CYBELE_FIRST_THORN + i];

		thorn->x = toFIXED(128);
		thorn->y = toFIXED(64);
		thorn->velX = -((syRandom() & 255) + 200) << 9;
		thorn->velY = 0;
		thorn->variant = 0;
	}
	btEffectShowLivingTargets(state->args);

	height = -toFIXED(64);
	speed = 0;
	for(frm = 0; frm != CYBELE_TICKS; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		/* A or B skips the vines straight to the drop. */
		if((PerRepeat & (KEY_A | KEY_B)) != 0 && frm > 190 && frm <= 285) {
			syWordsClear(canvas, CANVAS_BYTES);
			frm = 286;
		}
		if(frm == 224)
			state->transferMode = CANVAS_COPY;
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* Cybele lands; the screen shakes and the targets flinch. */
		if(frm == 31) {
			state->shakeFrm = 8;
			sySndPlayStub(SE_GATE_MOVE);
			EFFECT_FOR_EACH_TARGET(i, state->args)
				btMotionApplyKnockbackStub(
					state->args->tgts[i], KNOCKBACK_BOUNCE);
		}
		if(frm == 72)
			sySndPlayStub(SE_WHOOSH);
		if(frm == 140)
			sySndPlayStub(SE_SWITCH_MECHANISM);

		/* Cybele settles, and her mouth opens as the thorns fly and shuts
		   again. */
		speed += toFIXED(1) / 4;
		height += speed;
		if(height > toFIXED(64))
			height = toFIXED(64);
		btEffectPlaceFormationSprs(
			SUMMON_FORMATION_CYBELE, toFIXED(128), height);
		if(frm >= 48 && frm <= 96) {
			Sint32 pose = (frm - 48) / 24 % 3;

			fdSprStartAnimationStub(state->sprs[CYBELE_FACE_LEFT_SPRITE],
				CybeleObjectPoses[pose][CYBELE_POSE_FACE_LEFT]);
			fdSprStartAnimationStub(state->sprs[CYBELE_FACE_RIGHT_SPRITE],
				CybeleObjectPoses[pose][CYBELE_POSE_FACE_RIGHT]);
		}

		/* Thorns fly from her mouth to the ground. */
		if(frm >= 72 && frm <= 127) {
			for(i = 0; i != CYBELE_THORNS; i++) {
				PARTICLE *thorn = &state->particles[CYBELE_FIRST_THORN + i];

				if(frm >= i + 72 && thorn->y < toFIXED(104)) {
					Sint32 pictureIdx = (frm + i) / 4 % 5;
					Sint32 x = FIXEDtoINT(thorn->x);
					Sint32 y = FIXEDtoINT(thorn->y);
					Uint8 *picture =
						state->sheet + CybeleThornPictures[pictureIdx];
					Uint32 width = CybeleThornWidths[pictureIdx];
					Uint32 tall;

					x -= width >> 1;
					tall = CybeleThornHeights[pictureIdx];
					y -= tall >> 1;
					draw[0](canvas, picture, x, y, width, tall);
					btParticleAdvanceWithGravity2D(
						thorn, PARTICLE_DAMPING_NONE, toFIXED(1) / 16);
				}
			}
		}
		if(frm == 128) {
			for(i = 0; i != CYBELE_CHIPS; i++) {
				PARTICLE *chip = &state->particles[CYBELE_FIRST_CHIP + i];

				chip->x = toFIXED(syRandom() % 96);
				chip->y = INTtoFIXED((syRandom() & 7) + 88);
				chip->velX = ((syRandom() & 255) - 128) << 11;
				chip->velY = -(syRandom() & 255) << 11;
				chip->variant = -(syRandom() & 15) - 16;
			}
			REG_BG2X = 0;
		}

		/* Grow the vines, shaking the screen while they grow. */
		phase = frm - 128;
		if(phase >= 0 && phase <= CYBELE_VINE_TICKS) {
			rise = phase;
			if(rise > CYBELE_VINE_GROW_TICKS)
				rise = CYBELE_VINE_GROW_TICKS;
			else
				state->shakeFrm = 2;
			for(i = 0; i != CYBELE_VINES_DRAWN; i++) {
				PARTICLE *vine = &state->particles[i];

				if(rise > vine->variant) {
					Uint8 *sheet = state->sheet;
					Sint32 top;
					Sint32 wrap;
					Sint32 k;

					if(i > 5)
						sheet = state->sheet + CYBELE_THIN_VINE;
					top = (rise - vine->variant) * vine->velY;
					/* A vine taller than the battle view has lost its tip off
					   the top; past a stem more, it is wound back a stem, so
					   its stems keep rising. */
					for(wrap = top;
						wrap > BATTLE_VIEW_HEIGHT + CYBELE_VINE_STEM_HEIGHT;
						wrap -= CYBELE_VINE_STEM_HEIGHT)
						;
					if(wrap < BATTLE_VIEW_HEIGHT)
						draw[i & 1](canvas, sheet, vine->x,
							vine->y - wrap - CYBELE_VINE_TIP_HEIGHT,
							CYBELE_VINE_WIDTH, CYBELE_VINE_TIP_HEIGHT);
					for(k = 0; k != 3; k++) {
						Sint32 y = vine->y - wrap + k * CYBELE_VINE_STEM_HEIGHT;
						Sint32 skip = 0;
						Sint32 rows = CYBELE_VINE_STEM_HEIGHT;

						if(y >= -CYBELE_VINE_STEM_HEIGHT) {
							if(y < 0) {
								skip = -y * CYBELE_VINE_WIDTH;
								rows = y + CYBELE_VINE_STEM_HEIGHT;
								y = 0;
							}
							if(y + rows > vine->y)
								rows -= y + rows - vine->y;
							draw[i & 1](canvas, sheet + skip + CYBELE_VINE_STEM,
								vine->x, y, CYBELE_VINE_WIDTH, rows);
						}
					}
					if((i & 1) != 0) {
						Sint32 y = ((vine->y - top) & 127) - 16;
						Sint32 kind = i % 3;
						Sint32 full = CybeleFlowerSizes[kind];
						Sint32 rows = full;

						if(y + rows > vine->y)
							rows -= y + rows - vine->y;
						if(rows > 0)
							draw[i & 1](canvas,
								state->sheet + CybeleFlowerPictures[kind],
								vine->x + 8, y, full, rows);
					}
				}
			}
		}
		/* Chips fly up. */
		if(phase >= 0 && phase < CYBELE_VINE_TICKS) {
			for(i = 0; i != CYBELE_FLYING_CHIPS; i++) {
				PARTICLE *chip =
					&state->particles[CYBELE_FIRST_FLYING_CHIP + i];

				if(chip->variant >= 0) {
					Sint32 pictureIdx = i % 5;

					draw[0](canvas,
						state->sheet + CybeleChipPictures[pictureIdx],
						FIXEDtoINT(chip->x), FIXEDtoINT(chip->y),
						CybeleChipWidths[pictureIdx],
						CybeleChipHeights[pictureIdx]);
					chip->x += chip->velX;
					chip->y += chip->velY;
					chip->velY += toFIXED(1) / 4;
					if((chip->y < 0 || chip->y > toFIXED(120)) && frm <= 159) {
						chip->x = toFIXED(syRandom() % 96);
						chip->y = INTtoFIXED((syRandom() & 7) + 88);
						chip->velX = ((syRandom() & 255) - 128) << 11;
						chip->velY = -(syRandom() & 255) << 11;
					}
				}
				chip->variant++;
			}
		}

		/* The colours fade to grey. */
		if(frm >= 224 && frm <= 247 && (frm & 3) == 0) {
			Uint16 *color = (Uint16 *)BG_PLTT;

			for(i = 0; i != SHEET_PALETTE_COLORS; i++) {
				Sint32 red = RGB_RED(*color);
				Sint32 green = RGB_GREEN(*color);
				Sint32 blue = RGB_BLUE(*color);
				Sint32 grey = (red + green + blue) / 3;

				if(red > grey)
					red--;
				if(red < grey)
					red++;
				if(green > grey)
					green--;
				if(green < grey)
					green++;
				if(blue > grey)
					blue--;
				if(blue < grey)
					blue++;
				*color++ = RGB(red, green, blue);
			}
		}

		/* Six sparks keep flashing around each target's body. */
		if(phase >= 0 && phase <= CYBELE_SPARKS_END) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				ACTOR *actor = btObjectGetStub(state->args->tgts[i])->actor;

				SET_VECTOR(
					point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
				btProjectionPoint(point, &screen);
				for(j = 0; j != CYBELE_TARGET_SPARKS; j++) {
					PARTICLE *spark = &state->particles[CYBELE_FIRST_SPARK +
						i * CYBELE_TARGET_SPARKS + j];

					if(spark->variant == 0) {
						spark->x = screen.x + (syRandom() & 15) - 8;
						spark->y = screen.y + (syRandom() & 15) - 40;
					}
					if(spark->variant >= 0 && spark->variant <= 4) {
						Uint8 *picture =
							state->sheet + CybeleSparkPictures[spark->variant];
						Uint32 size = CybeleSparkSizes[spark->variant];
						Uint32 half = size >> 1;

						draw[0](canvas, picture, spark->x - half,
							spark->y - half, size, size);
					}
					spark->variant++;
					if(frm <= 199 && spark->variant == 5)
						spark->variant = -(syRandom() & 7);
				}
			}
		}

		/* The canvas dissolves pixel by pixel from the top. */
		if(frm > 232) {
			Sint32 row = frm * 2 - 496;

			for(i = 0; i != CYBELE_DISSOLVE_BAND; i++) {
				for(j = 0; j != CYBELE_DISSOLVE_PIXELS; j++) {
					Sint32 y = row + i;

					if(y >= 0 && y < CANVAS_SIZE) {
						Sint32 x = order[(y & (CYBELE_DISSOLVE_RUNS - 1)) *
								CANVAS_SIZE +
							i * CYBELE_DISSOLVE_PIXELS + j];

						canvas[CANVAS_PIXEL_OFFSET(x, y)] = 0;
					}
				}
			}
			row++;
			for(i = 0; i != CYBELE_DISSOLVE_BAND; i++) {
				for(j = 0; j != CYBELE_DISSOLVE_PIXELS; j++) {
					Sint32 y = row + i;

					if(y >= 0 && y < CANVAS_SIZE) {
						Sint32 x = order[(y & (CYBELE_DISSOLVE_RUNS - 1)) *
								CANVAS_SIZE +
							i * CYBELE_DISSOLVE_PIXELS + j];

						canvas[CANVAS_PIXEL_OFFSET(x, y)] = 0;
					}
				}
			}
		}

		/* The vines lift the targets. */
		if(frm >= 161 && frm <= 223) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				if(frm > i * 8 + 160) {
					BATTLE_OBJECT *slot = btObjectGetStub(state->args->tgts[i]);
					ACTOR		  *actor = slot->actor;
					void		  *record;
					Sint32		   n;

					actor->y.fixed += toFIXED(8);
					if(actor->y.fixed > toFIXED(128))
						actor->y.fixed = toFIXED(128);
					actor->gravity = 0;
					for(n = 0;
						(record = btActorGetSprStub(slot->actor, n)) != 0; n++)
						fdSprStartAnimationStub(record, POSE_DAMAGE);
				}
			}
		}
		/* The targets drop. */
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			if(frm == i * 5 + CYBELE_LANDING_TICK - 16) {
				ACTOR *actor = btObjectGetStub(state->args->tgts[i])->actor;

				actor->y.fixed = toFIXED(96);
				actor->gravity = BATTLE_UNIT_GRAVITY;
			}
			if(frm == i * 5 + CYBELE_LANDING_TICK) {
				btEffectTintBattler(
					state->args->tgts[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
				sySndPlayStub(SE_HEAVY_IMPACT);
				state->shakeFrm = 8;
			}
		}

		if(frm == CYBELE_LANDING_TICK) {
			EFFECT_RELEASE_BLITTERS();
			btResourceLoadAndDecompress(
				RESOURCE_YELLOW_RING_SHEET, state->sheet, 1, 0);
			btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET,
				state->sheet + CYBELE_BLAST_PICTURES, 1, 1);
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
			draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
			REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 |
				BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			REG_BG2PA = Q_8_8(0.5);
			REG_BG2X = 0;
			EFFECT_START_TRANSFER(
				state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				ACTOR *actor = btObjectGetStub(state->args->tgts[i])->actor;

				for(j = 0; j != CYBELE_TARGET_DUST; j++) {
					PARTICLE *dust =
						&state->particles[i * CYBELE_TARGET_DUST + j];

					SET_POSITION(
						dust, actor->x.fixed, toFIXED(20), actor->z.fixed);
					SET_VELOCITY(dust, (sySin(j * CYBELE_DUST_SPREAD) * 4),
						(syRandom() & 0x7fff) + toFIXED(1),
						(syCos(j * CYBELE_DUST_SPREAD) * 4));
					dust->variant = 0;
				}
			}
		}
		/* Each target lands on a spreading ring. */
		if(frm >= CYBELE_LANDING_TICK) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				Sint32 start = i * 4 + CYBELE_LANDING_TICK;

				if(frm >= start && frm < i * 4 + CYBELE_LANDING_TICK + 12) {
					Sint32 pictureIdx = (frm - start) / 2;
					ACTOR *actor = btObjectGetStub(state->args->tgts[i])->actor;

					SET_VECTOR(point, actor->x.fixed, 0, actor->z.fixed);
					btProjectionPoint(point, &screen);
					screen.x >>= 1;
					draw[0](canvas,
						state->sheet +
							pictureIdx * PICTURE_BYTES(CYBELE_RING_HALF),
						screen.x - CYBELE_RING_HALF_WIDTH,
						screen.y - CYBELE_RING_HALF_HEIGHT,
						CYBELE_RING_HALF_WIDTH, CYBELE_RING_HALF_HEIGHT);
					draw[1](canvas,
						state->sheet +
							pictureIdx * PICTURE_BYTES(CYBELE_RING_HALF),
						screen.x, screen.y - CYBELE_RING_HALF_HEIGHT,
						CYBELE_RING_HALF_WIDTH, CYBELE_RING_HALF_HEIGHT);
				}
				if(frm >= start + 6) {
					for(j = 0; j != CYBELE_TARGET_DUST_DRAWN; j++) {
						PARTICLE *dust =
							&state->particles[i * CYBELE_TARGET_DUST + j];

						btProjectionPoint(&dust->x, &screen);
						screen.x >>= 1;
						if(dust->variant >= 0 &&
							dust->variant <= CYBELE_DUST_LAST_TICK) {
							Sint32 pictureIdx = CYBELE_DUST_PICTURE;
							Sint32 ofs = CybeleBlastPictures[pictureIdx];
							Uint32 size = CybeleBlastSizes[pictureIdx];

							draw[0](canvas, state->sheet + ofs,
								screen.x - (size >> 1), screen.y - (size >> 1),
								size, size);
						}
						btParticleAdvanceWithGravity2D(
							dust, 60, toFIXED(1) / 16);
						dust->variant++;
					}
				}
			}
		}

		if(frm <= 127)
			btCameraShake(4, 16);
		else if(frm < CYBELE_LANDING_TICK)
			btCameraShake(2, 2);
		else
			btCameraShake(4, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	btEffectRunImpactBurst(SUMMON_FORMATION_CYBELE, toFIXED(128), height);
	for(i = 0; i != CYBELE_SPRITES; i++)
		fdSprDestroyStub(state->sprs[i]);
	btEffectEndCanvasLayer();
}

#undef CYBELE_FIRST_SPARK
#undef CYBELE_FIRST_THORN
#undef CYBELE_FIRST_CHIP
#undef CYBELE_DUST_PICTURE
#undef CYBELE_BLAST_PICTURES
#undef CYBELE_SPRITES
#undef CYBELE_FACE_LEFT_SPRITE
#undef CYBELE_FACE_RIGHT_SPRITE
#undef CYBELE_CANVAS_X
#undef CYBELE_DISSOLVE_RUNS
#undef CYBELE_DISSOLVE_BAND
#undef CYBELE_DISSOLVE_PIXELS
#undef CYBELE_VINE_TICKS
#undef CYBELE_VINE_GROW_TICKS
#undef CYBELE_VINES_DRAWN
#undef CYBELE_FLYING_CHIPS
#undef CYBELE_FIRST_FLYING_CHIP
#undef CYBELE_TARGET_SPARKS
#undef CYBELE_SPARKS_END
#undef CYBELE_TARGET_DUST
#undef CYBELE_TARGET_DUST_DRAWN
#undef CYBELE_DUST_SPREAD
#undef CYBELE_DUST_LAST_TICK
