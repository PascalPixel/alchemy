/* The Boreas summon. Part of battle_effect.c. */

/* ticks of the crystals, block and spray */
#define BOREAS_OPENING_TICKS	  288
#define BOREAS_TORRENT_TICKS	  146 /* ticks of the torrent */
/* The opening: the crystals crumble three times, the big ice block drops, lands
   on Boreas's head and sinks, and the spray streams out. */
#define BOREAS_CRUMBLE_A_TICK	  32
#define BOREAS_CRUMBLE_B_TICK	  64
#define BOREAS_CRUMBLE_C_TICK	  104
#define BOREAS_BLOCK_DROP_TICK	  160
#define BOREAS_BLOCK_LAND_TICK	  176
#define BOREAS_BLOCK_SINK_TICK	  208
#define BOREAS_SPRAY_TICK		  224
/* The torrent: crystals rise among the targets, then crash down as the streak
   of ice falls. */
#define BOREAS_TORRENT_RISE_TICK  16
#define BOREAS_TORRENT_CRASH_TICK 72

/* The first part's particles in PARTICLES: the crystals of the mass, the shards
   that fly from it in two batches, and the flecks of spray, with a sparkle
   trailing each after them. */
#define BOREAS_MASS_CRYSTALS	64
#define BOREAS_SHARDS			64
#define BOREAS_SPRAY			128
#define BOREAS_SPRAY_SPARKLES	(PARTICLES + BOREAS_SPRAY)
/* The second part: the crystals of the torrent in state->particles and the
   sparkles among them in PARTICLES. */
#define BOREAS_TORRENT_CRYSTALS 64
#define BOREAS_TORRENT_SPARKLES 64

/* IceBlockSheet's first crystal, its big block and its streak. */
#define BOREAS_CRYSTAL_WIDTH	24
#define BOREAS_CRYSTAL_HEIGHT	48
#define BOREAS_BIG_BLOCK_WIDTH	24
#define BOREAS_BIG_BLOCK_HEIGHT 48
#define BOREAS_STREAK_WIDTH		40
#define BOREAS_STREAK_HEIGHT	80

/* The pictures in the ice block sprite sheet that this file draws; the tables
   in battle_effect_table.c read the rest. */
typedef enum ICE_BLOCK_SHEET_PICTURE {
	/* After the three crystals and the four large shards. */
	BOREAS_BIG_BLOCK =
		24 * 48 + 24 * 38 + 20 * 26 + 12 * 10 + 11 * 16 + 8 * 14 + 10 * 15,
	/* After the big block, the three spray pictures and the seven small
	   shards. */
	BOREAS_STREAK = BOREAS_BIG_BLOCK + 24 * 48 + 3 * (16 * 16) + 8 * 4 +
		12 * 4 + 5 * 3 + 7 * 4 + 7 * 6 + 6 * 12 + 7 * 10
} ICE_BLOCK_SHEET_PICTURE;

/* BoreasPoints has an x and a y for each of BOREAS_POINTS crystals: the mass's
   standing crystals, all BOREAS_POINTS at first,
   BOREAS_CRYSTALS_AFTER_CRUMBLE_A after the first crumble,
   BOREAS_CRYSTALS_AFTER_CRUMBLE_B after the second and none after the third,
   and the crystals that rise among the targets. */
#define BOREAS_POINTS					16
#define BOREAS_POINT_X(crystal)			BoreasPoints[crystal][0]
#define BOREAS_POINT_Y(crystal)			BoreasPoints[crystal][1]
#define BOREAS_CRYSTALS_AFTER_CRUMBLE_A 10
#define BOREAS_CRYSTALS_AFTER_CRUMBLE_B 6
/* The ticks a sparkle twinkles: its nine pictures, two ticks each. */
#define BOREAS_SPARKLE_LIFE				18
/* How far left of the mass's crystals those rising among the targets stand. */
#define BOREAS_TARGETS_X_SHIFT			56
/* A spray picture's width and height. */
#define BOREAS_SPRAY_SIZE				16
/* Boreas is three rows of three 32x32 sprites. */
#define BOREAS_SPRITE_ROWS				3
#define BOREAS_SPRITES					(BOREAS_SPRITE_ROWS * BOREAS_SPRITE_ROWS)

/* The screen opens on Boreas standing in a mass of ice crystals over the violet
   sky; the crystals break away, a big ice block drops onto its head and a spray
   of ice streams out to the left. Then a torrent of ice crystals sweeps down
   over the targets, crystals rise among them and burst, and a streak of ice
   falls through them.

   Abilities: Boreas. */
void btEffectRunBoreas(BATTLE_EFFECT_PARM *args)
{
	static SPR_SCALE	 unitScale = {{{toFIXED(1), toFIXED(1)}}};
	UNUSED Sint32		 crystalPoints[32][2];
	SPR_PLACEMENT		 pos;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 tick;
	DRAW_RECTANGLE		 draw;
	DRAW_RECTANGLE		 drawAlt;
	Uint8				*sparkleSheet;
	Sint32				 edgeSpeed;
	Sint32				 row;
	Sint32				 cnt;
	Sint32				 i;
	Sint32				 j;
	Sint32				 shakeX;
	Sint32				 shakeY;
	Sint32				 y;
	Sint32				 dropTick;
	PARTICLE			*spray;
	PARTICLE			*trail;
	SPR_SCALE			 scale;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	sparkleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	edgeSpeed = 1;
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	REG_WIN0H = WIN_RANGE(240, 240); /* an empty window */
	btResourceLoadAndDecompress(RESOURCE_ICE_BLOCK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkleSheet, 0, 0);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	drawAlt = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectShowLivingTargets(state->args);
	sySynch(1);
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
	btEffectCreateSprs(BOREAS_SPRITES, SPRITE_BOREAS, 1);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(i = 0; i != BOREAS_MASS_CRYSTALS; i++) {
		PARTICLES[i].x = (syRandom() & 63) + 32;
		PARTICLES[i].y = (syRandom() & 31) + 120;
		PARTICLES[i].variant = -1;
	}

	/* DEAD: a copy of BoreasPoints into crystalPoints that nothing reads; the
	   ROM keeps the loop and its stores. */
	/* UB: the copy takes 32 rows from a table of 16, so rows 16 to 31 come from
	   the 32 bytes after the table. */
#if FIX_BOREAS_POINTS_OVERRUN
	for(i = 0; i != BOREAS_POINTS; i++) {
#else
	for(i = 0; i != ARRAY_COUNT(crystalPoints); i++) {
#endif
		crystalPoints[i][0] = BOREAS_POINT_X(i);
		crystalPoints[i][1] = BOREAS_POINT_Y(i);
	}

	sySndPlayStub(SE_RUMBLE);

	/* First half: the crystals, the big ice block and the spray. */
	for(tick = 0; tick != BOREAS_OPENING_TICKS; tick++) {
		cnt = BOREAS_POINTS;
		if((PerRepeat & (KEY_A | KEY_B)) && tick > 16) {
			break;
		}

		/* Open the window from the left with a ragged edge. */
		if(tick >= 0 && tick < 16) {
			Uint16 *edge = WINDOW_EDGE_RIGHT;
			if(tick == 1) {
				Uint8 *inset = WINDOW_EDGE_INSETS;
				Sint32 line;
				for(line = 0; line != WINDOW_EDGE_INSET_LINES; line++) {
					inset[line] = syRandom() & 63;
				}
				*edge = 0;
				sySetEvent(btEffectBuildWindowEdgeTable, EVENT_PASS_VBLANK);
			}
			*edge += edgeSpeed;
			edgeSpeed += 3;
			if(tick == 15) {
				syCloseEvent(btEffectBuildWindowEdgeTable);
			}
		}

		if(tick >= BOREAS_CRUMBLE_C_TICK) {
			cnt = 0;
		} else if(tick >= BOREAS_CRUMBLE_B_TICK) {
			cnt = BOREAS_CRYSTALS_AFTER_CRUMBLE_B;
		} else if(tick >= BOREAS_CRUMBLE_A_TICK) {
			cnt = BOREAS_CRYSTALS_AFTER_CRUMBLE_A;
		}

		if(tick <= 167) {
			shakeX = (syRandom() & 3) - 1;
			shakeY = (syRandom() & 3) - 1;
			BgScrollWork[1].x = shakeX;
			BgScrollWork[1].y = shakeY + BATTLE_BACKGROUND_SCROLL_Y;
		} else {
			shakeX = 0;
			shakeY = 0;
			BgScrollWork[1].x = 0;
			BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		}
		if(tick >= BOREAS_BLOCK_LAND_TICK &&
			tick < BOREAS_BLOCK_LAND_TICK + 4) {
			shakeX = -BoreasShakeOffsets[tick - BOREAS_BLOCK_LAND_TICK];
			shakeY = BoreasShakeOffsets[tick - BOREAS_BLOCK_LAND_TICK];
			BgScrollWork[1].x = shakeX;
			BgScrollWork[1].y = shakeY + BATTLE_BACKGROUND_SCROLL_Y;
		}

		for(i = 0; i != cnt; i++) {
			Sint32 picture = i % 3;
			draw(canvas, state->sheet + BoreasCrystalPictures[picture],
				BOREAS_POINT_X(i) - shakeX,
				BOREAS_POINT_Y(i) - BoreasCrystalHeights[picture] - shakeY,
				BoreasCrystalWidths[picture], BoreasCrystalHeights[picture]);
		}

		scale = unitScale;
		/* Just before the block lands, Boreas comes in front of the canvas: the
		   block settles behind its head and sinks out of sight into it. */
		if(tick == BOREAS_BLOCK_LAND_TICK - 2) {
			for(i = 0; i != BOREAS_SPRITES; i++) {
				state->sprs[i]->image.pri = 0;
			}
		}
		if(tick > BOREAS_BLOCK_SINK_TICK) {
			fdSprStartAnimationStub(
				state->sprs[2], BoreasAnimations[(tick / 4) & 3]);
		}

		/* DEAD: stores of the 1.0 the copy of unitScale already holds; the ROM
		   keeps the stores. */
		scale.x = toFIXED(1);
		scale.y = toFIXED(1);
		pos.groundHeight = 0;
		pos.height = toFIXED(255);
		for(row = 0; row != BOREAS_SPRITE_ROWS; row++) {
			for(j = 0; j != BOREAS_SPRITE_ROWS; j++) {
				pos.x = INTtoFIXED(j * 32 + 144 - shakeX);
				pos.y = INTtoFIXED(row * 32 + 76 - shakeY);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[row * BOREAS_SPRITE_ROWS + j], &pos, &scale, 0);
			}
		}

		/* The big ice block falls onto Boreas's head and slowly sinks. */
		dropTick = tick - BOREAS_BLOCK_DROP_TICK;
		if(dropTick >= 0 && dropTick < 158) {
			Sint32 sinkTick = tick - BOREAS_BLOCK_SINK_TICK;
			Sint32 blockX = 80;
			Sint32 blockY = 8;
			if(tick < BOREAS_BLOCK_LAND_TICK) {
				blockX = 96 - dropTick;
				blockY = dropTick * 4 - 56;
			} else if(tick > BOREAS_BLOCK_SINK_TICK) {
				blockY = sinkTick / 4 + 8;
			}
			drawAlt(canvas, state->sheet + BOREAS_BIG_BLOCK, blockX, blockY,
				BOREAS_BIG_BLOCK_WIDTH, BOREAS_BIG_BLOCK_HEIGHT);
		}

		if(tick == BOREAS_CRUMBLE_A_TICK) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		if(tick == BOREAS_CRUMBLE_B_TICK) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		if(tick == BOREAS_CRUMBLE_C_TICK) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		if(tick == BOREAS_BLOCK_LAND_TICK) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		if(tick == BOREAS_SPRAY_TICK + 2) { /* the spray bursts out */
			sySndPlayStub(SE_CRASH);
		}
		syInitMatrix();

		/* Each time crystals crumble, shards fly up. */
		if(tick == BOREAS_CRUMBLE_A_TICK) {
			for(i = 0; i != BOREAS_SHARDS / 2; i++) {
				PARTICLES[i].x = INTtoFIXED((syRandom() & 31) + 68);
				PARTICLES[i].y = INTtoFIXED((syRandom() & 31) + 8);
				PARTICLES[i].velX = ((syRandom() & 127) - 63) << 11;
				PARTICLES[i].velY = ((-syRandom() & 127) - 64) << 11;
				PARTICLES[i].variant = (syRandom() & 15) + 32;
			}
		}
		if(tick == BOREAS_CRUMBLE_B_TICK) {
			for(i = 0; i != BOREAS_SHARDS / 2; i++) {
				PARTICLES[i + BOREAS_SHARDS / 2].x =
					INTtoFIXED(syRandom() % 48 + 60);
				PARTICLES[i + BOREAS_SHARDS / 2].y =
					INTtoFIXED((syRandom() & 31) + 52);
				PARTICLES[i + BOREAS_SHARDS / 2].velX =
					((syRandom() & 127) - 63) << 12;
				PARTICLES[i + BOREAS_SHARDS / 2].velY =
					((-syRandom() & 31) - 32) << 13;
				PARTICLES[i + BOREAS_SHARDS / 2].variant =
					(syRandom() & 15) + 32;
			}
		}
		if(tick == BOREAS_CRUMBLE_C_TICK) {
			for(i = 0; i != BOREAS_SHARDS / 2; i++) {
				PARTICLES[i].x = INTtoFIXED((syRandom() & 63) + 52);
				PARTICLES[i].y = INTtoFIXED((syRandom() & 31) + 72);
				PARTICLES[i].velX = ((syRandom() & 127) - 63) << 11;
				PARTICLES[i].velY = ((-syRandom() & 31) - 32) << 13;
				PARTICLES[i].variant = (syRandom() & 15) + 32;
			}
		}

		if(tick >= BOREAS_CRUMBLE_A_TICK && tick < BOREAS_BLOCK_SINK_TICK) {
			for(i = 0; i != BOREAS_SHARDS; i++) {
				if(PARTICLES[i].variant >= 0) {
					Sint32 picture;
					if(tick > 191) {
						picture = i % 7 + 4;
					} else {
						picture = i & 3;
					}
					drawAlt(canvas,
						state->sheet + BoreasDebrisPictures[picture],
						FIXEDtoINT(PARTICLES[i].x), FIXEDtoINT(PARTICLES[i].y),
						BoreasDebrisWidths[picture],
						BoreasDebrisHeights[picture]);
					PARTICLES[i].x += PARTICLES[i].velX;
					PARTICLES[i].y += PARTICLES[i].velY;
					PARTICLES[i].velY += toFIXED(1) / 8;
				}
			}
		}

		/* The spray streams out to the bottom left, leaving sparkles. */
		if(tick >= BOREAS_SPRAY_TICK) {
			if(tick == BOREAS_SPRAY_TICK) {
				for(i = 0; i != BOREAS_SPRAY; i++) {
					PARTICLES[i].x = toFIXED(72);
					PARTICLES[i].y = toFIXED(56);
					PARTICLES[i].velX = (-(syRandom() & 127) - 64) << 11;
					PARTICLES[i].velY = ((syRandom() & 127) + 16) << 11;
					PARTICLES[i].variant = syRandom();
				}
			}
			for(i = 0; i != BOREAS_SPRAY; i++) {
				if(tick >= i / 4 + BOREAS_SPRAY_TICK) {
					Sint32 picture = i % 3;
					if((i & 1) == 0) {
						draw(canvas,
							state->sheet + BoreasSprayPictures[picture],
							FIXEDtoINT(PARTICLES[i].x),
							FIXEDtoINT(PARTICLES[i].y),
							BoreasSprayWidths[picture],
							BoreasSprayHeights[picture]);
					}
					PARTICLES[i].x += PARTICLES[i].velX;
					PARTICLES[i].y += PARTICLES[i].velY;
					if(FIXEDtoINT(PARTICLES[i].x) < -BOREAS_SPRAY_SIZE ||
						FIXEDtoINT(PARTICLES[i].y) > BATTLE_VIEW_HEIGHT) {
						PARTICLES[i].x = toFIXED(72);
						PARTICLES[i].y = toFIXED(56);
					}
					/* DEAD: a fleck's variant, random from tick 224, is never
					   read (the second half sets 0 to 63 anew); the ROM keeps
					   the count. */
					PARTICLES[i].variant++;
				}
			}
			if(tick == 228) {
				for(i = 0; i != BOREAS_SPRAY; i++) {
					spray = &PARTICLES[i];
					trail = &BOREAS_SPRAY_SPARKLES[i];
					trail->x = spray->x;
					trail->y = spray->y;
					trail->variant = 0;
				}
			}
			for(i = 0, trail = BOREAS_SPRAY_SPARKLES, spray = PARTICLES;
				i != BOREAS_SPRAY; i++, trail++, spray++) {
				if(tick >= i + 228) {
					Sint32 picture = trail->variant / 2 % 9;
					Sint32 size = BattleEffectSparkleSizes[picture];
					draw(canvas,
						sparkleSheet + BattleEffectSparklePictures[picture],
						FIXEDtoINT(trail->x) - size / 2,
						FIXEDtoINT(trail->y) - size / 2, size, size);
					if(++trail->variant == BOREAS_SPARKLE_LIFE) {
						trail->x = spray->x;
						trail->y = spray->y;
						trail->variant = 0;
					}
				}
			}
		}

		EFFECT_PRESENT_CANVAS(state);
	}

	/* Second half: ice crystals rain down on the targets, and more rise among
	   them and burst. */
	for(i = 0; i != BOREAS_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectSetupBlendedDisp();
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);

	for(i = 0; i != BOREAS_TORRENT_CRYSTALS; i++) {
		state->particles[i].x = INTtoFIXED((syRandom() & 127) + 64);
		state->particles[i].z = -toFIXED(16);
		state->particles[i].y = toFIXED(-(syRandom() & 127) - 64);
		state->particles[i].velX = (-(syRandom() & 63) - 127) << 12;
		state->particles[i].velY = ((syRandom() & 63) + 127) << 12;
		state->particles[i].variant = 0;
	}
	for(i = 0; i != BOREAS_TORRENT_SPARKLES; i++) {
		PARTICLES[i].x = syRandom() & 127;
		PARTICLES[i].y = (syRandom() & 63) + i / 2;
		PARTICLES[i].variant = -i / 2;
	}

	for(tick = 0; tick != BOREAS_TORRENT_TICKS; tick++) {
		cnt = 0;
		if(tick == 96) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(tick == BOREAS_TORRENT_RISE_TICK) {
			state->shakeFrm = 32;
		}
		/* Crystals rise among the targets. */
		if(tick > BOREAS_TORRENT_RISE_TICK) {
			cnt = (tick - BOREAS_TORRENT_RISE_TICK) / 2;
			if(cnt > BOREAS_POINTS) {
				cnt = BOREAS_POINTS;
			}
		}
		if(tick >= 9 && tick < BOREAS_TORRENT_CRASH_TICK && (tick & 3) == 0) {
			sySndPlayStub(SE_WATER);
		}
		if(tick == BOREAS_TORRENT_CRASH_TICK) {
			sySndPlayStub(SE_CRASH);
		}
		/* A streak of ice falls from the top right. */
		if(tick > 64) {
			draw(canvas, state->sheet + BOREAS_STREAK, (64 - tick) * 7 + 88,
				tick * 14 - 992, BOREAS_STREAK_WIDTH, BOREAS_STREAK_HEIGHT);
		}
		if(tick < BOREAS_TORRENT_CRASH_TICK) {
			for(i = 0; i != cnt; i++) {
				Sint32 picture = i % 3;
				draw(canvas,
					state->sheet + BoreasRisingCrystalPictures[picture],
					BOREAS_POINT_X(i) - BOREAS_TARGETS_X_SHIFT,
					BOREAS_POINT_Y(i) - BoreasRisingCrystalHeights[picture],
					BoreasRisingCrystalWidths[picture],
					BoreasRisingCrystalHeights[picture]);
			}
		}
		if(tick == BOREAS_TORRENT_CRASH_TICK) {
			for(i = 0; i != BOREAS_TORRENT_CRYSTALS; i++) {
				state->particles[i].x =
					INTtoFIXED(BOREAS_POINT_X(i & (BOREAS_POINTS - 1)) -
						BOREAS_TARGETS_X_SHIFT);
				state->particles[i].y =
					toFIXED(BOREAS_POINT_Y(i & (BOREAS_POINTS - 1)));
				state->particles[i].velX = ((syRandom() & 127) - 63) << 13;
				state->particles[i].velY = (-(syRandom() & 31) - 16) << 14;
			}
			/* DEAD: always true inside the crash's test; the ROM keeps the
			   second compare. */
			if(tick == BOREAS_TORRENT_CRASH_TICK) {
				state->shakeFrm = 4;
			}
		}
		if(tick >= BOREAS_TORRENT_CRASH_TICK) {
			cnt = BOREAS_TORRENT_CRYSTALS / 2;
			if(tick != BOREAS_TORRENT_CRASH_TICK) {
				cnt = BOREAS_TORRENT_CRYSTALS;
			}
			for(i = 0; i != cnt; i++) {
				PARTICLE *crystal = &state->particles[i];
				y = FIXEDtoINT(crystal->y);
				if(y <= 135) {
					Sint32 picture = i % 3;
					draw(canvas, state->sheet + BoreasFallPictures[picture],
						FIXEDtoINT(crystal->x), y - BoreasFallHeights[picture],
						BoreasFallWidths[picture], BoreasFallHeights[picture]);
					btParticleAdvanceWithGravity2D(
						crystal, PARTICLE_DAMPING_NONE, toFIXED(1));
					/* A falling crystal bounces off the battle view's foot. */
					if(FIXEDtoINT(crystal->y) > BATTLE_VIEW_HEIGHT &&
						crystal->velY > toFIXED(8)) {
						crystal->velY = -crystal->velY / 4;
						crystal->y = toFIXED(BATTLE_VIEW_HEIGHT);
						state->shakeFrm = 1;
					}
				}
			}
		}
		/* Crystals rain down. */
		if(tick < BOREAS_TORRENT_CRASH_TICK) {
			for(i = 0; i != BOREAS_TORRENT_CRYSTALS; i++) {
				draw(canvas, state->sheet,
					FIXEDtoINT(state->particles[i].x) -
						BOREAS_CRYSTAL_WIDTH / 2,
					FIXEDtoINT(state->particles[i].y) -
						BOREAS_CRYSTAL_HEIGHT / 2,
					BOREAS_CRYSTAL_WIDTH, BOREAS_CRYSTAL_HEIGHT);
				state->particles[i].x += state->particles[i].velX;
				state->particles[i].y += state->particles[i].velY;
				if(FIXEDtoINT(state->particles[i].y) > BATTLE_VIEW_HEIGHT &&
					tick <= 47) {
					state->particles[i].x = INTtoFIXED((syRandom() & 127) + 64);
					state->particles[i].y = -toFIXED(16);
				}
			}
		}

		/* Each target turns blue and flinches in turn. */
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			if(tick == i * 8 + 32) {
				btEffectTintBattler(state->args->tgts[i],
					SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, TINT_KEEP, 0);
			}
		}

		if(tick > BOREAS_TORRENT_CRASH_TICK) {
			for(i = 0; i != BOREAS_TORRENT_SPARKLES; i++) {
				if(PARTICLES[i].variant >= 0 &&
					PARTICLES[i].variant < BOREAS_SPARKLE_LIFE) {
					Sint32 picture = PARTICLES[i].variant / 2 % 9;
					Sint32 size = BattleEffectSparkleSizes[picture];
					draw(canvas,
						sparkleSheet + BattleEffectSparklePictures[picture],
						PARTICLES[i].x - size / 2, PARTICLES[i].y - size / 2,
						size, size);
				}
				if(++PARTICLES[i].variant == BOREAS_SPARKLE_LIFE &&
					tick <= 127) {
					PARTICLES[i].x = syRandom() & 127;
					PARTICLES[i].y = (syRandom() & 63) + (tick - 54) / 2;
					PARTICLES[i].variant = 0;
				}
			}
		}

		if(tick >= BOREAS_TORRENT_CRASH_TICK &&
			tick < BOREAS_TORRENT_CRASH_TICK + 8) {
			btCameraShake(8, 8);
		} else {
			btCameraShake(2, 2);
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef BOREAS_SPRAY_SPARKLES
#undef BOREAS_POINTS
#undef BOREAS_POINT_X
#undef BOREAS_POINT_Y
#undef BOREAS_CRYSTALS_AFTER_CRUMBLE_A
#undef BOREAS_CRYSTALS_AFTER_CRUMBLE_B
#undef BOREAS_SPARKLE_LIFE
#undef BOREAS_TARGETS_X_SHIFT
#undef BOREAS_SPRAY_SIZE
#undef BOREAS_SPRITE_ROWS
#undef BOREAS_SPRITES
