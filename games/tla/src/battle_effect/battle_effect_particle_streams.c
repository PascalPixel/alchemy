/* Meteor and Outer Space; the summons' formations and the burst they end in;
   brightening the background's colours; and an unused copy of Nereid. Part of
   battle_effect.c. */

/* The particle streams' modes (btEffectRunStarfieldFall). */
typedef enum PARTICLE_STREAMS_MODE {
	STARFIELD_FALL_METEOR, /* one spawned sprite falls */
	/* the user leaps, and two animated sprites fall in the lightning bolt's
	   colours */
	STARFIELD_FALL_OUTER_SPACE
} PARTICLE_STREAMS_MODE;

/* Unused: nothing calls it. Knocks a unit back: unless it is the Fusion Dragon
   or one of three of the Doom Dragon's forms, it leaps back to 130 to 193
   percent of its place's x, then takes its damage pose. */
UNUSED void btEffectStartActorMotionRandomDistance(Sint32 id)
{
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;
	Uint32		   enemy;

	slot = btObjectGetStub(id);
	object = slot->object;
	enemy = syUnitGet(id)->enemyId;
	if(enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A &&
		enemy != ENEMY_DOOM_DRAGON_E && enemy != ENEMY_DOOM_DRAGON_H) {
		Sint32 percent = (syRandom() & 63) + 130;

		object->acceleration = toFIXED(8);
		object->speedLimit = toFIXED(10);
		object->velocityY = toFIXED(3) / 2;
		fdActorStartIdleStub(object);
		fdActorSetMoveTargetStub(
			object, slot->anchorX * percent / 100, 0, slot->anchorZ);
	}
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorSelectAnimationStub(object, POSE_DAMAGE);
}

/* Unused: nothing reaches it (no handler, summon case or call). A second copy
   of btEffectRunNereid, with its names: Nereid rides in on her turtle over
   rising waves, columns of water and spray rise over the targets, and she
   bursts away. */
UNUSED void btEffectRunNereidCopy(BATTLE_EFFECT_PARM *args)
{
	DRAW_RECTANGLE		 draw[2];
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*cells;
	Uint8				*palette;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	cells = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);

	/* The spray's colours and pictures, the column of water and the motes. */
	palette = syResourceGetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
	syDMAXCopy(palette, (void *)BG_PLTT,
		DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
	palette += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(palette, state->sheet);
	palette = syResourceGetTableEntry(RESOURCE_FIRE_PILLAR_SHEET_A);
	palette += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(palette, state->sheet + NEREID_COLUMN_PICTURE);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), cells);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	{
		POINT2_D  focus;
		POINT2_D  velocity;
		Sint32	  rise;
		Sint32	  frame;
		Sint32	  k;
		PARTICLE *point;
		PARTICLE *drop;
		PARTICLE *drops;

		/* Nereid rides in from the right. */
		rise = 0;
		focus.x = toFIXED(256);
		focus.y = toFIXED(88);
		velocity.x = -toFIXED(16);
		velocity.y = -toFIXED(4);
		EFFECT_FREE_PARTICLES(k, state->particles, 64);
		/* DEAD: particles NEREID_UNREAD_FIRST on get a place and delay nothing
		   reads; the ROM keeps the stores. */
		for(k = 0; k != NEREID_UNREAD_COUNT; k++) {
			point = &state->particles[NEREID_UNREAD_FIRST + k];
			point->x = syRandom() & 127;
			point->y = (syRandom() & 7) + 56;
			point->variant = -(syRandom() & 15);
		}
		/* Every drop in the map cell buffer starts free. */
		EFFECT_FREE_PARTICLES(k, PARTICLES, NEREID_DROPS);
		drops = PARTICLES;
		btEffectShowLivingTargets(state->args);
		sySynch(1);
		btEffectCreateSprites(NEREID_OBJECTS, SPRITE_NEREID, 2);

		for(frame = 0; frame != NEREID_TICKS; frame++) {
			if((PerWork.repeat & (KEY_A | KEY_B)) &&
				frame > NEREID_COLUMNS_TICK && frame < 98)
				frame = 98;
			if(frame == 120)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			/* The waves rise. */
			if(frame <= 15)
				rise += 2;

			/* She slows, and is pushed back right once she passes x = 120. */
			if(frame <= 99) {
				focus.x += velocity.x;
				focus.y += velocity.y;
				velocity.x = velocity.x * 58 / DAMPING_SCALE;
				velocity.y = velocity.y * 56 / DAMPING_SCALE;
				if(focus.x < toFIXED(120))
					velocity.x += toFIXED(1) / 2;
			}
			btEffectPlaceFormationSprites(
				SUMMON_FORMATION_NEREID, focus.x, focus.y);

			/* Drops burst up among the targets. */
			if(frame == 28) {
				for(k = 0; k != NEREID_BURST_DROPS; k++) {
					drop = &drops[k];
					if(drop->variant == -1) {
						Sint32 radius = syRandom() & 63;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

						drop->x = ((sySin(angle) * radius) >> 3) + toFIXED(32);
						drop->y = ((syCos(angle) * radius) >> 2) + toFIXED(96);
						drop->velocityX = ((syRandom() & 63) - 32) << 14;
						drop->velocityY = (-(syRandom() & 63) - 8) << 13;
						drop->variant = 0;
					}
				}
			}
			if(frame >= NEREID_COLUMNS_TICK &&
				frame < NEREID_COLUMNS_END_TICK) {
				Sint32 spawned = 0;

				for(k = 0; k != NEREID_DROPS; k++) {
					drop = &drops[k];
					if(drop->variant == -1) {
						Sint32 radius = syRandom() & 63;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

						drop->x = ((sySin(angle) * radius) >> 3) + toFIXED(32);
						drop->y = ((syCos(angle) * radius) >> 2) + toFIXED(96);
						drop->velocityX = ((syRandom() & 63) - 32) << 14;
						drop->velocityY = (-(syRandom() & 63) - 8) << 13;
						drop->variant = 0;
						spawned++;
						if(spawned == NEREID_DROPS_A_TICK)
							break;
					}
				}
			}
			if(frame == 0)
				sySoundPlayStub(SE_SCATTER);
			if(frame == NEREID_COLUMNS_TICK)
				sySoundPlayStub(SE_CRASH);
			if(frame == NEREID_COLUMNS_END_TICK)
				sySoundPlayStub(SE_CLATTER);

			/* The columns of water scroll up. */
			if(frame >= NEREID_COLUMNS_TICK &&
				frame < NEREID_COLUMNS_END_TICK) {
				for(k = 0; k != NEREID_COLUMNS; k++) {
					Sint32 span =
						(frame * 16 - 256 + k * 25) % FIRE_PILLAR_HEIGHT;

					draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
						NereidCopyColumns[k * 2] - FIRE_PILLAR_WIDTH / 2,
						NereidCopyColumns[k * 2 + 1] - span -
							FIRE_PILLAR_HEIGHT,
						FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
					draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
						NereidCopyColumns[k * 2] - FIRE_PILLAR_WIDTH / 2,
						NereidCopyColumns[k * 2 + 1] - span, FIRE_PILLAR_WIDTH,
						span);
				}
			}

			/* The waves along the bottom. */
			if(frame <= 95) {
				for(k = 0; k != 5; k++)
					draw[0](canvas, state->sheet,
						k * WATER_WAVE_WIDTH + ((frame / 4) & 31) -
							WATER_WAVE_WIDTH,
						120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
			}

			/* Drops swell as they clear: they fall under gravity until tick
			   80, then rise away, and a falling drop that sinks into the waves
			   is freed. */
			for(k = 0; k != NEREID_DROPS; k++) {
				drop = &drops[k];
				if(drop->variant >= 0) {
					Sint32 size = k % 3 + 2;

					if(drop->velocityY > 0)
						size += 2;
					if(frame > 68 && size <= 5)
						size = 6;
					if(frame > 70 && size <= 6)
						size = 7;
					if(frame > 72 && size <= 7)
						size = 8;
					if(frame > 74 && size <= 8)
						size = 9;
					if(frame > 76)
						size = MOTE_PICTURES;
					EFFECT_DRAW_MOTE(draw[drop->velocityY > 0], canvas, cells,
						FIXED_INTEGER_PART(drop->x),
						FIXED_INTEGER_PART(drop->y), size);
					drop->x += drop->velocityX;
					drop->y += drop->velocityY;
					if(frame > NEREID_COLUMNS_END_TICK)
						drop->velocityY += -toFIXED(1) / 2;
					else
						drop->velocityY += NereidCopyGravity[k & 3];
					drop->velocityX = drop->velocityX * 62 / DAMPING_SCALE;
					drop->velocityY = drop->velocityY * 62 / DAMPING_SCALE;
					drop->variant++;
					if(drop->velocityY > 0 &&
						FIXED_INTEGER_PART(drop->y) > NEREID_WAVES_Y)
						drop->variant = -1;
				}
			}

			/* The targets bounce and flash white every twelve ticks from tick
			   30. */
			if(frame < NEREID_COLUMNS_END_TICK) {
				for(k = 0; k != args->targetCount; k++) {
					if(frame > 29) {
						Sint32 phase = frame % 12;

						if(phase == 0) {
							MOTION_OBJECT *target =
								btObjectGetStub(args->targets[k])->object;

							btEffectTintBattler(args->targets[k],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, TINT_KEEP, 0);
							target->velocityY = toFIXED(9) / 2;
							target->verticalMotionStrength =
								BATTLE_UNIT_GRAVITY;
						}
						if(phase == 6)
							btEffectTintBattler(args->targets[k],
								SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP, 0);
					}
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		}

		/* She bursts away. */
		syCloseEvent(btEffectTransferCanvas);
		EFFECT_RELEASE_BLITTERS();
		btEffectRunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
		for(k = 0; k != NEREID_OBJECTS; k++)
			fdSpriteDestroyStub(state->sprites[k]);
	}
	btEffectEndCanvasLayer();
}

extern const SCALE SummonFormationScale;
extern const SCALE SummonFormationLargeScale;

/* The grid formations' sprites stand this many pixels apart. */
#define FORMATION_SPACING 32

/* Arranges state->sprites in a formation (SUMMON_FORMATION); x and z are its
   origin in 16.16 pixels. */
void btEffectPlaceFormationSprites(Sint32 formation, Sint32 x, Sint32 z)
{
	BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEffect;
	SCALE				 normal = SummonFormationScale;
	SCALE				 big = SummonFormationLargeScale;
	FIXED				 pos[4];
	Sint32				 i;
	void			   **objects;

	pos[3] = 0;
	pos[1] = toFIXED(255);
	switch(formation) {
	case SUMMON_FORMATION_THOR:
		objects = state->sprites;
		for(i = 0; i != 3 * 3; i++) {
			pos[0] = toFIXED((i % 3) * FORMATION_SPACING) + x;
			pos[2] = toFIXED((i / 3) * FORMATION_SPACING) + z;
			fdRenderApplyProjectedPlacementStub(*objects++, pos, &normal.x, 0);
		}
		break;
	case SUMMON_FORMATION_MOLOCH:
		for(i = 0; i != 4 * 3; i++) {
			pos[0] = toFIXED((i % 4) * FORMATION_SPACING) + x;
			pos[2] = toFIXED((i / 4) * FORMATION_SPACING) + z;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &normal.x, 0);
		}
		break;
	case SUMMON_FORMATION_NEREID:
		for(i = 0; i != NEREID_OBJECTS; i++) {
			pos[0] = toFIXED(SummonFormationNereidX[i]) + x - toFIXED(16);
			pos[2] = toFIXED(SummonFormationNereidZ[i]) + z - toFIXED(32);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &normal.x, 0);
		}
		break;
	case SUMMON_FORMATION_CYBELE:
		for(i = 0; i != 8; i++) {
			pos[0] = toFIXED(SummonFormationCybeleX[i]) + x + toFIXED(16);
			pos[2] = toFIXED(SummonFormationCybeleZ[i]) + z;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &normal.x, 0);
		}
		break;
	case SUMMON_FORMATION_TIAMAT_ALTERNATE:
		for(i = 0; i != 8; i++) {
			pos[0] = toFIXED(SummonFormationTiamatAlternateX[i]) + x;
			pos[2] = toFIXED(SummonFormationTiamatAlternateZ[i]) + z;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &normal.x, 0);
		}
		break;
	case SUMMON_FORMATION_TIAMAT:
		for(i = 0; i != 8; i++) {
			pos[0] = toFIXED(SummonFormationTiamatX[i]) + x;
			pos[2] = toFIXED(SummonFormationTiamatZ[i]) + z;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &big.x, 0);
		}
		break;
	case SUMMON_FORMATION_MEGAERA:
		for(i = 0; i != MEGAERA_OBJECTS; i++) {
			pos[0] = toFIXED(SummonFormationMegaeraX[i]) + x;
			pos[2] = toFIXED(SummonFormationMegaeraZ[i]) + z;
			fdRenderApplyProjectedPlacementStub(
				state->sprites[i], pos, &normal.x, 0);
		}
		break;
	}
}

/* The fragment pictures in the sprite sheet, four of 12x24, after the flash. */
#define IMPACT_BURST_SHARD_PICTURES	 23000
#define IMPACT_BURST_FRAGMENT_WIDTH	 12 /* each fragment picture's width */
#define IMPACT_BURST_FRAGMENT_HEIGHT 24

#define IMPACT_BURST_TICKS			72
#define IMPACT_BURST_FRAGMENTS		3 /* fragments, state->particles 61 to 63 */
#define IMPACT_BURST_DRAWN_LARGE	30 /* large sparks drawn */
#define IMPACT_BURST_DRAWN_SMALL	60 /* small sparks drawn */
#define IMPACT_BURST_LARGE_SPARKS	64 /* large sparks thrown */
#define IMPACT_BURST_SMALL_SPARKS	64 /* small sparks thrown */
/* the first fragment in state->particles */
#define IMPACT_BURST_FIRST_FRAGMENT 61
#define IMPACT_BURST_FRAGMENT_SPEED 32 /* in the units of a spark's speed */
/* A spark shrinks a size each time this many ticks of its life run out. */
#define IMPACT_BURST_SHRINK_TICKS	16

/* Opens an impact flash and scatters sparks and fragments, keeping the effect's
   sprites in formation for the first 36 ticks. */
void btEffectRunImpactBurst(Sint32 formation, Sint32 x, FIXED y)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 additive;
	DRAW_RECTANGLE		 maxBlend;
	Uint8				*sheet;
	Sint32				 orgX;
	PARTICLE			*p;
	Sint32				 frame;
	Sint32				 i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sheet = RamHeapSlots->battleSheet;
	orgX = x;
	x += toFIXED(40);
	x /= 2;
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	maxBlend = RamHeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	additive = RamHeapSlots->ramCodePaired;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FLARE_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FLARE_IMAGE, state->sheet + IMPACT_BURST_SHARD_PICTURES, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	/* Large sparks fly. */
	for(i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
		Sint32 speed = (syRandom() & 255) + 256;
		Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

		state->particles[i].x = x;
		state->particles[i].y = y;
		state->particles[i].velocityX = (sySin(angle) * speed) >> 7;
		state->particles[i].velocityY = -((syCos(angle) * speed) >> 6);
		state->particles[i].variant = (syRandom() & 15) + 16;
	}

	/* Three fragments fly apart. */
	for(i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
		state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
		state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
		state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
			(sySin(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 6;
		state->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
			-((syCos(i * (DEGtoANG(120))) * IMPACT_BURST_FRAGMENT_SPEED) >> 5);
	}

	/* Small sparks fly. */
	for(i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
		Sint32	  speed = (syRandom() & 255) + 32;
		Sint32	  angle = syRandom() & TRIG_ANGLE_MASK;
		PARTICLE *cells = PARTICLES;

		cells[i].x = x;
		cells[i].y = y;
		cells[i].velocityX = (sySin(angle) * speed) >> 6;
		cells[i].velocityY = -((syCos(angle) * speed) >> 5);
		cells[i].variant = (syRandom() & 15) + 20;
	}

	for(frame = 0; frame != IMPACT_BURST_TICKS; frame++) {
		if(frame == 4)
			sySoundPlayStub(SE_CAST);
		if(frame == 32)
			sySoundPlayStub(SE_POWER_UP);

		if(frame <= 47) {
			Sint32 picture = (frame - 8) / 5;
			Uint32 width;
			Uint32 height;

			if(picture < 0)
				picture = 0;
			/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
			width = ImpactBurstPictureWidths[picture];
			height = ImpactBurstPictureHeights[picture];
			maxBlend(canvas, state->sheet + ImpactBurstPictures[picture],
				FIXEDtoINT(x) - (width >> 1), FIXEDtoINT(y) - (height >> 1),
				width, height);
#else
			maxBlend(canvas, state->sheet + ImpactBurstPictures[picture],
				(FIXEDtoINT(x)) -
					((width = ImpactBurstPictureWidths[picture]) >> 1),
				(FIXEDtoINT(y)) -
					((height = ImpactBurstPictureHeights[picture]) >> 1),
				width, height);
#endif
		}

		for(i = 0, p = state->particles; i != IMPACT_BURST_DRAWN_LARGE;
			i++, p++) {
			if(frame > i / 2 && p->variant > 0) {
				Sint32 size;
				Sint32 px;
				Sint32 py;

				p->variant--;
				btParticleAdvanceWithGravity2D(p, 60, 0);
				size = p->variant / IMPACT_BURST_SHRINK_TICKS + 3;
				px = FIXED_INTEGER_PART(p->x);
				py = FIXED_INTEGER_PART(p->y);
				EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
			}
		}

		for(i = 0, p = PARTICLES; i != IMPACT_BURST_DRAWN_SMALL; i++, p++) {
			if(frame > 35 && p->variant > 0) {
				Sint32 size;
				Sint32 px;
				Sint32 py;

				p->variant--;
				btParticleAdvanceWithGravity2D(p, 60, 0);
				size = p->variant / IMPACT_BURST_SHRINK_TICKS + 1;
				px = FIXED_INTEGER_PART(p->x);
				py = FIXED_INTEGER_PART(p->y);
				EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
			}
		}

		for(i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
			PARTICLE *shard =
				&state->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

			if(frame >= 36 && frame <= 63) {
				Sint32 picture;
				Sint32 px;
				Sint32 py;

				btParticleAdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE, 0);
				picture = (frame - 36) / 7;
				px = FIXED_INTEGER_PART(shard->x);
				py = FIXED_INTEGER_PART(shard->y);
				maxBlend(canvas,
					state->sheet +
						picture * PICTURE_BYTES(IMPACT_BURST_FRAGMENT) +
						IMPACT_BURST_SHARD_PICTURES,
					px - IMPACT_BURST_FRAGMENT_WIDTH / 2,
					py - IMPACT_BURST_FRAGMENT_HEIGHT / 2,
					IMPACT_BURST_FRAGMENT_WIDTH, IMPACT_BURST_FRAGMENT_HEIGHT);
			}
		}

		if(frame <= 35)
			btEffectPlaceFormationSprites(formation, orgX, y);
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	EFFECT_RELEASE_BLITTERS();
}

/* Brightens background colours 1 to 63 by a step in each channel, or darkens
   them by a negative one. */
void btPaletteBrightenBgEntries(
	Sint32 blueDelta, Sint32 greenDelta, Sint32 redDelta)
{
	Uint16 *color = (Uint16 *)&BG_PLTT_COLOR(0, 1);
	Sint32	i;

	for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
		Sint32 blue = RGB_BLUE(*color);
		Sint32 green = RGB_GREEN(*color);
		Sint32 red = RGB_RED(*color);

		blue += blueDelta;
		green += greenDelta;
		red += redDelta;
		if(blue > RGB_LEVEL_MAX)
			blue = RGB_LEVEL_MAX;
		if(green > RGB_LEVEL_MAX)
			green = RGB_LEVEL_MAX;
		if(red > RGB_LEVEL_MAX)
			red = RGB_LEVEL_MAX;
		if(blue < 0)
			blue = 0;
		if(green < 0)
			green = 0;
		if(red < 0)
			red = 0;
		*color++ = RGB(red, green, blue);
	}
}

/* Brightens the battle background's colours, BG palettes 6 to 13, by a step in
   each channel, or darkens them by a negative one. */
void btPaletteBrightenBattleBg(
	Sint32 redDelta, Sint32 greenDelta, Sint32 blueDelta)
{
	Uint16 *color = (Uint16 *)&BG_PLTT_COLOR(6, 0);
	Sint32	i;

	for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
		Sint32 blue = RGB_BLUE(*color);
		Sint32 green = RGB_GREEN(*color);
		Sint32 red = RGB_RED(*color);

		red += redDelta;
		green += greenDelta;
		blue += blueDelta;
		if(red > RGB_LEVEL_MAX)
			red = RGB_LEVEL_MAX;
		if(green > RGB_LEVEL_MAX)
			green = RGB_LEVEL_MAX;
		if(blue > RGB_LEVEL_MAX)
			blue = RGB_LEVEL_MAX;
		if(red < 0)
			red = 0;
		if(green < 0)
			green = 0;
		if(blue < 0)
			blue = 0;
		*color++ = RGB(red, green, blue);
	}
}

/* As btPaletteBrightenBattleBg, from the battle background's own colours as the
   battle keeps them. */
void btPaletteBrightenSavedBattleBg(
	Sint32 redDelta, Sint32 greenDelta, Sint32 blueDelta)
{
	Uint16 *color = (Uint16 *)&BG_PLTT_COLOR(6, 0);
	Uint16 *src = RamHeapSlots->battleState->palette;
	Sint32	i;

	for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
		Sint32 blue = RGB_BLUE(*src);
		Sint32 green = RGB_GREEN(*src);
		Sint32 red = RGB_RED(*src);

		red += redDelta;
		green += greenDelta;
		blue += blueDelta;
		if(red > RGB_LEVEL_MAX)
			red = RGB_LEVEL_MAX;
		if(green > RGB_LEVEL_MAX)
			green = RGB_LEVEL_MAX;
		if(blue > RGB_LEVEL_MAX)
			blue = RGB_LEVEL_MAX;
		if(red < 0)
			red = 0;
		if(green < 0)
			green = 0;
		if(blue < 0)
			blue = 0;
		*color++ = RGB(red, green, blue);
		src++;
	}
}

/* Streams the fall's backdrop colours into colour 0, one each line. */
void btEffectArmPaletteHBlankDMA(void)
{
	BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEffect;
	volatile Uint16		*channel = REG_DMA0;

	channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
	channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_ENABLE;
	(void)channel[DMA_CONTROL_HALF];
	syDMAXCopy(state->backdropLines, (void *)BG_PLTT,
		DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)channel);
}

/* The battle dissolves into a starfield and a meteor falls through it in flames
   among falling fireballs, then bursts in embers and flashes.

   Abilities: Meteor. */
void btEffectRunMeteor(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_METEOR);
}

/* The Fusion Dragon leaps out of sight and falls back through a starfield in
   flames, then bursts in embers and flashes, all in the lightning bolt's
   colours.

   Abilities: Outer Space, the Fusion Dragon's. */
void btEffectRunOuterSpace(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_OUTER_SPACE);
}

/* The fall and the burst, in ticks. */
#define STARFIELD_FALL_FALL_TICKS	 192
#define STARFIELD_FALL_BURST_TICKS	 54
/* The burst's tick the damage is told from. */
#define PARTICLE_STREAMS_DAMAGE_TICK 48

/* The pictures in FlameSheetB (RESOURCE_FLAME_SHEET_B), each starting where the
   one before ends: six flames, then three large and three small glows, each
   glow brighter at its centre than the one before. The fall draws only the six
   and the brightest glow of each size. */
#define STARFIELD_FALL_FLAME_SIZE	   32
#define STARFIELD_FALL_LARGE_GLOW_SIZE 24
#define STARFIELD_FALL_SMALL_GLOW_SIZE 8

/* Where each picture starts in FlameSheetB, in bytes. */
typedef enum PARTICLE_STREAMS_SHEET_PICTURE {
	/* The six 32x32 flames, in a row */
	STARFIELD_FALL_FLAMES = 0,
	/* The brightest 24x24 glow */
	STARFIELD_FALL_LARGE_GLOW = STARFIELD_FALL_FLAMES +
		6 * STARFIELD_FALL_FLAME_SIZE * STARFIELD_FALL_FLAME_SIZE +
		2 * STARFIELD_FALL_LARGE_GLOW_SIZE * STARFIELD_FALL_LARGE_GLOW_SIZE,
	/* The brightest 8x8 glow */
	STARFIELD_FALL_SMALL_GLOW = STARFIELD_FALL_LARGE_GLOW +
		STARFIELD_FALL_LARGE_GLOW_SIZE * STARFIELD_FALL_LARGE_GLOW_SIZE +
		2 * STARFIELD_FALL_SMALL_GLOW_SIZE * STARFIELD_FALL_SMALL_GLOW_SIZE
} PARTICLE_STREAMS_SHEET_PICTURE;

/* The burst's flashes in PARTICLES, then its embers, and how many of each it
   draws. */
#define STARFIELD_FALL_FLASHES			  128
#define STARFIELD_FALL_EMBER_COUNT		  512
#define STARFIELD_FALL_EMBERS			  (&PARTICLES[STARFIELD_FALL_FLASHES])
#define STARFIELD_FALL_FLASHES_DRAWN	  64
#define STARFIELD_FALL_EMBERS_DRAWN		  64
/* The fall's flames, from state->particles[0], and its landings after them. */
#define STARFIELD_FALL_FLAME_COUNT		  32
#define STARFIELD_FALL_LANDINGS			  16
/* The starfield's stars. */
#define STARFIELD_FALL_STARS			  256
/* How far down each column's ragged edge starts: the dissolve sweeps the
   picture's rows and this many more. */
#define STARFIELD_FALL_RAGGED_EDGE		  64
/* The backdrop's glow turns blue, and later green, once its level passes
   these. */
#define STARFIELD_FALL_GLOW_BLUE_LEVEL	  32
#define STARFIELD_FALL_GLOW_GREEN_LEVEL	  80
/* The burst's flashes rise from this tick on, three more a tick, and the
   background brightens from STARFIELD_FALL_BRIGHTEN_DELAY ticks after it. */
#define STARFIELD_FALL_FLASH_TICK		  16
#define STARFIELD_FALL_BRIGHTEN_DELAY	  20
/* The blast sprite sheet's flash pictures, each shown two ticks. */
#define STARFIELD_FALL_FLASH_PICTURES	  7
/* An ember's depth is held between these: the nearest are drawn as mote 9, each
   PARTICLE_STREAMS_EMBER_SIZE_DEPTH deeper a size smaller. */
#define PARTICLE_STREAMS_EMBER_NEAR		  160
#define PARTICLE_STREAMS_EMBER_FAR		  799
#define PARTICLE_STREAMS_EMBER_SIZE_DEPTH 64

/* The mote pictures in the blast sprite sheet, after its seven blast pictures
   (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart. */
#define STARFIELD_FALL_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/* Meteor and Outer Space: the fall through the starfield, then the burst. */
void btEffectRunStarfieldFall(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	Uint8				 offsets[128];
	void				*canvas;
	DRAW_RECTANGLE		 draw;
	Sint32				 direction;
	BATTLE_EFFECT_STATE *state;

	/* Starts a landing where a fireball reaches the ground; it is never
	   drawn. */
	void syParticleStreamsSpawnLanding(Sint32 x, Sint32 y, Sint32 velocityX)
	{
		Sint32 i;

		for(i = 0; i != STARFIELD_FALL_LANDINGS; i++) {
			PARTICLE *landing =
				&state->particles[STARFIELD_FALL_FLAME_COUNT + i];

			if(landing->variant == -1) {
				landing->variant = 0;
				landing->x = x;
				landing->y = y;
				landing->velocityX = velocityX;
				break;
			}
		}
	}

	/* Unused: starts a flame; the fall starts its own inline. */
	UNUSED void syParticleStreamsSpawnFlame(Sint32 x, Sint32 y)
	{
		Sint32 i;

		for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
			PARTICLE *flame = &state->particles[i];

			if(flame->variant == -1) {
				flame->variant = 0;
				flame->x = x;
				flame->y = y;
				break;
			}
		}
	}

	FIXED		 pos[4];
	FIXED		 vector[3];
	SCREEN_POINT spot;
	SCREEN_POINT screen;
	SCALE		 scale;
	Sint32		 frame;
	Sint32		 i;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);

	/* In Outer Space the Fusion Dragon leaps out of sight. The stream then runs
	   rightward (direction 1) when the party casts, else leftward (-1). */
	direction = mode;
	if(direction == STARFIELD_FALL_OUTER_SPACE) {
		MOTION_OBJECT *user = btObjectGetStub(args->user)->object;

		user->velocityY = toFIXED(10);
		user->verticalMotionStrength = toFIXED(57) / 100;
		btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, TINT_KEEP, 0);
		sySoundPlayStub(SE_CRASH);
		direction = args->side == EFFECT_SIDE_PARTY ? mode : -1;
	} else {
		direction = -1;
	}

	/* The backdrop and colour 1 go black for the wipe. */
	btEffectConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	syCloseEvent(btEffectTransferCanvas);

	/* The Fusion Dragon's two halves, or the meteor. */
	if(mode == STARFIELD_FALL_OUTER_SPACE) {
		for(i = 0; i != 2; i++) {
			SPR *sprite = fdSpriteCreateStub(
				SPRITE_BATTLE_FUSION_DRAGON + i * SPRITE_SECOND_PART);

			state->sprites[i] = sprite;
			if(sprite != 0) {
				sprite->flags = 0;
				fdSpriteStartAnimationStub(sprite, 2);
				((SPR *)state->sprites[i])->image.priority = 3;
			}
		}
	} else {
		btEffectCreateSprites(1, SPRITE_METEOR, 3);
	}
	/* The starfield. */
	btResourceLoadAndDecompress(RESOURCE_FLAME_SHEET_B, state->sheet, 1, 1);
	if(mode == STARFIELD_FALL_OUTER_SPACE)
		syWordsCopy((void *)BG_PLTT,
			syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
			SHEET_PALETTE_BYTES);

	/* BG1's picture all colour 1, black, for the dissolve to erase. */
	IWRAM_FILL_WORDS(RamMapMetatileBuffer, 256 * 128, CANVAS_FILL_WORD(1));
	syWordsCopy((void *)BG_CHAR_BLOCK(2), RamMapMetatileBuffer,
		BATTLE_BACKGROUND_BYTES);
	sySynch(1);
	REG_BLDCNT = 0;
	REG_BG2PA = Q_8_8(1);
	REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;

	/* The stars: pale colours scattered over the canvas. */
	for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
		Sint32 red = (syRandom() & 15) + 16;
		Sint32 green = (syRandom() & 15) + 16;
		Sint32 blue = (syRandom() & 15) + 16;

		BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
	}
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);

	for(i = 0; i != STARFIELD_FALL_STARS; i++) {
		Sint32 x = syRandom() & (CANVAS_SIZE - 1);
		Sint32 y = syRandom() & (CANVAS_SIZE - 1);
		Sint32 shade = (syRandom() & 63) + 64;

		((Uint8 *)canvas)[CANVAS_PIXEL_OFFSET(x, y)] = shade;
	}
	syWordsCopy((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
	/* The dissolve: the map is erased from the top in ragged columns. */
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectShowLivingTargets(state->args);

	state->bg2x = 0;
	state->bg2y = 0;
	state->scrollTimer = 0;
	state->scrollInterval = 2;
	/* Half a pixel a step with the stream and a pixel down, in 20.8. */
	state->scrollStepX = direction << 7;
	state->scrollStepY = 1 << 8;
	sySetEvent(btEffectAdvanceBg2OnInterval, EVENT_PASS_VBLANK + 127);
	sySetEvent(btEffectTransferFullScreenImage, EVENT_PASS_VBLANK);

	/* Each column starts at its own random row, and the rows go faster and
	   faster. */
	for(i = 0; i != ARRAY_COUNT(offsets); i++)
		offsets[i] = syRandom() & (STARFIELD_FALL_RAGGED_EDGE - 1);
	{
		Sint32 limit = 0;
		Sint32 step = 1;
		Sint32 row = 0;

		do {
			limit += step / 2;
			step += 4;
			for(; row != limit; row++) {
				Sint32 x;

				for(x = 0; x != 256; x++) {
					Sint32 y = row - offsets[x & 127];

					if(y >= 0)
						if(y <= 127)
							RamMapMetatileBuffer[((y / 8 * 32 + x / 8) * 8 +
													 (y & 7)) *
									8 +
								(x & 7)] = 0;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		} while(limit < CANVAS_SIZE + STARFIELD_FALL_RAGGED_EDGE);
	}
	/* The fall. */
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	{
		Sint32					 savedX = BgScrollWork[1].x;
		Sint32					 savedY = BgScrollWork[1].y;
		BATTLE_BACKGROUND_STATE *transition = RamHeapSlots->battleBg;
		Sint32					 transferMode = CANVAS_COPY_AND_DARKEN;

		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		btEffectBuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7,
			COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, BLITTER_MAX);
		draw = RamHeapSlots->ramCode;
		EFFECT_SET_TRANSFER(state, transferMode, CANVAS_FILL_WORD(2));
		sySetEvent(btEffectArmPaletteHBlankDMA, EVENT_PASS_VBLANK + 126);
		EFFECT_FREE_PARTICLES(i, state->particles, 64);
		transition->busy = 1;
		state->frame = 0;
		frame = 0;
		do {
			Sint32	growth = frame << 8;
			Sint32	value = state->frame;
			Sint32	dividend;
			Sint32	glow;
			Uint16 *line;
			Sint32	spriteScale;
			Sint32	x;
			Sint32	y;
			Sint32	hold;

			/* FAKEMATCH(temporary): the division written out on a copy of
			 * value. */
			dividend = value;
			/* FAKEMATCH(dead): updates of value that cancel out. */
			if(state) {
				value++;
				value--;
			}
			if(dividend < 0)
				dividend += 3;
			glow = dividend >> 2;
			/* FAKEMATCH(barrier): hold set by an empty asm that reads value and
			 * clobbers r1 and r6. */
			asm("" : "=r"(hold) : "r"(value) : "r1", "r6");
			line = state->backdropLines;

			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				if((PerWork.repeat & (KEY_A | KEY_B)) && frame > 16)
					break;
			} else {
				if((PerWork.repeat & (KEY_A | KEY_B)) && frame > 4)
					break;
			}
			if(frame == 0)
				sySoundPlayStub(SE_RUMBLE);
			/* FAKEMATCH(dead): updates of hold that cancel out. */
			if(hold) {
				hold++;
				hold--;
			}

			/* The backdrop: a blue glow that climbs and pales as the fall goes
			   on, over the battle view. Entry i colours line i + 1: the HBlank
			   DMA writes it after line i. */
			for(i = 0; i != BATTLE_VIEW_TOP - 1; i++)
				*line++ = 0;
			for(; i != BATTLE_VIEW_BOTTOM - 1; i++) {
				Sint32 green = i - BATTLE_VIEW_TOP;
				Sint32 blue = green / 4;

				value = blue + glow;
				blue = value - STARFIELD_FALL_GLOW_BLUE_LEVEL;
				green = value - STARFIELD_FALL_GLOW_GREEN_LEVEL;

				CLAMP(blue, 0, RGB_LEVEL_MAX);
				CLAMP(green, 0, RGB_LEVEL_MAX);
				*line++ = RGB(green >> 1, green, blue);
			}
			for(; i != DISPLAY_HEIGHT; i++)
				*line++ = 0;

			/* The falling sprite drifts with the stream and grows. */
			if(direction == 1)
				x = frame / 4;
			else
				x = 64 - frame / 4;
			y = 96 - frame;
			pos[3] = 0;
			pos[1] = toFIXED(255);
			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				spriteScale = growth + toFIXED(5) / 8;
				scale.x = spriteScale;
				scale.y = spriteScale;
				pos[0] = toFIXED(x) + toFIXED(80);
				pos[2] = toFIXED(64 - y);
				fdRenderApplyProjectedPlacementStub(
					state->sprites[0], pos, &scale, 0);
				fdRenderApplyProjectedPlacementStub(
					state->sprites[1], pos, &scale, 0);
			} else {
				spriteScale = growth + toFIXED(1);
				scale.x = spriteScale;
				scale.y = spriteScale;
				pos[0] = toFIXED(x) + toFIXED(96);
				pos[2] = toFIXED(96 - y);
				fdRenderApplyProjectedPlacementStub(
					state->sprites[0], pos, &scale, 0);
			}

			/* A new flame below the falling sprite. */
			for(i = 0, y = 32 - y; i != STARFIELD_FALL_FLAME_COUNT; i++) {
				PARTICLE *flame = &state->particles[i];

				if(flame->variant == -1) {
					Sint32 angle = (syRandom() & 0x7fff) + DEGtoANG(90);

					flame->variant = 0;
					flame->x = INTtoFIXED(x + 96) +
						sySin(angle) * 30 / toFIXED(1) * spriteScale;
					flame->y = toFIXED(y) -
						syCos(angle) * 30 / toFIXED(1) * spriteScale;
					break;
				}
			}

			/* Seven sparks orbit the falling sprite. The orbit's centre is set
			   in screen, which the burst's embers are later projected into: the
			   tick has no other slot for it. */
			screen.x = 0;
			screen.y = 0;
			screen.depth = toFIXED(512);
			syInitMatrix();
			syMatrixTranslateVector(syScreenPointCoordinates(&screen));
			syRotZ(TRIG_TURN / 32);
			syRotY(frame << 8);
			for(i = 0; i != 7; i++) {
				vector[0] = toFIXED(StarfieldFallOrbitPoints[i][0] / 2);
				vector[1] = toFIXED(StarfieldFallOrbitPoints[i][1] + frame);
				vector[2] = toFIXED(StarfieldFallOrbitPoints[i][2] / 2);
				btEffectPosProjectPoint(vector, &spot);
				spot.x = FIXEDtoINT(spot.x) + 128;
				spot.y = FIXEDtoINT(spot.y) + 60;
				draw(RamMapMetatileBuffer,
					state->sheet + STARFIELD_FALL_SMALL_GLOW,
					spot.x - STARFIELD_FALL_SMALL_GLOW_SIZE / 2,
					spot.y - STARFIELD_FALL_SMALL_GLOW_SIZE / 2,
					STARFIELD_FALL_SMALL_GLOW_SIZE,
					STARFIELD_FALL_SMALL_GLOW_SIZE);
			}

			/* Seven fireballs fall; one that reaches the ground becomes a
			   landing. */
			if(direction == 1)
				x = frame / 4 - 16;
			else
				x = 16 - frame / 4;
			y = frame - 96;
			for(i = 0; i != 7; i++) {
				Sint32 fireballY = StarfieldFallFireballPoints[i][1] + y;

				if(fireballY <= 93)
					draw(RamMapMetatileBuffer,
						state->sheet + STARFIELD_FALL_LARGE_GLOW,
						StarfieldFallFireballPoints[i][0] + x -
							STARFIELD_FALL_LARGE_GLOW_SIZE / 2,
						fireballY - STARFIELD_FALL_LARGE_GLOW_SIZE / 2,
						STARFIELD_FALL_LARGE_GLOW_SIZE,
						STARFIELD_FALL_LARGE_GLOW_SIZE);
				else if(fireballY <= 95)
					syParticleStreamsSpawnLanding(
						toFIXED(StarfieldFallFireballPoints[i][0] + x),
						toFIXED(fireballY), 1);
			}

			for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
				PARTICLE *flame = &state->particles[i];

				/* A flame rises against the stream through its six pictures. */
				if(flame->variant >= 0) {
					draw(RamMapMetatileBuffer,
						state->sheet + STARFIELD_FALL_FLAMES +
							flame->variant *
								(STARFIELD_FALL_FLAME_SIZE *
									STARFIELD_FALL_FLAME_SIZE),
						FIXED_INTEGER_PART(flame->x) -
							STARFIELD_FALL_FLAME_SIZE / 2,
						FIXED_INTEGER_PART(flame->y) -
							STARFIELD_FALL_FLAME_SIZE / 2,
						STARFIELD_FALL_FLAME_SIZE, STARFIELD_FALL_FLAME_SIZE);
					flame->x -= direction * (toFIXED(5) / 4);
					flame->y -= toFIXED(5);
					flame->variant++;
					if(flame->variant == 6)
						flame->variant = -1;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
			frame++;
			state->frame++;
		} while(frame != STARFIELD_FALL_FALL_TICKS);
		sySynch(1);
		transition->busy = 0;
		syCloseEvent(btEffectAdvanceBg2OnInterval);
		syCloseEvent(btEffectArmPaletteHBlankDMA);
		syCloseEvent(btEffectTransferFullScreenImage);
		BgScrollWork[1].x = savedX;
		BgScrollWork[1].y = savedY;
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	/* The burst: embers, rising motes and growing flashes from the impact. */
	btEffectSetupBlendedDisplay();
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7) |
		BGCNT_WRAP;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw = RamHeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 0);
	for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
		PARTICLE *mote = &state->particles[i];

		mote->x = syRandom() & 127;
		mote->y = (syRandom() & 127) + 127;
	}
	for(i = 0; i != STARFIELD_FALL_FLASHES; i++) {
		PARTICLE *flash = &PARTICLES[i];

		SET_POSITION(flash, 0, 0, 0);
		SET_VELOCITY(flash, ((syRandom() & 255) - 127) << 12,
			(syRandom() & 255) << 11, ((syRandom() & 255) - 127) << 12);
		flash->variant = 0;
	}
	for(i = 0; i != STARFIELD_FALL_EMBER_COUNT; i++) {
		PARTICLE *ember = &STARFIELD_FALL_EMBERS[i];

		SET_POSITION(ember, 0, 0, 0);
		SET_VELOCITY(ember, ((syRandom() & 255) - 128) << 13,
			(syRandom() & 255) << 11, ((syRandom() & 255) - 128) << 13);
		ember->variant = 0;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

	for(frame = 0; frame != STARFIELD_FALL_BURST_TICKS; frame++) {
		Sint32		   dragonScale = (frame + 464) << 8;
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
		Sint32		   elapsed = frame - STARFIELD_FALL_FLASH_TICK;
		SCALE		   scale;
		Sint32		   x;
		Sint32		   y;
		Sint32		   grow;

		if(elapsed >= STARFIELD_FALL_BRIGHTEN_DELAY)
			btPaletteBrightenBgEntries(2, 2, 2);
		if(frame == 0)
			sySoundPlayStub(SE_SWITCH_MECHANISM);
		if(frame == 40)
			sySoundPlayStub(SE_CRASH);
		if(frame == PARTICLE_STREAMS_DAMAGE_TICK) {
			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				fdSpriteDestroyStub(state->sprites[0]);
				fdSpriteDestroyStub(state->sprites[1]);
				btObjectPutStub();
			}
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}

		EFFECT_VIEW_FROM_CAMERA(camera);
		for(i = 0; i != STARFIELD_FALL_EMBERS_DRAWN; i++) {
			/* Embers fall and bounce back up from the ground. */
			PARTICLE *ember = &STARFIELD_FALL_EMBERS[i];

			if(ember->y >= 0) {
				Sint32 size;

				btEffectPosProjectPoint(&ember->x, &screen);
				screen.x >>= 1;
				if(screen.depth < PARTICLE_STREAMS_EMBER_NEAR)
					screen.depth = PARTICLE_STREAMS_EMBER_NEAR;
				if(screen.depth > PARTICLE_STREAMS_EMBER_FAR)
					screen.depth = PARTICLE_STREAMS_EMBER_FAR;
				size = 9 -
					(screen.depth - PARTICLE_STREAMS_EMBER_NEAR) /
						PARTICLE_STREAMS_EMBER_SIZE_DEPTH;
				/* UB: from depth 736 the size is 0, a picture 0 pixels wide,
				   and its picture is read from BattleEffectMotePictures[-1];
				   UBFIX draws the farthest embers at the smallest size. */
#if FIX_ZERO_SIZE_EMBER
				if(size < 1)
					size = 1;
#endif
				draw(canvas,
					state->sheet +
						(BattleEffectMotePictures[size - 1] +
							(i & 1) * MOTE_SET_BYTES) +
						STARFIELD_FALL_MOTE_PICTURES,
					screen.x - size / 2, screen.y - size, size, size * 2);
				btParticleAdvanceWithGravity3D(
					ember, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
				if(ember->y <= toFIXED(20)) {
					ember->x = 0;
					ember->z = 0;
					ember->y = toFIXED(20);
					SET_VELOCITY(ember, ((syRandom() & 63) - 32) << 15,
						(syRandom() & 63) << 13,
						((syRandom() & 63) - 32) << 15);
				}
			}
		}

		for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
			PARTICLE *mote = &state->particles[i];
			/* Motes rise up the screen and wrap round. */
			Uint32 rise = i & 7;
			Sint32 size = rise + 3;

			draw(canvas,
				state->sheet +
					(BattleEffectMotePictures[size - 1] +
						(i & 1) * MOTE_SET_BYTES) +
					STARFIELD_FALL_MOTE_PICTURES,
				mote->x - size / 2, mote->y - size, size, size * 2);
			mote->y = mote->y - rise - 8;
			if(mote->y < -10)
				mote->y = 128;
		}

		for(i = 0; i != STARFIELD_FALL_FLASHES_DRAWN; i++) {
			/* Flashes grow and rise, three more each tick. */
			PARTICLE *flash = &PARTICLES[i];

			if(i / 3 < elapsed && flash->y >= 0) {
				btEffectPosProjectPoint(&flash->x, &screen);
				screen.x >>= 1;
				if((Uint32)flash->variant < 2 * STARFIELD_FALL_FLASH_PICTURES) {
					Sint32 picture = flash->variant / 2;

					draw(canvas,
						state->sheet + StarfieldFallFlashPictures[picture],
						screen.x - StarfieldFallFlashSizes[picture] / 2,
						screen.y - StarfieldFallFlashSizes[picture] / 2,
						StarfieldFallFlashSizes[picture],
						StarfieldFallFlashSizes[picture]);
				}
				flash->variant++;
				if(flash->variant == 2 * STARFIELD_FALL_FLASH_PICTURES) {
					flash->y = toFIXED(20);
					flash->x = 0;
					flash->z = toFIXED((syRandom() & 255) - 127);
					SET_VELOCITY(flash, 0, (syRandom() & 255) << 11, 0);
					flash->variant = 0;
				} else {
					btParticleAdvanceWithGravity3D(
						flash, PARTICLE_DAMPING_NONE, 1);
				}
			}
		}

		/* The falling sprite comes down onto the targets, growing. */
		if(direction == 1)
			x = frame / 2 + 24;
		else
			x = 56 - frame / 2;
		y = 64 - frame * 2;
		grow = (frame << 8) + toFIXED(2);
		pos[3] = 0;
		pos[1] = toFIXED(255);
		if(mode == STARFIELD_FALL_OUTER_SPACE) {
			scale.x = dragonScale;
			scale.y = dragonScale;
			pos[0] = toFIXED(x) + toFIXED(96);
			pos[2] = toFIXED(96 - y);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[0], pos, &scale, 0);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[1], pos, &scale, 0);
		} else {
			scale.x = grow;
			scale.y = grow;
			pos[0] = toFIXED(x) + toFIXED(96);
			pos[2] = toFIXED(96 - y);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[0], pos, &scale, 0);
		}
		state->shakeFrame = 1;
		btCameraShake(8, 8);
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(mode == STARFIELD_FALL_METEOR)
		fdSpriteDestroyStub(state->sprites[0]);
	btEffectEndCanvasLayer();
}

#undef FORMATION_SPACING
#undef STARFIELD_FALL_FALL_TICKS
#undef STARFIELD_FALL_BURST_TICKS
#undef PARTICLE_STREAMS_DAMAGE_TICK
#undef STARFIELD_FALL_FLAME_SIZE
#undef STARFIELD_FALL_LARGE_GLOW_SIZE
#undef STARFIELD_FALL_SMALL_GLOW_SIZE
#undef STARFIELD_FALL_FLASHES
#undef STARFIELD_FALL_EMBER_COUNT
#undef STARFIELD_FALL_EMBERS
#undef STARFIELD_FALL_FLASHES_DRAWN
#undef STARFIELD_FALL_EMBERS_DRAWN
#undef STARFIELD_FALL_FLAME_COUNT
#undef STARFIELD_FALL_LANDINGS
#undef STARFIELD_FALL_STARS
#undef STARFIELD_FALL_RAGGED_EDGE
#undef STARFIELD_FALL_GLOW_BLUE_LEVEL
#undef STARFIELD_FALL_GLOW_GREEN_LEVEL
#undef STARFIELD_FALL_FLASH_TICK
#undef STARFIELD_FALL_BRIGHTEN_DELAY
#undef STARFIELD_FALL_FLASH_PICTURES
#undef PARTICLE_STREAMS_EMBER_NEAR
#undef PARTICLE_STREAMS_EMBER_FAR
#undef PARTICLE_STREAMS_EMBER_SIZE_DEPTH
#undef STARFIELD_FALL_MOTE_PICTURES
