/* The Thor summon. Part of battle_effect.c. */

#define THOR_OPENING_TICKS 170 /* ticks of the ring, triangle and beam */
#define THOR_STORM_TICKS   128 /* ticks of the pillar, Thor and his bolts */
/* The opening's beam of light, and the storm's bolts. */
#define THOR_BEAM_TICK	   151
#define THOR_BOLTS_TICK	   80

/* The sparks, from the first word after the glint pictures that draw the
   crackles of lightning. */
#define THOR_PARTICLES \
	((PARTICLE *)(RamMapMetatileBuffer + ((GLINT_SET_BYTES + 3) & ~3)))
#define THOR_SPARK_COUNT		910
/* The sparks the pillar of lightning throws up from its foot, each bolt throws
   out, and the glow at Thor's hammer sheds each tick; a spark's mote shrinks a
   size for each THOR_SPARK_SHRINK_TICKS of its life it loses. */
#define THOR_PILLAR_SPARKS		512
#define THOR_BOLT_SPARKS		16
#define THOR_HAMMER_GLOW_SPARKS 4
#define THOR_SPARK_SHRINK_TICKS 16

/* The particle sprites, after the light fan pictures. */
#define THOR_SPRITE_PICTURES 6144

/* The glow that flickers about the head of Thor's hammer while his bolts strike
   (RESOURCE_VIOLET_MOTE_SHEET's picture, after the second part's sprite sheet),
   a quarter drawn four times mirrored. */
#define THOR_HAMMER_GLOW_PICTURE		24576
#define THOR_HAMMER_GLOW_PICTURE_HEIGHT 24
/* The light fan, at the first part's sprite sheet start, drawn twice
   mirrored. */
#define THOR_LIGHT_FAN_WIDTH			32
#define THOR_LIGHT_FAN_HEIGHT			96
/* The second part's sprite sheet: the pillar of lightning in two pictures, then
   the bolts. */
#define THOR_BOLT_WIDTH					72
#define THOR_BOLT_HEIGHT				72

/* Where Thor keeps the dimmed copies of the mote pictures in its sprite sheet,
   which its ring darkens through once the beam has fallen. */
#define THOR_DIMMED_MOTES 10000

#define THOR_SPRITES 9 /* Thor's sprites, in SUMMON_FORMATION_THOR */

/* The canvas point a projected (0, 0) is drawn at; the canvas is shown at twice
   its width, so a projected x is halved first. */
#define THOR_CANVAS_CENTER_X (CANVAS_SIZE / 2)
#define THOR_CANVAS_CENTER_Y 60

/* The ring of motes, and the three lines across it, between the points of
   ThorPoints, measured from the corner of the ring's square. Each line draws
   four motes a tick from THOR_LINES_TICK, the next starting when one is
   whole. */
#define THOR_RING_RADIUS 96
#define THOR_RING_MOTES	 128
#define THOR_LINE_MOTES	 48
#define THOR_LINES_TICK	 64

/* The level the canvas is left at the tick before the beam falls: the sky
   flashes. */
#define THOR_BEAM_FLASH_LEVEL 26
/* The beam of light: a ramp of levels THOR_BEAM_WIDTH wide, brightest in its
   middle, down from THOR_BEAM_TOP, its tip narrowing a pixel each side a row
   over THOR_BEAM_TIP_ROWS rows, and THOR_BEAM_SPEED rows longer each tick; it
   narrows a pixel each side a tick after its first. */
#define THOR_BEAM_WIDTH		  32
#define THOR_BEAM_X			  ((CANVAS_SIZE - THOR_BEAM_WIDTH) / 2)
#define THOR_BEAM_TOP		  16
#define THOR_BEAM_TIP_ROWS	  9
#define THOR_BEAM_SPEED		  20
#define THOR_BEAM_LAST_TICK	  (THOR_BEAM_TICK + 16)

/* A dead loop's step between pillars of lightning, of which Thor strikes one,
   THOR_PILLAR_TICK ticks into the storm. */
#define THOR_PILLAR_INTERVAL 128
#define THOR_PILLAR_TICK	 8

#define THOR_BOLTS			  18 /* bolts, one each 2 ticks from THOR_BOLTS_TICK */
#define THOR_FLASHES		  6 /* the canvas flashes, one each 8 ticks from then */
/* The level each flash fills the canvas to. */
#define THOR_BOLT_FLASH_LEVEL 16

/* The crackles of lightning around Thor: within 8 pixels across and 40 up and
   down of him. */
#define THOR_CRACKLE_RADIUS_X 8
#define THOR_CRACKLE_RADIUS_Y 40

/* The first particles of the effect's state, which the storm fills with random
   offsets and nothing in Thor reads. */
#define THOR_STRAY_PARTICLES 16

/* Launches up to max sparks from a point, each at angle at a random speed. */
#define THOR_LAUNCH_SPARKS(spark, index, startX, startY, angleExpr, max) \
	{ \
		spawned = 0; \
		for(index = 0, spark = THOR_PARTICLES; index != THOR_SPARK_COUNT; \
			index++, spark++) { \
			if(spark->variant == 0) { \
				Sint32 speed = (syRandom() & 0x3ff) + 32; \
				Sint32 ang = angleExpr; \
				spark->x = startX; \
				spark->y = startY; \
				spark->velX = (sySin(ang) * speed) >> 7; \
				spark->velY = -((syCos(ang) * speed) << 1) >> 7; \
				spark->variant = (syRandom() & 7) + 32; \
				if(++spawned == (max)) { \
					break; \
				} \
			} \
		} \
	}

/* Over scrolling dusk clouds a ring with a triangle across it draws itself and
   turns, its colours cycling, then tips flat, sinks into the sky and sends down
   a beam of light; then a pillar of lightning strikes the targets, Thor appears
   in a spray of sparks, bolts of lightning from his hammer strike the targets
   again and again, and Thor drifts back in crackling lightning.

   Abilities: Thor. */
void btEffectRunThor(BATTLE_EFFECT_PARM *args)
{
	Uint8					 ramp[THOR_BEAM_WIDTH];
	FIXED					 point[3];
	SCREEN_POINT			 screen[11];
	Sint32					 pos[3];
	Sint32					 scale[3];
	BATTLE_BACKGROUND_STATE *view;
	BATTLE_EFFECT_STATE		*state;
	void					*canvas;
	Uint8					*sprs;
	Sint32					 frm;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	Sint32		   size;
	Sint32		   cnt;
	Sint32		   i;
	Sint32		   j;
	Sint32		   moteCnt;
	Sint32		   spawned;
	PARTICLE	  *spark;

	view = RamHeapSlots->battleBg;
	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	sprs = RamHeapSlots->battleSheet;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btResourceLoadAndDecompress(RESOURCE_LIGHT_FAN_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_TALL_GLOW_SHEET,
		state->sheet + THOR_SPRITE_PICTURES, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);

	{
		Sint32 ofs;
		for(i = 0, ofs = 0; i != DIMMED_COPIES; i++) {
			Uint8 *src;
			for(j = 0, src = sprs; j != MOTE_SET_BYTES; j++) {
				Sint32 v = *src++;

				EFFECT_CAP_LEVEL(v, DIMMED_COPY_CAP(i));
				(state->sheet + THOR_DIMMED_MOTES)[ofs + j] = v;
			}
			ofs += j;
		}
	}

	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7) |
		BGCNT_WRAP;
	state->scrollTmr = 0;
	state->scrollInterval = 2;
	state->scrollStepX = 1;
	state->scrollStepY = 0;
	view->busy = 1;
	sySetEvent(btEffectAdvanceScrollOnInterval, EVENT_PASS_VBLANK);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	state->transferMode = CANVAS_COPY;
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_DUSK_CLOUDS_BACKDROP, 0);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
	REG_BG2PA = Q_8_8(0.5);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit[0] = RamHeapSlots->ramCode;
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX - 1);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* One row of the beam, its edges dimmest. */
	for(i = 0; i != THOR_BEAM_WIDTH / 2; i++) {
		ramp[i] = i * 8 + 3;
		ramp[THOR_BEAM_WIDTH - 1 - i] = i * 8 + 3;
		if(ramp[i] > CANVAS_LEVEL_BRIGHTEST) {
			ramp[i] = CANVAS_LEVEL_BRIGHTEST;
		}
		if(ramp[THOR_BEAM_WIDTH - 1 - i] > CANVAS_LEVEL_BRIGHTEST) {
			ramp[THOR_BEAM_WIDTH - 1 - i] = CANVAS_LEVEL_BRIGHTEST;
		}
	}
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	syWordsCopy((void *)BG_PLTT, syResourceGetTableEntry(RESOURCE_BLAST_SHEET),
		SHEET_PALETTE_BYTES);

	/* First part (A or B skips it): the ring, the triangle and the beam. */
	for(frm = 0; frm != THOR_OPENING_TICKS; frm++) {
		size = 2;
		cnt = frm * 2;
		if(frm == 16) {
			sySndPlayStub(SE_SPIN);
		}
		if(frm == 132) {
			sySndPlayStub(SE_PSYNERGY_SPARKLE);
		}
		if(frm == THOR_BEAM_TICK) {
			sySndPlayStub(SE_CRASH);
		}
		if(PerWork.repeat & (KEY_A | KEY_B)) {
			break;
		}
		/* The colours cycle, stepping twice a tick toward the blast sprite
		   sheet's, then the Mercury, Mars and Jupiter Djinn sprite sheets', 16
		   ticks each. */
		for(i = 0; i != 2; i++) {
			if((frm & 63) <= 15) {
				btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);
			} else if((frm & 63) <= 31) {
				btEffectStepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
			} else if((frm & 63) <= 47) {
				btEffectStepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
			} else {
				btEffectStepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
			}
		}
		if(frm == 150) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
				CANVAS_FILL_WORD(THOR_BEAM_FLASH_LEVEL));
		} else {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
		}
		sySetScreenDist(BATTLE_CAMERA_DISTANCE,
			syDivFX(BATTLE_CAMERA_DISTANCE, toFIXED(3) / 4), BATTLE_FAR_DEPTH);
		syInitMatrix();
		if(frm > 128) {
			Sint32 sinkFrm = frm - 128;
			if(sinkFrm > 22) {
				sinkFrm = 20;
			}
			SET_VECTOR(pos, 0, -sinkFrm * toFIXED(2), toFIXED(512));
			size = (sinkFrm >> 2) + 2;
			if(size > 8) {
				size = 8;
			}
			syMatrixTranslateVector(pos);
			syRotX(-DEGtoANG(22.5));
			syRotZ(DEGtoANG(180));
			syRotX(DEGtoANG(180));
			syRotY(sinkFrm * (TRIG_TURN / 16));
			if(frm > 150) {
				/* From the beam the ring spreads, from 37/64 of its size by
				   3/32 a tick. */
				SET_VECTOR(scale,
					(frm * 2 + frm) * (toFIXED(1) / 32) - toFIXED(869) / 64,
					scale[0], scale[0]);
				size = 5;
			} else {
				SET_VECTOR(scale, toFIXED(1) - sinkFrm * 3 * (toFIXED(1) / 64),
					scale[0], scale[0]);
			}
			syMatrixScaleVector(scale);
		} else {
			SET_VECTOR(pos, 0, 0, toFIXED(256));
			syMatrixTranslateVector(pos);
			syRotX(-DEGtoANG(22.5));
			syRotZ(frm * (TRIG_TURN / 256));
			syRotX(frm * (TRIG_TURN / 256));
		}

		/* The three lines, drawn mote by mote from end to end. */
		if(frm <= 149) {
			for(i = 0; i != 6; i++) {
				SET_VECTOR(point,
					INTtoFIXED(ThorPoints[i][0] - THOR_RING_RADIUS), 0,
					INTtoFIXED(ThorPoints[i][1] - THOR_RING_RADIUS));
				btEffectPosProjectPoint(point, &screen[i]);
				screen[i].x =
					(FIXEDtoINT(screen[i].x) >> 1) + THOR_CANVAS_CENTER_X;
				screen[i].y = FIXEDtoINT(screen[i].y) + THOR_CANVAS_CENTER_Y;
			}
			for(i = 0; i != 3; i++) {
				moteCnt = cnt * 2 - i * THOR_LINE_MOTES - THOR_LINES_TICK * 4;
				if(moteCnt > THOR_LINE_MOTES) {
					moteCnt = THOR_LINE_MOTES;
				}
				if(moteCnt >= 0) {
					for(j = 0; j != moteCnt; j++) {
						Sint32 x = screen[i * 2].x +
							syMulFX((screen[i * 2 + 1].x - screen[i * 2].x) * j,
								toFIXED(1) / THOR_LINE_MOTES);
						Sint32 y = screen[i * 2].y +
							syMulFX((screen[i * 2 + 1].y - screen[i * 2].y) * j,
								toFIXED(1) / THOR_LINE_MOTES);
						EFFECT_DRAW_MOTE(blit[0], canvas, sprs, x, y, size);
					}
				}
			}
		}

		/* The ring of motes, two more each tick, darkening once the beam has
		   fallen. */
		if(frm <= 179) {
			DRAW_RECTANGLE drawAlt;
			Sint32		   level = 0;

			if(frm > 155) {
				level = frm - 156;
			}
			if(level > DIMMED_COPIES - 1) {
				level = DIMMED_COPIES - 1;
			}
			if(frm <= 139) {
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
				drawAlt = RamHeapSlots->ramCodePaired;
			} else {
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
				drawAlt = RamHeapSlots->ramCodePaired;
			}
			moteCnt = cnt;
			if(moteCnt > THOR_RING_MOTES) {
				moteCnt = THOR_RING_MOTES;
			}
			for(i = 0; i != moteCnt; i++) {
				Sint32 moteSize = size + 1;
				point[0] =
					sySin(i * (TRIG_TURN / THOR_RING_MOTES)) * THOR_RING_RADIUS;
				point[2] = -(syCos(i * (TRIG_TURN / THOR_RING_MOTES)) *
					THOR_RING_RADIUS);
				btEffectPosProjectPoint(point, &screen[0]);
				screen[0].x =
					(FIXEDtoINT(screen[0].x) >> 1) + THOR_CANVAS_CENTER_X;
				screen[0].y = FIXEDtoINT(screen[0].y) + THOR_CANVAS_CENTER_Y;
				drawAlt(canvas,
					state->sheet +
						(level * MOTE_SET_BYTES +
							BattleEffectMotePictures[moteSize - 1]) +
						THOR_DIMMED_MOTES,
					screen[0].x - moteSize / 2, screen[0].y - moteSize,
					moteSize, moteSize * 2);
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}

		/* The beam, and the light fan falling down it. */
		if(frm >= THOR_BEAM_TICK && frm <= THOR_BEAM_LAST_TICK) {
			Sint32 beamHeight;
			Sint32 inset = 0;
			Sint32 flg;

			beamHeight = (frm - THOR_BEAM_TICK) * THOR_BEAM_SPEED + 44;
			if(frm > THOR_BEAM_TICK) {
				inset = frm - THOR_BEAM_TICK - 1;
			}
			if(inset > THOR_BEAM_WIDTH / 2 - 1) {
				inset = THOR_BEAM_WIDTH / 2 - 1;
			}
			for(i = 1; i != THOR_BEAM_TIP_ROWS + 1; i++) {
				if(inset + i <= THOR_BEAM_WIDTH / 2 - 1) {
					blit[0](canvas, &ramp[inset + i], inset + THOR_BEAM_X + i,
						THOR_BEAM_TOP - i, THOR_BEAM_WIDTH - inset * 2 - i * 2,
						1);
				}
			}
			for(i = 0; i != beamHeight; i++) {
				blit[0](canvas, ramp + inset, inset + THOR_BEAM_X,
					i + THOR_BEAM_TOP, THOR_BEAM_WIDTH - inset * 2, 1);
			}
			flg = EFFECT_CLIP_BOUNDS;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flg, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas, state->sheet,
				CANVAS_SIZE / 2 - THOR_LIGHT_FAN_WIDTH,
				(frm - THOR_BEAM_TICK) * THOR_BEAM_SPEED - 12,
				THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			flg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flg, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas, state->sheet, CANVAS_SIZE / 2,
				(frm - THOR_BEAM_TICK) * THOR_BEAM_SPEED - 12,
				THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	/* Second part: the pillar, Thor and his bolts. */
	view->busy = 0;
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	btEffectSetupBlendedDisp();
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprs(THOR_SPRITES, SPRITE_THOR, 1);
	btResourceLoadAndDecompress(
		RESOURCE_TORNADO_SHEET, RamMapMetatileBuffer, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_LIGHTNING_PILLAR_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_VIOLET_MOTE_SHEET,
		state->sheet + THOR_HAMMER_GLOW_PICTURE, 1, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_CRYSTAL_SHEET), SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_PALE_BLUE_GLOW_SHEET, sprs, 0, 0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	{
		FIXED  thorX = toFIXED(80);
		FIXED  thorY = toFIXED(64);
		Sint32 thorSpeedX = 0;
		Sint32 thorSpeedY = 0;
		Sint32 pillarX;
		Sint32 pillarY;

		for(i = 0; i != THOR_SPARK_COUNT; i++) {
			THOR_PARTICLES[i].variant = 0;
		}
		/* Nothing in Thor reads these THOR_STRAY_PARTICLES particles, and
		   btEffectRunImpactBurst overwrites them; only their random draws
		   change what is drawn later. */
		for(i = 0; i != THOR_STRAY_PARTICLES; i++) {
			state->particles[i].x = (syRandom() & 31) - 16;
			state->particles[i].y = syRandom() & 63;
			state->particles[i].variant = -(syRandom() & 15);
		}
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		sySndPlayStub(SE_CRASH);

		for(frm = 0; frm != THOR_STORM_TICKS; frm++) {
			Sint32 stormFrm;
			if(frm == 20) {
				btResourceLoadAndDecompress(
					RESOURCE_VIOLET_LIGHTNING_SHEET_B, state->sheet, 1, 0);
			}
			stormFrm = frm - THOR_BOLTS_TICK;
			if(stormFrm >= 0 && stormFrm <= 59 && !(frm & 7)) {
				sySndPlayStub(SE_HEAVY_IMPACT);
			}
			if(frm == 120) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
			/* The pillar of lightning, throwing sparks up from its foot. */
			/* DEAD: a loop over one pillar, j being 0; the ROM keeps its
			   counter, the step of start by THOR_PILLAR_INTERVAL and its exit
			   test. */
			for(j = 0; j != 1; j++) {
				Sint32 start = j * THOR_PILLAR_INTERVAL + THOR_PILLAR_TICK;
				if(frm >= start && frm < j * THOR_PILLAR_INTERVAL + 17) {
					pillarX = 60;
					pillarY = LIGHTNING_PILLAR_HEIGHT;
					if(frm >= j * THOR_PILLAR_INTERVAL + 9 &&
						frm < j * THOR_PILLAR_INTERVAL + 12) {
						blit[0](canvas, state->sheet, 36, 0,
							LIGHTNING_PILLAR_WIDTH, pillarY);
					}
					if(frm >= j * THOR_PILLAR_INTERVAL + 12 &&
						frm < start + 8) {
						blit[0](canvas,
							state->sheet + PICTURE_BYTES(LIGHTNING_PILLAR), 36,
							0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
					}
					if(frm == start + 2) {
						THOR_LAUNCH_SPARKS(spark, i, toFIXED(pillarX),
							toFIXED(pillarY),
							(syRandom() & 0x7fff) - DEGtoANG(90),
							THOR_PILLAR_SPARKS);
						state->shakeFrm = 8;
					}
				}
			}
			if(frm == 10) {
				for(i = 0; i != args->tgtCnt; i++) {
					EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_HURL);
				}
			}
			/* The bolts, each striking the targets, and the glow at Thor's
			   hammer. */
			if(stormFrm >= 0 && stormFrm <= 63) {
				Sint32 glowY;
				Sint32 glowX;
				for(i = 0; i != THOR_FLASHES; i++) {
					if(frm == i * 8 + THOR_BOLTS_TICK) {
						syWordsFillEntry(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(THOR_BOLT_FLASH_LEVEL));
					}
				}
				for(i = 0; i != THOR_BOLTS; i++) {
					if(frm >= i * 2 + THOR_BOLTS_TICK) {
						if(frm < i * 2 + THOR_BOLTS_TICK + 2) {
							Sint32 boltX = (syRandom() & 3) - 3;
							Sint32 boltY = (syRandom() & 3) + 32;
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								EFFECT_CLIP_BOUNDS, BLITTER_MAX);
							(RamHeapSlots->ramCodePaired)(canvas,
								state->sheet + i % 3 * PICTURE_BYTES(THOR_BOLT),
								boltX, boltY, THOR_BOLT_WIDTH,
								THOR_BOLT_HEIGHT);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
						if(frm == i * 2 + THOR_BOLTS_TICK) {
							THOR_LAUNCH_SPARKS(spark, j, toFIXED(16),
								toFIXED(80), syRandom() & TRIG_ANGLE_MASK,
								THOR_BOLT_SPARKS);
							state->shakeFrm = 8;
							for(j = 0; j != args->tgtCnt; j++) {
								btEffectTintBattler(args->tgts[j],
									ThorTargetPalettes[syRandom() & 3],
									POSE_DAMAGE, j, 4);
								btMotionApplyKnockbackStub(
									args->tgts[j], KNOCKBACK_HURL);
							}
						}
					}
				}
				{
					Sint32 radius = (syRandom() & 7) + 8;
					Sint32 ang = syRandom() & TRIG_ANGLE_MASK;
					glowX = FIXEDtoINT(sySin(ang) * radius) + 72;
					glowY = 32 - FIXEDtoINT(syCos(ang) * radius);
				}
				btCanvasBlitMirroredQuarters(
					state->sheet + THOR_HAMMER_GLOW_PICTURE, glowX, glowY,
					THOR_HAMMER_GLOW_PICTURE_HEIGHT);
				/* Sparks fly off the glow. */
				spawned = 0;
				for(i = 0, spark = THOR_PARTICLES; i != THOR_SPARK_COUNT;
					i++, spark++) {
					if(spark->variant == 0) {
						Sint32 speed = (syRandom() & 63) + 64;
						Sint32 ang = syRandom() & TRIG_ANGLE_MASK;
						spark->x = toFIXED(glowX);
						spark->y = toFIXED(glowY);
						spark->velX = (sySin(ang) * speed) >> 7;
						spark->velY = -((syCos(ang) * speed) << 1) >> 7;
						spark->variant = (syRandom() & 7) + 16;
						if(++spawned == THOR_HAMMER_GLOW_SPARKS) {
							break;
						}
					}
				}
			}

			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
			{
				/* Sparks slow and bounce off the ground, shrinking as they
				   die. */
				DRAW_RECTANGLE draw = RamHeapSlots->ramCodePaired;
				PARTICLE	  *spark;
				for(i = 0, spark = THOR_PARTICLES; i != THOR_SPARK_COUNT;
					i++, spark++) {
					if(spark->variant > 0) {
						spark->variant--;
						btParticleAdvanceWithGravity2D(spark, 60, 0);
						if(spark->y > toFIXED(120)) {
							spark->velY = -spark->velY / 2;
						} else if((Uint32)spark->x <= toFIXED(127) - 1 &&
							spark->y >= 0) {
							Sint32 x = FIXEDtoINT(spark->x);
							Sint32 y = FIXEDtoINT(spark->y);
							Sint32 moteSize =
								spark->variant / THOR_SPARK_SHRINK_TICKS + 1;
							EFFECT_DRAW_MOTE(
								draw, canvas, sprs, x, y, moteSize);
						}
					}
				}
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);

			/* Thor stands over the targets, then is pushed back. */
			if(frm > 15) {
				if(frm == 32) {
					thorSpeedX = toFIXED(4);
					thorSpeedY = -toFIXED(1) / 4;
				}
				if(frm > 31) {
					thorX += thorSpeedX;
					thorY += thorSpeedY;
					thorSpeedX = thorSpeedX * 60 / DAMPING_SCALE;
					thorSpeedY = thorSpeedY * 60 / DAMPING_SCALE;
				}
				btEffectPlaceFormationSprs(SUMMON_FORMATION_THOR, thorX, thorY);
			}
			/* Lightning crackles about him. */
			if(frm >= 16 && frm <= 143) {
				Sint32 thorScreenX;
				for(i = 0, thorScreenX = FIXEDtoINT(thorX) >> 1; i != 3; i++) {
					Sint32 cell;
					Sint32 ang;
					Sint32 x;
					Sint32 y;
					Uint32 flip;
					cell = i & 3;
					ang = syRandom() & TRIG_ANGLE_MASK;
					x = (FIXEDtoINT(sySin(ang) * THOR_CRACKLE_RADIUS_X)) +
						thorScreenX -
						(BattleEffectGlintPictureWidths[cell] >> 1);
					y = (FIXEDtoINT(syCos(ang) * THOR_CRACKLE_RADIUS_Y)) -
						(BattleEffectGlintPictureHeights[cell] >> 1);
					flip = syRandom();
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | ThorCrackleFlips[flip & 3],
						BLITTER_MAX);
					(RamHeapSlots->ramCodePaired)(canvas,
						RamMapMetatileBuffer + BattleEffectGlintPictures[cell],
						x, y + 56, BattleEffectGlintPictureWidths[cell],
						BattleEffectGlintPictureHeights[cell]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
			if(frm <= 31) {
				btCameraShake(8, 16);
			} else {
				btCameraShake(4, 4);
			}
			EFFECT_END_TICK(state);
		}

		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		syCloseEvent(btEffectTransferCanvas);
		btEffectRunImpactBurst(SUMMON_FORMATION_THOR, thorX, thorY);
	}
	for(i = 0; i != THOR_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

#undef THOR_LAUNCH_SPARKS
#undef THOR_PARTICLES
#undef THOR_HAMMER_GLOW_PICTURE
#undef THOR_DIMMED_MOTES
#undef THOR_SPRITE_PICTURES
#undef THOR_OPENING_TICKS
#undef THOR_STORM_TICKS
#undef THOR_BEAM_TICK
#undef THOR_BOLTS_TICK
#undef THOR_SPARK_COUNT
#undef THOR_PILLAR_SPARKS
#undef THOR_BOLT_SPARKS
#undef THOR_HAMMER_GLOW_SPARKS
#undef THOR_SPARK_SHRINK_TICKS
#undef THOR_HAMMER_GLOW_PICTURE_HEIGHT
#undef THOR_LIGHT_FAN_WIDTH
#undef THOR_LIGHT_FAN_HEIGHT
#undef THOR_BOLT_WIDTH
#undef THOR_BOLT_HEIGHT
#undef THOR_SPRITES
#undef THOR_CANVAS_CENTER_X
#undef THOR_CANVAS_CENTER_Y
#undef THOR_RING_RADIUS
#undef THOR_RING_MOTES
#undef THOR_LINE_MOTES
#undef THOR_LINES_TICK
#undef THOR_BEAM_FLASH_LEVEL
#undef THOR_BEAM_WIDTH
#undef THOR_BEAM_X
#undef THOR_BEAM_TOP
#undef THOR_BEAM_TIP_ROWS
#undef THOR_BEAM_SPEED
#undef THOR_BEAM_LAST_TICK
#undef THOR_PILLAR_INTERVAL
#undef THOR_PILLAR_TICK
#undef THOR_BOLTS
#undef THOR_FLASHES
#undef THOR_BOLT_FLASH_LEVEL
#undef THOR_CRACKLE_RADIUS_X
#undef THOR_CRACKLE_RADIUS_Y
#undef THOR_STRAY_PARTICLES
