/* @file */
/* The Azul summon, the effect steps under gravity and the projection of effect
 */
/* positions onto the screen. */
/* Part of battle_effect.c. */

struct MESH_TRIANGLE;
struct STRIP_QUAD;

/* One face of a shard of the shattered battlefield: its three corners and */
/* their texture coordinates in the battlefield's picture. */
typedef struct AZUL_FACE {
	u8 corner[3];
	u8 pad;
	u8 uv[3][2];
	u8 endPad[2];
} AZUL_FACE;

/* Clamps a sprite's projected y to the screen's band. */
#define CLAMP_Y(place)                                                         \
	((place)[2] < -toFIXED(1) ? (void)((place)[2] = -toFIXED(1)) : (void)0,    \
	 (place)[2] > toFIXED(DISPLAY_HEIGHT) - 1                                  \
		 ? (void)((place)[2] = toFIXED(DISPLAY_HEIGHT))                        \
		 : (void)0)

/* Azul's timeline, in frames. The serpent plunges down from frame 0 and the */
/* battlefield shatters behind it; bubbles rise round the foes; the serpent */
/* swims in from the right and circles them while the water sphere closes; */
/* it darts off, the sphere swirls, the serpent rises from below, and the */
/* sphere bursts. A or B from AZUL_BUBBLES_FRAME on skips to the burst. */
#define AZUL_FRAMES		   228
#define AZUL_SHATTER_FRAME 10
#define AZUL_BUBBLES_FRAME 16
#define AZUL_SWIM_FRAME	   54
#define AZUL_SPHERE_FRAME  78 /* the sphere is built; it shows two frames on */
#define AZUL_DART_FRAME	   138
#define AZUL_SWIRL_FRAME   152
#define AZUL_DEEPEN_FRAME  160 /* the water deepens again */
#define AZUL_RISE_FRAME	   168
#define AZUL_BURST_FRAME   186
#define AZUL_DAMAGE_FRAME  214

/* Azul's objects: the serpent's twelve, then sixteen copies of its body */
/* segments 5 to 8 that trail behind them; and the 31 shards the battlefield */
/* breaks into as it plunges down. */
#define AZUL_SERPENT_OBJECTS 12
#define AZUL_BODY_SEGMENT	 5
#define AZUL_BODY_COPIES	 16
#define AZUL_OBJECTS		 (AZUL_SERPENT_OBJECTS + AZUL_BODY_COPIES)
#define AZUL_PARTS			 31
/* A shard's points in Azul_PartPoints: its middle, then its four corners. */
#define AZUL_PART_POINTS 5
/* The point of the canvas the shards fly out from. */
#define AZUL_SHATTER_CENTER 60
/* The bubbles that rise from frame 16, in the particles after the shards. */
#define AZUL_BUBBLE_COUNT 27

/* Azul's pictures and models in the effect's pictures: the shards' faces */
/* after their corners, the sphere, the grid, BlueBeamSheet, and its first */
/* picture, a blue bubble, with every other pixel kept as the canvas's halved */
/* width draws it. */
#define AZUL_FACES		   496
#define AZUL_SPHERE		   2048
#define AZUL_GRID		   3072
#define AZUL_BLUE_BEAM	   11192
#define AZUL_BUBBLE		   11768
#define AZUL_BUBBLE_WIDTH  12
#define AZUL_BUBBLE_HEIGHT 24
/* WispSheet's six 64x32 pictures, turned a quarter and every other row kept: */
/* six 16x64 wisps of foam. */
#define AZUL_WISPS		   12056
#define AZUL_WISP_WIDTH	   16
#define AZUL_WISP_HEIGHT   64
#define AZUL_WISP_PICTURES 6
/* The water sphere: 16 rings of 16 vertices from pole to pole, 48 pixels */
/* round. */
#define AZUL_SPHERE_RINGS		  16
#define AZUL_SPHERE_RING_VERTICES 16
#define AZUL_SPHERE_RADIUS		  48
/* The phase of a bubble's sway, a bubble after the one before. */
#define AZUL_BUBBLE_PHASE_STEP 6678
/* The bubbles show one after another: bubble i waits AZUL_BUBBLE_DELAY times */
/* 1 - cos(i / 128 of a turn) frames, each a little longer than the last. */
#define AZUL_BUBBLE_DELAY 32
/* The water behind the foes: the backdrop's colour on each line of the battle
 */
/* view, its top line these levels of blue, green and red (cyan, the brighter */
/* levels held at the brightest), each channel a level darker every so many */
/* lines down. The water deepens a level every 8 frames to AZUL_WATER_DEPTH, */
/* and from frame 160 a level every 4 frames to AZUL_WATER_DEEPEST. */
#define AZUL_WATER_BLUE	   40
#define AZUL_WATER_GREEN   32
#define AZUL_WATER_RED	   16
#define AZUL_WATER_DEPTH   12
#define AZUL_WATER_DEEPEST 20
/* The drops the sphere bursts into, kept in the map grid's cell buffer. */
#define AZUL_DROPS 256
/* How far FireRingTexture's square is turned about the upright. */
#define AZUL_RING_TURN 13384
/* Where Azul decodes, one after another, WispSheet, CornerRuneASheet, */
/* FireRingTexture and SwirlTexture in the map cell buffer. */
#define AZUL_TEXTURES 16384

/* Azul: the serpent plunges down and the battlefield shatters behind */
/* it, bubbles rise round the foes, the serpent swims in from the right and */
/* closes them in a great sphere of water, and the sphere bursts over the */
/* field in foam and falling drops. */
/* */
/* Abilities: Azul. */
void btEffectRunAzul(BATTLE_EFFECT_PARM *args) {
	u8 *canvas;
	BATTLE_EFFECT_STATE *state;
	u8 *sprites;
	DRAW_RECTANGLE alternateBlit;
	DRAW_RECTANGLE blit;
	s32 frame;
	BATTLE_STATE *battle;
	POINT2_D base;
	POINT2_D pos;
	POINT2_D vel;
	MODEL_VERTEX *meshVertices;
	AZUL_FACE *faces;
	u8 *sphere;
	u8 *grid;
	s32 i;
	s32 j;
	s32 k;
	s32 n; /* a byte along a line of the background, then a shard */
	u32 shatterFrame;
	u32 sphereFrame;
	FIXED place[4];
	MODEL_TEXTURE texture;
	MODEL_TEXTURE scratchTexture;
	PARTICLE *particle;
	RENDER_MODEL *model;
	void *vertices;
	SPR *sprite;
	const u8 *objectX;
	u8 *mapCells;
	u8 *planes;
	s32 wispBottom;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	sprites = Ram_HeapSlots->battleSheet;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(BGCNT_PRIORITY(3));
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						BLITTER_ADDITIVE);
	blit = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btEffectShowLivingTargets(state->args);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	/* The water's colour for each line streams in by DMA, armed early in each
	 */
	/* VBlank ahead of the canvas transfer: in a pass the higher rank runs
	 * first. */
	sySetEvent(btEffectArmPaletteHBlankDma, TASK_PASS_VBLANK + 126);
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	battle = Ram_HeapSlots->battleState;
	meshVertices = (MODEL_VERTEX *)state->sheet;
	faces = (AZUL_FACE *)(state->sheet + AZUL_FACES);
	sphere = state->sheet + AZUL_SPHERE;
	grid = state->sheet + AZUL_GRID;
	{
		u8 *cells = MapGridCellBuffer;

		/* The battle background's 256-colour tiles laid out as one picture, a
		 */
		/* line at a time. */
		for(i = 0; i < BATTLE_BACKGROUND_HEIGHT; i++) {
			for(n = 0; n < BATTLE_BACKGROUND_WIDTH * 8; n += 8 * 8) {
				syWordsCopyEntry(
					cells,
					(void *)(BG_CHAR_BLOCK(2) +
							 ((i / 8 * BATTLE_BACKGROUND_WIDTH + (i & 7)) * 8 +
							  n)),
					8);
				cells += 8;
			}
		}
	}
	{
		u8 *cells = state->sheet + AZUL_BLUE_BEAM;
		u8 *dst;
		s32 count;

		btResourceLoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET, cells, 1, 1);
		i = 0;
		count = PICTURE_BYTES(AZUL_BUBBLE);
		dst = state->sheet + AZUL_BUBBLE;
		for(; i != count; i++) {
			*dst++ = *cells;
			cells += 2;
		}
	}
	sySynch(1);
	btEffectCreateSprs(AZUL_SERPENT_OBJECTS, SPRITE_AZUL, 1);
	for(i = 0; i != AZUL_BODY_COPIES; i++) {
		sprite = btEffectSprAcquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
		state->sprs[i + AZUL_SERPENT_OBJECTS] = sprite;
		syWordsCopyEntry(sprite, state->sprs[AZUL_BODY_SEGMENT + (i & 3)],
						 offsetof(SPR, lastNo));
		((SPR *)state->sprs[i])->image.pri = 1;
	}
	mapCells = MapGridCellBuffer;
	planes = Ram_MapCellBuffer + AZUL_TEXTURES;
	/* The wisps are turned a quarter as they are copied: each row of a picture
	 */
	/* in WispSheet becomes a column of its wisp, from the bottom up. */
	wispBottom = AZUL_WISPS + (AZUL_WISP_HEIGHT - 1) * AZUL_WISP_WIDTH;
	for(frame = 0; frame != AZUL_FRAMES; frame++) {
		/* A or B skips to the burst in a white flash. */
		if((InputWork.repeat & (KEY_A | KEY_B)) &&
		   frame >= AZUL_BUBBLES_FRAME && frame < AZUL_BURST_FRAME) {
			syWordsFillEntry(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			EFFECT_PRESENT_CANVAS(state);
			frame = AZUL_BURST_FRAME;
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = DEGtoANG(90);
			}
		}
		/* The battlefield's colours come up from dark over the first frames. */
		if(frame <= 8) {
			s32 fade = 24 - frame * 3;

			syColBufferScaleStub(battle->pal, (void *)&BG_PLTT_COLOR(6, 0),
								 FIXED_ONE - fade * BACKGROUND_LEVEL_STEP,
								 BATTLE_BACKGROUND_COLORS);
		}
		if(frame == AZUL_BURST_FRAME) {
			base.x = toFIXED(48);
			base.y = toFIXED(66.00005);
			vel.x = 0;
			vel.y = -toFIXED(6);
		}
		if(frame == 0) {
			REG_BLDALPHA = BLEND_ALPHA(16, 0);
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, frame);
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = TRIG_TURN * 3 / 4;
			}
			base.x = toFIXED(120);
			base.y = -toFIXED(120);
			vel.x = 0;
			vel.y = toFIXED(12);
			IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT,
								IO_WRITE_HALFWORD);
			for(n = 0; n != AZUL_PARTS; n++) {
				for(i = 0; i != 4; i++) {
					meshVertices[n * 4 + i].x =
						Azul_PartPoints[n * AZUL_PART_POINTS + i + 1][0] -
						Azul_PartPoints[n * AZUL_PART_POINTS][0];
					meshVertices[n * 4 + i].y =
						Azul_PartPoints[n * AZUL_PART_POINTS + i + 1][1] -
						Azul_PartPoints[n * AZUL_PART_POINTS][1];
					meshVertices[n * 4 + i].z = 0;
				}
				faces[n * 3].corner[1] = 1;
				faces[n * 3].corner[2] = 2;
				faces[n * 3].corner[0] = 0;
				faces[n * 3 + 1].corner[1] = 2;
				faces[n * 3 + 1].corner[2] = 3;
				faces[n * 3 + 1].corner[0] = 0;
				faces[n * 3 + 2].corner[0] = 0;
				faces[n * 3 + 2].corner[1] = 0;
				faces[n * 3 + 2].corner[2] = 0;
				for(i = 0; i != 3; i++) {
					faces[n * 3].uv[i][0] =
						Azul_PartPoints[n * AZUL_PART_POINTS +
										faces[n * 3].corner[i] + 1][0];
					faces[n * 3].uv[i][1] =
						Azul_PartPoints[n * AZUL_PART_POINTS +
										faces[n * 3].corner[i] + 1][1];
					faces[n * 3 + 1].uv[i][0] =
						Azul_PartPoints[n * AZUL_PART_POINTS +
										faces[n * 3 + 1].corner[i] + 1][0];
					faces[n * 3 + 1].uv[i][1] =
						Azul_PartPoints[n * AZUL_PART_POINTS +
										faces[n * 3 + 1].corner[i] + 1][1];
				}
			}
			for(i = 0; i != args->tgtCnt; i++) {
				btObjectGetStub(args->tgts[i])->obj->y = 0;
			}
			REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) |
						 BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		}
		/* The canvas fades in over the shattering. */
		shatterFrame = frame - AZUL_SHATTER_FRAME;
		if(shatterFrame <= BLEND_WEIGHT_MAX) {
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, shatterFrame);
		}
		/* Bubbles rise round the foes for 64 frames. */
		if((u32)(frame - AZUL_BUBBLES_FRAME) <= 63) {
			if(frame == AZUL_BUBBLES_FRAME) {
				s32 mask;

				i = 0;
				mask = 31;
				particle = &state->particles[AZUL_PARTS];
				for(; i != AZUL_BUBBLE_COUNT; i++, particle++) {
					particle->x = INTtoFIXED((syRandom() & 63) + 32);
					particle->y = INTtoFIXED((syRandom() & 15) + 120);
					particle->velX = ((syRandom() & mask) - 16) << 13;
					particle->velY = ((syRandom() & mask) - 63) << 12;
					particle->variant =
						FIXEDtoINT(syCos(i * (TRIG_TURN / 128)) *
								   AZUL_BUBBLE_DELAY) -
						AZUL_BUBBLE_DELAY;
				}
			}
			i = 0;
			particle = &state->particles[AZUL_PARTS];
			for(; i != AZUL_BUBBLE_COUNT; i++, particle++) {
				if(particle->variant >= 0) {
					FIXED sway = sySin(frame * (TRIG_TURN / 64) +
									   i * AZUL_BUBBLE_PHASE_STEP);

					blit(canvas, state->sheet + AZUL_BUBBLE,
						 (FIXEDtoINT(particle->x + (sway << 4))) -
							 AZUL_BUBBLE_WIDTH / 2,
						 FIXED_INTEGER_PART(particle->y) -
							 AZUL_BUBBLE_HEIGHT / 2,
						 AZUL_BUBBLE_WIDTH, AZUL_BUBBLE_HEIGHT);
					btParticleAdvanceWithGravity2D(particle, 63,
												   -toFIXED(1) / 4);
				}
				particle->variant++;
			}
		}
		/* The water behind the foes deepens. */
		if(frame <= 95 || (u32)(frame - AZUL_DEEPEN_FRAME) <= 31) {
			u16 *c = state->backdropLines;
			s32 level = frame / 8;

			if(frame >= AZUL_DEEPEN_FRAME) {
				level = (frame - AZUL_DEEPEN_FRAME) / 4 + AZUL_WATER_DEPTH;
				if(level > AZUL_WATER_DEEPEST) {
					level = AZUL_WATER_DEEPEST;
				}
			}
			else if(level > AZUL_WATER_DEPTH) {
				level = AZUL_WATER_DEPTH;
			}
			for(i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
				*c++ = 0;
			}
			for(; i != BATTLE_VIEW_BOTTOM - 1; i++) {
				s32 depth = i - (BATTLE_VIEW_TOP - 1);
				s32 b =
					-(depth / GRADIENT_BLUE_LINES) - level + AZUL_WATER_BLUE;
				s32 g =
					-(depth / GRADIENT_GREEN_LINES) - level + AZUL_WATER_GREEN;
				s32 r = -(depth / GRADIENT_RED_LINES) - level + AZUL_WATER_RED;

				if(b < 0) {
					b = 0;
				}
				if(b > RGB_LEVEL_MAX) {
					b = RGB_LEVEL_MAX;
				}
				if(g < 0) {
					g = 0;
				}
				if(g > RGB_LEVEL_MAX) {
					g = RGB_LEVEL_MAX;
				}
				if(r < 0) {
					r = 0;
				}
				if(r > RGB_LEVEL_MAX) {
					r = RGB_LEVEL_MAX;
				}
				*c++ = RGB(r, g, b);
			}
			for(; i != DISPLAY_HEIGHT; i++) {
				*c++ = 0;
			}
		}
		/* DEAD: frame never falls below 0; the ROM keeps the compare and
		 * branch. */
		if(frame >= 0) {
			base.x += vel.x;
			base.y += vel.y;
		}
		/* The serpent rises from below and lifts the foes. */
		if(frame == AZUL_RISE_FRAME) {
			base.x = toFIXED(48);
			base.y = toFIXED(168);
			vel.x = 0;
			vel.y = -toFIXED(6);
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = DEGtoANG(90);
			}
			for(i = 0; i != args->tgtCnt; i++) {
				MOTION_OBJECT *actor = btObjectGetStub(args->tgts[i])->obj;

				btEffectStartActorMotion(args->tgts[i], 1, 0, 0, toFIXED(4),
										 100);
				actor->verticalMotionStrength = 0;
			}
		}
		if(frame == AZUL_BURST_FRAME) {
			vel.y = -toFIXED(16);
		}
		/* The serpent upright, plunging and then rising. */
		if((frame < AZUL_SWIM_FRAME || frame >= AZUL_RISE_FRAME) &&
		   frame < AZUL_DAMAGE_FRAME) {
			place[3] = 0;
			place[1] = toFIXED(255);
			if(frame >= AZUL_RISE_FRAME) {
				pos.y -= toFIXED(6);
			}
			if(frame < AZUL_SWIM_FRAME) {
				for(i = 0; i != AZUL_SERPENT_OBJECTS; i++) {
					place[0] = INTtoFIXED(40 - Azul_ObjectX[i]) + base.x;
					place[2] =
						INTtoFIXED(128 - Azul_ObjectZ[i] * 31 / 32) + base.y;
					if(i > 8) {
						place[2] += -toFIXED(240);
					}
					CLAMP_Y(place);
					btRenderApplyProjectedPlacementStub(
						state->sprs[i], place, (s32 *)&Azul_UnitScale, 0);
				}
				for(i = 0; i != AZUL_BODY_COPIES; i++) {
					place[0] =
						INTtoFIXED(39 -
								   Azul_ObjectX[(i & 3) + AZUL_BODY_SEGMENT]) +
						base.x;
					place[2] =
						INTtoFIXED(-(Azul_ObjectZ[(i & 3) + AZUL_BODY_SEGMENT] *
									 30 / 32) -
								   i / 4 * 60) +
						base.y + toFIXED(66);
					CLAMP_Y(place);
					sprite = state->sprs[i + AZUL_SERPENT_OBJECTS];
					sprite->image.tile =
						((SPR *)state->sprs[(i & 3) + AZUL_BODY_SEGMENT])
							->image.tile;
					btRenderApplyProjectedPlacementStub(
						sprite, place, (s32 *)&Azul_UnitScale, 0);
				}
			}
			else {
				for(i = 0; i != AZUL_SERPENT_OBJECTS; i++) {
					place[0] = INTtoFIXED(Azul_ObjectX[i]) + base.x;
					place[2] = INTtoFIXED(Azul_ObjectZ[i]) + base.y;
					if(i > 8) {
						place[2] += toFIXED(256);
					}
					CLAMP_Y(place);
					btRenderApplyProjectedPlacementStub(
						state->sprs[i], place, (s32 *)&Azul_UnitScale, 0);
				}
				i = 0;
				objectX = Azul_ObjectX;
				for(; i != AZUL_BODY_COPIES; i++) {
					s32 segment = i & 3;
					s32 row = segment + AZUL_BODY_SEGMENT;

					place[0] = INTtoFIXED(objectX[row]) + base.x - toFIXED(1);
					place[2] = INTtoFIXED(Azul_ObjectZ[row] + i / 4 * 64) +
							   base.y + toFIXED(64);
					CLAMP_Y(place);
					sprite = state->sprs[i + AZUL_SERPENT_OBJECTS];
					sprite->image.tile =
						((SPR *)state->sprs[segment + AZUL_BODY_SEGMENT])
							->image.tile;
					btRenderApplyProjectedPlacementStub(
						sprite, place, (s32 *)&Azul_UnitScale, 0);
				}
			}
		}
		else if(frame >= AZUL_SWIM_FRAME && frame < AZUL_RISE_FRAME) {
			/* The serpent swims in from the right and circles, then darts off.
			 */
			s32 x;
			s32 y;

			if(frame == AZUL_SWIM_FRAME) {
				for(i = 0; i != AZUL_OBJECTS; i++) {
					((SPR *)state->sprs[i])->rotation = 0;
				}
				base.x = toFIXED(256);
				base.y = toFIXED(72);
				vel.x = -toFIXED(6);
				vel.y = 0;
			}
			if(frame == AZUL_DART_FRAME) {
				vel.x = toFIXED(8);
			}
			place[3] = 0;
			place[1] = toFIXED(255);
			vel.x = vel.x * 56 / DAMPING_SCALE;
			vel.y = vel.y * 56 / DAMPING_SCALE;
			if(frame < AZUL_DART_FRAME) {
				pos.x = (sySin(frame << 9) << 4) + base.x;
				x = pos.x;
				pos.y = (syCos(frame << 9) << 3) + base.y;
				y = pos.y;
			}
			else {
				x = base.x + (sySin(frame << 9) << 4);
				y = (syCos(frame << 9) << 3) + base.y;
			}
			for(i = 0; i != 8; i++) {
				place[0] = INTtoFIXED(Azul_LateObjectX[i]) + x;
				place[2] = INTtoFIXED(Azul_LateObjectZ[i]) + y;
				btRenderApplyProjectedPlacementStub(
					state->sprs[i], place, (s32 *)&Azul_LateUnitScale, 0);
			}
		}
		/* The sphere drifts left as the serpent darts off, then back. */
		if((u32)(frame - AZUL_DART_FRAME) <= 29) {
			s32 drift = -toFIXED(2);

			if(frame >= AZUL_SWIRL_FRAME) {
				drift = frame * (toFIXED(1) / 8) - toFIXED(21);
			}
			pos.x = pos.x + drift;
		}
		if(frame == AZUL_SPHERE_FRAME) {
			sySndPlayStub(SE_SPIN);
		}
		if(frame == AZUL_DART_FRAME) {
			sySndPlayStub(SE_LAUNCH);
		}
		if(frame == AZUL_SWIRL_FRAME) {
			sySndPlayStub(SE_WAVE_HIGH);
		}
		if(frame == 7) {
			sySndPlayStub(SE_POWER_UP);
		}
		/* The battlefield shatters into shards. */
		if(frame == AZUL_SHATTER_FRAME) {
			const u8 *piece;

			sySndPlayStub(SE_CLATTER);
			i = 0;
			particle = state->particles;
			piece = Azul_PartPoints[0];
			for(; i != AZUL_PARTS;
				i++, piece += AZUL_PART_POINTS * 2, particle++) {
				s32 cx = piece[0] >> 1;
				s32 cy = piece[1];
				s32 dx;
				s32 dy;

				SET_POSITION(particle, INTtoFIXED(cx), INTtoFIXED(cy), 0);
				/* A shard flies out a 16th of a pixel a frame across, and a
				 * 64th */
				/* down, for each pixel it lies from the centre. */
				dx = cx - AZUL_SHATTER_CENTER;
				dy = cy - AZUL_SHATTER_CENTER;
				particle->velX = dx * (toFIXED(1) / 16);
				particle->velY = dy * (toFIXED(1) / 64);
				particle->velZ = ((syRandom() & 127) << 2) + 512;
				if(syRandom() & 1) {
					particle->velZ = -particle->velZ;
				}
			}
			IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
								DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
									DISPCNT_BG0_ON | DISPCNT_BG2_ON |
									DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
									DISPCNT_WIN1_ON,
								IO_WRITE_HALFWORD);
		}
		/* The foes are tinted blue and float up at the shattering, and stand
		 * again */
		/* at the burst. */
		if(frame == AZUL_SHATTER_FRAME || frame == AZUL_BURST_FRAME) {
			for(i = 0; i != args->tgtCnt; i++) {
				BATTLE_OBJECT *slot = btObjectGetStub(args->tgts[i]);
				MOTION_OBJECT *actor = slot->obj;
				void *record;

				if(frame == AZUL_BURST_FRAME) {
					btObjectDestroySprsStub(args->tgts[i]);
				}
				if(frame == AZUL_SHATTER_FRAME) {
					for(j = 0; (record = fdActorGetSprStub(slot->obj, j)) != 0;
						j++) {
						fdSprRemoveAnimationStub(record,
												 (s32)slot->shadowEntry);
					}
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_BLUE_TINT,
										POSE_DAMAGE, i, 8);
					btEffectStartActorMotion(args->tgts[i], 1, 0, 0,
											 (syRandom() << 1) + toFIXED(1),
											 100);
					actor->verticalMotionStrength = 0;
				}
			}
			if(frame == AZUL_BURST_FRAME) {
				args->tgts[args->tgtCnt] = BATTLE_UNIT_LIST_END;
				btObjectSpawnObjectsForListStub(args->tgts, 0, 0);
			}
		}
		if(frame >= AZUL_SHATTER_FRAME) {
			for(i = 0; i != args->tgtCnt; i++) {
				MOTION_OBJECT *actor = btObjectGetStub(args->tgts[i])->obj;

				actor->velY = actor->velY * 60 / DAMPING_SCALE;
			}
		}
		/* The shards tumble away until the serpent swims in. */
		if(shatterFrame <= 43) {
			vertices = syHeapAllocEwram(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 8;
			texture.logHeight = 7;
			texture.pixels = mapCells;
			model->texture = &texture;
			model->kind = MODEL_TEXTURED;
			model->vertices = vertices;
			model->scrollU = BgScrollWork[1].x;
			for(i = 0; i != AZUL_PARTS; i++) {
				particle = &state->particles[i];
				model->mesh = &faces[i * 3];
				syInitMatrix();
				syTranslate(particle->x - toFIXED(64),
							particle->y - toFIXED(64), 0);
				syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
				switch(i & 3) {
				case 0:
					syRotX(particle->z);
					syRotZ(particle->z);
					break;
				case 1:
					syRotX(particle->z);
					syRotZ(-particle->z);
					break;
				case 2:
					syRotX(particle->z);
					syRotY(particle->z);
					break;
				case 3:
					syRotZ(particle->z);
					syRotY(-particle->z);
					break;
				}
				btRenderTransformVertices(&meshVertices[i * 4], vertices, 4);
				btParticleAdvanceWithGravity3D(particle, 62, 0);
				if(frame >= (8 - i) * 2 + 16) {
					particle->velY += -toFIXED(1) / 2;
				}
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		/* The sphere's rings, and the foam, rune and ring textures. */
		if(frame == AZUL_SPHERE_FRAME) {
			s32 angle;
			s32 offset;

			btMeshBuildGrid(grid, AZUL_SPHERE_RING_VERTICES, AZUL_SPHERE_RINGS,
							8, 8);
			k = 0;
			angle = 0;
			offset = 0;
			for(; k != AZUL_SPHERE_RINGS;
				angle += DEGtoANG(12),
				offset += AZUL_SPHERE_RING_VERTICES * sizeof(MODEL_VERTEX),
				k++) {
				s32 y = FIXEDtoINT(-syCos(angle) * AZUL_SPHERE_RADIUS);
				s32 radius = FIXEDtoINT(sySin(angle) * AZUL_SPHERE_RADIUS);
				MODEL_VERTEX *v;

				j = 0;
				v = (MODEL_VERTEX *)(sphere + offset);
				for(; j != AZUL_SPHERE_RING_VERTICES; j++) {
					SET_POSITION(
						v,
						FIXEDtoINT(
							sySin(j * (TRIG_TURN / AZUL_SPHERE_RING_VERTICES)) *
							radius),
						y,
						FIXEDtoINT(
							syCos(j * (TRIG_TURN / AZUL_SPHERE_RING_VERTICES)) *
							radius));
					v++;
				}
			}
			btResourceLoadAndDecompress(
				RESOURCE_WISP_SHEET, Ram_MapCellBuffer + AZUL_TEXTURES, 1, 0);
			for(i = 0; i != AZUL_WISP_PICTURES; i++) {
				for(k = 0; k != AZUL_WISP_WIDTH; k++) {
					for(j = 0; j != AZUL_WISP_HEIGHT; j++) {
						state->sheet[wispBottom +
									 (i * AZUL_WISP_HEIGHT - j) *
										 AZUL_WISP_WIDTH +
									 k] = (planes + i * PICTURE_BYTES(WISP) +
										   k * (WISP_WIDTH * 2))[j];
					}
				}
			}
			btResourceLoadAndDecompress(RESOURCE_CORNER_RUNE_A_SHEET,
										(Ram_MapCellBuffer + AZUL_TEXTURES), 1,
										0);
			btPictureMirrorIntoQuadrants(Ram_MapCellBuffer + AZUL_TEXTURES,
										 MapGridCellBuffer, 64, 64);
			btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
										Ram_MapCellBuffer + AZUL_TEXTURES, 0,
										0);
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frame == AZUL_SWIRL_FRAME) {
			btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
										Ram_MapCellBuffer + AZUL_TEXTURES, 0,
										0);
		}
		/* The water sphere grows round the foes. */
		sphereFrame = frame - (AZUL_SPHERE_FRAME + 2);
		if(sphereFrame <= 107) {
			s32 scale;
			s32 shade;
			s32 size;

			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			/* Foam streams up beside the rising serpent. */
			if((u32)(frame - AZUL_RISE_FRAME) <= 17) {
				s32 y;
				s32 wisp;
				s32 width;
				s32 height;

				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
									BLITTER_ADDITIVE);
				alternateBlit = Ram_HeapSlots->ramCodePaired;
				i = 0;
				y = (FIXEDtoINT(base.y)) - 32;
				width = AZUL_WISP_WIDTH;
				height = AZUL_WISP_HEIGHT;
				wisp = frame;
				for(; i != 2; i++, wisp += 3) {
					u8 *cells =
						state->sheet + AZUL_WISPS +
						wisp % AZUL_WISP_PICTURES * PICTURE_BYTES(AZUL_WISP);

					blit(canvas, cells, 30, y, width, height);
					alternateBlit(canvas, cells, 46, y, width, height);
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
			if(frame < AZUL_BURST_FRAME) {
				model = btRenderAllocateModelList(1);
				scale = sphereFrame * 3 << 8;
				shade = 0;
				if(scale > FIXED_ONE) {
					scale = FIXED_ONE;
				}
				scratchTexture.logWidth = 7;
				scratchTexture.logHeight = 7;
				scratchTexture.pixels = mapCells;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &scratchTexture;
				model->mesh = grid;
				model->vertices = meshVertices;
				model->shade = shade;
				model->scrollU = shade;
				model->scrollV = shade;
				syInitMatrix();
				syTranslate(pos.x / 2 - toFIXED(68), pos.y - toFIXED(80), 0);
				syScale(scale, scale * 2, scale);
				syTranslate(-toFIXED(40), 0, 0);
				syRotX(frame * (TRIG_TURN / 64));
				syRotY(frame * (TRIG_TURN / 64));
				btRenderTransformVertices(sphere, meshVertices, 256);
				btRenderDrawModelList(model);
				if(frame >= AZUL_DART_FRAME) {
					shade = 1444 - frame * 10;
					size = frame * (FIXED_ONE / 8) + -toFIXED(17);

					if(shade > 0) {
						shade = 0;
					}
					if(shade > MODEL_SHADE_INVISIBLE) {
						scratchTexture.pixels =
							Ram_MapCellBuffer + AZUL_TEXTURES;
						scratchTexture.logWidth = 7;
						model->mesh = BattleEffect_LargeSquareMesh;
						scratchTexture.logHeight = 7;
						model->shade = shade;
						syInitMatrix();
						syTranslate(toFIXED(44), 0, 0);
						syRotY(AZUL_RING_TURN);
						syMatrixScaleUniform(size);
						btRenderTransformVertices(BattleEffect_SquareCorners,
												  meshVertices, 4);
						btRenderDrawModelList(model);
					}
				}
				if(frame >= AZUL_SWIRL_FRAME) {
					state->shakeFrm = 2;
					shade = 0;
					size = frame * (FIXED_ONE / 8) + -toFIXED(19);
					scratchTexture.logWidth = 6;
					scratchTexture.logHeight = 6;
					scratchTexture.pixels = Ram_MapCellBuffer + AZUL_TEXTURES;
					model->shade = shade;
					model->mesh = BattleEffect_SquareMesh;
					syInitMatrix();
					syTranslate(-toFIXED(16), pos.y - toFIXED(80), 0);
					syScale(size >> 1, size, size >> 1);
					syRotZ(frame * (TRIG_TURN / 32));
					btRenderTransformVertices(BattleEffect_SquareCorners,
											  meshVertices, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
			}
		}
		/* The sphere bursts into drops that fall and bounce. */
		if(frame >= AZUL_BURST_FRAME) {
			if(frame == AZUL_BURST_FRAME) {
				PARTICLE *drop = (PARTICLE *)MapGridCellBuffer;

				for(i = 0; i != AZUL_DROPS; i++, drop++) {
					s32 angle = i * (TRIG_TURN / AZUL_DROPS);
					s32 r = syRandom() & 127;
					s32 vy;

					drop->velX = sySin(angle) * r >> 5;
					vy = syCos(angle) * r >> 4;
					drop->x = drop->velX * 6 + toFIXED(48);
					drop->y = vy * 5;
					drop->velY = vy;
					drop->variant = (syRandom() & 31) + 32;
				}
			}
			particle = (PARTICLE *)MapGridCellBuffer;
			for(i = 0; i != AZUL_DROPS; i++, particle++) {
				if(particle->variant >= 0) {
					blit(canvas, state->sheet + AZUL_BUBBLE,
						 FIXED_INTEGER_PART(particle->x) -
							 AZUL_BUBBLE_WIDTH / 2,
						 FIXED_INTEGER_PART(particle->y) -
							 AZUL_BUBBLE_HEIGHT / 2,
						 AZUL_BUBBLE_WIDTH, AZUL_BUBBLE_HEIGHT);
					btParticleAdvanceWithGravity2D(particle, 63, toFIXED(1));
					if(particle->y > toFIXED(96)) {
						particle->velY = -particle->velY / 2;
						particle->velX <<= 2;
						particle->variant -= 8;
					}
				}
				particle->variant--;
			}
		}
		if(frame == AZUL_BURST_FRAME) {
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg,
								 BACKGROUND_DARKEN_FRAMES);
			syWordsFillEntry(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT,
								IO_WRITE_HALFWORD);
			for(i = 0; i != args->tgtCnt; i++) {
				EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_HURL);
			}
			sySndPlayStub(SE_CRASH);
			state->shakeFrm = 16;
		}
		if(frame == AZUL_DAMAGE_FRAME) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frame >= AZUL_BURST_FRAME) {
			btCameraShake(8, 8);
		}
		else {
			btCameraShake(2, 2);
		}
		if(frame >= AZUL_BURST_FRAME) {
			btEffectCountDownBattlerTints();
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmPaletteHBlankDma);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	for(i = 0; i != AZUL_OBJECTS; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

/* Unused: moves a particle one frame in three dimensions, as */
/* btParticleAdvanceWithGravity3D does, which the effects call; nothing calls */
/* this copy. */
UNUSED void btEffectPosAdvanceWithGravity3D(PARTICLE *step, s32 damping,
											s32 gravity) {
	s32 vx = step->velX;
	s32 vy = step->velY;
	s32 vz = step->velZ;

	step->x += vx;
	step->y += vy;
	step->z += vz;
	SET_VELOCITY(step, vx * damping / DAMPING_SCALE,
				 (vy + gravity) * damping / DAMPING_SCALE,
				 vz * damping / DAMPING_SCALE);
}

/* Unused: moves a particle one frame on the screen, as */
/* btParticleAdvanceWithGravity2D does; nothing calls this copy. */
UNUSED void btEffectPosAdvanceWithGravity2D(PARTICLE *step, s32 damping,
											s32 gravity) {
	s32 x = step->x;
	s32 vx = step->velX;
	s32 vy = step->velY;

	step->x = x + vx;
	step->y += vy;
	step->velX = vx * damping / DAMPING_SCALE;
	step->velY = (vy + gravity) * damping / DAMPING_SCALE;
}

/* Projects a 16.16 point onto the screen through the current transform, 16 */
/* pixels higher; returns syConvert3Dto2D's result. */
s32 btEffectPosProjectPoint(const void *point, SCR_POINT *position) {
	s32 result = syConvert3Dto2D(point, syScrPointCoordinates(position));
	position->y -= 16;
	return result;
}

/* Projects a unit's sprite onto the screen, raised by half its height, 16 */
/* pixels higher. */
void btEffectPosProjectSpr(s32 id, SCR_POINT *position) {
	btUnitProjectToScr(id, syScrPointCoordinates(position));
	position->y -= 16;
}

/* Projects a unit's place onto the screen at its scale, 16 pixels higher. */
void btEffectPosProjectScaledBattler(s32 id, SCR_POINT *position) {
	btMotionProjectScaledPosStub(id, syScrPointCoordinates(position));
	position->y -= 16;
}

/* Projects a unit's place onto the screen, 16 pixels higher. */
void btEffectPosProjectBattler(s32 id, SCR_POINT *position) {
	btMotionProjectPosStub(id, syScrPointCoordinates(position));
	position->y -= 16;
}

/* Midpoint between the first and last targets. */
void btEffectGetTargetsMidpoint(SCR_POINT *position) {
	BATTLE_EFFECT_STATE *state;
	SCR_POINT first;
	SCR_POINT last;

	state = Ram_HeapSlots->battleEff;
	btEffectPosProjectBattler(state->args->tgts[0], &first);
	btEffectPosProjectBattler(state->args->tgts[state->args->tgtCnt - 1],
							  &last);
	position->x = first.x + (last.x - first.x) / 2;
	position->y = first.y;
	position->depth = first.depth;
}
#undef CLAMP_Y
#undef AZUL_FRAMES
#undef AZUL_SHATTER_FRAME
#undef AZUL_BUBBLES_FRAME
#undef AZUL_SWIM_FRAME
#undef AZUL_SPHERE_FRAME
#undef AZUL_DART_FRAME
#undef AZUL_SWIRL_FRAME
#undef AZUL_DEEPEN_FRAME
#undef AZUL_RISE_FRAME
#undef AZUL_BURST_FRAME
#undef AZUL_DAMAGE_FRAME
#undef AZUL_SERPENT_OBJECTS
#undef AZUL_BODY_SEGMENT
#undef AZUL_BODY_COPIES
#undef AZUL_OBJECTS
#undef AZUL_PARTS
#undef AZUL_PART_POINTS
#undef AZUL_SHATTER_CENTER
#undef AZUL_BUBBLE_COUNT
#undef AZUL_FACES
#undef AZUL_SPHERE
#undef AZUL_GRID
#undef AZUL_BLUE_BEAM
#undef AZUL_BUBBLE
#undef AZUL_BUBBLE_WIDTH
#undef AZUL_BUBBLE_HEIGHT
#undef AZUL_WISPS
#undef AZUL_WISP_WIDTH
#undef AZUL_WISP_HEIGHT
#undef AZUL_WISP_PICTURES
#undef AZUL_SPHERE_RINGS
#undef AZUL_SPHERE_RING_VERTICES
#undef AZUL_SPHERE_RADIUS
#undef AZUL_BUBBLE_PHASE_STEP
#undef AZUL_BUBBLE_DELAY
#undef AZUL_WATER_BLUE
#undef AZUL_WATER_GREEN
#undef AZUL_WATER_RED
#undef AZUL_WATER_DEPTH
#undef AZUL_WATER_DEEPEST
#undef AZUL_DROPS
#undef AZUL_RING_TURN
#undef AZUL_TEXTURES
