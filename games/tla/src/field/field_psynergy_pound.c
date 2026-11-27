/* Pound: its shine, the object it drives into the ground, and a picture's
   columns shifted by a wave. Part of field_psynergy.c. */

/* The sparks of Pound's shine; the first 12 are drawn. */
#define POUND_SPARK_COUNT	 32
#define POUND_DRAWN_SPARKS	 12
/* The shine's colours, two palettes of the canvas: blue full, red and green
   swinging together round their middle as the cycle steps. */
#define POUND_SHINE_COLORS	 (2 * PALETTE_COLORS)
/* The pieces the fist shatters into when it cannot come down. */
#define POUND_SHATTER_PIECES 16
/* The glow under the fist, 64 pixels square in 16 colours. */
#define POUND_GLOW_SIZE		 64

/* The stages of the fist's fall (POUND_STATE.state). */
typedef enum POUND_STAGE {
	POUND_FALLS = 0, /* the fist comes down; it lands on the eighth tick */
	POUND_SHINE_GROWS = 1, /* the shine spreads, its colours cycling */
	POUND_SHINE_HOLDS = 2,
	POUND_SETTLES = 3, /* the shaking stops */
	POUND_LIFTS = 4	   /* the fist rises off the ground */
} POUND_STAGE;

/* Pound's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the star tiles, the
   shine's 32 sparks, the glow under the fist, the fist itself and the stage of
   its fall. */
typedef struct POUND_STATE {
	Uint8					tiles[4096];
	FIELD_PSYNERGY_PARTICLE particles[POUND_SPARK_COUNT];
	EFFECT_SPR				sprs[POUND_SPARK_COUNT];
	EFFECT_SPR				glow;
	ACTOR *actor; /* the fist, then what sinks into the ground */
	/* the stage of the fall; FIELD_PSYNERGY_FINISHED once it ends */
	Sint16 state;
	Sint16 tmr;		 /* ticks into the stage */
	Sint8  sparkles; /* nonzero draws the sparks */
	Sint8  landed;	 /* set on the tick the fist lands, passed to the script */
} POUND_STATE;

/* Event of a Pound that comes down: the fist falls and lands with a shake, the
   shine spreads round it with its sparks, then the fist lifts again. */
void fdPsynergyUpdatePound(void)
{
	POUND_STATE				*state = RamHeapSlots->battleEff;
	CANVAS_SPR_STATE		*canvas = RamHeapSlots->canvasSpr;
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR				*spr;
	Sint32					 i;
	Sint32					 red, green;

	state->landed = 0;
	if(state->state == POUND_SHINE_GROWS) {
		/* Each colour is a 32nd of a turn further along the cycle, which moves
		   on every other tick. */
		for(i = 0; i < POUND_SHINE_COLORS; i++) {
			Uint32 ang = (INTtoFIXED(((Uint32)canvas->drawnFrm >> 1) + i) +
							 toFIXED(9)) >>
				5;
			Sint32 blue = RGB(0, 0, RGB_LEVEL_MAX);

			red = sySin(ang) * CANVAS_SHINE_SWING;
			green = sySin((INTtoFIXED(((Uint32)canvas->drawnFrm >> 1) + i) +
							  toFIXED(9)) >>
						5) *
				CANVAS_SHINE_SWING;
			red = FIXEDtoINT(red) + CANVAS_SHINE_MIDDLE;
			green = FIXEDtoINT(green) + CANVAS_SHINE_MIDDLE;
			syIOWrite16(RGB(0, green, 0) | blue | red,
				&OBJ_PLTT_COLOR(CANVAS_PALETTE, i));
		}
	}
	if(state->sparkles) {
		particle = state->particles;
		spr = state->sprs;
		for(i = 0; i < POUND_DRAWN_SPARKS; i++, spr++, particle++) {
			if(particle->tmr >= 0 && particle->tmr < 16) {
				spr->x = particle->x;
				spr->y = particle->y;
				fdEffectSprDraw(spr);
				fdAdvanceWithGravity2D(particle, 64, 0);
			}
			particle->tmr++;
		}
	}
	switch(state->state) {
	case POUND_FALLS:
		state->actor->y.fixed -= toFIXED(7) / 2;
		if(state->tmr == 8) {
			canvas->scaleX = toFIXED(1) / 8;
			canvas->scaleY = toFIXED(1) / 8;
			fdMapGridSetScreenShakeStub(
				toFIXED(13) / 10, toFIXED(13) / 10, toFIXED(1));
			sySndPlayStub(SE_POUND);
			state->sparkles = 1;
			state->landed = POUND_STAGE_LANDED;
		}
		if(state->tmr == 8) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case POUND_SHINE_GROWS:
		canvas->scaleX += toFIXED(1) / 16;
		canvas->scaleY = canvas->scaleX;
		if(canvas->scaleX > toFIXED(4) / 5 - 1) {
			canvas->scaleX = toFIXED(4) / 5;
			canvas->scaleY = toFIXED(4) / 5;
			state->state++;
			state->tmr = -1;
		}
		break;
	case POUND_SHINE_HOLDS:
		canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
		if(state->tmr == 32) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case POUND_SETTLES:
		if(state->tmr == 0)
			fdMapGridSetScreenShakeStub(1, 1, 1);
		if(state->tmr == 10) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case POUND_LIFTS:
		state->actor->y.fixed += toFIXED(3) / 2;
		if(state->tmr == 13)
			state->state = FIELD_PSYNERGY_FINISHED;
		break;
	}
	state->tmr++;
}

/* Event of a Pound that cannot come down: the fist sinks 2.5 pixels and shrinks
   by 1/64 a tick for 8 ticks, before it shatters. */
void fdPoundSinkFist(void)
{
	POUND_STATE *state = RamHeapSlots->battleEff;

	if(state->state == POUND_FALLS) {
		ACTOR *actor = state->actor;

		actor->y.fixed += -(toFIXED(5) / 2);
		actor->scaleX += -(toFIXED(1) / 64);
		actor->scaleY += -(toFIXED(1) / 64);
		if(state->tmr == 8)
			state->state = FIELD_PSYNERGY_FINISHED;
	}
	state->tmr++;
}

/* Pound cast on a target: runs fdPsynergyPound. */
void fdPsynergyPoundOnTarget(void)
{
	fdPsynergyPound();
}

void fdPsynergyStartSceneSpr(void);
void fdPsynergyShrinkToCaster(ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);

/* Pound ("Drive objects into the ground."): a giant fist forms over the spot 32
   pixels before the caster, or over the target, and comes down on it. Out of
   reach, or with a solid actor in the way, it shatters with the error sound;
   else the place's Pound script runs. */
void fdPsynergyPound(void)
{
	POUND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(POUND_STATE));
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	ACTOR					*child = scene->tgt;
	ACTOR					*caster = scene->caster;
	ACTOR					*actor;
	ACTOR					*fist;
	CANVAS_SPR_STATE		*canvas;
	EFFECT_SPR				*spr;
	EFFECT_SPR				*glow;
	void					*pending;
	Sint32					 pos[3];
	Sint32					 starEntry;
	Sint32					 starTile;
	Sint32					 glowEntry;
	Sint32					 inReach;
	FIXED					 lift;
	Sint32					 glowTile;
	Sint32					 size = POUND_GLOW_SIZE * POUND_GLOW_SIZE / 2;
	Sint32					 riseTicks;
	Sint32					 speed;
	Sint32					 ang;
	Sint32					 frm;
	Sint32					 j;
	FIELD_PSYNERGY_PARTICLE *particle;

	lift = toFIXED(32);
	if(child == NULL) {
		SET_POSITION(scene, caster->x.fixed, caster->y.fixed, caster->z.fixed);
		syVectorAddPolarOffset(lift, scene->ang, &scene->x);
	} else if(child->motionFlg & ACTOR_FLOATS) {
		scene->y = child->groundHeight;
		lift = child->y.fixed - child->groundHeight + toFIXED(32);
	}
	if(scene->aimAhead != 0) {
		/* fdMeasurePsynergyHeight returns -1 where the spot is out of reach. */
		if(fdMeasurePsynergyHeight(scene->fieldPsynergy, caster, &scene->x) ==
			-1)
			inReach = 0;
		else
			inReach = 1;
		if(inReach == 1) {
			for(j = 0; j < FIELD_ACTOR_COUNT; j++) {
				actor = field->actors[j];
				if(actor != NULL && actor->script != NULL &&
					(actor->collisionFlg & ACTOR_SOLID) &&
					fdPsynergyCheckOverlap(&scene->x, 4, &actor->x.fixed,
						actor->radius - 2) >= 0) {
					inReach = 0;
					break;
				}
			}
		}
	} else {
		inReach = 1;
		if(child != NULL && child->sceneByte != 0)
			inReach = 0;
	}

	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ITEM_BREAK);
	fist = fdActorSpawn(SPRITE_POUND_FIST, 0, 0, 0);
	state->actor = fist;
	if(fist != NULL) {
		fist->motionFlg = 0;
		if(field->psynergyDrawFlg & PSYNERGY_DRAW_FIXED_PRIORITY) {
			if((fist->sprStorageKind & ACTOR_SPRITE_KIND_MASK) ==
				ACTOR_SPRITE_ONE) {
				((EFFECT_SPR *)fist->spr)->oam.pri = 1;
				fist->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			}
		} else {
			fist->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		}
		fist->facing = caster->facing;
		fdActorSelectAnimationStub(fist, 2);
		if(!(field->psynergyDrawFlg & PSYNERGY_DRAW_NO_SCENE_SPRITE))
			fdPsynergyStartSceneSpr();

		fdBattleEffectStartCanvasSpr();
		canvas = RamHeapSlots->canvasSpr;
		canvas->pri = fdActorGetSprPriority(scene->caster);
		canvas->depth = 1;
		canvas->mode = CANVAS_SPRITE_256_COLORS;
		canvas->scaleX = 0;
		canvas->scaleY = 0;
		pos[0] = scene->x;
		pos[1] = scene->y;
		pos[2] = scene->z;
		fdCameraWorldToScreen(pos);
		canvas->x = (Sint16)FIXEDtoINT(pos[0]);
		canvas->y = (Sint16)FIXEDtoINT(pos[2]);
		syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_GLOW_DISC),
			RamHeapSlots->battleCanvas);
		canvas->command = CANVAS_UPLOAD;

		IWRAM_FILL_WORDS(state, size, 0);
		glowEntry = syVRAMCacheFindFreeEntry();
		glowTile = syVRAMCacheLoad(glowEntry, size, state);
		glow = &state->glow;
		fdEffectSprInit(glow, 32, 32, OBJ_SIZE_64X64, glowTile);
		glow->oam.pri = 0;
		glow->oam.fullCol = 0;
		glow->oam.pal = 0;
		glow->depth = 0;
		glow->scaleY = glow->scaleX = toFIXED(4) / 5;
		pos[0] = scene->x;
		pos[1] = scene->y;
		pos[2] = scene->z;
		fdCameraWorldToScreen(pos);
		glow->x = pos[0];
		glow->y = pos[2];

		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_GOLD_STAR), state);
		starEntry = syVRAMCacheFindFreeEntry();
		starTile = syVRAMCacheLoad(starEntry, 256, state);
		for(j = 0, particle = state->particles; j < POUND_SPARK_COUNT;
			j++, particle++) {
			spr = &state->sprs[j];
			speed = syRandom() * 18 / 10;
			speed += toFIXED(5) / 2;
			ang = j * (DEGtoANG(30)) + (syRandom() >> 5);
			pos[0] = scene->x;
			pos[1] = scene->y;
			pos[2] = scene->z;
			syVectorAddPolarOffset(toFIXED(4), ang, pos);
			fdCameraWorldToScreen(pos);
			particle->x = pos[0];
			particle->y = pos[2];
			particle->velX = syMulFX(syCos(ang), speed);
			particle->velY = syMulFX(sySin(ang), speed);
			particle->tmr = -j;
			fdEffectSprInit(spr, 8, 8, OBJ_SIZE_16X16, starTile);
			spr->oam.fullCol = 1;
			spr->oam.pal = 0;
			spr->oam.pri = 0;
			spr->depth = OAM_DEPTH_WINDOW;
			spr->scaleY = spr->scaleX = toFIXED(4) / 5;
		}
		sySynch(1);
		fdPsynergyGrowFromCaster(fist, scene->x, scene->y + lift, scene->z);
		fdActorSelectAnimationStub(fist, 1);
		sySynch(15);
		for(riseTicks = 0; riseTicks < 25; riseTicks++) {
			fist->y.fixed += toFIXED(1) / 2;
			sySynch(1);
		}
		sySynch(15);
		state->state = POUND_FALLS;
		state->tmr = 0;
		state->sparkles = 0;
		if(!inReach) {
			sySetEvent(fdPoundSinkFist, EVENT_PASS_TICK);
			while(state->state != FIELD_PSYNERGY_FINISHED)
				sySynch(1);
			syCloseEvent(fdPoundSinkFist);
			for(j = 0; j < POUND_SHATTER_PIECES; j++)
				fdBattleEffectSpawnScatterParticle(
					fist->x.fixed, fist->y.fixed, fist->z.fixed);
			sySndPlayStub(SE_MENU_ERROR);
			sySynch(1);
		} else {
			pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
				scene->fieldPsynergy,
				fdSelectPsynergyTarget(scene->fieldPsynergy));
			sySetEvent(fdPsynergyUpdatePound, EVENT_PASS_TICK);
			frm = 0;
			while(state->state != FIELD_PSYNERGY_FINISHED) {
				sySynch(1);
				scene->stage = state->landed;
				scene->frm = frm;
				if(field->mode != ACTOR_SYSTEM_WORLD_MAP &&
					!(field->psynergyDrawFlg & PSYNERGY_DRAW_NO_SCENE_SPRITE)) {
					glow = &state->glow;
					fdEffectSprDraw(glow);
				}
				fdRunPsynergyScript(pending, state->landed, frm++);
			}
			syCloseEvent(fdPsynergyUpdatePound);
			sySynch(10);
			sySndPlayStub(SE_MENU_ERROR);
			fdPsynergyShrinkToCaster(
				fist, fist->x.fixed, fist->y.fixed, fist->z.fixed);
		}
		fdActorReleaseStub(fist);
		syVRAMCacheFreeEntry(starEntry);
		syVRAMCacheFreeEntry(glowEntry);
		fdBattleEffectEndCanvasSpr();
	}
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* The pictures fdPictureShiftColumnsByWave shifts, 32 pixels square. */
#define WAVE_PICTURE_SIZE 32

/* Copies a 32 by 32 byte picture into a cleared one stretched down, its rows
   four pixels apart, each column slid down by phase (1.6 pixels a step, from
   124 pixels up) less a sine wave across the columns; what falls outside is
   dropped. The Scoop draws its rising picture with it. */
void fdPictureShiftColumnsByWave(Uint8 *src, Uint8 *dst, Sint32 phase)
{
	Sint32 width = WAVE_PICTURE_SIZE;
	Sint32 height = WAVE_PICTURE_SIZE;
	Sint32 column;
	Sint32 row;

	syWordsClear(dst, WAVE_PICTURE_SIZE * WAVE_PICTURE_SIZE);
	for(column = 0; column < width; column++) {
		FIXED wave = sySin(column * (TRIG_TURN / 8)) * 8 + toFIXED(8);

		for(row = 0; row < height; row++) {
			FIXED pos =
				phase * (toFIXED(8) / 5) - toFIXED(124) + row * (toFIXED(4));
			/* In two steps: the wave subtracted, then the shift. */
			Sint32 shift = pos - wave;
			shift = FIXEDtoINT(shift);

			if(shift >= 0 && shift < height)
				dst[shift * WAVE_PICTURE_SIZE + column] =
					src[row * width + column];
		}
	}
}

#undef POUND_SHINE_COLORS
#undef POUND_SHATTER_PIECES
#undef POUND_GLOW_SIZE
#undef WAVE_PICTURE_SIZE
