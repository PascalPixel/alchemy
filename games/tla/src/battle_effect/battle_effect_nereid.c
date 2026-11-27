/* The Nereid summon. Part of battle_effect.c. */

#define NEREID_TICKS			124
/* The columns of water rise among the targets, from the crash to the
   clatter. */
#define NEREID_COLUMNS_TICK		32
#define NEREID_COLUMNS_END_TICK 80

#define NEREID_OBJECTS		  12 /* the sprites that draw Nereid on her turtle */
/* Wrapping columns of water. */
#define NEREID_COLUMNS		  3
#define NEREID_DROPS		  PARTICLE_POOL_COUNT /* drops of spray in PARTICLES */
/* the column of water in the sprite sheet */
#define NEREID_COLUMN_PICTURE 1764
/* The canvas line below which a falling drop has sunk into the waves. */
#define NEREID_WAVES_Y		  104
/* Particles given a place and a delay that nothing reads. */
#define NEREID_UNREAD_FIRST	  24
#define NEREID_UNREAD_COUNT	  16

/* Nereid rides in on her turtle over rising waves, columns of water and spray
   rise over the targets, and she bursts away. A or B skips ahead to her
   leaving.

   Abilities: Nereid. */
void btEffectRunNereid(BATTLE_EFFECT_PARM *args)
{
	DRAW_RECTANGLE		 draw[2];
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*graphics;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	graphics = Ram_HeapSlots->battleSheet;

	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);
	btResourceLoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A,
		state->sheet + NEREID_COLUMN_PICTURE, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	{
		POINT2_D  focus;
		POINT2_D  vel;
		Sint32	  rise;
		Sint32	  frm;
		Sint32	  k;
		PARTICLE *point;
		PARTICLE *drop;
		PARTICLE *drops;

		/* Nereid rides in from the right. */
		rise = 0;
		focus.x = toFIXED(256);
		focus.y = toFIXED(88);
		vel.x = -toFIXED(16);
		vel.y = -toFIXED(4);
		EFFECT_FREE_PARTICLES(k, state->particles, 64);
		/* DEAD: particles NEREID_UNREAD_FIRST on get a place and delay nothing
		   reads; the ROM keeps the stores. */
		for(k = 0; k != NEREID_UNREAD_COUNT; k++) {
			point = &state->particles[NEREID_UNREAD_FIRST + k];
			point->x = syRandom() & 127;
			point->y = (syRandom() & 7) + 56;
			point->variant = -(syRandom() & 15);
		}

		EFFECT_FREE_PARTICLES(k, PARTICLES, NEREID_DROPS);
		drops = PARTICLES;
		btEffectShowLivingTargets(state->args);
		sySynch(1);
		btEffectCreateSprs(NEREID_OBJECTS, SPRITE_NEREID, 2);

		for(frm = 0; frm != NEREID_TICKS; frm++) {
			if((PerWork.repeat & (KEY_A | KEY_B)) &&
				frm > NEREID_COLUMNS_TICK && frm <= 97)
				frm = 98;
			if(frm == 120)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			/* The waves rise. */
			if(frm <= 15)
				rise += 2;

			/* She slows, and is pushed back right once she passes x = 120. */
			if(frm <= 99) {
				focus.x += vel.x;
				focus.y += vel.y;
				vel.x = vel.x * 58 / DAMPING_SCALE;
				vel.y = vel.y * 56 / DAMPING_SCALE;
				if(focus.x < toFIXED(120))
					vel.x += toFIXED(1) / 2;
			}
			btEffectPlaceFormationSprs(
				SUMMON_FORMATION_NEREID, focus.x, focus.y);

			/* Drops burst up among the targets. */
			if(frm == 28) {
				for(k = 0; k != NEREID_BURST_DROPS; k++) {
					drop = &drops[k];
					if(drop->variant == -1) {
						Sint32 radius = syRandom() & 63;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

						drop->x = ((sySin(angle) * radius) >> 3) + toFIXED(32);
						drop->y = ((syCos(angle) * radius) >> 2) + toFIXED(96);
						drop->velX = ((syRandom() & 63) - 32) << 14;
						drop->velY = (-(syRandom() & 63) - 8) << 13;
						drop->variant = 0;
					}
				}
			}
			if(frm >= NEREID_COLUMNS_TICK && frm < NEREID_COLUMNS_END_TICK) {
				Sint32 spawned = 0;

				for(k = 0; k != NEREID_DROPS; k++) {
					drop = &drops[k];
					if(drop->variant == -1) {
						Sint32 radius = syRandom() & 63;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

						drop->x = ((sySin(angle) * radius) >> 3) + toFIXED(32);
						drop->y = ((syCos(angle) * radius) >> 2) + toFIXED(96);
						drop->velX = ((syRandom() & 63) - 32) << 14;
						drop->velY = (-(syRandom() & 63) - 8) << 13;
						drop->variant = 0;
						spawned++;
						if(spawned == NEREID_DROPS_A_TICK)
							break;
					}
				}
			}
			if(frm == 0)
				sySndPlayStub(SE_SCATTER);
			if(frm == NEREID_COLUMNS_TICK)
				sySndPlayStub(SE_CRASH);
			if(frm == NEREID_COLUMNS_END_TICK)
				sySndPlayStub(SE_CLATTER);

			/* The columns of water scroll up. */
			if(frm >= NEREID_COLUMNS_TICK && frm < NEREID_COLUMNS_END_TICK) {
				for(k = 0; k != NEREID_COLUMNS; k++) {
					Sint32 span =
						(frm * 16 - 256 + k * 25) % FIRE_PILLAR_HEIGHT;

					draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
						NereidColumns[k * 2] - FIRE_PILLAR_WIDTH / 2,
						NereidColumns[k * 2 + 1] - span - FIRE_PILLAR_HEIGHT,
						FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
					draw[0](canvas, state->sheet + NEREID_COLUMN_PICTURE,
						NereidColumns[k * 2] - FIRE_PILLAR_WIDTH / 2,
						NereidColumns[k * 2 + 1] - span, FIRE_PILLAR_WIDTH,
						span);
				}
			}

			/* The waves along the bottom. */
			if(frm <= 95) {
				for(k = 0; k != 5; k++)
					draw[0](canvas, state->sheet,
						k * WATER_WAVE_WIDTH + ((frm / 4) & 31) -
							WATER_WAVE_WIDTH,
						120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
			}

			/* Drops swell as they clear: they fall under gravity until
			   NEREID_COLUMNS_END_TICK, then rise away, and a falling drop that
			   sinks into the waves is freed. */
			for(k = 0; k != NEREID_DROPS; k++) {
				drop = &drops[k];
				if(drop->variant >= 0) {
					Sint32 size = k % 3 + 2;

					if(drop->velY > 0)
						size += 2;
					if(frm > 68 && size <= 5)
						size = 6;
					if(frm > 70 && size <= 6)
						size = 7;
					if(frm > 72 && size <= 7)
						size = 8;
					if(frm > 74 && size <= 8)
						size = 9;
					if(frm > 76)
						size = MOTE_PICTURES;
					EFFECT_DRAW_MOTE(draw[drop->velY > 0], canvas, graphics,
						FIXED_INTEGER_PART(drop->x),
						FIXED_INTEGER_PART(drop->y), size);
					drop->x += drop->velX;
					drop->y += drop->velY;
					if(frm > NEREID_COLUMNS_END_TICK)
						drop->velY += -toFIXED(1) / 2;
					else
						drop->velY += NereidGravity[k & 3];
					drop->velX = drop->velX * 62 / DAMPING_SCALE;
					drop->velY = drop->velY * 62 / DAMPING_SCALE;
					drop->variant++;
					if(drop->velY > 0 &&
						FIXED_INTEGER_PART(drop->y) > NEREID_WAVES_Y)
						drop->variant = -1;
				}
			}

			/* The targets bounce and flash white. */
			if(frm < NEREID_COLUMNS_END_TICK) {
				for(k = 0; k != args->tgtCnt; k++) {
					if(frm > 29) {
						Sint32 phase = frm % 12;

						if(phase == 0) {
							MOTION_OBJECT *tgt =
								btObjectGetStub(args->tgts[k])->obj;

							btEffectTintBattler(args->tgts[k],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, TINT_KEEP, 0);
							tgt->velY = toFIXED(9) / 2;
							tgt->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
						}
						if(phase == 6)
							btEffectTintBattler(args->tgts[k],
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
			fdSprDestroyStub(state->sprs[k]);
	}
	btEffectEndCanvasLayer();
}

/* NEREID_TICKS, NEREID_COLUMNS_TICK, NEREID_COLUMNS_END_TICK,
   NEREID_OBJECTS, NEREID_COLUMNS, NEREID_DROPS, NEREID_COLUMN_PICTURE,
   NEREID_WAVES_Y, NEREID_UNREAD_FIRST and NEREID_UNREAD_COUNT stay defined:
   particle_streams.c's copy of Nereid runs by them. */
