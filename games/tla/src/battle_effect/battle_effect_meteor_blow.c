/* Meteor Blow. Part of battle_effect.c. */

/* One star of the sky the meteor flies through: a point on a sphere and the
   picture it is drawn with, or METEOR_BLOW_PIXEL_STAR. */
typedef struct METEOR_STAR {
	Sint32 x;
	Sint32 y;
	Sint32 z;
	Sint32 kind;
} METEOR_STAR;

extern const SCALE MeteorBlowDoubleScale;

/* The sky, drawn over the battle background's image in the map cell buffer:
   METEOR_BLOW_STAR_COUNT stars on a sphere METEOR_BLOW_SKY_RADIUS across,
   centred at (METEOR_BLOW_SKY_CENTER_X, METEOR_BLOW_SKY_CENTER_Y). Every
   METEOR_BLOW_STARS_PER_ORB-th is a glowing orb, one of GlowOrbSetSheet's
   METEOR_BLOW_ORB_PICTURE_COUNT pictures in turn; the rest are single pixels of
   the first orb's middle row, METEOR_BLOW_BIG_ORB_SIZE across. The stars are
   kept in the effect's pictures after the orbs'. */
#define METEOR_BLOW_STAR_COUNT		  256
#define METEOR_BLOW_SKY_RADIUS		  160
#define METEOR_BLOW_SKY_CENTER_X	  120
#define METEOR_BLOW_SKY_CENTER_Y	  64
#define METEOR_BLOW_STARS_PER_ORB	  32
#define METEOR_BLOW_ORB_PICTURE_COUNT 7
#define METEOR_BLOW_PIXEL_STAR		  METEOR_BLOW_ORB_PICTURE_COUNT
#define METEOR_BLOW_BIG_ORB_SIZE	  30
#define METEOR_BLOW_STARS \
	(METEOR_BLOW_BIG_ORB_SIZE * METEOR_BLOW_BIG_ORB_SIZE + 3 * 16 * 16 + \
		10 * 16 + 2 * 8 * 8)
/* The view starts turned up METEOR_BLOW_START_PITCH and round
   METEOR_BLOW_START_YAW, and turns a 256th of a turn a tick on each. */
#define METEOR_BLOW_START_PITCH		 3328
#define METEOR_BLOW_START_YAW		 9472
/* From tick 48 the turn round slows by 4 a tick, until it stops at
   METEOR_BLOW_YAW_STILL_TICK. The turn up would slow by 8 a tick from tick
   160, as if it were to stop at METEOR_BLOW_PITCH_STILL_TICK, but the sky is
   drawn only to tick 115. */
#define METEOR_BLOW_YAW_STILL_TICK	 112
#define METEOR_BLOW_PITCH_STILL_TICK 152

/* The sky's colour, the backdrop's a line at a time over the battle view: each
   line's level is a quarter of its line plus a base, half of state->frm (which
   rises as the meteor comes) over METEOR_BLOW_SKY_BASE_LEVEL; its blue rises
   from level METEOR_BLOW_SKY_BLUE_LEVEL and its green, with half as much red,
   from METEOR_BLOW_SKY_GREEN_LEVEL. */
#define METEOR_BLOW_SKY_BASE_LEVEL	16
#define METEOR_BLOW_SKY_BLUE_LEVEL	32
#define METEOR_BLOW_SKY_GREEN_LEVEL 80

/* The cone of flame the meteor plunges in: METEOR_BLOW_CONE_ROWS rings of two
   corners down an arc of a circle METEOR_BLOW_CONE_RADIUS across, six degrees
   apart, a strip of DustBlobSheet's picture drawn METEOR_BLOW_CONE_STRIPS times
   round it, each METEOR_BLOW_CONE_STRIP_SCROLL texels further down the picture;
   its corners, their projections and its mesh are kept in the effect's
   pictures. */
#define METEOR_BLOW_CONE_ROWS		  16
#define METEOR_BLOW_CONE_RADIUS		  48
#define METEOR_BLOW_CONE_STRIPS		  5
#define METEOR_BLOW_CONE_STRIP_SCROLL 20
#define METEOR_BLOW_CONE_PROJECTED	  22336
#define METEOR_BLOW_CONE_CORNERS	  24384
#define METEOR_BLOW_CONE_MESH		  25408

/* Where DustBlobSheet, BillowTexture and FlameBandTexture are decoded in the
   effect's pictures. */
#define METEOR_BLOW_DUST_BLOB_PICTURES 8192
#define METEOR_BLOW_BILLOW			   16384
#define METEOR_BLOW_FLAME_BAND		   20480

/* The meteor's step on one axis each tick of its flight: its speed times the
   heading's sine or cosine, times 64. */
#define METEOR_BLOW_FLIGHT_STEP(trig, speed) (((trig) * (speed)) >> 10)
/* The meteor's heading starts at DEGtoANG(135); from tick 24 it swings towards
   0 by METEOR_BLOW_HEADING_TURN a tick, as if it had swung since
   METEOR_BLOW_HEADING_TURN_TICK, and stays at 0 from tick 102. */
#define METEOR_BLOW_HEADING_TURN			 286
#define METEOR_BLOW_HEADING_TURN_TICK		 16

/* The landing: the meteor comes down on the foes from METEOR_BLOW_LANDING_X,
   and METEOR_BLOW_SPARKS burst from it at METEOR_BLOW_BURST_TICK. The battle
   background is tinted dark orange for it, its red raised by
   METEOR_BLOW_LANDING_TINT_RED levels and its green and blue lowered. */
#define METEOR_BLOW_TICKS			   50
#define METEOR_BLOW_LANDING_X		   144
#define METEOR_BLOW_BURST_TICK		   16
#define METEOR_BLOW_SPARKS			   256
#define METEOR_BLOW_LANDING_TINT_RED   5
#define METEOR_BLOW_LANDING_TINT_GREEN (-4)
#define METEOR_BLOW_LANDING_TINT_BLUE  (-12)
/* The billows lean back METEOR_BLOW_BILLOW_TILT and the rings of fire
   METEOR_BLOW_RING_TILT. */
#define METEOR_BLOW_BILLOW_TILT		   1000
#define METEOR_BLOW_RING_TILT		   2000

/* Meteor Blow: the screen goes black and the view flies through a sky of stars
   as a burning meteor comes in, growing, and plunges in a cone of flame; then
   it lands on the foes, rings of fire and billows spread from it and sparks fly
   up as every foe is thrown back.

   Abilities: Meteor Blow. */
void btEffectRunMeteorBlow(BATTLE_EFFECT_PARM *args)
{
	Uint8				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*map;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE			 blit[2];
	Uint8					*sprs;
	Uint8					*coneProjected;
	Uint8					*coneCorners;
	Uint8					*coneMesh;
	BATTLE_BACKGROUND_STATE *background;
	METEOR_STAR				*stars;
	Uint16					 pitch;
	Uint16					 yaw;
	FIXED					 x;
	FIXED					 z;
	Sint32					 size;
	FIXED					 scale;
	Sint32					 meteorX;
	Sint32					 meteorZ;
	void					*verts;
	Sint32					 billowX;
	Sint32					 canvasX;
	Sint32					 blank;
	Sint32					 ofs;
	Sint32					 frm;
	Sint32					 arcAng;
	Sint32					 j;
	Sint32					 i;
	Sint32					 pos[3];
	Sint32					 screen[3];
	Sint32					 point[3];
	FIXED					 flightPlace[4];
	FIXED					 meteorPlace[4];
	MODEL_TEXTURE			 landingTexture;
	SCALE					 meteorScale;
	Sint32					 meteorSize[2];
	MODEL_TEXTURE			 texture;

	canvas = Ram_HeapSlots->battleCanvas;
	map = MapGridMetatileBuffer;
	state = Ram_HeapSlots->battleEff;
	sprs = Ram_HeapSlots->battleSheet;
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	blank = 0;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	syCloseEvent(btEffectTransferCanvas);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_ORB_SET_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
		state->sheet + METEOR_BLOW_DUST_BLOB_PICTURES, 1, 1);
	syWordsCopyEntry(sprs,
		state->sheet + METEOR_BLOW_BIG_ORB_SIZE / 2 * METEOR_BLOW_BIG_ORB_SIZE,
		METEOR_BLOW_BIG_ORB_SIZE);
	LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	btGraphicsLoadGreyRamp();
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	IWRAM_FILL_WORDS(map, 256 * 128, CANVAS_FILL_WORD(1));
	syWordsCopyEntry((void *)BG_CHAR_BLOCK(2), map, BATTLE_BACKGROUND_BYTES);
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprs(1, SPRITE_METEOR, 1);
	REG_BLDCNT = blank;
	REG_BG2PA = Q_8_8(1);
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	syWordsCopyEntry((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
	sySetEvent(btEffectTransferFullScreenImage, EVENT_PASS_VBLANK);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	coneProjected = state->sheet + METEOR_BLOW_CONE_PROJECTED;
	coneCorners = state->sheet + METEOR_BLOW_CONE_CORNERS;
	coneMesh = state->sheet + METEOR_BLOW_CONE_MESH;
	background = Ram_HeapSlots->battleBg;
	stars = (METEOR_STAR *)(state->sheet + METEOR_BLOW_STARS);
	/* Default: the flight sets these at tick 0. */
	pitch = 0;
	yaw = 0;
	x = 0;
	z = 0;
	size = 0;
	scale = toFIXED(1);
	btMeshBuildGrid(coneMesh, 2, METEOR_BLOW_CONE_ROWS, 32, 2);
	for(ofs = 0, frm = 0, arcAng = 0; frm != METEOR_BLOW_CONE_ROWS;
		arcAng += DEGtoANG(6), ofs += 2 * sizeof(MODEL_VERTEX), frm++) {
		Sint32		  y = FIXEDtoINT(-syCos(arcAng) * METEOR_BLOW_CONE_RADIUS);
		Sint32		  r = FIXEDtoINT(sySin(arcAng) * METEOR_BLOW_CONE_RADIUS);
		MODEL_VERTEX *v;

		j = 0;
		v = (MODEL_VERTEX *)(coneCorners + ofs);
		for(; j != 2; j++, v++) {
			v->x = FIXEDtoINT(sySin(j * DEGtoANG(45)) * r);
			v->y = y;
			v->z = FIXEDtoINT(syCos(j * DEGtoANG(45)) * r);
		}
	}
	/* The sky: the blitter and the transfer, then the stars on their sphere. */
	{
		Sint32		 transferMode = CANVAS_COPY_AND_DARKEN;
		Sint32		 star;
		Sint32		*starKind;
		METEOR_STAR *cur;

		btEffectBuildBlitter(
			HEAP_SLOT_RAM_CODE, 8, 7, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		blit[0] = Ram_HeapSlots->ramCode;
		/* DEAD: both fields are set again before anything reads them; the ROM
		   keeps the two stores. */
		EFFECT_SET_TRANSFER(state, transferMode, CANVAS_FILL_WORD(4));
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		sySetEvent(btEffectArmMeteorBlowPaletteDMA, EVENT_PASS_VBLANK + 126);
		BgScrollWork[1].x = 0;
		background->busy = 1;
		starKind = &stars->kind;
		cur = stars;
		for(star = 0; star != METEOR_BLOW_STAR_COUNT;
			star++, starKind += sizeof(METEOR_STAR) / sizeof(Sint32), cur++) {
			Sint32 a = (star & (METEOR_BLOW_STAR_COUNT - 1)) * (TRIG_TURN / 2) /
				METEOR_BLOW_STAR_COUNT;
			Sint32 r = METEOR_BLOW_SKY_RADIUS;
			Sint32 b;

			cur->y = sySin(a) * r;
			r = FIXEDtoINT(syCos(a) * r);
			b = syRandom() & (DEGtoANG(180) - 1);
			cur->x = sySin(b) * r;
			cur->z = syCos(b) * r;
			if((star & (METEOR_BLOW_STARS_PER_ORB - 1)) == 0) {
				*starKind = (star / METEOR_BLOW_STARS_PER_ORB) %
					METEOR_BLOW_ORB_PICTURE_COUNT;
			} else {
				*starKind = METEOR_BLOW_PIXEL_STAR;
			}
		}
	}

	/* The flight through the stars. */
	for(frm = 0;
		frm != 160 && !(PerWork.repeat & KEY_A) && !(PerWork.repeat & KEY_B);
		frm++) {
		Sint32 fade;
		Sint32 speed;

		if(frm == 0) {
			x = toFIXED(272);
			z = toFIXED(88);
			size = toFIXED(1) / 64;
			pitch = METEOR_BLOW_START_PITCH;
			yaw = METEOR_BLOW_START_YAW;
			state->frm = 8;
			sySndPlayStub(SE_WAVE);
		}
		if(frm == 120) {
			sySndPlayStub(SE_WAVE_HIGH);
		}
		fade = frm - 96;
		if((Uint32)fade <= 16) {
			REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
		}
		if(frm == 115) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
		}
		if(frm == 116) {
			syCloseEvent(btEffectTransferFullScreenImage);
			sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2CNT,
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7),
				IO_WRITE_HALFWORD);
		}
		if(frm > 128) {
			btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
		}
		if(frm == 117) {
			state->frm = 45;
			EFFECT_FREE_PARTICLES(
				i, state->particles, ARRAY_COUNT(state->particles));
			scale = toFIXED(1);
		}
		/* DEAD: tick never reaches 1600 here; the ROM keeps the compare and
		   branch. */
		if(frm <= 1599) {
			Uint16 *dst = state->backdropLines;
			Sint32	value = state->frm;
			Sint32	base = value / 2 + METEOR_BLOW_SKY_BASE_LEVEL;

			if(frm > 79) {
				state->frm++;
				if(state->frm > 236) {
					state->frm = 236;
				}
			}
			for(i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
				*dst++ = 0;
			}
			for(; i != BATTLE_VIEW_BOTTOM - 1; i++) {
				Sint32 g = i - BATTLE_VIEW_TOP;
				Sint32 b = g / 4;

				value = b + base;
				b = value - METEOR_BLOW_SKY_BLUE_LEVEL;
				g = value - METEOR_BLOW_SKY_GREEN_LEVEL;

				if(b < 0)
					b = 0;
				if(b > RGB_LEVEL_MAX)
					b = RGB_LEVEL_MAX;
				if(g < 0)
					g = 0;
				if(g > RGB_LEVEL_MAX)
					g = RGB_LEVEL_MAX;
				*dst++ = RGB(g >> 1, g, b);
			}
			for(; i != DISPLAY_HEIGHT; i++) {
				*dst++ = 0;
			}
		}

		if(frm > 115) {
			RENDER_MODEL *model;
			Sint32		  shade;
			Sint32		  scroll;
			Sint32		  ang;

			model = btRenderAllocateModelList(1);
			shade = frm * 4 - 528;
			if(shade > 0) {
				shade = 0;
			}
			texture.logWidth = 5;
			texture.logHeight = 5;
			texture.pixels = state->sheet + METEOR_BLOW_DUST_BLOB_PICTURES;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = coneMesh;
			model->vertices = coneProjected;
			model->shade = shade;
			model->scrollU = 0;
			if(frm > 127) {
				Sint32 d = (frm << 6) - toFIXED(1) / 8;

				if(d > toFIXED(1) / 64 - 1) {
					d = toFIXED(1) / 64;
				}
				scale += d;
				syScale(scale, scale, scale);
			}
			i = 0;
			ang = -DEGtoANG(90);
			scroll = frm * 2;
			for(; i != METEOR_BLOW_CONE_STRIPS; i++) {
				model->scrollV = ~scroll & TEXTURE_SCROLL_MASK;
				syInitMatrix();
				syTranslate(-toFIXED(2), -toFIXED(64), 0);
				syScale(toFIXED(2625) / 4096, toFIXED(9317) / 4096, toFIXED(1));
				syScale(scale, scale, scale);
				syRotZ(DEGtoANG(180));
				syRotY(ang);
				btRenderTransformVertices(
					coneCorners, coneProjected, 2 * METEOR_BLOW_CONE_ROWS);
				btRenderDrawModelList(model);
				ang += TRIG_TURN * 3 / 32;
				scroll += METEOR_BLOW_CONE_STRIP_SCROLL;
			}
			syHeapRelease(model);
		} else {
			/* DEAD: tick is at most 115 here, so the second arm never runs;
			   the ROM keeps both compares and the arm. */
			if(frm <= 159) {
				pitch += TRIG_TURN / 256;
			} else if(frm <= 191) {
				pitch = pitch - frm * 8 + METEOR_BLOW_PITCH_STILL_TICK * 8;
			}
			if(frm <= 47) {
				yaw += TRIG_TURN / 256;
			} else if(frm <= 111) {
				yaw = yaw - frm * 4 + METEOR_BLOW_YAW_STILL_TICK * 4;
			}
			pos[0] = METEOR_BLOW_SKY_CENTER_X;
			pos[1] = METEOR_BLOW_SKY_CENTER_Y;
			pos[2] = 0;
			syInitMatrix();
			syMatrixTranslateVector(pos);
			syRotX(pitch);
			syRotY(yaw);
			if(frm <= 112) {
				METEOR_STAR *s = stars;

				for(i = 0; i != METEOR_BLOW_STAR_COUNT; i++, s++) {
					Sint32 kind = s->kind;

					point[0] = FIXED_INTEGER_PART(s->x);
					point[1] = FIXED_INTEGER_PART(s->y);
					point[2] = FIXED_INTEGER_PART(s->z);
					syConvert3Dto2D(point, screen);
					if(screen[2] > 0) {
						if(kind == METEOR_BLOW_PIXEL_STAR) {
							blit[0](MapGridMetatileBuffer,
								sprs + i % METEOR_BLOW_BIG_ORB_SIZE, screen[0],
								screen[1], 1, 1);
						} else {
							blit[0](MapGridMetatileBuffer,
								state->sheet + MeteorBlowPictures[kind],
								screen[0], screen[1],
								MeteorBlowPictureWidths[kind],
								MeteorBlowPictureHeights[kind]);
						}
					}
				}
			} else {
				syWordsClearEntry(
					MapGridMetatileBuffer, BATTLE_BACKGROUND_BYTES);
			}
		}

		meteorSize[1] = size;
		meteorSize[0] = size;
		flightPlace[3] = 0;
		flightPlace[1] = toFIXED(255);
		flightPlace[0] = x;
		flightPlace[2] = z;
		fdRenderApplyProjectedPlacementStub(
			state->sprs[0], flightPlace, meteorSize, 0);
		speed = toFIXED(1) / 32;
		{
			Sint32 a = DEGtoANG(135);

			if(frm > 23) {
				a = a - frm * METEOR_BLOW_HEADING_TURN +
					METEOR_BLOW_HEADING_TURN_TICK * METEOR_BLOW_HEADING_TURN;
				if(a < 0) {
					a = 0;
				}
			}
			if(frm > 87) {
				speed = speed - (frm << 6) + toFIXED(11) / 128;
				if(speed < 0) {
					speed = 0;
				}
			}
			x -= METEOR_BLOW_FLIGHT_STEP(sySin(a), speed);
			z += METEOR_BLOW_FLIGHT_STEP(syCos(a), speed);
			size += toFIXED(1) / 128;
		}
		if(frm > 127) {
			speed = frm * (toFIXED(1) / 8) - toFIXED(16);
			if(speed > toFIXED(2) - 1) {
				speed = toFIXED(2);
			}
			z += speed;
			size += toFIXED(1) / 128;
		} else if(size > toFIXED(1)) {
			size = toFIXED(1);
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	sySynch(1);
	background->busy = 0;
	syCloseEvent(btEffectArmMeteorBlowPaletteDMA);
	if(frm <= 115) {
		syCloseEvent(btEffectTransferFullScreenImage);
	} else {
		syCloseEvent(btEffectTransferCanvas);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	btEffectSetupBlendedDisp();
	/* DEAD: tick 0 sets meteorX and meteorZ again before any read; the ROM
	   keeps both moves. */
	meteorX = 200;
	meteorZ = 0;
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_BG2 | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG0 |
		WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2;
	REG_WIN0V = WIN_RANGE(16, 124);
	REG_WIN1V = WIN_RANGE(0, 136);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = Ram_HeapSlots->ramCode;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_BILLOW_TEXTURE, state->sheet + METEOR_BLOW_BILLOW, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
		state->sheet + METEOR_BLOW_FLAME_BAND, 0, 0);
	LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	for(i = 0; i != METEOR_BLOW_SPARKS; i++)
		PARTICLES[i].variant = 0;
	btPaletteBrightenBattleBg(METEOR_BLOW_LANDING_TINT_RED,
		METEOR_BLOW_LANDING_TINT_GREEN, METEOR_BLOW_LANDING_TINT_BLUE);

	/* The landing: rings of fire and billows spread, and sparks fly. */
	for(frm = 0; frm != METEOR_BLOW_TICKS; frm++) {
		RENDER_MODEL *model;
		Sint32		  ang;
		Sint32		  back;
		Sint32		  shade;
		Sint32		  step;

		if(frm == 0) {
			meteorX = METEOR_BLOW_LANDING_X;
			meteorZ = 0;
		}
		if(frm <= 33) {
			meteorScale = MeteorBlowDoubleScale;
			meteorPlace[3] = 0;
			meteorPlace[1] = toFIXED(255);
			meteorPlace[0] = toFIXED(meteorX);
			meteorPlace[2] = toFIXED(meteorZ);
			if(frm <= 15) {
				meteorX -= 1;
				meteorZ += 9;
			} else {
				meteorZ += 2;
			}
			fdRenderApplyProjectedPlacementStub(
				state->sprs[0], meteorPlace, (Sint32 *)&meteorScale, 0);
		}
		if(frm == 34) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frm == METEOR_BLOW_BURST_TICK) {
			PARTICLE *p;
			Sint32	  count = METEOR_BLOW_SPARKS;

			p = (PARTICLE *)MapGridMetatileBuffer;
			i = 0;
			canvasX = toFIXED(meteorX / 2);
			/* FAKEMATCH(dead): updates of meteorX that cancel out, after it is
			 * halved for the sparks' start. */
			meteorX++;
			meteorX--;
			for(; i != count; i++, p++) {
				Sint32 r;
				Sint32 a;

				/* FAKEMATCH(dead): updates of count. */
				count++;
				count--;
				r = (syRandom() & 511) + count;
				a = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);

				p->x = canvasX;
				p->y = toFIXED(114);
				p->velX = (sySin(a) * r) >> 6;
				p->velY = (syCos(a) * r * 2) >> 6;
				p->variant = (syRandom() & 7) + 24;
			}
			state->shakeFrm = 4;
			sySndPlayStub(SE_CRASH);
			for(i = 0; i != args->tgtCnt; i++) {
				btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_HURL);
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
			}
		}

		verts = syHeapAllocEWRAM(1024);
		model = btRenderAllocateModelList(1);
		shade = 0;
		model->texture = &landingTexture;
		model->vertices = verts;
		model->shade = shade;
		landingTexture.logWidth = 6;
		landingTexture.logHeight = 6;
		landingTexture.pixels = state->sheet + METEOR_BLOW_BILLOW;
		model->kind = MODEL_SHADED_ADDITIVE;
		model->mesh = BattleEffectRingMesh;
		billowX = meteorX / 2 - 60;
		step = frm * (TRIG_TURN / 32);
		i = 0;
		back = -step;
		ang = step;
		for(; i != 4; i++, back += DEGtoANG(45), ang += DEGtoANG(45)) {
			Sint32 start = MeteorBlowWaveStarts[i] + METEOR_BLOW_BURST_TICK;

			if(frm > start) {
				shade = (start - frm) * 8 + 56;

				if(shade > -16) {
					shade = -16;
				}
				if(shade > MODEL_SHADE_INVISIBLE) {
					Sint32 s = (frm - start) * MeteorBlowWaveGrowth[i] * 2000 +
						toFIXED(2673) / 4096;

					model->shade = shade;
					syInitMatrix();
					syTranslate(toFIXED(billowX), toFIXED(40), 0);
					syScale(s, s * MeteorBlowWaveStretch[i], s / 4);
					syRotX(METEOR_BLOW_BILLOW_TILT);
					if(i & 1) {
						syRotY(ang);
					} else {
						syRotY(back);
					}
					btRenderTransformVertices(
						BattleEffectRingCorners, verts, 32);
					btRenderDrawModelList(model);
				}
			}
		}
		model->mesh = BattleEffectCylinderMesh;
		landingTexture.pixels = state->sheet + METEOR_BLOW_FLAME_BAND;
		landingTexture.logWidth = 7;
		landingTexture.logHeight = 7;
		/* FAKEMATCH(dead): meteorX halved by hand, the division's own
		 * instructions written out so the halving is not shared with billowX's;
		 * as meteorX / 2 - CANVAS_SIZE / 2. */
		canvasX = ((meteorX + (Sint32)((Uint32)meteorX >> 31)) >> 1) -
			CANVAS_SIZE / 2;
		for(i = 0; i != 3; i++) {
			Sint32 start = MeteorBlowRingStarts[i] + 8;

			if(frm > start) {
				shade = (start - frm) * 8 + 40;

				if(shade > 0) {
					shade = 0;
				}
				if(shade > MODEL_SHADE_INVISIBLE) {
					Sint32 s = (frm - start) * MeteorBlowRingGrowth[i] * 1500 +
						toFIXED(131) / 512;

					model->shade = shade;
					model->scrollU = ((frm + i * 5) & 7) << 4;
					syInitMatrix();
					syTranslate(
						toFIXED(canvasX), toFIXED(MeteorBlowRingHeights[i]), 0);
					syScale(s, s * 2, s);
					syRotX(METEOR_BLOW_RING_TILT);
					btRenderTransformVertices(
						BattleEffectCylinderCorners, verts, 32);
					btRenderDrawModelList(model);
				}
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);

		{
			PARTICLE *p;

			p = (PARTICLE *)MapGridMetatileBuffer;
			for(i = 0; i != METEOR_BLOW_SPARKS; i++, p++) {
				if(p->variant >= 0) {
					Sint32 n = (p->variant >> 2) + 1;

					blit[0](canvas, sprs + BattleEffectMotePictures[n - 1],
						FIXED_INTEGER_PART(p->x) - n / 2,
						FIXED_INTEGER_PART(p->y) - n, n, n * 2);
					p->variant--;
					btParticleAdvanceWithGravity2D(p, 56, toFIXED(1) / 8);
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	fdSprDestroyStub(state->sprs[0]);
	btEffectEndCanvasLayer();
}
