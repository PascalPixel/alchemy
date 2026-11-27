/* Ocean Fist. Part of battle_effect.c. */

#define OCEAN_FIST_TICKS	   92
/* The drops stop rising and the target is struck; the column fades from here
   for OCEAN_FIST_FADE_TICKS. */
#define OCEAN_FIST_STRIKE_TICK 32
#define OCEAN_FIST_FADE_TICKS  12
/* The damage is shown, and the camera's long shake gives way to a harder
   one. */
#define OCEAN_FIST_DAMAGE_TICK 38

/* The column: WhirlBeamSheet's three pictures of a swirling torrent, one a
   tick in turn, each drawn in up to three bands one above another from its
   foot up to its top, which rises OCEAN_FIST_COLUMN_RISE lines a tick from the
   bottom of the view. */
#define OCEAN_FIST_COLUMN_WIDTH	   96
#define OCEAN_FIST_COLUMN_HEIGHT   64
#define OCEAN_FIST_COLUMN_PICTURES 3
#define OCEAN_FIST_COLUMN_BANDS	   3
#define OCEAN_FIST_COLUMN_TICKS	   42
#define OCEAN_FIST_COLUMN_RISE	   8
/* The canvas line the column stands on, where the drops leave from; a falling
   drop is gone below OCEAN_FIST_DROP_FLOOR. */
#define OCEAN_FIST_COLUMN_FOOT_Y   108
#define OCEAN_FIST_DROP_FLOOR	   104

/* FlashBurstSheet, the spray, decoded after the drops in the map cell
   buffer. */
#define OCEAN_FIST_SPRAY_SHEET (RamMapMetatileBuffer + 16384)
/* The sprays given a countdown, of which the first OCEAN_FIST_SPRAYS burst at
   the column's foot, one a tick; each shows the sprite sheet's six pictures
   for three ticks apiece. */
#define OCEAN_FIST_SPRAY_SLOTS 32
#define OCEAN_FIST_SPRAYS	   24
#define OCEAN_FIST_SPRAY_TICKS (6 * 3)

/* The drops, in the map cell buffer: up to OCEAN_FIST_DROPS_A_TICK leave the
   column's foot each tick until the strike, and at OCEAN_FIST_FOUNTAIN_TICK
   every free one of the first OCEAN_FIST_FOUNTAIN_DROPS bursts up at once. */
#define OCEAN_FIST_DROPS		  512
#define OCEAN_FIST_DROPS_A_TICK	  16
#define OCEAN_FIST_FOUNTAIN_DROPS 256
#define OCEAN_FIST_FOUNTAIN_TICK  28

/* A swirling blue torrent rises from the ground at the target, spray bursting
   at its foot, while drops shoot up through it and a fountain of them bursts at
   once. At OCEAN_FIST_STRIKE_TICK the target is struck and thrown up, the
   column fades and the drops fall back.

   Abilities: Ocean Fist. */
void btEffectRunOceanFist(BATTLE_EFFECT_PARM *args)
{
	Uint8				*spraySheet = OCEAN_FIST_SPRAY_SHEET;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	Uint8				*sprites;
	BLITTER_PAIR		 blitters;
	SCREEN_POINT		 target;
	PARTICLE			*drops = (PARTICLE *)MapGridMetatileBuffer;
	PARTICLE			*drop;
	PARTICLE			*spray;
	Sint32				 tick;
	Sint32				 halfWidth;
	Sint32				 width;
	Uint8				*sheet;
	Sint32				 i;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	sprites = RamHeapSlots->battleSheet;
	btEffectConfigureEffectDisplay();
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, blitters);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, spraySheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	LOAD_BG_PALETTE(RESOURCE_BLUE_FLAME_SHEET);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	btEffectPosProjectBattler(args->targets[0], &target);

	{
		Sint32 scroll = 0;
		for(tick = 0; tick != OCEAN_FIST_TICKS;
			scroll += OCEAN_FIST_COLUMN_RISE, tick++) {
			if(tick == 0) {
				EFFECT_FREE_PARTICLES(i, PARTICLES, OCEAN_FIST_DROPS);
				/* DEAD: the column's pictures are already decoded; the ROM
				   keeps the second load. */
				btResourceLoadAndDecompress(
					RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1, 0);
				sySoundPlayStub(SE_CRASH);
				state->shakeFrame = 4;
				halfWidth = OCEAN_FIST_COLUMN_WIDTH / 2;
				width = OCEAN_FIST_COLUMN_WIDTH;
				sheet = OCEAN_FIST_SPRAY_SHEET;
			}
			if(tick == OCEAN_FIST_STRIKE_TICK) {
				btEffectTintBattler(
					args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				btEffectStartActorMotion(
					args->targets[0], 1, 0, 0, toFIXED(15), 100);
			}
			if(tick == OCEAN_FIST_DAMAGE_TICK) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				state->shakeFrame = 16;
			}

			/* From the strike, each tick halves the brightness of the canvas
			   shown, so each of the three fades to a sixteenth. */
			if(tick >= OCEAN_FIST_STRIKE_TICK &&
				tick < OCEAN_FIST_STRIKE_TICK + OCEAN_FIST_FADE_TICKS) {
				Uint8 *pixel = state->sheet +
					(tick % OCEAN_FIST_COLUMN_PICTURES) *
						PICTURE_BYTES(OCEAN_FIST_COLUMN);

				for(i = 0; i != PICTURE_BYTES(OCEAN_FIST_COLUMN); i++) {
					Sint32 value = *pixel;

					*pixel = value >> 1;
					pixel++;
				}
			}

			/* The column, its bands scrolling up as its top rises. */
			if(tick >= 0 && tick < OCEAN_FIST_COLUMN_TICKS) {
				Sint32 top = BATTLE_VIEW_HEIGHT - scroll;
				Sint32 picture = tick % OCEAN_FIST_COLUMN_PICTURES;
				Sint32 x = target.x / 2;
				Sint32 phase = scroll % OCEAN_FIST_COLUMN_HEIGHT;

				if(top < 0) {
					top = 0;
				}
				for(i = 0; i != OCEAN_FIST_COLUMN_BANDS; i++) {
					Sint32 y = i * OCEAN_FIST_COLUMN_HEIGHT - phase;
					Sint32 height = OCEAN_FIST_COLUMN_FOOT_Y - y;

					if(height > OCEAN_FIST_COLUMN_HEIGHT) {
						height = OCEAN_FIST_COLUMN_HEIGHT;
					}
					if(height > 0 && y >= top - OCEAN_FIST_COLUMN_HEIGHT) {
						if(y < top) {
							height -= top - y;
							y = top;
						}
						blitters[1](canvas,
							state->sheet +
								picture * PICTURE_BYTES(OCEAN_FIST_COLUMN),
							x - halfWidth, y, width, height);
					}
				}
			}

			/* The spray at the column's foot: spray i sets out at tick i + 3
			   and rises as it burns out. */
			if(tick == 0) {
				for(i = 0; i != OCEAN_FIST_SPRAY_SLOTS; i++) {
					state->particles[i].variant = -i - 4;
				}
			}
			for(i = 0, spray = state->particles; i != OCEAN_FIST_SPRAYS;
				i++, spray++) {
				if((Uint32)spray->variant < OCEAN_FIST_SPRAY_TICKS) {
					blitters[0](canvas,
						(Uint8 *)((spray->variant / 3) *
								PICTURE_BYTES(FLASH_BURST) +
							(Sint32)sheet),
						FIXED_INTEGER_PART(spray->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(spray->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(spray, 62, -toFIXED(1) / 8);
				}
				if(++spray->variant == 0) {
					spray->x = toFIXED((syRandom() & 31) + target.x / 2 - 16);
					spray->y = INTtoFIXED((syRandom() & 7) + 88);
					spray->velocityX = ((syRandom() & 255) - 128) << 11;
					spray->velocityY = 0;
				}
			}

			/* The fountain: free drops set out in an oval about canvas (32, 96)
			   and fly up and out. */
			if(tick == OCEAN_FIST_FOUNTAIN_TICK) {
				PARTICLE *fountain;

				for(i = 0, fountain = drops; i != OCEAN_FIST_FOUNTAIN_DROPS;
					i++, fountain++) {
					if(fountain->variant == -1) {
						Sint32 radius = syRandom() & 63;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;

						fountain->x =
							((sySin(angle) * radius) >> 3) + toFIXED(32);
						fountain->y =
							((syCos(angle) * radius) >> 2) + toFIXED(96);
						fountain->velocityX = ((syRandom() & 63) - 32) << 14;
						fountain->velocityY = (-(syRandom() & 63) - 8) << 13;
						fountain->variant = 0;
					}
				}
			}

			/* Until the strike, drops leave the column's foot and are pulled
			   up, each by one of OceanFistDropGravity's four; then they
			   fall. */
			if(tick >= 0 && tick < OCEAN_FIST_STRIKE_TICK) {
				Sint32 spawned = 0;

				for(i = 0, drop = drops; i != OCEAN_FIST_DROPS; i++, drop++) {
					if(drop->variant == -1) {
						drop->x =
							toFIXED((syRandom() & 31) + target.x / 2 - 16);
						drop->y = toFIXED(OCEAN_FIST_COLUMN_FOOT_Y);
						drop->velocityX = ((syRandom() & 63) - 32) << 14;
						drop->velocityY = (-(syRandom() & 63) - 8) << 13;
						drop->variant = 0;
						if(++spawned == OCEAN_FIST_DROPS_A_TICK) {
							break;
						}
					}
				}
			}
			for(i = 0, drop = drops; i != OCEAN_FIST_DROPS; i++, drop++) {
				if(drop->variant >= 0) {
					Sint32 size = i % 3 + 3;

					EFFECT_DRAW_MOTE(blitters[i & 1], canvas, sprites,
						FIXED_INTEGER_PART(drop->x),
						FIXED_INTEGER_PART(drop->y), size);
					if(tick >= OCEAN_FIST_STRIKE_TICK) {
						btParticleAdvanceWithGravity2D(
							drop, 62, toFIXED(1) / 4);
					} else {
						btParticleAdvanceWithGravity2D(
							drop, 62, OceanFistDropGravity[i & 3]);
					}
					drop->variant++;
					if(drop->velocityY > 0 &&
						FIXEDtoINT(drop->y) > OCEAN_FIST_DROP_FLOOR) {
						drop->variant = -1;
					}
					/* BUG: the drop's 16.16 y is compared with its size in
					   whole pixels, so a rising drop goes as it crosses the
					   canvas's top line rather than once it is past it. */
#if FIX_OCEAN_FIST_DROPS_VANISH_AT_TOP
					if(drop->y < toFIXED(-size)) {
#else
					if(drop->y < -size) {
#endif
						drop->variant = -1;
					}
				}
			}
			if(tick < OCEAN_FIST_DAMAGE_TICK) {
				state->shakeFrame = 16;
			}
			if(tick <= 7) {
				btCameraShake(8, 8);
			} else if(tick < OCEAN_FIST_DAMAGE_TICK) {
				btCameraShake(4, 4);
			} else {
				btCameraShake(8, 8);
			}
			EFFECT_END_TICK(state);
		}
	}
	EFFECT_FINISH_CANVAS();
}

#undef OCEAN_FIST_TICKS
#undef OCEAN_FIST_STRIKE_TICK
#undef OCEAN_FIST_FADE_TICKS
#undef OCEAN_FIST_DAMAGE_TICK
#undef OCEAN_FIST_COLUMN_WIDTH
#undef OCEAN_FIST_COLUMN_HEIGHT
#undef OCEAN_FIST_COLUMN_PICTURES
#undef OCEAN_FIST_COLUMN_BANDS
#undef OCEAN_FIST_COLUMN_TICKS
#undef OCEAN_FIST_COLUMN_RISE
#undef OCEAN_FIST_COLUMN_FOOT_Y
#undef OCEAN_FIST_DROP_FLOOR
#undef OCEAN_FIST_SPRAY_SHEET
#undef OCEAN_FIST_SPRAY_SLOTS
#undef OCEAN_FIST_SPRAYS
#undef OCEAN_FIST_SPRAY_TICKS
#undef OCEAN_FIST_DROPS
#undef OCEAN_FIST_DROPS_A_TICK
#undef OCEAN_FIST_FOUNTAIN_DROPS
#undef OCEAN_FIST_FOUNTAIN_TICK
