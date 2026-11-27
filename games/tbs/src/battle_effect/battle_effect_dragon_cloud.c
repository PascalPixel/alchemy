/* Dragon Cloud. Part of battle_effect.c. */

#define DRAGON_CLOUD_TICKS		  150
#define DRAGON_CLOUD_STRIKE_TICK  83 /* the flame strikes the first target */
/* The trail picture's size, in pixels. */
#define DRAGON_CLOUD_TRAIL_WIDTH  24
#define DRAGON_CLOUD_TRAIL_HEIGHT 39
#define DRAGON_CLOUD_TRAIL_PICTURE_BYTES \
	PICTURE_BYTES(DRAGON_CLOUD_TRAIL) /* bytes in each trail copy */
#define DRAGON_CLOUD_FIRST_EMBER   8  /* the first ember in state->particles */
#define DRAGON_CLOUD_FIRST_FLASH   48 /* the first flash in state->particles */
/* FirePillarSheetB: five flames, then the trail picture. */
#define DRAGON_CLOUD_FLAME_WIDTH   24
#define DRAGON_CLOUD_FLAME_HEIGHT  48
#define DRAGON_CLOUD_TRAIL_PICTURE (5 * PICTURE_BYTES(DRAGON_CLOUD_FLAME))
#define DRAGON_CLOUD_PUFF_PICTURES \
	15360 /* the puff pictures in the sprite sheet */
#define DRAGON_CLOUD_PUFFS				8 /* puffs, particles 0 to 7 */
#define DRAGON_CLOUD_EMBERS				40 /* embers, from DRAGON_CLOUD_FIRST_EMBER */
#define DRAGON_CLOUD_FLASHES			16 /* flashes, from DRAGON_CLOUD_FIRST_FLASH */
/* The trail's copies of the flame's path, the older ones from
   DRAGON_CLOUD_TRAIL_BRIGHT on drawn darker, four levels a copy. */
#define DRAGON_CLOUD_TRAIL_LENGTH		20
#define DRAGON_CLOUD_TRAIL_BRIGHT		10
/* The puffs' arc: this many pixels across and down from its centre. */
#define DRAGON_CLOUD_PUFF_RADIUS_X		24
#define DRAGON_CLOUD_PUFF_RADIUS_Y		16
/* The particles from DRAGON_CLOUD_FIRST_EMBER drawn as embers: the embers and
   the first eight flashes; the rest play the flash pictures. The Lost Age keeps
   the same count for Dragon Cloud (DragonCloudSparkCounts). */
#define DRAGON_CLOUD_DRAWN_EMBERS		48
/* An ember is drawn at size DRAGON_CLOUD_EMBER_SIZE_MAX at MOTE_DEPTH_NEAR or
   nearer, one size smaller every MOTE_DEPTH_STEP farther. */
#define DRAGON_CLOUD_EMBER_SIZE_MAX		9
/* The flash and puff animations: six pictures, two ticks each. */
#define DRAGON_CLOUD_ANIMATION_PICTURES 6

/* Draws one puff picture sized by the puff tables. */
#define DRAGON_CLOUD_DRAW_PUFF(blit, canvas, sheet, picture, x, y) \
	blit(canvas, \
		&(sheet)[DRAGON_CLOUD_PUFF_PICTURES] + \
			BattleEffectPuffArcPictures[picture], \
		x, y, BattleEffectPuffArcPictureWidths[picture], \
		BattleEffectPuffArcPictureHeights[picture])

/* A flame leaves the user along a stored path with a fading trail behind it,
   strikes the first target and bursts into embers and flashes.

   Abilities: Dragon Cloud. */
void btEffectRunDragonCloud(BATTLE_EFFECT_PARM *args)
{
	POINT2_D			 trail[DRAGON_CLOUD_TRAIL_LENGTH];
	SCREEN_POINT		 pos;
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 drift;
	Sint8				*path;
	Sint32				 x;
	Sint32				 y;
	Uint8				*sheet;
	Sint32				 i;
	Sint32				 j;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	drift = 0;
	x = 0;
	y = 0;
	sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_PILLAR_SHEET_B, state->sheet, 1, 1);
	/* Darker copies of the trail picture, for the trail's tail. */
	for(i = 1; i != DRAGON_CLOUD_TRAIL_LENGTH; i++) {
		for(j = 0; j != DRAGON_CLOUD_TRAIL_PICTURE_BYTES; j++) {
			Sint32 shade = state->sheet[DRAGON_CLOUD_TRAIL_PICTURE + j];

			if(i > DRAGON_CLOUD_TRAIL_BRIGHT) {
				shade = shade - (i - DRAGON_CLOUD_TRAIL_BRIGHT) * 4;
				if(shade < 0)
					shade = 0;
				state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
					(i - DRAGON_CLOUD_TRAIL_BRIGHT) *
						DRAGON_CLOUD_TRAIL_PICTURE_BYTES +
					j] = shade;
			}
		}
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
		&state->sheet[DRAGON_CLOUD_PUFF_PICTURES], 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_FLASH_BURST_SHEET, RamMapGridMetatileBuffer, 1, 0);
	btEffectBuildCanvasBlitters(state->args->side, draw);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The embers and flashes thrown at the strike, and the puffs on an arc. */
	for(j = 0; j != DRAGON_CLOUD_EMBERS; j++) {
		PARTICLE *ember = &state->particles[DRAGON_CLOUD_FIRST_EMBER + j];

		if(state->args->side == EFFECT_SIDE_ENEMIES)
			ember->x = -toFIXED(56);
		else
			ember->x = toFIXED(56);
		ember->y = 0;
		ember->z = 0;
		SET_VELOCITY(ember, ((syRandom() & 63) - 32) << 14,
			(syRandom() & 63) << 13, ((syRandom() & 63) - 32) << 14);
		ember->variant = 1;
	}
	for(j = 0; j != DRAGON_CLOUD_FLASHES; j++) {
		PARTICLE *flash = &state->particles[DRAGON_CLOUD_FIRST_FLASH + j];

		if(state->args->side == EFFECT_SIDE_ENEMIES)
			flash->x = -toFIXED(56);
		else
			flash->x = toFIXED(56);
		flash->y = toFIXED(20);
		flash->z = 0;
		SET_VELOCITY(flash, ((syRandom() & 63) - 32) << 14,
			(syRandom() & 63) << 12, ((syRandom() & 63) - 32) << 14);
		flash->variant = 0;
	}
	for(i = 0, j = -DEGtoANG(90); i != DRAGON_CLOUD_PUFFS;
		i++, j += TRIG_TURN / 16) {
		PARTICLE *puff = &state->particles[i];

		if(state->args->side == EFFECT_SIDE_PARTY)
			puff->x = FIXEDtoINT(sySin(j) * DRAGON_CLOUD_PUFF_RADIUS_X) + 88;
		else
			puff->x = FIXEDtoINT(-(sySin(j) * DRAGON_CLOUD_PUFF_RADIUS_X)) + 16;
		puff->y = FIXEDtoINT(syCos(j) * DRAGON_CLOUD_PUFF_RADIUS_Y) + 40;
		puff->variant = -(i * 2);
	}
	path = syResourceGetTableEntry(RESOURCE_DRAGON_CLOUD_FLAME_PATH);

	for(frm = 0; frm != DRAGON_CLOUD_TICKS; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		if(frm == DRAGON_CLOUD_STRIKE_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frm == 0)
			sySndPlayStub(SE_WHOOSH);
		if(frm == 50)
			sySndPlayStub(SE_WHOOSH);
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			if(frm <= 63)
				camera->yaw -= TRIG_TURN / 256;
		} else {
			if(frm <= 63)
				camera->yaw += TRIG_TURN / 256;
		}
		btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
		if(frm <= 17) {
			Sint32 picture = frm / 3;

			DRAGON_CLOUD_DRAW_PUFF(draw[0], canvas, state->sheet, picture, 48,
				BattleEffectPuffArcPictureBiasY[picture] + 60);
			DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, state->sheet, picture, 56,
				BattleEffectPuffArcPictureBiasY[picture] + 60);
		}
		/* The flame flies along its path. */
		if(frm >= 18 && frm <= 58) {
			if(frm == 18) {
				x = (path[0] << 8) + (Uint8)path[1];
				y = (path[2] << 8) + (Uint8)path[3] + 16;
				path += 4;
			} else {
				x += path[0];
				y += path[1];
				path += 2;
			}
		}
		if(frm >= 78 && frm <= 118) {
			if(frm == 78) {
				x = -56;
				y = 48;
			} else {
				y -= 16;
			}
		}
		for(j = DRAGON_CLOUD_TRAIL_LENGTH - 1; j != 0; j--) {
			if(frm > j + 18 && frm <= j + 83) {
				trail[j].x = trail[j - 1].x;
				trail[j].y = trail[j - 1].y;
				if(j > DRAGON_CLOUD_TRAIL_BRIGHT)
					draw[0](canvas,
						&state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
							(j - DRAGON_CLOUD_TRAIL_BRIGHT) *
								DRAGON_CLOUD_TRAIL_PICTURE_BYTES],
						trail[j].x, trail[j].y, DRAGON_CLOUD_TRAIL_WIDTH,
						DRAGON_CLOUD_TRAIL_HEIGHT);
				else
					draw[0](canvas, &state->sheet[DRAGON_CLOUD_TRAIL_PICTURE],
						trail[j].x, trail[j].y, DRAGON_CLOUD_TRAIL_WIDTH,
						DRAGON_CLOUD_TRAIL_HEIGHT);
			}
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* The flame leans as it rises and falls. */
		if(frm >= 18 && frm <= DRAGON_CLOUD_STRIKE_TICK) {
			Sint32 lift;

			if(state->args->side == EFFECT_SIDE_PARTY)
				pos.x = 64 - x / 2;
			else
				pos.x = x / 2 + 64;
			pos.y = 60 - y;
			lift = (pos.y - trail[0].y - 24) / 2;
			if(lift > 2)
				lift = 2;
			if(lift < -2)
				lift = -2;
			drift += lift;
			if(drift > 8)
				drift = 8;
			if(drift < -8)
				drift = -8;
			lift = drift / 4 + 2;
			trail[0].x = pos.x - 12;
			trail[0].y = pos.y - 20;
			draw[0](canvas,
				&state->sheet[lift * PICTURE_BYTES(DRAGON_CLOUD_FLAME)],
				trail[0].x - 6, trail[0].y - 2, DRAGON_CLOUD_FLAME_WIDTH,
				DRAGON_CLOUD_FLAME_HEIGHT);
		}
		if(frm == DRAGON_CLOUD_STRIKE_TICK) {
			state->shakeFrm = 8;
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_HOP);
		}
		/* After the strike the embers rise and fall back. */
		if(frm > DRAGON_CLOUD_STRIKE_TICK) {
			for(i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
				PARTICLE *ember =
					&state->particles[DRAGON_CLOUD_FIRST_EMBER + i];

				if(ember->y >= 0) {
					Sint32 size;

					btProjectionPoint(&ember->x, &pos);
					pos.x >>= 1;
					if(pos.depth < MOTE_DEPTH_NEAR)
						pos.depth = MOTE_DEPTH_NEAR;
					if(pos.depth > MOTE_DEPTH_FAR)
						pos.depth = MOTE_DEPTH_FAR;
					size = DRAGON_CLOUD_EMBER_SIZE_MAX -
						(pos.depth - MOTE_DEPTH_NEAR) / MOTE_DEPTH_STEP;
					if(i >= DRAGON_CLOUD_DRAWN_EMBERS) {
						if(ember->variant <
							2 * DRAGON_CLOUD_ANIMATION_PICTURES) {
							draw[0](canvas,
								RamMapGridMetatileBuffer +
									ember->variant / 2 *
										PICTURE_BYTES(FLASH_BURST),
								pos.x - FLASH_BURST_WIDTH / 2,
								pos.y - FLASH_BURST_HEIGHT / 2,
								FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
							ember->variant++;
						}
					} else {
						EFFECT_DRAW_MOTE(
							draw[0], canvas, sheet, pos.x, pos.y, size);
					}
					ember->x += ember->velX;
					ember->y += ember->velY;
					ember->z += ember->velZ;
					ember->velY -= toFIXED(1) / 8;
				}
			}
		}
		if(frm == 50) {
			state->shakeFrm = 12;
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
		}
		/* The puffs. */
		if(frm > 49) {
			for(i = 0; i != DRAGON_CLOUD_PUFFS; i++) {
				PARTICLE *puff = &state->particles[i];

				if(puff->variant >= 0 &&
					puff->variant < 2 * DRAGON_CLOUD_ANIMATION_PICTURES) {
					Sint32 picture = puff->variant / 2;

					DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, state->sheet,
						picture,
						puff->x - BattleEffectPuffArcPictureWidths[picture] / 2,
						puff->y + BattleEffectPuffArcPictureBiasY[picture]);
				}
				puff->variant++;
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef DRAGON_CLOUD_TICKS
#undef DRAGON_CLOUD_STRIKE_TICK
#undef DRAGON_CLOUD_TRAIL_PICTURE_BYTES
#undef DRAGON_CLOUD_FIRST_EMBER
#undef DRAGON_CLOUD_FIRST_FLASH
#undef DRAGON_CLOUD_TRAIL_PICTURE
#undef DRAGON_CLOUD_PUFF_PICTURES
#undef DRAGON_CLOUD_PUFFS
#undef DRAGON_CLOUD_EMBERS
#undef DRAGON_CLOUD_FLASHES
#undef DRAGON_CLOUD_DRAW_PUFF
#undef DRAGON_CLOUD_TRAIL_LENGTH
#undef DRAGON_CLOUD_TRAIL_BRIGHT
#undef DRAGON_CLOUD_PUFF_RADIUS_X
#undef DRAGON_CLOUD_PUFF_RADIUS_Y
#undef DRAGON_CLOUD_DRAWN_EMBERS
#undef DRAGON_CLOUD_EMBER_SIZE_MAX
#undef DRAGON_CLOUD_ANIMATION_PICTURES
