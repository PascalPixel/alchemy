/* The body of Heavy Divide and Strong Hit, then Cool, Wyrd Curse, Radiant Fire
   and Hurricane, and the handlers that play Earth Force's and Thunder Mine's
   bolt trail. Part of battle_effect.c. */

/* GreatSwordSheet's sword, point up, and the HEAVY_DIVIDE_AFTER_IMAGES copies
   the effect makes after it, each a silhouette in one colour of its own, BG
   colours HEAVY_DIVIDE_FIRST_AFTER_IMAGE_COLOR on. */
#define HEAVY_DIVIDE_SWORD_WIDTH			 32
#define HEAVY_DIVIDE_SWORD_HEIGHT			 128
#define HEAVY_DIVIDE_AFTER_IMAGES			 4
#define HEAVY_DIVIDE_FIRST_AFTER_IMAGE_COLOR 60
#define HEAVY_DIVIDE_FRAMES					 80
#define STRONG_HIT_FRAMES					 64
/* The sword stands over the user until HEAVY_DIVIDE_SWING_FRAME, swings down to
   strike at HEAVY_DIVIDE_STRIKE_FRAME (HeavyBlow_StrikeFrames' first row), then
   rebounds up to HEAVY_DIVIDE_REBOUND, a sine's toFIXED(1) being a whole
   turn. */
#define HEAVY_DIVIDE_SWING_FRAME			 16
#define HEAVY_DIVIDE_STRIKE_FRAME			 38
#define HEAVY_DIVIDE_REBOUND				 (TRIG_TURN / 16)
/* The axe takes STRONG_HIT_FLIGHT_FRAMES to cross to the target, turning a
   sixteenth of a turn each frame. */
#define STRONG_HIT_FLIGHT_FRAMES			 32

/* The chips set out in the map cell buffer; HeavyBlow_ChipCounts says how many
   fly. Each flies out its own way at HEAVY_BLOW_CHIP_SPEED_MIN 64ths of a pixel
   a frame and up to 255 more, and bounces when it falls below
   HEAVY_BLOW_GROUND_Y. */
#define HEAVY_BLOW_CHIPS		  256
#define HEAVY_BLOW_CHIP_SPEED_MIN 128
#define HEAVY_BLOW_GROUND_Y		  104

/* The body of Heavy Divide and Strong Hit. Heavy Divide: a great sword rises
   over the user and swings down onto the target, four blue after-images
   trailing it, and rebounds. Strong Hit: an axe spins across from the user to
   the target. Either way chips fly from the target and bounce. */
void btEffectRunHeavyBlow(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	Sint32		   frame;
	void		  *caster;
	Sint32		   frames;
	SCREEN_POINT   hit;
	SCREEN_POINT   source;
	SCREEN_POINT   target;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	void		  *vertices;
	Sint32		   i;
	Sint32		   k;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = BLEND_ALPHA(16, 4);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	blit[0] = Ram_HeapSlots->ramCode;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	caster = btObjectGetStub(args->user)->obj;
	if(mode == HEAVY_BLOW_HEAVY_DIVIDE) {
		frames = HEAVY_DIVIDE_FRAMES;
		btResourceLoadAndDecompress(
			RESOURCE_GREAT_SWORD_SHEET, state->sheet, 1, 1);

		/* The after-images, each a darker blue, four levels apart down to
		   half. */
		for(k = 1; k != HEAVY_DIVIDE_AFTER_IMAGES + 1; k++) {
			BG_PLTT[HEAVY_DIVIDE_FIRST_AFTER_IMAGE_COLOR - 1 + k] = RGB(
				0, 0, (HEAVY_DIVIDE_AFTER_IMAGES - k) * 4 + RGB_LEVEL_MAX / 2);
			for(i = 0; i != PICTURE_BYTES(HEAVY_DIVIDE_SWORD); i++) {
				if(state->sheet[i] == 0) {
					state->sheet[k * PICTURE_BYTES(HEAVY_DIVIDE_SWORD) + i] = 0;
				} else {
					state->sheet[k * PICTURE_BYTES(HEAVY_DIVIDE_SWORD) + i] =
						HEAVY_DIVIDE_FIRST_AFTER_IMAGE_COLOR - 1 + k;
				}
			}
		}
	} else {
		btResourceLoadAndDecompress(RESOURCE_AXE_SHEET, state->sheet, 1, 1);
		frames = STRONG_HIT_FRAMES;
	}
	sySndPlayStub(SE_POWER_UP);

	for(frame = 0; frame != frames; frame++) {
		Sint32 shade = 0;

		if(frame == 0) {
			btEffectPosProjectBattler(args->tgts[0], &hit);
			for(i = 0; i != HEAVY_BLOW_CHIPS; i++) {
				PARTICLE *chip = &PARTICLES[i];
				Sint32	  angle = syRandom();
				Sint32	  speed = 255;

				speed &= syRandom();
				chip->x = toFIXED(hit.x / 2);
				chip->y = INTtoFIXED(hit.y - 16);
				chip->velX =
					(sySin(angle) * (speed + HEAVY_BLOW_CHIP_SPEED_MIN)) >> 6;
				chip->velY =
					(syCos(angle) * (speed + HEAVY_BLOW_CHIP_SPEED_MIN)) >> 6;
				chip->variant = (syRandom() & 15) + 16;
			}
		}
		/* The user takes its leap pose; with the axe it also moves twice as
		   fast. */
		if(mode == HEAVY_BLOW_HEAVY_DIVIDE) {
			if(frame == 8) {
				btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
			}
		} else if(frame == 0) {
			btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
			fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 2);
		}

		vertices = syHeapAllocEWRAM(32);
		model = btRenderAllocateModelList(1);
		btEffectPosProjectBattler(args->user, &source);
		btEffectPosProjectBattler(args->tgts[0], &target);
		model->kind = MODEL_SHADED;
		model->texture = &texture;
		model->vertices = vertices;
		model->shade = shade;
		if(mode == HEAVY_BLOW_HEAVY_DIVIDE) {
			Sint32 size;
			Sint32 angle;

			texture.logWidth = 5;
			texture.logHeight = 7;
			model->mesh = HeavyBlow_SwordMesh;
			/* The sword grows in over the first frames and shrinks away from
			   frame 60. */
			if(frame > 59) {
				size = 68 - frame;
			} else {
				size = frame + 2;
			}
			if(size > 8) {
				size = 8;
			}
			if(size > 0) {
				/* It swings down a little over a quarter turn by the strike,
				   then rebounds. */
				if(frame > HEAVY_DIVIDE_STRIKE_FRAME) {
					angle = DEGtoANG(90) -
						sySin((frame - (HEAVY_DIVIDE_STRIKE_FRAME + 1)) *
							(TRIG_TURN / 128)) /
							(toFIXED(1) / HEAVY_DIVIDE_REBOUND);
				} else if(frame >= HEAVY_DIVIDE_SWING_FRAME) {
					angle = (TRIG_TURN -
								syCos((frame - HEAVY_DIVIDE_SWING_FRAME) *
									(TRIG_TURN / 128))) /
						2;
				} else {
					angle = 0;
				}

				/* After-image i keeps the sword's angle of i frames before; it
				   shows from frame 2i until i frames after the strike. */
				for(i = HEAVY_DIVIDE_AFTER_IMAGES; i != 0; i--) {
					state->particles[i].variant =
						state->particles[i - 1].variant;
				}
				state->particles[0].variant = angle;
				for(i = HEAVY_DIVIDE_AFTER_IMAGES; i != -1; i--) {
					PARTICLE *step = &state->particles[i];

					if(i != 0 && frame > i + HEAVY_DIVIDE_STRIKE_FRAME) {
						continue;
					}
					if(frame < i * 2) {
						continue;
					}
					texture.pixels =
						state->sheet + i * PICTURE_BYTES(HEAVY_DIVIDE_SWORD);
					syInitMatrix();
					syTranslate(INTtoFIXED(source.x / 2 - 64),
						INTtoFIXED(source.y - 88), 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					if(args->side == EFFECT_SIDE_ENEMIES) {
						syRotZ(DEGtoANG(180) - step->variant);
					} else {
						syRotZ(step->variant + DEGtoANG(180));
					}
					syTranslate(0, -toFIXED(16), 0);
					syScale(size * (toFIXED(1) / 2),
						size * (toFIXED(1649) / 8192), size * (toFIXED(1) / 8));
					if(args->side == EFFECT_SIDE_ENEMIES) {
						syRotY(DEGtoANG(180));
					}
					btRenderTransformVertices(
						BattleEffect_BeamCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
			}
		} else {
			model->mesh = BattleEffect_SquareMesh;
			texture.logWidth = 6;
			texture.logHeight = 6;
			/* DEAD: particles 1 to 4, which nothing reads; the ROM keeps the
			   loop over five. */
			if(frame == 0) {
				for(i = 0; i != 5; i++) {
					state->particles[i].x = toFIXED(source.x / 2);
					state->particles[i].y = INTtoFIXED(source.y - 32);
					state->particles[i].velX = (target.x / 2 - source.x / 2) *
						(toFIXED(1) / STRONG_HIT_FLIGHT_FRAMES);
					state->particles[i].velY = (target.y - source.y) *
						(toFIXED(1) / STRONG_HIT_FLIGHT_FRAMES);
					state->particles[i].z = 0;
					if(args->side == EFFECT_SIDE_ENEMIES) {
						state->particles[i].velZ = -(TRIG_TURN / 16);
					} else {
						state->particles[i].velZ = TRIG_TURN / 16;
					}
				}
			}
			/* The axe flies from frame 8, to reach the target at the strike. */
			if(frame > 7) {
				FIXED scale = toFIXED(625) / 512;

				texture.pixels = state->sheet;
				syInitMatrix();
				syTranslate(state->particles[0].x - toFIXED(64),
					state->particles[0].y - toFIXED(64), 0);
				syScale(scale >> 1, scale, toFIXED(1) / 2);
				syRotZ(state->particles[0].z);
				if(args->side == EFFECT_SIDE_ENEMIES) {
					syRotY(DEGtoANG(180));
				}
				btRenderTransformVertices(
					BattleEffect_SquareCorners, vertices, 4);
				btRenderDrawModelList(model);
				btParticleAdvanceWithGravity3D(
					state->particles, PARTICLE_DAMPING_NONE, 0);
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);

		if(frame == HeavyBlow_StrikeFrames[mode]) {
			EFFECT_SHOW_DAMAGE(SE_CLATTER);
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			state->shakeFrm = 8;
		}
		/* The chips fly from the strike on, each drawn and moved three times a
		   frame; a bounce costs a chip eight frames of its life. */
		if(frame >= HeavyBlow_StrikeFrames[mode]) {
			for(i = 0; i != HeavyBlow_ChipCounts[mode]; i++) {
				PARTICLE *chip = &PARTICLES[i];

				if(chip->variant >= 0) {
					blit[0](canvas, HeavyBlow_ChipPixels,
						FIXED_INTEGER_PART(chip->x),
						FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
					btParticleAdvanceWithGravity2D(chip, 62, toFIXED(1) / 16);
					blit[0](canvas, HeavyBlow_ChipPixels,
						FIXED_INTEGER_PART(chip->x),
						FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
					btParticleAdvanceWithGravity2D(chip, 62, toFIXED(1) / 16);
					blit[0](canvas, HeavyBlow_ChipPixels,
						FIXED_INTEGER_PART(chip->x),
						FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
					btParticleAdvanceWithGravity2D(chip, 62, toFIXED(1) / 16);
					if(FIXED_INTEGER_PART(chip->y) > HEAVY_BLOW_GROUND_Y) {
						chip->velY = -chip->velY;
						chip->variant -= 8;
					}
					chip->variant--;
				}
			}
		}
		if(mode == HEAVY_BLOW_HEAVY_DIVIDE) {
			btCameraShake(4, 16);
		} else {
			btCameraShake(8, 8);
		}
		EFFECT_END_FRAME(state);
	}
	fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* CrestSheet's two spikes of ice, leaning left: a broad one, then a slim
   one. */
#define COOL_SPIKE_WIDTH	   36
#define COOL_SPIKE_HEIGHT	   60
#define COOL_SLIM_SPIKE_WIDTH  17
#define COOL_SLIM_SPIKE_HEIGHT 35
#define COOL_SLIM_SPIKE		   PICTURE_BYTES(COOL_SPIKE)
/* Where BlastSheet's pictures, the puffs of frost, are decoded. */
#define COOL_PUFF_PICTURES	   12384
/* The puffs set out at frame 0, as many as Megacool draws. A puff starts on its
   third picture, moves on every three frames and holds the last
   (COOL_PUFF_LAST_PICTURE) until its life passes COOL_PUFF_END. */
#define COOL_PUFFS			   16
#define COOL_PUFF_START		   8
#define COOL_PUFF_END		   28
#define COOL_PUFF_LAST_PICTURE 6
/* Cool's frames; each level adds four. */
#define COOL_FRAMES			   54

/* How a spike leans as it grows (Cool_ColumnLeans). */
enum { LEAN_LEFT, LEAN_RIGHT, SLIM_LEAN_LEFT, SLIM_LEAN_RIGHT };

/* Spikes of ice burst up one after another among the targets, leaning as they
   grow, each striking a target, and sink back; puffs of frost rise and drift
   away.

   Abilities: Cool, Supercool, Megacool. */
void btEffectRunCool(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 stagger;
	Sint32				 sinkFrame;
	DRAW_RECTANGLE		 draw[2];
	SCREEN_POINT		 ends[2];
	Sint32				 frame;
	Sint32				 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_CREST_SHEET, state->sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_BLAST_SHEET, state->sheet + COOL_PUFF_PICTURES, 1, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
		SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	stagger = args->variant * 4;
	sinkFrame = stagger + 32;

	/* The canvas is centred between the first and the last target. */
	btEffectPosProjectBattler(args->tgts[0], &ends[0]);
	btEffectPosProjectBattler(args->tgts[args->tgtCnt - 1], &ends[1]);
	ends[0].x += (ends[1].x - ends[0].x) / 2;
	REG_BG2X = (CANVAS_SIZE / 2 - ends[0].x) << 8;

	frame = 0;
	if(stagger != -COOL_FRAMES) {
		do {
			if(frame == 0) {
				PARTICLE *puff;

				for(i = 0, puff = state->particles; i != COOL_PUFFS;
					i++, puff++) {
					Sint32 speed = (syRandom() & 511) + 128;
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

					puff->x = INTtoFIXED((syRandom() & 63) + 32);
					puff->y = toFIXED(96);
					puff->velX = (sySin(angle) * speed) >> 8;
					puff->velY = (syCos(angle) * speed) >> 8;
					puff->variant = COOL_PUFF_START;
				}
			}
			if(frame == 32)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

			/* DEAD: frame is never negative; the ROM keeps the test. */
			if(frame >= 0) {
				Uint32 mode = BLITTER_MAX;

				/* Supercool's and Megacool's spikes add their light to the
				   first 24 frames' drawing. */
				if(frame <= 23 && args->variant > 0)
					mode = BLITTER_ADDITIVE;
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
				draw[0] = Ram_HeapSlots->ramCode;
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
				draw[1] = Ram_HeapSlots->ramCodePaired;

				for(i = 0; i != args->variant * 3 + 4; i++) {
					Sint32 age = frame - Cool_ColumnStarts[i];
					Sint32 x = Cool_ColumnX[i];
					Sint32 scale;
					Sint32 height;

					/* Two frames after it starts, a spike strikes the next
					   target. */
					if(age == 2) {
						Sint32 member = i % args->tgtCnt;

						EFFECT_STRIKE_TARGET(args, member, 3, KNOCKBACK_NUDGE);
						sySndPlayStub(SE_POWER_UP);
						state->shakeFrm = 3;
					}

					/* A spike rises 10 pixels a frame to its height, in 64ths
					   of its picture's, and sinks as fast. */
					scale = COOL_SPIKE_HEIGHT;
					if(Cool_ColumnLeans[i] > LEAN_RIGHT)
						scale = COOL_SLIM_SPIKE_HEIGHT;
					scale = Cool_ColumnHeights[i] * scale / 64;
					if(frame < sinkFrame)
						height = age * 10;
					else
						height = scale - (frame - sinkFrame) * 10;
					if(height > scale)
						height = scale;

					switch(Cool_ColumnLeans[i]) {
					case LEAN_LEFT:
						x -= height / 2;
						break;
					case LEAN_RIGHT:
						x += height / 2;
						break;
					case SLIM_LEAN_LEFT:
						x -= height / 4;
						break;
					case SLIM_LEAN_RIGHT:
						x += height / 4;
						break;
					}
					if(Cool_ColumnLeans[i] <= LEAN_RIGHT)
						draw[Cool_ColumnLeans[i] & 1](canvas, state->sheet,
							x - COOL_SPIKE_WIDTH / 2, 112 - height,
							COOL_SPIKE_WIDTH, height);
					else
						draw[Cool_ColumnLeans[i] & 1](canvas,
							state->sheet + COOL_SLIM_SPIKE, x - 9, 108 - height,
							COOL_SLIM_SPIKE_WIDTH, height);
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}

			/* Puff i rises from frame 8 + 2i. */
			btEffectBuildCanvasBlitters(args->side, draw);
			for(i = 0; i != args->variant * 4 + 8; i++) {
				PARTICLE *puff = &state->particles[i];

				/* DEAD: the first test is never false, since no level draws
				   more than COOL_PUFFS; the ROM keeps it and the wait for the
				   spikes to sink. */
				if((i <= COOL_PUFFS - 1 || frame >= sinkFrame) &&
					frame >= i * 2 + 8 && puff->variant <= COOL_PUFF_END) {
					Sint32 cell = puff->variant / 3;
					Sint32 x = FIXED_INTEGER_PART(puff->x);
					Sint32 y = FIXED_INTEGER_PART(puff->y);
					Uint32 size;
					Uint8 *src;

					if(cell > COOL_PUFF_LAST_PICTURE)
						cell = COOL_PUFF_LAST_PICTURE;
					src = state->sheet + Cool_SparkPictures[cell] +
						COOL_PUFF_PICTURES;
					size = Cool_SparkSizes[cell];
					draw[0](
						canvas, src, x - size / 2, y - size / 2, size, size);
					puff->variant++;
					btParticleAdvanceWithGravity2D(puff, 62, -toFIXED(1) / 8);
				}
			}
			EFFECT_RELEASE_BLITTERS();

			btCameraShake(4, 4);
			EFFECT_END_FRAME(state);
		} while(++frame != stagger + COOL_FRAMES);
	}
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#define WYRD_CURSE_FRAMES 120

/* The drops set out at frame 0, of which the first WYRD_CURSE_SKULL_COUNT fall,
   one every eight frames. A landed drop feeds the glow a share each frame,
   halving it: the first drop's share starts at WYRD_CURSE_FIRST_DROP_SHARE,
   each later one's WYRD_CURSE_DROP_SHARE_STEP less. */
#define WYRD_CURSE_DROPS			16
#define WYRD_CURSE_SKULL_COUNT		6
#define WYRD_CURSE_FIRST_DROP_SHARE 160
#define WYRD_CURSE_DROP_SHARE_STEP	16
/* The drops land, and the glow lies, on WYRD_CURSE_GROUND_Y. */
#define WYRD_CURSE_GROUND_Y			112
/* At WYRD_CURSE_RISE_FRAME the drops stop, the glow starts to fade and the
   skulls start to rise, one every four frames; each strikes the target 16
   frames after it rises. */
#define WYRD_CURSE_RISE_FRAME		64

/* The glow, WYRD_CURSE_GLOW_SIZE square, grows an eighth of its full size for
   each WYRD_CURSE_LEVEL_PER_EIGHTH of its level, and loses WYRD_CURSE_GLOW_FADE
   a frame as the skulls rise. */
#define WYRD_CURSE_GLOW_SIZE		32
#define WYRD_CURSE_LEVEL_PER_EIGHTH 824
#define WYRD_CURSE_GLOW_FADE		24

/* Where the glow, mirrored from its shaded disc, and BurningSkullSheet's two
   skulls are kept in the effect's pictures. */
#define WYRD_CURSE_GLOW			  4096
#define WYRD_CURSE_SKULL_PICTURES 16384
#define BURNING_SKULL_WIDTH		  32
#define BURNING_SKULL_HEIGHT	  32

/* Drops of red fall one by one at the target's feet and gather in a red glow
   there; then a burning skull rises from where each fell, drifting aside, and
   the target is struck six times.

   Abilities: Wyrd Curse. */
void btEffectRunWyrdCurse(BATTLE_EFFECT_PARM *args)
{
	MODEL_TEXTURE texture;
	SCREEN_POINT  screen;
	void		 *canvas;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE		 draw[2];
	Sint32				 frame;
	Uint8				*sprites;
	void				*caster;
	BATTLE_EFFECT_STATE *state;
	Sint32				 i;
	PARTICLE			*drop;
	PARTICLE			*last;
	Uint16				*palette;
	Sint32				 size;
	Sint32				 width = WYRD_CURSE_GLOW_SIZE;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = 0;
	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	}
	draw[0] = Ram_HeapSlots->ramCode;
	/* DEAD: set again before the transfer starts; the ROM keeps the two
	   stores. */
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	palette = (Uint16 *)&BG_PLTT_COLOR(0, 1);
	caster = btObjectGetStub(args->user)->obj;

	/* The canvas's levels are reds, from 8 to 28. */
	syWordsClearEntry((void *)BG_PLTT, SHEET_PALETTE_BYTES);
	for(i = 0; i != CANVAS_LEVEL_BRIGHTEST; i++) {
		*palette++ = RGB(i / 3 + 8, 0, 0);
	}

	/* The glow: the quarter of a shaded disc, each pixel doubled across and
	   mirrored into the four quarters of the glow's square. The clear covers
	   twice the glow's bytes. */
	{
		Sint32 y;
		Sint32 x;
		Sint32 half;
		Sint32 offset;

		size = width / 2;
		half = size / 2;
		syWordsClear(state->sheet + WYRD_CURSE_GLOW,
			WYRD_CURSE_GLOW_SIZE * WYRD_CURSE_GLOW_SIZE * 2);
		btEffectDrawShadedCircles(state->sheet, size, toFIXED(1));
		y = 0;
		offset = 0;
	nextRow:
		for(x = 0; x != half; x++) {
			Uint8 v = state->sheet[offset / 2 + x];

			if(v != 0) {
				Sint32 mirrorRow;
				Sint32 row;

				row = y * WYRD_CURSE_GLOW_SIZE;
				state->sheet[WYRD_CURSE_GLOW + row + x * 2] = v;
				state->sheet[WYRD_CURSE_GLOW + row + x + x + 1] = v;
				state->sheet[WYRD_CURSE_GLOW + (offset + (size - x - 1)) * 2] =
					v;
				state->sheet[WYRD_CURSE_GLOW + (offset + (size - x - 1)) * 2 +
					1] = v;
				mirrorRow = (WYRD_CURSE_GLOW_SIZE - y) * WYRD_CURSE_GLOW_SIZE;
				mirrorRow -= WYRD_CURSE_GLOW_SIZE;
				state->sheet[WYRD_CURSE_GLOW + mirrorRow + x * 2] = v;
				state->sheet[WYRD_CURSE_GLOW + mirrorRow + x * 2 + 1] = v;
				state->sheet[WYRD_CURSE_GLOW + mirrorRow + (size - x - 1) * 2] =
					v;
				state->sheet[WYRD_CURSE_GLOW + mirrorRow + (size - x - 1) * 2 +
					1] = v;
			}
		}
		y++;
		offset += size;
		if(y != size) {
			goto nextRow;
		}
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_BURNING_SKULL_SHEET,
		state->sheet + WYRD_CURSE_SKULL_PICTURES, 0, 0);

	for(frame = 0; frame != WYRD_CURSE_FRAMES; frame++) {
		RENDER_MODEL *model;
		void		 *vertices;
		Sint32		  shade = 0;
		Sint32		  level;

		if(frame == 0) {
			btEffectPosProjectBattler(args->tgts[0], &screen);
			for(i = 0; i != WYRD_CURSE_DROPS; i++) {
				drop = &state->particles[i];
				drop->x = toFIXED(screen.x / 2 + (syRandom() & 15) - 8);
				drop->y = -8;
				drop->velX = 0;
				drop->velY = 0;
				drop->z = (syRandom() & 3) + 4;
				drop->velZ = WYRD_CURSE_FIRST_DROP_SHARE -
					i * WYRD_CURSE_DROP_SHARE_STEP;
				drop->variant = 0;
			}
			{
				PARTICLE *glow =
					&state->particles[ARRAY_COUNT(state->particles) - 1];

				glow->x = toFIXED(screen.x / 2);
				glow->y = toFIXED(WYRD_CURSE_GROUND_Y);
				glow->variant = 0;
			}
		}

		/* The drops fall, each a mote its size, and stop where they land; a
		   landed drop keeps the speed its skull will drift aside with. */
		if(frame < WYRD_CURSE_RISE_FRAME) {
			for(i = 0, drop = state->particles; i != WYRD_CURSE_SKULL_COUNT;
				i++, drop++) {
				if(frame > i * 8 && drop->variant == 0) {
					Sint32 dropSize = drop->z;

					EFFECT_DRAW_MOTE(draw[0], canvas, sprites,
						FIXED_INTEGER_PART(drop->x),
						FIXED_INTEGER_PART(drop->y), dropSize);
					btParticleAdvanceWithGravity2D(
						drop, PARTICLE_DAMPING_NONE, toFIXED(1) / 2);
					if(FIXED_INTEGER_PART(drop->y) >= WYRD_CURSE_GROUND_Y) {
						drop->variant = 1;
						/* DEAD: the store, overwritten by the next; the random
						   draw still moves the sequence; the ROM keeps the
						   call, its arithmetic and the store. */
						drop->velX =
							((syRandom() & 31) - 16) * (toFIXED(1) / 16);
						drop->velX =
							WyrdCurse_DropBounces[i % 4] * (toFIXED(1) / 8);
						drop->velY = 0;
					}
				}
			}
		}

		vertices = syHeapAllocEWRAM(32);
		model = btRenderAllocateModelList(1);
		texture.logWidth = 5;
		texture.logHeight = 5;
		texture.pixels = state->sheet + WYRD_CURSE_GLOW;
		model->kind = MODEL_SHADED_MAX;
		model->mesh = BattleEffect_SmallSquareMeshReversed;
		model->texture = &texture;
		model->vertices = vertices;
		model->shade = shade;

		/* The glow gathers what the landed drops bring, of the first eight;
		   only the WYRD_CURSE_SKULL_COUNT that fall ever land. */
		{
			PARTICLE *glow;
			PARTICLE *landed;

			i = 0;
			glow = &state->particles[ARRAY_COUNT(state->particles) - 1];
			landed = state->particles;
			for(; i != 8; i++, landed++) {
				if(landed->variant > 0) {
					glow->variant += landed->velZ;
					landed->velZ /= 2;
				}
			}
		}
		last = &state->particles[ARRAY_COUNT(state->particles) - 1];
		level = last->variant;
		if(level > 0) {
			Sint32 scale =
				level * (toFIXED(1) / 8) / WYRD_CURSE_LEVEL_PER_EIGHTH;

			if(frame >= WYRD_CURSE_RISE_FRAME) {
				last->variant = level - WYRD_CURSE_GLOW_FADE;
			}
			syInitMatrix();
			syTranslate(last->x - toFIXED(64), last->y - toFIXED(64), 0);
			syScale(scale * 2, scale, scale * 2);
			syRotX(-DEGtoANG(45));
			btRenderTransformVertices(
				BattleEffect_GroundSquareCorners, vertices, 4);
			btRenderDrawModelList(model);
		}

		/* The skulls rise, the two pictures in turn. */
		texture.logWidth = 5;
		texture.logHeight = 5;
		model->kind = MODEL_SHADED_ADDITIVE;
		model->mesh = BattleEffect_SmallSquareMeshReversed;
		for(i = 0; i != WYRD_CURSE_SKULL_COUNT; i++) {
			drop = &state->particles[i];
			if(frame > i * 4 + WYRD_CURSE_RISE_FRAME) {
				FIXED skullScale = toFIXED(3125) / 8192;

				model->shade = shade;
				texture.pixels = state->sheet + WYRD_CURSE_SKULL_PICTURES +
					(i % 2) * PICTURE_BYTES(BURNING_SKULL);
				syInitMatrix();
				syTranslate(drop->x - toFIXED(64), drop->y - toFIXED(48), 0);
				syScale(skullScale >> 1, skullScale, skullScale >> 1);
				syRotX(-DEGtoANG(90));
				btRenderTransformVertices(
					BattleEffect_GroundSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
				btParticleAdvanceWithGravity2D(drop, 60, -toFIXED(1) / 2);
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);

		if(frame == WYRD_CURSE_RISE_FRAME) {
			sySndPlayStub(SE_BEAST_CRY);
		}
		for(i = 0; i != WYRD_CURSE_SKULL_COUNT; i++) {
			if(frame == i * 4 + WYRD_CURSE_RISE_FRAME + 16) {
				state->shakeFrm = 2;
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 2);
				if(i == 0) {
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				} else {
					sySndPlayStub(SE_IMPACT);
				}
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}
	fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#define RADIANT_FIRE_FRAMES 66

/* The shaded discs drawn into the map cell buffer, 2 to twice
   RADIANT_FIRE_DISCS across, smallest first; their offsets are kept in the
   state's sheet. */
#define RADIANT_FIRE_DISCS		  32
/* The flames placed about the target at frame 0, of which the first
   RADIANT_FIRE_BURSTS burn, one every four frames. A burst's disc swells and
   shrinks over RADIANT_FIRE_BURST_FRAMES, to a radius of
   RADIANT_FIRE_BURST_RADIUS for the first and a pixel less for each after it;
   its ring of fire grows for RADIANT_FIRE_RING_FRAMES, fading from
   RADIANT_FIRE_RING_FADE. */
#define RADIANT_FIRE_FLAMES		  32
#define RADIANT_FIRE_BURSTS		  8
#define RADIANT_FIRE_BURST_FRAMES 32
#define RADIANT_FIRE_BURST_RADIUS 32
#define RADIANT_FIRE_RING_FRAMES  24
#define RADIANT_FIRE_RING_FADE	  16

/* Where FireRingTexture is decoded in the effect's pictures. */
#define RADIANT_FIRE_FIRE_RING 4096

/* Eight bursts of fire bloom one after another about the target, each a glowing
   disc that swells and shrinks under a ring of fire that grows, tumbling, and
   fades. The canvas flashes white as the first comes, and each burst strikes
   the target.

   Abilities: Radiant Fire. */
void btEffectRunRadiantFire(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screen;
	MODEL_TEXTURE		 texture;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	BATTLE_CAMERA		*camera;
	Sint32				 savedYaw;
	MOTION_OBJECT		*target;
	Sint16				*discs;
	PARTICLE			*flame;
	PARTICLE			*burst;
	PARTICLE			*ring;
	RENDER_MODEL		*model;
	void				*vertices;
	Sint32				 offset;
	Sint32				 size;
	Sint32				 cells;
	Sint32				 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
		state->sheet + RADIANT_FIRE_FIRE_RING, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
		SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	camera = Ram_HeapSlots->cameraState;
	offset = 0;
	savedYaw = camera->yaw;
	size = 2;
	target = btObjectGetStub(args->tgts[0])->obj;

	/* The discs are drawn a little over full strength, so that the middle of
	   each burns at the brightest level. */
	cells = (Sint32)Ram_MapMetatileBuffer;
	discs = (Sint16 *)state->sheet;
	for(i = 0; i != RADIANT_FIRE_DISCS; i++) {
		btEffectDrawShadedCircles(
			(void *)(offset + cells), size, toFIXED(2673) / 2048);
		discs[i] = offset;
		offset += (i * 2 + 3) * size / 2;
		size += 2;
	}

	for(frame = 0; frame != RADIANT_FIRE_FRAMES; frame++) {
		/* Each flame takes a random place about a point 48 pixels over the
		   target, further out the later it comes, and keeps where that is on
		   the screen. */
		if(frame == 0) {
			camera->yaw = savedYaw;
			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);
			flame = state->particles;
			for(i = 0; i != RADIANT_FIRE_FLAMES; i++, flame++) {
				Sint32 angle;
				Sint32 radius;

				syRandom();
				angle = syRandom() & TRIG_ANGLE_MASK;
				radius = (syRandom() & 7) + i * 4;
				SET_POSITION(flame, target->x + sySin(angle) * radius * 2,
					target->y + syCos(angle) * radius + toFIXED(48), target->z);
				btEffectPosProjectPoint(&flame->x, &screen);
				flame->x = screen.x / 2;
				flame->y = screen.y;
				syRandom();
				flame->velX = RADIANT_FIRE_BURST_RADIUS - i;
				flame->velY = syRandom();
				flame->velZ = syRandom();
				flame->variant = -i * 4;
			}
		}
		if(frame == 0)
			btEffectSetApproachMotion(args->user, args->tgts[0], 8, 80);

		if(frame == 8) {
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			state->shakeFrm = 16;
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			EFFECT_SHOW_DAMAGE(SE_CRASH);
		}

		if(frame >= 8) {
			i = 0;
			burst = state->particles;
			for(; i != RADIANT_FIRE_BURSTS; i++, burst++) {
				burst->variant++;
				if(burst->variant == 8) {
					sySndPlayStub(SE_CLATTER);
					state->shakeFrm = 8;
					btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
					btEffectTintBattler(
						args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				}
				if(burst->variant >= 0 &&
					burst->variant < RADIANT_FIRE_BURST_FRAMES) {
					Sint32 diameter;
					Sint32 index;

					diameter =
						FIXEDtoINT(
							sySin(burst->variant *
								(DEGtoANG(180) / RADIANT_FIRE_BURST_FRAMES)) *
							burst->velX) *
						2;
					/* Drawn 2 to 62 across: at its widest the first burst skips
					   a frame. */
					index = diameter - 2;
					if((Uint32)index < 2 * RADIANT_FIRE_DISCS - 2)
						btCanvasBlitMirroredQuarters(
							(void *)(*(Sint16 *)(index + (Sint32)discs) +
								cells),
							burst->x, burst->y, diameter);
				}
			}

			vertices = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = state->sheet + RADIANT_FIRE_FIRE_RING;
			model->kind = MODEL_SHADED_MAX;
			model->vertices = vertices;
			model->mesh = BattleEffect_LargeSquareMesh;
			model->texture = &texture;
			i = 0;
			ring = state->particles;
			for(; i != RADIANT_FIRE_BURSTS; i++, ring++) {
				if(ring->variant >= 0 &&
					ring->variant < RADIANT_FIRE_RING_FRAMES) {
					Sint32 brightness = 0;
					Sint32 scale = ring->variant * (toFIXED(3) / 32);

					if(ring->variant >= RADIANT_FIRE_RING_FADE)
						brightness =
							(RADIANT_FIRE_RING_FADE - ring->variant) * 8;
					model->shade = brightness;
					syInitMatrix();
					syTranslate(
						INTtoFIXED(ring->x - 64), INTtoFIXED(ring->y - 64), 0);
					syRotZ(ring->velZ);
					syRotX(ring->velY);
					syMatrixScaleUniform(scale);
					btRenderTransformVertices(
						BattleEffect_SquareCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}

		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define HURRICANE_FRAMES 68

/* The streak the effect draws into the state's sprite sheet: one pixel of level
   HURRICANE_STREAK_LEVEL a row, a pixel further over every 16 rows, slanting as
   the streaks fall. The first HURRICANE_STREAKS of the state's particles fall
   as streaks, one more each frame, and a streak whose foot reaches
   HURRICANE_GROUND_Y starts a puff there. */
#define HURRICANE_STREAK_WIDTH	4
#define HURRICANE_STREAK_HEIGHT 64
#define HURRICANE_STREAK_LEVEL	24
#define HURRICANE_STREAKS		32
#define HURRICANE_GROUND_Y		112

/* The puffs, SmokeSheet's, in the map cell buffer; only the first
   HURRICANE_STREAKS are ever started. */
#define HURRICANE_PUFFS			256
#define HURRICANE_PUFF_PICTURES 12800

/* Rain driven by the wind streaks down over the target, and puffs rise where
   each streak lands; the target flashes as it is lashed, and the user closes in
   to strike at frame 46. Then the canvas is cleared and Dull's effect follows.

   Abilities: Hurricane. */
void btEffectRunHurricane(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	SCREEN_POINT   position;
	PARTICLE	  *streak;
	PARTICLE	  *puff;
	Sint32		   size = CANVAS_VIEW_BYTES;
	Sint32		   offsetX;
	Sint32		   frame;
	Sint32		   i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit[0] = Ram_HeapSlots->ramCode;
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_CYAN_PALETTE), SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	btEffectPosProjectBattler(args->tgts[0], &position);
	offsetX = CANVAS_SIZE / 2 - position.x;
	REG_BG2X = offsetX << 8;

	syWordsClearEntry(state->sheet, PICTURE_BYTES(HURRICANE_STREAK));
	for(i = 0; i != HURRICANE_STREAK_HEIGHT; i++) {
		if(args->side == EFFECT_SIDE_ENEMIES)
			state->sheet[i * HURRICANE_STREAK_WIDTH - i / 16 +
				HURRICANE_STREAK_WIDTH - 1] = HURRICANE_STREAK_LEVEL;
		else
			state->sheet[i * HURRICANE_STREAK_WIDTH + i / 16] =
				HURRICANE_STREAK_LEVEL;
	}
	btResourceLoadAndDecompress(
		RESOURCE_SMOKE_SHEET, state->sheet + HURRICANE_PUFF_PICTURES, 1, 0);
	REG_BG2PA = Q_8_8(1);

	for(frame = 0; frame != HURRICANE_FRAMES; frame++) {
		/* The streaks wait above the screen, drifting with the wind. */
		if(frame == 0) {
			sySndPlayStub(SE_AMBIENT_WATER);
			EFFECT_FREE_PARTICLES(i, PARTICLES, HURRICANE_PUFFS);
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				streak = &state->particles[i];
				streak->y = toFIXED((syRandom() & 31) - 64);
				if(args->side == EFFECT_SIDE_ENEMIES) {
					streak->x = INTtoFIXED((syRandom() & 63) + 40);
					streak->velX = -toFIXED(1);
				} else {
					streak->x = INTtoFIXED((syRandom() & 63) + 24);
					streak->velX = toFIXED(1);
				}
				streak->velY = toFIXED(16);
				streak->variant = -1;
			}
		}
		if(frame == 32)
			btMotionApproachTargetStub(
				args->user, args->tgts[0], 8, toFIXED(3));
		if(frame == 46) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
		}

		puff = PARTICLES;
		for(i = 0; i != HURRICANE_PUFFS; i++, puff++) {
			if(puff->variant >= 0 && puff->variant < SMOKE_PUFF_FRAMES) {
				blit[0](canvas,
					state->sheet + HURRICANE_PUFF_PICTURES +
						puff->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
					FIXED_INTEGER_PART(puff->x) - SMOKE_PUFF_WIDTH / 2,
					FIXED_INTEGER_PART(puff->y) - 32, SMOKE_PUFF_WIDTH,
					SMOKE_PUFF_HEIGHT);
				btParticleAdvanceWithGravity2D(puff, 56, -toFIXED(1) / 4);
				puff->variant++;
			}
		}

		/* The target flashes every four frames from frame 6. */
		for(i = 0; i != 8; i++) {
			if(frame == i * 4 + 6) {
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 3);
				sySndPlayStub(SE_WATER);
			}
		}

		/* Streak i falls from frame i, cut short at the ground. */
		for(i = 0; i != HURRICANE_STREAKS; i++) {
			Sint32 y;
			Sint32 height;
			Sint32 x;

			streak = &state->particles[i];
			if(frame < i)
				continue;
			y = FIXED_INTEGER_PART(streak->y);
			if(y >= HURRICANE_GROUND_Y)
				continue;
			height = HURRICANE_STREAK_HEIGHT;
			if(y > HURRICANE_GROUND_Y - HURRICANE_STREAK_HEIGHT)
				height = HURRICANE_GROUND_Y - y;
			blit[0](canvas, state->sheet, FIXED_INTEGER_PART(streak->x), y,
				HURRICANE_STREAK_WIDTH, height);
			x = streak->x + streak->velX;
			streak->x = x;
			streak->y += streak->velY;
			if(FIXEDtoINT(streak->y) >
				HURRICANE_GROUND_Y - HURRICANE_STREAK_HEIGHT) {
				puff = PARTICLES;
				if(puff[i].variant == -1) {
					puff[i].y = toFIXED(HURRICANE_GROUND_Y);
					puff[i].variant = 0;
					puff[i].x = x;
					puff[i].velX = (syRandom() % 12 - 6) << 15;
					puff[i].velY = (-syRandom() & 7) << 10;
				}
			}
		}

		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}

	/* The canvas and its characters are cleared for Dull's effect. */
	syCloseEvent(btEffectTransferCanvas);
	/* FAKEMATCH(dead): an empty once-only loop before the release. */
	do {
	} while(0);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syWordsClearEntry((void *)BG_CHAR_BLOCK(1), size);
	syWordsClearEntry(canvas, size);
	syCloseEvent(btEffectDarkenBackgroundStep);
	syCloseEvent(btEffectTransferCanvas);
	btEffectRunDull(args);
}

/* Abilities: Earth Force. */
void btEffectRunEarthForce(BATTLE_EFFECT_PARM *args)
{
	btEffectRunBallLightning(args, BALL_LIGHTNING_EARTH_FORCE);
}

/* Abilities: Thunder Mine. */
void btEffectRunThunderMine(BATTLE_EFFECT_PARM *args)
{
	btEffectRunBallLightning(args, BALL_LIGHTNING_THUNDER_MINE);
}

#undef BURNING_SKULL_WIDTH
#undef BURNING_SKULL_HEIGHT
