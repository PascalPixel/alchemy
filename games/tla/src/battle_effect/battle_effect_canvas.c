/* Eclipse and the Doom Dragon's change of form, then the canvas layer: its
   transfers, opening and closing it, its maps, the battle background's
   darkening behind it, the targets' flash timers, and lines. Part of
   battle_effect.c. */

#include "field/stubs.h"
#include "battle/formations.h"

/* The form change's flashes, in the effect scratch, where Eclipse decodes its
   swirl texture. */
#define FLASHES ((PARTICLE *)EffectScratch)

/* Where TornadoSheet is decoded in the effect's pictures. */
#define ECLIPSE_TORNADO_PICTURES 8192

/* Where GlowBandTextureA is decoded in the effect's pictures. */
#define ECLIPSE_GLOW_BAND_A 16384

/* Where GlowBandTextureB is decoded in the effect's pictures. */
#define ECLIPSE_GLOW_BAND_B 14400

/* Where FlameBandTexture is decoded in the effect's pictures. */
#define ECLIPSE_FLAME_BAND 1024

/* A glow band texture's bytes: GlowBandTextureA and B are 32 by 4. */
#define GLOW_BAND_BYTES	   (32 * 4)
/* The blast's glow, drawn as shaded circles after GlowBandTextureB. */
#define ECLIPSE_BLAST_GLOW (ECLIPSE_GLOW_BAND_B + GLOW_BAND_BYTES)

/* The golden dragon: a sprite for each of GoldenDragonTiles' pictures and one
   for the first of GoldenDragonHeadTiles', 32x32 in 256 colours, of which each
   resource brings the first ECLIPSE_DRAGON_COLORS. */
#define ECLIPSE_DRAGON_COLORS	  240
#define ECLIPSE_BODY_PICTURES	  31
#define ECLIPSE_DRAGON_SPRITES	  32
/* GoldenDragonHeadTiles' other pictures, which as many of the dragon's sprites
   take (EclipseTileObjects) as it opens its jaws and moves its wings. */
#define ECLIPSE_OPEN_JAW_PICTURES 7
/* The sprite that shows a wing tip until the jaws open, then the tip of the
   snout at (32, 64) about the dragon. */
#define ECLIPSE_SNOUT_SPRITE	  25

/* The dragon's moments: it bobs from ECLIPSE_BOB_START, opens its jaws and
   gathers the orb at ECLIPSE_JAWS_OPEN, breathes at ECLIPSE_BREATH, and from
   ECLIPSE_BREATH_FADE the breath narrows to nothing by
   ECLIPSE_DRAGON_TICKS. */
#define ECLIPSE_BOB_START	 32
#define ECLIPSE_JAWS_OPEN	 136
#define ECLIPSE_BREATH		 172
#define ECLIPSE_BREATH_FADE	 204
#define ECLIPSE_DRAGON_TICKS 220
/* The canvas fills that flicker the sky while the dragon gathers its breath and
   breathes, rows of EclipseFillTicks and EclipseFillLevels. */
#define ECLIPSE_FILLS		 17

/* The glowing orb the dragon gathers: its radius in variant, drawn as shaded
   circles into the effect's pictures. It swells to ECLIPSE_ORB_GATHERED as the
   jaws open and to ECLIPSE_ORB_BREATH as the dragon breathes. */
#define ECLIPSE_ORB			 (state->particles[16])
#define ECLIPSE_ORB_GATHERED 32
#define ECLIPSE_ORB_BREATH	 64
/* The head of the blast, as it falls from the top right, and the ticks it
   whites out the field. */
#define ECLIPSE_BLAST		 (state->particles[63])
#define ECLIPSE_BLAST_TICKS	 48

/* Rays of light that spin out from a point, each a tapered beam that widens and
   narrows over LIGHT_RAY_TICKS: Eclipse and the form change each set up
   LIGHT_RAYS, and Eclipse draws only the first ECLIPSE_LIGHT_RAYS. */
#define LIGHT_RAYS		   16
#define LIGHT_RAY_TICKS	   48
#define ECLIPSE_LIGHT_RAYS 8
/* The tick Eclipse's rays start, each 8 ticks after the last. */
#define ECLIPSE_RAYS_START 48

/* The swirl's scale, which shrinks from 4 this much a tick while the dragon
   gathers its breath. */
#define ECLIPSE_SWIRL_SHRINK (toFIXED(375) / 4096)
/* The brightest a band of light is shaded: a quarter of the way to
   MODEL_SHADE_INVISIBLE. */
#define BAND_SHADE_BRIGHTEST (MODEL_SHADE_INVISIBLE / 4)

/* The square ramp drawn as the blast begins, brightening from left to right. */
#define ECLIPSE_RAMP_SIZE		   120
/* The landing's buffers in the map cell buffer: the dome's vertices and its
   grid mesh, and the band's vertices and mesh; its motes are ECLIPSE_MOTE_COUNT
   of EXTRA_PARTICLES. */
#define ECLIPSE_DOME_VERTICES	   (Ram_MapMetatileBuffer + 2048)
#define ECLIPSE_DOME_MESH		   (Ram_MapMetatileBuffer + 4096)
#define ECLIPSE_BAND_VERTICES	   (Ram_MapMetatileBuffer + 8192)
#define ECLIPSE_BAND_MESH		   (Ram_MapMetatileBuffer + 10240)
#define ECLIPSE_MOTE_COUNT		   512
/* The landing's ticks, and the motes it throws up each tick. */
#define ECLIPSE_LANDING_TICKS	   26
#define ECLIPSE_MOTES_A_TICK	   16
/* The dome the blast lands as: its rings of vertices run from its top, its
   height up, to its rim, its radius out. */
#define ECLIPSE_DOME_RINGS		   8
#define ECLIPSE_DOME_RING_VERTICES 16
#define ECLIPSE_DOME_HEIGHT		   96
#define ECLIPSE_DOME_RADIUS		   48
/* The band of the four rings that spread from the dome: two rings of
   ECLIPSE_BAND_SEGMENTS vertices (btMeshBuildCylinderBand). */
#define ECLIPSE_BAND_SEGMENTS	   32
/* A ring's life, how long it takes to fade in and when it starts to fade out,
   and how far it leans. */
#define ECLIPSE_RING_TICKS		   64
#define ECLIPSE_RING_FADE_IN	   16
#define ECLIPSE_RING_FADE_OUT	   40
#define ECLIPSE_RING_TILT		   10000
/* The aerial view whitens as the blast lands and fades to this tint, its red
   and green darkened and its blue brightened. */
#define ECLIPSE_LANDING_RED		   (-10)
#define ECLIPSE_LANDING_GREEN	   (-15)
#define ECLIPSE_LANDING_BLUE	   4

/* Eclipse: after a flash the golden dragon looms over the horizon while rays of
   light spin out from its head; it opens its jaws, gathers an orb of light and
   breathes it as a blast that whites out the field, and the blast lands on the
   land far below as a dome that spreads in rings before a last white flash.

   Abilities: Eclipse. */
void btEffectRunEclipse(BATTLE_EFFECT_PARM *args)
{
	Uint8				*canvas;
	BATTLE_EFFECT_STATE *state;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE			 blit[2];
	Sint32					 frm;
	Sint32					 savedScroll;
	BATTLE_BACKGROUND_STATE *background;
	Uint8					*sprs;
	BATTLE_STATE			*battle;
	Sint32					 i;
	Sint32					 j;
	Sint32					 k;
	Sint32					 bgX;
	FIXED					 baseX;
	FIXED					 baseY;
	Sint32					 velX;
	Sint32					 velY;
	FIXED					 cx;
	FIXED					 cy;
	Uint32					 u;
	Sint32					 cnt;
	Sint32					 ring;
	Sint32					 shade;
	Sint32					 ofs;
	Uint8					*cells;
	Sint32					 fade;
	Sint16					 list[UNIT_MAX];
	FIXED					 place[4];
	SCALE					 req;
	MODEL_TEXTURE			 texture;
	MODEL_TEXTURE			 breathTexture;
	MODEL_TEXTURE			 landingTexture;
	RENDER_MODEL			*model;
	PARTICLE				*p;
	SPR						*obj;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	savedScroll = BgScrollWork[1].x;
	background = Ram_HeapSlots->battleBg;
	sprs = Ram_HeapSlots->battleSheet;
	battle = Ram_HeapSlots->battleState;
	btEffectStartCanvasLayer(0);
	btEffectFillWhiteBand();
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* The battle flashes away; then the sky and the dragon's sprites load. */
	bgX = -80;
	for(frm = 0; frm != SUMMON_OPENING_TICKS; frm++) {
		if(frm == SUMMON_OPENING_LOAD_TICK) {
			Uint8 *tiles;

			list[0] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForListStub(list, 0);
			syVRAMCacheFreeEntry(battle->arrowTiles);
			fdActorShadowFreeTilesStub();
			ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, RESOURCE_EARTH_HORIZON_BACKDROP, 0);
			state->scrollTmr = 0;
			state->scrollInterval = 1;
			state->scrollStepX = -1;
			state->scrollStepY = 0;
			sySetEvent(btEffectAdvanceScrollOnInterval, EVENT_PASS_TICK);
			background->busy = 1;
			cells = syResourceGetTableEntry(RESOURCE_GOLDEN_DRAGON_TILES);
			syWordsCopy((void *)OBJ_PLTT, cells, ECLIPSE_DRAGON_COLORS * 2);
			cells += ECLIPSE_DRAGON_COLORS * 2;
			syDecompressTaggedLZ(cells, MapGridMetatileBuffer);
			tiles = MapGridMetatileBuffer;
			for(i = 0; i != ECLIPSE_BODY_PICTURES; i++) {
				obj = btEffectSprCreateWithTiles(
					32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
				obj->image.pri = 2;
				state->sprs[i] = obj;
				syWordsCopy(
					(void *)OBJ_TILES(obj), tiles, OBJ_32X32_8BPP_BYTES);
				tiles += OBJ_32X32_8BPP_BYTES;
			}
			/* The head's first picture; the rest stay decoded for its open
			   jaws. */
			cells = syResourceGetTableEntry(RESOURCE_GOLDEN_DRAGON_HEAD_TILES);
			syWordsCopy((void *)OBJ_PLTT, cells, ECLIPSE_DRAGON_COLORS * 2);
			cells += ECLIPSE_DRAGON_COLORS * 2;
			syDecompressTaggedLZ(cells, MapGridMetatileBuffer);
			obj = btEffectSprCreateWithTiles(
				32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
			state->sprs[ECLIPSE_BODY_PICTURES] = obj;
			obj->image.pri = 2;
			syWordsCopy((void *)OBJ_TILES(obj), MapGridMetatileBuffer,
				OBJ_32X32_8BPP_BYTES);
		}
		bgX += 4;
		REG_BG2X = bgX << 8;
		EFFECT_PRESENT_CANVAS(state);
	}
	syWordsFill(canvas, CANVAS_VIEW_BYTES, 0);
	syWordsFill((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	REG_BG2X = 0;
	baseX = 0;
	baseY = 0;
	velX = 0;
	velY = 0;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = Ram_HeapSlots->ramCode;
	syWordsCopy((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
		SHEET_PALETTE_BYTES);
	{
		const Uint8 *texturePixels = (const Uint8 *)FLASHES;

		/* The dragon looms over the horizon while rays of light spin out from
		   its head, then breathes its blast; A or B cuts it short. */
		for(frm = 0;
			frm != ECLIPSE_DRAGON_TICKS && !(PerWork.repeat & (KEY_A | KEY_B));
			frm++) {
			if(frm == 0) {
				baseX = -toFIXED(48);
				baseY = toFIXED(212);
				velX = toFIXED(4);
				velY = -toFIXED(10);
				state->fadeFrm = 1;
				state->fadeStep = frm;
				ECLIPSE_ORB.variant = frm;
				btResourceLoadAndDecompress(
					RESOURCE_SWIRL_TEXTURE, FLASHES, 0, 0);
			}
			if(frm == ECLIPSE_JAWS_OPEN) {
				/* The head's other pictures, still decoded after its first. */
				Uint8 *dst = Ram_MapMetatileBuffer + OBJ_32X32_8BPP_BYTES;
				for(i = 0; i != ECLIPSE_OPEN_JAW_PICTURES; i++) {
					void *tiles = (void *)OBJ_TILES(
						(SPR *)state->sprs[EclipseTileObjects[i]]);
					/* FAKEMATCH(dead): size zeroed before its value. */
					Sint32 size = 0;

					size = OBJ_32X32_8BPP_BYTES;
					syWordsCopyEntry(
						tiles, &dst[i * OBJ_32X32_8BPP_BYTES], size);
				}
			}
			if(frm == ECLIPSE_RAYS_START) {
				sySndPlayStub(SE_DJINNI_SET);
			}
			if(frm == ECLIPSE_JAWS_OPEN) {
				sySndPlayStub(SE_SPIN);
			}
			if(frm == ECLIPSE_BREATH) {
				sySndPlayStub(SE_CLATTER);
			}
			for(i = 0; i != ECLIPSE_FILLS; i++) {
				if(frm == EclipseFillTicks[i]) {
					syWordsFill(
						canvas, CANVAS_VIEW_BYTES, EclipseFillLevels[i]);
				}
			}
			req = EclipseDragonScale;
			place[3] = 0;
			place[1] = toFIXED(255);
			baseX += velX;
			baseY += velY;
			if(frm >= ECLIPSE_BREATH) {
				if(frm > 183) {
					velX += -toFIXED(1) / 128;
					velY += toFIXED(1) / 64;
				}
				if(frm <= 205) {
					velX = velX * 54 / DAMPING_SCALE;
					velY = velY * 54 / DAMPING_SCALE;
				}
			} else {
				if(velY < 0) {
					velY += toFIXED(1) / 4;
				}
				if(velX > 0) {
					velX += -toFIXED(1) / 8;
				}
				if(frm >= ECLIPSE_BOB_START) {
					baseY += sySin((frm - ECLIPSE_BOB_START) << 9) << 2 >> 4;
					if(frm <= 159) {
						baseX += toFIXED(1) / 4;
					}
				}
			}
			for(i = 0; i != ECLIPSE_DRAGON_SPRITES; i++) {
				if(i == ECLIPSE_SNOUT_SPRITE && frm >= ECLIPSE_JAWS_OPEN) {
					place[0] = baseX + toFIXED(32);
					place[2] = baseY + toFIXED(64);
				} else {
					place[0] = baseX + toFIXED(EclipseObjectX[i]);
					place[2] = baseY + toFIXED(EclipseObjectZ[i]);
				}
				if(place[2] <= toFIXED(200) - 1) {
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], place, (Sint32 *)&req, 0);
				}
			}
			if(frm == ECLIPSE_JAWS_OPEN) {
				state->fadeFrm = BACKGROUND_DARKEN_TICKS;
				state->fadeStep = 0;
			}
			/* The orb's place, which shifts left and down as it swells. */
			cx = baseX / 2 - ECLIPSE_ORB.variant * (toFIXED(1) / 8) +
				toFIXED(28);
			cy = baseY + ECLIPSE_ORB.variant * (toFIXED(1) / 4) + toFIXED(56);
			u = frm - ECLIPSE_JAWS_OPEN;
			if(u <= ECLIPSE_DRAGON_TICKS - ECLIPSE_JAWS_OPEN) {
				if(frm == ECLIPSE_JAWS_OPEN) {
					ECLIPSE_ORB.variant = 4;
					ECLIPSE_ORB.x = 0;
					ECLIPSE_ORB.z = 0;
					btEffectDrawShadedCircles(
						state->sheet, ECLIPSE_ORB.variant * 2, toFIXED(1));
				}
				if(frm >= ECLIPSE_BREATH) {
					if(frm == ECLIPSE_BREATH) {
						ECLIPSE_ORB.variant = ECLIPSE_ORB_BREATH;
						btEffectDrawShadedCircles(
							state->sheet, ECLIPSE_ORB_BREATH * 2, toFIXED(1));
						velX = toFIXED(3);
						velY = -toFIXED(3);
					}
					if(frm >= ECLIPSE_BREATH_FADE && ECLIPSE_ORB.variant > 1) {
						ECLIPSE_ORB.variant -= 4;
						if(ECLIPSE_ORB.variant <= 1) {
							ECLIPSE_ORB.variant = 2;
						}
						btEffectDrawShadedCircles(
							state->sheet, ECLIPSE_ORB.variant * 2, toFIXED(1));
					}
				} else if(frm >= ECLIPSE_JAWS_OPEN) {
					velX += -toFIXED(1) / 32;
					velY += toFIXED(1) / 32;
					if(++ECLIPSE_ORB.variant <= ECLIPSE_ORB_GATHERED) {
						btEffectDrawShadedCircles(
							state->sheet, ECLIPSE_ORB.variant * 2, toFIXED(1));
					} else {
						ECLIPSE_ORB.variant = ECLIPSE_ORB_GATHERED;
					}
				}
				{
					Sint32 size = ECLIPSE_ORB.variant;

					btCanvasBlitMirroredQuarters(
						state->sheet, FIXEDtoINT(cx), FIXEDtoINT(cy), size * 2);
				}
			}
			if(frm >= ECLIPSE_RAYS_START) {
				RENDER_MODEL *model;
				void		 *verts;
				Sint32		  modelShade;

				/* Each ray has an angle (x), a spin (y) and an age
				   (variant). */
				if(frm == ECLIPSE_RAYS_START) {
					for(i = 0; i != LIGHT_RAYS; i++) {
						state->particles[i].x = syRandom() & TRIG_ANGLE_MASK;
						state->particles[i].y = (syRandom() & 255) + 128;
						if(syRandom() & 1) {
							state->particles[i].y = -state->particles[i].y;
						}
						state->particles[i].variant = i * -8;
					}
					btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET,
						state->sheet + ECLIPSE_TORNADO_PICTURES, 1, 0);
					btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
						state->sheet + ECLIPSE_GLOW_BAND_A, 0, 0);
				}
				verts = syHeapAllocEWRAM(32);
				model = btRenderAllocateModelList(1);
				modelShade = 0;
				model->shade = modelShade;
				texture.logWidth = 5;
				texture.logHeight = 2;
				texture.pixels = state->sheet + ECLIPSE_GLOW_BAND_A;
				model->kind = MODEL_SHADED_MAX;
				model->mesh = BattleEffectSmallSquareMesh;
				model->texture = &texture;
				model->vertices = verts;
				for(i = 0; i != ECLIPSE_LIGHT_RAYS; i++) {
					state->particles[i].variant++;
					state->particles[i].x += state->particles[i].y;
					if(state->particles[i].variant >= 0 &&
						state->particles[i].variant < LIGHT_RAY_TICKS) {
						syInitMatrix();
						{
							Sint32 x = toFIXED((i & 3) * 2) + toFIXED(2);
							Sint32 o = -toFIXED(64);
							syTranslate(cx + x + o,
								cy - INTtoFIXED((i & 3) + 2) + o, 0);
						}
						syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
						syRotZ(state->particles[i].x);
						syScale(sySin(state->particles[i].variant *
									(TRIG_TURN / 2 / LIGHT_RAY_TICKS)) +
								toFIXED(1) / 4,
							toFIXED(2), toFIXED(1));
						btRenderTransformVertices(
							BattleEffectTaperedBeamCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
				/* As the jaws open a swirl turns in on the orb, shrinking and
				   brightening. */
				if(frm >= ECLIPSE_JAWS_OPEN && frm < ECLIPSE_BREATH) {
					Sint32 s = toFIXED(4) - u * ECLIPSE_SWIRL_SHRINK;

					modelShade = u * 4 + MODEL_SHADE_INVISIBLE;
					if(modelShade > BAND_SHADE_BRIGHTEST) {
						modelShade = BAND_SHADE_BRIGHTEST;
					}
					texture.logWidth = 6;
					model->shade = modelShade;
					texture.logHeight = 6;
					model->kind = MODEL_SHADED_ADDITIVE;
					texture.pixels = texturePixels;
					model->texture = &texture;
					model->mesh = BattleEffectSquareMesh;
					model->vertices = verts;
					syInitMatrix();
					syTranslate(cx - toFIXED(62), cy - toFIXED(66), 0);
					syScale(s >> 1, s, s);
					syRotX(DEGtoANG(45));
					syRotZ(TRIG_TURN * 5 / 32);
					syRotY(-frm * (TRIG_TURN / 64));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
				/* The breath: a beam from the orb, narrowing to nothing at the
				   end. */
				if(frm >= ECLIPSE_BREATH && frm <= ECLIPSE_DRAGON_TICKS) {
					syInitMatrix();
					syTranslate(cx - toFIXED(62), cy - toFIXED(66), 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
					syRotZ(TRIG_TURN * 5 / 32);
					if(frm >= ECLIPSE_BREATH_FADE) {
						Sint32 fadeFrm = frm - ECLIPSE_BREATH_FADE;

						/* Half a unit narrower a tick. */
						syScale(toFIXED(8) - fadeFrm * (toFIXED(1) / 2),
							toFIXED(2), toFIXED(1));
					} else {
						syScale(toFIXED(8), toFIXED(2), toFIXED(1));
					}
					btRenderTransformVertices(
						BattleEffectBeamCorners, verts, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
				syHeapRelease(verts);
			}
			/* Glints sparkle about the orb, and then along the breath. */
			if(frm >= 148 && frm < ECLIPSE_DRAGON_TICKS) {
				for(j = 0; j != 4; j++) {
					Sint32 t = (frm + j) & 3;
					Sint32 x = 0;
					Sint32 y = 0;

					if(frm < ECLIPSE_BREATH) {
						Sint32 size = ECLIPSE_ORB.variant * 2;

						x = syRandom() % size + (FIXEDtoINT(cx)) - (size >> 1) -
							(BattleEffectGlintPictureWidths[t] >> 1);
						y = syRandom() % size + (FIXEDtoINT(cy)) - (size >> 1) -
							(BattleEffectGlintPictureHeights[t] >> 1);
					} else {
						Uint32 a = syRandom() & 63;
						Sint32 r = ECLIPSE_ORB.variant;

						x = (FIXEDtoINT(cx)) - (a >> 1) -
							(BattleEffectGlintPictureWidths[t] >> 1) +
							syRandom() % r - r / 2 - 8;
						y = a + (FIXEDtoINT(cy)) -
							(BattleEffectGlintPictureHeights[t] >> 1);
					}
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EclipseGlintFlips[syRandom() & 3] | EFFECT_CLIP_BOUNDS,
						BLITTER_MAX);
					(Ram_HeapSlots->ramCodePaired)(canvas,
						state->sheet + ECLIPSE_TORNADO_PICTURES +
							BattleEffectGlintPictures[t],
						x, y, BattleEffectGlintPictureWidths[t],
						BattleEffectGlintPictureHeights[t]);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			EFFECT_PRESENT_CANVAS(state);
		}
	}
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	BgScrollWork[1].x = savedScroll;
	background->busy = 0;
	for(i = 0; i != ECLIPSE_DRAGON_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	fdActorShadowLoadTilesStub();
	battle->arrowTiles = syVRAMCacheAllocEntry(128);
	btEffectShowLivingTargets(state->args);
	for(i = 0; i != args->tgtCnt; i++) {
		btEffectTintBattler(
			args->tgts[i], SPRITE_COLOR_GRB, TINT_KEEP, TINT_KEEP, 0);
		btEffectTintBattler(
			args->tgts[i], SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
	}
	syPaletteLoadBasicSprCols();
	REG_DISPCNT = DISPCNT_MODE_1;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
	sySynch(1);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_DISPCNT = EFFECT_DISPCNT;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
	syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	args->tgts[args->tgtCnt] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(args->tgts, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_BAND_TEXTURE_B, state->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	for(i = 0; i != ECLIPSE_RAMP_SIZE; i++) {
		Sint32 v = i / 2;

		if(v > CANVAS_LEVEL_BRIGHTEST) {
			v = CANVAS_LEVEL_BRIGHTEST;
		}
		for(j = 0; j != ECLIPSE_RAMP_SIZE; j++) {
			state->sheet[j * ECLIPSE_RAMP_SIZE + i] = v;
		}
	}
	/* The blast whites out the battlefield. */
	fade = 0;
	for(frm = 0; frm != ECLIPSE_BLAST_TICKS; frm++) {
		if(frm > 39) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(4));
		}
		if(frm == 16) {
			sySndPlayStub(SE_SPIN);
		}
		if(frm == 18) {
			state->shakeFrm = 90;
			sySndPlayStub(SE_CRASH);
			syWordsFill(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 4);
			}
		}
		/* DEAD: the blast ends before tick 110, so this test is always true;
		   the ROM keeps its compare. */
		if(frm <= 109) {
			RENDER_MODEL *model = 0;
			void		 *verts = 0;

			verts = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			breathTexture.logWidth = 5;
			breathTexture.logHeight = 2;
			breathTexture.pixels = state->sheet + ECLIPSE_GLOW_BAND_B;
			model->mesh = BattleEffectSmallSquareMesh;
			model->shade = fade;
			model->texture = &breathTexture;
			model->vertices = verts;
			model->kind = MODEL_SHADED_MAX;
			/* The blast's beam, its head sweeping down from the top right over
			   four ticks. */
			if(frm > 15) {
				PARTICLE *q = &ECLIPSE_BLAST;

				if(frm == 16) {
					q->x = toFIXED(128);
					q->y = -toFIXED(40);
				}
				if(frm <= 19) {
					q->x += -toFIXED(32);
					q->y += toFIXED(64);
				}
				if(frm <= 27) {
					cells = state->sheet + ECLIPSE_BLAST_GLOW;
					btEffectDrawShadedCircles(cells, 96, toFIXED(1));
					btCanvasBlitMirroredQuarters(
						cells, FIXEDtoINT(q->x), FIXEDtoINT(q->y), 96);
				}
				syInitMatrix();
				syTranslate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotZ(TRIG_TURN * 19 / 32);
				syScale(
					toFIXED(3525) / 256, toFIXED(2673) / 1024, toFIXED(1) / 2);
				btRenderTransformVertices(BattleEffectBeamCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(verts);
		}
		{
			Sint32 r = 8;

			/* DEAD: the blast ends before tick 110, so the shake stays at 8;
			   the ROM keeps the three compares. */
			if(frm > 130) {
				r = 2;
			} else if(frm > 120) {
				r = 4;
			} else if(frm > 110) {
				r = 6;
			}
			btCameraShake(r, r);
		}
		EFFECT_END_TICK(state);
	}
	/* The blast lands on the land far below and spreads in rings of light. */
	cnt = -1;
	background->busy = 1;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 0);
	btPaletteBrightenSavedBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
	BgScrollWork[1].x = 0;
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	btResourceLoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FLAME_BAND_TEXTURE, state->sheet + ECLIPSE_FLAME_BAND, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_BAND_TEXTURE_B, state->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
	/* Every mote starts free. */
	{
		Sint32 *v = &EXTRA_PARTICLES[0].variant;
		Sint32	c = -1;

		for(i = 0; i != ECLIPSE_MOTE_COUNT; i++) {
			*v = c;
			v += sizeof(PARTICLE) / sizeof(*v);
		}
	}
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	{
		const void *mesh = ECLIPSE_DOME_MESH;
		void	   *vertices = MapGridMetatileBuffer;
		const void *bandMesh = ECLIPSE_BAND_MESH;

		for(frm = 0; frm != ECLIPSE_LANDING_TICKS; frm++) {
			/* A or B ends the landing eight ticks later, the view
			   whitening. */
			if((PerWork.repeat & (KEY_A | KEY_B)) && cnt == -1) {
				cnt = 8;
			}
			if(frm <= 6 && cnt == -1) {
				Sint32 b = RGB_LEVEL_MAX - frm * 8;
				Sint32 a = b;
				Sint32 c = b;

				if(c < ECLIPSE_LANDING_RED) {
					a = ECLIPSE_LANDING_RED;
				}
				if(c < ECLIPSE_LANDING_GREEN) {
					b = ECLIPSE_LANDING_GREEN;
				}
				if(c < ECLIPSE_LANDING_BLUE) {
					c = ECLIPSE_LANDING_BLUE;
				}
				btPaletteBrightenSavedBattleBg(a, b, c);
			}
			if(frm > 17 || cnt > 0) {
				btPaletteBrightenBattleBg(4, 4, 4);
				if(--cnt == 0) {
					break;
				}
			}
			/* Motes spring up about where the blast landed, farther out each
			   tick. */
			for(i = 0; i != ECLIPSE_MOTES_A_TICK; i++) {
				PARTICLE *q = EXTRA_PARTICLES +
					((frm * ECLIPSE_MOTES_A_TICK + i) &
						(ECLIPSE_MOTE_COUNT - 1));
				Sint32 a = 0;
				Sint32 r = 0;

				a = syRandom();
				r = (syRandom() & 15) + frm * 2;
				q->x = sySin(a) * r + toFIXED(60);
				q->y = syCos(a) * r + toFIXED(60);
				q->velX = 0;
				q->velY = 0;
				q->variant = (syRandom() & 15) + 16;
			}
			for(i = 0; i != ECLIPSE_MOTE_COUNT; i++) {
				p = EXTRA_PARTICLES + i;
				if(p->variant >= 0) {
					Sint32 n = (p->variant >> 3) + 1;

					blit[0](canvas, sprs + BattleEffectMotePictures[n - 1],
						(FIXEDtoINT(p->x)) - n / 2, (FIXEDtoINT(p->y)) - n, n,
						n * 2);
					btParticleAdvanceWithGravity2D(p, 60, -toFIXED(1) / 4);
					p->variant--;
				}
			}
			model = btRenderAllocateModelList(1);
			shade = 0;
			/* The blast's last beam, which narrows from tick 16. */
			/* DEAD: the landing ends before tick 32, so this test is always
			   true; the ROM keeps its compare. */
			if(frm <= 31) {
				Sint32 s = toFIXED(1);

				if(frm > 15) {
					s += toFIXED(1) - frm * (toFIXED(1) / 16);
				}
				landingTexture.logWidth = 5;
				landingTexture.logHeight = 2;
				landingTexture.pixels = state->sheet + ECLIPSE_GLOW_BAND_B;
				model->mesh = BattleEffectSmallSquareMesh;
				model->shade = shade;
				model->vertices = MapGridMetatileBuffer;
				model->kind = MODEL_SHADED_MAX;
				model->texture = &landingTexture;
				syInitMatrix();
				syTranslate(-toFIXED(2), toFIXED(16), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotZ(TRIG_TURN * 19 / 32);
				syScale(s, toFIXED(2), toFIXED(1) / 2);
				btRenderTransformVertices(
					BattleEffectBeamCorners, MapGridMetatileBuffer, 4);
				btRenderDrawModelList(model);
			}
			/* The dome's rings of vertices, from its top to its rim, and the
			   rings' band. */
			if(frm == 0) {
				sySndPlayStub(SE_BANE_ESCAPE);
				btMeshBuildGrid(ECLIPSE_DOME_MESH, ECLIPSE_DOME_RING_VERTICES,
					ECLIPSE_DOME_RINGS, 8, 8);
				{
					Sint32 ringAng = 0;
					for(ring = 0, ofs = 0; ring != ECLIPSE_DOME_RINGS;
						ringAng += DEGtoANG(12),
					ofs += ECLIPSE_DOME_RING_VERTICES * sizeof(MODEL_VERTEX),
					ring++) {
						Sint32 y =
							FIXEDtoINT(-syCos(ringAng) * ECLIPSE_DOME_HEIGHT);
						Sint32 rr =
							FIXEDtoINT(sySin(ringAng) * ECLIPSE_DOME_RADIUS);
						MODEL_VERTEX *v = (MODEL_VERTEX *)ECLIPSE_DOME_VERTICES;

						k = 0;
						v = (MODEL_VERTEX *)((Uint8 *)v + ofs);
						for(; k != ECLIPSE_DOME_RING_VERTICES; k++) {
							SET_POSITION(v,
								FIXEDtoINT(
									sySin(k *
										(TRIG_TURN /
											ECLIPSE_DOME_RING_VERTICES)) *
									rr),
								y,
								FIXEDtoINT(
									syCos(k *
										(TRIG_TURN /
											ECLIPSE_DOME_RING_VERTICES)) *
									rr));
							v++;
						}
					}
				}
				btMeshBuildCylinderBand(ECLIPSE_BAND_VERTICES,
					ECLIPSE_BAND_MESH, ECLIPSE_BAND_SEGMENTS);
			}
			landingTexture.logWidth = 5;
			landingTexture.logHeight = 5;
			landingTexture.pixels = state->sheet;
			model->kind = MODEL_SHADED_MAX;
			model->mesh = mesh;
			model->vertices = vertices;
			model->texture = &landingTexture;
			model->shade = shade;
			model->flg = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
			/* The dome swells from half its size, turning. */
			{
				Sint32 s = (frm << 9) + toFIXED(1) / 2;

				if(s > toFIXED(1)) {
					s = toFIXED(1);
				}
				syInitMatrix();
				syTranslate(-toFIXED(2), toFIXED(12), 0);
				syRotX(DEGtoANG(45));
				syRotY(-frm * (TRIG_TURN / 64));
				syMatrixScaleUniform(s);
				btRenderTransformVertices(ECLIPSE_DOME_VERTICES, vertices,
					ECLIPSE_DOME_RINGS * ECLIPSE_DOME_RING_VERTICES);
				btRenderDrawModelList(model);
			}
			landingTexture.logHeight = 5;
			landingTexture.logWidth = 7;
			landingTexture.pixels = state->sheet + ECLIPSE_FLAME_BAND;
			model->texture = &landingTexture;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->mesh = bandMesh;
			model->flg = shade;
			model->vertices = vertices;
			model->scrollU = shade;
			model->scrollV = shade;
			/* Four rings of flame spread from the dome one after another,
			   fading in and out. */
			for(i = 0; i != 4; i++) {
				Sint32 t = frm + 24 - i * 6;

				if(t >= 0 && t < ECLIPSE_RING_TICKS) {
					Sint32 s = EclipseRingGrowth[i] * t * (toFIXED(1) / 64) +
						toFIXED(0.25);

					shade = 0;
					if(t < ECLIPSE_RING_FADE_IN) {
						shade = t * 4 + MODEL_SHADE_INVISIBLE;
					}
					if(t > ECLIPSE_RING_FADE_OUT) {
						shade = (ECLIPSE_RING_FADE_OUT - t) * 4;
					}
					model->shade = shade;
					syInitMatrix();
					syTranslate(-toFIXED(2), toFIXED(32), 0);
					syScale(s / 4, s / 4, s / 4);
					syRotX(ECLIPSE_RING_TILT);
					syRotY((frm + i * 8) * (TRIG_TURN / 64));
					syScale(toFIXED(1), toFIXED(6), toFIXED(1));
					syTranslate(0, toFIXED(EclipseRingHeights[i]), 0);
					btRenderTransformVertices(ECLIPSE_BAND_VERTICES, vertices,
						ECLIPSE_BAND_SEGMENTS * 2);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			EFFECT_PRESENT_CANVAS(state);
		}
	}
	/* A last white flash over the battle, as the damage shows. */
	syWordsFill((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES,
		CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	BgScrollWork[1].x = savedScroll;
	background->busy = 0;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	sySynch(1);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
	btGraphicsLoadGreyRamp();
	syWordsFill(
		canvas, CANVAS_VIEW_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	for(frm = 0; frm != 32; frm++) {
		EFFECT_PRESENT_CANVAS(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Unused: does nothing; nothing calls it. */
UNUSED void btEffectNoOperationA(void)
{
}

/* Unused: does nothing; nothing calls it. */
UNUSED void btEffectNoOperationB(void)
{
}

/* Where GlowBandTextureA is decoded in the effect's pictures. */
#define FORM_CHANGE_GLOW_BAND	 12288
/* Where BillowTexture, the columns' texture, is decoded in the map cell
   buffer. */
#define FORM_CHANGE_BILLOW		 (Ram_MapMetatileBuffer + 8192)
/* The tick the change bursts: the screen shakes, flashes burst and the columns
   start to rise. At FORM_CHANGE_SWAP the battle swaps in the next form. */
#define FORM_CHANGE_BURST		 48
#define FORM_CHANGE_SWAP		 62
/* The flashes: FORM_CHANGE_FLASHES of them, two a tick from the burst, each
   thrown at FORM_CHANGE_FLASH_SPEED eighths of a pixel a tick and up to as
   much again at random, and each showing FlashBurstSheet's
   FLASH_BURST_PICTURE_COUNT pictures for FLASH_PICTURE_TICKS apiece. The burst
   first frees FORM_CHANGE_FLASH_SLOTS. */
#define FORM_CHANGE_FLASHES		 16
#define FORM_CHANGE_FLASH_SLOTS	 64
#define FORM_CHANGE_FLASH_SPEED	 32
#define FLASH_PICTURE_TICKS		 3
/* The glow's largest radius: it widens a pixel a tick from 8. */
#define FORM_CHANGE_GLOW_RADIUS	 64
/* The level the last form's change fills the canvas with after each copy, as
   the screen whitens. */
#define FORM_CHANGE_FINALE_LEVEL 16
/* How far the columns lean. */
#define FORM_CHANGE_COLUMN_TILT	 1000

/* The Doom Dragon's change of form, played over the dragon once a form is
   beaten. A glow widens over it while rays of light spin out from it, and it
   flashes white; at FORM_CHANGE_BURST the screen shakes, flashes burst upward
   and four columns of light rise from the ground one after another, spinning.
   After its first and second forms (FORMATION_DOOM_DRAGON_A and B) the glow
   stands to its right and to its left, and the battle swaps in the next form at
   FORM_CHANGE_SWAP; after its last (FORMATION_DOOM_DRAGON_C) the glow stands
   over it, the screen whitens, and it puts the battle's view back itself. */
void btEffectRunFormChange(BATTLE_EFFECT_PARM *args, Sint32 kind)
{
	SCREEN_POINT		 center;
	MODEL_TEXTURE		 texture;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 draw[2];
	Sint32				 ticks;
	Sint32				 frm;
	Sint32				 idx;
	Sint32				 i;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	state->framesSinceTransfer = 1;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
		state->sheet + FORM_CHANGE_GLOW_BAND, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_BILLOW_TEXTURE, FORM_CHANGE_BILLOW, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;

	/* The last form's change runs on while the screen whitens. */
	ticks = 100;
	if(kind != FORMATION_DOOM_DRAGON_C)
		ticks = 76;
	for(frm = 0; frm != ticks; frm++) {
		idx = (frm - FORM_CHANGE_BURST) * 2;
		/* The effect's centre, beside or above the dragon. */
		btEffectPosProjectBattler(args->user, &center);
		if(kind == FORMATION_DOOM_DRAGON_A) {
			center.x += 40;
			center.y -= 24;
		} else if(kind == FORMATION_DOOM_DRAGON_B) {
			center.x -= 16;
			center.y -= 24;
		} else {
			/* FAKEMATCH(dead): the third arm steps x by nothing, as the two
			 * arms above step x and y. */
			center.x += 0;
			center.y -= 48;
		}

		if(frm == 0) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			if(kind == FORMATION_DOOM_DRAGON_B)
				syWordsCopyEntry((void *)BG_PLTT,
					syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
					SHEET_PALETTE_BYTES);
			else
				syWordsCopyEntry((void *)BG_PLTT,
					syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_C),
					SHEET_PALETTE_BYTES);
			/* DEAD: inside tick 0's block this test is always true; the ROM
			   keeps its compare. */
			if(frm == 0)
				sySndPlayStub(SE_SPIN);
		}
		if(frm == FORM_CHANGE_BURST)
			sySndPlayStub(SE_CLATTER);
		/* The dragon roars after the burst, and shows white from tick 32 to
		   68. */
		if(frm == 56)
			sySndPlayStub(SE_BOSS_ROAR);
		if(frm == 32)
			btEffectTintBattler(
				args->user, SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, -1);
		if(frm == 68)
			btEffectTintBattler(
				args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, -1);
		if(kind == FORMATION_DOOM_DRAGON_C) {
			/* After the last form the screen whitens. */
			if(frm >= FORM_CHANGE_SWAP) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
					CANVAS_FILL_WORD(FORM_CHANGE_FINALE_LEVEL));
				btPaletteBrightenBgEntries(2, 2, 2);
			}
		} else if(frm == FORM_CHANGE_SWAP) {
			btChangeEnemyFormStub(kind);
		}

		/* The burst: shake, flash the canvas, and burst two flashes a tick
		   from a point near the centre. */
		if(frm >= FORM_CHANGE_BURST) {
			PARTICLE *flashes;

			if(frm == FORM_CHANGE_BURST) {
				{
					Sint32 *variant = &FLASHES[0].variant;

					for(i = 0; i != FORM_CHANGE_FLASH_SLOTS; i++) {
						*variant = -1;
						variant += sizeof(PARTICLE) / sizeof(*variant);
					}
				}
				state->shakeFrm = 8;
				syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			flashes = FLASHES;
			if(frm >= FORM_CHANGE_BURST &&
				frm < FORM_CHANGE_BURST + FORM_CHANGE_FLASHES / 2) {
				Sint32 x = toFIXED((syRandom() & 63) + center.x / 2 - 32);
				Sint32 y = toFIXED((syRandom() & 31) + center.y);
				Sint32 k = 0;
				/* Each flash starts within 8 pixels of the point. */
				Sint32	  spread = 15;
				PARTICLE *burst = &flashes[idx];

				for(; k != 2; k++) {
					PARTICLE *flash = &burst[k];
					Sint32	  speed = FORM_CHANGE_FLASH_SPEED - 1;
					Sint32	  ang;

					speed &= syRandom();
					ang = syRandom();

					flash->x = x + toFIXED((syRandom() & spread) - 8);
					flash->y = y + toFIXED((syRandom() & spread) - 8);
					flash->velX =
						sySin(ang) * (speed + FORM_CHANGE_FLASH_SPEED) >> 3;
					flash->velY =
						syCos(ang) * (speed + FORM_CHANGE_FLASH_SPEED) >> 3;
					flash->variant = 0;
				}
			}
			{
				PARTICLE *flash = flashes;

				for(i = 0; i != FORM_CHANGE_FLASHES; i++, flash++) {
					if(flash->variant >= 0 &&
						flash->variant <
							FLASH_BURST_PICTURE_COUNT * FLASH_PICTURE_TICKS) {
						draw[0](canvas,
							state->sheet +
								flash->variant / FLASH_PICTURE_TICKS *
									PICTURE_BYTES(FLASH_BURST),
							FIXED_INTEGER_PART(flash->x) - 16,
							FIXED_INTEGER_PART(flash->y) - 48,
							FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
						btParticleAdvanceWithGravity2D(
							flash, 62, -toFIXED(1) / 32);
						flash->variant++;
					}
				}
			}
		}

		/* DEAD: tick never falls below 0; the ROM keeps the compare and
		   branch. */
		if(frm >= 0) {
			FIXED		  cx = toFIXED(center.x / 2);
			FIXED		  cy = toFIXED(center.y);
			RENDER_MODEL *model;
			void		 *corners;
			Sint32		  bias;

			/* The glow widens until it reaches FORM_CHANGE_GLOW_RADIUS. */
			if(frm <= 63) {
				Sint32 size = FORM_CHANGE_GLOW_RADIUS * 2;
				Sint32 radius = frm + 8;

				if(radius <= FORM_CHANGE_GLOW_RADIUS) {
					size = radius * 2;
					btEffectDrawShadedCircles(
						Ram_MapMetatileBuffer, size, toFIXED(1));
				}
				btCanvasBlitMirroredQuarters(Ram_MapMetatileBuffer,
					FIXEDtoINT(cx), FIXEDtoINT(cy), size);
			}

			/* Each ray has an angle (x), a spin (y) and an age (variant), each
			   starting 2 ticks after the last. */
			if(frm == 0) {
				for(i = 0; i != LIGHT_RAYS; i++) {
					PARTICLE *ray = &state->particles[i];

					ray->x = syRandom() & TRIG_ANGLE_MASK;
					ray->y = (syRandom() & 255) + 128;
					if(syRandom() & 1)
						ray->y = -ray->y;
					ray->variant = -4 * i;
				}
			}

			corners = syHeapAllocEWRAM(512);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 5;
			bias = 0;
			model->shade = bias;
			texture.logHeight = 2;
			texture.pixels = state->sheet + FORM_CHANGE_GLOW_BAND;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->mesh = BattleEffectSmallSquareMesh;
			model->vertices = corners;

			/* The rays spin twice as fast as Eclipse's and widen and narrow in
			   24 ticks. */
			for(i = 0; i != LIGHT_RAYS; i++) {
				PARTICLE *ray = &state->particles[i];

				ray->variant += 2;
				ray->x += ray->y * 2;
				if(ray->variant >= 0 && ray->variant < LIGHT_RAY_TICKS) {
					syInitMatrix();
					{
						Sint32 column = i & 3;

						syTranslate(
							cx + INTtoFIXED(column * 2 + 2) - toFIXED(64),
							cy - INTtoFIXED(column + 2) - toFIXED(64), 0);
					}
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
					syRotZ(ray->x);
					syScale(sySin(ray->variant *
								(TRIG_TURN / 2 / LIGHT_RAY_TICKS)) +
							toFIXED(1),
						toFIXED(2), toFIXED(1));
					btRenderTransformVertices(
						BattleEffectTaperedBeamCorners, corners, 4);
					btRenderDrawModelList(model);
				}
			}

			/* The four columns rise from the ground after the burst. */
			model->texture = &texture;
			model->vertices = corners;
			texture.logWidth = 6;
			texture.pixels = FORM_CHANGE_BILLOW;
			model->kind = MODEL_SHADED_ADDITIVE;
			texture.logHeight = 6;
			model->mesh = BattleEffectRingMesh;
			for(i = 0; i != 4; i++) {
				Sint32 start = FormChangeWaveStarts[i] + FORM_CHANGE_BURST;

				model->scrollV = (frm * 8 + i * 32) & TEXTURE_SCROLL_MASK;
				if(frm > start) {
					bias = (start - frm) * 8 + 56;
					if(bias > BAND_SHADE_BRIGHTEST)
						bias = BAND_SHADE_BRIGHTEST;
					if(bias > MODEL_SHADE_INVISIBLE) {
						Sint32 growth =
							(frm - start) * FormChangeWaveGrowth[i] * 2000 +
							toFIXED(1) / 4;

						model->shade = bias;
						syInitMatrix();
						syTranslate(
							INTtoFIXED(center.x / 2 - 60), toFIXED(32), 0);
						syScale(growth * 2,
							FormChangeWaveStretch[i] * growth + toFIXED(4),
							growth / 4);
						syRotX(FORM_CHANGE_COLUMN_TILT);
						if(i & 1)
							syRotY((i * 8 + frm) * (TRIG_TURN / 64));
						else
							syRotY((i * 8 - frm) * (TRIG_TURN / 64));
						btRenderTransformVertices(
							BattleEffectRingCorners, corners, 32);
						btRenderDrawModelList(model);
					}
				}
			}
			syHeapRelease(model);
			syHeapRelease(corners);
		}

		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(kind == FORMATION_DOOM_DRAGON_C) {
		/* Put the battle's view back by hand. */
		sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
		BgScrollWork[1].x = state->savedBg1X;
		BgScrollWork[1].y = state->savedBg1Y;
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
		syCloseEvent(btEffectDarkenBackgroundStep);
	} else {
		btEffectEndCanvasLayer();
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Copies the canvas to VRAM once the effect has finished a tick, then clears,
   fades, darkens or brightens it as transferMode says. */
void btEffectTransferCanvas(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void				*src = Ram_HeapSlots->battleCanvas;
	Uint32				 ime;

	IRQ_LOCK(ime);
	if(state->transferPending == 1) {
		state->transferPending = 0;
		IRQ_UNLOCK(ime);
		switch(state->transferMode) {
		case CANVAS_COPY:
			syWordsCopy((void *)BG_CHAR_BLOCK(1), src, CANVAS_BYTES);
			break;
		case CANVAS_COPY_AND_CLEAR:
			syWordsCopy((void *)BG_CHAR_BLOCK(1), src, CANVAS_BYTES);
			syWordsFill(src, CANVAS_BYTES, state->transferValue);
			break;
		case CANVAS_COPY_AND_FADE:
			if(state->transferValue == CANVAS_FADE_HALF)
				syColBufferHalve(src, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
			else
				syColBufferScaleThreeQuarters(
					src, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
			break;
		case CANVAS_COPY_AND_DARKEN:
			syColBufferDarken(src, state->transferValue,
				(void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
			break;
		case CANVAS_COPY_AND_BRIGHTEN:
			syColBufferBrighten(src, state->transferValue,
				(void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
			break;
		}
		/* DEAD: nothing uses the counter read here; the ROM keeps its load. */
		(void)state->framesSinceTransfer;
		state->framesSinceTransfer = 1;
	} else {
		IRQ_UNLOCK(ime);
		state->framesSinceTransfer++;
	}
}

/* Copies the stamp's picture to its sprite's tiles once the effect has drawn
   it, then clears the picture. */
void btEffectProcessPendingStampTransfer(void)
{
	Uint8  *state = Ram_HeapSlots->battleEff;
	Uint32	ime;
	Sint32 *pending;

	IRQ_LOCK(ime);
	pending = &((BATTLE_EFFECT_STATE *)state)->stampPending;
	if(*pending == 1) {
		*pending = 0;
		IRQ_UNLOCK(ime);
		syWordsCopy((void *)STAMP_TILES(state), STAMP_CANVAS(state),
			OBJ_64X64_8BPP_BYTES);
		syWordsFill(STAMP_CANVAS(state), OBJ_64X64_8BPP_BYTES, 0);
	} else {
		IRQ_UNLOCK(ime);
	}
}

/* Copies a picture the size of the battle background from the map cell buffer
   to BG character block 2 once the effect has finished a tick, as transferMode
   says. */
void btEffectTransferFullScreenImage(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void				*source = Ram_MapMetatileBuffer;
	Uint32				 ime;

	IRQ_LOCK(ime);
	if(state->transferPending == 1) {
		state->transferPending = 0;
		IRQ_UNLOCK(ime);
		switch(state->transferMode) {
		case CANVAS_COPY:
			syWordsCopy(
				(void *)BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
			break;
		case CANVAS_COPY_AND_CLEAR:
			syWordsCopy(
				(void *)BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
			syWordsFill(source, BATTLE_BACKGROUND_BYTES, state->transferValue);
			break;
		case CANVAS_COPY_AND_FADE:
			if(state->transferValue == CANVAS_FADE_HALF)
				syColBufferHalve(
					source, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
			else
				syColBufferScaleThreeQuarters(
					source, (void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
			break;
		case CANVAS_COPY_AND_DARKEN:
			syColBufferDarken(source, state->transferValue,
				(void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES);
			break;
		}
		/* DEAD: nothing uses the counter read here; the ROM keeps its load. */
		(void)state->framesSinceTransfer;
		state->framesSinceTransfer = 1;
	} else {
		IRQ_UNLOCK(ime);
		state->framesSinceTransfer++;
	}
}

/* The same for a picture drawn in the canvas's heap slot: a plain copy takes
   all of BATTLE_BACKGROUND_BYTES, while the clear, the fade and the darkening
   take only its first CANVAS_VIEW_BYTES. */
void btEffectProcessPendingBlock2Transfer(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void				*src;
	Uint32				 ime;

	IRQ_LOCK(ime);
	if(state->transferPending == 1) {
		src = Ram_HeapSlots->battleCanvas;
		state->transferPending = 0;
		IRQ_UNLOCK(ime);
		switch(state->transferMode) {
		case CANVAS_COPY:
			syWordsCopy((void *)BG_CHAR_BLOCK(2), src, BATTLE_BACKGROUND_BYTES);
			break;
		case CANVAS_COPY_AND_CLEAR:
			syWordsCopy((void *)BG_CHAR_BLOCK(2), src, BATTLE_BACKGROUND_BYTES);
			syWordsFill(src, CANVAS_VIEW_BYTES, state->transferValue);
			break;
		case CANVAS_COPY_AND_FADE:
			if(state->transferValue == CANVAS_FADE_HALF)
				syColBufferHalve(
					src, (void *)BG_CHAR_BLOCK(2), CANVAS_VIEW_BYTES);
			else
				syColBufferScaleThreeQuarters(
					src, (void *)BG_CHAR_BLOCK(2), CANVAS_VIEW_BYTES);
			break;
		case CANVAS_COPY_AND_DARKEN:
			syColBufferDarken(src, state->transferValue,
				(void *)BG_CHAR_BLOCK(2), CANVAS_VIEW_BYTES);
			break;
		}
		/* DEAD: nothing uses the counter read here; the ROM keeps its load. */
		(void)state->framesSinceTransfer;
		state->framesSinceTransfer = 1;
	} else {
		IRQ_UNLOCK(ime);
		state->framesSinceTransfer++;
	}
}

/* Row `row` of BG1's map. */
#define BG1_MAP_ROW(row) \
	(BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) + (row) * MAP_ROW_BYTES)

/* Writes BG1's map to show a picture CANVAS_SIZE wide from BG character block
   2, as btTransitionBuildFlatTilemap does for the battle background: four rows
   of blank entries, two of BG1_CLEAR_TILE, then the picture's tiles in order,
   CANVAS_TILES rows of them, each row closed with BG1_CLEAR_TILE. */
void btEffectBuildCanvasTilemap(void)
{
	Uint32 *map = (Uint32 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
	Uint32	tiles;
	Sint32	row;
	Sint32	column;

	syWordsFill((void *)map, 4 * MAP_ROW_BYTES, -1);
	map += 4 * MAP_ROW_WORDS;
	syWordsFill((void *)map, 2 * MAP_ROW_BYTES,
		MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE));
	map += 2 * MAP_ROW_WORDS;
	tiles = MAP_ENTRY_PAIR(BG1_BLOCK2_FIRST_TILE, BG1_BLOCK2_FIRST_TILE + 1);
	for(row = 0; row != CANVAS_TILES; row++) {
		for(column = 0; column != 8; column++) {
			*map++ = tiles;
			tiles += MAP_ENTRY_PAIR(2, 2);
		}
		for(column = 0; column != 8; column++)
			*map++ = MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE);
	}
}

/* Writes the window, display and blend registers the effect keeps in its state,
   all at once. */
void btEffectApplyWindowBlendRegisters(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	REG_WIN0H = state->win0h;
	REG_WIN0V = state->win0v;
	REG_WIN1H = state->win1h;
	REG_WIN1V = state->win1v;
	REG_WININ = state->winin;
	REG_WINOUT = state->winout;
	REG_DISPCNT = state->dispcnt;
	REG_BLDCNT = state->bldcnt;
	REG_BLDALPHA = state->bldalpha;
}

/* Writes the effect's BG2 scale to BG2PA and BG2PD and its reference point to
   BG2X and BG2Y. */
void btDispApplyBg2Affine(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	REG_BG2PA = state->bg2pa;
	REG_BG2PD = state->bg2pd;
	REG_BG2X = state->bg2x;
	REG_BG2Y = state->bg2y;
}

/* Writes the effect's BG2 reference point to BG2X and BG2Y. */
void btEffectApplyBg2Reference(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	REG_BG2X = state->bg2x;
	REG_BG2Y = state->bg2y;
}

/* Darkens the battle background a step each tick while the canvas is open, so
   the effect stands out, for as many ticks as fadeTicks says. */
void btEffectDarkenBackgroundStep(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	Sint32				*ticksLeft;
	BATTLE_STATE		*battle;

	ticksLeft = &state->fadeFrm;
	battle = Ram_HeapSlots->battleState;

	if(*ticksLeft > 0) {
		Sint32 *step = &state->fadeStep;

		(*step)++;
		btColBufferScaleStub(battle->pal, (void *)&BG_PLTT_COLOR(6, 0),
			toFIXED(1) - *step * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
		(*ticksLeft)--;
	}
}

/* Clears every target's flash timer. */
void btEffectClearTintTimers(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	syWordsClear(state->actorTmrs, sizeof state->actorTmrs);
}

/* Counts down each target's flash timer and gives it its own palette back when
   one runs out. */
void btEffectCountDownBattlerTints(void)
{
	BATTLE_EFFECT_STATE *state;
	Sint32				 actorIdx;

	state = Ram_HeapSlots->battleEff;
	for(actorIdx = 0; actorIdx != 8; actorIdx += 1) {
		if(state->actorTmrs[actorIdx] != 0) {
			if((state->actorTmrs[actorIdx] = state->actorTmrs[actorIdx] - 1) ==
				0) {
				btEffectTintBattler(state->args->tgts[actorIdx],
					SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
			}
		}
	}
}

/* Shows the canvas on BG2 stretched to twice its width across the battle view,
   blended over the battle, with windows 0 and 1 over the battle view. */
void btEffectSetupCanvasWindows(void)
{
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BG2X = 0;
	REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = Q_8_8(1);
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
}

/* Opens the canvas layer over the battle, darkening the battle background
   behind it. */
void btEffectStartCanvasLayer(Sint32 bgControl)
{
	BATTLE_EFFECT_STATE		*state = Ram_HeapSlots->battleEff;
	BATTLE_STATE			*battle = Ram_HeapSlots->battleState;
	void					*canvas = Ram_HeapSlots->battleCanvas;
	BATTLE_BACKGROUND_STATE *display = Ram_HeapSlots->battleBg;
	Sint32					 row;
	Sint32					 column;
	Sint32					 ofs;

	btEffectConfigureEffectDisp();
	btEffectClearTintTimers();
	display->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
	sySynch(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	sySynch(1);

	btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	state->fadeFrm = 0;
	state->fadeStep = 0;
	sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_TICK);

	/* Change BG2's settings while it is hidden. */
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	sySynch(1);
	REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
	display->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
	sySynch(1);

	btEffectSetupCanvasWindows();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

	ofs = 0;
	for(row = 0; row != CANVAS_TILES; row++) {
		EFFECT_WRITE_CANVAS_MAP_ROW(row, column, ofs);
	}

	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	state->shakeFrm = 0;
	state->transferPending = 0;
	state->savedBg1X = BgScrollWork[1].x;
	state->savedBg1Y = BgScrollWork[1].y;
	sySynch(1);
}

/* Unused: opens the canvas layer as btEffectStartCanvasLayer does, with the
   windows off and the battle background's saved colours put back before it
   darkens; nothing calls it. */
UNUSED void btEffectOpenCanvasLayer(Sint32 bgControl)
{
	BATTLE_EFFECT_STATE		*state = Ram_HeapSlots->battleEff;
	BATTLE_STATE			*battle = Ram_HeapSlots->battleState;
	void					*canvas = Ram_HeapSlots->battleCanvas;
	BATTLE_BACKGROUND_STATE *display = Ram_HeapSlots->battleBg;
	Sint32					 row;
	Sint32					 column;
	Sint32					 ofs;

	btEffectClearTintTimers();
	display->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
	sySynch(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	sySynch(1);
	btBackgroundSetModeStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BATTLE_BACKGROUND_FADE_RESTORE);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	sySetEvent(btEffectDarkenBackgroundStep, EVENT_PASS_TICK);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	sySynch(1);
	REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
	display->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
	sySynch(1);

	btEffectSetupCanvasWindows();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	sySynch(1);

	ofs = 0;
	for(row = 0; row != CANVAS_TILES; row++) {
		EFFECT_WRITE_CANVAS_MAP_ROW(row, column, ofs);
	}

	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	state->shakeFrm = 0;
	state->transferPending = 0;
	sySynch(1);
}

/* Opens the canvas layer with the canvas repeated across a wrapping map. */
void btEffectStartTiledCanvas(Sint32 bgControl)
{
	Sint32 row;
	Sint32 column;
	Sint32 ofs;

	btEffectStartCanvasLayer(bgControl);
	REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_SIZE(1);
	ofs = 0;
	for(row = 0; row != CANVAS_TILES; row++) {
		EFFECT_WRITE_CANVAS_MAP_ROW(row, column, ofs);
		for(column = 0; column != 8; column++, ofs += 2) {
			*(volatile Uint16 *)((Uint8 *)BG_CANVAS_MAP + ofs) = 0;
		}
	}
}

/* Two entries of BG2's affine map, a byte each, both showing canvas tile
   `tile`. */
#define AFFINE_TILE_PAIR(tile) ((tile) << 8 | (tile))

/* Opens the canvas layer as btEffectStartTiledCanvas does, with the canvas's
   top right tile in the rest of each row in place of its top left. */
void btEffectStartTiledCanvasFilled(Sint32 bgControl)
{
	Sint32 row;
	Sint32 column;
	Sint32 ofs;

	btEffectStartCanvasLayer(bgControl);
	REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_SIZE(1);
	ofs = 0;
	for(row = 0; row != CANVAS_TILES; row++) {
		EFFECT_WRITE_CANVAS_MAP_ROW(row, column, ofs);
		for(column = 0; column != 8; column++, ofs += 2) {
			*(volatile Uint16 *)((Uint8 *)BG_CANVAS_MAP + ofs) =
				AFFINE_TILE_PAIR(CANVAS_TILES - 1);
		}
	}
}

/* The levels the battle background brightens by each tick as the canvas
   closes, back to its own colours. */
#define BACKGROUND_RESTORE_STEP 3

/* Closes the canvas layer and brings the battle background's colours back. */
void btEffectEndCanvasLayer(void)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	BATTLE_STATE		*battle = Ram_HeapSlots->battleState;
	Sint32				 i;

	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = state->savedBg1Y;
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	syCloseEvent(btEffectDarkenBackgroundStep);
	/* This overwrites the saved y restored above, so BG1 is always left at
	   BATTLE_BACKGROUND_SCROLL_Y. */
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	REG_BLDCNT = 0;
	sySynch(1);

	btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 7);
	sySynch(1);
	/* The background brightens from the darkened level back to its own colours
	   over eight ticks. */
	for(i = 0; i != BACKGROUND_DARKENED_LEVEL / BACKGROUND_RESTORE_STEP; i++) {
		btBackgroundSetPaletteLevelStub(battle->bg,
			BACKGROUND_DARKENED_LEVEL - BACKGROUND_RESTORE_STEP -
				i * BACKGROUND_RESTORE_STEP);
		sySynch(1);
	}

	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	sySynch(1);
}

/* Hides BG1 and the windows, starts bringing the battle background's colours
   back, and gives BG1 back its own map. */
void btEffectHideBg1AndRestoreBackdrop(void)
{
	BATTLE_STATE *battle;
	Sint32		 *flg;

	flg = &(Ram_HeapSlots->battleBg)->bg1ScrollMode;
	battle = Ram_HeapSlots->battleState;
	*flg = BATTLE_BACKGROUND_FIXED;
	syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON);
	sySynch(1);
	btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG1CNT,
		BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK),
		IO_WRITE_HALFWORD);
	sySynch(1);
}

/* The tile, cleared, that BG1 shows around the canvas. */
#define CANVAS_BG1_BLANK_TILE 255
/* The canvas's first tile counted from BG1's character base: the canvas is
   character block 1. */
#define CANVAS_BG1_FIRST_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The map row where the canvas starts on BG1, 96 pixels down: the effects that
   show it there scroll BG1 to y 80 to show it from the battle view's top
   line. */
#define CANVAS_BG1_TOP_ROW	  12

/* Shows the canvas on BG1 instead of BG2, at its own size. */
void btEffectSetupCanvasTileMap(void)
{
	void					*canvas = Ram_HeapSlots->battleCanvas;
	BATTLE_STATE			*screen = Ram_HeapSlots->battleState;
	BATTLE_BACKGROUND_STATE *layer = Ram_HeapSlots->battleBg;
	Sint32					 row;
	Sint32					 column;
	Sint32					 byteOfs;

	btEffectClearTintTimers();
	btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, screen->bg, 0);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	layer->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
	/* A blank tile, then the rows above the canvas, from the first one BG1
	   shows, which show nothing. */
	syWordsClear(BG_CHAR_BLOCK(0) + CANVAS_BG1_BLANK_TILE * TILE_SIZE_8BPP,
		TILE_SIZE_8BPP);
	syWordsFill(BG1_MAP_ROW(BATTLE_BACKGROUND_SCROLL_Y / 8),
		(CANVAS_BG1_TOP_ROW - BATTLE_BACKGROUND_SCROLL_Y / 8) * MAP_ROW_BYTES,
		-1);
	/* The canvas's tiles, then the blank tile across the rest of each row. */
	byteOfs = 0;
	for(row = 0; row != CANVAS_TILES; row++) {
		for(column = 0; column != BG_SCREEN_WIDTH; column++, byteOfs += 2) {
			if(column >= CANVAS_TILES)
				*(volatile Uint16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) +
					byteOfs) = CANVAS_BG1_BLANK_TILE;
			else
				*(volatile Uint16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) +
					byteOfs) =
					row * CANVAS_TILES + CANVAS_BG1_FIRST_TILE + column;
		}
	}
	REG_DISPCNT = EFFECT_DISPCNT;
	REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	sySynch(1);
}

/* A line follows its slower axis in fixed point with LINE_FRACTION_BITS of
   fraction, kept at the top of a word: a pixel's worth carries into the sign
   bit, LINE_FRACTION_SHIFT up. */
#define LINE_FRACTION_BITS	8
#define LINE_PIXEL			(1 << LINE_FRACTION_BITS)
#define LINE_FRACTION_SHIFT (31 - LINE_FRACTION_BITS)

/* Draws a line on the canvas, keeping the brighter colour where it crosses what
   is drawn. */
void btEffectDrawClippedCanvasLine(
	Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 col)
{
	Sint32 dx = x1 - x0;
	Sint32 dy = y1 - y0;
	Sint32 frac = LINE_PIXEL / 2 << LINE_FRACTION_SHIFT;
	Uint8 *canvas = Ram_HeapSlots->battleCanvas;
	Sint32 step;
	Sint32 i;
	Sint32 x;
	Sint32 y;
	Sint32 tmp;
	Uint32 ofs;

	CLAMP(y0, 0, CANVAS_SIZE - 1);
	CLAMP(y1, 0, CANVAS_SIZE - 1);

	if(ABS(dx) < ABS(dy)) {
		if(dy < 0) {
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			/* LEFTOVER(template): dy recomputed after the swap and never read
			 * again, as the shallow arm's swap below recomputes both deltas and
			 * reads dy for its y step; as the line removed. */
			dy = y1 - y0;
		}
		step = (ABS(x1 - x0) << LINE_FRACTION_BITS) / ABS(y1 - y0);
		step <<= LINE_FRACTION_SHIFT;
		x = x0;
		/* dx now steps x a pixel toward the line's end. */
		if(dx < 0)
			dx = -1;
		else
			dx = 1;
		for(i = y0; i != y1; i++) {
			ofs = CANVAS_PIXEL_OFFSET((Uint32)x, (Uint32)i);
			if(canvas[ofs] < col)
				canvas[ofs] = col;
			frac += step;
			if(frac < 0) {
				x += dx;
				frac = (Uint32)(frac << 1) >> 1;
			}
		}
	} else {
		if(dx < 0) {
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			/* LEFTOVER(template): dx recomputed after the swap and never read
			 * again, as the steep arm's swap above recomputes both deltas and
			 * reads dx for its x step; as the line removed. */
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = (ABS(y1 - y0) << LINE_FRACTION_BITS) / ABS(x1 - x0);
		step <<= LINE_FRACTION_SHIFT;
		y = y0;
		if(dy < 0)
			dy = -1;
		else
			dy = 1;
		for(i = x0; i != x1; i++) {
			ofs = CANVAS_PIXEL_OFFSET((Uint32)i, (Uint32)y);
			if(canvas[ofs] < col)
				canvas[ofs] = col;
			frac += step;
			if(frac < 0) {
				y += dy;
				frac = (Uint32)(frac << 1) >> 1;
			}
		}
	}
}

#undef FLASHES
#undef GLOW_BAND_BYTES
#undef LIGHT_RAYS
#undef LIGHT_RAY_TICKS
#undef BAND_SHADE_BRIGHTEST
#undef FLASH_PICTURE_TICKS
#undef BG1_MAP_ROW
#undef AFFINE_TILE_PAIR
#undef BACKGROUND_RESTORE_STEP
#undef CANVAS_BG1_BLANK_TILE
#undef CANVAS_BG1_FIRST_TILE
#undef CANVAS_BG1_TOP_ROW
#undef LINE_FRACTION_BITS
#undef LINE_PIXEL
#undef LINE_FRACTION_SHIFT
