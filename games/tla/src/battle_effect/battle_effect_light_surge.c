/* Light Surge, then the handlers that play the water column for Whorl, Gale,
   Lethe Albion, Poison Flow and Raging Flood. Part of battle_effect.c. */

#define LIGHT_SURGE_TICKS		 74
/* The tick the battle shows the damage. */
#define LIGHT_SURGE_DAMAGE_TICK	 64
/* From this tick the colours step towards the Mercury Djinn sprite sheet's,
   one step every fourth tick. */
#define LIGHT_SURGE_RECOLOR_TICK 32

/* The state's particles: each bolt's flash from the first, its glow from
   SURGE_FIRST_GLOW and the bolt itself from SURGE_FIRST_BOLT. The first tick
   sets out a bolt in every particle from SURGE_FIRST_BOLT on, each starting
   SURGE_BOLT_INTERVAL ticks after the last; the first SURGE_BOLTS fall. */
#define SURGE_FIRST_GLOW	16
#define SURGE_FIRST_BOLT	32
#define SURGE_BOLTS			14
#define SURGE_BOLT_INTERVAL 4

/* A bolt's head falls from the sky to the target's feet in SURGE_BOLT_FALL
   ticks and lands with a flash; its tail leaves the sky at SURGE_BOLT_TAIL and
   follows it down, leaves a glow where it is at SURGE_BOLT_GLOW, and the bolt
   is gone after SURGE_BOLT_TICKS. */
#define SURGE_BOLT_FALL		  6
#define SURGE_BOLT_TAIL		  12
#define SURGE_BOLT_GLOW		  16
#define SURGE_BOLT_TICKS	  18
/* Half a bolt's width on the canvas. */
#define SURGE_BOLT_HALF_WIDTH 2

/* The shaded discs the flashes and glows are drawn from: SURGE_CIRCLES quarter
   discs, 2 to SURGE_CIRCLE_SIZE_MAX pixels, packed in the map cell buffer. */
#define SURGE_CIRCLES		  32
#define SURGE_CIRCLE_SIZE_MAX (SURGE_CIRCLES * 2)
/* The discs a tick draws: the flashes, and the glows of all but the last two
   bolts. Each swells and shrinks over SURGE_DISC_TICKS, half a turn of a
   sine. */
#define SURGE_DISCS			  28
#define SURGE_DISC_TICKS	  16

/* The camera swings around the target SURGE_SWING_TURN a tick from
   SURGE_SWING_START, slowing over SURGE_SWING_SLOWING ticks to a stop at
   SURGE_SWING_END, after the effect has ended. */
#define SURGE_SWING_START	20
#define SURGE_SWING_END		80
#define SURGE_SWING_TURN	(TRIG_TURN / 256)
#define SURGE_SWING_SLOWING 8

/* The four rings of light about the target: they show from SURGE_RINGS_START,
   swell over a quarter turn of a sine to their full size at SURGE_RINGS_GROWN,
   brighten until SURGE_RINGS_SHOWN and dim from SURGE_RINGS_DIMMING, 8 shades a
   tick. */
#define SURGE_RINGS			4
#define SURGE_RINGS_START	16
#define SURGE_RINGS_SHOWN	24
#define SURGE_RINGS_GROWN	48
#define SURGE_RINGS_DIMMING 64
#define SURGE_RINGS_END		80

/* Where GlowOrbTexture is decoded in the effect's pictures. */
#define LIGHT_SURGE_GLOW_ORB 4096

/* Where GlowBandTextureA is decoded in the effect's pictures. */
#define LIGHT_SURGE_GLOW_BAND 16384

/* The camera swings around the target while SURGE_BOLTS bolts of light rain
   onto it, SURGE_BOLT_INTERVAL ticks apart; each lands with a flash and leaves
   a glow, and SURGE_RINGS rings of light swell about the target and sway.

   Abilities: Light Surge. */
void btEffectRunLightSurge(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 radius;
	Sint32				 angle;
	Sint32				 z;
	Sint32				 turn;
	Sint32				 half;
	Sint32				 index;
	Sint16				*entry;
	/* FAKEMATCH(temporary): out, a second pointer holding &screen, taken from
	 * pos in one arm and as &screen in the other. */
	SCREEN_POINT	 *out;
	Sint32			  growth;
	Uint32			  scale;
	BATTLE_CAMERA	 *camera;
	Sint16			  savedYaw;
	MOTION_OBJECT	 *target;
	Sint16			 *circles;
	Sint32			  x;
	Sint32			  y;
	SCREEN_POINT	  screen;
	SCREEN_POINT	 *pos;
	Sint32			  point[3];
	SCREEN_POINT	  center;
	MODEL_TEXTURE	  texture;
	RENDER_MODEL	 *model;
	PARTICLE		 *fall;
	PARTICLE		 *bolt;
	PARTICLE		 *disc;
	PARTICLE		 *flash;
	PARTICLE		 *glow;
	PROJECTED_VERTEX *vertices;
	Sint32			  offset;
	Sint32			  size;
	Sint32			  shade;
	Uint8			 *sprites;
	Uint8			 *cells;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	sprites = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_ORB_TEXTURE, state->sheet + LIGHT_SURGE_GLOW_ORB, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
		state->sheet + LIGHT_SURGE_GLOW_BAND, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_F);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	camera = RamHeapSlots->cameraState;
	savedYaw = camera->yaw;
	target = btObjectGetStub(args->targets[0])->object;

	/* The discs' quarters, packed in the map cell buffer; the sheet's start
	   keeps where each lies. */
	cells = MapGridMetatileBuffer;
	circles = (Sint16 *)state->sheet;
	offset = 0;
	size = 2;
	for(i = 0; i != SURGE_CIRCLES; i++) {
		btEffectDrawShadedCircles(
			(Uint8 *)(offset + (Sint32)cells), size, toFIXED(1));
		circles[i] = offset;
		offset += (i * 2 + 3) * size / 2;
		size += 2;
	}

	frame = 0;
	/* FAKEMATCH(dead): pos first takes the camera's look-at point. */
	pos = (SCREEN_POINT *)camera->focus;
	/* FAKEMATCH(dead): the texture's address, computed as center's address
	 * less 8. */
	vertices = (PROJECTED_VERTEX *)((Sint32)&center - 8);
	/* FAKEMATCH(temporary): pos holds &screen. */
	pos = &screen;
	for(; frame != LIGHT_SURGE_TICKS; frame++) {
		if(frame == LIGHT_SURGE_DAMAGE_TICK) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frame >= LIGHT_SURGE_RECOLOR_TICK && (frame & 3) == 0) {
			btEffectStepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
		}
		if(frame == 0) {
			camera->yaw = savedYaw;
			EFFECT_FREE_PARTICLES(i, state->particles, SURGE_FIRST_BOLT);
			/* Each bolt falls from 110 pixels up, 32 to 95 pixels out from the
			   middle of the battlefield, to the ground 4 to 35 pixels out from
			   the target in the same direction. */
			for(i = 0; i != ARRAY_COUNT(state->particles) - SURGE_FIRST_BOLT;
				i++) {
				fall = &state->particles[SURGE_FIRST_BOLT + i];
				radius = (syRandom() & 63) + 32;
				angle = syRandom() & TRIG_ANGLE_MASK;

				fall->x = sySin(angle) * radius;
				fall->z = syCos(angle) * radius;
				fall->y = toFIXED(110);
				radius = (syRandom() & 31) + 4;
				fall->velocityX = target->x + sySin(angle) * radius;
				z = target->z + syCos(angle) * radius;
				/* DEAD: the z and y speeds are overwritten below before any
				   read; the ROM keeps the stores. */
				fall->velocityZ = z;
				fall->velocityY = -toFIXED(8);
				fall->velocityX = (fall->velocityX - fall->x) / SURGE_BOLT_FALL;
				fall->velocityY = (-toFIXED(8) - fall->y) / SURGE_BOLT_FALL;
				fall->velocityZ = (z - fall->z) / SURGE_BOLT_FALL;
				fall->variant = -i * SURGE_BOLT_INTERVAL;
			}
		}
		if((Uint32)(frame - SURGE_SWING_START) <
			SURGE_SWING_END - SURGE_SWING_START) {
			if(frame < SURGE_SWING_END - SURGE_SWING_SLOWING) {
				turn = SURGE_SWING_TURN;
			} else {
				turn = SURGE_SWING_TURN -
					(frame - (SURGE_SWING_END - SURGE_SWING_SLOWING)) *
						(SURGE_SWING_TURN / SURGE_SWING_SLOWING);
			}
			if(args->side == EFFECT_SIDE_ENEMIES) {
				camera->yaw += turn;
			} else {
				camera->yaw -= turn;
			}
		}
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);

		/* The flashes and glows, each a shaded disc of four quarters: a flash
		   shows only its upper half. */
		for(i = 0, disc = state->particles; i != SURGE_DISCS; disc++, i++) {
			Sint32 size;

			if((Uint32)disc->variant >= SURGE_DISC_TICKS) {
				continue;
			}
			/* Its size swells to twice the radius in velocityX and shrinks
			   back. */
			size = (FIXEDtoINT(sySin(disc->variant *
								   (DEGtoANG(180) / SURGE_DISC_TICKS)) *
					   disc->velocityX)) *
				2;
			index = size - 2;
			disc->variant++;
			/* A disc with no size, or as large as the largest circle, is not
			   drawn. */
			if((Uint32)index >= SURGE_CIRCLE_SIZE_MAX - 2) {
				continue;
			}
			btEffectPosProjectPoint(disc, pos);
			index >>= 1;
			x = pos->x / 2;
			y = pos->y;
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
			index <<= 1;
			entry = (Sint16 *)(index + (Sint32)circles);
			half = size >> 1;
			(RamHeapSlots->ramCodePaired)(canvas,
				(void *)(*entry + (Sint32)cells), x - half, y - size, half,
				size);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
			(RamHeapSlots->ramCodePaired)(canvas,
				(void *)(*entry + (Sint32)cells), x, y - size, half, size);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			if(i >= SURGE_FIRST_GLOW) {
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
				(RamHeapSlots->ramCodePaired)(canvas,
					(void *)(*entry + (Sint32)cells), x - half, y, half, size);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
				(RamHeapSlots->ramCodePaired)(
					canvas, (void *)(*entry + (Sint32)cells), x, y, half, size);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}

		/* The bolts: GlowBandTextureA across a quad from its tail to its
		   head. */
		vertices = syHeapAllocateEWRAM(4 * sizeof(PROJECTED_VERTEX));
		model = btRenderAllocateModelList(1);
		shade = 0;
		texture.logWidth = 5;
		texture.logHeight = 2;
		model->texture = &texture;
		model->mesh = BattleEffectSmallSquareMeshReversed;
		model->vertices = (Sint16 *)vertices;
		model->shade = shade;
		model->kind = MODEL_SHADED_ADDITIVE;
		texture.pixels = state->sheet + LIGHT_SURGE_GLOW_BAND;
		for(i = 0, flash = state->particles,
		glow = &state->particles[SURGE_FIRST_GLOW],
		bolt = &state->particles[SURGE_FIRST_BOLT];
			i != SURGE_BOLTS; flash++, glow++, bolt++, i++) {
			Sint32 t = ++bolt->variant;

			if((Uint32)t >= SURGE_BOLT_TICKS) {
				continue;
			}
			if(t >= SURGE_BOLT_TAIL) {
				point[0] = bolt->x + bolt->velocityX * (t - SURGE_BOLT_TAIL);
				point[1] = bolt->y +
					bolt->velocityY * (bolt->variant - SURGE_BOLT_TAIL);
				point[2] = bolt->z +
					bolt->velocityZ * (bolt->variant - SURGE_BOLT_TAIL);
				if(bolt->variant == SURGE_BOLT_GLOW) {
					SET_POSITION(glow, point[0], point[1], point[2]);
					glow->velocityX = (syRandom() & 15) + 8;
					glow->variant = 0;
				}
				out = pos;
				btEffectPosProjectPoint(point, out);
			} else {
				out = &screen;
				btEffectPosProjectPoint(bolt, out);
			}
			vertices[0].x = out->x / 2 - SURGE_BOLT_HALF_WIDTH;
			vertices[0].y = out->y;
			vertices[1].x = out->x / 2 + SURGE_BOLT_HALF_WIDTH;
			vertices[1].y = out->y;
			if(bolt->variant < SURGE_BOLT_FALL) {
				point[0] = bolt->x + bolt->velocityX * bolt->variant;
				point[1] = bolt->y + bolt->velocityY * bolt->variant;
				point[2] = bolt->z + bolt->velocityZ * bolt->variant;
			} else {
				point[0] = bolt->x + bolt->velocityX * SURGE_BOLT_FALL;
				point[1] = bolt->y + bolt->velocityY * SURGE_BOLT_FALL;
				point[2] = bolt->z + bolt->velocityZ * SURGE_BOLT_FALL;
			}
			if(bolt->variant == SURGE_BOLT_FALL) {
				SET_POSITION(flash, point[0], point[1], point[2]);
				flash->velocityX = (syRandom() & 7) + 24;
				flash->variant = 0;
				state->shakeFrame = 4;
				sySoundPlayStub(SE_CRASH);
				btMotionApplyKnockbackStub(args->targets[0], KNOCKBACK_NUDGE);
				btEffectTintBattler(
					args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 2);
			}
			btEffectPosProjectPoint(point, out);
			vertices[2].x = out->x / 2 - SURGE_BOLT_HALF_WIDTH;
			vertices[2].y = out->y;
			vertices[3].x = out->x / 2 + SURGE_BOLT_HALF_WIDTH;
			vertices[3].y = out->y;
			btRenderDrawModelList(model);
		}

		/* The rings: GlowOrbTexture on a square, twice as tall on the canvas as
		   it is wide, so round on the screen. */
		if((Uint32)(frame - SURGE_RINGS_START) <
			SURGE_RINGS_END - SURGE_RINGS_START) {
			btEffectPosProjectBattler(args->targets[0], &center);
			texture.logWidth = 5;
			texture.logHeight = 5;
			texture.pixels = state->sheet + LIGHT_SURGE_GLOW_ORB;
			model->kind = MODEL_SHADED_MAX;
			model->mesh = BattleEffectSmallSquareMeshReversed;
			for(i = 0; i != SURGE_RINGS; i++) {
				/* Each sways along a line through the target, turned with the
				   camera:
				   across the screen, and into it drawn as up and down. */
				FIXED  sway = sySin((frame << 8) + TRIG_TURN / 16);
				Sint32 x = INTtoFIXED(center.x / 2 - CANVAS_SIZE / 2) -
					(LightSurgeRingSways[i] * sway / toFIXED(1)) *
						sySin(camera->yaw);
				Sint32 z = -((LightSurgeRingSways[i] * sway / toFIXED(1)) *
					syCos(camera->yaw));

				/* FAKEMATCH(dead): a store both arms below overwrite. */
				if(i == 300)
					growth = 0;
				if(frame < SURGE_RINGS_GROWN) {
					growth = sySin((frame - SURGE_RINGS_START) *
						(DEGtoANG(90) /
							(SURGE_RINGS_GROWN - SURGE_RINGS_START)));
				} else {
					growth = toFIXED(1);
				}
				scale = (Uint32)(LightSurgeRingSizes[i] * growth) / toFIXED(1);
				if(frame >= SURGE_RINGS_DIMMING) {
					shade = LightSurgeRingShades[i] -
						(frame - SURGE_RINGS_DIMMING) * 8;
				} else if(frame < SURGE_RINGS_SHOWN) {
					shade = LightSurgeRingShades[i] +
						(frame - SURGE_RINGS_SHOWN) * 8;
				} else {
					shade = LightSurgeRingShades[i];
				}
				model->shade = shade;
				syInitMatrix();
				syTranslate(x, z, 0);
				syScale(scale, scale * 2, scale);
				btRenderTransformVertices(
					BattleEffectSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);
		/* FAKEMATCH(dead): a read of shade that does nothing. */
		btCameraShake(4 + ((shade & 255) >> 8), 4);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Abilities: Whorl. */
void btEffectRunWhorl(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTwistingColumn(args, TWISTING_COLUMN_WHORL);
}

/* Abilities: Gale. */
void btEffectRunGale(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTwistingColumn(args, TWISTING_COLUMN_GALE);
}

/* Abilities: Lethe Albion. */
void btEffectRunLetheAlbion(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTwistingColumn(args, TWISTING_COLUMN_LETHE_ALBION);
}

/* Abilities: Poison Flow. */
void btEffectRunPoisonFlow(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTwistingColumn(args, TWISTING_COLUMN_POISON_FLOW);
}

/* Abilities: Raging Flood. */
void btEffectRunRagingFlood(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTwistingColumn(args, TWISTING_COLUMN_RAGING_FLOOD);
}

#undef LIGHT_SURGE_TICKS
#undef LIGHT_SURGE_DAMAGE_TICK
#undef LIGHT_SURGE_RECOLOR_TICK
#undef SURGE_FIRST_GLOW
#undef SURGE_FIRST_BOLT
#undef SURGE_BOLTS
#undef SURGE_BOLT_INTERVAL
#undef SURGE_BOLT_FALL
#undef SURGE_BOLT_TAIL
#undef SURGE_BOLT_GLOW
#undef SURGE_BOLT_TICKS
#undef SURGE_BOLT_HALF_WIDTH
#undef SURGE_CIRCLES
#undef SURGE_CIRCLE_SIZE_MAX
#undef SURGE_DISCS
#undef SURGE_DISC_TICKS
#undef SURGE_SWING_START
#undef SURGE_SWING_END
#undef SURGE_SWING_TURN
#undef SURGE_SWING_SLOWING
#undef SURGE_RINGS
#undef SURGE_RINGS_START
#undef SURGE_RINGS_SHOWN
#undef SURGE_RINGS_GROWN
#undef SURGE_RINGS_DIMMING
#undef SURGE_RINGS_END
#undef LIGHT_SURGE_GLOW_ORB
#undef LIGHT_SURGE_GLOW_BAND
