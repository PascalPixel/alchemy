/* The body of Thunder Mine and Earth Force, then Megaera and Void Beam. Part of
   battle_effect.c. */

/* Thunder Mine's sparks, in EXTRA_PARTICLES: those cleared, and of them those
   that burst out as the ball arrives. */
#define BALL_LIGHTNING_SPARKS_CLEARED 512
#define BALL_LIGHTNING_SPARK_COUNT	  128
/* The canvas row Thunder Mine's sparks bounce at. */
#define BALL_LIGHTNING_SPARK_FLOOR	  111

/* Where TornadoSheet is decoded in the effect's pictures, for its glints. */
#define BALL_LIGHTNING_TORNADO_PICTURES 2048
/* The bolts' four pictures (LightningBoltSheet), each copied into the middle of
   a 32x128 picture, after the glints: a bolt is drawn as a column textured with
   a picture. */
#define BALL_LIGHTNING_BOLT_PICTURES \
	(BALL_LIGHTNING_TORNADO_PICTURES + GLINT_SET_BYTES)
#define BALL_LIGHTNING_BOLT_PICTURE_COUNT  4
#define BALL_LIGHTNING_BOLT_PICTURE_WIDTH  32
#define BALL_LIGHTNING_BOLT_PICTURE_HEIGHT 128
#define BALL_LIGHTNING_BOLT_PICTURE_INSET \
	((BALL_LIGHTNING_BOLT_PICTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2)
/* The bolts of lightning set up, and of them those drawn, each for eight ticks
   from the tick of its number. */
#define BALL_LIGHTNING_BOLTS_SET_UP 63
#define BALL_LIGHTNING_BOLTS		40

#define BALL_LIGHTNING_TICKS  50
/* The ball's size, which grows two pixels a tick from 16 to this. */
#define BALL_LIGHTNING_SIZE	  48
#define EARTH_FORCE_BALL_SIZE 64

/* The body of Thunder Mine and Earth Force: ball lightning. Thunder Mine's ball
   of light grows at the caster as bolts of lightning crackle out of it all
   round, and crosses to the target; Earth Force's dark ball, rimmed in magenta,
   with bolts of lightning inside it, grows as it falls from above the caster
   onto the target. As it arrives the screen flashes white and the target is
   hurled away, and Thunder Mine's ball bursts into sparks that bounce. */
void btEffectRunBallLightning(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Uint8		  *sprs;
	Sint32		   ballSize;
	Sint32		   ticks;
	Sint32		   sparkFloor;
	Sint32		   x;
	Sint32		   y;
	SCREEN_POINT   tgt;
	SCREEN_POINT   src;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	PARTICLE	  *ball;
	PARTICLE	  *bolt;
	PARTICLE	  *spark;
	PARTICLE	  *dot;
	void		  *vertices;
	Sint32		   i;
	Sint32		   k;
	Uint8		  *buf;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	sprs = RamHeapSlots->battleSheet;
	buf = RamMapMetatileBuffer;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	if(mode == BALL_LIGHTNING_THUNDER_MINE) {
		REG_BLDALPHA = EFFECT_BLEND_ADD;
	} else {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX / 2);
	}
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	ballSize = 0;
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET,
		state->sheet + BALL_LIGHTNING_TORNADO_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, buf, 1, 1);
	if(mode == BALL_LIGHTNING_EARTH_FORCE) {
		syWordsCopy((void *)BG_PLTT,
			syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_C),
			SHEET_PALETTE_BYTES);
	}

	syWordsClearEntry(state->sheet + BALL_LIGHTNING_BOLT_PICTURES,
		BALL_LIGHTNING_BOLT_PICTURE_COUNT *
			PICTURE_BYTES(BALL_LIGHTNING_BOLT_PICTURE));
	for(i = 0; i != BALL_LIGHTNING_BOLT_PICTURE_COUNT; i++) {
		for(k = 0; k != LIGHTNING_BOLT_HEIGHT; k++) {
			Sint32 j;

			for(j = 0; j != LIGHTNING_BOLT_WIDTH; j++) {
				state->sheet[BALL_LIGHTNING_BOLT_PICTURES +
					BALL_LIGHTNING_BOLT_PICTURE_INSET +
					i * PICTURE_BYTES(BALL_LIGHTNING_BOLT_PICTURE) +
					k * BALL_LIGHTNING_BOLT_PICTURE_WIDTH + j] =
					buf[i * LIGHTNING_BOLT_PICTURE_BYTES +
						k * LIGHTNING_BOLT_WIDTH + j];
			}
		}
	}
	if(mode == BALL_LIGHTNING_THUNDER_MINE) {
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else {
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	if(mode == BALL_LIGHTNING_THUNDER_MINE) {
		ticks = 24;
		/* Only Thunder Mine's sparks bounce: Ball Lightning never reads the
		   floor. */
		sparkFloor = BALL_LIGHTNING_SPARK_FLOOR;
	} else {
		ticks = 40;
	}

	for(frm = 0; frm != BALL_LIGHTNING_TICKS; frm++) {
		ball = state->particles;
		if(frm == 0) {
			btEffectPosProjectBattler(args->user, &src);
			btEffectPosProjectBattler(args->tgts[0], &tgt);
			if(mode == BALL_LIGHTNING_THUNDER_MINE) {
				ball->x = toFIXED(src.x);
				ball->y = INTtoFIXED(src.y - 32);
				ball->velX = (toFIXED(tgt.x) - ball->x) / ticks;
			} else {
				if(args->side == EFFECT_SIDE_ENEMIES) {
					ball->x = INTtoFIXED(src.x - 32);
				} else {
					ball->x = INTtoFIXED(src.x + 32);
				}
				ball->y = INTtoFIXED(src.y - 80);
				ball->velX = (INTtoFIXED(tgt.x - 32) - ball->x) / ticks;
			}
			ball->velY = (INTtoFIXED(tgt.y - 32) - ball->y) / ticks;
			sySndPlayStub(SE_RISE_WITH_SPARKS);
			/* Each bolt turns its own way at its own speed, on a picture of its
			   own. */
			for(i = 0, bolt = &state->particles[1];
				i != BALL_LIGHTNING_BOLTS_SET_UP; i++, bolt++) {
				bolt->z = syRandom();
				bolt->velZ = (syRandom() & 255) + 128;
				if(syRandom() & 1) {
					bolt->velZ = -bolt->velZ;
				}
				bolt->variant = syRandom() & 3;
			}
			/* UB: the variants are cleared through one Sint32 pointer stepped a
			   whole step at a time, past the step's scalar members. */
#if FIX_POINTER_OUTSIDE_OBJECT
			for(i = 0; i != BALL_LIGHTNING_SPARKS_CLEARED; i++)
				EXTRA_PARTICLES[i].variant = -1;
#else
			{
				Sint32 *variant = &EXTRA_PARTICLES[0].variant;

				for(i = 0; i != BALL_LIGHTNING_SPARKS_CLEARED; i++) {
					*variant = -1;
					variant += sizeof(PARTICLE) / sizeof(Sint32);
				}
			}
#endif
		}

		if(frm == ticks) {
			Sint32 n;

			for(n = 0, spark = EXTRA_PARTICLES; n != BALL_LIGHTNING_SPARK_COUNT;
				n++, spark++) {
				Sint32 ang;
				Sint32 speed;

				spark->x = ball->x / 2;
				spark->y = ball->y;
				ang = syRandom();
				speed = syRandom() % 400 + 32;
				spark->velX = (sySin(ang) * speed) >> 7;
				spark->velY = (syCos(ang) * speed) >> 6;
				spark->variant = (syRandom() & 7) + 16;
			}
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			state->shakeFrm = 16;
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			if(mode == BALL_LIGHTNING_THUNDER_MINE) {
				EFFECT_SHOW_DAMAGE(SE_CRASH);
			} else {
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
				sySndPlayStub(SE_CRASH);
			}
		}

		/* The ball, growing, with four glints about it; Earth Force's ball is a
		   shaded disc turned over, dark inside a bright rim. */
		if(frm <= ticks) {
			Sint32 largest;

			ball->variant++;
			x = FIXEDtoINT(ball->x) >> 1;
			y = FIXEDtoINT(ball->y);
			if(mode == BALL_LIGHTNING_THUNDER_MINE) {
				ballSize = frm * 2 + 16;
				largest = BALL_LIGHTNING_SIZE;
			} else {
				ballSize = frm * 2 + 16;
				largest = EARTH_FORCE_BALL_SIZE;
			}
			if(ballSize > largest) {
				ballSize = largest;
			} else {
				btEffectDrawShadedCircles(buf, ballSize, toFIXED(2673) / 2048);
				if(mode == BALL_LIGHTNING_EARTH_FORCE) {
					for(i = 0; i != ballSize * ballSize / 2; i++) {
						if(buf[i] != 0) {
							buf[i] = CANVAS_LEVEL_BRIGHTEST + 1 - buf[i];
						}
					}
				}
			}
			btCanvasBlitMirroredQuarters(buf, x, y, ballSize);
			for(i = 0; i != 4; i++) {
				/* DEAD: i runs 0 to 3, so the mask changes nothing; the ROM
				   keeps the and. */
				Sint32 picture = i & 3;
				Sint32 half = ballSize / 2;
				Sint32 ang = syRandom();
				Sint32 glintX = x + (FIXEDtoINT(sySin(ang) * half)) -
					(BattleEffectGlintPictureWidths[picture] >> 1);
				Sint32 glintY = y + (FIXEDtoINT(syCos(ang) * half)) -
					(BattleEffectGlintPictureHeights[picture] >> 1);
				Sint32 flg =
					EFFECT_CLIP_BOUNDS | BoltTrailGlintFlips[syRandom() & 3];
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE_PAIRED, flg, BLITTER_MAX);
				(RamHeapSlots->ramCodePaired)(canvas,
					state->sheet + BALL_LIGHTNING_TORNADO_PICTURES +
						BattleEffectGlintPictures[picture],
					glintX, glintY, BattleEffectGlintPictureWidths[picture],
					BattleEffectGlintPictureHeights[picture]);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
			btParticleAdvanceWithGravity2D(ball, PARTICLE_DAMPING_NONE, 0);
		}

		/* Thunder Mine's sparks bounce, each drawn at eight steps of its flight
		   a tick, so that it streaks. */
		if(mode == BALL_LIGHTNING_THUNDER_MINE && frm >= ticks) {
			for(i = 0, dot = EXTRA_PARTICLES;
				i != BALL_LIGHTNING_SPARKS_CLEARED; i++, dot++) {
				if(dot->variant >= 0) {
					Sint32 size = (dot->variant >> 3) + 2;
					Sint32 step;

					for(step = 0; step != 8; step++) {
						draw[0](canvas,
							sprs + BattleEffectMotePictures[size - 1],
							(FIXEDtoINT(dot->x)) - size / 2,
							(FIXEDtoINT(dot->y)) - size, size, size * 2);
						btParticleAdvanceWithGravity2D(
							dot, 62, toFIXED(1) / 128);
					}
					if((FIXEDtoINT(dot->y)) > sparkFloor) {
						dot->velY = -dot->velY * 48 / DAMPING_SCALE;
					}
					dot->variant--;
				}
			}
		}

		/* The bolts of lightning about the ball, each shown for eight ticks:
		   dimmed a quarter on its first, whole for three, then fading. */
		if(frm <= ticks) {
			vertices = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 5;
			texture.logHeight = 7;
			/* DEAD: both kinds are overwritten by the MODEL_SHADED_MAX below;
			   the ROM keeps the test and both stores. */
			if(mode == BALL_LIGHTNING_THUNDER_MINE) {
				model->kind = MODEL_SHADED_MAX;
			} else {
				model->kind = MODEL_SHADED;
			}
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffectColumnMesh;
			model->vertices = vertices;
			for(i = 0, bolt = &state->particles[1]; i != BALL_LIGHTNING_BOLTS;
				i++, bolt++) {
				Sint32 shade;

				if(frm < i || frm >= i + 8) {
					continue;
				}
				shade = 0;
				if(frm == i) {
					shade = MODEL_SHADE_INVISIBLE / 4;
				} else if(frm >= i + 4) {
					Sint32 fade = (frm - i) * 16 - 64;

					shade = -fade;
				}
				texture.pixels = state->sheet + BALL_LIGHTNING_BOLT_PICTURES +
					bolt->variant * PICTURE_BYTES(BALL_LIGHTNING_BOLT_PICTURE);
				model->shade = shade;
				syInitMatrix();
				syTranslate(
					ball->x / 2 - toFIXED(64), ball->y - toFIXED(64), 0);
				syRotZ(bolt->z);
				bolt->z += bolt->velZ;
				if(mode == BALL_LIGHTNING_THUNDER_MINE) {
					syScale(toFIXED(4), toFIXED(1), toFIXED(1));
					btRenderTransformVertices(
						BattleEffectTaperedBeamCorners, vertices, 4);
				} else {
					syScale(ballSize * (toFIXED(1) / 32), ballSize * 312,
						toFIXED(1));
					btRenderTransformVertices(
						BattleEffectBeamCorners, vertices, 4);
				}
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Megaera's sparks and sparkles, in PARTICLES. */
#define MEGAERA_SPARK_COUNT 512
/* Megaera's glints, bolts of lightning and sparkles, in the map cell buffer
   from its second 16 KB on. */
#define MEGAERA_PICTURES	(RamMapMetatileBuffer + 16384)
extern const SCALE MegaeraUnitScale;

/* Where FlashBurstSheet, LightningBoltSheet and SparkleDots are decoded in the
   effect's pictures. */
#define MEGAERA_FLASH_BURST_PICTURES	14336
#define MEGAERA_LIGHTNING_BOLT_PICTURES GLINT_SET_BYTES
#define MEGAERA_SPARKLE_DOTS_PICTURES	11798

#define MEGAERA_TICKS			  170
/* Two swords fall point down among the foes, the second 16 ticks after the
   first, each landing at MEGAERA_SWORD_GROUND_Y with a flash, a bolt of
   lightning and a burst of sparks that fall back and go out below
   MEGAERA_SPARK_FLOOR. A sword's picture is 64x64 in 256 colours: as it lands,
   its bottom row of tiles, the point, is cleared, so it stands buried. */
#define MEGAERA_SWORDS			  2
#define MEGAERA_SWORD_GROUND_Y	  132
#define MEGAERA_SWORD_POINT_BYTES (64 * 8)
#define MEGAERA_SPARK_FLOOR		  104
/* Each sword's bolt of lightning reaches down the canvas to this row, where its
   sparks burst. */
#define MEGAERA_BOLT_HEIGHT		  112
/* From MEGAERA_FIRE_TICK the field bursts into flame: two columns of fiery
   streaks (WhirlBeamSheet's three 96x64 pictures) sway over it while billows of
   fire rise, the burst's pictures from its third, four ticks each. */
#define MEGAERA_FIRE_TICK		  64
#define WHIRL_BEAM_WIDTH		  96
#define WHIRL_BEAM_HEIGHT		  64
#define MEGAERA_BILLOWS_SET_UP	  32
#define MEGAERA_BILLOWS			  24
#define MEGAERA_BILLOW_TICKS	  16
#define MEGAERA_BILLOW_PICTURES \
	(MEGAERA_FLASH_BURST_PICTURES + 2 * PICTURE_BYTES(FLASH_BURST))
/* Particles given a place and a delay that nothing reads, as Nereid's are. */
#define MEGAERA_UNREAD_FIRST  24
#define MEGAERA_UNREAD_COUNT  16
/* Megaera swoops in across the foes at MEGAERA_SWOOP_TICK, trailing sparkles
   for MEGAERA_TRAIL_TICKS; each sparkle twinkles for
   MEGAERA_SPARKLE_TICKS. */
#define MEGAERA_SWOOP_TICK	  108
#define MEGAERA_TRAIL_TICKS	  24
#define MEGAERA_SPARKLE_TICKS 36

/* Megaera: two swords fall point down among the foes, each with a flash, a bolt
   of lightning and a burst of sparks; then the field bursts into flame, and
   Megaera swoops in across the foes in a trail of sparkles, and bursts on them.
   Holding A or B after tick 10 skips to the swoop. */
void btEffectRunMegaera(BATTLE_EFFECT_PARM *args)
{
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frm;
	Uint8				*sprs;
	DRAW_RECTANGLE		*rects;
	POINT2_D			 pos;
	POINT2_D			 vel;
	DRAW_RECTANGLE		 blitters[2];
	SCALE				 scale;
	Sint32				 placement[4];
	PARTICLE			*p;
	Uint8				*pictures;
	Sint32				 i;
	Sint32				 j;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEff;
	sprs = RamHeapSlots->battleSheet;
	pictures = MEGAERA_PICTURES;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	rects = blitters;
	btEffectBuildCanvasBlitters(0, rects);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + MEGAERA_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
		pictures + MEGAERA_LIGHTNING_BOLT_PICTURES, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_SPARKLE_DOTS, pictures + MEGAERA_SPARKLE_DOTS_PICTURES, 0, 0);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		state->particles[i].variant = -1;
	}
	/* DEAD: particles laid along canvas rows 56 to 63, which nothing in Megaera
	   reads; the ROM keeps the loop. */
	for(i = 0, p = &state->particles[MEGAERA_UNREAD_FIRST];
		i != MEGAERA_UNREAD_COUNT; i++, p++) {
		p->x = syRandom() & 127;
		p->y = (syRandom() & 7) + 56;
		p->variant = -(syRandom() & 15);
	}
	for(i = 0, p = state->particles; i != MEGAERA_BILLOWS_SET_UP; i++, p++) {
		p->x = INTtoFIXED((syRandom() & 31) + 28);
		p->y = INTtoFIXED((syRandom() % 48) + 56);
		p->velX = ((syRandom() & 255) - 128) << 12;
		p->velY = (16 - (syRandom() & 127)) << 10;
		p->variant = ~i;
	}
	for(i = 0; i != MEGAERA_SPARK_COUNT; i++) {
		PARTICLES[i].variant = -1;
	}
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET),
		SHEET_PALETTE_BYTES);
	btEffectShowLivingTargets(state->args);
	sySynch(1);

	for(frm = 0; frm != MEGAERA_TICKS; frm++) {
		if((PerWork.repeat & (KEY_A | KEY_B)) && frm > 10) {
			if(frm < MEGAERA_SWOOP_TICK) {
				frm = MEGAERA_SWOOP_TICK - 1;
			}
		}
		if(frm == MEGAERA_SWOOP_TICK - 1) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == 148) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frm == 0) {
			pos.x = toFIXED(32);
			pos.y = toFIXED(72);
			vel.x = toFIXED(9);
			vel.y = -toFIXED(2);
			btEffectCreateSprs(MEGAERA_SWORDS, SPRITE_MEGAERA_SWORD, 2);
		}
		if(frm == MEGAERA_SWOOP_TICK) {
			fdSprDestroyStub(state->sprs[0]);
			fdSprDestroyStub(state->sprs[1]);
			btEffectCreateSprs(MEGAERA_OBJECTS, SPRITE_MEGAERA, 2);
		}

		/* Megaera swoops, trailing sparkles. */
		if(frm >= MEGAERA_SWOOP_TICK) {
			pos.x += vel.x;
			pos.y += vel.y;
			vel.x = vel.x * 59 / DAMPING_SCALE;
			vel.y = vel.y * 59 / DAMPING_SCALE;
			btEffectPlaceFormationSprs(SUMMON_FORMATION_MEGAERA, pos.x, pos.y);
			if(frm == MEGAERA_SWOOP_TICK) {
				for(i = 0; i != MEGAERA_SPARK_COUNT; i++) {
					PARTICLES[i].variant = -1;
				}
			}
			{
				Sint32 trailFrm = frm - MEGAERA_SWOOP_TICK;
				if((Uint32)trailFrm < MEGAERA_TRAIL_TICKS) {
					Sint32 cnt = 10 - trailFrm / 2;

					if(cnt > 0) {
						Sint32 half;

						for(i = 0, half = pos.x / 2; i != cnt / 5; i++) {
							PARTICLE *sparkle;
							Sint32	  ang;
							Sint32	  radius;

							sparkle = &PARTICLES[((trailFrm << 5) + i) &
								(MEGAERA_SPARK_COUNT - 1)];
							ang = syRandom();
							radius = (syRandom() & 31) + 16;
							sparkle->x =
								sySin(ang) * radius + half + toFIXED(16);
							sparkle->y =
								syCos(ang) * radius + pos.y - toFIXED(8);
							sparkle->velX = 0;
							sparkle->velY = 0;
							sparkle->variant = syRandom() & 15;
						}
					}
				}
			}
		}
		if((Uint32)(frm - MEGAERA_FIRE_TICK) <= 3) {
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if(frm > 103) {
			btEffectStepPaletteToResource(RESOURCE_TAN_PALETTE);
		}
		if(frm == MEGAERA_FIRE_TICK) {
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
				SHEET_PALETTE_BYTES);
		}

		/* The field in flames: two columns of fiery streaks, each drawn again
		   below itself, sway until Megaera comes, and billows of fire rise. */
		if(frm >= MEGAERA_FIRE_TICK) {
			if(frm <= 111) {
				Sint32 picture = (frm / 4) % 3;
				Sint32 swayX =
					(FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) << 3)) - 16;
				Sint32			which = frm == MEGAERA_SWOOP_TICK;
				DRAW_RECTANGLE *draw = &blitters[which];
				Uint8		   *cells =
					state->sheet + picture * PICTURE_BYTES(WHIRL_BEAM);

				(*draw)(canvas, cells, swayX, 0, WHIRL_BEAM_WIDTH,
					WHIRL_BEAM_HEIGHT);
				(*draw)(canvas, cells, swayX, WHIRL_BEAM_HEIGHT,
					WHIRL_BEAM_WIDTH, 52);
				{
					Sint32 flickerFrm = (frm / 2) % 3;
					Uint8 *flickerPixels;

					swayX = FIXEDtoINT(
						sySin(frm * (TRIG_TURN / 16) + DEGtoANG(180)) << 3);
					flickerPixels =
						state->sheet + flickerFrm * PICTURE_BYTES(WHIRL_BEAM);
					(*draw)(canvas, flickerPixels, swayX, 0, WHIRL_BEAM_WIDTH,
						WHIRL_BEAM_HEIGHT);
					(*draw)(canvas, flickerPixels, swayX, WHIRL_BEAM_HEIGHT,
						WHIRL_BEAM_WIDTH, 52);
				}
			}
			for(i = 0, p = state->particles; i != MEGAERA_BILLOWS; i++, p++) {
				if((Uint32)p->variant < MEGAERA_BILLOW_TICKS) {
					Sint32 row = p->variant / 4;
					Sint32 which = i & 1;
					Uint8 *smoke = state->sheet + MEGAERA_BILLOW_PICTURES +
						row * PICTURE_BYTES(FLASH_BURST);

					rects[which](canvas, smoke,
						FIXED_INTEGER_PART(p->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(p->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(p, 62, -toFIXED(1) / 4);
				}
				p->variant++;
				/* Megaera's coming blows the fire aside; until then each billow
				   rises again from the field when it has burnt out. */
				if(frm == MEGAERA_SWOOP_TICK) {
					p->velX += toFIXED(2);
					p->velY /= 4;
				}
				if(frm < MEGAERA_SWOOP_TICK) {
					Sint32 v = p->variant;

					if(v == MEGAERA_BILLOW_TICKS) {
						p->variant = v = 0;
					}
					if(v == 0) {
						p->x = INTtoFIXED((syRandom() & 31) + 28);
						p->y = INTtoFIXED((syRandom() % 48) + 56);
						p->velX = ((syRandom() & 255) - 128) << 12;
						p->velY = (16 - (syRandom() & 127)) << 10;
					}
				}
			}
		}
		if(frm == MEGAERA_FIRE_TICK) {
			sySndPlayStub(SE_WAVE_HIGH);
		}
		/* The fire crackles eight times, four ticks apart. */
		{
			Sint32 crackleFrm;

			for(i = 0, crackleFrm = 75; i != 8; i++, crackleFrm += 4) {
				if(frm == crackleFrm) {
					sySndPlayStub(SE_HEAVY_IMPACT);
				}
			}
		}
		/* As the first sword lands the foes flash white. */
		if(frm == 10) {
			state->shakeFrm = 8;
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
			}
		}
		/* The sparkles twinkle through SparkleDots' nine pictures, drifting
		   down and to the left. */
		if(frm >= MEGAERA_SWOOP_TICK) {
			for(i = 0, p = PARTICLES; i != MEGAERA_SPARK_COUNT; i++, p++) {
				Sint32 k;

				if((Uint32)p->variant >= MEGAERA_SPARKLE_TICKS) {
					continue;
				}
				k = (p->variant / 2) % 9;
				blitters[0](canvas,
					(MEGAERA_PICTURES + MEGAERA_SPARKLE_DOTS_PICTURES) +
						BattleEffectSparklePictures[k],
					FIXED_INTEGER_PART(p->x) - BattleEffectSparkleSizes[k] / 2,
					FIXED_INTEGER_PART(p->y) - BattleEffectSparkleSizes[k] / 2,
					BattleEffectSparkleSizes[k], BattleEffectSparkleSizes[k]);
				btParticleAdvanceWithGravity2D(p, 58, 0);
				if(i & 1) {
					p->velX += -toFIXED(1) / 32;
					p->velY += toFIXED(1) / 16;
				} else {
					p->velX += -toFIXED(1) / 16;
					p->velY += toFIXED(1) / 32;
				}
				p->variant++;
			}
		}

		/* The two swords: each falls 16 pixels a tick, and lands with a flash,
		   a bolt of lightning, a burst of sparks and glints. */
		if(frm < MEGAERA_FIRE_TICK) {
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			for(i = 0; i != MEGAERA_SWORDS; i++) {
				Sint32 fallFrm = i * 16;
				Sint32 column = i * 32 + 24;

				if(frm == fallFrm + 5) {
					sySndPlayStub(SE_POWER_UP);
				}
				if(frm == fallFrm + 9) {
					state->shakeFrm = 8;
					syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					sySndPlayStub(SE_CLATTER);
				}
				if(frm >= fallFrm) {
					Sint32 t = frm - fallFrm;
					Sint32 fallY = t * 16 - 16;

					scale = MegaeraUnitScale;
					if(fallY > MEGAERA_SWORD_GROUND_Y - 1) {
						fallY = MEGAERA_SWORD_GROUND_Y;
					}
					placement[3] = 0;
					placement[1] = 0;
					placement[0] = toFIXED(column * 2);
					placement[2] = toFIXED(fallY);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], placement, &scale.x, 0);
					/* The sword's bottom row of tiles; a sprite's tile number
					   counts tiles of 16 colours, even in 256. */
					if(t == 9) {
						syWordsClearEntry((Uint8 *)OBJ_VRAM0 +
								(OBJ_64X64_8BPP_BYTES -
									MEGAERA_SWORD_POINT_BYTES) +
								((SPR *)state->sprs[i])->image.tile *
									TILE_SIZE_4BPP,
							MEGAERA_SWORD_POINT_BYTES);
					}
					if(frm == fallFrm + 10) {
						for(j = 0; j != MEGAERA_SPARK_COUNT / MEGAERA_SWORDS;
							j++) {
							PARTICLE *spark = &PARTICLES[i *
									(MEGAERA_SPARK_COUNT / MEGAERA_SWORDS) +
								j];
							Sint32	  ang = syRandom() & 0x7fff;
							Sint32	  speed = syRandom() & 255;

							spark->x = toFIXED(column);
							spark->y = toFIXED(MEGAERA_BOLT_HEIGHT);
							spark->velY = (-sySin(ang) * speed) >> 5;
							spark->velX = (syCos(ang) * speed) >> 5;
							spark->variant = 0;
						}
					}
					if(frm >= fallFrm + 10 && frm < fallFrm + 18) {
						blitters[0](canvas,
							(MEGAERA_PICTURES +
								MEGAERA_LIGHTNING_BOLT_PICTURES) +
								(((frm + i) / 4) & 3) *
									LIGHTNING_BOLT_PICTURE_BYTES,
							column - LIGHTNING_BOLT_WIDTH / 2, 0,
							LIGHTNING_BOLT_WIDTH, MEGAERA_BOLT_HEIGHT);
					}
					if(frm >= fallFrm + 10) {
						for(j = 0; j != 2; j++) {
							Sint32 k = j & 3;
							Sint32 glintX = column + (syRandom() & 7) -
								(BattleEffectGlintPictureWidths[k] >> 1);
							Sint32 glintY = (syRandom() & 63) -
								(BattleEffectGlintPictureHeights[k] >> 1);
							Uint32 flip = syRandom();
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS |
									MegaeraGlintFlips[flip & 3],
								MegaeraGlintModes[args->variant]);
							(RamHeapSlots->ramCodePaired)(canvas,
								MEGAERA_PICTURES + BattleEffectGlintPictures[k],
								glintX - 4, glintY + 40,
								BattleEffectGlintPictureWidths[k],
								BattleEffectGlintPictureHeights[k]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
					}
				}
			}
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				BLITTER_ADDITIVE);
			rects[1] = RamHeapSlots->ramCodePaired;
			if(frm <= 69) {
				for(i = 0, p = PARTICLES; i != MEGAERA_SPARK_COUNT; i++, p++) {
					Sint32 size;

					if(p->variant < 0) {
						continue;
					}
					size = i % 3 + 1;
					blitters[p->velY > 0](canvas,
						sprs + BattleEffectMotePictures[size - 1],
						FIXED_INTEGER_PART(p->x) - size / 2,
						FIXED_INTEGER_PART(p->y) - size, size, size * 2);
					btParticleAdvanceWithGravity2D(p, 62, toFIXED(1) / 4);
					p->variant++;
					if(p->velY > 0 &&
						FIXED_INTEGER_PART(p->y) > MEGAERA_SPARK_FLOOR) {
						p->variant = -1;
					}
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	btEffectRunImpactBurst(SUMMON_FORMATION_MEGAERA, pos.x, pos.y);
	for(i = 0; i != MEGAERA_OBJECTS; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

#define VOID_BEAM_TICKS		  88
#define VOID_BEAM_SEGMENTS	  10
/* The hexagon of motes is traced eight motes a tick from tick 42,
   VOID_BEAM_SIDE_MOTES to a side; the ring behind the caster gains three motes
   every two ticks up to VOID_BEAM_RING_MOTES, 40 pixels out. */
#define VOID_BEAM_SIDE_MOTES  36
#define VOID_BEAM_RING_MOTES  64
#define VOID_BEAM_RING_RADIUS 40
/* The beam fires at VOID_BEAM_FIRE_TICK, and fades from
   VOID_BEAM_FADE_TICK. */
#define VOID_BEAM_FIRE_TICK	  72
#define VOID_BEAM_FADE_TICK	  80

/* Where CometSheetB and RuneSheet are decoded in the effect's pictures. */
#define VOID_BEAM_COMET_B_PICTURES 4096
#define VOID_BEAM_RUNE_PICTURES	   4096
/* The beam's body, a 32x32 gradient built after EmberStreakSheet, and
   CometSheetB's comet, its head. */
#define VOID_BEAM_BODY			   6688
#define VOID_BEAM_BODY_SIZE		   32
#define VOID_BEAM_COMET_WIDTH	   32
#define VOID_BEAM_COMET_HEIGHT	   36

/* A hexagon of motes is traced, side by side, around the caster while a ring of
   motes gathers behind; at VOID_BEAM_FIRE_TICK a beam, the comet's head and a
   32-line gradient repeated nine times, shoots from the caster toward the
   targets' side, fading out from VOID_BEAM_FADE_TICK.

   Abilities: Void Beam. */
void btEffectRunVoidBeam(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_CAMERA		*camera;
	Uint8				*sprs;
	Sint32				 point[3];
	SCREEN_POINT		 corners[11];
	SCREEN_POINT		 ring;
	SCREEN_POINT		 segment;
	Sint32				 flip;
	Sint32				 cnt;
	Uint8				*row;
	Sint32				 i;
	Sint32				 k;
	Uint8				*buf;
	Sint32				 size;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	sprs = RamHeapSlots->battleSheet;
	buf = RamMapMetatileBuffer;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(
		RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_COMET_SHEET_B,
		state->sheet + VOID_BEAM_COMET_B_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);

	/* The motes, one shade dimmer. */
	for(i = 0; i != MOTE_SET_BYTES; i++) {
		Sint32 pixel = sprs[i];

		if(pixel != 0) {
			pixel -= 8;
			if(pixel <= 0) {
				pixel = 1;
			}
			sprs[i] = pixel;
		}
	}
	btResourceLoadAndDecompress(
		RESOURCE_RUNE_SHEET, buf + VOID_BEAM_RUNE_PICTURES, 1, 0);
	syWordsClearEntry(buf, RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE);

	/* The beam's head: the rune, mirrored to 48 columns. */
	for(i = 0; i != RUNE_HALF_HEIGHT; i++) {
		Sint32 j;

		for(j = 0; j != RUNE_HALF_WIDTH; j++) {
			buf[RUNE_TEXTURE_RUNE + i * RUNE_TEXTURE_SIZE + j] =
				buf[VOID_BEAM_RUNE_PICTURES + i * RUNE_HALF_WIDTH + j];
			buf[RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1 +
				i * RUNE_TEXTURE_SIZE - j] =
				buf[VOID_BEAM_RUNE_PICTURES + i * RUNE_HALF_WIDTH + j];
		}
	}
	syWordsCopy((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
		SHEET_PALETTE_BYTES);

	/* The beam's body: a 32-line gradient, rising over a quarter turn of a
	   sine. */
	row = state->sheet + VOID_BEAM_BODY;
	for(i = 0; i != VOID_BEAM_BODY_SIZE; i++) {
		Sint32 shade =
			FIXEDtoINT(sySin(i * (DEGtoANG(90) / VOID_BEAM_BODY_SIZE)) *
				CANVAS_LEVEL_BRIGHTEST);
		Sint32 j;

		for(j = 0; j != VOID_BEAM_BODY_SIZE; j++) {
			*row++ = shade;
		}
	}
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	/* Each picture of the beam is drawn above its line and again upside down
	   below it, turned to face the targets. */
	flip = COMPOSITE_FLIP_X;
	if(args->side != EFFECT_SIDE_ENEMIES) {
		flip = 0;
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, flip | EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		flip | EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	draw[1] = RamHeapSlots->ramCodePaired;
	state->cameraDelta = 0;
	state->cameraPhase = 0;
	sySetEvent(btCameraApplyPhasedDelta, EVENT_PASS_TICK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	sySndPlayStub(SE_SPIN);
	size = 4;

	for(frm = 0; frm != VOID_BEAM_TICKS; frm++) {
		/* The camera turns a 256th of a turn a tick from tick 9, and eases to
		   a stop from 64 to 79; the other way round over the party. */
		if((Uint32)(frm - 9) <= 54) {
			state->cameraDelta = -(TRIG_TURN / 256);
		} else if((Uint32)(frm - 64) <= 15) {
			state->cameraDelta = frm * 16 - 1280;
		} else {
			state->cameraDelta = 0;
		}
		if(args->side == EFFECT_SIDE_PARTY) {
			state->cameraDelta = -state->cameraDelta;
		}
		if(frm > 79) {
			Sint32 fade = frm * 2 - 160;

			REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
		}
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);
		{
			MOTION_OBJECT *caster;
			Sint32		   traced;

			caster = btObjectGetStub(args->user)->obj;
			if(caster->x > 0) {
				point[0] = caster->x - toFIXED(8);
			} else {
				point[0] = caster->x + toFIXED(8);
			}

			traced = frm * 8 - 336;
			if(traced >= 0) {
				for(i = 0; i != 6; i++) {
					Sint32 corner = (i + 4) % 6;

					point[1] =
						toFIXED(VoidBeamCorners[corner][1]) + toFIXED(30);
					point[2] = toFIXED(VoidBeamCorners[corner][0]) + caster->z;
					btEffectPosProjectPoint(point, &corners[i]);
					corners[i].x >>= 1;
				}
				for(i = 0; i != 6; i++) {
					Sint32 motes = traced - i * VOID_BEAM_SIDE_MOTES;

					if(motes > VOID_BEAM_SIDE_MOTES) {
						motes = VOID_BEAM_SIDE_MOTES;
					}
					if(motes < 0) {
						continue;
					}
					for(k = 0; k != motes; k++) {
						Sint32 x = corners[i].x +
							syMulFX(k * (corners[(i + 1) % 6].x - corners[i].x),
								toFIXED(1) / VOID_BEAM_SIDE_MOTES);
						Sint32 y = corners[i].y +
							syMulFX(k * (corners[(i + 1) % 6].y - corners[i].y),
								toFIXED(1) / VOID_BEAM_SIDE_MOTES);

						draw[0](canvas,
							sprs + BattleEffectMotePictures[size - 1],
							x - size / 2, y - size, size, size * 2);
					}
				}
			}

			/* The ring behind the caster. */
			cnt = frm * 3 / 2;
			if(cnt > VOID_BEAM_RING_MOTES) {
				cnt = VOID_BEAM_RING_MOTES;
			}
			for(i = 0; i != cnt; i++) {
				Sint32 ang = i * (TRIG_TURN / VOID_BEAM_RING_MOTES);

				point[1] = caster->y - syCos(ang) * VOID_BEAM_RING_RADIUS +
					toFIXED(32);
				point[2] = caster->z + sySin(ang) * VOID_BEAM_RING_RADIUS;
				btEffectPosProjectPoint(point, &ring);
				ring.x >>= 1;
				/* DEAD: the depth clamped to the range other effects size motes
				   by; nothing reads it, and the ROM keeps the tests and
				   stores. */
				CLAMP(ring.depth, 160, 799);
				draw[1](canvas, sprs + BattleEffectMotePictures[size - 1],
					ring.x - size / 2, ring.y - size, size, size * 2);
			}

			if(frm >= VOID_BEAM_FIRE_TICK) {
				Sint32 fade;

				if(frm == VOID_BEAM_FIRE_TICK) {
					state->particles[0].x = point[0];
					if(state->particles[0].x > 0) {
						state->particles[0].velX = -toFIXED(32);
					} else {
						state->particles[0].velX = toFIXED(32);
					}
					REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
						BGCNT_SCREEN_BASE(7);
				}
				if(frm == VOID_BEAM_FIRE_TICK) {
					state->shakeFrm = 8;
					EFFECT_SHOW_DAMAGE(SE_CRASH);
					btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
					btEffectTintBattler(
						args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				}
				for(i = 0; i != VOID_BEAM_SEGMENTS; i++) {
					point[0] =
						state->particles[0].x + state->particles[0].velX * i;
					point[1] = toFIXED(30);
					point[2] = caster->z;
					btEffectPosProjectPoint(point, &segment);
					segment.x >>= 1;
					if(args->side == EFFECT_SIDE_ENEMIES) {
						segment.x -= 32;
					}
					if(i > 0) {
						draw[0](canvas, state->sheet + VOID_BEAM_BODY,
							segment.x, segment.y - VOID_BEAM_BODY_SIZE,
							VOID_BEAM_BODY_SIZE, VOID_BEAM_BODY_SIZE);
						draw[1](canvas, state->sheet + VOID_BEAM_BODY,
							segment.x, segment.y, VOID_BEAM_BODY_SIZE,
							VOID_BEAM_BODY_SIZE);
					} else if(frm <= 81) {
						draw[0](canvas,
							state->sheet + VOID_BEAM_COMET_B_PICTURES,
							segment.x, segment.y - VOID_BEAM_COMET_HEIGHT,
							VOID_BEAM_COMET_WIDTH, VOID_BEAM_COMET_HEIGHT);
						draw[1](canvas,
							state->sheet + VOID_BEAM_COMET_B_PICTURES,
							segment.x, segment.y, VOID_BEAM_COMET_WIDTH,
							VOID_BEAM_COMET_HEIGHT);
					}
				}
				/* The body fades from VOID_BEAM_FADE_TICK. */
				row = state->sheet + VOID_BEAM_BODY;
				fade = 0;
				if(frm >= VOID_BEAM_FADE_TICK) {
					fade = frm * 8 - 640;
				}
				for(i = 0; i != VOID_BEAM_BODY_SIZE; i++) {
					Sint32 shade =
						(FIXEDtoINT(
							sySin(i * (DEGtoANG(90) / VOID_BEAM_BODY_SIZE)) *
							CANVAS_LEVEL_BRIGHTEST)) -
						fade;
					Sint32 j;

					if(shade < 0) {
						shade = 0;
					}
					for(j = 0; j != VOID_BEAM_BODY_SIZE; j++) {
						*row++ = shade;
					}
				}
			}
		}
		if(state->cameraPhase == 0) {
			state->cameraPhase = 1;
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btCameraApplyPhasedDelta);
	btEffectEndCanvasLayer();
}

#undef MEGAERA_PICTURES
#undef VOID_BEAM_TICKS
#undef VOID_BEAM_SEGMENTS
#undef VOID_BEAM_SIDE_MOTES
#undef VOID_BEAM_RING_MOTES
#undef VOID_BEAM_RING_RADIUS
#undef VOID_BEAM_FIRE_TICK
#undef VOID_BEAM_FADE_TICK
#undef BALL_LIGHTNING_SPARKS_CLEARED
#undef BALL_LIGHTNING_SPARK_COUNT
#undef BALL_LIGHTNING_SPARK_FLOOR
#undef BALL_LIGHTNING_TORNADO_PICTURES
#undef BALL_LIGHTNING_BOLT_PICTURES
#undef BALL_LIGHTNING_BOLT_PICTURE_COUNT
#undef BALL_LIGHTNING_BOLT_PICTURE_WIDTH
#undef BALL_LIGHTNING_BOLT_PICTURE_HEIGHT
#undef BALL_LIGHTNING_BOLT_PICTURE_INSET
#undef BALL_LIGHTNING_BOLTS_SET_UP
#undef BALL_LIGHTNING_BOLTS
#undef BALL_LIGHTNING_TICKS
#undef BALL_LIGHTNING_SIZE
#undef EARTH_FORCE_BALL_SIZE
#undef MEGAERA_SPARK_COUNT
#undef MEGAERA_FLASH_BURST_PICTURES
#undef MEGAERA_LIGHTNING_BOLT_PICTURES
#undef MEGAERA_SPARKLE_DOTS_PICTURES
#undef MEGAERA_TICKS
#undef MEGAERA_SWORDS
#undef MEGAERA_SWORD_GROUND_Y
#undef MEGAERA_SWORD_POINT_BYTES
#undef MEGAERA_SPARK_FLOOR
#undef MEGAERA_BOLT_HEIGHT
#undef MEGAERA_FIRE_TICK
#undef WHIRL_BEAM_WIDTH
#undef WHIRL_BEAM_HEIGHT
#undef MEGAERA_BILLOWS_SET_UP
#undef MEGAERA_BILLOWS
#undef MEGAERA_BILLOW_TICKS
#undef MEGAERA_BILLOW_PICTURES
#undef MEGAERA_UNREAD_FIRST
#undef MEGAERA_UNREAD_COUNT
#undef MEGAERA_SWOOP_TICK
#undef MEGAERA_TRAIL_TICKS
#undef MEGAERA_SPARKLE_TICKS
#undef VOID_BEAM_COMET_B_PICTURES
#undef VOID_BEAM_RUNE_PICTURES
#undef VOID_BEAM_BODY
#undef VOID_BEAM_BODY_SIZE
#undef VOID_BEAM_COMET_WIDTH
#undef VOID_BEAM_COMET_HEIGHT
