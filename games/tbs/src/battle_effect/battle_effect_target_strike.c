/* @file */
/* Monster bites and slashes on one target. */
/* Part of battle_effect.c. */

/* The motes in PARTICLES, 128 for each target, then the strike sparks. */
#define TARGET_STRIKE_MOTES_PER_TARGET 128
#define TARGET_STRIKE_MOTE_SLOTS	   512
#define TARGET_STRIKE_SPARK_COUNT	   512
#define TARGET_STRIKE_SPARKS		   (PARTICLES + TARGET_STRIKE_MOTE_SLOTS)

/* The smoke pictures in the sprite sheet: six 24x48 puffs, each shown for */
/* TARGET_STRIKE_SMOKE_FRAMES as a puff rises. */
#define TARGET_STRIKE_SMOKE_PICTURES 12800
#define TARGET_STRIKE_SMOKE_PUFFS	 6
#define TARGET_STRIKE_SMOKE_FRAMES	 4
/* The sparks each hit lights, a block of them for each hit: a slash's and a */
/* bite's; of the sparks, the first TARGET_STRIKE_SPARKS_SHOWN are drawn. */
#define TARGET_STRIKE_SPARKS_PER_SLASH 32
#define TARGET_STRIKE_SPARKS_PER_BITE  64
#define TARGET_STRIKE_SPARKS_SHOWN	   128
/* The smoke puffs a hit raises, from a block of TARGET_STRIKE_HIT_BLOCK in */
/* the effect's particles, and the motes it sends out, the block's whole of */
/* the target's motes; of these, the first TARGET_STRIKE_DRAIN_MOTES are drawn.
 */
#define TARGET_STRIKE_PUFFS_PER_HIT 8
#define TARGET_STRIKE_HIT_BLOCK		16
/* The hit a frame belongs to: hits are counted six pictures apart, each with */
/* its block of sparks, puffs and motes. */
#define TARGET_STRIKE_HIT(mode, frame)                                         \
	((frame) / (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 6))
#define TARGET_STRIKE_DRAIN_MOTES 64
/* The frames a drained mote flies out before the user pulls it in. */
#define TARGET_STRIKE_MOTE_FLIGHT_FRAMES 10
/* The canvas row the fangs' pictures start at. */
#define TARGET_STRIKE_BITE_Y 56
/* The bite's stages after the tornado sprite sheet: BeamImage for a slash, */
/* BlueDropletSheet otherwise. */
#define TARGET_STRIKE_BITE_PICTURES 3200
#define TARGET_STRIKE_BITE_WIDTH	18
#define TARGET_STRIKE_BITE_HEIGHT	48
/* The bite's last stage, the fangs met (TargetStrike_BiteStages). */
#define TARGET_STRIKE_FANGS_MET 3

/* Column `field` of style `mode`'s row of TargetStrike_Styles. */
#define TARGET_STRIKE_VALUE(mode, field) TargetStrike_Styles[(mode)][(field)]

/* The columns of TargetStrike_Styles. */
enum {
	TARGET_STRIKE_KIND,			  /* STRIKE_KIND_SLASH or STRIKE_KIND_BITE */
	TARGET_STRIKE_HITS,			  /* how many times the target is struck */
	TARGET_STRIKE_FLAGS,		  /* STRIKE_SMOKE and so on */
	TARGET_STRIKE_COLOR,		  /* background palette, a STRIKE_COLOR_ */
	TARGET_STRIKE_PICTURE_FRAMES, /* frames each strike picture is shown */
	TARGET_STRIKE_COMPOSITE, /* how the fangs are drawn (COMPOSITE_LIGHTEN or */
							 /* _ADDITIVE) */
	TARGET_STRIKE_FRAMES	 /* length of the effect in frames */
};

/* One slash strikes the first target among glints, in lightning colours. */
/* */
/* Abilities: Numbing Sting. */
void btEffectRunNumbingSting(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}
/* Fangs bite the first target three times, in ember colours, and drain it */
/* into the user. */
/* */
/* Abilities: Cannibal Fang. */
void btEffectRunCannibalFang(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES);
}
/* Fangs bite the first target once, in lime, with smoke. */
/* */
/* Abilities: Poisonous Bite; Poison Fang. */
void btEffectRunPoisonousBite(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_BITE);
}
/* Fangs bite the first target once, with smoke, in Mars Djinn colours. */
/* */
/* Abilities: Rabid Fang. */
void btEffectRunRabidFang(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_SMOKE_BITE);
}
/* Fangs bite the first target three times among glints, in lightning */
/* colours. */
/* */
/* Abilities: Electric Bite. */
void btEffectRunElectricBite(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES);
}
/* One lime slash strikes the first target, with smoke. */
/* */
/* Abilities: Poison Tail. */
void btEffectRunPoisonTail(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}
/* Fangs bite the first target three times and drain it, in Mars Djinn */
/* colours. */
/* */
/* Abilities: Vampiric Fang. */
void btEffectRunVampiricFang(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_DRAIN_BITES);
}
/* Fangs bite the first target twice, in blue, with sparks. */
/* */
/* Abilities: Double Fang. */
void btEffectRunDoubleFang(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITES);
}
/* One heavy slash strikes the first target, with sparks, in Mars Djinn */
/* colours. */
/* */
/* Abilities: Mortal Blow. */
void btEffectRunMortalBlow(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}
/* Two slashes strike the first target, with sparks, in lightning colours. */
/* */
/* Abilities: Twin Beaks. */
void btEffectRunTwinBeaks(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES);
}
/* Fangs bite the first target three times, in pink, with smoke. */
/* */
/* Abilities: Acid Bite. */
void btEffectRunAcidBite(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_PINK_SMOKE_BITES);
}
/* Fangs bite the first target three times, added to what lies under them, */
/* and drain it with large motes, in Mars Djinn colours. */
/* */
/* Abilities: Drain Fang. */
void btEffectRunDrainFang(BATTLE_EFFECT_PARM *args) {
	btEffectRunTargetStrike(args,
							TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES);
}
/* Strikes the first target with slashes or a bite of closing fangs, with the */
/* extras its flags choose. */
void btEffectRunTargetStrike(BATTLE_EFFECT_PARM *args, s32 mode) {
	SCR_POINT ground;
	SCR_POINT targetPosition;
	SCR_POINT alternate;
	SCR_POINT position;
	BLITTER_PAIR draw;
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 frame;
	s32 k;
	u8 *motes;
	BATTLE_CAMERA *camera;
	ACTOR *target;
	ACTOR *user;
	s32 resource;
	s32 i;
	DRAW_RECTANGLE *pairedBlitter;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	pairedBlitter =
		(DRAW_RECTANGLE *)&HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	if(mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH)
		btEffectStartCanvasLayer(0);
	else
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
	if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH)
		btResourceLoadAndDecompress(RESOURCE_BEAM_IMAGE,
									state->sheet + TARGET_STRIKE_BITE_PICTURES,
									0, 0);
	else
		btResourceLoadAndDecompress(RESOURCE_BLUE_DROPLET_SHEET,
									state->sheet + TARGET_STRIKE_BITE_PICTURES,
									0, 0);
	switch(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COLOR)) {
	case STRIKE_COLOR_BLUE:
		resource = RESOURCE_BLUE_BEAM_SHEET;
		break;
	case STRIKE_COLOR_LIME:
		resource = RESOURCE_LIME_PALETTE;
		break;
	case STRIKE_COLOR_PINK:
		resource = RESOURCE_PINK_PALETTE;
		break;
	case STRIKE_COLOR_EMBER:
		resource = RESOURCE_EMBER_STREAK_SHEET;
		break;
	case STRIKE_COLOR_MARS:
		resource = RESOURCE_MARS_DJINN_SHEET;
		break;
	case STRIKE_COLOR_LIGHTNING:
	default:
		resource = RESOURCE_LIGHTNING_BOLT_SHEET;
		break;
	}
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(resource),
				SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET,
								state->sheet + TARGET_STRIKE_SMOKE_PICTURES, 1,
								0);

	/* LEFTOVER(template): a per-target loop whose count is 1, the first target
	 * only, as btEffectRunDrain (battle_effect_drain.c) seeds each target's
	 * motes over every target; as its body once with k 0. */
	for(k = 0; k != 1; k++) {
		target = btObjectGetStub(state->args->tgts[k])->actor;
		for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
			PARTICLE *puff = &state->particles[i];

			btProjectionUnit(state->args->tgts[k], &ground);
			SET_POSITION(puff, INTtoFIXED(ground.x / 2), toFIXED(80), 0);
			SET_VELOCITY(puff, ((syRandom() & 255) - 128) << 9,
						 ((syRandom() & 255) - 128) << 9, 0);
			puff->variant = -1;
		}
		for(i = 0; i != TARGET_STRIKE_MOTES_PER_TARGET; i++) {
			PARTICLE *mote = &PARTICLES[k * TARGET_STRIKE_MOTES_PER_TARGET + i];

			SET_POSITION(mote, target->x.fixed, toFIXED(20), target->z.fixed);
			SET_VELOCITY(mote, ((syRandom() & 255) - 128) << 11,
						 ((syRandom() & 255) - 128) << 11,
						 ((syRandom() & 255) - 128) << 11);
			mote->variant = -1;
		}
		for(i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++) {
			TARGET_STRIKE_SPARKS[i].x = target->x.fixed;
			TARGET_STRIKE_SPARKS[i].y = toFIXED(20);
			TARGET_STRIKE_SPARKS[i].z = target->z.fixed;
			if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) ==
			   STRIKE_KIND_BITE) {
				TARGET_STRIKE_SPARKS[i].velX = ((syRandom() & 255) - 128) << 11;
				TARGET_STRIKE_SPARKS[i].velY = ((syRandom() & 255) - 128) << 11;
				TARGET_STRIKE_SPARKS[i].velZ = ((syRandom() & 255) - 128) << 11;
			}
			else {
				if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
				   STRIKE_HEAVY) {
					TARGET_STRIKE_SPARKS[i].velX = (syRandom() & 0x1ff) << 11;
					TARGET_STRIKE_SPARKS[i].velY = ((syRandom() & 0x1ff) - 256)
												   << 11;
					TARGET_STRIKE_SPARKS[i].velZ = ((syRandom() & 0x1ff) - 256)
												   << 11;
				}
				else {
					TARGET_STRIKE_SPARKS[i].velX = (syRandom() & 255) << 11;
					TARGET_STRIKE_SPARKS[i].velY = ((syRandom() & 255) - 128)
												   << 11;
					TARGET_STRIKE_SPARKS[i].velZ = ((syRandom() & 255) - 128)
												   << 11;
				}
				if(TARGET_STRIKE_SPARKS[i].x > 0)
					TARGET_STRIKE_SPARKS[i].velX =
						-TARGET_STRIKE_SPARKS[i].velX;
			}
			TARGET_STRIKE_SPARKS[i].variant = -1;
		}
	}

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	btProjectionScaledUnit(state->args->tgts[0], &targetPosition);
	btProjectionUnit(state->args->tgts[0], &alternate);
	targetPosition.y += (alternate.y - targetPosition.y) / 2;

	for(frame = 0; frame != TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FRAMES);
		frame++) {
		s32 span = TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) *
				   TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES);
		s32 length = span * 4;
		s32 glint = 0;

		if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_KIND) == STRIKE_KIND_SLASH) {
			/* Slashes sweep across the target from the user's side. */
			if(frame < span * 6) {
				s32 picture =
					frame /
					TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) % 6;

				if(state->args->side == EFFECT_SIDE_PARTY) {
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
										EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
										COMPOSITE_LIGHTEN);
					EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
						canvas, state->sheet + TargetStrike_Pictures[picture],
						targetPosition.x / 2 -
							(TargetStrike_Reach[picture] / 2) -
							TargetStrike_Widths[picture] + 8,
						targetPosition.y - (TargetStrike_Heights[picture] / 2),
						TargetStrike_Widths[picture],
						TargetStrike_Heights[picture]);
				}
				else {
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
										COMPOSITE_LIGHTEN);
					EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
						canvas, state->sheet + TargetStrike_Pictures[picture],
						targetPosition.x / 2 +
							(TargetStrike_Reach[picture] / 2) - 8,
						targetPosition.y - (TargetStrike_Heights[picture] / 2),
						TargetStrike_Widths[picture],
						TargetStrike_Heights[picture]);
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				if(frame % (TARGET_STRIKE_VALUE(mode,
												TARGET_STRIKE_PICTURE_FRAMES) *
							6) ==
				   TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) *
					   4) {
					if(mode == TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH) {
						EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					}
					else {
						sySndPlayStub(SE_IMPACT);
						EFFECT_SHOW_DAMAGE(SE_IMPACT);
					}
					if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) &
					   STRIKE_HEAVY) {
						state->shakeFrm = 8;
						EFFECT_STRIKE_TARGET(state->args, 0, 12,
											 KNOCKBACK_HURL);
						for(i = 0; i != TARGET_STRIKE_SPARK_COUNT; i++)
							TARGET_STRIKE_SPARKS[i].variant =
								(syRandom() & 15) + 15;
					}
					else {
						state->shakeFrm = 4;
						btEffectTintBattler(state->args->tgts[0],
											SPRITE_COLOR_WHITE, POSE_DAMAGE, 0,
											8);
						for(i = 0; i != TARGET_STRIKE_SPARKS_PER_SLASH; i++) {
							PARTICLE *spark =
								&TARGET_STRIKE_SPARKS
									[TARGET_STRIKE_HIT(mode, frame) *
										 TARGET_STRIKE_SPARKS_PER_SLASH +
									 i];

							spark->variant = (syRandom() & 15) + 7;
						}
					}
					for(i = 0; i != TARGET_STRIKE_PUFFS_PER_HIT; i++) {
						PARTICLE *puff =
							&state->particles[TARGET_STRIKE_HIT(mode, frame) *
												  TARGET_STRIKE_HIT_BLOCK +
											  i];

						puff->variant = 0;
					}
				}
			}
			if(frame >= 12 && frame <= 31)
				glint = 1;
		}
		else if(frame < length + 4) {
			/* Bites: rows of fangs close above and below the target and meet in
			 * a */
			/* flash. */
			s32 stage;

			if(frame < length) {
				s32 picture = frame / TARGET_STRIKE_VALUE(
										  mode, TARGET_STRIKE_PICTURE_FRAMES);

				while(picture > 4)
					picture -= 4;
				stage = TargetStrike_BiteStages[picture];
			}
			else {
				stage = TARGET_STRIKE_FANGS_MET;
			}
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
				TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
			EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
				canvas,
				state->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE) +
					TARGET_STRIKE_BITE_PICTURES,
				targetPosition.x / 2 - TARGET_STRIKE_BITE_WIDTH,
				TARGET_STRIKE_BITE_Y, TARGET_STRIKE_BITE_WIDTH,
				TARGET_STRIKE_BITE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
				TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_COMPOSITE));
			EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
				canvas,
				state->sheet + stage * PICTURE_BYTES(TARGET_STRIKE_BITE) +
					TARGET_STRIKE_BITE_PICTURES,
				targetPosition.x / 2, TARGET_STRIKE_BITE_Y,
				TARGET_STRIKE_BITE_WIDTH, TARGET_STRIKE_BITE_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			if(frame %
				   (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) *
					4) ==
			   TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES) * 3) {
				/* A hit: the target flashes, and the last lets the battle show
				 * the */
				/* damage. Hits are counted six pictures apart, as a slash's
				 * are, so a */
				/* third bite relights the second's sparks and smoke rather than
				 * a */
				/* block of its own. */
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, 0, 8);
				state->shakeFrm = 4;
				if(frame >
				   (TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_HITS) * 4 - 4) *
					   TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_PICTURE_FRAMES))
					EFFECT_SHOW_DAMAGE(SE_IMPACT);
				else
					sySndPlayStub(SE_IMPACT);
				for(i = 0; i != TARGET_STRIKE_SPARKS_PER_BITE; i++) {
					PARTICLE *spark = &TARGET_STRIKE_SPARKS
										  [TARGET_STRIKE_HIT(mode, frame) *
											   TARGET_STRIKE_SPARKS_PER_BITE +
										   i];

					spark->variant = (syRandom() & 15) + 7;
				}
				for(i = 0; i != TARGET_STRIKE_PUFFS_PER_HIT; i++) {
					PARTICLE *puff =
						&state->particles[TARGET_STRIKE_HIT(mode, frame) *
											  TARGET_STRIKE_HIT_BLOCK +
										  i];

					puff->variant = 0;
				}
				for(i = 0; i != TARGET_STRIKE_HIT_BLOCK; i++)
					PARTICLES[TARGET_STRIKE_HIT(mode, frame) *
								  TARGET_STRIKE_HIT_BLOCK +
							  i]
						.variant = 0;
			}
			if(stage == TARGET_STRIKE_FANGS_MET)
				glint = 1;
		}

		/* Glints scatter around the target, up to half their radius away. */
		if((TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_GLINTS) &&
		   glint) {
			for(i = 0; i != 3; i++) {
				s32 image = frame & 3;
				s32 angle = syRandom() & TRIG_ANGLE_MASK;
				s32 radius = (syRandom() & 31) + 4;
				s32 x;
				s32 y;

				x = targetPosition.x / 2 +
					(FIXEDtoINT(sySin(angle) * radius) >> 1) -
					(BattleEffect_GlintPictureWidths[image] / 2);
				y = targetPosition.y -
					(FIXEDtoINT(syCos(angle) * radius) >> 1) -
					(BattleEffect_GlintPictureHeights[image] / 2);
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE_PAIRED,
					TargetStrike_GlintDrawFlags[syRandom() & 3] |
						EFFECT_CLIP_BOUNDS,
					COMPOSITE_ADDITIVE);
				(*pairedBlitter)(
					canvas, state->sheet + BattleEffect_GlintPictures[image], x,
					y, BattleEffect_GlintPictureWidths[image],
					BattleEffect_GlintPictureHeights[image]);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}

		EFFECT_VIEW_FROM_CAMERA(camera);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
							COMPOSITE_LIGHTEN);
		draw[0] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
							COMPOSITE_ADDITIVE);
		draw[1] = *pairedBlitter;
		/* DEAD: a loop that runs once, for the user; the ROM keeps its counter,
		 * its */
		/* test against 1 and its branch back. */
		for(k = 0; k != 1; k++) {
			user = btObjectGetStub(state->args->user)->actor;
			/* Smoke puffs rise and spread. */
			if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SMOKE) {
				for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
					PARTICLE *puff = &state->particles[i];

					if(puff->variant >= 0 &&
					   puff->variant < TARGET_STRIKE_SMOKE_PUFFS *
										   TARGET_STRIKE_SMOKE_FRAMES) {
						s32 picture =
							puff->variant / TARGET_STRIKE_SMOKE_FRAMES;

						draw[i & 1](canvas,
									state->sheet +
										picture * PICTURE_BYTES(SMOKE_PUFF) +
										TARGET_STRIKE_SMOKE_PICTURES,
									FIXEDtoINT(puff->x) - SMOKE_PUFF_WIDTH / 2,
									FIXEDtoINT(puff->y) - SMOKE_PUFF_HEIGHT / 2,
									SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
						btParticleAdvanceWithGravity2D(puff, 62,
													   -toFIXED(1) / 64);
						puff->variant++;
					}
				}
			}
			/* Motes burst out of the target and are pulled into the user, who
			 * glows. */
			if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_ABSORB) {
				s32 size = 3;

				if(mode == TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES)
					size = 8;
				if(frame == 55)
					btEffectTintBattler(state->args->user, SPRITE_COLOR_WHITE,
										TINT_KEEP, TINT_KEEP, 0);
				if(frame == 90)
					btEffectTintBattler(state->args->user, SPRITE_COLOR_NORMAL,
										TINT_KEEP, TINT_KEEP, 0);
				for(i = 0; i != TARGET_STRIKE_DRAIN_MOTES; i++) {
					PARTICLE *mote = &PARTICLES[i];

					if(mote->variant >= 0) {
						btProjectionPoint(mote, &position);
						position.x >>= 1;
						EFFECT_DRAW_MOTE(draw[1], canvas, motes, position.x,
										 position.y, size);
						btParticleAdvanceWithGravity3D(mote, 60, 0);
						mote->variant++;
						if(mote->variant > TARGET_STRIKE_MOTE_FLIGHT_FRAMES) {
							s32 dx = (user->x.fixed - mote->x) >> 8;
							s32 dy =
								(user->y.fixed - mote->y + toFIXED(20)) >> 8;
							s32 dz = (user->z.fixed - mote->z) >> 8;

							mote->velX += dx;
							mote->velY += dy;
							mote->velZ += dz;
							if((dx > -(toFIXED(1) / 16) &&
								dx < toFIXED(1) / 16) &&
							   (dz > -(toFIXED(1) / 16) &&
								dz < toFIXED(1) / 16))
								mote->variant = -1;
						}
					}
				}
			}
			if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_SPARKS) {
				for(i = 0; i != TARGET_STRIKE_SPARKS_SHOWN; i++) {
					PARTICLE *spark = &TARGET_STRIKE_SPARKS[i];
					s32 size = spark->variant;

					if(size >= 0) {
						btProjectionPoint(spark, &position);
						position.x >>= 1;
						btParticleAdvanceWithGravity3D(spark, 60, 0);
						size >>= 3;
						size++;
						EFFECT_DRAW_MOTE(draw[1], canvas, motes, position.x,
										 position.y, size);
						spark->variant--;
					}
				}
			}
		}
		EFFECT_RELEASE_BLITTERS();
		if(TARGET_STRIKE_VALUE(mode, TARGET_STRIKE_FLAGS) & STRIKE_HEAVY)
			btCameraShake(8, 8);
		else
			btCameraShake(2, 4);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef TARGET_STRIKE_SPARKS
#undef TARGET_STRIKE_VALUE
#undef TARGET_STRIKE_SMOKE_PICTURES
#undef TARGET_STRIKE_SMOKE_PUFFS
#undef TARGET_STRIKE_SMOKE_FRAMES
#undef TARGET_STRIKE_SPARKS_PER_SLASH
#undef TARGET_STRIKE_SPARKS_PER_BITE
#undef TARGET_STRIKE_SPARKS_SHOWN
#undef TARGET_STRIKE_PUFFS_PER_HIT
#undef TARGET_STRIKE_HIT_BLOCK
#undef TARGET_STRIKE_HIT
#undef TARGET_STRIKE_DRAIN_MOTES
#undef TARGET_STRIKE_MOTE_FLIGHT_FRAMES
#undef TARGET_STRIKE_BITE_Y
#undef TARGET_STRIKE_FANGS_MET
