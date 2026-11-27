/* Hail and Ground, and the battle camera's shakes. Part of battle_effect.c. */

#define HAIL_CHIPS 64 /* ice chips in PARTICLES */

#define GROUND_TICKS	96 /* ticks of Ground */
#define HAIL_TICKS		64 /* ticks of Hail */
/* Hail's last ticks, which fade the canvas out. */
#define HAIL_FADE_TICKS 16

/* The ice chips, after the wave. */
#define HAIL_CHIP_PICTURES	PICTURE_BYTES(WATER_WAVE)
/* PortalSheet's portal. */
#define PORTAL_WIDTH		24
#define PORTAL_HEIGHT		48
#define HAIL_DJINNI_PICTURE 26048 /* the small Mercury Djinni */

/* After a Mercury Djinni enters, ice chips burst out of the first target and
   waves of water rise along the bottom of the screen.

   Abilities: Hail. */
void btEffectRunHail(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 screen[3];
	Sint32				 world[3];
	SCREEN_POINT		 origin;
	BLITTER_PAIR		 draw;
	PARTICLE			*chip;
	Sint32				 i;
	Sint32				 frm;
	Sint32				 rise;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side, DJINNI_ANCHOR_OVER_USER),
		screen, world);
	btEffectBuildCanvasBlitters(args->side, draw);
	btResourceLoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The chips burst up from just above the first target. */
	btEffectPosProjectBattler(args->tgts[0], &origin);
	for(i = 0; i != HAIL_CHIPS; i++) {
		Sint32 ang;
		Sint32 speed;

		ang = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);
		speed = (syRandom() & 511) + 128;
		PARTICLES[i].x = toFIXED((origin.x / 2 + (syRandom() & 15)) - 8);
		PARTICLES[i].y = INTtoFIXED(origin.y + 8);
		PARTICLES[i].velX = (sySin(ang) * speed) >> 9;
		PARTICLES[i].velY = (syCos(ang) * speed) >> 6;
		PARTICLES[i].z = syRandom() & 127;
		PARTICLES[i].velZ = syRandom() & 127;
		PARTICLES[i].variant = (syRandom() & 15) + 32;
	}

	for(frm = 0; frm != HAIL_TICKS; frm++) {
		if(frm >= HAIL_TICKS - HAIL_FADE_TICKS)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(HAIL_TICKS - frm);
		/* The chips and the Djinni load on tick 1, so on tick 0 the prelude
		   draws whatever the sprite sheet holds. */
		if(frm == 1) {
			btResourceLoadAndDecompress(RESOURCE_ICE_CHIP_SHEET,
				state->sheet + HAIL_CHIP_PICTURES, 1, 1);
			btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
				state->sheet + HAIL_DJINNI_PICTURE, 1, 0);
		}

		/* The small Djinni circles. */
		if(args->prelude == 1) {
			Sint32 orbit;
			Sint32 x;
			Sint32 y;

			orbit = frm * (TRIG_TURN / 32);
			x = FIXEDtoINT(-sySin(orbit) * 4) + screen[0] / 2 -
				SMALL_DJINNI_WIDTH / 2;
			y = FIXEDtoINT(syCos(orbit) * 2) + screen[1] - 22;
			/* DEAD: tick never passes 69, the loop ending at tick 63; the ROM
			   keeps the compare and the rise. */
			if(frm > 69)
				y = (y - frm * 2) + 138;
			draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
				SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			if(frm <= 3)
				draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
		}

		/* The chips fly up, odd ones mirrored. */
		for(i = 0, chip = PARTICLES; i != HAIL_CHIPS; i++, chip++) {
			if(frm >= i / 4 + 4) {
				Sint32 picture;
				Uint32 width;
				Uint32 height;

				picture = (chip->z / 128) & 3;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = HailPictureWidths[picture];
				height = HailPictureHeights[picture];
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + HailPictures[picture],
					FIXED_INTEGER_PART(chip->x) - width / 2,
					FIXED_INTEGER_PART(chip->y) - height / 2, width, height);
#else
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + HailPictures[picture],
					FIXED_INTEGER_PART(chip->x) -
						(width = HailPictureWidths[picture]) / 2,
					FIXED_INTEGER_PART(chip->y) -
						(height = HailPictureHeights[picture]) / 2,
					width, height);
#endif
				btParticleAdvanceWithGravity3D(chip, 63, toFIXED(1) / 16);
			}
		}

		if(frm == 8) {
			state->shakeFrm = frm;
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			EFFECT_STRIKE_TARGET(args, 0, 16, KNOCKBACK_LAUNCH);
		}

		/* Waves rise along the bottom, drifting away from the user. */
		rise = frm * 4;
		if(rise > WATER_WAVE_HEIGHT)
			rise = WATER_WAVE_HEIGHT;
		if(args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != 5; i++)
				draw[0](canvas, state->sheet, (i << 5) - (frm / 4 & 31),
					120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		} else {
			for(i = 0; i != 5; i++)
				draw[0](canvas, state->sheet,
					((i << 5) + (frm / 4 & 31)) - WATER_WAVE_WIDTH, 120 - rise,
					WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		}

		btCameraShake(4, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* The small Venus Djinni in the portal sprite sheet. */
#define GROUND_DJINNI_PICTURE	   26048
/* The crackles round the portals, four a portal, in state->particles from
   GROUND_FIRST_CRACKLE. */
#define GROUND_FIRST_CRACKLE	   32
#define GROUND_CRACKLES			   32
#define GROUND_CRACKLES_PER_PORTAL 4
#define GROUND_PORTALS			   6

/* After a Venus Djinni enters, six glowing portals leave the user, swirl and
   close in on the first target, each striking it as it arrives.

   Abilities: Ground. */
void btEffectRunGround(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 i;
	Sint32				 frm;
	BLITTER_PAIR		 draw;
	BATTLE_CAMERA		*camera;
	MOTION_OBJECT		*user;
	Sint32				 screen[3];
	Sint32				 world[3];
	SCREEN_POINT		 pos;
	Sint32				 j;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	camera = Ram_HeapSlots->cameraState;
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(ELEMENT_VENUS, args->side, DJINNI_ANCHOR_OVER_USER),
		screen, world);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(args->side == EFFECT_SIDE_PARTY)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	else
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_PORTAL_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
		state->sheet + GROUND_DJINNI_PICTURE, 1, 1);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	user = btObjectGetStub(args->user)->obj;
	{
		MOTION_OBJECT *tgt = btObjectGetStub(args->tgts[0])->obj;

		/* The portals fan out from above the user: eight set out, of which the
		   first GROUND_PORTALS fly. */
		for(i = 0; i != 8; i++) {
			PARTICLE *portal = &state->particles[i];

			SET_POSITION(portal, user->x, toFIXED(66), user->z);
			portal->velX = (i * toFIXED(80)) >> 5;
			portal->velY = (Sint32)toFIXED((syRandom() & 127) - 64) >> 6;
			portal->velZ = (Sint32)toFIXED((syRandom() & 255) - 127) >> 5;
			if(portal->x > 0)
				portal->velX = -portal->velX;
			portal->variant = 1;
		}

		for(frm = 0; frm != GROUND_TICKS; frm++) {
			/* The canvas's colours step towards the portal sprite sheet's, and
			   the portals turn from yellow to violet. */
			if(frm > 16)
				btEffectStepPaletteToResource(RESOURCE_PORTAL_SHEET);
			/* The Djinni circles above the user, then rises away. */
			if(args->prelude == 1) {
				Sint32 ang = frm * (TRIG_TURN / 32);
				Sint32 x = FIXEDtoINT(-sySin(ang) * 4) + screen[0] / 2 -
					SMALL_DJINNI_WIDTH / 2;
				Sint32 y = FIXEDtoINT(syCos(ang) * 2) + screen[1] - 22;

				if(frm > 16)
					y = y - frm * 2 + 32;
				if(args->side == EFFECT_SIDE_PARTY)
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						BLITTER_ADDITIVE);
				else
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
				if(frm <= 3)
					(Ram_HeapSlots->ramCodePaired)(canvas,
						state->sheet + GROUND_DJINNI_PICTURE, x, y,
						SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				draw[0](canvas, state->sheet + GROUND_DJINNI_PICTURE, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			}
			/* Crackles flicker round each portal. */
			if((frm & 1) == 0) {
				for(i = 0; i != GROUND_CRACKLES; i++) {
					PARTICLE *crackle =
						&state->particles[GROUND_FIRST_CRACKLE + i];

					crackle->velX = syRandom() % 6 + 3;
					crackle->velY = GroundCrackleFlips[syRandom() & 3];
				}
			}
			EFFECT_VIEW_FROM_CAMERA(camera);
			for(i = 0; i != GROUND_PORTALS; i++) {
				PARTICLE *portal = &state->particles[i];

				if(portal->variant == 1) {
					if(frm > i * 2) {
						Sint32 x;
						Sint32 y;

						btEffectPosProjectPoint(&portal->x, &pos);
						pos.x >>= 1;
						x = pos.x - PORTAL_WIDTH / 2;
						y = pos.y - PORTAL_HEIGHT / 2;
						draw[0](canvas, state->sheet, x, y, PORTAL_WIDTH,
							PORTAL_HEIGHT);
						/* Its glow flickers between the sheet's next two
						   pictures. */
						if((frm & 3) <= 1)
							draw[0](canvas,
								state->sheet + GroundPortalPictures[1],
								x + GroundPortalX[1], y + GroundPortalY[1],
								GroundPortalWidths[1], GroundPortalHeights[1]);
						else
							draw[0](canvas,
								state->sheet + GroundPortalPictures[2],
								x + GroundPortalX[2], y + GroundPortalY[2],
								GroundPortalWidths[2], GroundPortalHeights[2]);
						for(j = 0; j != GROUND_CRACKLES_PER_PORTAL; j++) {
							PARTICLE *crackle =
								&state->particles[GROUND_FIRST_CRACKLE +
									i * GROUND_CRACKLES_PER_PORTAL + j];
							Sint32 crackleX;
							Sint32 crackleY;

							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
								crackle->velY, BLITTER_MAX);
							draw[1] = Ram_HeapSlots->ramCodePaired;
							if(crackle->velY & COMPOSITE_FLIP_X)
								crackleX = x -
									GroundPortalWidths[crackle->velX] -
									GroundPortalX[crackle->velX] + PORTAL_WIDTH;
							else
								crackleX = x + GroundPortalX[crackle->velX];
							if(crackle->velY & COMPOSITE_FLIP_Y)
								crackleY = y -
									GroundPortalHeights[crackle->velX] -
									GroundPortalY[crackle->velX] +
									PORTAL_HEIGHT;
							else
								crackleY = y + GroundPortalY[crackle->velX];
							draw[1](canvas,
								state->sheet +
									GroundPortalPictures[crackle->velX],
								crackleX, crackleY,
								GroundPortalWidths[crackle->velX],
								GroundPortalHeights[crackle->velX]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
						portal->x += portal->velX;
						portal->y += portal->velY;
						portal->z += portal->velZ;
					}
					/* The portal homes in on the target and strikes it. */
					if(frm > i * 2 + 16) {
						portal->velX += (tgt->x - portal->x) >> 8;
						portal->velY += (toFIXED(20) - portal->y) >> 8;
						portal->velZ += (tgt->z - portal->z) >> 8;
						if(frm < i * 2 + 85) {
							portal->velX = portal->velX * 60 / DAMPING_SCALE;
							portal->velY = portal->velY * 60 / DAMPING_SCALE;
							portal->velZ = portal->velZ * 60 / DAMPING_SCALE;
						}
						if(portal->y < toFIXED(20)) {
							state->shakeFrm = 8;
							portal->variant = 0;
							sySndPlayStub(SE_HEAVY_IMPACT);
							EFFECT_STRIKE_TARGET(args, 0, 4, KNOCKBACK_HURL);
						}
					}
				}
			}
			btCameraShake(16, 16);
			EFFECT_END_TICK(state);
		}
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Shakes the battle view while the effect's shake lasts: BG1 and the
   projection's centre move by a random offset each tick, and once the shake
   runs out BG1 goes back to where it was.

   rangeX, rangeY: the offsets' ranges, powers of two. Both offsets are centred
				   on half of rangeY, so with rangeX the smaller the view only
				   shakes to the left. */
void btCameraShake(Sint32 rangeX, Uint32 rangeY)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	if(state->shakeFrm > 0) {
		Sint32 randomX = (rangeX - 1) & syRandom();
		Sint32 halfRange = (Sint32)rangeY / 2;
		Sint32 ofsY = ((rangeY - 1) & syRandom()) - halfRange;
		Sint32 ofsX = randomX - halfRange;

		BgScrollWork[1].x = ofsX;
		BgScrollWork[1].y = ofsY + BATTLE_BACKGROUND_SCROLL_Y;
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X - ofsX;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y - ofsY;
		state->shakeFrm--;
		return;
	}
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = state->savedBg1Y;
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
}

/* Shakes the battle view up and down only, by a random offset within range,
   while the effect's shake lasts. */
void btCameraApplyVerticalShake(Sint32 range)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;

	if(state->shakeFrm > 0) {
		Sint32 ofsY = ((range - 1) & syRandom()) - range / 2;

		BgScrollWork[1].y = ofsY + BATTLE_BACKGROUND_SCROLL_Y;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y - ofsY;
		state->shakeFrm--;
	} else {
		BgScrollWork[1].y = state->savedBg1Y;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	}
}
