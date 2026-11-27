/* Star Mine, the enemies' Rising Dragon and the backdrop it scrolls, then the
   entry points of the rushing strikes. Part of battle_effect.c. */

#define STAR_MINE_TICKS			 144
#define STAR_MINE_STARS			 6
/* The flash bursts' slots in state->particles after the stars, eight for each
   star, and the sparks', in PARTICLES. */
#define STAR_MINE_FLASH_BURSTS	 48
#define STAR_MINE_SPARKS		 192
/* What a star's speed gains toward its target each tick: 1/1024 of the
   distance between them. */
#define STAR_MINE_PULL(distance) ((distance) >> 10)

/* Where the two stars are built in the effect's pictures, each a 24 by 48
   picture mirrored from one half of BlueCrescentSheet's. */
#define STAR_MINE_STAR_PICTURES		   4608
#define STAR_WIDTH					   24
#define STAR_HEIGHT					   48
/* The bytes of a star's source in BlueCrescentSheet, 24 by 24: each row's even
   pixels are the right half of the star's row, its odd ones the left. */
#define STAR_HALF					   (PICTURE_BYTES(STAR) / 2)
/* The flash bursts and the sparks each star bursts into. */
#define STAR_BURST_FLASH_BURSTS		   4
#define STAR_BURST_SPARKS			   (STAR_MINE_SPARKS / STAR_MINE_STARS)
/* Where FlashBurstSheet is decoded in the effect's pictures. */
#define STAR_MINE_FLASH_BURST_PICTURES 16384
/* A flash burst plays FlashBurstSheet's pictures, a flash turning to a fireball
   that burns out, FLASH_BURST_PICTURE_TICKS ticks apiece; its first two
   ticks are not drawn. */
#define FLASH_BURST_PICTURE_TICKS	   4

/* Six twinkling stars (a 24 by 48 picture mirrored both ways from the sprite
   sheet's two halves) are thrown up from the caster, each at a target in turn;
   from tick 8 * i + 11 each is pulled toward its target and bursts once it has
   passed it, into four flash bursts and 32 sparks, knocking the target back.

   Abilities: Star Mine. */
void btEffectRunStarMine(BATTLE_EFFECT_PARM *args)
{
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frm;
	Uint8				*sprs;
	DRAW_RECTANGLE		 blitters[2];
	SCREEN_POINT		 screenPos;
	PARTICLE			*star;
	PARTICLE			*particle;
	PARTICLE			*spark;
	BATTLE_CAMERA		*camera;
	Sint32				 delay;
	Sint32				 member;
	Sint32				 firstSpark;
	Sint32				 i;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	sprs = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, blitters);
	btResourceLoadAndDecompress(
		RESOURCE_BLUE_CRESCENT_SHEET, state->sheet, 1, 1);

	/* The star: each half of the sprite sheet's 24-wide picture mirrored left
	   to right and top to bottom. */
	for(i = 0; i != 2; i++) {
		Uint8 *picture =
			state->sheet + STAR_MINE_STAR_PICTURES + i * PICTURE_BYTES(STAR);
		Sint32 y;

		for(y = 0; y != STAR_HEIGHT / 2; y++) {
			Sint32 x;

			for(x = 0; x != STAR_WIDTH / 2; x++) {
				picture[y * STAR_WIDTH + x] =
					state->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2 + 1];
				picture[y * STAR_WIDTH + STAR_WIDTH - 1 - x] =
					state->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2];
				picture[(STAR_HEIGHT - 1 - y) * STAR_WIDTH + x] =
					state->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2 + 1];
				picture[(STAR_HEIGHT - 1 - y) * STAR_WIDTH + STAR_WIDTH - 1 -
					x] = state->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2];
			}
		}
	}
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + STAR_MINE_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
		SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != STAR_MINE_TICKS; frm++) {
		camera = Ram_HeapSlots->cameraState;
		if(frm == 0) {
			PARTICLE *thrown;

			for(i = 0, thrown = state->particles; i != STAR_MINE_STARS;
				i++, thrown++) {
				ACTOR *caster = btObjectGetStub(args->user)->obj;
				Sint32 height = btUnitGetEffectHeightStub(args->user) / 2;
				Sint32 vel;

				SET_POSITION(thrown, caster->x.fixed, caster->y.fixed + height,
					caster->z.fixed);
				vel = ((syRandom() & 255) - 128) << 8;
				/* DEAD: both stores are overwritten by the 0 below; the ROM
				   keeps them. */
				thrown->velX = vel;
				if(thrown->x > 0) {
					thrown->velX = -vel;
				}
				thrown->velX = 0;
				thrown->velY = ((syRandom() % 400) - 128) << 9;
				thrown->velZ = ((syRandom() & 255) - 128) << 10;
				thrown->variant = 0;
			}
			for(i = 0; i != STAR_MINE_FLASH_BURSTS; i++) {
				state->particles[STAR_MINE_STARS + i].variant = -1;
			}
			EFFECT_FREE_PARTICLES(i, PARTICLES, STAR_MINE_SPARKS);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);

		for(i = 0, star = state->particles, delay = 0, firstSpark = 0;
			i != STAR_MINE_STARS;
			star++, delay += 8, firstSpark += STAR_BURST_SPARKS, i++) {
			Sint32 k;
			ACTOR *tgt;
			Sint32 height;
			Sint32 picture;
			Sint32 dx;
			Sint32 dy;
			Sint32 dz;
			Sint32 starX;
			FIXED  tgtX;

			member = i % args->tgtCnt;
			tgt = btObjectGetStub(args->tgts[member])->obj;
			height = btUnitGetEffectHeightStub(args->tgts[member]) / 2;
			if(star->variant < 0) {
				continue;
			}
			picture = ((frm + i) / 2) & 1;
			btEffectPosProjectPoint(star, &screenPos);
			screenPos.x >>= 1;
			blitters[0](canvas,
				state->sheet + STAR_MINE_STAR_PICTURES +
					picture * PICTURE_BYTES(STAR),
				screenPos.x - STAR_WIDTH / 2, screenPos.y - STAR_HEIGHT / 2,
				STAR_WIDTH, STAR_HEIGHT);
			btParticleAdvanceWithGravity3D(star, 63, 0);
			if(frm <= delay + 10) {
				continue;
			}
			dx = STAR_MINE_PULL(tgt->x.fixed - star->x);
			dy = STAR_MINE_PULL(tgt->y.fixed + height - star->y);
			dz = STAR_MINE_PULL(tgt->z.fixed - star->z);
			star->velX += dx;
			star->velY += dy;
			star->velZ += dz;
			/* Until it has passed its target, farther from the field's
			   middle. */
			starX = star->x;
			if(starX < 0) {
				starX = -starX;
			}
			tgtX = tgt->x.fixed;
			if(tgtX < 0) {
				tgtX = -tgtX;
			}
			if(starX <= tgtX) {
				continue;
			}

			/* Passed its target: the burst. */
			for(k = 0; k != STAR_BURST_FLASH_BURSTS; k++) {
				particle = &state->particles[STAR_MINE_STARS + delay + k];
				particle->x = toFIXED(screenPos.x);
				particle->y = toFIXED(screenPos.y);
				particle->velX = ((syRandom() & 127) - 64) << 12;
				particle->velY = (192 - (syRandom() & 255)) << 10;
				particle->variant = 0;
			}
			for(k = 0; k != STAR_BURST_SPARKS; k++) {
				Sint32 ang;
				Sint32 speed;

				spark = &PARTICLES[firstSpark + k];
				spark->x = toFIXED(screenPos.x);
				spark->y = toFIXED(screenPos.y);
				ang = (syRandom() & 0x7fff) + DEGtoANG(90);
				speed = (syRandom() & 255) + 127;
				spark->velX = (sySin(ang) * speed) >> 5;
				spark->velY = (syCos(ang) * speed) >> 5;
				spark->variant = (syRandom() & 15) + 16;
			}
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			sySndPlayStub(SE_CRASH);
			btEffectStartActorMotion(args->tgts[member], 1, toFIXED(2),
				toFIXED(8), toFIXED(2), i * 5 + 110);
			btEffectTintBattler(args->tgts[member], SPRITE_COLOR_WHITE,
				POSE_DAMAGE, member, 16);
			state->shakeFrm = 4;
			star->variant = -1;
		}

		for(i = 0; i != STAR_MINE_FLASH_BURSTS; i++) {
			particle = &state->particles[STAR_MINE_STARS + i];
			if((Uint32)particle->variant <
				FLASH_BURST_PICTURE_COUNT * FLASH_BURST_PICTURE_TICKS) {
				if(particle->variant > 1) {
					blitters[0](canvas,
						state->sheet + STAR_MINE_FLASH_BURST_PICTURES +
							(particle->variant / FLASH_BURST_PICTURE_TICKS) *
								PICTURE_BYTES(FLASH_BURST),
						FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(particle->y) -
							FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
				}
				btParticleAdvanceWithGravity2D(particle, 60, -toFIXED(1) / 16);
				particle->variant++;
			}
		}
		for(i = 0; i != STAR_MINE_SPARKS; i++) {
			particle = &PARTICLES[i];
			if(particle->variant >= 0) {
				Sint32 size = (particle->variant >> 3) + 1;

				blitters[0](canvas, sprs + BattleEffectMotePictures[size - 1],
					FIXED_INTEGER_PART(particle->x) - size / 2,
					FIXED_INTEGER_PART(particle->y) - size, size, size * 2);
				btParticleAdvanceWithGravity2D(particle, 60, toFIXED(1) / 16);
				particle->variant--;
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

/* The battle background's image in BG1's flat map: BACKDROP_ROWS rows of
   BG_SCREEN_WIDTH tiles from BACKDROP_FIRST_TILE. */
#define BACKDROP_FIRST_TILE 512
#define BACKDROP_ROWS		15
/* The first tile of the picture's right half. */
#define BACKDROP_RIGHT_HALF (BACKDROP_FIRST_TILE + BG_SCREEN_WIDTH / 2)

/* Lays BG1's map with the right half of the battle background's image, 16 tiles
   by BACKDROP_ROWS, twice across and twice down, so that the backdrop scrolls
   on without end. */
void btEffectBuildRepeatingTilemap(void)
{
	Uint16 *dst = (Uint16 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
	Sint32	y;
	Sint32	x;

	for(y = 0; y != 2 * BACKDROP_ROWS; y++) {
		if(y >= BACKDROP_ROWS) {
			for(x = 0; x != BG_SCREEN_WIDTH; x++)
				*dst++ = (y - BACKDROP_ROWS) * BG_SCREEN_WIDTH + (x & 15) +
					BACKDROP_RIGHT_HALF;
		} else {
			for(x = 0; x != BG_SCREEN_WIDTH; x++)
				*dst++ = y * BG_SCREEN_WIDTH + (x & 15) + BACKDROP_RIGHT_HALF;
		}
	}
}

/* Two corners of a row of the strip. */
typedef struct CORNER_ROW {
	CORNER corners[2];
} CORNER_ROW;

/* The dragon's buffers in the map cell buffer: its strip's corners, their
   transformed vertices, the strip mesh, and the sparks, in EXTRA_PARTICLES: the
   first half thrown up at the start, the second the trail behind the dragon,
   and all of them scattered from its top at tick 134. */
#define RISING_DRAGON_ENEMY_VERTICES	4096
#define RISING_DRAGON_ENEMY_STRIP		8192
#define RISING_DRAGON_ENEMY_SPARK_COUNT 256
#define TRAIL_SPARKS					(RISING_DRAGON_ENEMY_SPARK_COUNT / 2)

/* The dragon's body: a strip of RISING_DRAGON_ENEMY_SEGMENTS rows,
   RISING_DRAGON_ENEMY_BODY_WIDTH across, each row swaying
   RISING_DRAGON_ENEMY_ROW_PHASE further along than the one before, the sway
   turning a 64th of a turn a tick; the strip is drawn at
   RISING_DRAGON_ENEMY_BODY_SCALE. */
#define RISING_DRAGON_ENEMY_SEGMENTS   33
#define RISING_DRAGON_ENEMY_BODY_WIDTH 20
#define RISING_DRAGON_ENEMY_ROW_PHASE  2018
#define RISING_DRAGON_ENEMY_BODY_SCALE toFIXED(2.09156)
#define RISING_DRAGON_ENEMY_SWAY_STEP  (TRIG_TURN / 64)

/* Where FlashBurstSheet is decoded in the effect's pictures. */
#define RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES	   2048
/* Where ClawSlashSheet, a dragon's head of flame, is decoded in the effect's
   pictures. */
#define RISING_DRAGON_ENEMY_FLAME_DRAGON_HEAD_PICTURES 16384
/* The flash bursts at the top of the dragon from tick 134:
   RISING_DRAGON_ENEMY_BURSTS_A_TICK a tick for
   RISING_DRAGON_ENEMY_BURST_TICKS. */
#define RISING_DRAGON_ENEMY_BURST_TICKS				   8
#define RISING_DRAGON_ENEMY_BURSTS_A_TICK			   4
#define RISING_DRAGON_ENEMY_FLASH_BURSTS \
	(RISING_DRAGON_ENEMY_BURST_TICKS * RISING_DRAGON_ENEMY_BURSTS_A_TICK)
/* How many lines a tick the sky scrolls past as the dragon climbs. */
#define RISING_DRAGON_ENEMY_HAZE_SPEED	8
/* The level the canvas is cleared to each tick of the whitening from tick
   152, until its last five ticks turn it CANVAS_LEVEL_BRIGHTEST. */
#define RISING_DRAGON_ENEMY_FLASH_LEVEL 32

#define RISING_DRAGON_ENEMY_TICKS 186

/* A dragon of flame bursts up under the target in a shower of sparks and
   carries it off in its jaws, into a teal sky (the red haze, its colours
   turned) that scrolls past as it climbs, swaying. At tick 134 the dragon's
   top bursts into flashes and sparks, and from tick 152 the screen whitens; at
   tick 166 the battle's own background is back and the target drops to the
   field, landing at 179.

   Abilities: Rising Dragon (the enemies'). */
void btEffectRunRisingDragonEnemy(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*vertices;
	Uint8				*strip;
	CORNER_ROW			*cells;
	DRAW_RECTANGLE		 blit[2];
	Uint8				*sprs;
	MOTION_OBJECT		*victim;
	BATTLE_CAMERA		*camera;
	Sint16				 savedYaw;
	Sint32				 height;
	Sint32				 savedMotionStrength;
	Sint32				 savedX;
	Sint32				 savedZ;
	SCREEN_POINT		 source;
	SCREEN_POINT		 tgt;
	MODEL_TEXTURE		 texture;
	RENDER_MODEL		*model;
	/* FAKEMATCH(temporary): zero, 0 held in a local; as 0 at its uses. */
	Sint32	  zero;
	Sint32	  burstFrm;
	PARTICLE *spawned;
	PARTICLE *particle;
	PARTICLE *scatter;
	Sint32	  frm;
	Sint32	  i;
	Sint32	  j;

	vertices = Ram_MapMetatileBuffer + RISING_DRAGON_ENEMY_VERTICES;
	strip = Ram_MapMetatileBuffer + RISING_DRAGON_ENEMY_STRIP;
	cells = (CORNER_ROW *)Ram_MapMetatileBuffer;
	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprs = Ram_HeapSlots->battleSheet;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = Ram_HeapSlots->ramCode;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	victim = btObjectGetStub(args->tgts[0])->obj;
	camera = Ram_HeapSlots->cameraState;
	savedYaw = camera->yaw;
	height = 0;
	savedMotionStrength = victim->verticalMotionStrength;
	savedX = victim->x;
	savedZ = victim->z;
	btEffectPosProjectBattler(args->tgts[0], &source);
	btEffectPosProjectBattler(args->tgts[0], &tgt);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
		state->sheet + RISING_DRAGON_ENEMY_FLAME_DRAGON_HEAD_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	REG_BG2PA = Q_8_8(0.5);
	btRenderBuildQuadStrip(strip, 4, 32, RISING_DRAGON_ENEMY_SEGMENTS - 1);

	for(frm = 0; frm != RISING_DRAGON_ENEMY_TICKS; frm++) {
		if(frm == 0) {
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
				SHEET_PALETTE_BYTES);
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				state->particles[i].variant = -1;
			}
			for(i = 0; i != TRAIL_SPARKS; i++) {
				EXTRA_PARTICLES[TRAIL_SPARKS + i].variant = -1;
			}
			for(i = 0, spawned = EXTRA_PARTICLES; i != TRAIL_SPARKS;
				i++, spawned++) {
				Sint32 ang;
				Sint32 speed;

				spawned->x = toFIXED(source.x / 2);
				spawned->y = toFIXED(120);
				ang = (syRandom() & 0x7fff) + DEGtoANG(90);
				speed = (syRandom() & 1023) + 32;
				spawned->velX = (sySin(ang) * speed) >> 6;
				spawned->velY = (syCos(ang) * speed) >> 6;
				spawned->variant = (syRandom() & 7) + 32;
			}
		}
		if(frm == 4) {
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 2);
			state->shakeFrm = 8;
			sySndPlayStub(SE_CRASH);
		}
		if(frm == 163) {
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		}
		if(frm == 164) {
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
				Ram_HeapSlots->battleState->bg, BACKGROUND_DARKEN_TICKS);
		}
		if(frm == 165) {
			ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
				SHEET_PALETTE_BYTES);
		}
		if(frm == 166) {
			camera->yaw = savedYaw;
			victim->verticalMotionStrength = savedMotionStrength;
			victim->x = savedX;
			victim->y = toFIXED(100);
			victim->z = savedZ;
			BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
		}
		if(frm == 179) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			state->shakeFrm = 8;
		}
		if(frm > 165) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
		} else {

			if(frm == 59) {
				syWordsFillEntry(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			if(frm == 60) {
				Uint16 *col;
				Uint16	channelMax = RGB_LEVEL_MAX;

				/* Into the sky: the red haze, each colour's blue made its red
				   and its red its green and blue, which turns it teal. */
				btBackgroundLoadStub(
					BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP, 0);
				col = (Uint16 *)&BG_PLTT_COLOR(6, 0);
				for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++, col++) {
					Sint32 entry = *col;
					Sint32 red;
					Sint32 green;
					Sint32 blue;

					red = channelMax & entry;
					/* LEFTOVER(template): green, taken and clamped as
					 * btEffectRunCoatlicue and btEffectRunJudgment take all
					 * three channels in their palette loops, and left out of
					 * the colour here; as no green at all. */
					green = RGB_GREEN((Uint16)entry);
					blue = RGB_BLUE((Uint16)entry);
					/* DEAD: red and blue are already at most 31; the ROM keeps
					   both compares. */
					if(red > RGB_LEVEL_MAX) {
						red = RGB_LEVEL_MAX;
					}
					if(green > RGB_LEVEL_MAX) {
						green = RGB_LEVEL_MAX;
					}
					if(blue > RGB_LEVEL_MAX) {
						blue = RGB_LEVEL_MAX;
					}
					*col = RGB(blue, red, red);
				}
				btEffectBuildRepeatingTilemap();
				BgScrollWork[1].y = BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP;
				ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
				if(args->side == EFFECT_SIDE_ENEMIES) {
					camera->yaw = DEGtoANG(90);
				} else {
					camera->yaw = TRIG_TURN * 3 / 4;
				}
				for(i = 0; i != RISING_DRAGON_ENEMY_SPARK_COUNT; i++) {
					EXTRA_PARTICLES[i].variant = -1;
				}
				height = 220;
			}
			if(frm == 134) {
				sySndPlayStub(SE_CRASH);
				syWordsFillEntry(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			if(frm > 59) {
				Sint32 scroll = BgScrollWork[1].y;
				Sint32 climb;

				/* A scroll above the haze's range is a negative one, read back
				   from its halfword. */
				if(scroll > BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP) {
					scroll -= BG_SCROLL_WRAP;
				}
				scroll -= RISING_DRAGON_ENEMY_HAZE_SPEED;
				if(scroll < -BATTLE_VIEW_TOP) {
					scroll += BATTLE_BACKGROUND_HEIGHT;
				}
				BgScrollWork[1].y = scroll;
				climb = 4 - (frm - 60) / 8;
				if(climb < 0) {
					climb = 0;
				}
				if(frm > 119) {
					climb = frm - 120;
				}
				height -= climb;
			} else {
				height = 200 - frm * 8;
			}
			if(height <= 55) {
				height = 56;
			}
			victim->verticalMotionStrength = 0;
			if((Uint32)(frm - 4) <= 55) {
				victim->y = toFIXED(frm * 9) - toFIXED(18);
			} else if(frm > 59) {
				victim->x = 0;
				victim->y = toFIXED(294 - height);
				victim->z = -(sySin(frm * RISING_DRAGON_ENEMY_SWAY_STEP) * 8);
				if(frm == 60) {
					btEffectPosProjectBattler(args->tgts[0], &source);
				}
				btEffectPosProjectBattler(args->tgts[0], &tgt);
			}
			if((Uint32)(frm - 152) <= 15) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
					CANVAS_FILL_WORD(RISING_DRAGON_ENEMY_FLASH_LEVEL));
				if(frm > 162) {
					state->transferValue =
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
				}
				btPaletteBrightenBgEntries(2, 2, 2);
			}
			/* DEAD: tick is never negative; the ROM keeps the compare and
			   branch. */
			if(frm >= 0) {

				/* The dragon's body, swaying. */
				for(j = 0; j != RISING_DRAGON_ENEMY_SEGMENTS; j++) {
					for(i = 0; i != 2; i++) {
						(cells[j].corners + i)->x =
							i * RISING_DRAGON_ENEMY_BODY_WIDTH +
							(FIXEDtoINT(
								sySin(j * RISING_DRAGON_ENEMY_ROW_PHASE +
									frm * RISING_DRAGON_ENEMY_SWAY_STEP) *
								2)) -
							RISING_DRAGON_ENEMY_BODY_WIDTH / 2;
						(cells[j].corners + i)->z = 0;
						(cells[j].corners + i)->y = j * 2 - 64;
					}
				}
				zero = 0;
				model = btRenderAllocateModelList(1);
				model->kind = MODEL_SHADED_MAX;
				model->shade = zero;
				model->scrollV = zero;
				model->vertices = vertices;
				model->texture = &texture;
				texture.logWidth = 6;
				texture.logHeight = 5;
				texture.pixels = state->sheet;
				model->mesh = strip;
				model->scrollU = TEXTURE_SCROLL_MASK & ~(frm << 2);
				syInitMatrix();
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syPushMatrix();
				syTranslate(INTtoFIXED(source.x - 128), toFIXED(height), 0);
				if(args->side == EFFECT_SIDE_PARTY) {
					syRotY(DEGtoANG(180));
				}
				syMatrixScaleUniform(RISING_DRAGON_ENEMY_BODY_SCALE);
				btRenderTransformVertices(
					cells, vertices, RISING_DRAGON_ENEMY_SEGMENTS * 2);
				btRenderDrawModelList(model);
				syPopMatrix();
				/* The dragon's head, its jaws about the target, until tick 16
				   and again from tick 60 until the burst. */
				if(frm <= 133 && (Uint32)(frm - 17) > 42) {
					texture.logWidth = 6;
					texture.logHeight = 6;
					model->mesh = BattleEffectSquareMesh;
					model->scrollU = zero;
					texture.pixels = state->sheet +
						RISING_DRAGON_ENEMY_FLAME_DRAGON_HEAD_PICTURES;
					syTranslate(
						INTtoFIXED(tgt.x - 124), INTtoFIXED(height - 144), 0);
					syRotZ(DEGtoANG(90));
					/* Drawn one and a third to one and a half times its
					   size. */
					syScale(toFIXED(1) + 40000, toFIXED(1) + 20000, toFIXED(1));
					btRenderTransformVertices(
						BattleEffectSquareCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);

				if(frm == 134) {
					for(i = 0, scatter = EXTRA_PARTICLES;
						i != RISING_DRAGON_ENEMY_SPARK_COUNT; i++, scatter++) {
						Sint32 ang;
						Sint32 speed;

						scatter->x = toFIXED(source.x / 2);
						scatter->y = 0;
						ang = (syRandom() & 0x7fff) - DEGtoANG(90);
						speed = (syRandom() & 255) + 32;
						scatter->velX = (sySin(ang) * speed) >> 5;
						scatter->velY = (syCos(ang) * speed) >> 5;
						scatter->variant = (syRandom() & 7) + 32;
					}
				}
				burstFrm = frm - 134;
				if((Uint32)burstFrm < RISING_DRAGON_ENEMY_BURST_TICKS) {
					Sint32 first = burstFrm * RISING_DRAGON_ENEMY_BURSTS_A_TICK;
					for(i = 0; i != RISING_DRAGON_ENEMY_BURSTS_A_TICK; i++) {
						PARTICLE *burst = &state->particles[first + i];
						Sint32	  ang;
						Sint32	  speed;

						burst->x = toFIXED(source.x / 2);
						burst->y = 0;
						ang = (syRandom() & 0x7fff) - DEGtoANG(90);
						speed = (syRandom() & 255) + 32;
						burst->velX = (sySin(ang) * speed) >> 5;
						burst->velY = (syCos(ang) * speed) >> 5;
						burst->variant = 0;
					}
				}
				for(i = 0, particle = state->particles;
					i != RISING_DRAGON_ENEMY_FLASH_BURSTS; i++, particle++) {
					Sint32 age = particle->variant;

					if((Uint32)age <
						FLASH_BURST_PICTURE_COUNT * FLASH_BURST_PICTURE_TICKS) {
						if(age > 1) {
							blit[0](canvas,
								state->sheet +
									RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES +
									(age / FLASH_BURST_PICTURE_TICKS) *
										PICTURE_BYTES(FLASH_BURST),
								FIXED_INTEGER_PART(particle->x) -
									FLASH_BURST_WIDTH / 2,
								FIXED_INTEGER_PART(particle->y) -
									FLASH_BURST_HEIGHT / 2,
								FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
						}
						btParticleAdvanceWithGravity2D(
							particle, 60, -toFIXED(1) / 16);
						particle->variant++;
					}
				}
				if((Uint32)frm <= 133) {
					PARTICLE *trail;
					FIXED	  startY;
					Sint32	  slot;
					Sint32	  first;
					Sint32	  y;

					slot = (frm - 16) & 63;
					y = 120;
					if(frm > 59) {
						y = height - 80;
					}
					first = slot * 2;
					for(i = 0, startY = toFIXED(y),
					trail = &EXTRA_PARTICLES[TRAIL_SPARKS + first];
						i != 2; i++, trail++) {
						Sint32 random;
						Sint32 ang;
						Sint32 speed;
						Sint32 x = source.x / 2;

						trail->y = startY;
						trail->x = toFIXED(x);
						random = syRandom() & 0x7fff;
						ang = random + DEGtoANG(90);
						if(frm > 59) {
							ang = random + TRIG_TURN * 3 / 4;
						}
						speed = (syRandom() & 255) + 32;
						trail->velX = (sySin(ang) * speed) >> 6;
						trail->velY = (syCos(ang) * speed) >> 6;
						if(frm > 59) {
							trail->velX /= 2;
						}
						trail->variant = (syRandom() & 7) + 32;
					}
				}
				{
					PARTICLE *spark;
					FIXED	  gravity = -toFIXED(1) / 16;

					if(frm > 59) {
						gravity = toFIXED(1) / 4;
					}
					for(i = 0, spark = EXTRA_PARTICLES;
						i != RISING_DRAGON_ENEMY_SPARK_COUNT; i++, spark++) {
						Sint32 size;

						if(spark->variant < 0) {
							continue;
						}
						size = (spark->variant - 8) / 8;
						if(frm > 59) {
							size += 2;
						}
						if(size <= 0) {
							size = 1;
						}
						blit[0](canvas,
							sprs + BattleEffectMotePictures[size - 1],
							FIXED_INTEGER_PART(spark->x) - size / 2,
							FIXED_INTEGER_PART(spark->y) - size, size,
							size * 2);
						btParticleAdvanceWithGravity2D(spark, 60, gravity);
						spark->variant--;
					}
				}
			}
		}
		btEffectCountDownBattlerTints();
		if((Uint32)(frm - 32) > 133) {
			btCameraShake(8, 8);
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Abilities: Bone Charge. */
void btEffectRunBoneCharge(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_STUN_MUSCLE);
}

/* Abilities: Brute Force, Troll. */
void btEffectRunBruteForce(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}

/* Abilities: Onslaught. */
void btEffectRunOnslaught(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}

/* Abilities: Headbutt. */
void btEffectRunHeadbutt(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}

/* Abilities: Stun Muscle. */
void btEffectRunStunMuscle(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_STUN_MUSCLE);
}

/* Abilities: Living Armor, Minotaur, Power Crush. */
void btEffectRunLivingArmor(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_LIVING_ARMOR);
}

#undef STAR_MINE_TICKS
#undef STAR_MINE_STARS
#undef STAR_MINE_FLASH_BURSTS
#undef STAR_MINE_SPARKS
#undef STAR_MINE_PULL
#undef STAR_MINE_STAR_PICTURES
#undef STAR_MINE_FLASH_BURST_PICTURES
#undef STAR_WIDTH
#undef STAR_HEIGHT
#undef STAR_HALF
#undef STAR_BURST_FLASH_BURSTS
#undef STAR_BURST_SPARKS
#undef FLASH_BURST_PICTURE_TICKS
#undef BACKDROP_FIRST_TILE
#undef BACKDROP_ROWS
#undef BACKDROP_RIGHT_HALF
#undef RISING_DRAGON_ENEMY_SPARK_COUNT
#undef TRAIL_SPARKS
#undef RISING_DRAGON_ENEMY_VERTICES
#undef RISING_DRAGON_ENEMY_STRIP
#undef RISING_DRAGON_ENEMY_SEGMENTS
#undef RISING_DRAGON_ENEMY_BODY_WIDTH
#undef RISING_DRAGON_ENEMY_ROW_PHASE
#undef RISING_DRAGON_ENEMY_BODY_SCALE
#undef RISING_DRAGON_ENEMY_SWAY_STEP
#undef RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES
#undef RISING_DRAGON_ENEMY_FLAME_DRAGON_HEAD_PICTURES
#undef RISING_DRAGON_ENEMY_BURST_TICKS
#undef RISING_DRAGON_ENEMY_BURSTS_A_TICK
#undef RISING_DRAGON_ENEMY_FLASH_BURSTS
#undef RISING_DRAGON_ENEMY_HAZE_SPEED
#undef RISING_DRAGON_ENEMY_FLASH_LEVEL
#undef RISING_DRAGON_ENEMY_TICKS
