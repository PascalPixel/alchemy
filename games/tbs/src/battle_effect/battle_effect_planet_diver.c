/* Planet Diver, Haunt and the orbiting spirals. Part of battle_effect.c. */

/* One chip of Planet Diver's impact. Chips are kept in particle-sized slots,
   but with their own layout: the x velocity where a particle keeps z. */
typedef struct IMPACT_CHIP {
	FIXED  x;		   /* canvas position, 16.16 fixed point */
	FIXED  y;		   /* canvas position, 16.16; the ground is y 112 */
	FIXED  velX;	   /* 16.16 pixels a tick, damped to 56/64 each tick */
	Sint32 unusedVelX; /* a Particle's velocityX; never read */
	FIXED  velY;	   /* 16.16 pixels a tick, with 1/8 pixel of gravity */
	Sint32 unusedVelZ; /* a Particle's velocityZ; never read */
	Sint32 life;	   /* ticks left; 0 is free */
} IMPACT_CHIP;

/* RamMapGridMetatileBuffer seen as Planet Diver's chips. */
#define PLANET_DIVER_IMPACT_CHIPS ((IMPACT_CHIP *)RamMapGridMetatileBuffer)

#define PLANET_DIVER_TICKS 88
#define PLANET_DIVER_CLEARED_CHIPS \
	1024							  /* map grid buffer slots cleared first */
#define PLANET_DIVER_CHIPS		  256 /* chips the fireball bursts into */
#define PLANET_DIVER_FLASH_BURSTS 16  /* the ring of flashes round the user */

/* The user flashes and leaps into the air, turns into a ball of fire, falls
   onto the first target and bursts into chips that bounce on the ground.

   Abilities: Planet Diver. */
void btEffectRunPlanetDiver(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screenPos;
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*motes;
	ACTOR				*tgt;
	FIXED				 stepX;
	ACTOR				*user;
	Uint8				*resource;
	Sint32				 frm;
	Sint32				 i;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);
	resource = syResourceGetTableEntry(RESOURCE_FLASH_BURST_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
		PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
	user = btObjectGetStub(state->args->user)->actor;
	tgt = btObjectGetStub(state->args->tgts[0])->actor;
	if(user->x.fixed > 0)
		stepX = -toFIXED(15);
	else
		stepX = toFIXED(15);

	for(frm = 0; frm != PLANET_DIVER_TICKS; frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;

		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frm > 17 || frm == 0) {
			btProjectionUnit(state->args->user, &screenPos);
			screenPos.x /= 2;
		}

		/* The flash spreads from the user. */
		if(frm >= 2 && frm <= 3)
			draw[0](canvas, state->sheet, screenPos.x - FLASH_BURST_WIDTH / 2,
				screenPos.y - FLASH_BURST_HEIGHT, FLASH_BURST_WIDTH,
				FLASH_BURST_HEIGHT);
		if(frm >= 4 && frm <= 15) {
			for(i = 0; i != PLANET_DIVER_FLASH_BURSTS; i++) {
				Sint32 ang = i * (TRIG_TURN / PLANET_DIVER_FLASH_BURSTS);
				Sint32 x = screenPos.x + FIXEDtoINT(frm * sySin(ang));
				Sint32 y = screenPos.y + FIXEDtoINT(frm * syCos(ang)) - frm;

				draw[0](canvas,
					state->sheet + (frm - 4) / 2 * PICTURE_BYTES(FLASH_BURST),
					x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT,
					FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
			}
		}
		/* The user leaps, heading out past its own position. */
		if(frm == 4) {
			user->velY = toFIXED(20);
			user->acceleration = toFIXED(1);
			user->speedLimit = toFIXED(3);
			user->gravity = BATTLE_UNIT_GRAVITY;
			user->steeringFlg = 0;
			user->snapToTgt = 0;
			fdActorSetMoveTargetStub(user, user->x.fixed * 3, 0, user->z.fixed);
			fdActorSelectAnimationStub(user, POSE_LEAP);
			state->shakeFrm = 4;
			sySndPlayStub(SE_WHOOSH);
		}
		if(frm == 16) {
			resource = syResourceGetTableEntry(RESOURCE_FLAMEBALL_SHEET);
			syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
			resource += SHEET_PALETTE_BYTES;
			syDecompressTaggedLZ(resource, state->sheet);
			user->gravity = 0;
			user->velX = 0;
			user->velY = 0;
			user->z.fixed = tgt->z.fixed;
			fdActorStartIdleStub(user);
		}
		if(frm > 17) {
			if(user->y.fixed > 0) {
				user->x.fixed += stepX;
				user->y.fixed -= toFIXED(8);
				/* The nudge after drawing lasts only to this tick's landing
				   test, where the chips start; the next tick projects the user
				   afresh. On EFFECT_SIDE_PARTY the nudge moves y, so the chips
				   start 8 pixels lower rather than 8 pixels across. XXX (no
				   evidence): whether the party side's nudge was meant to move
				   x, as the enemies' side does; The Lost Age's Planet Diver was
				   rewritten without it, and nothing else shows the intent. */
				if(state->args->side == EFFECT_SIDE_ENEMIES) {
					draw[0](canvas, state->sheet,
						screenPos.x - FLAMEBALL_WIDTH / 2, screenPos.y - 52,
						FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
					screenPos.x -= 8;
				} else {
					draw[1](canvas, state->sheet, screenPos.x - 26,
						screenPos.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
					screenPos.y += 8;
				}
			}
			if(user->y.fixed < 0) {
				user->y.fixed = 0;
				/* Chips fly off the landing. */
				for(i = 0; i != PLANET_DIVER_CHIPS; i++) {
					IMPACT_CHIP *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
					Sint32		 speed;
					Sint32		 ang;

					speed = (syRandom() & 0x3ff) + 32;
					ang = syRandom() & TRIG_ANGLE_MASK;
					chip->x = toFIXED(screenPos.x);
					chip->y = INTtoFIXED(screenPos.y - 24);
					chip->velX = sySin(ang) * speed >> 6;
					chip->velY = -(syCos(ang) * speed << 1) >> 6;
					chip->life = (syRandom() & 7) + 32;
				}
				state->shakeFrm = 8;
				EFFECT_SHOW_DAMAGE(SE_CRASH);
				btMotionApplyKnockbackStub(
					state->args->tgts[0], KNOCKBACK_HURL);
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, 0, 8);
			}
		}

		/* Chips slow, fall and bounce. */
		for(i = 0; i != PLANET_DIVER_CHIPS; i++) {
			IMPACT_CHIP *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
			Sint32		 y;

			if(chip->life > 0) {
				chip->x += chip->velX;
				y = chip->y + chip->velY;
				chip->life--;
				chip->y = y;
				chip->velX = chip->velX * 56 / DAMPING_SCALE;
				chip->velY = chip->velY * 56 / DAMPING_SCALE + toFIXED(1) / 8;
				if(chip->y > toFIXED(112)) {
					chip->velY = -chip->velY / 2;
				} else if((chip->x >= 0 && chip->x < toFIXED(127)) &&
					chip->y >= 0) {
					Sint32 size = chip->life / 8 + 1;

					EFFECT_DRAW_MOTE(draw[i & 1], canvas, motes,
						FIXEDtoINT(chip->x), FIXEDtoINT(chip->y), size);
				}
			}
		}
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* The particles Haunt scatters at random about its centre as it starts, up to
   64 pixels out along each axis. */
#define HAUNT_SCATTERED 512
#define HAUNT_SKULLS	8  /* skulls drawn around each target */
#define HAUNT_SLOTS		64 /* particles reserved for each target */

/* The view ripples as skulls close in on each target in turn.

   Abilities: Haunt. */
void btEffectRunHaunt(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 screenPos;
	Sint32				 point[3];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	BLITTER_PAIR		 draw;
	Uint8				*resource;
	Sint32				 i;
	Sint32				 k;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	resource = syResourceGetTableEntry(RESOURCE_SKULL_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	resource = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));

	for(i = 0; i != HAUNT_SCATTERED; i++) {
		PARTICLES[i].x = ((syRandom() & 255) - 127) << 15;
		PARTICLES[i].y = ((syRandom() & 255) - 127) << 15;
		PARTICLES[i].z = ((syRandom() & 255) - 127) << 15;
	}
	sySndPlayStub(SE_SWIRL);

	for(frm = 0; frm != state->args->tgtCnt * 32 + 96; frm++) {
		Sint32		  *row;
		BATTLE_CAMERA *camera = BattleCameraWork;

		if(frm == 96)
			EFFECT_SHOW_DAMAGE(0);

		row = state->bg2xLines;
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ = CANVAS_WAVE_BG2X(
					toFIXED(6) - sySin((frm + i) * (TRIG_TURN / 32)) * 6);
		} else {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ =
					CANVAS_WAVE_BG2X(sySin((frm + i) * (TRIG_TURN / 32)) * 6);
		}

		EFFECT_FOR_EACH_TARGET(k, state->args) {
			ACTOR *tgt = btObjectGetStub(state->args->tgts[k])->actor;

			/* Spin the skulls about a point above the target. */
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, tgt->x.fixed, toFIXED(20), tgt->z.fixed);
			syMatrixTranslateVector(point);
			if(frm > k * 32) {
				syRotX(frm << 9);
				if(frm == k * 32 + 32)
					btEffectTintBattler(state->args->tgts[k],
						SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 32);

				/* Each skull steps nearer the centre. */
				for(i = 0; i != HAUNT_SKULLS; i++) {
					PARTICLE *skull = &PARTICLES[k * HAUNT_SLOTS + i];

					if(frm > (k * 8 + i) * 4) {
						Sint32 x = (skull->x >> 8) * (skull->x >> 8);
						Sint32 y = (skull->y >> 8) * (skull->y >> 8);
						Sint32 z = (skull->z >> 8) * (skull->z >> 8);
						Sint32 dist = sySquartIWRAM(x + y + z) >> 8;

						if(dist != 0) {
							btProjectionPoint(skull, &screenPos);
							screenPos.x >>= 1;
							draw[0](canvas,
								state->sheet + i % 3 * PICTURE_BYTES(SKULL),
								screenPos.x - SKULL_WIDTH / 2,
								screenPos.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
								SKULL_HEIGHT);
							skull->x -= skull->x / dist;
							skull->y -= skull->y / dist;
							skull->z -= skull->z / dist;
							skull->variant++;
						}
					}
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

/* The spirals: ORBITING_SPIRALS a quarter turn apart, circling
   ORBITING_SPIRALS_RADIUS pixels out, each target's ORBITING_SPIRALS_INTERVAL
   ticks after the one before and for ORBITING_SPIRALS_LIFE ticks less one. */
#define ORBITING_SPIRALS			   4
#define ORBITING_SPIRALS_RADIUS		   16
#define ORBITING_SPIRALS_INTERVAL	   16
#define ORBITING_SPIRALS_LIFE		   60
/* The canvas's scroll on the party side until the first tick's wave rows
   replace it, 8 pixels short of NARROW_CANVAS_PARTY_X. */
#define ORBITING_SPIRALS_PARTY_START_X (NARROW_CANVAS_PARTY_X - 8)

/* Unused: no row of AbilityBattleEffects in either game names its kind, and the
   handler table is reached with no other kind (btEffectRunFromTable), so the
   player never sees it. Staged by hand (Quake's row of AbilityBattleEffects
   given its kind, cast by Isaac at Garet, Ivan and Mia), four spirals circle
   the point 40 pixels above Garet, then Ivan, then Mia, turning a 128th of a
   turn a tick; each target takes its damage pose 32 ticks into its turn. */
void btEffectRunUnusedOrbitingSpirals(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	void				*resource;
	BLITTER_PAIR		 draw;
	Sint32				 point[3];
	SCREEN_POINT		 screen;
	Sint32				 member;
	Sint32				 ofsX;
	Sint32				 frm;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(0);
	resource = syResourceGetTableEntry(RESOURCE_SPIRAL_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	syDecompressTaggedLZ((Uint8 *)resource + SHEET_PALETTE_BYTES, state->sheet);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	if(state->args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(ORBITING_SPIRALS_PARTY_START_X << 8);
		ofsX = -NARROW_CANVAS_PARTY_X;
	} else {
		ofsX = 0;
	}
	for(frm = 0; frm != (state->args->tgtCnt * ORBITING_SPIRALS_INTERVAL) + 48;
		frm++) {
		BATTLE_CAMERA *camera = BattleCameraWork;
		Sint32		  *row;
		Sint32		   i;

		row = state->bg2xLines;
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ = CANVAS_WAVE_BG2X(
					toFIXED(8) - sySin((frm + i) * (TRIG_TURN / 64)) * 8);
		} else {
			for(i = 0; i != DISPLAY_HEIGHT; i++)
				*row++ =
					CANVAS_WAVE_BG2X(sySin((frm + i) * (TRIG_TURN / 64)) * 8) -
					(NARROW_CANVAS_PARTY_X << 8);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR *actor;

			actor = btObjectGetStub(state->args->tgts[member])->actor;
			if(frm > member * ORBITING_SPIRALS_INTERVAL &&
				frm < (member * ORBITING_SPIRALS_INTERVAL) +
						ORBITING_SPIRALS_LIFE) {
				Sint32 spin;

				if(frm == (member * ORBITING_SPIRALS_INTERVAL) + 32) {
					btEffectTintBattler(state->args->tgts[member],
						SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP, 0);
				}
				SET_VECTOR(point, actor->x.fixed, toFIXED(40), actor->z.fixed);
				btProjectionPoint(point, &screen);

				for(i = 0; i != ORBITING_SPIRALS; i++) {
					Sint32 x;
					Sint32 y;
					Sint32 phase;

					spin = frm * (TRIG_TURN / 128) +
						i * (TRIG_TURN / ORBITING_SPIRALS);
					x = (screen.x +
							FIXEDtoINT(sySin(spin) * ORBITING_SPIRALS_RADIUS)) +
						ofsX;
					y = screen.y +
						FIXEDtoINT(syCos(spin) * ORBITING_SPIRALS_RADIUS);
					/* Each spiral turns through its four pictures, mirrored
					   every other 16 ticks. */
					phase = frm / 16;
					draw[phase & 1](canvas,
						state->sheet +
							(frm / 4 - phase * 4) * PICTURE_BYTES(SPIRAL),
						x - SPIRAL_WIDTH / 2, y - SPIRAL_HEIGHT / 2,
						SPIRAL_WIDTH, SPIRAL_HEIGHT);
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

#undef PLANET_DIVER_IMPACT_CHIPS
#undef PLANET_DIVER_TICKS
#undef PLANET_DIVER_CLEARED_CHIPS
#undef PLANET_DIVER_CHIPS
#undef HAUNT_SCATTERED
#undef HAUNT_SKULLS
#undef HAUNT_SLOTS
#undef PLANET_DIVER_FLASH_BURSTS
#undef ORBITING_SPIRALS
#undef ORBITING_SPIRALS_RADIUS
#undef ORBITING_SPIRALS_INTERVAL
#undef ORBITING_SPIRALS_LIFE
#undef ORBITING_SPIRALS_PARTY_START_X
