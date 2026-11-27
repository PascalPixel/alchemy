/* Legend, and the Bramble, Frost, Flame and Thunder Cards. Part of
   battle_effect.c. */

/* Legend's sparks, and the cards', are the first LEGEND_SPARK_COUNT of
   EXTRA_PARTICLES, 128 for each of Legend's swords; they bounce once on the
   ground at SPARK_GROUND_Y. */
#define LEGEND_SPARK_COUNT	512
#define SPARK_GROUND_Y		104
#define LEGEND_SWORD_SPARKS 128

/* BladeSheet's sword, point down, in the effect's pictures; it is drawn cut off
   at SPARK_GROUND_Y, stuck in the ground. */
#define LEGEND_BLADE_PICTURES 16384
#define LEGEND_SWORD_WIDTH	  40
#define LEGEND_SWORD_HEIGHT	  80

/* Where FlameGradientTexture is decoded in the effect's pictures. */
#define LEGEND_FLAME_GRADIENT 20480

/* The fan of LEGEND_FAN_FLAMES flames, FlameGradientTexture's dark red to
   white, tilted LEGEND_FAN_TILT. */
#define LEGEND_FAN_FLAMES		8
#define LEGEND_FAN_TILT			18384
/* Each sword's ring of fire grows by this share of its size a tick, from
   half. */
#define LEGEND_FIRE_RING_GROWTH 12384

/* The swords set out LEGEND_SWORD_INTERVAL ticks apart; each falls from
   LEGEND_SWORD_FALL_TICK ticks after it sets out, rings out at
   LEGEND_SWORD_SOUND_TICK and strikes at LEGEND_SWORD_STRIKE_TICK. */
#define LEGEND_SWORD_INTERVAL		16
#define LEGEND_SWORD_FALL_TICK		50
#define LEGEND_SWORD_SOUND_TICK		54
#define LEGEND_SWORD_STRIKE_TICK	59
/* Each sword falls for LEGEND_SWORD_FALL_TICKS ticks, from
   LEGEND_SWORD_START_DISTANCE pixels above and beside the target. */
#define LEGEND_SWORD_FALL_TICKS		12
#define LEGEND_SWORD_START_DISTANCE 80
/* with one sword; each further one adds an interval */
#define LEGEND_TICKS				86

/* Legend, Excalibur's unleash: a fan of flames opens over the target until the
   first sword falls, while a rune, mirrored from one corner, rises and turns
   behind it. Then one sword for each step of the variant (variant + 1 in all)
   falls on the target from above the user's side: it strikes, throwing sparks
   that bounce on the ground, while a ring of fire grows and fades where it
   lands.

   Abilities: Legend. */
void btEffectRunLegend(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	DRAW_RECTANGLE		 draw[2];
	Uint8				*graphics;
	SCREEN_POINT		 start;
	SCREEN_POINT		 tgt;
	MODEL_TEXTURE		 texture;
	PARTICLE			*step;
	Sint32				 i;
	Sint32				 j;
	Uint8				*cells = RamMapMetatileBuffer;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	graphics = RamHeapSlots->battleSheet;

	/* Set up the canvas and a clipped additive blitter, mirrored to face the
	   target; mirror the rune's corner into the sheet; load the sparks, the
	   sword and the ring of fire. */
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(args->side == EFFECT_SIDE_ENEMIES)
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	else
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	draw[0] = RamHeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_CORNER_RUNE_A_SHEET, cells, 1, 1);
	btPictureMirrorIntoQuadrants(cells, state->sheet, 64, 64);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_BLADE_SHEET, state->sheet + LEGEND_BLADE_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FLAME_GRADIENT_TEXTURE,
		state->sheet + LEGEND_FLAME_GRADIENT, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != args->variant * LEGEND_SWORD_INTERVAL + LEGEND_TICKS;
		frm++) {
		/* Tick 0: the swords set out from above and beside the target, on the
		   user's side, toward it. */
		if(frm == 0) {
			btEffectPosProjectBattler(args->tgts[0], &tgt);
			if(args->side == EFFECT_SIDE_ENEMIES)
				start.x = tgt.x + LEGEND_SWORD_START_DISTANCE;
			else
				start.x = tgt.x - LEGEND_SWORD_START_DISTANCE;
			start.y = tgt.y - LEGEND_SWORD_START_DISTANCE;
			for(i = 0; i != 3; i++) {
				step = &state->particles[i];
				step->x = start.x * (toFIXED(1) / 2);
				step->y = toFIXED(start.y);
				step->velX = (tgt.x - start.x) * (toFIXED(1) / 32);
				step->velY = (tgt.y - start.y) * (toFIXED(1) / 16);
				step->variant = 0;
			}
			EFFECT_FREE_PARTICLES(i, EXTRA_PARTICLES, LEGEND_SPARK_COUNT);
		}

		/* The swords, one each LEGEND_SWORD_INTERVAL ticks. */
		for(i = 0; i != args->variant + 1; i++) {
			step = &state->particles[i];
			if(frm == i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_SOUND_TICK)
				sySndPlayStub(SE_POWER_UP);

			/* The strike: the target reels and flashes, and sparks fly up from
			   it. The last sword ends the action. */
			if(frm == i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_STRIKE_TICK) {
				if(i == args->variant)
					EFFECT_SHOW_DAMAGE(SE_CLATTER);
				else
					sySndPlayStub(SE_CLATTER);
				btEffectStartActorMotion(args->tgts[0], 1, 0, 0, 0, 100);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				state->shakeFrm = 4;
				for(j = 0; j != LEGEND_SWORD_SPARKS; j++) {
					PARTICLE *spark =
						&EXTRA_PARTICLES[i * LEGEND_SWORD_SPARKS + j];
					Sint32 ang;
					Sint32 force;

					spark->x = tgt.x * (toFIXED(1) / 2);
					spark->y = toFIXED(96);
					ang = syRandom() % (TRIG_TURN * 3 / 4) + DEGtoANG(45);
					force = syRandom() & 127;
					spark->velX = (sySin(ang) * force) >> 4;
					spark->velY = (syCos(ang) * force) >> 3;
					spark->variant = (syRandom() & 7) + 16;
				}
			}

			/* The sword falls, faster each tick, until it sticks in the
			   ground. */
			if(frm >= i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_FALL_TICK) {
				Sint32 x = FIXED_INTEGER_PART(step->x) - LEGEND_SWORD_WIDTH / 2;
				Sint32 y = FIXED_INTEGER_PART(step->y) - 50;
				Sint32 height = LEGEND_SWORD_HEIGHT;

				if(y > SPARK_GROUND_Y - LEGEND_SWORD_HEIGHT)
					height = SPARK_GROUND_Y - y;
				draw[0](canvas, state->sheet + LEGEND_BLADE_PICTURES, x, y,
					LEGEND_SWORD_WIDTH, height);
				if(step->variant <= LEGEND_SWORD_FALL_TICKS) {
					step->x += step->velX;
					step->y += step->velY;
					step->velX += (tgt.x - start.x) * (toFIXED(1) / 128);
					step->velY += (tgt.y - start.y) * (toFIXED(1) / 64);
				}
				step->variant++;
			}
		}

		/* The sparks bounce on the ground. */
		for(i = 0; i != LEGEND_SPARK_COUNT; i++) {
			step = &EXTRA_PARTICLES[i];
			if(step->variant >= 0) {
				Sint32 size = (step->variant >> 2) + 1;

				EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
					FIXED_INTEGER_PART(step->x), FIXED_INTEGER_PART(step->y),
					size);
				if(step->y > toFIXED(SPARK_GROUND_Y)) {
					step->y = toFIXED(SPARK_GROUND_Y);
					step->velY = -step->velY / 2;
				}
				btParticleAdvanceWithGravity2D(step, 62, toFIXED(1) / 4);
				step->variant--;
			}
		}

		/* This tick's models: one quad, retextured for each part. */
		{
			RENDER_MODEL *model;
			void		 *vertices;
			FIXED		  scale;

			vertices = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = state->sheet;
			model->mesh = BattleEffectLargeSquareMesh;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->vertices = vertices;

			/* Ticks 16-107: the rune rises behind the target, turning, then
			   fades. */
			if(frm >= 16 && frm <= 107) {
				Sint32 height = frm * 2 - 96;
				Sint32 alpha = 0;

				scale = frm * (toFIXED(1) / 8) - toFIXED(2);
				if(height > -48)
					height = -48;
				/* It fades from 60 ticks after the last sword sets out, four
				   levels a tick. */
				if(frm >= args->variant * LEGEND_SWORD_INTERVAL + 60)
					alpha =
						-((frm - args->variant * LEGEND_SWORD_INTERVAL) * 4 +
							-240);
				if(scale > toFIXED(2))
					scale = toFIXED(2);
				model->shade = alpha;
				syInitMatrix();
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syTranslate(INTtoFIXED(start.x - 128), toFIXED(height), 0);
				syRotX(TRIG_TURN * 89 / 256);
				if(args->side == EFFECT_SIDE_ENEMIES)
					syRotY(TRIG_TURN * 25 / 256);
				else
					syRotY(-TRIG_TURN * 25 / 256);
				syRotZ(frm * (TRIG_TURN / 128));
				syMatrixScaleUniform(scale);
				syScale(toFIXED(1), toFIXED(1), toFIXED(1));
				btRenderTransformVertices(
					BattleEffectSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
			}

			/* Each sword's ring of fire, FireRingTexture's, grows where it
			   lands, holds 8 ticks and fades over 8. */
			texture.pixels = cells;
			for(i = 0; i != args->variant + 1; i++) {
				if(frm >= i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_FALL_TICK) {
					Sint32 age = frm - i * LEGEND_SWORD_INTERVAL -
						LEGEND_SWORD_FALL_TICK;
					Sint32 alpha = -MODEL_SHADE_INVISIBLE - age * 8;

					if(alpha > 0)
						alpha = 0;
					if(alpha > MODEL_SHADE_INVISIBLE) {
						scale = age * LEGEND_FIRE_RING_GROWTH + toFIXED(1) / 2;
						model->shade = alpha;
						syInitMatrix();
						syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
						syTranslate(INTtoFIXED(start.x - 128), -toFIXED(48), 0);
						syRotX(TRIG_TURN * 89 / 256);
						if(args->side == EFFECT_SIDE_ENEMIES)
							syRotY(TRIG_TURN * 25 / 256);
						else
							syRotY(-TRIG_TURN * 25 / 256);
						syRotZ(frm * (TRIG_TURN / 128));
						syMatrixScaleUniform(scale);
						btRenderTransformVertices(
							BattleEffectSquareCorners, vertices, 4);
						btRenderDrawModelList(model);
					}
				}
			}

			/* Until the swords fall: the fan of flames opens, pulsing, and
			   closes. */
			if(frm < LEGEND_SWORD_FALL_TICK) {
				texture.logWidth = 5;
				texture.logHeight = 6;
				texture.pixels = state->sheet + LEGEND_FLAME_GRADIENT;
				model->mesh = BattleEffectTallMesh;
				for(i = 0; i != LEGEND_FAN_FLAMES; i++) {
					model->shade = 0;
					syInitMatrix();
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syTranslate(INTtoFIXED(start.x - 128), -toFIXED(80), 0);
					syRotX(LEGEND_FAN_TILT);
					syRotZ((frm + i * 16) << 9);
					scale = frm * (toFIXED(1) / 16) - toFIXED(1) / 4;
					if(scale > toFIXED(1))
						scale = toFIXED(1);
					if(frm > 31)
						scale = toFIXED(3) - frm * (toFIXED(1) / 16);
					syScale(toFIXED(1) / 2, toFIXED(1),
						(sySin((frm + i * 8) << 9) / 8 + scale) * 2);
					btRenderTransformVertices(
						LegendFanFlameCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Abilities: Bramble Card. */
void btEffectRunBrambleCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunElementCards(args, CARD_BRAMBLE);
}

/* Abilities: Frost Card. */
void btEffectRunFrostCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunElementCards(args, CARD_FROST);
}

/* Abilities: Flame Card. */
void btEffectRunFlameCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunElementCards(args, CARD_FLAME);
}

/* Abilities: Thunder Card. */
void btEffectRunThunderCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunElementCards(args, CARD_THUNDER);
}

/* A card's picture, CARD_WIDTH by CARD_HEIGHT, at its sprite sheet's start; the
   Bramble Card's sprite sheet has its three vines after it, 32 by 128 as
   JaggedStreakSheet's streaks are. */
#define CARD_STREAK_WIDTH	   32
#define CARD_STREAK_HEIGHT	   128
/* Where the card's element pictures are decoded in the effect's pictures, after
   the card: CrystalSheet, FlashBurstSheet, SmokeSheet or JaggedStreakSheet. */
#define ELEMENT_CARDS_PICTURES PICTURE_BYTES(CARD)
/* The state's particles: the shards, smoke or pictures the card throws, then
   one strike countdown for each target. Each target has its own run of the
   sparks, and a strike throws up the next free 16 of them. The sparks are
   Legend's, LEGEND_SPARK_COUNT of EXTRA_PARTICLES. */
#define CARD_PIECES			   56
#define CARD_STRIKE_TIMERS	   CARD_PIECES
#define CARD_SPARKS_PER_TARGET 64
#define CARD_STRIKE_SPARKS	   16

/* The card rises and grows for CARD_RISE_TICKS ticks, then shows its picture;
   from CARD_SHRINK_TICK it rises faster and shrinks away. It swings from side
   to side once every 96 ticks as it rises, and leans CARD_TILT. */
#define CARD_RISE_TICKS			   48
#define CARD_SHRINK_TICK		   96
#define CARD_TILT				   6000
/* The shards and smoke land on the ground at CARD_GROUND_Y. A Frost Card's
   shard starts tumbling CARD_SHARD_TUMBLE_SPEED a tick, slowing by one every
   other tick, and is drawn until its tumble reaches CARD_SHARD_TUMBLE_END. */
#define CARD_GROUND_Y			   96
#define CARD_SHARD_TUMBLE_SPEED	   32
#define CARD_SHARD_TUMBLE_END	   1024
/* The ticks the strikes run between: the Thunder Card's start first and the
   Bramble Card's end first. */
#define CARD_STRIKES_FROM		   68
#define CARD_THUNDER_STRIKES_FROM  52
#define CARD_BRAMBLE_STRIKES_FROM  60
#define CARD_STRIKES_UNTIL		   110
#define CARD_BRAMBLE_STRIKES_UNTIL 96
/* The vines lean CARD_VINE_ANGLE, each swaying TRIG_TURN / CARD_VINE_SWAY
   either way from its own phase. */
#define CARD_VINE_ANGLE			   12192
#define CARD_VINE_SWAY			   16

/* The cards of the four suits (Bramble, Frost, Flame and Thunder Card): a card
   rises from the user, spinning, swinging and growing, then shows its picture,
   and the background takes its colours. The Frost Card throws crystal shards
   and the Flame Card puffs of fiery smoke at the targets, while the Bramble
   Card's thorny vines and the Thunder Card's bolts of lightning reach out over
   them. Each target is struck every five ticks for a while, reeling, flashing
   and throwing up sparks, before the card shrinks away. */
void btEffectRunElementCards(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	DRAW_RECTANGLE		 draw[2];
	Uint8				*graphics;
	FIXED				 scale;
	SCREEN_POINT		 card;
	SCREEN_POINT		 tgts[8];
	SCREEN_POINT		 aim;
	MODEL_TEXTURE		 texture;
	RENDER_MODEL		*model;
	void				*vertices;
	Sint32				 i;
	Sint32				 firstHit;
	Sint32				 lastHit;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	graphics = RamHeapSlots->battleSheet;

	/* Set up the canvas, a clipped additive blitter, the sparks, and the card's
	   picture and its extras. */
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = RamHeapSlots->ramCode;
	scale = 0;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	if(mode == CARD_BRAMBLE) {
		btResourceLoadAndDecompress(
			RESOURCE_VINE_CARD_SHEET, state->sheet, 1, 1);
	} else if(mode == CARD_FROST) {
		btResourceLoadAndDecompress(
			RESOURCE_DRAGON_CARD_SHEET, state->sheet, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_CRYSTAL_SHEET,
			state->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
	} else if(mode == CARD_FLAME) {
		btResourceLoadAndDecompress(
			RESOURCE_SUN_CARD_SHEET, state->sheet, 1, 1);
		/* DEAD: SmokeSheet is decoded over FlashBurstSheet straight after; the
		   ROM keeps both loads. */
		btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
			state->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_SMOKE_SHEET, state->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
	} else {
		btResourceLoadAndDecompress(
			RESOURCE_BOLT_CARD_SHEET, state->sheet, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_JAGGED_STREAK_SHEET,
			state->sheet + ELEMENT_CARDS_PICTURES, 0, 0);
	}
	REG_BLDCNT = 0;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	for(frm = 0; frm != ElementCardsTicks[mode]; frm++) {
		/* Tick 0: clear the particles, give each member a strike countdown,
		   and find the user and every target. */
		if(frm == 0) {
			sySndPlayStub(SE_WARP);
			EFFECT_FREE_PARTICLES(i, state->particles, CARD_PIECES);
			for(i = 0; i != args->tgtCnt; i++)
				state->particles[CARD_STRIKE_TIMERS + i].variant =
					(syRandom() & 3) + 2;
			EFFECT_FREE_PARTICLES(i, EXTRA_PARTICLES, LEGEND_SPARK_COUNT);
			btEffectPosProjectSpr(args->user, &card);
			for(i = 0; i != args->tgtCnt; i++)
				btEffectPosProjectScaledBattler(args->tgts[i], &tgts[i]);
			REG_BLDCNT = 0;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
			scale = toFIXED(1) / 4;
		}

		/* The card rises and grows, and later rises faster and shrinks. */
		if(frm < CARD_RISE_TICKS) {
			card.y--;
			scale += toFIXED(1) / 128;
			if(scale > toFIXED(1) / 2)
				scale = toFIXED(1) / 2;
		}
		if(frm >= CARD_SHRINK_TICK) {
			scale -= toFIXED(1) / 128;
			card.y -= 2;
		}

		/* The card shows its picture. */
		if(frm == CARD_RISE_TICKS) {
			if(mode == CARD_BRAMBLE)
				sySndPlayStub(SE_BOSS_ROAR);
			if(mode == CARD_FROST)
				sySndPlayStub(SE_CLATTER);
			if(mode == CARD_FLAME)
				sySndPlayStub(SE_LAUNCH);
			if(mode == CARD_THUNDER)
				sySndPlayStub(SE_RISE_WITH_SPARKS);
			if(mode != CARD_BRAMBLE) {
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
				REG_BLDCNT =
					BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			}
		}

		if(frm >= CARD_RISE_TICKS) {
			/* The background steps toward the card's colours. */
			if(mode != CARD_BRAMBLE) {
				if(mode == CARD_FROST)
					btEffectStepPaletteToResource(RESOURCE_CRYSTAL_SHEET);
				else if(mode == CARD_THUNDER)
					btEffectStepPaletteToResource(RESOURCE_VIOLET_PALETTE_F);
				else
					btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
			}

			/* The Frost and Flame cards throw one shard or puff of smoke a
			   tick at a target in turn, until the card shrinks. */
			if(mode == CARD_FROST || mode == CARD_FLAME) {
				Sint32 member = frm % args->tgtCnt;

				aim.x = tgts[member].x + (syRandom() & 63) - 32;
				if(mode == CARD_FROST)
					aim.y = tgts[member].y + (syRandom() & 255) - 96;
				else if(mode == CARD_FLAME)
					aim.y = tgts[member].y + (syRandom() & 63) + 32;
				if(frm < CARD_SHRINK_TICK) {
					PARTICLE *step = &state->particles[frm % CARD_PIECES];

					step->x = card.x * (toFIXED(1) / 2);
					step->y = toFIXED(card.y);
					step->velX = (aim.x - card.x) * (toFIXED(1) / 32);
					step->velY = (aim.y - card.y) * (toFIXED(1) / 32);
					step->z = CARD_SHARD_TUMBLE_SPEED;
					step->variant = 0;
				}
			}

			if(mode == CARD_FROST) {
				/* The shards tumble through their three cells, bounce and
				   slow. */
				for(i = 0; i != CARD_PIECES; i++) {
					PARTICLE *step = &state->particles[i];

					if(step->variant >= 0 &&
						step->variant < CARD_SHARD_TUMBLE_END) {
						Sint32 cell =
							i % 5 * 3 + step->variant / SHARD_TURN % 3;

						/* UB: a fifth of the shards (i % 5 == 4) take cells 12
						   to 14, past the end of PrismShardPictures' twelve;
						   the heights read there are 0, so they draw
						   nothing. */
#if FIX_CRYSTAL_SHARD_TABLE_OVERRUN
						if(i % 5 != 4)
#endif
							draw[0](canvas,
								state->sheet + PrismShardPictures[cell] +
									ELEMENT_CARDS_PICTURES +
									CRYSTAL_SHARD_PICTURES,
								FIXED_INTEGER_PART(step->x) -
									PrismShardWidths[cell] / 2,
								FIXED_INTEGER_PART(step->y) -
									PrismShardHeights[cell] / 2,
								PrismShardWidths[cell],
								PrismShardHeights[cell]);
						btParticleAdvanceWithGravity2D(
							step, PARTICLE_DAMPING_NONE, 0);
						if(step->y > toFIXED(CARD_GROUND_Y))
							step->velY = -step->velY / 2;
						step->variant += step->z;
						if(step->z > 1 && (frm & 1))
							step->z--;
					}
				}
			} else if(mode == CARD_FLAME) {
				/* The smoke flies to the ground and plays its six cells. */
				for(i = 0; i != CARD_PIECES; i++) {
					PARTICLE *step = &state->particles[i];

					if(step->variant >= 0) {
						if(step->variant > 1)
							draw[0](canvas,
								state->sheet + ELEMENT_CARDS_PICTURES +
									step->variant / 4 *
										PICTURE_BYTES(SMOKE_PUFF),
								FIXED_INTEGER_PART(step->x) - 12,
								FIXED_INTEGER_PART(step->y) - 24,
								SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
						step->x += step->velX;
						step->y += step->velY;
						if(step->y > toFIXED(CARD_GROUND_Y)) {
							step->y = toFIXED(CARD_GROUND_Y);
							step->velY = 0;
						}
						step->variant++;
						if(step->variant == SMOKE_PUFF_TICKS)
							step->variant = -1;
					}
				}
			}
		}

		/* The strikes: every five ticks each target in turn reels, flashes and
		   throws up 16 sparks. */
		if(mode == CARD_THUNDER)
			firstHit = CARD_THUNDER_STRIKES_FROM;
		else if(mode == CARD_BRAMBLE)
			firstHit = CARD_BRAMBLE_STRIKES_FROM;
		else
			firstHit = CARD_STRIKES_FROM;
		if(mode == CARD_BRAMBLE)
			lastHit = CARD_BRAMBLE_STRIKES_UNTIL;
		else
			lastHit = CARD_STRIKES_UNTIL;
		if(frm >= firstHit && frm < lastHit) {
			Sint32 speed = (frm - CARD_STRIKES_FROM) * 3;

			for(i = 0; i != args->tgtCnt; i++) {
				PARTICLE *step = &state->particles[CARD_STRIKE_TIMERS + i];

				if(--step->variant == 0) {
					Sint32 burst;
					Sint32 j;

					step->variant = 5;
					state->shakeFrm = 4;
					if(mode == CARD_FROST)
						sySndPlayStub(SE_HEAVY_IMPACT);
					else if(mode == CARD_THUNDER)
						sySndPlayStub(SE_THUNDER_CARD);
					else
						sySndPlayStub(SE_IMPACT);
					if(i == 0)
						EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					btEffectStartActorMotion(args->tgts[i], 1, toFIXED(2),
						toFIXED(8), toFIXED(1), 100 + speed);
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
					burst = 0;
					btEffectPosProjectScaledBattler(args->tgts[i], &aim);
					for(j = 0; j != CARD_SPARKS_PER_TARGET; j++) {
						PARTICLE *spark =
							&EXTRA_PARTICLES[i * CARD_SPARKS_PER_TARGET + j];

						if(spark->variant < 0) {
							Sint32 ang;
							Sint32 force;

							spark->x = aim.x * (toFIXED(1) / 2);
							spark->y = INTtoFIXED(aim.y + 8);
							ang =
								syRandom() % (TRIG_TURN * 3 / 4) + DEGtoANG(45);
							force = syRandom() & 127;
							spark->velX = (sySin(ang) * force) >> 5;
							spark->velY = (syCos(ang) * force) >> 4;
							if(mode == CARD_THUNDER) {
								spark->velX /= 2;
								spark->velY /= 2;
							}
							spark->variant = (syRandom() & 7) + 16;
							if(++burst == CARD_STRIKE_SPARKS)
								break;
						}
					}
				}
			}
		}

		/* This tick's models: the card, then the textures. */
		vertices = syHeapAllocEWRAM(32);
		model = btRenderAllocateModelList(1);
		texture.logWidth = 5;
		texture.logHeight = 6;
		texture.pixels = state->sheet;
		model->kind = MODEL_TEXTURED;
		model->texture = &texture;
		model->mesh = BattleEffectTallMesh;
		model->vertices = vertices;
		/* DEAD: tick is never negative; the ROM keeps the test. */
		if(frm >= 0) {
			/* The card: it swings from side to side as it rises, tilted
			   back. */
			syInitMatrix();
			syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
			syTranslate(toFIXED(card.x) - toFIXED(128),
				toFIXED(card.y) - toFIXED(64), 0);
			if(frm < CARD_RISE_TICKS) {
				syRotX((CARD_RISE_TICKS - frm) * (TRIG_TURN / 64));
				if(args->side == EFFECT_SIDE_ENEMIES)
					syTranslate(-sySin(frm * (TRIG_TURN / 96)) << 7, 0, 0);
				else
					syTranslate(sySin(frm * (TRIG_TURN / 96)) << 7, 0, 0);
			}
			syRotY(CARD_TILT);
			syRotZ(frm * (TRIG_TURN / 64));
			syMatrixScaleUniform(scale);
			syScale(toFIXED(1), toFIXED(2), toFIXED(1));
			btRenderTransformVertices(BattleEffectSquareCorners, vertices, 4);
			btRenderDrawModelList(model);
			if(mode == CARD_BRAMBLE || mode == CARD_THUNDER) {
				syMatrixScaleUniform(toFIXED(1) / 4);
				btRenderTransformVertices(
					BattleEffectSquareCorners, vertices, 4);
			}
		}

		/* The Bramble Card's vines and the Thunder Card's bolts reach out from
		   the card's corners for 64 ticks; the bolts fade as the card
		   shrinks. */
		if((mode == CARD_THUNDER || mode == CARD_BRAMBLE) &&
			(frm >= CARD_RISE_TICKS && frm <= 111)) {
			Sint32 alpha = 0;

			if(mode == CARD_THUNDER && frm >= CARD_SHRINK_TICK)
				alpha = frm * -4 + 384;
			texture.logWidth = 5;
			texture.logHeight = 7;
			model->shade = alpha;
			model->texture = &texture;
			model->mesh = BattleEffectColumnMesh;
			model->vertices = vertices;
			if(mode == CARD_BRAMBLE) {
				/* The vines sway, each from its own phase. */
				model->kind = MODEL_SHADED;
				if(frm == 48) {
					Sint32 first = syRandom();

					for(i = 0; i != 4; i++) {
						PARTICLE *step = &state->particles[i];

						step->velY = i * (TRIG_TURN / 8);
						step->z = (first + i) % 3 * PICTURE_BYTES(CARD_STREAK) +
							PICTURE_BYTES(CARD);
					}
				}
				for(i = 0; i != 4; i++) {
					PARTICLE *step = &state->particles[i];

					step->velZ = sySin(step->velY + frm * (TRIG_TURN / 64)) /
							CARD_VINE_SWAY +
						CARD_VINE_ANGLE;
				}
			} else {
				/* The bolts flicker every other tick. */
				model->kind = MODEL_SHADED_ADDITIVE;
				if((frm & 1) == 0) {
					Sint32 first = syRandom();

					for(i = 0; i != 4; i++) {
						PARTICLE *step = &state->particles[i];

						step->velZ = (syRandom() & 8191) + 8096;
						step->z = (first + i) % 3 * PICTURE_BYTES(CARD_STREAK) +
							ELEMENT_CARDS_PICTURES;
					}
				}
			}
			for(i = 0; i != 4; i++) {
				state->particles[i].x = ((Sint16 *)vertices)[(i & 3) * 4];
				state->particles[i].y = ((Sint16 *)vertices)[(i & 3) * 4 + 1];
			}
			for(i = 0; i != 4; i++) {
				PARTICLE *step = &state->particles[i];

				texture.pixels = state->sheet + step->z;
				syInitMatrix();
				/* Each vine or bolt stands 4 pixels across from its corner. */
				syTranslate(INTtoFIXED(step->x + 4 - CANVAS_SIZE / 2),
					INTtoFIXED(step->y - CANVAS_SIZE / 2), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				if(args->side == EFFECT_SIDE_PARTY)
					syRotY(DEGtoANG(180));
				syRotZ(step->velZ);
				if(mode == CARD_BRAMBLE) {
					Sint32 length = frm * (toFIXED(1) / 8) - toFIXED(6);

					if(length > toFIXED(2))
						length = toFIXED(2);
					if(frm >= CARD_SHRINK_TICK)
						length = length - frm * (toFIXED(1) / 8) + toFIXED(12);
					syScale(toFIXED(4), length, toFIXED(1));
				} else {
					syScale(toFIXED(2), toFIXED(2), toFIXED(1));
				}
				btRenderTransformVertices(BattleEffectBeamCorners, vertices, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);

		/* The sparks bounce on the ground. */
		for(i = 0; i != LEGEND_SPARK_COUNT; i++) {
			PARTICLE *step = &EXTRA_PARTICLES[i];

			if(step->variant >= 0) {
				Sint32 size = step->variant >> 3;
				Sint32 j;

				if(mode == CARD_BRAMBLE || mode == CARD_THUNDER)
					size += 1;
				else
					size += 3;
				for(j = 0; j != ElementCardsSparkMotions[mode].steps; j++) {
					EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
						FIXED_INTEGER_PART(step->x),
						FIXED_INTEGER_PART(step->y), size);
					if(step->y > toFIXED(SPARK_GROUND_Y)) {
						step->y = toFIXED(SPARK_GROUND_Y);
						step->velY = -step->velY / 2;
					}
					btParticleAdvanceWithGravity2D(step,
						ElementCardsSparkMotions[mode].damping,
						ElementCardsSparkMotions[mode].gravity);
				}
				step->variant--;
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Abilities: Sleep Card. */
void btEffectRunSleepCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunCards(args, CARD_SLEEP);
}

/* Abilities: Baffle Card. */
void btEffectRunBaffleCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunCards(args, CARD_BAFFLE);
}

/* Abilities: Death Card. */
void btEffectRunDeathCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunCards(args, CARD_DEATH);
}

/* Abilities: Sword Card. */
void btEffectRunSwordCard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunCards(args, CARD_SWORD);
}

#undef LEGEND_SPARK_COUNT
#undef SPARK_GROUND_Y
#undef LEGEND_SWORD_SPARKS
#undef LEGEND_BLADE_PICTURES
#undef LEGEND_SWORD_WIDTH
#undef LEGEND_SWORD_HEIGHT
#undef LEGEND_FLAME_GRADIENT
#undef LEGEND_FAN_FLAMES
#undef LEGEND_FAN_TILT
#undef LEGEND_FIRE_RING_GROWTH
#undef LEGEND_SWORD_INTERVAL
#undef LEGEND_SWORD_FALL_TICK
#undef LEGEND_SWORD_SOUND_TICK
#undef LEGEND_SWORD_STRIKE_TICK
#undef LEGEND_SWORD_FALL_TICKS
#undef LEGEND_SWORD_START_DISTANCE
#undef LEGEND_TICKS
#undef CARD_STREAK_WIDTH
#undef CARD_STREAK_HEIGHT
#undef ELEMENT_CARDS_PICTURES
#undef CARD_PIECES
#undef CARD_STRIKE_TIMERS
#undef CARD_SPARKS_PER_TARGET
#undef CARD_STRIKE_SPARKS
#undef CARD_RISE_TICKS
#undef CARD_SHRINK_TICK
#undef CARD_TILT
#undef CARD_GROUND_Y
#undef CARD_SHARD_TUMBLE_SPEED
#undef CARD_SHARD_TUMBLE_END
#undef CARD_STRIKES_FROM
#undef CARD_THUNDER_STRIKES_FROM
#undef CARD_BRAMBLE_STRIKES_FROM
#undef CARD_STRIKES_UNTIL
#undef CARD_BRAMBLE_STRIKES_UNTIL
#undef CARD_VINE_ANGLE
#undef CARD_VINE_SWAY
