/* Ragnarok. Part of battle_effect.c. */

/* FlameBladeSheet's blade, then FlashBurstSheet's pictures. */
#define RAGNAROK_BLADE_WIDTH	  64
#define RAGNAROK_BLADE_HEIGHT	  128
#define RAGNAROK_FLASH_PICTURES	  PICTURE_BYTES(RAGNAROK_BLADE)
/* The blade comes down to RAGNAROK_BLADE_HOLD_TOP, and is cut off at the canvas
   row RAGNAROK_BLADE_FLOOR as it sinks into the ground. */
#define RAGNAROK_BLADE_HOLD_TOP	  16
#define RAGNAROK_BLADE_FLOOR	  104
/* A flash waits out its life, the ring's inner ones least, and in its last
   RAGNAROK_FLASH_BURN_TICKS burns down through six pictures, three ticks
   each. */
#define RAGNAROK_FLASH_LIFE		  25
#define RAGNAROK_FLASH_BURN_TICKS 18
/* The canvas row the sparks bounce on. */
#define RAGNAROK_GROUND_Y		  108

#define RAGNAROK_COLUMNS	  2 /* columns that strike */
#define RAGNAROK_SEEDED		  3 /* columns given particles; the third is unused */
#define RAGNAROK_FLASHES	  16  /* flashes kept per column */
#define RAGNAROK_RING		  12  /* of which drawn as the ring */
#define RAGNAROK_SPARKS		  340 /* sparks kept per column */
#define RAGNAROK_DRAWN_SPARKS 256 /* of which flown */

#define RAGNAROK_TICKS			   80
#define RAGNAROK_BLADE_SWING_TICKS 10 /* the blade comes down */
#define RAGNAROK_BLADE_HOLD_TICK   20 /* then holds, then creeps on */
#define RAGNAROK_BLADE_LAST_TICK   39 /* the blade is gone after it */
#define RAGNAROK_FIRST_STRIKE_TICK \
	16							/* each column strikes 8 ticks after the last */
#define RAGNAROK_DAMAGE_TICK 40 /* the damage is shown */

/* A huge flaming blade swings down over the targets and light bursts over them
   twice.

   Abilities: Ragnarok. */
void btEffectRunRagnarok(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 column;
	Uint8				*motes;
	ACTOR				*user;
	DRAW_RECTANGLE		 blit[2];
	Sint32				 frm;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);

	user = btObjectGetStub(args->user)->actor;
	/* Nothing in the effect or the battle reads the variant afterwards. XXX (no
	   evidence): why it is forced to the stronger one; the store changes
	   nothing the game shows, so only the author's intent would say. */
	args->variant = EFFECT_VARIANT_STRONGER;
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(state->args->side, blit);
	/* The user takes its leap pose and animates fast until the end. */
	fdActorSelectAnimationStub(user, POSE_LEAP);
	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL * 3);

	btResourceLoadAndDecompress(RESOURCE_FLAME_BLADE_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);

	/* Flashes and sparks for three columns, though only two strike: the third's
	   random draws are still taken. */
	for(column = 0; column != RAGNAROK_SEEDED; column++) {
		for(i = 0; i != RAGNAROK_FLASHES; i++) {
			PARTICLE *flash = &state->particles[column * RAGNAROK_FLASHES + i];

			Sint32 radius = i * 2;
			Sint32 ang = syRandom() & TRIG_ANGLE_MASK;

			flash->x = sySin(ang) * radius;
			flash->y = -(syCos(ang) * radius);
			flash->variant = i / 2 + RAGNAROK_FLASH_LIFE;
		}

		for(i = 0; i != RAGNAROK_SPARKS; i++) {
			PARTICLE *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];

			Sint32 speed;
			Sint32 ang;

			speed = (syRandom() & 0x1ff) + 32;
			ang = syRandom() & TRIG_ANGLE_MASK;
			spark->x =
				toFIXED(RagnarokStrikeColumns[state->args->side][column]);
			spark->y = toFIXED(88);
			spark->velX = (sySin(ang) * speed) >> 6;
			spark->velY = -(syCos(ang) * speed * 2) >> 6;
			spark->variant = (syRandom() & 7) + 32;
		}
	}

	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frm = 0; frm != RAGNAROK_TICKS; frm++) {
		if(frm == 4) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == 8) {
			state->shakeFrm = 8;
		}
		if(frm == 18) {
			sySndPlayStub(SE_CRASH);
		}
		if(frm == RAGNAROK_DAMAGE_TICK) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}

		/* The blade: down 16 pixels a tick from above the canvas, held, then
		   creeping on. */
		if(frm <= RAGNAROK_BLADE_LAST_TICK) {
			Sint32 height;
			Sint32 x;
			Sint32 top;

			height = RAGNAROK_BLADE_HEIGHT;
			if(state->args->side == EFFECT_SIDE_PARTY) {
				if(frm < RAGNAROK_BLADE_SWING_TICKS) {
					x = frm * 10 - 8;
					top = frm * 16 - RAGNAROK_BLADE_HEIGHT;
				} else if(frm > RAGNAROK_BLADE_HOLD_TICK) {
					x = frm + 62;
					top = frm * 2 - 24;
				} else {
					x = 82;
					top = RAGNAROK_BLADE_HOLD_TOP;
				}
			} else {
				if(frm < RAGNAROK_BLADE_SWING_TICKS) {
					x = 128 - frm * 10;
					top = frm * 16 - RAGNAROK_BLADE_HEIGHT;
				} else if(frm > RAGNAROK_BLADE_HOLD_TICK) {
					x = 58 - frm;
					top = frm * 2 - 24;
				} else {
					x = 38;
					top = RAGNAROK_BLADE_HOLD_TOP;
				}
			}
			if(top + RAGNAROK_BLADE_HEIGHT > RAGNAROK_BLADE_FLOOR) {
				height = height - top -
					(RAGNAROK_BLADE_HEIGHT - RAGNAROK_BLADE_FLOOR);
			}
			if(height > 0) {
				blit[0](canvas, state->sheet, x - RAGNAROK_BLADE_WIDTH / 2, top,
					RAGNAROK_BLADE_WIDTH, height);
			}
		}

		if(frm > RAGNAROK_FIRST_STRIKE_TICK) {
			btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);
		}

		/* The two columns of light strike, with flashes and then sparks. */
		for(column = 0; column != RAGNAROK_COLUMNS; column++) {
			Sint32 start = column * 8 + RAGNAROK_FIRST_STRIKE_TICK;

			if(frm == start) {
				state->shakeFrm = 12;
			}
			if(frm >= start) {
				if(frm < start + 2) {
					blit[0](canvas, state->sheet + RAGNAROK_FLASH_PICTURES,
						RagnarokStrikeColumns[state->args->side][column] -
							FLASH_BURST_WIDTH / 2,
						56, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
				}
				for(i = 0; i != RAGNAROK_RING; i++) {
					PARTICLE *flash =
						&state->particles[column * RAGNAROK_FLASHES + i];

					Sint32 x;
					Sint32 life;
					Sint32 y;

					y = FIXEDtoINT(flash->y);
					x = FIXEDtoINT(flash->x) +
						RagnarokStrikeColumns[state->args->side][column];
					life = flash->variant;
					if(life >= 0 && life < RAGNAROK_FLASH_BURN_TICKS) {
						blit[0](canvas,
							state->sheet +
								RagnarokFlashPictures[life / 3] *
									PICTURE_BYTES(FLASH_BURST) +
								RAGNAROK_FLASH_PICTURES,
							x - FLASH_BURST_WIDTH / 2, y + 56,
							FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					}
					life = flash->variant;
					flash->variant = life > 0 ? life - 1 : -1;
				}
			}

			/* Sparks fall and bounce. */
			if(frm > start + 5) {
				for(i = 0; i != RAGNAROK_DRAWN_SPARKS; i++) {
					PARTICLE *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];

					if(spark->variant > 0) {
						Sint32 x;
						Sint32 y;
						Sint32 size;

						btParticleAdvanceWithGravity2D(
							spark, PARTICLE_DAMPING_NONE, toFIXED(1) / 16);
						spark->variant--;
						y = spark->y;
						if(y > toFIXED(RAGNAROK_GROUND_Y)) {
							spark->velY = -spark->velY / 2;
						} else {
							x = spark->x;
							if((x >= 0 && x < toFIXED(127)) && y >= 0) {
								Uint8 *picture;
								Sint32 lane;
								Sint32 drawY = FIXEDtoINT(y);
								Sint32 drawX = FIXEDtoINT(x);

								size = spark->variant / 5 + 1;
								lane = i & 1;
								picture =
									motes + BattleEffectMotePictures[size - 1];
								drawX -= size / 2;
								drawY -= size;
								blit[lane](canvas, picture, drawX, drawY, size,
									size * 2);
							}
						}
					}
				}
			}

			/* Every target flashes and recoils. */
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				if(frm == start + 6) {
					btEffectTintBattler(state->args->tgts[i],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 10);
					btMotionApplyKnockbackStub(
						state->args->tgts[i], KNOCKBACK_HURL);
				}
			}
		}

		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL);
	EFFECT_FINISH_CANVAS();
}

#undef RAGNAROK_BLADE_WIDTH
#undef RAGNAROK_BLADE_HEIGHT
#undef RAGNAROK_FLASH_PICTURES
#undef RAGNAROK_BLADE_HOLD_TOP
#undef RAGNAROK_BLADE_FLOOR
#undef RAGNAROK_FLASH_LIFE
#undef RAGNAROK_FLASH_BURN_TICKS
#undef RAGNAROK_GROUND_Y
#undef RAGNAROK_COLUMNS
#undef RAGNAROK_SEEDED
#undef RAGNAROK_FLASHES
#undef RAGNAROK_RING
#undef RAGNAROK_SPARKS
#undef RAGNAROK_DRAWN_SPARKS
#undef RAGNAROK_TICKS
#undef RAGNAROK_BLADE_SWING_TICKS
#undef RAGNAROK_BLADE_HOLD_TICK
#undef RAGNAROK_BLADE_LAST_TICK
#undef RAGNAROK_FIRST_STRIKE_TICK
#undef RAGNAROK_DAMAGE_TICK
