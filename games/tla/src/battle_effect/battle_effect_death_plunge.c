/* Death Plunge. Part of battle_effect.c. */

#define DEATH_PLUNGE_TICKS		  96
/* The plunge strikes: the target flashes, the user flashes and the damage
   shows. */
#define DEATH_PLUNGE_STRIKE_TICK  54
/* Both land the right way up, and Death Leap's rings start to fade. */
#define DEATH_PLUNGE_LAND_TICK	  64
/* How far toward the target the user runs, in hundredths of the way. */
#define DEATH_PLUNGE_RUN_PERCENT  90
/* The flashes thrown in a ring after the strike. */
#define DEATH_PLUNGE_RING_FLASHES 16

/* Death Leap's bursts, given a countdown that sets out four a tick, of which
   the first DEATH_LEAP_BURSTS fly from the target; each shows FlashBurstSheet's
   six pictures for four ticks apiece as it rises. */
#define DEATH_LEAP_BURST_SLOTS		 32
#define DEATH_LEAP_BURSTS			 16
#define DEATH_LEAP_BURST_TICKS		 (6 * 4)
/* Death Leap's two rings of light, FireRingTexture a quarter turn apart, swell
   from a quarter of their size by an eighth a tick (toFIXED(1) >> (16
   - DEATH_LEAP_RING_GROWTH_SHIFT)) and fade DEATH_LEAP_RING_FADE a tick from
	 DEATH_PLUNGE_LAND_TICK. */
#define DEATH_LEAP_RINGS			 2
#define DEATH_LEAP_RING_GROWTH_SHIFT 13
#define DEATH_LEAP_RING_FADE		 4

/* The user runs at the first target, both leap and turn upside down, and on the
   hit they bounce apart while a ball of fire swells over them. Death Leap draws
   in violet: bursts fly up from the target and two rings of light swell over
   it.

   Abilities: Death Plunge, Death Leap. */
void btEffectRunDeathPlunge(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	BATTLE_OBJECT *userObject;
	BATTLE_OBJECT *tgtObject;
	ACTOR		  *user;
	ACTOR		  *tgt;
	Sint32		   percent;
	FIXED		   x;
	FIXED		   z;
	Sint32		   userHeight;
	Sint32		   tgtHeight;
	SCREEN_POINT   tgtScreen;
	FIXED		   point[3];
	SCREEN_POINT   screen;
	MODEL_TEXTURE  texture;
	PARTICLE	  *burst;
	RENDER_MODEL  *model;
	void		  *vertices;
	Sint32		   i;
	Uint8		  *pixels;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;

	/* Set up the canvas, the flash sprite sheet, the rings' texture and a
	   clipped blitter keeping the brighter; Death Leap takes violet colours. */
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
	pixels = RamMapMetatileBuffer;
	btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
	if(args->variant == EFFECT_VARIANT_STRONGER) {
		syWordsCopyEntry((void *)BG_PLTT,
			syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_F),
			SHEET_PALETTE_BYTES);
		state->transferValue = CANVAS_FADE_HALF;
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		REG_BLDALPHA = EFFECT_BLEND_ADD;
	} else {
		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	}
	state->transferMode = CANVAS_COPY_AND_FADE;
	draw[0] = RamHeapSlots->ramCode;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	/* The run, most of the way to the target. */
	userObject = btObjectGetStub(args->user);
	tgtObject = btObjectGetStub(args->tgts[0]);
	user = userObject->obj;
	tgt = tgtObject->obj;
	percent = DEATH_PLUNGE_RUN_PERCENT;
	x = user->x.fixed + percent * (tgt->x.fixed - user->x.fixed) / 100;
	z = user->z.fixed + percent * (tgt->z.fixed - user->z.fixed) / 100;
	userHeight = btUnitGetEffectHeightStub(args->user);
	tgtHeight = btUnitGetEffectHeightStub(args->tgts[0]);
	btEffectPosProjectBattler(args->tgts[0], &tgtScreen);
	fdActorStartIdleStub(user);
	fdActorSetMoveTargetStub(user, x, 0, z);
	fdActorSelectAnimationStub(user, POSE_LEAP);
	user->snapToTgt = 1;
	user->steeringFlg = ACTOR_FACES_MOTION;
	user->acceleration = toFIXED(2);
	user->speedLimit = toFIXED(8);
	sySynch(20);

	for(frm = 0; frm != DEATH_PLUNGE_TICKS; frm++) {
		BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

		/* Both leap at 15 pixels a tick, and soon turn upside down. */
		if(frm == 0) {
			tgt->velY = toFIXED(15);
			tgt->gravity = toFIXED(57) / 100;
			user->velY = toFIXED(15);
			user->gravity = toFIXED(57) / 100;
		}
		if(frm == 11) {
			tgt->scaleY = -tgt->scaleY;
			user->scaleY = -user->scaleY;
			user->y.fixed += userHeight;
			tgt->y.fixed += tgtHeight;
		}

		/* The target flashes and both bounce; the user heads back towards the
		   middle of the field. */
		if(frm == DEATH_PLUNGE_STRIKE_TICK) {
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 10);
			tgt->velY = toFIXED(8);
			tgt->gravity = BATTLE_UNIT_GRAVITY;
			user->velY = toFIXED(5);
			user->gravity = toFIXED(47) / 100;
			user->acceleration = toFIXED(1);
			user->speedLimit = toFIXED(2);
			user->steeringFlg = 0;
			fdActorStartIdleStub(user);
			fdActorSetMoveTargetStub(user, 0, 0, user->z.fixed);
		}

		if(args->variant == EFFECT_VARIANT_BASIC) {
			/* One big flash on the user, then a ring of flashes. */
			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, user->x.fixed, user->y.fixed, user->z.fixed);
			btEffectPosProjectPoint(point, &screen);
			screen.x >>= 1;
			if(frm == DEATH_PLUNGE_STRIKE_TICK ||
				frm == DEATH_PLUNGE_STRIKE_TICK + 1)
				draw[0](canvas, state->sheet, screen.x - FLASH_BURST_WIDTH / 2,
					screen.y - 16, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
			if(frm >= 56 && frm <= 67) {
				for(i = 0; i != DEATH_PLUNGE_RING_FLASHES; i++) {
					Sint32 ang = i * (TRIG_TURN / DEATH_PLUNGE_RING_FLASHES);
					Sint32 rx =
						screen.x + (FIXEDtoINT(sySin(ang) * (frm - 46) * 2));
					Sint32 ry =
						(FIXEDtoINT(syCos(ang) * (frm - 46) * 2)) - frm + 100;

					draw[0](canvas,
						state->sheet +
							(frm - 56) / 2 * PICTURE_BYTES(FLASH_BURST),
						rx - FLASH_BURST_WIDTH / 2, ry, FLASH_BURST_WIDTH,
						FLASH_BURST_HEIGHT);
				}
			}
		} else if(frm > 55) {
			/* The bursts fly from the target. */
			if(frm == 56) {
				burst = state->particles;
				for(i = 0; i != DEATH_LEAP_BURST_SLOTS; i++) {
					Sint32 ang;
					Sint32 force;

					burst->x = toFIXED(tgtScreen.x / 2);
					burst->y = INTtoFIXED(tgtScreen.y - 16);
					ang = syRandom();
					force = (syRandom() & 255) + 64;
					burst->velX = (sySin(ang) * force) >> 6;
					burst->velY = (syCos(ang) * force) >> 5;
					burst->variant = -i / 4;
					burst++;
				}
			}

			for(i = 0; i != DEATH_LEAP_BURSTS; i++) {
				burst = &state->particles[i];
				if(burst->variant >= 0 &&
					burst->variant < DEATH_LEAP_BURST_TICKS) {
					draw[0](canvas,
						state->sheet +
							burst->variant / 4 * PICTURE_BYTES(FLASH_BURST),
						FIXED_INTEGER_PART(burst->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(burst->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(burst, 62, -toFIXED(1) / 8);
				}
				burst->variant++;
			}
		}

		if(args->variant == EFFECT_VARIANT_STRONGER) {
			/* The rings swell from tick 56. */
			vertices = syHeapAllocEWRAM(512);
			model = btRenderAllocateModelList(1);
			if(frm > 55) {
				FIXED scale = ((frm - 56) << DEATH_LEAP_RING_GROWTH_SHIFT) +
					toFIXED(1) / 4;
				Sint32 shade = 0;

				if(frm >= DEATH_PLUNGE_LAND_TICK)
					shade =
						(DEATH_PLUNGE_LAND_TICK - frm) * DEATH_LEAP_RING_FADE;
				if(shade > MODEL_SHADE_INVISIBLE) {
					Sint32 ang;

					texture.logWidth = 7;
					texture.logHeight = 7;
					model->kind = MODEL_SHADED_ADDITIVE;
					model->texture = &texture;
					model->mesh = BattleEffectLargeSquareMesh;
					model->vertices = vertices;
					texture.pixels = pixels;
					model->shade = shade;
					for(i = 0, ang = DEGtoANG(45); i != DEATH_LEAP_RINGS;
						ang += DEGtoANG(90), i++) {
						syInitMatrix();
						syTranslate(
							INTtoFIXED(tgtScreen.x / 2 - 64), toFIXED(32), 0);
						syScale(scale, scale, scale);
						syRotZ(ang);
						syRotX(-TRIG_TURN / 16);
						btRenderTransformVertices(
							BattleEffectGroundSquareCorners, vertices, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}

		/* Both turn right side up again. */
		if(frm == DEATH_PLUNGE_LAND_TICK) {
			tgt->scaleY = -tgt->scaleY;
			user->scaleY = -user->scaleY;
			user->y.fixed -= userHeight;
			tgt->y.fixed -= tgtHeight;
			/* The user takes its first animation, a party member's stance
			   between turns. */
			fdActorSelectAnimationStub(user, POSE_STILL);
		}
		if(frm == DEATH_PLUNGE_STRIKE_TICK) {
			if(args->variant == EFFECT_VARIANT_BASIC)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			else
				EFFECT_SHOW_DAMAGE(SE_CRASH);
		}
		if(frm == 0)
			sySndPlayStub(SE_WHOOSH);
		if(frm == 0)
			state->shakeFrm = 6;
		if(frm == 53)
			state->shakeFrm = 6;
		btCameraShake(16, 16);
		EFFECT_PRESENT_CANVAS(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DEATH_PLUNGE_TICKS
#undef DEATH_PLUNGE_STRIKE_TICK
#undef DEATH_PLUNGE_LAND_TICK
#undef DEATH_PLUNGE_RUN_PERCENT
#undef DEATH_PLUNGE_RING_FLASHES
#undef DEATH_LEAP_BURST_SLOTS
#undef DEATH_LEAP_BURSTS
#undef DEATH_LEAP_BURST_TICKS
#undef DEATH_LEAP_RINGS
#undef DEATH_LEAP_RING_GROWTH_SHIFT
#undef DEATH_LEAP_RING_FADE
