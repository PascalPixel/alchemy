/* Odyssey, a cloud swirl nothing shows, Iris's screen wipe and teardown, and
   the shade ramps Iris's textures are filled with. Part of battle_effect.c. */

#define ODYSSEY_TICKS 114

/* The effect's pictures: GiantSwordSheet's sword at their start, then
   GiantSwordBladeSheet's top half of the colossal sword, then DustBlobSheet. */
#define ODYSSEY_SWORD_WIDTH			  40
#define ODYSSEY_SWORD_HEIGHT		  80
#define ODYSSEY_COLOSSAL_SWORD		  PICTURE_BYTES(ODYSSEY_SWORD)
#define ODYSSEY_COLOSSAL_SWORD_WIDTH  158
#define ODYSSEY_COLOSSAL_SWORD_HEIGHT 56
#define ODYSSEY_DUST_BLOB \
	(ODYSSEY_COLOSSAL_SWORD + PICTURE_BYTES(ODYSSEY_COLOSSAL_SWORD))

/* Where FlameGradientTexture is decoded in the map cell buffer, after
   FireRingTexture's 128 by 128 texels. */
#define ODYSSEY_FLAME_GRADIENT (128 * 128)

/* The shards the hit scatters, in the map cell buffer. */
#define ODYSSEY_SHARDS		((PARTICLE *)(RamMapMetatileBuffer + 24576))
#define ODYSSEY_SHARD_COUNT 128
/* The shards' canvas levels, eight from this one. */
#define ODYSSEY_SHARD_LEVEL 32

/* The two swords plunge onto the target ODYSSEY_SWORD_INTERVAL ticks apart
   from ODYSSEY_SWORD_TICK, the first from above and the second from below;
   each strikes ODYSSEY_SWORD_PLUNGE_TICKS later and stays until the colossal
   sword's blow. */
#define ODYSSEY_SWORDS			   2
#define ODYSSEY_SWORD_TICK		   24
#define ODYSSEY_SWORD_INTERVAL	   16
#define ODYSSEY_SWORD_PLUNGE_TICKS 8
#define ODYSSEY_SWORD_HIT_TICK	   (ODYSSEY_SWORD_TICK + ODYSSEY_SWORD_PLUNGE_TICKS)
/* The sparks each sword's hit scatters, in its half of the particles. */
#define ODYSSEY_SWORD_SPARKS	   16

/* The rings of light, FireRingTexture flashing out from the caster as the
   effect starts and from the target as each sword strikes. Each is drawn for
   ODYSSEY_RING_TICKS: it shines for ODYSSEY_RING_SHINE_TICKS, then fades 8
   levels a tick. */
#define ODYSSEY_RING_TICKS		 16
#define ODYSSEY_RING_SHINE_TICKS 8

/* From ODYSSEY_GATHER_TICK, two rings of light close in on the caster, each
   for ODYSSEY_GATHER_RING_TICKS, and the caster flashes white with each. */
#define ODYSSEY_GATHER_TICK		  64
#define ODYSSEY_GATHER_RING_TICKS 11

/* "Pierce a foe with a colossal sword." The red haze, turned blue, scrolls
   behind the caster and the target alone; two rings of light flash out from the
   caster, and two swords plunge onto the target, one from above and one from
   below, each with a ring and a burst of sparks. Rings close in on the caster,
   who charges (tick 84); the colossal sword sweeps across the screen (86) and
   pierces the target (90), scattering 128 tumbling shards, and from tick 100
   everything fades to white before the battle's own backdrop returns.

   Abilities: Odyssey. */
void btEffectRunOdyssey(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT			 to;
	SCREEN_POINT			 from;
	Sint16					 list[UNIT_MAX];
	DRAW_RECTANGLE			 draw[2];
	MODEL_TEXTURE			 texture;
	BATTLE_EFFECT_STATE		*state;
	void					*canvas;
	Uint8					*graphics;
	MOTION_OBJECT			*caster;
	BATTLE_BACKGROUND_STATE *transition;
	Sint32					 savedScrollX;
	RENDER_MODEL			*model;
	void					*projected;
	PARTICLE				*step;
	Sint32					 frame;
	Sint32					 i;
	Sint32					 k;
	Uint8					*pixels;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	graphics = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_GIANT_SWORD_SHEET, state->sheet, 0, 0);
	/* DEAD: DustBlobSheet, which Megiddo draws, is decoded and never drawn; the
	   ROM keeps the load. */
	btResourceLoadAndDecompress(
		RESOURCE_DUST_BLOB_SHEET, state->sheet + ODYSSEY_DUST_BLOB, 1, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	/* The blitter faces the caster's side. */
	if(args->side == EFFECT_SIDE_ENEMIES)
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	else
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	caster = btObjectGetStub(args->user)->object;
	transition = RamHeapSlots->battleBg;
	savedScrollX = BgScrollWork[1].x;

	for(frame = 0; frame != ODYSSEY_TICKS; frame++) {
		btEffectPosProjectScaledBattler(args->user, &from);
		btEffectPosProjectScaledBattler(args->targets[0], &to);

		if(frame == 0) {
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			btResourceLoadAndDecompress(
				RESOURCE_FIRE_RING_TEXTURE, MapGridMetatileBuffer, 0, 0);
			/* DEAD: FlameGradientTexture, which Megiddo draws, is decoded and
			   never drawn; the ROM keeps the load. */
			btResourceLoadAndDecompress(RESOURCE_FLAME_GRADIENT_TEXTURE,
				RamMapMetatileBuffer + ODYSSEY_FLAME_GRADIENT, 0, 0);
			btGraphicsLoadGreyRamp();
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			EFFECT_FREE_PARTICLES(
				i, state->particles, ARRAY_COUNT(state->particles));
		}

		/* Tick 2: only the caster and the target, over the red haze turned
		   blue:
		   each colour's red becomes its blue and half its green, and its blue
		   its red. */
		if(frame == 2) {
			Uint16 *palette;

			list[0] = args->user;
			list[1] = args->targets[0];
			list[2] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForListStub(list, 0);
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP, 0);
			btEffectBuildRepeatingTilemap();
			palette = (Uint16 *)&BG_PLTT_COLOR(6, 0);
			for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
				Sint32 entry = *palette;
				Sint32 red = RGB_LEVEL_MAX;
				Uint16 green;
				Sint32 blue;

				red &= entry;
				/* LEFTOVER(template): green, taken and clamped as
				 * btEffectRunCoatlicue and btEffectRunJudgment take all three
				 * channels in their palette loops, and left out of the colour
				 * here; as no green at all. */
				green = RGB_GREEN((Uint16)entry);
				blue = RGB_BLUE((Uint16)entry);
				red = red > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : red;
				green = green > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : green;
				blue = blue > RGB_LEVEL_MAX ? RGB_LEVEL_MAX : blue;
				*palette++ = RGB(blue, (Uint16)red >> 1, red);
			}
			transition->busy = 1;
		}
		if(frame > 1)
			BgScrollWork[1].x += 8;

		if(frame == 8) {
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			step = state->particles;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				step->x = syRandom() % 216 - 96;
				step->y = syRandom() % 216;
				step++;
			}
		}
		if(frame == 86)
			btResourceLoadAndDecompress(RESOURCE_GIANT_SWORD_BLADE_SHEET,
				state->sheet + ODYSSEY_COLOSSAL_SWORD, 1, 1);

		/* Tick 84: the caster charges at the target. */
		if(frame == 84) {
			sySoundPlayStub(SE_POWER_UP);
			fdActorSelectAnimationStub(caster, POSE_LEAP);
			fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 3);
			btEffectSetApproachMotion(args->user, args->targets[0], 7, 80);
		}

		/* Tick 90: the colossal sword's blow, and the shards scatter away from
		   the caster. */
		if(frame == 90) {
			PARTICLE *shard;

			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			sySoundPlayStub(SE_CLATTER);
			btEffectTintBattler(
				args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			state->shakeFrame = 8;
			shard = ODYSSEY_SHARDS;
			for(i = 0; i != ODYSSEY_SHARD_COUNT; i++) {
				Sint32 speed = (syRandom() & 0x1ff) + 32;
				Sint32 angle = syRandom() & 0x7fff;

				if(args->side == EFFECT_SIDE_PARTY)
					angle += DEGtoANG(180);
				shard->x = toFIXED(to.x / 2);
				shard->y = toFIXED(to.y);
				shard->velocityX = (sySin(angle) * speed) >> 6;
				shard->velocityY = -(syCos(angle) * speed) >> 5;
				shard->z = syRandom();
				shard->velocityZ = (syRandom() & 0x3ff) + toFIXED(1) / 64;
				shard->variant = (syRandom() & 0x3fff) + toFIXED(1) / 8;
				shard++;
			}
		}

		/* From tick 86: the colossal sword sweeps across the screen, drawn in
		   two halves, the lower one mirrored. */
		if(frame > 85) {
			Sint32 x;

			if(args->side == EFFECT_SIDE_ENEMIES) {
				x = 2872 - frame * 32;
				if(x < -38)
					x = -38;
			} else {
				x = frame * 32 - 2910;
				if(x > 0)
					x = 0;
			}
			draw[0](canvas, state->sheet + ODYSSEY_COLOSSAL_SWORD, x, 8,
				ODYSSEY_COLOSSAL_SWORD_WIDTH, ODYSSEY_COLOSSAL_SWORD_HEIGHT);
			if(args->side == EFFECT_SIDE_ENEMIES)
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
			draw[1] = RamHeapSlots->ramCodePaired;
			draw[1](canvas, state->sheet + ODYSSEY_COLOSSAL_SWORD, x, 64,
				ODYSSEY_COLOSSAL_SWORD_WIDTH, ODYSSEY_COLOSSAL_SWORD_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}

		/* Until tick 89: the sparks of the swords' hits, shrinking. */
		if(frame <= 89) {
			for(i = 0, step = state->particles;
				i != ARRAY_COUNT(state->particles); i++) {
				if(step->variant >= 0) {
					Sint32 size = (step->variant >> 2) + 1;

					draw[0](canvas,
						graphics + BattleEffectMotePictures[size - 1],
						FIXED_INTEGER_PART(step->x) - size / 2,
						FIXED_INTEGER_PART(step->y) - size, size, size * 2);
					btParticleAdvanceWithGravity2D(step, 60, 0);
					step->variant--;
				}
				step++;
			}
		}

		/* The two swords, the second flipped, each scattering its sparks as it
		   hits. */
		for(i = 0; i != ODYSSEY_SWORDS; i++) {
			Sint32 start = i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_TICK;

			if(frame ==
				i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_TICK +
					ODYSSEY_SWORD_PLUNGE_TICKS / 2)
				sySoundPlayStub(SE_POWER_UP);
			if(frame == i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_HIT_TICK) {
				for(k = 0; k != ODYSSEY_SWORD_SPARKS; k++) {
					PARTICLE *spark;
					Sint32	  speed = (syRandom() & 255) + 128;
					Sint32	  angle = syRandom() & 0x3fff;

					if(args->side == EFFECT_SIDE_ENEMIES) {
						if(i != 0)
							angle += DEGtoANG(90);
					} else if(i == 0) {
						angle -= DEGtoANG(90);
					} else {
						angle += DEGtoANG(180);
					}
					spark = &state->particles[i *
							(ARRAY_COUNT(state->particles) / ODYSSEY_SWORDS) +
						k];
					spark->x = toFIXED(to.x / 2);
					spark->y = toFIXED(to.y);
					spark->velocityX = (sySin(angle) * speed) >> 6;
					spark->velocityY = -(syCos(angle) * speed) >> 5;
					spark->variant = (syRandom() & 7) + 17;
				}
				sySoundPlayStub(SE_IMPACT);
				btEffectTintBattler(
					args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 64);
				state->shakeFrame = 4;
			}
			/* Each sword plunges 16 rows a tick, the first down to row 8 and
			   the second up to row 40, and stays there. */
			if(frame >= start && frame <= 89) {
				Sint32 age = frame - start;
				Sint32 x;
				Sint32 y;

				if(age <= ODYSSEY_SWORD_PLUNGE_TICKS) {
					if(args->side == EFFECT_SIDE_ENEMIES)
						x = to.x / 2 - age * 10 + 80;
					else
						x = to.x / 2 + age * 10 - 80;
					if(i == 0)
						y = age * 16 - 120;
					else
						y = 168 - age * 16;
				} else {
					/* DEAD: both arms alike; the ROM keeps the test of
					   effect->side. */
					if(args->side == EFFECT_SIDE_ENEMIES)
						x = to.x / 2;
					else
						x = to.x / 2;
					y = i == 0 ? 8 : 40;
				}
				if(i == 1) {
					if(args->side == EFFECT_SIDE_ENEMIES)
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
					else
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
					draw[1] = RamHeapSlots->ramCodePaired;
				}
				draw[i](canvas, state->sheet, x - ODYSSEY_SWORD_WIDTH / 2, y,
					ODYSSEY_SWORD_WIDTH, ODYSSEY_SWORD_HEIGHT);
				if(i == 1)
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}

		projected = syHeapAllocateEWRAM(32);
		model = btRenderAllocateModelList(1);
		pixels = MapGridMetatileBuffer;
		texture.logWidth = 7;
		texture.logHeight = 7;
		texture.pixels = pixels;
		model->mesh = BattleEffectLargeSquareMesh;
		model->kind = MODEL_SHADED_ADDITIVE;
		model->texture = &texture;
		model->vertices = projected;
		for(i = 0; i != 2; i++) {
			Sint32 start;

			/* Two rings of light flash out from the caster (ticks 2 and 8). */
			start = i * 6 + 2;
			if(frame == start)
				sySoundPlayStub(SE_POWER_UP);
			if(frame >= start && frame < i * 6 + 2 + ODYSSEY_RING_TICKS) {
				Sint32 age = frame - start;
				Sint32 shade = ODYSSEY_RING_SHINE_TICKS * 8 - age * 8;

				if(shade > 0)
					shade = 0;
				if(shade > MODEL_SHADE_INVISIBLE) {
					Sint32 scale = age * (toFIXED(1) / 4) + toFIXED(1) / 4;

					model->shade = shade;
					syInitMatrix();
					syTranslate(INTtoFIXED(from.x / 2 - 64), 0, 0);
					syScale(scale, scale * 2, scale);
					syRotX(OdysseyFlashPitches[i]);
					syRotY(OdysseyFlashYaws[i]);
					btRenderTransformVertices(
						BattleEffectSquareCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}

			/* Rings of light close in on the caster, who flashes white. */
			start = i * 8 + ODYSSEY_GATHER_TICK;
			if(frame == i * 8 + 68)
				sySoundPlayStub(SE_POWER_UP);
			if(frame == i * 8 + 72) {
				/* FAKEMATCH(temporary): none, 0 held in a local; as 0 at its
				 * uses. */
				Sint32 none = 0;

				btEffectTintBattler(
					args->user, SPRITE_COLOR_WHITE, TINT_KEEP, 0, none);
			} else if(frame == i * 8 + 76)
				btEffectTintBattler(
					args->user, SPRITE_COLOR_NORMAL, TINT_KEEP, 0, 0);
			if(frame >= start && frame < start + ODYSSEY_GATHER_RING_TICKS) {
				Sint32 scale = toFIXED(3) - (frame - start) * (toFIXED(1) / 4);

				model->shade = 0;
				syInitMatrix();
				syTranslate(INTtoFIXED(from.x / 2 - 64), 0, 0);
				syScale(scale, scale * 2, scale);
				syRotX(OdysseyFlashPitches[i]);
				syRotY(OdysseyFlashYaws[i]);
				btRenderTransformVertices(
					BattleEffectSquareCorners, projected, 4);
				btRenderDrawModelList(model);
			}

			/* A ring of light at the target with each sword's hit. */
			start = i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_HIT_TICK;
			if(frame >= start &&
				frame < i * ODYSSEY_SWORD_INTERVAL + ODYSSEY_SWORD_HIT_TICK +
						ODYSSEY_RING_TICKS) {
				Sint32 age = frame - start;
				Sint32 shade = ODYSSEY_RING_SHINE_TICKS * 8 - age * 8;

				if(shade > 0)
					shade = 0;
				if(shade > MODEL_SHADE_INVISIBLE) {
					FIXED scale = age * (toFIXED(1) / 4) + toFIXED(1) / 4;

					model->shade = shade;
					syInitMatrix();
					syTranslate(INTtoFIXED(to.x / 2 - 64), 0, 0);
					syScale(scale, scale * 2, scale);
					syRotX(OdysseyFlashPitches[i]);
					syRotY(OdysseyFlashYaws[i]);
					btRenderTransformVertices(
						BattleEffectSquareCorners, projected, 4);
					btRenderDrawModelList(model);
				}
			}
		}

		/* From tick 90: the shards, flat triangles in eight shades; from 100
		   the screen fades to white. */
		if(frame > 89) {
			Uint8  color[1];
			FIXED  center;
			Sint32 x;
			Sint32 y;

			model->mesh = BattleEffectTriangleMesh;
			model->kind = MODEL_SOLID_ADDITIVE;
			if(frame > 99) {
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(2));
				btPaletteBrightenBgEntries(2, 2, 2);
			}
			for(i = 0, step = ODYSSEY_SHARDS; i != ODYSSEY_SHARD_COUNT; i++) {
				color[0] = (i & 7) + ODYSSEY_SHARD_LEVEL;
				model->shade = (long)color;
				syInitMatrix();
				x = step->x;
				y = step->y;
				center = -toFIXED(64);
				syTranslate(x + center, y + center, 0);
				syScale(step->variant * 3, step->variant * 6, step->variant);
				syRotX(step->z);
				syRotY(step->z);
				btRenderTransformVertices(
					BattleEffectTriangleCorners, projected, 3);
				btRenderDrawModelList(model);
				btParticleAdvanceWithGravity3D(step, 62, 0);
				step++;
			}
		}
		syHeapRelease(model);
		syHeapRelease(projected);

		btCameraApplyVerticalShake(8);
		EFFECT_END_TICK(state);
	}

	/* The battle's own scene again. */
	btObjectPutStub();
	transition->busy = 0;
	BgScrollWork[1].x = savedScrollX;
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RamHeapSlots->battleState->bg,
		BACKGROUND_DARKEN_TICKS);
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* The swirl runs until A is pressed, at most this many ticks. */
#define SWIRL_MAX_TICKS 312000

/* The swirl's colours are BG palettes 6 on, the battle background's. */
#define SWIRL_FIRST_COLOR (6 * PALETTE_COLORS)

/* BG2 turns the swirl about this point of the screen, where the canvas's centre
   stays. */
#define SWIRL_CENTER_X (DISPLAY_WIDTH / 2)
#define SWIRL_CENTER_Y 72

/* BG2 shows the swirl at about twice its size: the steps of its matrix are the
   16.16 sine and cosine of its turn divided by this. */
#define SWIRL_STEP_DIVISOR 532

/* Unused: reached only from btEffectRunBackgroundFadeIn, which nothing refers
   to. A turning cloud swirl on BG2: the cloud sprite sheet is drawn once into
   the canvas in the battle background's colours and copied twice across the map
   buffer; then BG2 turns about the screen while BG1 scrolls, the palette
   flashes white at the start, steps toward BlueFlameSheet's colours every
   fourth tick and from tick 120 dims. B starts it over and A ends it. */
void btEffectRunCloudSwirl(BATTLE_EFFECT_PARM *args)
{
	BLITTER_PAIR			 sheetDraw;
	Sint32					 savedScrollX;
	Uint8					*canvas;
	BATTLE_EFFECT_STATE		*state;
	BATTLE_BACKGROUND_STATE *transition;
	DRAW_RECTANGLE			 draw;
	const Uint8				*palette;
	Sint32					 frame;
	Sint32					 enabled;
	FIXED					 sine;
	FIXED					 cosine;
	Sint32					 i;
	Uint8					*cells = RamMapMetatileBuffer;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	savedScrollX = BgScrollWork[1].x;
	transition = RamHeapSlots->battleBg;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(3));
	REG_DISPCNT = EFFECT_DISPCNT;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(0);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	/* The cloud sprite sheet, moved to the battle background's palettes (which
	   take EmberStreakSheet's colours), drawn into the canvas. */
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	sheetDraw[0] = RamHeapSlots->ramCode;
	palette = syResourceGetTableEntry(RESOURCE_CLOUD_SWIRL_SHEET);
	syWordsCopy((void *)&BG_PLTT_COLOR(6, 0), palette, SHEET_PALETTE_BYTES);
	palette += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(palette, state->sheet);
	palette = syResourceGetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
	syWordsCopy((void *)&BG_PLTT_COLOR(6, 0), palette, SHEET_PALETTE_BYTES);
	for(i = 0; i != CANVAS_BYTES; i++)
		state->sheet[i] += SWIRL_FIRST_COLOR;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	sheetDraw[0](canvas, state->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
	EFFECT_PRESENT_CANVAS(state);
	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);

	/* The swirl itself in the canvas (colour 0 made 1, so none is clear),
	   copied twice side by side into the map buffer. */
	btResourceLoadAndDecompress(RESOURCE_CLOUD_SWIRL_SHEET, canvas, 1, 1);
	for(i = 0; i != CANVAS_BYTES; i++) {
		if(canvas[i] == 0)
			canvas[i] = 1;
	}
	{
		/* One value is both the clip flags, EFFECT_CLIP_BOUNDS, and the mode,
		   BLITTER_COPY. */
		Sint32 clipAndCopy = BLITTER_COPY;

		enabled = 1;
		btEffectBuildBlitter(
			HEAP_SLOT_RAM_CODE, 8, 7, clipAndCopy, clipAndCopy);
	}
	draw = RamHeapSlots->ramCode;
	sySetEvent(btEffectTransferFullScreenImage, EVENT_PASS_VBLANK);
	transition->busy = enabled;
	draw(cells, canvas, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
	draw(cells, canvas, CANVAS_SIZE, 0, CANVAS_SIZE, CANVAS_SIZE);
	state->transferPending = enabled;
	sySynch(1);

	for(frame = 0; frame != SWIRL_MAX_TICKS; frame++) {
		if(PerWork.repeat & KEY_B)
			frame = 0;
		if(PerWork.repeat & KEY_A)
			break;

		if(frame == 0)
			syWordsFillEntry(
				(void *)BG_PLTT, SHEET_PALETTE_BYTES, COLOR_PAIR(RGB_WHITE));
		if(frame > 119)
			btPaletteBrightenBgEntries(-1, -1, -1);
		else if((frame & 3) == 1)
			btEffectStepPaletteToResource(RESOURCE_BLUE_FLAME_SHEET);
		BgScrollWork[1].x = frame / 2;

		/* BG2 turns a 1024th of a turn a tick. Its steps are 8.8, so the 16.16
		   centre is scaled by Q_8_8(1) for the products to come out 16.16. */
		sine = sySin(frame * (TRIG_TURN / 1024)) / SWIRL_STEP_DIVISOR;
		cosine = syCos(frame * (TRIG_TURN / 1024)) / SWIRL_STEP_DIVISOR;
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PA, cosine, IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PB, sine, IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PC, -sine, IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PD, cosine, IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X,
			(syMulFX(toFIXED(-SWIRL_CENTER_X) * Q_8_8(1), cosine) +
				syMulFX(toFIXED(-SWIRL_CENTER_Y) * Q_8_8(1), sine) +
				toFIXED(CANVAS_SIZE / 2)) >>
				8,
			IO_WRITE_WORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2Y,
			(syMulFX(toFIXED(-SWIRL_CENTER_X) * Q_8_8(1), -sine) +
				syMulFX(toFIXED(-SWIRL_CENTER_Y) * Q_8_8(1), cosine) +
				toFIXED(CANVAS_SIZE / 2)) >>
				8,
			IO_WRITE_WORD);
		sySynch(1);
	}

	BgScrollWork[1].x = savedScrollX;
	transition->busy = 0;
	syCloseEvent(btEffectTransferFullScreenImage);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	btEffectEndCanvasLayer();
}

/* The wipe's band slides this far along BG2, from just off one edge of the
   screen until it has passed the other. */
#define WIPE_DISTANCE	192
/* The band brightens over this many columns at its left and dims over as many
   at its right. */
#define WIPE_EDGE_WIDTH 32

/* Iris's screen wipe: a soft-edged white band, drawn once into the canvas and
   stretched across BG2, slides over the screen from the right (side 0, a
   positive speed) or from the left (side 1, a negative speed), speed of its
   pixels a tick. Halfway through, while the band covers the screen, it calls
   midpoint to change the scene behind it. */
void btIrisWipeScreen(Uint8 side, Uint8 speed, void (*midpoint)(void))
{
	Sint32				 scrollX;
	Uint8				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint16				 ramp[CANVAS_LEVEL_BRIGHTEST];
	Sint32				 half;
	Sint32				 y;
	Sint32				 i;

	scrollX = (side & 1) * WIPE_DISTANCE - 64;
	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	/* CANVAS_COPY does not read the share. */
	EFFECT_SET_TRANSFER(state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
	REG_BG2PA = Q_8_8(0.28125); /* about 3.6 times as wide */

	/* The band, column i: rising to the brightest level over the left edge, the
	   brightest through the middle and falling again over the right. */
	for(i = 0; i != CANVAS_SIZE; i++) {
		for(y = 0; y != CANVAS_SIZE; y++) {
			Sint32 tileX = i & 7;
			Sint32 color;
			Sint32 columnOffset;

			if(i < WIPE_EDGE_WIDTH)
				color = i * 2;
			else {
				color = CANVAS_LEVEL_BRIGHTEST -
					((i - (CANVAS_SIZE - WIPE_EDGE_WIDTH)) << 1);
				if(i < CANVAS_SIZE - WIPE_EDGE_WIDTH)
					color = CANVAS_LEVEL_BRIGHTEST;
			}
			columnOffset = tileX + (i >> 3) * CANVAS_TILE_BYTES;
			canvas[columnOffset + ((y & 7) << 3) + ((y >> 3) << 10)] = color;
		}
	}

	/* Colours 1 to 63: a grey ramp from black to white. */
	for(i = 0; i != CANVAS_LEVEL_BRIGHTEST; i++) {
		Sint32 level = i >> 1;

		ramp[i] = RGB(level, level, level);
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(0, 1), (Uint32)ramp,
		DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));

	REG_BG2X = scrollX << 8;
	/* y, done with the band, now counts the slide's ticks. */
	y = WIPE_DISTANCE / ((Sint8)speed < 0 ? -(Sint8)speed : (Sint8)speed) + 1;
	half = (y - 1) >> 1;
	for(i = 0; i != y; i++) {
		scrollX += (Sint8)speed;
		REG_BG2X = scrollX << 8;
		EFFECT_PRESENT_CANVAS(state);
		if(i == half && midpoint != NULL)
			midpoint();
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
}

/* Unused: sets the colour mode of each running animation layer of a sprite and
   marks it for redrawing, as fdActorSetSpriteColor does for an actor's. */
UNUSED void btSpriteSetColorMode(SPR *sprite, Sint32 mode)
{
	Sint32 i;

	for(i = 0; i < sprite->count; i++) {
		SPR_ANIMATION *part = sprite->entries[i];

		if(part != NULL && part->script != NULL)
			part->colorMode = mode;
	}
	sprite->dirty = 1;
}

/* Unused: nothing calls it. Fills count bytes of table with a rising shade ramp
   (0 to CANVAS_LEVEL_BRIGHTEST, by a step scaled by scale), then repeats it
   seven more times. */
UNUSED void btEffectBuildShadeRamp(Uint8 *table, Uint32 count, Sint32 scale)
{
	Uint32 step = count * 2;
	Sint32 total = 0;
	Uint32 i;
	Sint32 j;

	if(step > CANVAS_LEVEL_BRIGHTEST)
		step = CANVAS_LEVEL_BRIGHTEST;
	step = CANVAS_LEVEL_BRIGHTEST / step;
	if(step < 6)
		step = 6;
	for(i = 0; i != count; i++) {
		Sint32 value;

		total += step;
		value = FIXEDtoINT(scale * total);
		if(value > CANVAS_LEVEL_BRIGHTEST)
			value = CANVAS_LEVEL_BRIGHTEST;
		table[i] = value;
	}
	for(j = 1; j != 8; j++) {
		for(i = 0; i != count; i++)
			table[j * count + i] = table[i];
	}
}

/* Unused: nothing calls it. Fills table with a shade ramp that rises over count
   bytes and falls back, repeated repeats times. */
UNUSED void btEffectBuildShadeRampWave(
	Uint8 *table, Uint32 count, Uint32 repeats, Sint32 scale)
{
	Sint32 total = 0;
	Uint32 step = count * 2;
	Uint32 i;
	Uint32 j;

	if(step > CANVAS_LEVEL_BRIGHTEST)
		step = CANVAS_LEVEL_BRIGHTEST;
	step = CANVAS_LEVEL_BRIGHTEST / step;
	if(step < 6)
		step = 6;
	for(i = 0; i != count; i++) {
		Sint32 value;

		total += step;
		value = FIXEDtoINT(scale * total);
		if(value > CANVAS_LEVEL_BRIGHTEST)
			value = CANVAS_LEVEL_BRIGHTEST;
		table[i] = value;
	}
	for(i = 0; i != count; i++)
		table[count + i] = table[count - 1 - i];
	for(j = 1; j != repeats; j++) {
		for(i = 0; i != count * 2; i++)
			table[j * count * 2 + i] = table[i];
	}
}

/* As btEffectBuildShadeRampWave, at twice the step. */
void btEffectBuildSteepShadeRampWave(
	Uint8 *table, Uint32 count, Uint32 repeats, Sint32 scale)
{
	Sint32 total = 0;
	Uint32 step = count * 2;
	Uint32 i;
	Uint32 j;

	if(step > CANVAS_LEVEL_BRIGHTEST)
		step = CANVAS_LEVEL_BRIGHTEST;
	step = CANVAS_LEVEL_BRIGHTEST / step;
	if(step < 6)
		step = 6;
	step *= 2;
	for(i = 0; i != count; i++) {
		Sint32 value;

		total += step;
		value = FIXEDtoINT(scale * total);
		if(value > CANVAS_LEVEL_BRIGHTEST)
			value = CANVAS_LEVEL_BRIGHTEST;
		table[i] = value;
	}
	for(i = 0; i != count; i++)
		table[count + i] = table[count - 1 - i];
	for(j = 1; j != repeats; j++) {
		for(i = 0; i != count * 2; i++)
			table[j * count * 2 + i] = table[i];
	}
}

/* Iris's teardown: sets each target back on the ground at the battle origin
   with the usual bounce, frees the battle background, centres the projection
   again, reloads the sprites' basic colours and brings back the battle's own
   backdrop. */
void btIrisRestoreBattle(void)
{
	BATTLE_EFFECT_STATE		*state = RamHeapSlots->battleEffect;
	BATTLE_BACKGROUND_STATE *transition = RamHeapSlots->battleBg;
	BATTLE_STATE			*battle = RamHeapSlots->battleState;
	Sint32					 i;

	for(i = 0; i != state->args->targetCount; i++) {
		MOTION_OBJECT *object =
			btObjectGetStub(state->args->targets[i])->object;

		SET_POSITION(object, 0, toFIXED(120), 0);
		object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
		btMotionApplyKnockbackStub(state->args->targets[i], KNOCKBACK_HOP);
	}
	transition->busy = 0;
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	syPaletteLoadBasicSpriteColors();
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKEN_TICKS);
}

#undef ODYSSEY_TICKS
#undef ODYSSEY_SWORD_WIDTH
#undef ODYSSEY_SWORD_HEIGHT
#undef ODYSSEY_COLOSSAL_SWORD
#undef ODYSSEY_COLOSSAL_SWORD_WIDTH
#undef ODYSSEY_COLOSSAL_SWORD_HEIGHT
#undef ODYSSEY_DUST_BLOB
#undef ODYSSEY_FLAME_GRADIENT
#undef ODYSSEY_SHARDS
#undef ODYSSEY_SHARD_COUNT
#undef ODYSSEY_SHARD_LEVEL
#undef ODYSSEY_SWORDS
#undef ODYSSEY_SWORD_TICK
#undef ODYSSEY_SWORD_INTERVAL
#undef ODYSSEY_SWORD_PLUNGE_TICKS
#undef ODYSSEY_SWORD_HIT_TICK
#undef ODYSSEY_SWORD_SPARKS
#undef ODYSSEY_RING_TICKS
#undef ODYSSEY_RING_SHINE_TICKS
#undef ODYSSEY_GATHER_TICK
#undef ODYSSEY_GATHER_RING_TICKS
#undef SWIRL_MAX_TICKS
#undef SWIRL_FIRST_COLOR
#undef SWIRL_CENTER_X
#undef SWIRL_CENTER_Y
#undef SWIRL_STEP_DIVISOR
#undef WIPE_DISTANCE
#undef WIPE_EDGE_WIDTH
