/* Liquifier, and the canvas bursts: a glowing ball that strikes the target in
   flames and sparks. Part of battle_effect.c. */

#define LIQUIFIER_TICKS		  111
/* The state's particles: the sparks, then the beams. */
#define LIQUIFIER_SPARKS	  32
#define LIQUIFIER_FIRST_BEAM  LIQUIFIER_SPARKS
#define LIQUIFIER_BEAMS		  16
/* The tick of the first beam and the first of the eight strikes. */
#define LIQUIFIER_STRIKE_TICK 54
/* The sparks' speeds: a sine times 512 to 1023, shifted down by this as they
   burst out, one less upward, and two more as they gather. */
#define LIQUIFIER_BURST_SHIFT 8

/* Where FlashBurstSheet is decoded: the map cell buffer's second 16 KB, where
   FireRingTexture, decoded to EffectScratch, replaces it. */
#define LIQUIFIER_FLASH_BURST_PICTURES (RamMapMetatileBuffer + 16384)

/* Where GlowBandTextureA is decoded in the effect's pictures. */
#define LIQUIFIER_GLOW_BAND 4096

/* Sparks fall into the user as two rings of fire swell about it (ticks 0 to
   15); then the sparks burst from it and eight flames fan out above it and sink
   back. From tick 54 sixteen beams of light stab down about the target, one
   every two ticks, each narrowing away over a ring of fire that spreads on the
   ground, and the target is struck every four ticks; the eighth strike shows
   the damage.

   Abilities: Liquifier. */
void btEffectRunLiquifier(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 from;
	SCREEN_POINT		 to;
	DRAW_RECTANGLE		 draw[2];
	MODEL_TEXTURE		 texture;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frame;
	Uint8				*graphics;
	Sint32				 i;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	graphics = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);
	/* DEAD: FlashBurstSheet, which FireRingTexture, decoded at the same place
	   below, replaces; the ROM keeps the call. */
	btResourceLoadAndDecompress(
		RESOURCE_FLASH_BURST_SHEET, LIQUIFIER_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FLAME_WALL_TEXTURE, state->sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_BAND_TEXTURE_A, state->sheet + LIQUIFIER_GLOW_BAND, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_RING_TEXTURE, EffectScratch, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	btEffectPosProjectBattler(args->user, &from);
	btEffectPosProjectBattler(args->targets[0], &to);

	for(frame = 0; frame != LIQUIFIER_TICKS; frame++) {
		if(frame == 0) {
			PARTICLE *beam;

			/* DEAD: the map cell buffer's particles marked free, which
			   Liquifier never draws; the ROM keeps the loop. */
			EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT / 2);

			/* The beams stand about the target, each at its own slant, its
			   z. */
			for(i = 0, beam = &state->particles[LIQUIFIER_FIRST_BEAM];
				i != LIQUIFIER_BEAMS; i++, beam++) {
				SET_POSITION(beam, toFIXED(to.x + (syRandom() & 127) - 64),
					toFIXED(to.y), (syRandom() & 8191) + toFIXED(7) / 16);
			}
		}
		if(frame == 0) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frame == 12) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
		}
		if(frame == LIQUIFIER_STRIKE_TICK) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frame == 0)
			sySoundPlayStub(SE_DJINNI_SET);
		if(frame == 16)
			sySoundPlayStub(SE_HEAVY_IMPACT);

		if(frame > 15) {
			/* The burst: sparks fly out of the user, upward and forward. */
			if(frame == 16) {
				PARTICLE *step = state->particles;

				for(i = 0; i != LIQUIFIER_SPARKS; i++) {
					Uint32 angle =
						syRandom() % (DEGtoANG(135) - 1) + TRIG_TURN * 5 / 16;
					Sint32 speed = (syRandom() & 511) + 512;

					step->velocityX =
						(sySin(angle) * speed) >> LIQUIFIER_BURST_SHIFT;
					step->velocityY =
						(syCos(angle) * speed) >> (LIQUIFIER_BURST_SHIFT - 1);
					step->x = toFIXED(from.x / 2);
					step->y = toFIXED(from.y - (syRandom() & 15) - 8);
					step->variant = 0;
					step++;
				}
			}
			/* Two steps a tick, the n-th spark from tick n / 4. */
			for(i = 0; i != LIQUIFIER_SPARKS; i++) {
				/* FAKEMATCH(temporary): one, 1 held in a local; as 1 at its
				 * uses. */
				Sint32	  one = 1;
				PARTICLE *step = &state->particles[i];

				if(step->variant >= 0 && frame >= i / 4) {
					Sint32 odd = i & one;
					Sint32 size = odd + 5;
					Sint32 blend;
					Sint32 twice;
					Sint32 half;
					Sint32 j;

					j = 0;
					/* FAKEMATCH(temporary): blend, odd * 4 held as a byte
					 * offset into draw; as draw[odd]. */
					blend = odd * 4;
					twice = size * 2;
					half = size / 2;
					for(; j != 2; j++) {
						(*(DRAW_RECTANGLE *)((Uint8 *)draw + blend))(canvas,
							graphics + BattleEffectMotePictures[size - 1],
							FIXED_INTEGER_PART(step->x) - half,
							FIXED_INTEGER_PART(step->y) - size, size, twice);
						btParticleAdvanceWithGravity2D(
							step, PARTICLE_DAMPING_NONE, 0);
					}
					step->variant++;
				}
			}
		} else {
			PARTICLE *step;

			/* The gathering: sparks start 64 steps out and fall into the
			   user. */
			if(frame == 0) {
				PARTICLE *step = state->particles;

				for(i = 0; i != LIQUIFIER_SPARKS; i++) {
					Sint32 angle =
						(syRandom() & (DEGtoANG(180) - 1)) - DEGtoANG(90);
					Sint32 speed = (syRandom() & 511) + 512;
					Sint32 y;

					step->velocityX =
						(sySin(angle) * speed) >> (LIQUIFIER_BURST_SHIFT + 2);
					step->velocityY =
						(syCos(angle) * speed) >> (LIQUIFIER_BURST_SHIFT + 1);
					step->x = toFIXED(from.x / 2);
					y = from.y - (syRandom() & 15) - 8;
					step->x -= step->velocityX << 6;
					step->y = toFIXED(y) - (step->velocityY << 6);
					step->variant = 0;
					step++;
				}
			}
			/* Four steps a tick. */
			for(i = 0, step = state->particles; i != LIQUIFIER_SPARKS;
				i++, step++) {
				if(step->variant >= 0) {
					Sint32 size = (i & 1) + 2;
					Sint32 j;

					for(j = 0; j != 4; j++) {
						EFFECT_DRAW_MOTE(draw[i & 1], canvas, graphics,
							FIXED_INTEGER_PART(step->x),
							FIXED_INTEGER_PART(step->y), size);
						btParticleAdvanceWithGravity2D(
							step, PARTICLE_DAMPING_NONE, 0);
					}
					step->variant++;
				}
			}
		}

		{
			void		 *projected = syHeapAllocateEWRAM(32);
			RENDER_MODEL *model = btRenderAllocateModelList(1);
			void		 *fireRing = EffectScratch;

			/* Eight flames fan out above the user, each for 16 ticks from its
			   start, rising and sinking back on a sine. */
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->vertices = projected;
			texture.logWidth = 6;
			texture.logHeight = 6;
			texture.pixels = state->sheet;
			model->mesh = BattleEffectTallMesh;
			for(i = 0; i != 8; i++) {
				Sint32 start = LiquifierSplashStarts[i];

				if(frame >= start && frame < start + 16) {
					Sint32 age;

					model->shade = 0;
					age = frame - start;
					syInitMatrix();
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syTranslate(
						INTtoFIXED(from.x - 128), INTtoFIXED(from.y - 74), 0);
					syRotZ(i * (TRIG_TURN / 16) - TRIG_TURN * 7 / 32);
					syScale(toFIXED(1) / 3,
						sySin(age * (TRIG_TURN / 32)) *
							LiquifierSplashHeights[i] / 64 * 2,
						toFIXED(1) / 2);
					btRenderTransformVertices(
						LiquifierSplashCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}

			/* The beams: each from two ticks after the last, narrowing away
			   over 16 ticks. */
			texture.logWidth = 5;
			texture.logHeight = 2;
			texture.pixels = state->sheet + LIQUIFIER_GLOW_BAND;
			model->mesh = BattleEffectSmallSquareMesh;
			model->kind = MODEL_SHADED_ADDITIVE;
			for(i = 0; i != LIQUIFIER_BEAMS; i++) {
				Sint32	  shade = 0;
				PARTICLE *step = &state->particles[LIQUIFIER_FIRST_BEAM + i];
				Sint32	  start = i * 2 + LIQUIFIER_STRIKE_TICK;

				if(frame >= start && frame < start + 16) {
					Sint32 scale;

					scale = toFIXED(2) - (frame - start) * (toFIXED(1) / 8);
					model->shade = shade;
					syInitMatrix();
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syTranslate(
						step->x - toFIXED(128), step->y - toFIXED(64), 0);
					syRotZ(step->z);
					syScale(scale, toFIXED(1), toFIXED(1) / 2);
					btRenderTransformVertices(
						BattleEffectBeamCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}

			/* Their rings of fire spread on the ground, darkening, faster after
			   eight ticks. */
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = fireRing;
			model->mesh = BattleEffectLargeSquareMesh;
			model->kind = MODEL_SHADED_ADDITIVE;
			for(i = 0; i != LIQUIFIER_BEAMS; i++) {
				PARTICLE *step = &state->particles[i + LIQUIFIER_FIRST_BEAM];
				Sint32	  age = frame - LIQUIFIER_STRIKE_TICK - i * 2;

				if(frame >= i * 2 + LIQUIFIER_STRIKE_TICK &&
					frame < i * 2 + LIQUIFIER_STRIKE_TICK + 16) {
					Sint32 shade = -age * 4;
					Sint32 scale = age * (toFIXED(1) / 4);

					if(age > 7) {
						shade -= age * 8;
						shade += 64;
					}
					model->shade = shade;
					syInitMatrix();
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syTranslate(
						step->x - toFIXED(128), step->y - toFIXED(64), 0);
					syRotX(TRIG_TURN * 15 / 64);
					syScale(scale, scale, scale);
					btRenderTransformVertices(
						BattleEffectSquareCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}

			/* Two rings of fire about the user, from ticks 0 and 4 for 12
			   ticks each, fading in as they close. */
			for(i = 0; i != 2; i++) {
				if(frame >= i * 4 && frame < i * 4 + 12) {
					Sint32 age = frame - i * 4;
					Sint32 shade = age * 8 + MODEL_SHADE_INVISIBLE;
					Sint32 scale = toFIXED(3) / 2 - age * (toFIXED(1) / 8);

					if(shade > 0)
						shade = 0;
					model->shade = shade;
					syInitMatrix();
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syTranslate(
						INTtoFIXED(from.x - 128), INTtoFIXED(from.y - 80), 0);
					syScale(scale * 2, scale * 2, scale);
					syRotZ(TRIG_TURN * 3 / 8 - i * (TRIG_TURN / 4));
					syRotX(-TRIG_TURN / 16);
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}

			/* A strike every four ticks; the last shows the damage. */
			for(i = 0; i != 8; i++) {
				if(frame == i * 4 + LIQUIFIER_STRIKE_TICK) {
					if(i == 7)
						EFFECT_SHOW_DAMAGE(SE_CLATTER);
					else
						sySoundPlayStub(SE_CLATTER);
					btEffectStartActorMotion(args->targets[0], 1, 0, 0, 0, 100);
					btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE,
						POSE_DAMAGE, 0, 5);
					state->shakeFrame = 4;
				}
			}
			syHeapRelease(model);
			syHeapRelease(projected);
		}

		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* The canvas burst's modes (btEffectRunCanvasBurst). */
typedef enum CANVAS_BURST_MODE {
	CANVAS_BURST_SHRED, /* a glow grows on the target as rays close on it */
	CANVAS_BURST_SMALL_BALL, /* an enemy's attack: a small ball, no flames */
	CANVAS_BURST_LARGE_BALL, /* the same attack's critical hit: a large ball */
	CANVAS_BURST_MINE_BALL	 /* a large ball to the targets' midpoint */
} CANVAS_BURST_MODE;

/* An enemy class's plain attack that throws a glowing ball
   (ATTACK_EFFECT_BURST): a small ball, or a large one for its critical hit. */
void btEffectRunBallAttack(BATTLE_EFFECT_PARM *args)
{
	if(args->kind >= EFFECT_ID_CRITICAL_OFFSET)
		btEffectRunCanvasBurst(args, CANVAS_BURST_LARGE_BALL);
	else
		btEffectRunCanvasBurst(args, CANVAS_BURST_SMALL_BALL);
}

/* A glow grows on the first target as sixteen rays of motes close on it, and it
   bursts into flames and sparks; Impair follows.

   Abilities: Shred. */
void btEffectRunShred(BATTLE_EFFECT_PARM *args)
{
	btEffectRunCanvasBurst(args, CANVAS_BURST_SHRED);
}

/* From the user's leap pose, a glowing ball in BlueFlameSheet's colours flies
   to the targets' midpoint and bursts into flames and sparks.

   Abilities: Mine Ball. */
void btEffectRunMineBall(BATTLE_EFFECT_PARM *args)
{
	ACTOR *actor;

	actor = (ACTOR *)btObjectGetStub(args->user)->object;
	fdActorSelectAnimationStub(actor, POSE_LEAP);
	fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL * 7 / 4);
	btEffectRunCanvasBurst(args, CANVAS_BURST_MINE_BALL);
	fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
}

/* The ball: the state's last particle. */
#define CANVAS_BURST_BALL (&state->particles[ARRAY_COUNT(state->particles) - 1])

/* Draws a disc of the given size from the discs in the map cell buffer as its
   four quarters about (x, y), the same quarter mirrored each way. */
static inline void DrawDisc(void *canvas, Sint16 *discs, Sint32 cells, Sint32 x,
	Sint32 y, Sint32 size, Sint32 index)
{
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	(RamHeapSlots->ramCodePaired)(canvas, (Uint8 *)(discs[index] + cells),
		x - (size >> 1), y - size, size >> 1, size);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	(RamHeapSlots->ramCodePaired)(
		canvas, (Uint8 *)(discs[index] + cells), x, y - size, size >> 1, size);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
	(RamHeapSlots->ramCodePaired)(canvas, (Uint8 *)(discs[index] + cells),
		x - (size >> 1), y, size >> 1, size);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
	(RamHeapSlots->ramCodePaired)(
		canvas, (Uint8 *)(discs[index] + cells), x, y, size >> 1, size);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
}

/* Where FlashBurstSheet is decoded in the effect's pictures. */
#define CANVAS_BURST_FLASH_BURST_PICTURES 2048
/* The state's particles: Shred's rays, then at the hit the flames, and the
   sparks after them. */
#define CANVAS_BURST_RAYS				  16
#define CANVAS_BURST_FLAMES				  16
#define CANVAS_BURST_FIRST_SPARK		  CANVAS_BURST_FLAMES
#define CANVAS_BURST_SPARKS				  47
/* A flame's or spark's ticks to live at the hit, one more for every third; a
   flame shows its six pictures over its last 18. */
#define CANVAS_BURST_LIFE				  17
/* The shaded discs drawn into the map cell buffer, 2 to 64 pixels across. */
#define CANVAS_BURST_DISCS				  32
/* Shred's glow grows to the largest disc; the balls grow to these. */
#define CANVAS_BURST_GLOW_SIZE			  64
#define CANVAS_BURST_SMALL_BALL_SIZE	  24
#define CANVAS_BURST_LARGE_BALL_SIZE	  40
/* The motes of each of Shred's rays. A ray counts down from its own delay and
   shows from CANVAS_BURST_RAY_FIRST to CANVAS_BURST_RAY_LAST; its motes, 2 * (2
   count + j) pixels out, have all closed in on the target by -8. */
#define CANVAS_BURST_RAY_MOTES			  16
#define CANVAS_BURST_RAY_FIRST			  15
#define CANVAS_BURST_RAY_LAST			  (-32)
/* The ball steps a 64th of the screen's distance a tick (1 << 10 in 16.16). */
#define CANVAS_BURST_BALL_STEP_SHIFT	  10

/* A glowing ball, or Shred's glow growing on the target as sixteen rays close
   on it, hits the target; at the hit the targets flash and recoil, sixteen
   flames rise and 47 sparks scatter. Shred plays 78 ticks, hits at 54 and runs
   btEffectRunImpair after; the small ball plays 66, hits at 32, nudges the
   targets and throws its sparks at half speed, without flames; the large ball
   plays 66 and hits at 32; Mine Ball's flies to the targets' midpoint in its
   own colours, plays 58 and hits at 24. The balls of an enemy's attack close by
   restoring the battle display themselves. */
void btEffectRunCanvasBurst(BATTLE_EFFECT_PARM *args, Uint32 mode)
{
	SCREEN_POINT		 from;
	SCREEN_POINT		 to;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Uint8		  *graphics;
	Sint16		  *discs;
	Sint32		   ticks;
	Sint32		   hitFrame;
	Sint32		   offset;
	Sint32		   i;
	Sint32		   j;
	Sint32		   cells = (Sint32)RamMapMetatileBuffer;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	graphics = RamHeapSlots->battleSheet;
	if(mode <= CANVAS_BURST_SMALL_BALL)
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	else
		btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	discs = (Sint16 *)state->sheet;
	if(mode == CANVAS_BURST_SHRED)
		ticks = 78;
	else if(mode == CANVAS_BURST_MINE_BALL)
		ticks = 58;
	else
		ticks = 66;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);

	/* Discs of sizes 2 to 64 one after another in the map cell buffer, with
	   their offsets at the start of the sheet. */
	offset = 0;
	for(i = 0; i != CANVAS_BURST_DISCS; i++) {
		Sint32 size = i * 2 + 2;

		btEffectDrawShadedCircles(
			(Uint8 *)(offset + cells), size, toFIXED(2673) / 2048);
		discs[i] = offset;
		offset += (i * 2 + 3) * size / 2;
	}
	if(mode == CANVAS_BURST_MINE_BALL)
		syWordsCopy((void *)BG_PLTT,
			syResourceGetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
			SHEET_PALETTE_BYTES);
	else
		syWordsCopy((void *)BG_PLTT,
			syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
			SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + CANVAS_BURST_FLASH_BURST_PICTURES, 1, 0);

	for(frame = 0; frame != ticks; frame++) {
		btEffectPosProjectBattler(args->user, &from);
		if(mode == CANVAS_BURST_MINE_BALL)
			btEffectGetTargetsMidpoint(&to);
		else
			btEffectPosProjectBattler(args->targets[0], &to);
		if(mode == CANVAS_BURST_SHRED && frame == 0)
			sySoundPlayStub(SE_SPIN);

		if(frame == 0) {
			if(mode == CANVAS_BURST_SHRED) {
				PARTICLE *step;

				/* The rays: one every 1/16 turn, each starting after its own
				   delay. */
				for(i = 0; i != CANVAS_BURST_RAYS; i++) {
					step = &state->particles[i];
					step->z = i * (TRIG_TURN / CANVAS_BURST_RAYS);
					step->variant = (syRandom() & 31) + 16;
				}
			} else {
				/* The ball leaves the user's chest already seven steps
				   along. */
				CANVAS_BURST_BALL->x = toFIXED(from.x / 2);
				CANVAS_BURST_BALL->y = INTtoFIXED(from.y - 36);
				CANVAS_BURST_BALL->velocityX = (to.x - from.x)
					<< CANVAS_BURST_BALL_STEP_SHIFT;
				CANVAS_BURST_BALL->velocityY = (to.y - from.y)
					<< CANVAS_BURST_BALL_STEP_SHIFT;
				CANVAS_BURST_BALL->x += CANVAS_BURST_BALL->velocityX * 7;
				CANVAS_BURST_BALL->y += CANVAS_BURST_BALL->velocityY * 7;
			}
		}

		if(mode == CANVAS_BURST_SHRED) {
			/* Ticks 18-54: the glow grows at the target. */
			if((Uint32)(frame - 18) <= 36) {
				Sint32 size = frame * 2 - 32;
				Sint32 x;
				Sint32 y;

				if(size > CANVAS_BURST_GLOW_SIZE)
					size = CANVAS_BURST_GLOW_SIZE;
				x = to.x / 2;
				y = to.y - 32;
				DrawDisc(canvas, discs, cells, x, y, size, (size - 2) >> 1);
			}
		} else if((Uint32)(frame - 2) <= 30) {
			/* Ticks 2-32: the ball flies, growing. */
			Sint32 size = frame / 4 * 2 + 16;
			Sint32 index;
			Sint32 x;
			Sint32 y;

			if(mode == CANVAS_BURST_SMALL_BALL) {
				if(size > CANVAS_BURST_SMALL_BALL_SIZE)
					size = CANVAS_BURST_SMALL_BALL_SIZE;
			} else if(size > CANVAS_BURST_LARGE_BALL_SIZE) {
				size = CANVAS_BURST_LARGE_BALL_SIZE;
			}
			index = (size - 2) >> 1;
			x = FIXEDtoINT(CANVAS_BURST_BALL->x);
			y = FIXEDtoINT(CANVAS_BURST_BALL->y);
			CANVAS_BURST_BALL->x += CANVAS_BURST_BALL->velocityX;
			CANVAS_BURST_BALL->y += CANVAS_BURST_BALL->velocityY;
			DrawDisc(canvas, discs, cells, x, y, size, index);
		}

		/* Shred, ticks 0-53: sixteen rays of motes close on the target, each
		   from its own delay. */
		if(mode == CANVAS_BURST_SHRED && frame <= 53) {
			PARTICLE *step = state->particles;

			for(i = 0; i != CANVAS_BURST_RAYS; i++) {
				step->variant--;
				if(step->variant >= CANVAS_BURST_RAY_LAST &&
					step->variant <= CANVAS_BURST_RAY_FIRST) {
					Sint32 size = 4;

					for(j = 0; j != CANVAS_BURST_RAY_MOTES; j++) {
						Sint32 radius = (step->variant * 2 + j) * 2;

						if(radius >= 0) {
							Sint32 width = 4;
							Sint32 x = to.x / 2 +
								(FIXEDtoINT(sySin(step->z) * radius));
							Sint32 height = size * 2;
							Sint32 half = size >> 1;
							Sint32 y =
								to.y + (FIXEDtoINT(syCos(step->z) * radius));

							draw[0](canvas,
								graphics + BattleEffectMotePictures[size - 1],
								x - half, y - 32 - size, width, height);
						}
					}
				}
				step++;
			}
		}

		if(mode == CANVAS_BURST_SHRED)
			hitFrame = 54;
		else if(mode == CANVAS_BURST_MINE_BALL)
			hitFrame = 24;
		else
			hitFrame = 32;
		if(frame >= hitFrame) {
			PARTICLE *newFlame;
			PARTICLE *newSpark;
			PARTICLE *flame;
			PARTICLE *spark;

			if(frame == hitFrame) {
				/* The hit: the targets are hurled (the small ball nudges them)
				   and flash. */
				if(mode == CANVAS_BURST_SHRED ||
					mode == CANVAS_BURST_LARGE_BALL ||
					mode == CANVAS_BURST_MINE_BALL) {
					EFFECT_SHOW_DAMAGE(SE_CRASH);
					for(i = 0; i != args->targetCount; i++)
						btMotionApplyKnockbackStub(
							args->targets[i], KNOCKBACK_HURL);
				} else {
					EFFECT_SHOW_DAMAGE(SE_IMPACT);
					for(i = 0; i != args->targetCount; i++)
						btMotionApplyKnockbackStub(
							args->targets[i], KNOCKBACK_NUDGE);
				}
				for(i = 0; i != args->targetCount; i++)
					btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE,
						POSE_DAMAGE, i, 8);
				state->shakeFrame = 8;

				/* Sixteen flames, faster one by one. */
				for(i = 0; i != CANVAS_BURST_FLAMES; i++) {
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
					Sint32 speed;

					newFlame = &state->particles[i];
					speed = i * 4 + 4;
					newFlame->x = toFIXED(to.x / 2);
					newFlame->y = INTtoFIXED(to.y - 32);
					newFlame->velocityX = (speed * sySin(angle)) >> 3;
					newFlame->velocityY = -(speed * syCos(angle)) >> 3;
					newFlame->variant = i / 3 + CANVAS_BURST_LIFE;
				}

				/* The sparks, at half speed from the small ball. */
				for(i = 0; i != CANVAS_BURST_SPARKS; i++) {
					Sint32 speed = (syRandom() & 255) + 128;
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

					newSpark = &state->particles[CANVAS_BURST_FIRST_SPARK + i];

					newSpark->x = toFIXED(to.x / 2);
					newSpark->y = INTtoFIXED(to.y - 32);
					newSpark->velocityX = (sySin(angle) * speed) >> 6;
					newSpark->velocityY = -(syCos(angle) * speed) >> 5;
					if(mode == CANVAS_BURST_SMALL_BALL) {
						newSpark->velocityX /= 2;
						newSpark->velocityY /= 2;
					}
					newSpark->variant = i / 3 + CANVAS_BURST_LIFE;
				}
			}

			/* The flames: FlashBurstSheet's six pictures, three ticks each. */
			if(mode == CANVAS_BURST_SHRED || mode == CANVAS_BURST_LARGE_BALL ||
				mode == CANVAS_BURST_MINE_BALL) {
				flame = state->particles;
				for(i = 0; i != CANVAS_BURST_FLAMES; i++) {
					Sint32 x = FIXED_INTEGER_PART(flame->x);
					Sint32 y = FIXED_INTEGER_PART(flame->y);

					btParticleAdvanceWithGravity2D(flame, 60, 0);
					if((Uint32)flame->variant <= CANVAS_BURST_LIFE)
						draw[0](canvas,
							state->sheet + CANVAS_BURST_FLASH_BURST_PICTURES +
								CanvasBurstFlamePictures[flame->variant / 3] *
									PICTURE_BYTES(FLASH_BURST),
							x - FLASH_BURST_WIDTH / 2,
							y - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
							FLASH_BURST_HEIGHT);
					if(flame->variant > 0)
						flame->variant = flame->variant - 1;
					else
						flame->variant = -1;
					flame++;
				}
			}

			/* The sparks shrink as they fly. */
			for(i = 0; i != CANVAS_BURST_SPARKS; i++) {
				spark = &state->particles[CANVAS_BURST_FIRST_SPARK + i];
				if(spark->variant >= 0) {
					Sint32 size = (spark->variant >> 2) + 1;

					EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
						FIXED_INTEGER_PART(spark->x),
						FIXED_INTEGER_PART(spark->y), size);
					btParticleAdvanceWithGravity2D(spark, 60, 0);
					spark->variant--;
				}
			}
		}

		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(mode == CANVAS_BURST_SHRED) {
		syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
		syWordsClear(canvas, CANVAS_VIEW_BYTES);
		syCloseEvent(btEffectDarkenBackgroundStep);
		btEffectRunImpair(args);
	} else if(mode == CANVAS_BURST_MINE_BALL) {
		btEffectEndCanvasLayer();
	} else {
		/* Back to the battle display: BG1's scroll, BG2 as before, the palette
		   fade off and the backgrounds on again. */
		BATTLE_STATE *battle = RamHeapSlots->battleState;

		sySoundPlayStub(SOUND_FADE_OUT_ACTION_SE);
		BgScrollWork[1].x = state->savedBg1X;
		BgScrollWork[1].y = state->savedBg1Y;
		REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
		syCloseEvent(btEffectDarkenBackgroundStep);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
				DISPCNT_WIN1_ON,
			IO_WRITE_HALFWORD);
		REG_BLDCNT = 0;
		sySynch(1);
		btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
				DISPCNT_WIN1_ON,
			IO_WRITE_HALFWORD);
		sySynch(1);
	}
}

#undef CANVAS_BURST_BALL
