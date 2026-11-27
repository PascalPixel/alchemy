/* @file */
/* Trident, the leaping strikes of Planet Diver and Planetary, Meteor Blow's */
/* palette DMA, and the grid mesh builder. */
/* Part of battle_effect.c. */

/* The leaping strike's modes (btEffectRunLeapingStrike). */
typedef enum LEAPING_STRIKE_MODE {
	LEAPING_STRIKE_PLANET_DIVER,
	LEAPING_STRIKE_PLANETARY
} LEAPING_STRIKE_MODE;

extern const SCALE Trident_UnitScale;

#define TRIDENT_FRAMES 104
/* The Trident drops into place over its first 10 frames, and its strike */
/* lands at TRIDENT_STRIKE_FRAME. */
#define TRIDENT_STRIKE_FRAME 64
/* The Trident is four sprites of SPRITE_TRIDENT, stacked a sprite's height */
/* apart, shown in window 0. */
#define TRIDENT_SPRITES		  4
#define TRIDENT_SPRITE_HEIGHT 32
#define TRIDENT_HEIGHT		  (TRIDENT_SPRITES * TRIDENT_SPRITE_HEIGHT)
#define TRIDENT_WINDOW_TOP	  16
#define TRIDENT_WINDOW_BOTTOM 124
#define TRIDENT_WINDOW_HEIGHT (TRIDENT_WINDOW_BOTTOM - TRIDENT_WINDOW_TOP)

/* The sparks the strike throws, in the map cell buffer. */
#define TRIDENT_SPARKS		((PARTICLE *)(Ram_MapCellBuffer + 10596))
#define TRIDENT_SPARK_COUNT 128
/* The state's particles: the discs thrown up, the first one growing on the */
/* ground, then the strike's flashes, which live six pictures of four frames. */
#define TRIDENT_PARTICLES	 32
#define TRIDENT_DISCS		 14
#define TRIDENT_DISC_FRAMES	 32
#define TRIDENT_FLASH_FRAMES 24

/* Twelve shaded discs, 2 to 24 wide, drawn into the map cell buffer from */
/* TRIDENT_DISC_PICTURES; where each starts is kept in the effect's pictures */
/* at TRIDENT_DISC_TABLE, and the growing disc is drawn at */
/* TRIDENT_GROUND_DISC. */
#define TRIDENT_DISC_SIZES	  12
#define TRIDENT_DISC_PICTURES 9216
#define TRIDENT_DISC_TABLE	  19542
#define TRIDENT_GROUND_DISC	  19590

/* Where JaggedStreakSheet is decoded in the effect's pictures, after the */
/* tornado sprite sheet's glints: three pictures of a jagged bolt, */
/* JAGGED_STREAK_WIDTH by JAGGED_STREAK_HEIGHT. */
#define TRIDENT_JAGGED_STREAKS GLINT_SET_BYTES
/* Seven bolts spin in, one every 4 frames from TRIDENT_FIRST_BOLT_FRAME, */
/* each turning for 24 frames, dimmed by TRIDENT_BOLT_SHADE. */
#define TRIDENT_BOLTS			 7
#define TRIDENT_BOLT_SPINS		 8 /* rows of Trident_Spins */
#define TRIDENT_FIRST_BOLT_FRAME 18
#define TRIDENT_BOLT_SHADE		 (-16)

/* A ring of BattleEffect_CylinderMesh starts at about a quarter of its */
/* size and grows by its row's growth times CYLINDER_RING_GROWTH a frame. */
#define CYLINDER_RING_START_SCALE (FIXED_ONE * 131 / 512)
#define CYLINDER_RING_GROWTH	  1500
/* The rings spreading from the targets lean back by this angle. */
#define TRIDENT_RING_TILT 3000
#define TRIDENT_RINGS	  8

/* Where FlameZigzagTexture, BillowTexture and FlashBurstSheet are decoded in */
/* the map cell buffer. */
#define TRIDENT_FLAME_ZIGZAG 4096
#define TRIDENT_BILLOW		 5120
#define TRIDENT_FLASH_BURST	 20480

/* The Trident drops into place over the targets as rings spread out from */
/* them; seven jagged bolts spin in, shaded discs are thrown up from a disc */
/* growing on the ground, and then the strike lands on every target with */
/* flashes and a burst of sparks, and more rings spread. */
/* */
/* Abilities: Trident. */
void btEffectRunTrident(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	u8 *sprites;
	s32 centerX;
	s16 *circles;
	SCR_POINT middle;
	FIXED placement[4];
	SCALE source;
	MODEL_TEXTURE texture;
	RENDER_MODEL *model;
	void *vertices;
	PARTICLE *p;
	s32 frame;
	s32 i;
	s32 size;
	s32 offset;
	s32 x;
	s32 grow; /* also each bolt's first frame */
	s32 palette;
	u8 *cells;
	s32 orange;
	s32 orangeBase;
	DRAW_RECTANGLE draw;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprs(TRIDENT_SPRITES, SPRITE_TRIDENT, 1);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
				WININ_WIN1_BG2 | WININ_WIN1_CLR;
	REG_WINOUT = EFFECT_WINOUT;
	REG_WIN0V = WIN_RANGE(TRIDENT_WINDOW_TOP, TRIDENT_WINDOW_BOTTOM);
	REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw = Ram_HeapSlots->ramCode;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	blit[0] = draw;
	/* FAKEMATCH(temporary): the orange palette's id in a local. */
	orangeBase = RESOURCE_ORANGE_PALETTE_B - 4;
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	circles = (s16 *)(state->sheet + TRIDENT_DISC_TABLE);
	btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
	/* FAKEMATCH(dead): offset set to the cell buffer's address, which the disc
	 * loop below sets again before any read. */
	offset = (s32)MapGridCellBuffer;
	btResourceLoadAndDecompress(RESOURCE_JAGGED_STREAK_SHEET,
								state->sheet + TRIDENT_JAGGED_STREAKS, 0, 0);
	orange = orangeBase + 4;
	size = 2;
	/* FAKEMATCH(dead): a test on the blitter. */
	if(draw) {
		/* FAKEMATCH(dead): i set to four below the effect palette's id, which
		 * the loop below sets to 0 before any read. */
		i = RESOURCE_VIOLET_PALETTE_F - 4;
		cells = MapGridCellBuffer;
		palette = RESOURCE_VIOLET_PALETTE_F;
	}
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
								MapGridCellBuffer + TRIDENT_FLASH_BURST, 1, 0);
	offset = TRIDENT_DISC_PICTURES;
	for(i = 0; i != TRIDENT_DISC_SIZES; i++) {
		/* As an integer sum, the offset first. */
		btEffectDrawShadedCircles((u8 *)(offset + (s32)cells), size, FIXED_ONE);
		circles[i] = offset;
		offset += (i * 2 + 3) * size / 2;
		size += 2;
	}
	btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, cells, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLAME_ZIGZAG_TEXTURE,
								Ram_MapCellBuffer + TRIDENT_FLAME_ZIGZAG, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
								Ram_MapCellBuffer + TRIDENT_BILLOW, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
					 syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_F),
					 SHEET_PALETTE_BYTES);
	for(i = 0, p = state->particles; i != TRIDENT_PARTICLES; i++, p++) {
		s32 radius = i * 2;
		s32 angle = syRandom() & TRIG_ANGLE_MASK;

		/* DEAD: each place and life is set again, at frame 12 and as its disc
		 * is */
		/* thrown or its flash starts, before anything reads it; the ROM keeps
		 * the */
		/* stores, and the random draws move the random sequence. */
		p->x = sySin(angle) * radius;
		p->y = -(syCos(angle) * radius);
		p->variant = i / 2 + 25;
	}
	btEffectGetTargetsMidpoint(&middle);
	centerX = middle.x / 2;

	for(frame = 0; frame != TRIDENT_FRAMES; frame++) {
		if(frame == 0) {
			syWordsCopyEntry((void *)BG_PLTT, syResourceGetTableEntry(palette),
							 SHEET_PALETTE_BYTES);
		}
		if(frame > 76) {
			btEffectStepPaletteToResource(palette);
		}
		else if(frame > 64) {
			btEffectStepPaletteToResource(orange);
		}
		else if(frame > 23) {
			btEffectStepPaletteToResource(palette);
		}
		else if(frame > 8) {
			btEffectStepPaletteToResource(orange);
		}
		if(frame == 8) {
			state->shakeFrm = frame;
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectTintBattler(args->tgts[i], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, i, 16);
			}
		}
		if(frame == 48) {
			state->shakeFrm = 8;
		}
		if(frame == TRIDENT_STRIKE_FRAME) {
			state->shakeFrm = 16;
			EFFECT_SHOW_DAMAGE(SE_CLATTER);
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectStartActorMotion(args->tgts[i], 1, toFIXED(2),
										 toFIXED(8), toFIXED(8), 120);
			}
		}
		if(frame == 0) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frame == 4) {
			sySndPlayStub(SE_CLATTER);
		}
		if(frame == 16) {
			sySndPlayStub(SE_SCATTER);
		}
		if(frame == TRIDENT_STRIKE_FRAME) {
			sySndPlayStub(SE_CRASH);
		}

		/* The Trident drops into place, 16 pixels a frame, and stays until the
		 */
		/* strike. */
		if(frame < TRIDENT_STRIKE_FRAME) {
			s32 left = centerX - 8;
			s32 visible = TRIDENT_HEIGHT;
			s32 rise;

			if(frame <= 9) {
				rise = frame * 16 - 144;
			}
			else {
				rise = 0;
			}
			if(rise + TRIDENT_HEIGHT > TRIDENT_WINDOW_HEIGHT) {
				visible =
					visible - rise - (TRIDENT_HEIGHT - TRIDENT_WINDOW_HEIGHT);
			}
			/* DEAD: always true, as rise is at most 0; the ROM keeps the test.
			 */
			if(visible > 0) {
				source = Trident_UnitScale;
				placement[3] = 0;
				placement[1] = toFIXED(255);
				for(i = 0; i != TRIDENT_SPRITES; i++) {
					placement[0] = INTtoFIXED(left * 2) + toFIXED(16);
					placement[2] =
						INTtoFIXED(rise + 40 + i * TRIDENT_SPRITE_HEIGHT);
					btRenderApplyProjectedPlacementStub(
						state->sprs[i], placement, (s32 *)&source, 0);
				}
			}
		}

		/* A disc grows on the ground from frame 40, and discs are thrown up,
		 * one */
		/* every other frame from frame 12. */
		if((u32)(frame - 12) <= 52) {
			if(frame == 12) {
				for(i = 0; i != TRIDENT_PARTICLES; i++) {
					state->particles[i].variant = -2 * i;
				}
			}
			for(i = 0, p = state->particles, grow = frame * 4 - 156;
				i != TRIDENT_DISCS; i++, p++) {
				p->variant++;
				if(i == 0) {
					if(frame > 39) {
						u8 *disc = state->sheet + TRIDENT_GROUND_DISC;

						btEffectDrawShadedCircles(disc, grow, FIXED_ONE);
						btCanvasBlitMirroredQuarters(disc, centerX, 100, grow);
					}
					continue;
				}
				if(p->variant == 0) {
					s32 angle = syRandom() & 0x7fff;
					s32 heading = angle + DEGtoANG(90);
					s32 speed = (syRandom() & 511) + 512;
					FIXED sine;

					sine = sySin(heading) * speed;
					x = INTtoFIXED(centerX);
					p->x = (sine >> 4) + x;
					p->y = ((syCos(heading) * speed) >> 3) + toFIXED(104);
					angle += TRIG_TURN * 3 / 4;
					p->velX = (sySin(angle) * speed) >> 9;
					p->velY = (syCos(angle) * speed) >> 8;
					p->velZ = (syRandom() & 7) + 4;
				}
				/* A disc swells and shrinks over its life, half a turn of a
				 * sine. */
				if((u32)p->variant < TRIDENT_DISC_FRAMES) {
					s32 diameter =
						(FIXEDtoINT(sySin(p->variant * (TRIG_TURN / 64)) *
									p->velZ)) *
						2;
					s32 index = diameter - 2;

					if((u32)index <= 61) {
						s32 discX = FIXED_INTEGER_PART(p->x);
						s32 discY = FIXED_INTEGER_PART(p->y);
						s32 tier = index >> 1;

						btParticleAdvanceWithGravity2D(p, PARTICLE_DAMPING_NONE,
													   0);
						/* As an integer sum, the offset first. */
						btCanvasBlitMirroredQuarters(
							(u8 *)(circles[tier] + (s32)cells), discX, discY,
							diameter);
					}
				}
			}
		}

		if(frame > 7) {
			s32 scale;

			vertices = syHeapAllocEwram(1024);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->flg = 0;
			model->mesh = BattleEffect_CylinderMesh;
			model->vertices = vertices;
			model->scrollU = 0;
			model->scrollV = 0;

			/* Eight rings spread from the targets, four as the Trident drops
			 * and */
			/* four at the strike. */
			for(i = 0; i != TRIDENT_RINGS; i++) {
				s32 start = Trident_RingStarts[i];
				s32 shade;

				if(frame <= start) {
					continue;
				}
				scale = (frame - start) * Trident_RingGrowth[i] *
							CYLINDER_RING_GROWTH +
						CYLINDER_RING_START_SCALE;
				shade = (start - frame) * 8 + 56;
				if(shade > 0) {
					shade = 0;
				}
				if(shade <= MODEL_SHADE_INVISIBLE) {
					continue;
				}
				model->shade = shade;
				syInitMatrix();
				syTranslate(INTtoFIXED(centerX - 64),
							INTtoFIXED(Trident_RingHeights[i]), 0);
				syScale(scale, scale * 2, scale);
				model->scrollU = ((frame + i * 5) & 7) << 4;
				syRotX(TRIDENT_RING_TILT);
				btRenderTransformVertices(BattleEffect_CylinderCorners,
										  vertices, 32);
				btRenderDrawModelList(model);
			}

			if(frame <= 71) {
				const s32 *spin;
				PARTICLE *t;
				PARTICLE *spinner;

				texture.logWidth = 5;
				texture.logHeight = 7;
				model->texture = &texture;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = BattleEffect_ColumnMesh;
				model->vertices = vertices;
				model->scrollU = 0;
				model->scrollV = 0;
				if(frame == 8) {
					for(i = 0, t = &state->particles[TRIDENT_PARTICLES],
					spin = Trident_Spins;
						i != TRIDENT_BOLT_SPINS; i++, t++, spin += 2) {
						t->z = spin[0];
						t->velZ = spin[1];
						t->variant = (i % 6) * 4;
					}
				}
				for(i = 0, grow = TRIDENT_FIRST_BOLT_FRAME,
				spinner = &state->particles[TRIDENT_PARTICLES];
					i != TRIDENT_BOLTS; grow += 4, i++, spinner++) {

					scale = i * 4;
					if(frame < grow || frame >= scale + 42) {
						continue;
					}
					x = (spinner->variant / 8) % 3;
					/* FAKEMATCH(dead): a test on x with both arms alike. */
					if(x) {
						model->shade = TRIDENT_BOLT_SHADE;
					}
					else {
						model->shade = TRIDENT_BOLT_SHADE;
					}
					texture.pixels = state->sheet +
									 x * PICTURE_BYTES(JAGGED_STREAK) +
									 TRIDENT_JAGGED_STREAKS;
					syInitMatrix();
					syTranslate(INTtoFIXED(centerX - 64), toFIXED(32), 0);
					syRotZ(spinner->z);
					if(i & 1) {
						syRotY(DEGtoANG(180));
					}
					spinner->z += spinner->velZ;
					syScale(FIXED_ONE * 2, FIXED_ONE + 10000, FIXED_ONE);
					btRenderTransformVertices(Trident_ProngCorners + x * 4,
											  vertices, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}

		/* The strike: flashes, half along the Trident's length and half thrown
		 * out */
		/* from its foot, and a burst of sparks. */
		if(frame >= TRIDENT_STRIKE_FRAME) {
			PARTICLE *drawn;
			const u8 *cellTable;

			if(frame == TRIDENT_STRIKE_FRAME) {
				for(i = 0, p = state->particles, x = INTtoFIXED(centerX);
					i != TRIDENT_PARTICLES; i++, p++) {
					s32 angle = syRandom();
					s32 speed = (syRandom() & 511) + 512;
					s32 velocityY;

					p->velX = (sySin(angle) * speed) >> 8;
					velocityY = (syCos(angle) * speed) >> 7;
					p->velY = velocityY;
					if(i >= TRIDENT_PARTICLES / 2) {
						p->x = (p->velX << 2) + x;
						p->y = (velocityY << 2) + toFIXED(100);
						p->variant = (TRIDENT_PARTICLES / 2 - i) / 4;
					}
					else {
						p->x = x;
						p->y = INTtoFIXED(syRandom() % 104);
						p->variant = -i / 4;
					}
				}
				for(i = 0, p = TRIDENT_SPARKS; i != TRIDENT_SPARK_COUNT;
					i++, p++) {
					s32 angle = (syRandom() & 0x7fff) + DEGtoANG(90);
					s32 speed = (syRandom() & 511) + 512;

					p->x = x;
					p->y = toFIXED(100);
					p->velX = (sySin(angle) * speed) >> 6;
					p->velY = (syCos(angle) * speed) >> 5;
					p->variant = (syRandom() & 15) + 16;
				}
			}
			for(i = 0, drawn = state->particles; i != TRIDENT_PARTICLES;
				i++, drawn++) {
				if((u32)drawn->variant < TRIDENT_FLASH_FRAMES) {
					blit[0](
						canvas,
						Ram_MapCellBuffer + TRIDENT_FLASH_BURST +
							(drawn->variant / 4) * PICTURE_BYTES(FLASH_BURST),
						FIXED_INTEGER_PART(drawn->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(drawn->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(drawn, 62, -toFIXED(1) / 4);
				}
				drawn->variant++;
			}
			for(i = 0, cellTable = (const u8 *)BattleEffect_MotePictures,
			drawn = TRIDENT_SPARKS;
				i != TRIDENT_SPARK_COUNT; i++, drawn++) {
				s32 moteSize;
				s32 at;

				if(drawn->variant < 0) {
					continue;
				}
				moteSize = (drawn->variant >> 3) + 1;
				at = (moteSize - 1) * 2;
				blit[0](canvas, sprites + *(const u16 *)(cellTable + at),
						FIXED_INTEGER_PART(drawn->x) - moteSize / 2,
						FIXED_INTEGER_PART(drawn->y) - moteSize, moteSize,
						moteSize * 2);
				drawn->variant--;
				btParticleAdvanceWithGravity2D(drawn, 56, toFIXED(1) / 8);
			}
		}

		/* Glints flash up and down the Trident. */
		if((u32)(frame - 24) <= 39) {
			for(i = 0; i != 4; i++) {
				s32 k = i & 3;
				s32 angle = syRandom();
				s32 x = centerX + (FIXEDtoINT(sySin(angle) << 3)) -
						(BattleEffect_GlintPictureWidths[k] >> 1);
				s32 y = (i << 5) + (FIXEDtoINT(syCos(angle) << 4)) -
						(BattleEffect_GlintPictureHeights[k] >> 1);
				u32 flip = syRandom();

				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
									Trident_GlintFlips[flip & 3] |
										EFFECT_CLIP_BOUNDS,
									BLITTER_MAX);
				(Ram_HeapSlots->ramCodePaired)(
					canvas, state->sheet + BattleEffect_GlintPictures[k], x, y,
					BattleEffect_GlintPictureWidths[k],
					BattleEffect_GlintPictureHeights[k]);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}
		btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	for(i = 0; i != TRIDENT_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

#undef TRIDENT_FRAMES
#undef TRIDENT_STRIKE_FRAME
#undef TRIDENT_SPRITES
#undef TRIDENT_SPRITE_HEIGHT
#undef TRIDENT_HEIGHT
#undef TRIDENT_WINDOW_TOP
#undef TRIDENT_WINDOW_BOTTOM
#undef TRIDENT_WINDOW_HEIGHT
#undef TRIDENT_SPARKS
#undef TRIDENT_SPARK_COUNT
#undef TRIDENT_PARTICLES
#undef TRIDENT_DISCS
#undef TRIDENT_DISC_FRAMES
#undef TRIDENT_FLASH_FRAMES
#undef TRIDENT_DISC_SIZES
#undef TRIDENT_DISC_PICTURES
#undef TRIDENT_DISC_TABLE
#undef TRIDENT_GROUND_DISC
#undef TRIDENT_JAGGED_STREAKS
#undef TRIDENT_BOLTS
#undef TRIDENT_BOLT_SPINS
#undef TRIDENT_FIRST_BOLT_FRAME
#undef TRIDENT_BOLT_SHADE
#undef TRIDENT_RING_TILT
#undef TRIDENT_RINGS
#undef TRIDENT_FLAME_ZIGZAG
#undef TRIDENT_BILLOW
#undef TRIDENT_FLASH_BURST

/* Abilities: Planet Diver. */
void btEffectRunPlanetDiver(BATTLE_EFFECT_PARM *args) {
	btEffectRunLeapingStrike(args, LEAPING_STRIKE_PLANET_DIVER);
}

/* Abilities: Planetary. */
void btEffectRunPlanetary(BATTLE_EFFECT_PARM *args) {
	btEffectRunLeapingStrike(args, LEAPING_STRIKE_PLANETARY);
}

#define LEAPING_STRIKE_FRAMES 88
/* The caster leaps at frame 4 and comes down at 16, sliding to the target */
/* over the next 16 frames. */
#define LEAPING_STRIKE_LEAP_FRAME	4
#define LEAPING_STRIKE_LAND_FRAME	16
#define LEAPING_STRIKE_SLIDE_FRAMES 16
/* The flames that ring the caster as it leaps. */
#define LEAPING_STRIKE_FLAMES 16
/* The sparks that burst from the caster as it lands, in the map cell buffer. */
#define LEAPING_STRIKE_SPARKS		   ((PARTICLE *)(Ram_MapCellBuffer + 24576))
#define LEAPING_STRIKE_SPARK_COUNT	   256
#define LEAPING_STRIKE_SPARK_MIN_SPEED 32
/* Planetary's claw: tilted, and drawn about two and a half times its size. */
#define LEAPING_STRIKE_CLAW_TILT  13192
#define LEAPING_STRIKE_CLAW_SCALE (FIXED_ONE * 5173 / 2048)
/* Planetary's rings of billows spread from frame 40, each from its own frame */
/* after, and its pillars of flame from frame 34; both lean back. */
#define LEAPING_STRIKE_WAVE_FRAME	40
#define LEAPING_STRIKE_WAVE_GROWTH	2000
#define LEAPING_STRIKE_WAVE_TILT	1000
#define LEAPING_STRIKE_PILLAR_FRAME 34
#define LEAPING_STRIKE_PILLAR_TILT	2000

/* Where FlameballSheet (Planet Diver) or ClawSlashSheet (Planetary) is decoded
 */
/* in the effect's pictures. */
#define LEAPING_STRIKE_PICTURES 12288

/* Where BillowTexture and FlameBandTexture are decoded in the effect's */
/* pictures. */
#define LEAPING_STRIKE_BILLOW	  16384
#define LEAPING_STRIKE_FLAME_BAND 20480

/* The caster flashes and sixteen flames ring it (frames 2-15), leaps up at */
/* frame 4 and comes down on the target at frame 16, sliding to it over the */
/* frames after; on landing 256 sparks burst from it. For Planet Diver the */
/* caster then shows a fireball and one target is struck; for Planetary a claw
 */
/* slashes in 3D, three rings and two pillars spread, every target is struck */
/* and at frame 50 all are thrown back. */
void btEffectRunLeapingStrike(BATTLE_EFFECT_PARM *args, s32 mode) {
	SCR_POINT pos;
	MODEL_TEXTURE texture;
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	u8 *graphics;
	MOTION_OBJECT *caster;
	MOTION_OBJECT *target;
	s32 stepX;
	s32 stepY;
	s32 homeX;
	s32 homeZ;
	s32 landed;
	PARTICLE *spark;
	s32 frame;
	s32 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	graphics = Ram_HeapSlots->battleSheet;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
	if(mode == LEAPING_STRIKE_PLANET_DIVER) {
		btResourceLoadAndDecompress(RESOURCE_FLAMEBALL_SHEET,
									state->sheet + LEAPING_STRIKE_PICTURES, 1,
									0);
	}
	else {
		btResourceLoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
									state->sheet + LEAPING_STRIKE_PICTURES, 1,
									0);
		btResourceLoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
									state->sheet + LEAPING_STRIKE_BILLOW, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
									state->sheet + LEAPING_STRIKE_FLAME_BAND, 0,
									0);
	}
	syWordsCopyEntry((void *)BG_PLTT,
					 syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
					 SHEET_PALETTE_BYTES);
	/* The blitter faces the caster's side. */
	if(args->side == EFFECT_SIDE_ENEMIES)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
							BLITTER_MAX);
	else
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	state->transferMode = CANVAS_COPY_AND_FADE;
	if(mode == LEAPING_STRIKE_PLANET_DIVER)
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	else
		state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	for(i = 0; i != LEAPING_STRIKE_SPARK_COUNT; i++)
		LEAPING_STRIKE_SPARKS[i].variant = 0;

	caster = btObjectGetStub(args->user)->obj;
	target = btObjectGetStub(args->tgts[0])->obj;
	stepX = 0;
	stepY = 0;
	homeX = caster->x;
	homeZ = caster->z;
	landed = 0;

	for(frame = 0; frame != LEAPING_STRIKE_FRAMES; frame++) {
		if(frame == 0) {
			SET_POSITION(caster, homeX, 0, homeZ);
			landed = 0;
		}
		if(frame > 17 || frame == 0) {
			btEffectPosProjectBattler(args->user, &pos);
			pos.x /= 2;
		}

		/* Frames 2-15: a flash, then a ring of flames spreading round the
		 * caster. */
		if((u32)(frame - 2) <= 1)
			draw[0](canvas, state->sheet, pos.x - FLASH_BURST_WIDTH / 2,
					pos.y - FLASH_BURST_HEIGHT, FLASH_BURST_WIDTH,
					FLASH_BURST_HEIGHT);
		if((u32)(frame - 4) <= 11) {
			for(i = 0; i != LEAPING_STRIKE_FLAMES; i++) {
				s32 x =
					pos.x +
					(FIXEDtoINT(frame * sySin(i * (TRIG_TURN /
												   LEAPING_STRIKE_FLAMES))));
				s32 y =
					pos.y +
					(FIXEDtoINT(frame * syCos(i * (TRIG_TURN /
												   LEAPING_STRIKE_FLAMES)))) -
					frame;

				draw[0](canvas,
						state->sheet +
							(frame - 4) / 2 * PICTURE_BYTES(FLASH_BURST),
						x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
			}
		}

		/* Frame 4: the leap, away from the caster's own side. */
		if(frame == LEAPING_STRIKE_LEAP_FRAME) {
			caster->autoFaceMotion = 0;
			caster->verticalMotionStrength = toFIXED(3) / 5;
			caster->velY = toFIXED(15);
			if(caster->x < 0)
				caster->velX = -toFIXED(8);
			else
				caster->velX = toFIXED(8);
			if(mode == LEAPING_STRIKE_PLANETARY)
				caster->velX = -caster->velX;
			fdActorSelectAnimationStub(caster, POSE_LEAP);
			state->shakeFrm = 4;
			sySndPlayStub(SE_WHOOSH);
		}

		/* Down at the target's depth, then sliding to it. */
		if(frame == LEAPING_STRIKE_LAND_FRAME) {
			s32 slide = LEAPING_STRIKE_SLIDE_FRAMES;

			caster->verticalMotionStrength = 0;
			/* Planetary's depth of 0 is overwritten at once: both come down at
			 * the */
			/* target's depth. */
			if(mode == LEAPING_STRIKE_PLANET_DIVER)
				caster->z = target->z;
			else
				caster->z = 0;
			caster->z = target->z;
			caster->velX = 0;
			caster->velY = 0;
			caster->velZ = 0;
			stepX = (target->x * 60 / 100 - caster->x) / slide;
			stepY = (target->y - caster->y) / slide;
			fdActorStartIdleStub(caster);
		}

		if(frame > 17) {
			/* Sliding to the target while in the air. */
			if(caster->y > 0) {
				caster->x += stepX;
				caster->y += stepY;
			}
			if(caster->y <= 0) {
				if(!landed) {
					/* The landing: sparks burst from the caster. */
					landed = 1;
					caster->y = 0;
					spark = LEAPING_STRIKE_SPARKS;
					for(i = 0; i != LEAPING_STRIKE_SPARK_COUNT; i++) {
						s32 speed = 1023;
						s32 angle;

						speed &= syRandom();
						angle = syRandom() & TRIG_ANGLE_MASK;

						speed += LEAPING_STRIKE_SPARK_MIN_SPEED;
						spark->x = INTtoFIXED(pos.x);
						spark->y = INTtoFIXED(pos.y - 24);
						spark->velX = (sySin(angle) * speed) >> 6;
						spark->velY = -((syCos(angle) * speed) << 1) >> 6;
						spark->variant = (syRandom() & 7) + 32;
						spark++;
					}
					state->shakeFrm = 4;
					if(mode == LEAPING_STRIKE_PLANET_DIVER) {
						EFFECT_SHOW_DAMAGE(SE_CRASH);
						btMotionApplyKnockbackStub(args->tgts[0],
												   KNOCKBACK_HURL);
						btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
											POSE_DAMAGE, 0, 8);
					}
					else {
						sySndPlayStub(SE_CRASH);
						for(i = 0; i != args->tgtCnt; i++) {
							btMotionApplyKnockbackStub(args->tgts[i],
													   KNOCKBACK_LAUNCH);
							btEffectTintBattler(args->tgts[i],
												SPRITE_COLOR_WHITE, POSE_DAMAGE,
												i, 16);
						}
					}
				}
			}

			if(mode == LEAPING_STRIKE_PLANET_DIVER) {
				/* The fireball over the caster while it is in the air. */
				if(caster->y > 0)
					draw[0](canvas, state->sheet + LEAPING_STRIKE_PICTURES,
							pos.x - 20, pos.y - 52, FLAMEBALL_WIDTH,
							FLAMEBALL_HEIGHT);
			}
			else {
				RENDER_MODEL *model;
				void *projected;
				s32 shade;

				projected = syHeapAllocEwram(1024);
				model = btRenderAllocateModelList(1);
				shade = 0;
				if(frame == LEAPING_STRIKE_WAVE_FRAME)
					state->shakeFrm = 14;

				/* The claw, slashing until frame 45. */
				texture.logWidth = 6;
				texture.logHeight = 6;
				texture.pixels = state->sheet + LEAPING_STRIKE_PICTURES;
				model->mesh = BattleEffect_SquareMesh;
				model->kind = MODEL_SHADED_MAX;
				model->texture = &texture;
				model->vertices = projected;
				model->shade = shade;
				if(frame <= 45) {
					syInitMatrix();
					syTranslate(INTtoFIXED(pos.x - 60), INTtoFIXED(pos.y - 80),
								0);
					syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
					if(args->side == EFFECT_SIDE_ENEMIES) {
						syRotZ(-LEAPING_STRIKE_CLAW_TILT);
					}
					else {
						syRotY(DEGtoANG(180));
						syRotZ(-LEAPING_STRIKE_CLAW_TILT);
					}
					syMatrixScaleUniform(LEAPING_STRIKE_CLAW_SCALE);
					btRenderTransformVertices(BattleEffect_SquareCorners,
											  projected, 4);
					btRenderDrawModelList(model);
				}

				/* Three rings, each from its own frame, spinning as they
				 * spread. */
				texture.logWidth = 6;
				texture.logHeight = 6;
				texture.pixels = state->sheet + LEAPING_STRIKE_BILLOW;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = BattleEffect_RingMesh;
				for(i = 0; i != 3; i++) {
					s32 start =
						LeapingStrike_WaveStarts[i] + LEAPING_STRIKE_WAVE_FRAME;

					if(frame > start) {
						shade = (start - frame) * 8 + 56;

						if(shade > 0)
							shade = 0;
						if(shade > MODEL_SHADE_INVISIBLE) {
							s32 scale;

							start = frame - start;
							scale = start * LeapingStrike_WaveGrowth[i] *
										LEAPING_STRIKE_WAVE_GROWTH +
									FIXED_ONE / 2;

							model->shade = shade;
							syInitMatrix();
							model->scrollV = (frame * 8 + i * 16) & 63;
							syTranslate(
								INTtoFIXED(pos.x - 60),
								INTtoFIXED(LeapingStrike_WaveHeights[i]), 0);
							syScale(scale >> 1, scale << 2, scale / 4);
							syRotX(LEAPING_STRIKE_WAVE_TILT);
							if(i & 1)
								syRotY((i * 4 + frame) * (TRIG_TURN / 32));
							else
								syRotY((i * 4 - frame) * (TRIG_TURN / 32));
							btRenderTransformVertices(BattleEffect_RingCorners,
													  projected, 32);
							btRenderDrawModelList(model);
						}
					}
				}

				/* Two pillars where the caster landed, from their own frames.
				 */
				model->scrollV = 0;
				model->mesh = BattleEffect_CylinderMesh;
				texture.pixels = state->sheet + LEAPING_STRIKE_FLAME_BAND;
				texture.logWidth = 7;
				texture.logHeight = 7;
				for(i = 0; i != 2; i++) {
					s32 start = LeapingStrike_RingStarts[i] +
								LEAPING_STRIKE_PILLAR_FRAME;
					PARTICLE *base = &state->particles[i];

					if(frame == start) {
						base->x = pos.x;
						base->y = pos.y;
					}
					if(frame > start) {
						shade = (start - frame) * 8 + 40;

						if(shade > 0)
							shade = 0;
						if(shade > MODEL_SHADE_INVISIBLE) {
							s32 scale;
							s32 scroll;

							start = frame - start;
							scale = start * LeapingStrike_RingGrowth[i] *
										CYLINDER_RING_GROWTH +
									CYLINDER_RING_START_SCALE;

							model->shade = shade;
							/* Masked in place, the mask assigned first. */
							scroll = 7;
							scroll &= frame + i * 5;
							model->scrollU = scroll << 4;
							syInitMatrix();
							syTranslate(
								INTtoFIXED(base->x - 60),
								INTtoFIXED(LeapingStrike_RingHeights[i]), 0);
							syScale(scale, scale * 2, scale);
							syRotX(LEAPING_STRIKE_PILLAR_TILT);
							btRenderTransformVertices(
								BattleEffect_CylinderCorners, projected, 32);
							btRenderDrawModelList(model);
						}
					}
				}
				syHeapRelease(model);
				syHeapRelease(projected);
			}
		}

		if(mode == LEAPING_STRIKE_PLANETARY && frame == 50) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			for(i = 0; i != args->tgtCnt; i++)
				btEffectStartActorMotion(args->tgts[i], 1, toFIXED(4),
										 toFIXED(16), toFIXED(1), 250);
		}

		/* The sparks fall, bounce off the ground at 112 and shrink. */
		spark = LEAPING_STRIKE_SPARKS;
		for(i = 0; i != LEAPING_STRIKE_SPARK_COUNT; i++) {
			if(spark->variant > 0) {
				spark->variant--;
				btParticleAdvanceWithGravity2D(spark, 56, toFIXED(1) / 8);
				if(spark->y > toFIXED(112)) {
					spark->velY = -spark->velY / 2;
				}
				else if((u32)spark->x < toFIXED(127) && spark->y >= 0) {
					s32 size = spark->variant / 8 + 1;

					draw[0](canvas,
							graphics + BattleEffect_MotePictures[size - 1],
							(FIXEDtoINT(spark->x)) - size / 2,
							(FIXEDtoINT(spark->y)) - size, size, size * 2);
				}
			}
			spark++;
		}

		btCameraShake(12, 12);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Meteor Blow's horizontal-blank callback, the same function as */
/* battle_effect_particle_streams.c's btEffectArmPaletteHBlankDma: stops */
/* DMA 0, then arms it to copy one BG palette word per scanline from the line */
/* table in the effect state, so the backdrop's colour changes down the screen.
 */
void btEffectArmMeteorBlowPaletteDma(void) {
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	volatile u16 *channel = REG_DMA0;

	channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
	channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
	(void)channel[DMA_CONTROL_HALF];
	syDMAXCopy(state->backdropLines, (void *)BG_PLTT,
			   DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
}

/* Fills a grid of columns by rows vertices with two triangles a cell, each */
/* cell cellWidth by cellHeight texels of the texture, and ends the list with */
/* an all-zero triangle. */
void btMeshBuildGrid(MESH_TRIANGLE *triangles, s32 columns, s32 rows,
					 s32 cellWidth, s32 cellHeight) {
	s32 row;
	s32 column;
	MESH_TRIANGLE *second;

	for(row = 0; row != rows; row++) {
		for(column = 0; column != columns; column++) {
			if(row < rows - 1) {
				MESH_TRIANGLE *triangle =
					(MESH_TRIANGLE *)((row * columns + column) *
										  (2 * sizeof(MESH_TRIANGLE)) +
									  (u32)triangles);
				s32 left = column * cellWidth;
				s32 right = left + cellWidth - 1;
				s32 top = row * cellHeight;
				s32 bottom = top + cellHeight - 1;

				triangle[0].uv[0][0] = right;
				triangle[0].uv[0][1] = top;
				triangle[0].uv[1][0] = left;
				triangle[0].uv[1][1] = bottom;
				triangle[0].uv[2][0] = right;
				triangle[0].uv[2][1] = bottom;
				second = triangle + 1;
				second->uv[0][0] = left;
				second->uv[0][1] = top;
				second->uv[1][0] = left;
				second->uv[1][1] = bottom;
				second->uv[2][0] = right;
				second->uv[2][1] = top;
				triangle->vertex[0] = (column + 1) % columns + row * columns;
				triangle->vertex[1] =
					column % columns + columns + row * columns;
				triangle->vertex[2] =
					(column + 1) % columns + columns + row * columns;
				second->vertex[0] = column % columns + row * columns;
				second->vertex[1] = column % columns + columns + row * columns;
				second->vertex[2] = (column + 1) % columns + row * columns;
			}
		}
	}
	triangles[(rows - 1) * columns * 2].vertex[0] = 0;
	triangles[(rows - 1) * columns * 2].vertex[1] = 0;
	triangles[(rows - 1) * columns * 2].vertex[2] = 0;
}
#undef CYLINDER_RING_START_SCALE
#undef CYLINDER_RING_GROWTH
#undef LEAPING_STRIKE_FRAMES
#undef LEAPING_STRIKE_LEAP_FRAME
#undef LEAPING_STRIKE_LAND_FRAME
#undef LEAPING_STRIKE_SLIDE_FRAMES
#undef LEAPING_STRIKE_FLAMES
#undef LEAPING_STRIKE_SPARKS
#undef LEAPING_STRIKE_SPARK_COUNT
#undef LEAPING_STRIKE_SPARK_MIN_SPEED
#undef LEAPING_STRIKE_CLAW_TILT
#undef LEAPING_STRIKE_CLAW_SCALE
#undef LEAPING_STRIKE_WAVE_FRAME
#undef LEAPING_STRIKE_WAVE_GROWTH
#undef LEAPING_STRIKE_WAVE_TILT
#undef LEAPING_STRIKE_PILLAR_FRAME
#undef LEAPING_STRIKE_PILLAR_TILT
#undef LEAPING_STRIKE_PICTURES
#undef LEAPING_STRIKE_BILLOW
#undef LEAPING_STRIKE_FLAME_BAND
