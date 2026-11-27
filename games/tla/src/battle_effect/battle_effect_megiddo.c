/* Megiddo, the Sol Blade's unleash. Part of battle_effect.c. */

#define MEGIDDO_FRAMES 188

/* Where the textures are decoded: FlameGradientTexture at the start of the
   effect's pictures, then GlowBandTextureA, FireStripesTexture's five 64x16
   pictures of the corona's flames and DustBlobSheet's 32x32 cloud;
   FireRingTexture in the map cell buffer. */
#define MEGIDDO_GLOW_BAND			  2048
#define MEGIDDO_FIRE_STRIPES		  4096
#define MEGIDDO_FIRE_STRIPES_WIDTH	  64
#define MEGIDDO_FIRE_STRIPES_HEIGHT	  16
#define MEGIDDO_FIRE_STRIPES_PICTURES 5
#define MEGIDDO_DUST_BLOB			  9216
#define MEGIDDO_DUST_BLOB_SIZE		  32
#define MEGIDDO_FIRE_RING			  16384

/* The turned corners of the corona and of the flames, and the corona's strip,
   in the map cell buffer; the flames' corners and their strip in the effect's
   pictures. */
#define MEGIDDO_TRANSFORMED	  (Ram_MapMetatileBuffer + 4096)
#define MEGIDDO_CORONA_MESH	  (Ram_MapMetatileBuffer + 8192)
#define MEGIDDO_FLAME_CORNERS 24384
#define MEGIDDO_FLAME_MESH	  25408

/* The sun's corona: a ring of MEGIDDO_CORONA_QUADS quads of the fire stripes
   round the sun, from MEGIDDO_CORONA_INNER_RADIUS out to a flickering
   MEGIDDO_CORONA_OUTER_RADIUS, scaled by MEGIDDO_CORONA_SCALE with the sun. */
#define MEGIDDO_CORONA_QUADS		32
#define MEGIDDO_CORONA_VERTICES		((MEGIDDO_CORONA_QUADS + 1) * 2)
#define MEGIDDO_CORONA_INNER_RADIUS 80
#define MEGIDDO_CORONA_OUTER_RADIUS 120
#define MEGIDDO_CORONA_SCALE		toFIXED(0.71009)

/* The flames that wrap the falling sun: MEGIDDO_FLAMES bands of the cloud
   turned 3/32 of a turn apart about it, each a quarter circle of
   MEGIDDO_FLAME_ROWS rows from its point to its rim, MEGIDDO_FLAME_RADIUS round
   and an eighth of a turn wide. Each band's cloud is scrolled
   MEGIDDO_FLAME_SCROLL_STEP texels on from the one before. */
#define MEGIDDO_FLAMES			  10
#define MEGIDDO_FLAME_ROWS		  16
#define MEGIDDO_FLAME_VERTICES	  (MEGIDDO_FLAME_ROWS * 2)
#define MEGIDDO_FLAME_RADIUS	  48
#define MEGIDDO_FLAME_SCROLL_STEP 20
#define MEGIDDO_FLAME_SCROLL_MASK 0x7f

/* As the sun falls, its flames and the sparks about it start turned by
   MEGIDDO_FALL_TILT about the screen's horizontal and turn back over
   MEGIDDO_FALL_TILT_FRAMES. */
#define MEGIDDO_FALL_TILT		 (TRIG_TURN * 3 / 16)
#define MEGIDDO_FALL_TILT_FRAMES 32

/* How much the falling sun's scale grows a frame, from a quarter to three
   quarters. */
#define MEGIDDO_SUN_GROWTH toFIXED(0.01886)

/* How many lines a frame the red haze scrolls at its fastest; it slows to a
   stop at MEGIDDO_HAZE_REST_FRAME. */
#define MEGIDDO_HAZE_SPEED		   16
#define MEGIDDO_HAZE_REST_FRAME	   72
/* The red haze's colours: raised as it loads, its red by MEGIDDO_HAZE_FADE_RED
   and its green and blue by MEGIDDO_HAZE_FADE; lowered again as the sun falls,
   the red by MEGIDDO_HAZE_FALL_FADE_RED, below where it loaded. */
#define MEGIDDO_HAZE_FADE_RED	   8
#define MEGIDDO_HAZE_FADE		   2
#define MEGIDDO_HAZE_FALL_FADE_RED 12

/* Once the sun strikes, the canvas is filled at this level after each copy. */
#define MEGIDDO_STRIKE_FILL_LEVEL 16

/* The rings of light, FireRingTexture flashing out from the caster as the
   effect starts and from the target as the sun strikes (Megiddo_Flash*). Each
   shines for MEGIDDO_FLASH_SHINE_FRAMES, then fades 8 levels a frame. */
#define MEGIDDO_FLASHES			   5
#define MEGIDDO_FLASH_SHINE_FRAMES 8

/* The rays that fan out about the caster (Megiddo_Ray*), the flame of
   FlameGradientTexture stretched long: one every two frames from
   MEGIDDO_RAYS_START, each for MEGIDDO_RAY_FRAMES, shrinking away over the last
   MEGIDDO_RAY_SHRINK_FRAMES. */
#define MEGIDDO_RAYS			  8
#define MEGIDDO_RAYS_START		  16
#define MEGIDDO_RAY_FRAMES		  64
#define MEGIDDO_RAY_SHRINK_FRAMES 16

/* Megiddo: the caster leaps and hangs in the air before a red haze, rings of
   light flashing round them, rays fanning out about them and sparks rising; a
   burning sun sweeps down across the sky, then falls onto the target wrapped in
   flames and bursts in rings of light.

   Abilities: Megiddo. */
void btEffectRunMegiddo(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	Uint8				*canvas;
	Sint32				 frame;
	Sint32				 doubled;
	Sint32				 k;
	/* FAKEMATCH(temporary): 0 held in a local for the stores below; as 0. */
	Sint32					 zero;
	MODEL_VERTEX			*coronaCorners;
	DRAW_RECTANGLE			 blit[2];
	Uint8					*sprites;
	BATTLE_BACKGROUND_STATE *background;
	BATTLE_CAMERA			*camera;
	Sint16					 savedYaw;
	MODEL_VERTEX			*flameCorners;
	Uint8					*flameMesh;
	Sint32					 casterVerticalMotion;
	MOTION_OBJECT			*caster;
	FIXED					 x;
	FIXED					 z;
	FIXED					 scale;
	Uint8					*transformed;
	Uint8					*coronaMesh;
	Uint8					*fireRing;
	Sint32					 i;
	Sint32					 j;
	Sint32					 m;
	RENDER_MODEL			*model;
	void					*verts;
	PARTICLE				*p;
	SCREEN_POINT			 pos;
	Sint16					 list[UNIT_MAX];
	Sint16					 targetList[UNIT_MAX];
	FIXED					 place[4];
	Sint32					 screen[3];
	MODEL_TEXTURE			 scratchTexture;
	Sint32					 twice;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	background = Ram_HeapSlots->battleBg;
	camera = Ram_HeapSlots->cameraState;
	savedYaw = 0;
	zero = 0;
	coronaCorners = (MODEL_VERTEX *)MapGridMetatileBuffer;
	transformed = MEGIDDO_TRANSFORMED;
	coronaMesh = MEGIDDO_CORONA_MESH;
	fireRing = Ram_MapMetatileBuffer + MEGIDDO_FIRE_RING;
	/* The caster leaps on the spot. */
	btMotionApproachTargetStub(args->user, args->user, 150, toFIXED(5));
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = Ram_HeapSlots->ramCode;
	flameCorners = (MODEL_VERTEX *)(state->sheet + MEGIDDO_FLAME_CORNERS);
	flameMesh = state->sheet + MEGIDDO_FLAME_MESH;
	caster = btObjectGetStub(args->user)->obj;
	x = 0;
	z = 0;
	scale = toFIXED(1);
	btRenderBuildQuadStrip(
		coronaMesh, 8, MEGIDDO_FIRE_STRIPES_HEIGHT, MEGIDDO_CORONA_QUADS);
	btRenderBuildQuadStrip(
		flameMesh, 2, MEGIDDO_DUST_BLOB_SIZE, MEGIDDO_FLAME_ROWS - 1);
	for(j = 0; j != MEGIDDO_FLAME_ROWS; j++) {
		Sint32 angle = j * (DEGtoANG(90) / (MEGIDDO_FLAME_ROWS - 1));
		Sint32 y = FIXEDtoINT(-syCos(angle) * MEGIDDO_FLAME_RADIUS);
		Sint32 r = FIXEDtoINT(sySin(angle) * MEGIDDO_FLAME_RADIUS);

		for(m = 0; m != 2; m++) {
			MODEL_VERTEX *v = &flameCorners[j * 2 + m];

			SET_POSITION(v, FIXEDtoINT(sySin(m * (TRIG_TURN / 8)) * r),
				y + MEGIDDO_FLAME_RADIUS / 2,
				FIXEDtoINT(syCos(m * (TRIG_TURN / 8)) * r));
		}
	}
	/* The caster hangs in the air, still. */
	casterVerticalMotion = caster->verticalMotionStrength;
	fdActorSetAnimationSpeedStub(caster, 0);
	caster->velX = 0;
	caster->velY = 0;
	caster->velZ = 0;
	caster->acceleration = 0;
	caster->verticalMotionStrength = 0;

	for(frame = 0; frame != MEGIDDO_FRAMES; frame++) {
		if(frame == 4) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frame == 28) {
			sySndPlayStub(SE_SPIN);
		}
		if(frame == 84) {
			sySndPlayStub(SE_LAUNCH);
		}
		if(frame == 143) {
			sySndPlayStub(SE_TABLET_RISE_HIGH);
		}
		if(frame == 171) {
			sySndPlayStub(SE_CRASH);
		}
		/* A or B skips to the sun's fall. */
		if((PerWork.repeat & (KEY_A | KEY_B)) && frame > 3 && frame <= 142) {
			frame = 143;
		}
		if(frame == 0) {
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
				Ram_MapMetatileBuffer + MEGIDDO_FIRE_RING, 0, 0);
			btResourceLoadAndDecompress(
				RESOURCE_FLAME_GRADIENT_TEXTURE, state->sheet, 0, 0);
			/* DEAD: GlowBandTextureA, which nothing draws; the ROM keeps its
			   decode. */
			btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
				state->sheet + MEGIDDO_GLOW_BAND, 0, 0);
			btResourceLoadAndDecompress(RESOURCE_FIRE_STRIPES_TEXTURE,
				state->sheet + MEGIDDO_FIRE_STRIPES, 1, 1);
			btResourceLoadAndDecompress(RESOURCE_DUST_BLOB_SHEET,
				state->sheet + MEGIDDO_DUST_BLOB, 1, 0);
			btGraphicsLoadGreyRamp();
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			if(args->side == EFFECT_SIDE_ENEMIES) {
				x = toFIXED(160);
			} else {
				x = toFIXED(80);
			}
			z = -toFIXED(96);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2CNT,
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7),
				IO_WRITE_HALFWORD);
		}
		/* The caster alone before the red haze, and the sun's sprite. */
		if(frame == 1) {
			background->busy = 1;
			list[0] = args->user;
			list[1] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForListStub(list, 0);
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP, 0);
			btEffectBuildRepeatingTilemap();
			btPaletteBrightenBattleBg(
				MEGIDDO_HAZE_FADE_RED, MEGIDDO_HAZE_FADE, MEGIDDO_HAZE_FADE);
			btEffectCreateSprs(1, SPRITE_SUN, 2);
		}
		btEffectPosProjectScaledBattler(args->user, &pos);
		if(frame == 3) {
			savedYaw = camera->yaw;
		}
		if((Uint32)(frame - 16) <= 61) {
			if(args->side == EFFECT_SIDE_ENEMIES) {
				camera->yaw -= TRIG_TURN / 256;
			} else {
				camera->yaw += TRIG_TURN / 256;
			}
		}

		/* The corona's rim flickers a row at a time. */
		for(k = 0; k != MEGIDDO_CORONA_QUADS + 1; k++) {
			for(i = 0; i != 2; i++) {
				MODEL_VERTEX *v = &coronaCorners[k * 2 + i];
				Sint32		  r = MEGIDDO_CORONA_INNER_RADIUS;

				if(i != 1) {
					r = (FIXEDtoINT(
							sySin((frame + k * 4) * (TRIG_TURN / 16)) * 2)) +
						MEGIDDO_CORONA_OUTER_RADIUS;
				}
				SET_POSITION(v,
					FIXEDtoINT(
						sySin(k * (TRIG_TURN / MEGIDDO_CORONA_QUADS)) * r),
					FIXEDtoINT(
						syCos(k * (TRIG_TURN / MEGIDDO_CORONA_QUADS)) * r),
					0);
			}
		}

		/* The red haze scrolls fast, comes to rest at MEGIDDO_HAZE_REST_FRAME,
		   and scrolls the other way, the view with it, as the sun comes
		   down. */
		{
			Sint32 y = BgScrollWork[1].y;
			Sint32 d;

			/* A scroll above the haze's range is a negative one, read back from
			   its halfword. */
			if(y > BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP) {
				y -= BG_SCROLL_WRAP;
			}
			if(frame <= 63) {
				d = -MEGIDDO_HAZE_SPEED;
			} else if((Uint32)(frame - 64) <= 7) {
				twice = frame * 2;
				d = twice - 2 * MEGIDDO_HAZE_REST_FRAME;
			} else if((Uint32)(frame - MEGIDDO_HAZE_REST_FRAME) <= 11) {
				d = 0;
			} else if(frame > 154) {
				d = MEGIDDO_HAZE_SPEED - (frame - 155) / 2;
				if(d < 0) {
					d = 0;
				}
			} else {
				d = frame - 84;
				if(d > MEGIDDO_HAZE_SPEED) {
					d = MEGIDDO_HAZE_SPEED;
				}
				ProjectionWork.centerY -= d;
			}
			y += d;
			if(y < -BATTLE_VIEW_TOP) {
				y += BATTLE_BACKGROUND_HEIGHT;
			}
			if(y > BATTLE_BACKGROUND_HEIGHT - BATTLE_VIEW_TOP) {
				y -= BATTLE_BACKGROUND_HEIGHT;
			}
			BgScrollWork[1].y = y;
		}
		if((Uint32)(frame - 42) <= 37) {
			z += toFIXED(2);
			btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
		}
		/* The sun sweeps down across the sky. */
		if((Uint32)(frame - 84) <= 58) {
			if(args->side == EFFECT_SIDE_ENEMIES) {
				x -= toFIXED(2);
			} else {
				x += toFIXED(2);
			}
			z += toFIXED(4);
		}
		/* The fall: the camera comes back, and the sun falls on the target,
		   alone at the middle of the field. */
		if(frame == 143) {
			MOTION_OBJECT *target;

			btPaletteBrightenBattleBg(-MEGIDDO_HAZE_FALL_FADE_RED,
				-MEGIDDO_HAZE_FADE, -MEGIDDO_HAZE_FADE);
			camera->yaw = savedYaw;
			x = toFIXED(120);
			z = toFIXED(48);
			targetList[0] = args->tgts[0];
			targetList[1] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForListStub(targetList, 0);
			ProjectionWork.centerY = 556;
			target = btObjectGetStub(args->tgts[0])->obj;
			target->x = zero;
			target->z = zero;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		}
		if(frame > 154) {
			Sint32 d = MEGIDDO_HAZE_SPEED - (frame - 155) / 2;

			if(d < 0) {
				d = 0;
			}
			ProjectionWork.centerY -= d;
		}
		if(frame == 84) {
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2CNT,
				BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					BGCNT_SCREEN_BASE(7),
				IO_WRITE_HALFWORD);
			fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 8);
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
		}
		/* The sun strikes. */
		if(frame == 171) {
			state->shakeFrm = 8;
			sySndPlayStub(SE_CRASH);
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
				CANVAS_FILL_WORD(MEGIDDO_STRIKE_FILL_LEVEL));
		}
		if(frame > 174) {
			btPaletteBrightenBgEntries(2, 2, 2);
		}

		/* The sun's sprite, at twice its scale, while it is on screen. */
		if(frame > 1) {
			SPR	 *obj;
			FIXED req[2];

			if(frame > 142) {
				Sint32 n = frame - 143;

				scale = n * MEGIDDO_SUN_GROWTH + toFIXED(1) / 4;
				if(scale > toFIXED(3) / 4) {
					scale = toFIXED(3) / 4;
				}
			}
			req[1] = scale * 2;
			req[0] = scale * 2;
			place[3] = zero;
			place[1] = toFIXED(255);
			place[0] = x;
			place[2] = z;
			obj = state->sprs[0];
			obj->ofsY = 32;
			obj->ofsX = 0;
			if((Uint32)(place[2] + toFIXED(52)) < toFIXED(252)) {
				fdRenderApplyProjectedPlacementStub(obj, place, req, 0);
			}
		}
		/* FAKEMATCH(dead): a product nothing reads, between the two blocks that
		 * test frame > 1. */
		doubled = frame * 2;
		if(frame > 1) {
			MODEL_TEXTURE texture;
			/* FAKEMATCH(temporary): t, a pointer standing in for the texture's
			 * own address; as texture. */
			MODEL_TEXTURE *t;

			model = btRenderAllocateModelList(1);
			t = &texture;
			/* The corona: a 64x16 picture of the fire stripes, its sizes as
			   powers of two. */
			model->kind = MODEL_SHADED_MAX;
			/* FAKEMATCH(barrier): an empty asm that rewrites model and reads a
			 * zero. */
			asm("" : "+r"(model) : "r"(0));
			t->logWidth = 6;
			model->vertices = transformed;
			t->logHeight = 4;
			model->texture = t;
			model->shade = 0;
			model->scrollV = 0;
			/* FAKEMATCH(dead): the pixels' store alone in a once-only block; as
			 * the plain store. */
			do {
				t->pixels = state->sheet + MEGIDDO_FIRE_STRIPES +
					((frame / 2) % MEGIDDO_FIRE_STRIPES_PICTURES) *
						PICTURE_BYTES(MEGIDDO_FIRE_STRIPES);
			} while(0);
			model->mesh = coronaMesh;
			{
				FIXED widthScale = toFIXED(1) / 2;

				syInitMatrix();
				syScale(widthScale, toFIXED(1), widthScale);
			}
			syTranslate(x - toFIXED(126), z - toFIXED(76), 0);
			syMatrixScaleUniform(MEGIDDO_CORONA_SCALE);
			syMatrixScaleUniform(scale);
			btRenderTransformVertices(
				coronaCorners, transformed, MEGIDDO_CORONA_VERTICES);
			btRenderDrawModelList(model);

			/* The flames that wrap the sun as it falls. */
			/* FAKEMATCH(dead): updates of t that cancel out. */
			t += frame;
			t -= frame;
			t->logWidth = 5;
			t->logHeight = 5;
			t->pixels = state->sheet + MEGIDDO_DUST_BLOB;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->mesh = flameMesh;
			{
				Sint32 shade = -24;

				model->flg = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
				model->shade = shade;
			}
			model->scrollV = zero;
			if(frame > 142) {
				Sint32 n = frame - 143;
				Sint32 scroll;
				Sint32 angle;
				Sint32 tilt;

				if(n > MEGIDDO_FALL_TILT_FRAMES) {
					n = MEGIDDO_FALL_TILT_FRAMES;
				}
				i = 0;
				/* FAKEMATCH(temporary): twice, frame * 2 kept apart. */
				twice = frame * 2;
				angle = -DEGtoANG(90);
				tilt = n * (MEGIDDO_FALL_TILT / MEGIDDO_FALL_TILT_FRAMES);
				scroll = twice + frame;
				for(; i != MEGIDDO_FLAMES; i++) {
					model->scrollU = ~scroll & MEGIDDO_FLAME_SCROLL_MASK;
					syInitMatrix();
					syTranslate(-toFIXED(2), -toFIXED(32), 0);
					syScale(toFIXED(1.25123), toFIXED(3.32044), toFIXED(1));
					syMatrixScaleUniform(scale);
					syRotZ(DEGtoANG(180));
					scroll += MEGIDDO_FLAME_SCROLL_STEP;
					syRotX(tilt - MEGIDDO_FALL_TILT);
					syRotY(angle);
					btRenderTransformVertices(
						flameCorners, transformed, MEGIDDO_FLAME_VERTICES);
					btRenderDrawModelList(model);
					angle += TRIG_TURN * 3 / 32;
				}
			}
			syHeapRelease(model);
		}

		/* Sparks rise over the canvas while the rays shoot. */
		if(frame == 8) {
			syWordsFillEntry(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			p = state->particles;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++, p++) {
				p->x = syRandom() & 127;
				p->y = syRandom() & 127;
			}
		}
		if((Uint32)(frame - 8) <= 79) {
			Sint32 off = (frame * 2 - 16) & 127;

			p = state->particles;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++, p++) {
				if(p->variant >= 0) {
					Sint32 size = 1;
					Sint32 mx = p->x;
					Sint32 y = p->y - off;

					if(y > 127) {
						y -= 128;
					}
					if(y < 0) {
						y += 128;
					}
					blit[0](canvas,
						sprites + BattleEffect_MotePictures[size - 1], mx, y,
						size, 2);
				}
			}
		}
		/* Sparks rise about the falling sun. */
		if(frame > 142) {
			Sint32 n = frame - 143;

			if(n > MEGIDDO_FALL_TILT_FRAMES) {
				n = MEGIDDO_FALL_TILT_FRAMES;
			}
			if(frame == 143) {
				p = state->particles;
				for(i = 0; i != ARRAY_COUNT(state->particles); i++, p++) {
					SET_POSITION(p, syRandom() & 127, syRandom() % 192 - 64,
						syRandom() & 127);
				}
			}
			syInitMatrix();
			syRotX(n * (MEGIDDO_FALL_TILT / MEGIDDO_FALL_TILT_FRAMES) -
				MEGIDDO_FALL_TILT);
			p = state->particles;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++, p++) {
				Sint32 size = 1;

				syConvert3Dto2D((const FIXED *)p, screen);
				blit[0](canvas, sprites + BattleEffect_MotePictures[size - 1],
					screen[0], screen[1], size, 4);
				p->y -= 2;
			}
		}

		/* The rings of light. */
		verts = syHeapAllocEWRAM(32);
		model = btRenderAllocateModelList(1);
		scratchTexture.logWidth = 7;
		scratchTexture.logHeight = 7;
		scratchTexture.pixels = fireRing;
		model->kind = MODEL_SHADED_ADDITIVE;
		model->mesh = BattleEffect_LargeSquareMesh;
		model->texture = &scratchTexture;
		model->vertices = verts;
		for(i = 0; i != MEGIDDO_FLASHES; i++) {
			Sint32 start = Megiddo_FlashStarts[i];

			if(frame >= start && frame < start + 20) {
				Sint32 t = frame - start;
				Sint32 shade = MEGIDDO_FLASH_SHINE_FRAMES * 8 - t * 8;
				Sint32 s;

				if(shade > 0) {
					shade = 0;
				}
				if(shade > MODEL_SHADE_INVISIBLE) {
					s = t * (toFIXED(1) / 4) + toFIXED(1) / 4;
					model->shade = shade;
					syInitMatrix();
					if(i <= 1) {
						syTranslate(INTtoFIXED(pos.x / 2 - 64),
							INTtoFIXED(pos.y - 48), 0);
					} else {
						syTranslate(0, toFIXED(16), 0);
					}
					syScale(s, s * 2, s);
					syRotX(Megiddo_FlashPitches[i]);
					syRotY(Megiddo_FlashYaws[i]);
					btRenderTransformVertices(
						BattleEffect_SquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
		}
		/* The rays. */
		model->kind = MODEL_SHADED_ADDITIVE;
		scratchTexture.logWidth = 5;
		scratchTexture.logHeight = 6;
		scratchTexture.pixels = state->sheet;
		model->mesh = BattleEffect_TallMesh;
		for(i = 0; i != MEGIDDO_RAYS; i++) {
			Sint32 start = MEGIDDO_RAYS_START + i * 2;

			if(frame >= start &&
				frame < i * 2 + MEGIDDO_RAYS_START + MEGIDDO_RAY_FRAMES) {
				Sint32 t = frame - start;
				Sint32 shade = MODEL_SHADE_INVISIBLE + t * 8;
				Sint32 s = t * (toFIXED(1) / 32);

				if(shade > 0) {
					shade = 0;
				}
				if(s > toFIXED(1)) {
					s = toFIXED(1);
				}
				if(t >= MEGIDDO_RAY_FRAMES - MEGIDDO_RAY_SHRINK_FRAMES) {
					s = s - t * (toFIXED(1) / MEGIDDO_RAY_SHRINK_FRAMES) +
						toFIXED(3);
				}
				model->shade = shade;
				syInitMatrix();
				if(args->side == EFFECT_SIDE_ENEMIES) {
					syTranslate(INTtoFIXED(pos.x / 2 + Megiddo_RayX[i] - 64),
						INTtoFIXED(Megiddo_RayY[i] + pos.y - 54), 0);
				} else {
					syTranslate(INTtoFIXED(pos.x / 2 - Megiddo_RayX[i] - 62),
						INTtoFIXED(Megiddo_RayY[i] + pos.y - 54), 0);
				}
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				if(args->side == EFFECT_SIDE_ENEMIES) {
					syRotZ(Megiddo_RayRolls[i]);
				} else {
					syRotZ(-Megiddo_RayRolls[i]);
				}
				syRotX(Megiddo_RayPitches[i]);
				syRotY(Megiddo_RayYaws[i]);
				syScale(s / 4, s * 4, s / 2);
				btRenderTransformVertices(
					BattleEffect_StandingSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);

		/* Waits until a VBlank has passed since the canvas was last copied. */
		do {
			(void)state->vblanksSinceTransfer;
		} while(state->vblanksSinceTransfer <= 1);
		EFFECT_END_FRAME(state);
	}

	fdSprDestroyStub(state->sprs[0]);
	caster->verticalMotionStrength = casterVerticalMotion;
	btObjectPutStub();
	background->busy = 0;
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
		Ram_HeapSlots->battleState->bg, BACKGROUND_DARKEN_FRAMES);
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
