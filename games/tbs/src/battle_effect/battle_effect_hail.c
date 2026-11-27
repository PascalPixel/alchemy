/* Hail and Ground. Part of battle_effect.c. */

#define HAIL_CHIPS 64 /* ice chips in PARTICLES */

#define GROUND_TICKS 96 /* ticks of Ground */
#define HAIL_TICKS	 64 /* ticks of Hail */

#define HAIL_CHIP_PICTURES \
	PICTURE_BYTES(WATER_WAVE) /* the ice chips, after the wave */
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
	SCREEN_POINT		 origin;
	Sint32				 screenY;
	Sint32				 screenX;
	BLITTER_PAIR		 draw;
	PARTICLE			*chip;
	Sint32				 i;
	Sint32				 frm;
	Sint32				 rise;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args, ELEMENT_MERCURY, state->args->side,
		DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);
	btEffectBuildCanvasBlitters(state->args->side, draw);
	btResourceLoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	btProjectionUnit(state->args->tgts[0], &origin);

	/* The chips burst up from just above the first target. */
	for(i = 0; i != HAIL_CHIPS; i++) {
		Sint32 ang;
		Sint32 speed;

		ang = (syRandom() & 0x7fff) + DEGtoANG(90);
		speed = (syRandom() & 0x1ff) + 128;
		PARTICLES[i].x = toFIXED((origin.x / 2 + (syRandom() & 0xf)) - 8);
		PARTICLES[i].y = INTtoFIXED(origin.y + 8);
		PARTICLES[i].velX = (sySin(ang) * speed) >> 9;
		PARTICLES[i].velY = (syCos(ang) * speed) >> 6;
		PARTICLES[i].z = syRandom() & 0x7f;
		PARTICLES[i].velZ = syRandom() & 0x7f;
		PARTICLES[i].variant = (syRandom() & 0xf) + 32;
	}

	for(frm = 0; frm != HAIL_TICKS; frm++) {
		if(frm > 47)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(64 - frm);
		/* The chips and the Djinni load on tick 1, so on tick 0 the prelude
		   draws whatever the sprite sheet holds. */
		if(frm == 1) {
			btResourceLoadAndDecompress(RESOURCE_ICE_CHIP_SHEET,
				state->sheet + HAIL_CHIP_PICTURES, 1, 1);
			btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
				state->sheet + HAIL_DJINNI_PICTURE, 1, 0);
		}

		/* The small Djinni circles. */
		if(state->args->prelude == 1) {
			Sint32 orbit;
			Sint32 x;
			Sint32 y;

			orbit = frm * (TRIG_TURN / 32);
			x = FIXEDtoINT(-sySin(orbit) * 4) + screenX / 2 - 10;
			y = FIXEDtoINT(syCos(orbit) * 2) + screenY - 22;
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
		for(i = 0; i != HAIL_CHIPS; i++) {
			chip = &PARTICLES[i];
			if(frm >= i / 4 + 4) {
				Sint32 picture;
				Sint32 width;
				Sint32 height;

				picture = (chip->z / 128) & 3;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = HailPictureWidths[picture];
				height = HailPictureHeights[picture];
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + HailPictures[picture],
					FIXEDtoINT(chip->x) - width / 2,
					FIXEDtoINT(chip->y) - height / 2, width, height);
#else
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + HailPictures[picture],
					FIXEDtoINT(chip->x) -
						(width = HailPictureWidths[picture]) / 2,
					FIXEDtoINT(chip->y) -
						(height = HailPictureHeights[picture]) / 2,
					width, height);
#endif
				btParticleAdvanceWithGravity3D(chip, 63, toFIXED(1) / 16);
			}
		}

		if(frm == 8) {
			state->shakeFrm = frm;
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_LAUNCH);
		}

		/* Waves rise along the bottom, drifting away from the user. */
		rise = frm * 4;
		if(rise > WATER_WAVE_HEIGHT)
			rise = WATER_WAVE_HEIGHT;
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
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
/* The portals set out in state->particles from 0, and the first of them, the
   ones drawn. */
#define GROUND_PORTALS			   8
#define GROUND_PORTALS_DRAWN	   6
/* The crackles round the portals in state->particles from GROUND_FIRST_CRACKLE,
   GROUND_CRACKLES_PER_PORTAL a portal. */
#define GROUND_FIRST_CRACKLE	   32
#define GROUND_CRACKLES_PER_PORTAL 4
#define GROUND_CRACKLES			   (GROUND_PORTALS * GROUND_CRACKLES_PER_PORTAL)

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
	ACTOR				*tgt;
	ACTOR				*user;
	SCREEN_POINT		 pos;
	Sint32				 screenX;
	Sint32				 screenY;
	Sint32				 j;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args, ELEMENT_VENUS, state->args->side,
		DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(state->args->side == EFFECT_SIDE_PARTY)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	else
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	btResourceLoadAndDecompress(RESOURCE_PORTAL_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
		state->sheet + GROUND_DJINNI_PICTURE, 1, 1);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	user = btObjectGetStub(state->args->user)->actor;
	tgt = btObjectGetStub(state->args->tgts[0])->actor;
	/* The portals fan out from above the user. */
	/* DEAD: the last two portals are set out but never drawn or moved; the ROM
	   keeps their stores. */
	for(i = 0; i != GROUND_PORTALS; i++) {
		PARTICLE *portal = &state->particles[i];

		SET_POSITION(portal, user->x.fixed, toFIXED(66), user->z.fixed);
		SET_VELOCITY(portal, (i * toFIXED(80)) >> 5,
			(Sint32)toFIXED((syRandom() & 127) - 64) >> 6,
			(Sint32)toFIXED((syRandom() & 255) - 127) >> 5);
		if(portal->x > 0)
			portal->velX = -portal->velX;
		portal->variant = 1;
	}
	for(frm = 0; frm != GROUND_TICKS; frm++) {
		if(frm > 16)
			btEffectStepPaletteToResource(RESOURCE_PORTAL_SHEET);
		/* The Djinni circles above the user, then rises away. */
		if(state->args->prelude == 1) {
			Sint32 ang = frm * (TRIG_TURN / 32);
			Sint32 x = FIXEDtoINT(-sySin(ang) * 4) + screenX / 2 - 10;
			Sint32 y = FIXEDtoINT(syCos(ang) * 2) + screenY - 22;

			if(frm > 16)
				y = y - frm * 2 + 32;
			if(state->args->side == EFFECT_SIDE_PARTY)
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
			if(frm <= 3)
				EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
					state->sheet + GROUND_DJINNI_PICTURE, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			draw[0](canvas, state->sheet + GROUND_DJINNI_PICTURE, x, y,
				SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
		}
		/* Crackles flicker round each portal. */
		/* DEAD: the crackles of the two portals never drawn are rolled too; the
		   ROM keeps the rolls. */
		if((frm & 1) == 0) {
			for(i = 0; i != GROUND_CRACKLES; i++) {
				PARTICLE *crackle = &state->particles[GROUND_FIRST_CRACKLE + i];

				crackle->velX = syRandom() % 6 + 3;
				crackle->velY = GroundCrackleFlips[syRandom() & 3];
			}
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		for(i = 0; i != GROUND_PORTALS_DRAWN; i++) {
			PARTICLE *portal = &state->particles[i];

			if(portal->variant == 1) {
				if(frm > i * 2) {
					Sint32 x;
					Sint32 y;

					btProjectionPoint(&portal->x, &pos);
					pos.x >>= 1;
					x = pos.x - PORTAL_WIDTH / 2;
					y = pos.y - PORTAL_HEIGHT / 2;
					draw[0](canvas, state->sheet, x, y, PORTAL_WIDTH,
						PORTAL_HEIGHT);
					if((frm & 3) <= 1)
						draw[0](canvas, state->sheet + GroundPortalPictures[1],
							x + GroundPortalX[1], y + GroundPortalY[1],
							GroundPortalWidths[1], GroundPortalHeights[1]);
					else
						draw[0](canvas, state->sheet + GroundPortalPictures[2],
							x + GroundPortalX[2], y + GroundPortalY[2],
							GroundPortalWidths[2], GroundPortalHeights[2]);
					for(j = 0; j != GROUND_CRACKLES_PER_PORTAL; j++) {
						PARTICLE *crackle =
							&state->particles[GROUND_FIRST_CRACKLE +
								i * GROUND_CRACKLES_PER_PORTAL + j];
						Sint32 crackleX;
						Sint32 crackleY;

						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							crackle->velY, COMPOSITE_LIGHTEN);
						draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
						if(crackle->velY & 4)
							crackleX = x - GroundPortalWidths[crackle->velX] -
								GroundPortalX[crackle->velX] + 24;
						else
							crackleX = x + GroundPortalX[crackle->velX];
						if(crackle->velY & 8)
							crackleY = y - GroundPortalHeights[crackle->velX] -
								GroundPortalY[crackle->velX] + 48;
						else
							crackleY = y + GroundPortalY[crackle->velX];
						draw[1](canvas,
							state->sheet + GroundPortalPictures[crackle->velX],
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
					portal->velX += (tgt->x.fixed - portal->x) >> 8;
					portal->velY += (toFIXED(20) - portal->y) >> 8;
					portal->velZ += (tgt->z.fixed - portal->z) >> 8;
					if(frm < i * 2 + 85) {
						SET_VELOCITY(portal, portal->velX * 60 / DAMPING_SCALE,
							portal->velY * 60 / DAMPING_SCALE,
							portal->velZ * 60 / DAMPING_SCALE);
					}
					if(portal->y < toFIXED(20)) {
						state->shakeFrm = 8;
						portal->variant = 0;
						sySndPlayStub(SE_HEAVY_IMPACT);
						EFFECT_STRIKE_TARGET(state->args, 0, 4, KNOCKBACK_HURL);
					}
				}
			}
		}
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Shakes the battle view by up to rangeX and rangeY pixels while the effect's
   shakeFrm last, then puts it back where it was. */
void btCameraShake(Sint32 rangeX, Uint32 rangeY)
{
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

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

#undef HAIL_CHIPS
#undef GROUND_TICKS
#undef HAIL_TICKS
#undef HAIL_CHIP_PICTURES
#undef PORTAL_WIDTH
#undef PORTAL_HEIGHT
#undef HAIL_DJINNI_PICTURE
#undef GROUND_DJINNI_PICTURE
#undef GROUND_FIRST_CRACKLE
#undef GROUND_CRACKLES
#undef GROUND_CRACKLES_PER_PORTAL
#undef GROUND_PORTALS
#undef GROUND_PORTALS_DRAWN
