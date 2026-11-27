/* The Nereid summon. Part of battle_effect.c. */

/* Nereid's drops burst up from the targets, fall back and later rise away; one
   that falls back into the waves frees its slot. The Titan Blade throws its
   dust the same way. */

#define NEREID_TICKS			124
/* The columns of water rise among the targets, from the crash to the
   clatter. */
#define NEREID_COLUMNS_TICK		32
#define NEREID_COLUMNS_END_TICK 80

#define NEREID_OBJECTS 12 /* the sprites that draw Nereid on her turtle */
/* Wrapping columns of water; NereidColumns has a fourth row that nothing
   draws. */
#define NEREID_COLUMNS 3
#define NEREID_DROPS   PARTICLE_POOL_COUNT /* drops of spray in PARTICLES */
#define NEREID_COLUMN_PICTURE \
	1764 /* where the column of water starts in the sheet */
/* The drops burst up: every free one of the first NEREID_BURST_DROPS as the
   columns gather, then NEREID_DROPS_A_TICK a tick while they rise. */
#define NEREID_BURST_DROPS	256
#define NEREID_DROPS_A_TICK 16
/* The canvas line below which a falling drop has sunk into the waves. */
#define NEREID_WAVES_Y		104
/* Throws a drop up from a random point among the targets. */
#define NEREID_THROW_DROP(drop) \
	{ \
		Sint32 radius = syRandom() & 63; \
		Sint32 angle = syRandom() & TRIG_ANGLE_MASK; \
\
		(drop)->x = ((sySin(angle) * radius) >> 3) + toFIXED(32); \
		(drop)->y = ((syCos(angle) * radius) >> 2) + toFIXED(96); \
		(drop)->velocityX = ((syRandom() & 63) - 32) << 14; \
		(drop)->velocityY = (-(syRandom() & 63) - 8) << 13; \
		(drop)->variant = 0; \
	}
/* Particles given a place and a delay that nothing reads. */
#define NEREID_UNREAD_FIRST 24
#define NEREID_UNREAD_COUNT 16

/* Nereid rides in on her turtle over rising waves, columns of water and spray
   rise over the targets, and she bursts away.

   Abilities: Nereid. */
void btEffectRunNereid(BATTLE_EFFECT_PARM *args)
{
	BLITTER_PAIR		 draw;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*motes;
	Uint8				*resource;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);

	resource = syResourceGetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	resource = syResourceGetTableEntry(RESOURCE_FIRE_PILLAR_SHEET_A);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet + NEREID_COLUMN_PICTURE);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);
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

		/* Nereid rides in from the right. */
		rise = 0;
		focus.x = toFIXED(256);
		focus.y = toFIXED(88);
		velocity.x = -toFIXED(16);
		velocity.y = -toFIXED(4);
		EFFECT_FREE_PARTICLES(
			k, state->particles, ARRAY_COUNT(state->particles));
		/* DEAD: particles NEREID_UNREAD_FIRST on get a place and delay nothing
		   reads; the ROM keeps the stores. */
		for(k = 0; k != NEREID_UNREAD_COUNT; k++) {
			point = &state->particles[NEREID_UNREAD_FIRST + k];
			point->x = syRandom() & 127;
			point->y = (syRandom() & 7) + 56;
			point->variant = -(syRandom() & 15);
		}
		EFFECT_FREE_PARTICLES(k, PARTICLES, NEREID_DROPS);
		btEffectShowLivingTargets(state->args);
		sySynch(1);
		btEffectCreateSprites(NEREID_OBJECTS, SPRITE_NEREID, 2);

		for(frame = 0; frame != NEREID_TICKS; frame++) {
			/* A or B, once the columns rise, skips to the last ticks. */
			if((PerRepeat & (KEY_A | KEY_B)) && frame > NEREID_COLUMNS_TICK &&
				frame <= 97)
				frame = 98;
			if(frame == 120)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			/* The waves rise. */
			if(frame <= 15)
				rise += 2;

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
					drop = &PARTICLES[k];
					if(drop->variant == -1) {
						NEREID_THROW_DROP(drop);
					}
				}
			}
			if(frame >= NEREID_COLUMNS_TICK &&
				frame < NEREID_COLUMNS_END_TICK) {
				Sint32 spawned = 0;

				for(k = 0; k != NEREID_DROPS; k++) {
					drop = &PARTICLES[k];
					if(drop->variant == -1) {
						NEREID_THROW_DROP(drop);
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
						NereidColumns[k][NEREID_COLUMN_X] -
							FIRE_PILLAR_WIDTH / 2,
						NereidColumns[k][NEREID_COLUMN_Y] - span -
							FIRE_PILLAR_HEIGHT,
						FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
					draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
						NereidColumns[k][NEREID_COLUMN_X] -
							FIRE_PILLAR_WIDTH / 2,
						NereidColumns[k][NEREID_COLUMN_Y] - span,
						FIRE_PILLAR_WIDTH, span);
				}
			}

			/* The waves along the bottom. */
			if(frame <= 95) {
				for(k = 0; k != 5; k++)
					draw[0](canvas, state->sheet,
						k * 32 + ((frame / 4) & 31) - 32, 120 - rise,
						WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
			}

			/* Drops swell as they clear. */
			for(k = 0; k != NEREID_DROPS; k++) {
				drop = &PARTICLES[k];
				if(drop->variant >= 0) {
					Sint32 size = k % 3 + 2;

					if(drop->velocityY > 0)
						size += 2;
					EFFECT_SWELL_MOTE(size, frame);
					EFFECT_DRAW_MOTE(draw[drop->velocityY > 0], canvas, motes,
						FIXEDtoINT(drop->x), FIXEDtoINT(drop->y), size);
					drop->x += drop->velocityX;
					drop->y += drop->velocityY;
					EFFECT_FALL_MOTE(
						drop->velocityY, NereidGravity[k & 3], frame);
					drop->velocityX = drop->velocityX * 62 / DAMPING_SCALE;
					drop->velocityY = drop->velocityY * 62 / DAMPING_SCALE;
					drop->variant++;
					if(drop->velocityY > 0 &&
						FIXEDtoINT(drop->y) > NEREID_WAVES_Y)
						drop->variant = -1;
				}
			}

			/* The targets bounce and flash white. */
			if(frame <= 79) {
				EFFECT_FOR_EACH_TARGET(k, state->args) {
					if(frame > 29) {
						Sint32 phase = frame % 12;

						if(phase == 0) {
							ACTOR *actor =
								btObjectGetStub(state->args->targets[k])->actor;

							btEffectTintBattler(state->args->targets[k],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, TINT_KEEP, 0);
							actor->velocityY = toFIXED(9) / 2;
							actor->gravity = BATTLE_UNIT_GRAVITY;
						}
						if(phase == 6)
							btEffectTintBattler(state->args->targets[k],
								SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP, 0);
					}
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		}

		syCloseEvent(btEffectTransferCanvas);
		EFFECT_RELEASE_BLITTERS();
		btEffectRunImpactBurst(SUMMON_FORMATION_NEREID, focus.x, focus.y);
		for(k = 0; k != NEREID_OBJECTS; k++)
			fdSpriteDestroyStub(state->sprites[k]);
	}
	btEffectEndCanvasLayer();
}

#undef NEREID_TICKS
#undef NEREID_COLUMNS_TICK
#undef NEREID_COLUMNS_END_TICK
#undef NEREID_OBJECTS
#undef NEREID_COLUMNS
#undef NEREID_DROPS
#undef NEREID_COLUMN_PICTURE
#undef NEREID_BURST_DROPS
#undef NEREID_DROPS_A_TICK
#undef NEREID_WAVES_Y
#undef NEREID_UNREAD_FIRST
#undef NEREID_UNREAD_COUNT
#undef NEREID_THROW_DROP
