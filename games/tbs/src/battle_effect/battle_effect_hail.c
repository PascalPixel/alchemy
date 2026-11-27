/* Hail and Ground. Part of battle_effect.c. */

#define HAIL_CHIPS 64 /* ice chips in PARTICLES */

#define GROUND_FRAMES 96 /* frames of Ground */
#define HAIL_FRAMES	  64 /* frames of Hail */

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
	Sint32				 frame;
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
		Sint32 angle;
		Sint32 speed;

		angle = (syRandom() & 0x7fff) + DEGtoANG(90);
		speed = (syRandom() & 0x1ff) + 128;
		PARTICLES[i].x = toFIXED((origin.x / 2 + (syRandom() & 0xf)) - 8);
		PARTICLES[i].y = INTtoFIXED(origin.y + 8);
		PARTICLES[i].velX = (sySin(angle) * speed) >> 9;
		PARTICLES[i].velY = (syCos(angle) * speed) >> 6;
		PARTICLES[i].z = syRandom() & 0x7f;
		PARTICLES[i].velZ = syRandom() & 0x7f;
		PARTICLES[i].variant = (syRandom() & 0xf) + 32;
	}

	for(frame = 0; frame != HAIL_FRAMES; frame++) {
		if(frame > 47)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(64 - frame);
		/* The chips and the Djinni load on frame 1, so on frame 0 the prelude
		   draws whatever the sprite sheet holds. */
		if(frame == 1) {
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

			orbit = frame * (TRIG_TURN / 32);
			x = FIXEDtoINT(-sySin(orbit) * 4) + screenX / 2 - 10;
			y = FIXEDtoINT(syCos(orbit) * 2) + screenY - 22;
			/* DEAD: frame never passes 69, the loop ending at frame 63; the ROM
			   keeps the compare and the rise. */
			if(frame > 69)
				y = (y - frame * 2) + 138;
			draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
				SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			if(frame <= 3)
				draw[1](canvas, state->sheet + HAIL_DJINNI_PICTURE, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
		}

		/* The chips fly up, odd ones mirrored. */
		for(i = 0; i != HAIL_CHIPS; i++) {
			chip = &PARTICLES[i];
			if(frame >= i / 4 + 4) {
				Sint32 picture;
				Sint32 width;
				Sint32 height;

				picture = (chip->z / 128) & 3;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = Hail_PictureWidths[picture];
				height = Hail_PictureHeights[picture];
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + Hail_Pictures[picture],
					FIXEDtoINT(chip->x) - width / 2,
					FIXEDtoINT(chip->y) - height / 2, width, height);
#else
				draw[i & 1](canvas,
					state->sheet + HAIL_CHIP_PICTURES + Hail_Pictures[picture],
					FIXEDtoINT(chip->x) -
						(width = Hail_PictureWidths[picture]) / 2,
					FIXEDtoINT(chip->y) -
						(height = Hail_PictureHeights[picture]) / 2,
					width, height);
#endif
				btParticleAdvanceWithGravity3D(chip, 63, toFIXED(1) / 16);
			}
		}

		if(frame == 8) {
			state->shakeFrm = frame;
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			btEffectTintBattler(
				state->args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			btMotionApplyKnockbackStub(state->args->tgts[0], KNOCKBACK_LAUNCH);
		}

		/* Waves rise along the bottom, drifting away from the user. */
		rise = frame * 4;
		if(rise > WATER_WAVE_HEIGHT)
			rise = WATER_WAVE_HEIGHT;
		if(state->args->side == EFFECT_SIDE_ENEMIES) {
			for(i = 0; i != 5; i++)
				draw[0](canvas, state->sheet, (i << 5) - (frame / 4 & 31),
					120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		} else {
			for(i = 0; i != 5; i++)
				draw[0](canvas, state->sheet,
					((i << 5) + (frame / 4 & 31)) - WATER_WAVE_WIDTH,
					120 - rise, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		}

		btCameraShake(4, 8);
		EFFECT_END_FRAME(state);
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
	Sint32				 frame;
	BLITTER_PAIR		 draw;
	BATTLE_CAMERA		*camera;
	ACTOR				*target;
	ACTOR				*user;
	SCREEN_POINT		 position;
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
	target = btObjectGetStub(state->args->tgts[0])->actor;
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
	for(frame = 0; frame != GROUND_FRAMES; frame++) {
		if(frame > 16)
			btEffectStepPaletteToResource(RESOURCE_PORTAL_SHEET);
		/* The Djinni circles above the user, then rises away. */
		if(state->args->prelude == 1) {
			Sint32 angle = frame * (TRIG_TURN / 32);
			Sint32 x = FIXEDtoINT(-sySin(angle) * 4) + screenX / 2 - 10;
			Sint32 y = FIXEDtoINT(syCos(angle) * 2) + screenY - 22;

			if(frame > 16)
				y = y - frame * 2 + 32;
			if(state->args->side == EFFECT_SIDE_PARTY)
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
			if(frame <= 3)
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
		if((frame & 1) == 0) {
			for(i = 0; i != GROUND_CRACKLES; i++) {
				PARTICLE *crackle = &state->particles[GROUND_FIRST_CRACKLE + i];

				crackle->velX = syRandom() % 6 + 3;
				crackle->velY = Ground_CrackleFlips[syRandom() & 3];
			}
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		for(i = 0; i != GROUND_PORTALS_DRAWN; i++) {
			PARTICLE *portal = &state->particles[i];

			if(portal->variant == 1) {
				if(frame > i * 2) {
					Sint32 x;
					Sint32 y;

					btProjectionPoint(&portal->x, &position);
					position.x >>= 1;
					x = position.x - PORTAL_WIDTH / 2;
					y = position.y - PORTAL_HEIGHT / 2;
					draw[0](canvas, state->sheet, x, y, PORTAL_WIDTH,
						PORTAL_HEIGHT);
					if((frame & 3) <= 1)
						draw[0](canvas, state->sheet + Ground_PortalPictures[1],
							x + Ground_PortalX[1], y + Ground_PortalY[1],
							Ground_PortalWidths[1], Ground_PortalHeights[1]);
					else
						draw[0](canvas, state->sheet + Ground_PortalPictures[2],
							x + Ground_PortalX[2], y + Ground_PortalY[2],
							Ground_PortalWidths[2], Ground_PortalHeights[2]);
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
							crackleX = x - Ground_PortalWidths[crackle->velX] -
								Ground_PortalX[crackle->velX] + 24;
						else
							crackleX = x + Ground_PortalX[crackle->velX];
						if(crackle->velY & 8)
							crackleY = y - Ground_PortalHeights[crackle->velX] -
								Ground_PortalY[crackle->velX] + 48;
						else
							crackleY = y + Ground_PortalY[crackle->velX];
						draw[1](canvas,
							state->sheet + Ground_PortalPictures[crackle->velX],
							crackleX, crackleY,
							Ground_PortalWidths[crackle->velX],
							Ground_PortalHeights[crackle->velX]);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					}
					portal->x += portal->velX;
					portal->y += portal->velY;
					portal->z += portal->velZ;
				}
				/* The portal homes in on the target and strikes it. */
				if(frame > i * 2 + 16) {
					portal->velX += (target->x.fixed - portal->x) >> 8;
					portal->velY += (toFIXED(20) - portal->y) >> 8;
					portal->velZ += (target->z.fixed - portal->z) >> 8;
					if(frame < i * 2 + 85) {
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
		EFFECT_END_FRAME(state);
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
		Sint32 offsetY = ((rangeY - 1) & syRandom()) - halfRange;
		Sint32 offsetX = randomX - halfRange;

		BgScrollWork[1].x = offsetX;
		BgScrollWork[1].y = offsetY + BATTLE_BACKGROUND_SCROLL_Y;
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
		state->shakeFrm--;
		return;
	}
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = state->savedBg1Y;
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
}

#undef HAIL_CHIPS
#undef GROUND_FRAMES
#undef HAIL_FRAMES
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
