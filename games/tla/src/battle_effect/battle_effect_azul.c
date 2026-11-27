/* The Azul summon, the effect steps under gravity and the projection of effect
   positions onto the screen. Part of battle_effect.c. */

struct MESH_TRIANGLE;
struct STRIP_QUAD;

/* One face of a shard of the shattered battlefield: its three corners and their
   texture coordinates in the battlefield's picture. */
typedef struct AZUL_FACE {
	Uint8 corner[3];
	Uint8 pad;
	Uint8 uv[3][2];
	Uint8 endPad[2];
} AZUL_FACE;

/* Clamps a sprite's projected y to the screen's band. */
#define CLAMP_Y(place) \
	((place)[2] < -toFIXED(1) ? (void)((place)[2] = -toFIXED(1)) : (void)0, \
		(place)[2] > toFIXED(DISPLAY_HEIGHT) - 1 \
			? (void)((place)[2] = toFIXED(DISPLAY_HEIGHT)) \
			: (void)0)

/* Azul's timeline, in ticks. The serpent plunges down from tick 0 and the
   battlefield shatters behind it; bubbles rise round the foes; the serpent
   swims in from the right and circles them while the water sphere closes; it
   darts off, the sphere swirls, the serpent rises from below, and the sphere
   bursts. A or B from AZUL_BUBBLES_TICK on skips to the burst. */
#define AZUL_TICKS		  228
#define AZUL_SHATTER_TICK 10
#define AZUL_BUBBLES_TICK 16
#define AZUL_SWIM_TICK	  54
#define AZUL_SPHERE_TICK  78 /* the sphere is built; it shows two ticks on */
#define AZUL_DART_TICK	  138
#define AZUL_SWIRL_TICK	  152
#define AZUL_DEEPEN_TICK  160 /* the water deepens again */
#define AZUL_RISE_TICK	  168
#define AZUL_BURST_TICK	  186
#define AZUL_DAMAGE_TICK  214

/* Azul's objects: the serpent's twelve, then sixteen copies of its body
   segments 5 to 8 that trail behind them; and the 31 shards the battlefield
   breaks into as it plunges down. */
#define AZUL_SERPENT_OBJECTS 12
#define AZUL_BODY_SEGMENT	 5
#define AZUL_BODY_COPIES	 16
#define AZUL_OBJECTS		 (AZUL_SERPENT_OBJECTS + AZUL_BODY_COPIES)
#define AZUL_PARTS			 31
/* A shard's points in AzulPartPoints: its middle, then its four corners. */
#define AZUL_PART_POINTS	 5
/* The point of the canvas the shards fly out from. */
#define AZUL_SHATTER_CENTER	 60
/* The bubbles that rise from tick 16, in the particles after the shards. */
#define AZUL_BUBBLE_COUNT	 27

/* Azul's pictures and models in the effect's pictures: the shards' faces after
   their corners, the sphere, the grid, BlueBeamSheet, and its first picture, a
   blue bubble, with every other pixel kept as the canvas's halved width draws
   it. */
#define AZUL_FACES				  496
#define AZUL_SPHERE				  2048
#define AZUL_GRID				  3072
#define AZUL_BLUE_BEAM			  11192
#define AZUL_BUBBLE				  11768
#define AZUL_BUBBLE_WIDTH		  12
#define AZUL_BUBBLE_HEIGHT		  24
/* WispSheet's six 64x32 pictures, turned a quarter and every other row kept:
   six 16x64 wisps of foam. */
#define AZUL_WISPS				  12056
#define AZUL_WISP_WIDTH			  16
#define AZUL_WISP_HEIGHT		  64
#define AZUL_WISP_PICTURES		  6
/* The water sphere: 16 rings of 16 vertices from pole to pole, 48 pixels
   round. */
#define AZUL_SPHERE_RINGS		  16
#define AZUL_SPHERE_RING_VERTICES 16
#define AZUL_SPHERE_RADIUS		  48
/* The phase of a bubble's sway, a bubble after the one before. */
#define AZUL_BUBBLE_PHASE_STEP	  6678
/* The bubbles show one after another: bubble i waits AZUL_BUBBLE_DELAY times 1
   - cos(i / 128 of a turn) ticks, each a little longer than the last. */
#define AZUL_BUBBLE_DELAY		  32
/* The water behind the foes: the backdrop's colour on each line of the battle
   view, its top line these levels of blue, green and red (cyan, the brighter
   levels held at the brightest), each channel a level darker every so many
   lines down. The water deepens a level every 8 ticks to AZUL_WATER_DEPTH, and
   from tick 160 a level every 4 ticks to AZUL_WATER_DEEPEST. */
#define AZUL_WATER_BLUE			  40
#define AZUL_WATER_GREEN		  32
#define AZUL_WATER_RED			  16
#define AZUL_WATER_DEPTH		  12
#define AZUL_WATER_DEEPEST		  20
/* The drops the sphere bursts into, kept in the map grid's cell buffer. */
#define AZUL_DROPS				  256
/* How far FireRingTexture's square is turned about the upright. */
#define AZUL_RING_TURN			  13384
/* Where Azul decodes, one after another, WispSheet, CornerRuneASheet,
   FireRingTexture and SwirlTexture in the map cell buffer. */
#define AZUL_TEXTURES			  16384

/* Azul: the serpent plunges down and the battlefield shatters behind it,
   bubbles rise round the foes, the serpent swims in from the right and closes
   them in a great sphere of water, and the sphere bursts over the field in foam
   and falling drops.

   Abilities: Azul. */
void btEffectRunAzul(BATTLE_EFFECT_PARM *args)
{
	Uint8				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*sprs;
	DRAW_RECTANGLE		 alternateBlit;
	DRAW_RECTANGLE		 blit;
	Sint32				 frm;
	BATTLE_STATE		*battle;
	POINT2_D			 base;
	POINT2_D			 pos;
	POINT2_D			 vel;
	MODEL_VERTEX		*meshVertices;
	AZUL_FACE			*faces;
	Uint8				*sphere;
	Uint8				*grid;
	Sint32				 i;
	Sint32				 j;
	Sint32				 k;
	Sint32		  n; /* a byte along a line of the background, then a shard */
	Uint32		  shatterFrm;
	Uint32		  sphereFrm;
	FIXED		  place[4];
	MODEL_TEXTURE texture;
	MODEL_TEXTURE scratchTexture;
	PARTICLE	 *particle;
	RENDER_MODEL *model;
	void		 *vertices;
	SPR			 *spr;
	const Uint8	 *objectX;
	Uint8		 *mapCells;
	Uint8		 *planes;
	Sint32		  wispBottom;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	sprs = Ram_HeapSlots->battleSheet;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(BGCNT_PRIORITY(3));
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btEffectShowLivingTargets(state->args);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	/* The water's colour for each line streams in by DMA, armed early in each
	   VBlank ahead of the canvas transfer: in a pass the higher rank runs
	   first. */
	sySetEvent(btEffectArmPaletteHBlankDMA, EVENT_PASS_VBLANK + 126);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	battle = Ram_HeapSlots->battleState;
	meshVertices = (MODEL_VERTEX *)state->sheet;
	faces = (AZUL_FACE *)(state->sheet + AZUL_FACES);
	sphere = state->sheet + AZUL_SPHERE;
	grid = state->sheet + AZUL_GRID;
	{
		Uint8 *cells = MapGridMetatileBuffer;

		/* The battle background's 256-colour tiles laid out as one picture, a
		   line at a time. */
		for(i = 0; i < BATTLE_BACKGROUND_HEIGHT; i++) {
			for(n = 0; n < BATTLE_BACKGROUND_WIDTH * 8; n += 8 * 8) {
				syWordsCopyEntry(cells,
					(void *)(BG_CHAR_BLOCK(2) +
						((i / 8 * BATTLE_BACKGROUND_WIDTH + (i & 7)) * 8 + n)),
					8);
				cells += 8;
			}
		}
	}
	{
		Uint8 *cells = state->sheet + AZUL_BLUE_BEAM;
		Uint8 *dst;
		Sint32 cnt;

		btResourceLoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET, cells, 1, 1);
		i = 0;
		cnt = PICTURE_BYTES(AZUL_BUBBLE);
		dst = state->sheet + AZUL_BUBBLE;
		for(; i != cnt; i++) {
			*dst++ = *cells;
			cells += 2;
		}
	}
	sySynch(1);
	btEffectCreateSprs(AZUL_SERPENT_OBJECTS, SPRITE_AZUL, 1);
	for(i = 0; i != AZUL_BODY_COPIES; i++) {
		spr = btEffectSprAcquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
		state->sprs[i + AZUL_SERPENT_OBJECTS] = spr;
		syWordsCopyEntry(spr, state->sprs[AZUL_BODY_SEGMENT + (i & 3)],
			offsetof(SPR, lastNo));
		((SPR *)state->sprs[i])->image.pri = 1;
	}
	mapCells = MapGridMetatileBuffer;
	planes = Ram_MapMetatileBuffer + AZUL_TEXTURES;
	/* The wisps are turned a quarter as they are copied: each row of a picture
	   in WispSheet becomes a column of its wisp, from the bottom up. */
	wispBottom = AZUL_WISPS + (AZUL_WISP_HEIGHT - 1) * AZUL_WISP_WIDTH;
	for(frm = 0; frm != AZUL_TICKS; frm++) {
		/* A or B skips to the burst in a white flash. */
		if((PerWork.repeat & (KEY_A | KEY_B)) && frm >= AZUL_BUBBLES_TICK &&
			frm < AZUL_BURST_TICK) {
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			EFFECT_PRESENT_CANVAS(state);
			frm = AZUL_BURST_TICK;
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = DEGtoANG(90);
			}
		}
		/* The battlefield's colours come up from dark over the first ticks. */
		if(frm <= 8) {
			Sint32 fade = 24 - frm * 3;

			btColBufferScaleStub(battle->pal, (void *)&BG_PLTT_COLOR(6, 0),
				toFIXED(1) - fade * BACKGROUND_LEVEL_STEP,
				BATTLE_BACKGROUND_COLORS);
		}
		if(frm == AZUL_BURST_TICK) {
			base.x = toFIXED(48);
			base.y = toFIXED(66.00005);
			vel.x = 0;
			vel.y = -toFIXED(6);
		}
		if(frm == 0) {
			REG_BLDALPHA = BLEND_ALPHA(16, 0);
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, frm);
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = TRIG_TURN * 3 / 4;
			}
			base.x = toFIXED(120);
			base.y = -toFIXED(120);
			vel.x = 0;
			vel.y = toFIXED(12);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
			for(n = 0; n != AZUL_PARTS; n++) {
				for(i = 0; i != 4; i++) {
					meshVertices[n * 4 + i].x =
						AzulPartPoints[n * AZUL_PART_POINTS + i + 1][0] -
						AzulPartPoints[n * AZUL_PART_POINTS][0];
					meshVertices[n * 4 + i].y =
						AzulPartPoints[n * AZUL_PART_POINTS + i + 1][1] -
						AzulPartPoints[n * AZUL_PART_POINTS][1];
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
						AzulPartPoints[n * AZUL_PART_POINTS +
							faces[n * 3].corner[i] + 1][0];
					faces[n * 3].uv[i][1] =
						AzulPartPoints[n * AZUL_PART_POINTS +
							faces[n * 3].corner[i] + 1][1];
					faces[n * 3 + 1].uv[i][0] =
						AzulPartPoints[n * AZUL_PART_POINTS +
							faces[n * 3 + 1].corner[i] + 1][0];
					faces[n * 3 + 1].uv[i][1] =
						AzulPartPoints[n * AZUL_PART_POINTS +
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
		shatterFrm = frm - AZUL_SHATTER_TICK;
		if(shatterFrm <= BLEND_WEIGHT_MAX) {
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, shatterFrm);
		}
		/* Bubbles rise round the foes for 64 ticks. */
		if((Uint32)(frm - AZUL_BUBBLES_TICK) <= 63) {
			if(frm == AZUL_BUBBLES_TICK) {
				Sint32 mask;

				i = 0;
				mask = 31;
				particle = &state->particles[AZUL_PARTS];
				for(; i != AZUL_BUBBLE_COUNT; i++, particle++) {
					particle->x = INTtoFIXED((syRandom() & 63) + 32);
					particle->y = INTtoFIXED((syRandom() & 15) + 120);
					particle->velX = ((syRandom() & mask) - 16) << 13;
					particle->velY = ((syRandom() & mask) - 63) << 12;
					particle->variant =
						FIXEDtoINT(
							syCos(i * (TRIG_TURN / 128)) * AZUL_BUBBLE_DELAY) -
						AZUL_BUBBLE_DELAY;
				}
			}
			i = 0;
			particle = &state->particles[AZUL_PARTS];
			for(; i != AZUL_BUBBLE_COUNT; i++, particle++) {
				if(particle->variant >= 0) {
					FIXED sway = sySin(
						frm * (TRIG_TURN / 64) + i * AZUL_BUBBLE_PHASE_STEP);

					blit(canvas, state->sheet + AZUL_BUBBLE,
						(FIXEDtoINT(particle->x + (sway << 4))) -
							AZUL_BUBBLE_WIDTH / 2,
						FIXED_INTEGER_PART(particle->y) -
							AZUL_BUBBLE_HEIGHT / 2,
						AZUL_BUBBLE_WIDTH, AZUL_BUBBLE_HEIGHT);
					btParticleAdvanceWithGravity2D(
						particle, 63, -toFIXED(1) / 4);
				}
				particle->variant++;
			}
		}
		/* The water behind the foes deepens. */
		if(frm <= 95 || (Uint32)(frm - AZUL_DEEPEN_TICK) <= 31) {
			Uint16 *c = state->backdropLines;
			Sint32	level = frm / 8;

			if(frm >= AZUL_DEEPEN_TICK) {
				level = (frm - AZUL_DEEPEN_TICK) / 4 + AZUL_WATER_DEPTH;
				if(level > AZUL_WATER_DEEPEST) {
					level = AZUL_WATER_DEEPEST;
				}
			} else if(level > AZUL_WATER_DEPTH) {
				level = AZUL_WATER_DEPTH;
			}
			for(i = 0; i != BATTLE_VIEW_TOP - 1; i++) {
				*c++ = 0;
			}
			for(; i != BATTLE_VIEW_BOTTOM - 1; i++) {
				Sint32 depth = i - (BATTLE_VIEW_TOP - 1);
				Sint32 b =
					-(depth / GRADIENT_BLUE_LINES) - level + AZUL_WATER_BLUE;
				Sint32 g =
					-(depth / GRADIENT_GREEN_LINES) - level + AZUL_WATER_GREEN;
				Sint32 r =
					-(depth / GRADIENT_RED_LINES) - level + AZUL_WATER_RED;

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
		/* DEAD: tick never falls below 0; the ROM keeps the compare and
		   branch. */
		if(frm >= 0) {
			base.x += vel.x;
			base.y += vel.y;
		}
		/* The serpent rises from below and lifts the foes. */
		if(frm == AZUL_RISE_TICK) {
			base.x = toFIXED(48);
			base.y = toFIXED(168);
			vel.x = 0;
			vel.y = -toFIXED(6);
			for(i = 0; i != AZUL_OBJECTS; i++) {
				((SPR *)state->sprs[i])->rotation = DEGtoANG(90);
			}
			for(i = 0; i != args->tgtCnt; i++) {
				MOTION_OBJECT *actor = btObjectGetStub(args->tgts[i])->obj;

				btEffectStartActorMotion(
					args->tgts[i], 1, 0, 0, toFIXED(4), 100);
				actor->verticalMotionStrength = 0;
			}
		}
		if(frm == AZUL_BURST_TICK) {
			vel.y = -toFIXED(16);
		}
		/* The serpent upright, plunging and then rising. */
		if((frm < AZUL_SWIM_TICK || frm >= AZUL_RISE_TICK) &&
			frm < AZUL_DAMAGE_TICK) {
			place[3] = 0;
			place[1] = toFIXED(255);
			if(frm >= AZUL_RISE_TICK) {
				pos.y -= toFIXED(6);
			}
			if(frm < AZUL_SWIM_TICK) {
				for(i = 0; i != AZUL_SERPENT_OBJECTS; i++) {
					place[0] = toFIXED(40 - AzulObjectX[i]) + base.x;
					place[2] = toFIXED(128 - AzulObjectZ[i] * 31 / 32) + base.y;
					if(i > 8) {
						place[2] += -toFIXED(240);
					}
					CLAMP_Y(place);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], place, (Sint32 *)&AzulUnitScale, 0);
				}
				for(i = 0; i != AZUL_BODY_COPIES; i++) {
					place[0] =
						toFIXED(39 - AzulObjectX[(i & 3) + AZUL_BODY_SEGMENT]) +
						base.x;
					place[2] =
						toFIXED(-(AzulObjectZ[(i & 3) + AZUL_BODY_SEGMENT] *
									30 / 32) -
							i / 4 * 60) +
						base.y + toFIXED(66);
					CLAMP_Y(place);
					spr = state->sprs[i + AZUL_SERPENT_OBJECTS];
					spr->image.tile =
						((SPR *)state->sprs[(i & 3) + AZUL_BODY_SEGMENT])
							->image.tile;
					fdRenderApplyProjectedPlacementStub(
						spr, place, (Sint32 *)&AzulUnitScale, 0);
				}
			} else {
				for(i = 0; i != AZUL_SERPENT_OBJECTS; i++) {
					place[0] = toFIXED(AzulObjectX[i]) + base.x;
					place[2] = toFIXED(AzulObjectZ[i]) + base.y;
					if(i > 8) {
						place[2] += toFIXED(256);
					}
					CLAMP_Y(place);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], place, (Sint32 *)&AzulUnitScale, 0);
				}
				i = 0;
				objectX = AzulObjectX;
				for(; i != AZUL_BODY_COPIES; i++) {
					Sint32 segment = i & 3;
					Sint32 row = segment + AZUL_BODY_SEGMENT;

					place[0] = toFIXED(objectX[row]) + base.x - toFIXED(1);
					place[2] = toFIXED(AzulObjectZ[row] + i / 4 * 64) + base.y +
						toFIXED(64);
					CLAMP_Y(place);
					spr = state->sprs[i + AZUL_SERPENT_OBJECTS];
					spr->image.tile =
						((SPR *)state->sprs[segment + AZUL_BODY_SEGMENT])
							->image.tile;
					fdRenderApplyProjectedPlacementStub(
						spr, place, (Sint32 *)&AzulUnitScale, 0);
				}
			}
		} else if(frm >= AZUL_SWIM_TICK && frm < AZUL_RISE_TICK) {
			/* The serpent swims in from the right and circles, then darts
			   off. */
			Sint32 x;
			Sint32 y;

			if(frm == AZUL_SWIM_TICK) {
				for(i = 0; i != AZUL_OBJECTS; i++) {
					((SPR *)state->sprs[i])->rotation = 0;
				}
				base.x = toFIXED(256);
				base.y = toFIXED(72);
				vel.x = -toFIXED(6);
				vel.y = 0;
			}
			if(frm == AZUL_DART_TICK) {
				vel.x = toFIXED(8);
			}
			place[3] = 0;
			place[1] = toFIXED(255);
			vel.x = vel.x * 56 / DAMPING_SCALE;
			vel.y = vel.y * 56 / DAMPING_SCALE;
			if(frm < AZUL_DART_TICK) {
				pos.x = (sySin(frm << 9) << 4) + base.x;
				x = pos.x;
				pos.y = (syCos(frm << 9) << 3) + base.y;
				y = pos.y;
			} else {
				x = base.x + (sySin(frm << 9) << 4);
				y = (syCos(frm << 9) << 3) + base.y;
			}
			for(i = 0; i != 8; i++) {
				place[0] = toFIXED(AzulLateObjectX[i]) + x;
				place[2] = toFIXED(AzulLateObjectZ[i]) + y;
				fdRenderApplyProjectedPlacementStub(
					state->sprs[i], place, (Sint32 *)&AzulLateUnitScale, 0);
			}
		}
		/* The sphere drifts left as the serpent darts off, then back. */
		if((Uint32)(frm - AZUL_DART_TICK) <= 29) {
			Sint32 drift = -toFIXED(2);

			if(frm >= AZUL_SWIRL_TICK) {
				drift = frm * (toFIXED(1) / 8) - toFIXED(21);
			}
			pos.x = pos.x + drift;
		}
		if(frm == AZUL_SPHERE_TICK) {
			sySndPlayStub(SE_SPIN);
		}
		if(frm == AZUL_DART_TICK) {
			sySndPlayStub(SE_LAUNCH);
		}
		if(frm == AZUL_SWIRL_TICK) {
			sySndPlayStub(SE_WAVE_HIGH);
		}
		if(frm == 7) {
			sySndPlayStub(SE_POWER_UP);
		}
		/* The battlefield shatters into shards. */
		if(frm == AZUL_SHATTER_TICK) {
			const Uint8 *piece;

			sySndPlayStub(SE_CLATTER);
			i = 0;
			particle = state->particles;
			piece = AzulPartPoints[0];
			for(; i != AZUL_PARTS;
				i++, piece += AZUL_PART_POINTS * 2, particle++) {
				Sint32 cx = piece[0] >> 1;
				Sint32 cy = piece[1];
				Sint32 dx;
				Sint32 dy;

				SET_POSITION(particle, toFIXED(cx), toFIXED(cy), 0);
				/* A shard flies out a 16th of a pixel a tick across, and a
				   64th down, for each pixel it lies from the centre. */
				dx = cx - AZUL_SHATTER_CENTER;
				dy = cy - AZUL_SHATTER_CENTER;
				particle->velX = dx * (toFIXED(1) / 16);
				particle->velY = dy * (toFIXED(1) / 64);
				particle->velZ = ((syRandom() & 127) << 2) + 512;
				if(syRandom() & 1) {
					particle->velZ = -particle->velZ;
				}
			}
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
				DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
					DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
					DISPCNT_WIN1_ON,
				IO_WRITE_HALFWORD);
		}
		/* The foes are tinted blue and float up at the shattering, and stand
		   again at the burst. */
		if(frm == AZUL_SHATTER_TICK || frm == AZUL_BURST_TICK) {
			for(i = 0; i != args->tgtCnt; i++) {
				BATTLE_OBJECT *slot = btObjectGetStub(args->tgts[i]);
				MOTION_OBJECT *actor = slot->obj;
				void		  *record;

				if(frm == AZUL_BURST_TICK) {
					btObjectDestroySprsStub(args->tgts[i]);
				}
				if(frm == AZUL_SHATTER_TICK) {
					for(j = 0; (record = btActorGetSprStub(slot->obj, j)) != 0;
						j++) {
						fdSprRemoveAnimationStub(
							record, (Sint32)slot->shadowEntry);
					}
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_BLUE_TINT,
						POSE_DAMAGE, i, 8);
					btEffectStartActorMotion(args->tgts[i], 1, 0, 0,
						(syRandom() << 1) + toFIXED(1), 100);
					actor->verticalMotionStrength = 0;
				}
			}
			if(frm == AZUL_BURST_TICK) {
				args->tgts[args->tgtCnt] = BATTLE_UNIT_LIST_END;
				btObjectSpawnObjectsForListStub(args->tgts, 0, 0);
			}
		}
		if(frm >= AZUL_SHATTER_TICK) {
			for(i = 0; i != args->tgtCnt; i++) {
				MOTION_OBJECT *actor = btObjectGetStub(args->tgts[i])->obj;

				actor->velY = actor->velY * 60 / DAMPING_SCALE;
			}
		}
		/* The shards tumble away until the serpent swims in. */
		if(shatterFrm <= 43) {
			vertices = syHeapAllocEWRAM(32);
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
				syTranslate(
					particle->x - toFIXED(64), particle->y - toFIXED(64), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
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
				if(frm >= (8 - i) * 2 + 16) {
					particle->velY += -toFIXED(1) / 2;
				}
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		/* The sphere's rings, and the foam, rune and ring textures. */
		if(frm == AZUL_SPHERE_TICK) {
			Sint32 ang;
			Sint32 ofs;

			btMeshBuildGrid(
				grid, AZUL_SPHERE_RING_VERTICES, AZUL_SPHERE_RINGS, 8, 8);
			k = 0;
			ang = 0;
			ofs = 0;
			for(; k != AZUL_SPHERE_RINGS; ang += DEGtoANG(12),
				ofs += AZUL_SPHERE_RING_VERTICES * sizeof(MODEL_VERTEX), k++) {
				Sint32 y = FIXEDtoINT(-syCos(ang) * AZUL_SPHERE_RADIUS);
				Sint32 radius = FIXEDtoINT(sySin(ang) * AZUL_SPHERE_RADIUS);
				MODEL_VERTEX *v;

				j = 0;
				v = (MODEL_VERTEX *)(sphere + ofs);
				for(; j != AZUL_SPHERE_RING_VERTICES; j++) {
					SET_POSITION(v,
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
			btResourceLoadAndDecompress(RESOURCE_WISP_SHEET,
				Ram_MapMetatileBuffer + AZUL_TEXTURES, 1, 0);
			for(i = 0; i != AZUL_WISP_PICTURES; i++) {
				for(k = 0; k != AZUL_WISP_WIDTH; k++) {
					for(j = 0; j != AZUL_WISP_HEIGHT; j++) {
						state->sheet[wispBottom +
							(i * AZUL_WISP_HEIGHT - j) * AZUL_WISP_WIDTH + k] =
							(planes + i * PICTURE_BYTES(WISP) +
								k * (WISP_WIDTH * 2))[j];
					}
				}
			}
			btResourceLoadAndDecompress(RESOURCE_CORNER_RUNE_A_SHEET,
				(Ram_MapMetatileBuffer + AZUL_TEXTURES), 1, 0);
			btPictureMirrorIntoQuadrants(Ram_MapMetatileBuffer + AZUL_TEXTURES,
				MapGridMetatileBuffer, 64, 64);
			btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
				Ram_MapMetatileBuffer + AZUL_TEXTURES, 0, 0);
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frm == AZUL_SWIRL_TICK) {
			btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
				Ram_MapMetatileBuffer + AZUL_TEXTURES, 0, 0);
		}
		/* The water sphere grows round the foes. */
		sphereFrm = frm - (AZUL_SPHERE_TICK + 2);
		if(sphereFrm <= 107) {
			Sint32 scale;
			Sint32 shade;
			Sint32 size;

			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			/* Foam streams up beside the rising serpent. */
			if((Uint32)(frm - AZUL_RISE_TICK) <= 17) {
				Sint32 y;
				Sint32 wisp;
				Sint32 width;
				Sint32 height;

				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
				alternateBlit = Ram_HeapSlots->ramCodePaired;
				i = 0;
				y = (FIXEDtoINT(base.y)) - 32;
				width = AZUL_WISP_WIDTH;
				height = AZUL_WISP_HEIGHT;
				wisp = frm;
				for(; i != 2; i++, wisp += 3) {
					Uint8 *cells = state->sheet + AZUL_WISPS +
						wisp % AZUL_WISP_PICTURES * PICTURE_BYTES(AZUL_WISP);

					blit(canvas, cells, 30, y, width, height);
					alternateBlit(canvas, cells, 46, y, width, height);
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
			if(frm < AZUL_BURST_TICK) {
				model = btRenderAllocateModelList(1);
				scale = sphereFrm * 3 << 8;
				shade = 0;
				if(scale > toFIXED(1)) {
					scale = toFIXED(1);
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
				syRotX(frm * (TRIG_TURN / 64));
				syRotY(frm * (TRIG_TURN / 64));
				btRenderTransformVertices(sphere, meshVertices, 256);
				btRenderDrawModelList(model);
				if(frm >= AZUL_DART_TICK) {
					shade = 1444 - frm * 10;
					size = frm * (toFIXED(1) / 8) + -toFIXED(17);

					if(shade > 0) {
						shade = 0;
					}
					if(shade > MODEL_SHADE_INVISIBLE) {
						scratchTexture.pixels =
							Ram_MapMetatileBuffer + AZUL_TEXTURES;
						scratchTexture.logWidth = 7;
						model->mesh = BattleEffectLargeSquareMesh;
						scratchTexture.logHeight = 7;
						model->shade = shade;
						syInitMatrix();
						syTranslate(toFIXED(44), 0, 0);
						syRotY(AZUL_RING_TURN);
						syMatrixScaleUniform(size);
						btRenderTransformVertices(
							BattleEffectSquareCorners, meshVertices, 4);
						btRenderDrawModelList(model);
					}
				}
				if(frm >= AZUL_SWIRL_TICK) {
					state->shakeFrm = 2;
					shade = 0;
					size = frm * (toFIXED(1) / 8) + -toFIXED(19);
					scratchTexture.logWidth = 6;
					scratchTexture.logHeight = 6;
					scratchTexture.pixels =
						Ram_MapMetatileBuffer + AZUL_TEXTURES;
					model->shade = shade;
					model->mesh = BattleEffectSquareMesh;
					syInitMatrix();
					syTranslate(-toFIXED(16), pos.y - toFIXED(80), 0);
					syScale(size >> 1, size, size >> 1);
					syRotZ(frm * (TRIG_TURN / 32));
					btRenderTransformVertices(
						BattleEffectSquareCorners, meshVertices, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
			}
		}
		/* The sphere bursts into drops that fall and bounce. */
		if(frm >= AZUL_BURST_TICK) {
			if(frm == AZUL_BURST_TICK) {
				PARTICLE *drop = (PARTICLE *)MapGridMetatileBuffer;

				for(i = 0; i != AZUL_DROPS; i++, drop++) {
					Sint32 ang = i * (TRIG_TURN / AZUL_DROPS);
					Sint32 r = syRandom() & 127;
					Sint32 vy;

					drop->velX = sySin(ang) * r >> 5;
					vy = syCos(ang) * r >> 4;
					drop->x = drop->velX * 6 + toFIXED(48);
					drop->y = vy * 5;
					drop->velY = vy;
					drop->variant = (syRandom() & 31) + 32;
				}
			}
			particle = (PARTICLE *)MapGridMetatileBuffer;
			for(i = 0; i != AZUL_DROPS; i++, particle++) {
				if(particle->variant >= 0) {
					blit(canvas, state->sheet + AZUL_BUBBLE,
						FIXED_INTEGER_PART(particle->x) - AZUL_BUBBLE_WIDTH / 2,
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
		if(frm == AZUL_BURST_TICK) {
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKEN_TICKS);
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
			for(i = 0; i != args->tgtCnt; i++) {
				EFFECT_STRIKE_TARGET(args, i, 16, KNOCKBACK_HURL);
			}
			sySndPlayStub(SE_CRASH);
			state->shakeFrm = 16;
		}
		if(frm == AZUL_DAMAGE_TICK) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frm >= AZUL_BURST_TICK) {
			btCameraShake(8, 8);
		} else {
			btCameraShake(2, 2);
		}
		if(frm >= AZUL_BURST_TICK) {
			btEffectCountDownBattlerTints();
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmPaletteHBlankDMA);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	for(i = 0; i != AZUL_OBJECTS; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

/* Unused: moves a particle one tick in three dimensions, as
   btParticleAdvanceWithGravity3D does, which the effects call; nothing calls
   this copy. */
UNUSED void btEffectPosAdvanceWithGravity3D(
	PARTICLE *step, Sint32 damping, Sint32 gravity)
{
	Sint32 vx = step->velX;
	Sint32 vy = step->velY;
	Sint32 vz = step->velZ;

	step->x += vx;
	step->y += vy;
	step->z += vz;
	SET_VELOCITY(step, vx * damping / DAMPING_SCALE,
		(vy + gravity) * damping / DAMPING_SCALE, vz * damping / DAMPING_SCALE);
}

/* Unused: moves a particle one tick on the screen, as
   btParticleAdvanceWithGravity2D does; nothing calls this copy. */
UNUSED void btEffectPosAdvanceWithGravity2D(
	PARTICLE *step, Sint32 damping, Sint32 gravity)
{
	Sint32 x = step->x;
	Sint32 vx = step->velX;
	Sint32 vy = step->velY;

	step->x = x + vx;
	step->y += vy;
	step->velX = vx * damping / DAMPING_SCALE;
	step->velY = (vy + gravity) * damping / DAMPING_SCALE;
}

/* Projects a 16.16 point onto the screen through the current transform, 16
   pixels higher; returns syConvert3Dto2D's result. */
Sint32 btEffectPosProjectPoint(const void *point, SCREEN_POINT *pos)
{
	Sint32 result = syConvert3Dto2D(point, syScreenPointCoordinates(pos));
	pos->y -= 16;
	return result;
}

/* Projects a unit's sprite onto the screen, raised by half its height, 16
   pixels higher. */
void btEffectPosProjectSpr(Sint32 id, SCREEN_POINT *pos)
{
	btUnitProjectToScreen(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* Projects a unit's place onto the screen at its scale, 16 pixels higher. */
void btEffectPosProjectScaledBattler(Sint32 id, SCREEN_POINT *pos)
{
	btMotionProjectScaledPosStub(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* Projects a unit's place onto the screen, 16 pixels higher. */
void btEffectPosProjectBattler(Sint32 id, SCREEN_POINT *pos)
{
	btMotionProjectPosStub(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* Midpoint between the first and last targets. */
void btEffectGetTargetsMidpoint(SCREEN_POINT *pos)
{
	BATTLE_EFFECT_STATE *state;
	SCREEN_POINT		 first;
	SCREEN_POINT		 last;

	state = Ram_HeapSlots->battleEff;
	btEffectPosProjectBattler(state->args->tgts[0], &first);
	btEffectPosProjectBattler(
		state->args->tgts[state->args->tgtCnt - 1], &last);
	pos->x = first.x + (last.x - first.x) / 2;
	pos->y = first.y;
	pos->depth = first.depth;
}

#undef CLAMP_Y
#undef AZUL_TICKS
#undef AZUL_SHATTER_TICK
#undef AZUL_BUBBLES_TICK
#undef AZUL_SWIM_TICK
#undef AZUL_SPHERE_TICK
#undef AZUL_DART_TICK
#undef AZUL_SWIRL_TICK
#undef AZUL_DEEPEN_TICK
#undef AZUL_RISE_TICK
#undef AZUL_BURST_TICK
#undef AZUL_DAMAGE_TICK
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
