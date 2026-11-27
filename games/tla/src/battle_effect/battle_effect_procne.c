/* The Procne summon, and the Tiamat summon and Dragon Driver. Part of
   battle_effect.c. */

/* Who breathes the dragon's fire (btEffectRunDragonFire). */
typedef enum DRAGON_FIRE_MODE {
	/* Tiamat lands beside the targets and breathes the fire, then bursts away
	   (btEffectRunImpactBurst) */
	DRAGON_FIRE_TIAMAT,
	/* no dragon is drawn: the breath starts just in front of the targets, in
	   the LightningBolt colours */
	DRAGON_FIRE_DRAGON_DRIVER
} DRAGON_FIRE_MODE;

/* v % 3, through the multiply-high function and the reciprocal of 3, sparing a
   division. */
#define MODULO_THREE(v) syMathUnsignedMulHighAdd((v), RECIPROCAL_OF(3), -3)

#define PROCNE_TICKS		  400 /* ticks of Procne */
/* Procne appears in the violet sky, and the targets it lifted drop. */
#define PROCNE_ARRIVAL_TICK	  160
#define PROCNE_DROP_TICK	  395
/* The first comet leaves Procne, and the next each PROCNE_COMET_INTERVAL ticks
   after. */
#define PROCNE_COMET_TICK	  330
#define PROCNE_COMET_INTERVAL 12

#define PROCNE_SPRITES 9 /* Procne's sprites, on ProcneGridX, ProcneGridY */

/* The whirlwind sprite sheet: three whirlwind pictures, shown 4 ticks each,
   then the dust. */
#define PROCNE_WHIRLWIND_WIDTH	48
#define PROCNE_WHIRLWIND_HEIGHT 64
#define PROCNE_WHIRLWIND_PICTURE \
	(PROCNE_WHIRLWIND_WIDTH * PROCNE_WHIRLWIND_HEIGHT)
/* The whirlwind rises in two pieces: its foot, a picture's upper rows, then a
   whole picture above it. */
#define PROCNE_WHIRLWIND_FOOT_HEIGHT 48
/* The dust picture after them, a 16x16 puff, drawn with its middle 4 pixels
   left of and above its mote. */
#define PROCNE_DUST					 (3 * PROCNE_WHIRLWIND_PICTURE)
#define PROCNE_DUST_SIZE			 16
#define PROCNE_COMET_PICTURES		 13824

#define PROCNE_DUST_MOTES	  24 /* dust motes that swirl up the whirlwind */
/* How much faster a dust mote circles the whirlwind each tick, up to
   PROCNE_DUST_SPIN_MAX, in 1/65536 turns a tick. */
#define PROCNE_DUST_SPIN_GAIN 50
#define PROCNE_DUST_SPIN_MAX  (TRIG_TURN / 16)
/* The distance from the whirlwind's centre, in half pixels, that the lifted
   targets spiral in to. */
#define PROCNE_CORE_RADIUS	  16
/* How much faster the lifted targets circle each tick, in 1/65536 turns a
   tick, and how fast they circle once Procne arrives. */
#define PROCNE_SPIN_GAIN	  48
#define PROCNE_SKY_SPIN		  6000

/* Procne's sway: sixty-fourths of the wave's sine, as 16.16 pixels a tick,
   across and up and down; it bobs twice as far over its first 64 ticks. */
#define PROCNE_SWAY 32
#define PROCNE_BOB	16

/* Procne's particles in state->particles: the sparks that gather on it, the
   ring of light each comet leaves, then one per comet. */
#define PROCNE_GATHERING_SPARKS 32
#define PROCNE_RING				PROCNE_GATHERING_SPARKS
#define PROCNE_RING_MOTES		28
#define PROCNE_COMET_PARTICLES	(PROCNE_RING + PROCNE_RING_MOTES)

/* The four strikes, at ProcneTicks: each throws out its sparks from its own
   rows of PARTICLES. */
#define PROCNE_STRIKES			 4
#define PROCNE_STRIKE_SPARKS	 96
#define PROCNE_STRIKE_SPARK_ROWS 128

/* The depths a spark is drawn between. */
#define PROCNE_DEPTH_NEAR 170
#define PROCNE_DEPTH_FAR  350
/* A spark's mote size at `depth`: `largest` at PROCNE_DEPTH_NEAR, down to 1 at
   PROCNE_DEPTH_FAR. */
#define PROCNE_SPARK_SIZE(depth, largest) \
	((largest) - \
		syMathUnsignedMulHigh((depth) - PROCNE_DEPTH_NEAR, \
			RECIPROCAL_OF( \
				(PROCNE_DEPTH_FAR - PROCNE_DEPTH_NEAR) / ((largest) - 1))))

/* A whirlwind lifts the targets and spins them while dust swirls up it; then
   Procne, a great bird in a violet sky, gathers sparks and strikes them with
   four comets, each throwing out a burst of sparks. A or B skips to Procne's
   arrival, then to the drop.

   Abilities: Procne. */
void btEffectRunProcne(BATTLE_EFFECT_PARM *args)
{
	void				*canvas;
	BATTLE_CAMERA		*camera;
	BATTLE_EFFECT_STATE *state;
	BLITTER_PAIR		 draw;
	Sint32				 frm;
	Uint8				*graphics;
	MOTION_OBJECT		*tgt;
	BATTLE_OBJECT		*battleObject;
	PARTICLE			*point;
	PARTICLE			*spark;
	PARTICLE			*comet;
	Sint32				 idx;
	Sint32				 slot;
	Sint32				 size;
	Sint32				 moteSize;
	Sint32				 dist;
	Sint32				 arrived;
	Sint32				 heading[8];
	Sint32				 radius[8];
	Sint32				 spin[8];
	POINT2_D			 home[8];
	Sint32				 baseAng[8];
	FIXED				 world[3];
	SCREEN_POINT		 screen;
	FIXED				 depthOfs[3];
	SPR_PLACEMENT		 place;
	SCALE				 scale;

	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	state = RamHeapSlots->battleEff;
	graphics = RamHeapSlots->battleSheet;

	btEffectStartCanvasLayer(0);
	btEffectBuildCanvasBlitters(0, draw);
	{
		POINT2_D wave;
		POINT2_D drift;
		btResourceLoadAndDecompress(
			RESOURCE_WHIRLWIND_SHEET, state->sheet, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
		EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		wave.x = toFIXED(128);
		wave.y = toFIXED(40);
		drift.x = 0;
		drift.y = 0;

		/* Each target's place, for the whirlwind to turn it. */
		EFFECT_FOR_EACH_TARGET(idx, args) {
			battleObject = btObjectGetStub(args->tgts[idx]);
			tgt = battleObject->obj;
			home[idx].x = tgt->x;
			home[idx].y = tgt->z;
			baseAng[idx] = tgt->ang;
			heading[idx] = (Uint16)syAtan(tgt->x, tgt->z);
			radius[idx] = sySquartIWRAM((tgt->x >> 8) * (tgt->x >> 8) +
							  (tgt->z >> 8) * (tgt->z >> 8)) >>
				7;
			spin[idx] = 0;
			tgt->verticalMotionStrength = 0;
		}

		/* The dust motes start at the whirlwind's foot, each after a wait of
		   its own. */
		for(idx = 0; idx != ARRAY_COUNT(state->particles); idx++) {
			point = &state->particles[idx];
			point->z = 120;
			point->y = 0;
			syRandom(); /* a number drawn and not used */
			point->velY = 0;
			point->velX = 0;
			point->variant = syRandom() & 63;
		}

		for(frm = 0; frm != PROCNE_TICKS; frm++) {
			if((PerWork.repeat & (KEY_A | KEY_B)) != 0) {
				if(frm < PROCNE_ARRIVAL_TICK)
					frm = PROCNE_ARRIVAL_TICK;
				else if(frm < PROCNE_DROP_TICK)
					frm = PROCNE_DROP_TICK;
			}

			EFFECT_VIEW_FROM_CAMERA(camera);

			if(frm == 16)
				sySndPlayStub(SE_RUMBLE);
			if(frm == 256)
				sySndPlayStub(SE_SPIN);
			if(frm == 334)
				sySndPlayStub(SE_POWER_UP);
			if(frm == 347)
				sySndPlayStub(SE_POWER_UP);
			if(frm == 359)
				sySndPlayStub(SE_POWER_UP);
			if(frm == 372)
				sySndPlayStub(SE_POWER_UP);

			/* The targets, one every 16 ticks, spiral in toward the centre and
			   rise, spinning faster; each strike flashes them. */
			EFFECT_FOR_EACH_TARGET(idx, args) {
				if(frm > idx * 16) {
					battleObject = btObjectGetStub(args->tgts[idx]);
					tgt = battleObject->obj;
					tgt->x = (radius[idx] * sySin(heading[idx])) >> 1;
					tgt->z = (radius[idx] * syCos(heading[idx])) >> 1;

					if(frm < PROCNE_ARRIVAL_TICK) {
						if(frm > idx * 16 + 16)
							spin[idx] += PROCNE_SPIN_GAIN;
						if(radius[idx] <= 31)
							tgt->y += toFIXED(6);
						else
							tgt->y += toFIXED(1) / 2;
						if(tgt->y > toFIXED(124))
							tgt->y = toFIXED(124);
					}

					/* DEAD: always true, as the effect ends at tick 399; the
					   ROM keeps the test. */
					if(frm <= 490) {
						heading[idx] += spin[idx];
						if(heading[idx] > TRIG_TURN)
							heading[idx] -= TRIG_TURN;
						tgt->ang += spin[idx] / 4;
					}

					if(frm == PROCNE_DROP_TICK)
						tgt->verticalMotionStrength = BATTLE_UNIT_GRAVITY;

					for(slot = 0; slot != PROCNE_STRIKES; slot++) {
						if(frm == ProcneTicks[slot]) {
							tgt->velY = 0;
							btEffectTintBattler(args->tgts[idx],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, idx, 8);
						}
					}

					if(radius[idx] > PROCNE_CORE_RADIUS)
						radius[idx] -= 2;
				}
			}

			/* The whirlwind's foot grows from tick 16, its top from 48; once
			   Procne arrives it stands two pictures high. */
			if(frm >= 16 && frm < PROCNE_ARRIVAL_TICK) {
				size = frm * 2 - 32;
				if(size > PROCNE_WHIRLWIND_FOOT_HEIGHT)
					size = PROCNE_WHIRLWIND_FOOT_HEIGHT;
				draw[0](canvas,
					&state->sheet[MODULO_THREE(frm / 4) *
							PROCNE_WHIRLWIND_PICTURE +
						(PROCNE_WHIRLWIND_FOOT_HEIGHT - size) *
							PROCNE_WHIRLWIND_WIDTH],
					32, 112 - size, PROCNE_WHIRLWIND_WIDTH, size);
			}
			if(frm >= 48 && frm < PROCNE_ARRIVAL_TICK) {
				size = frm * 2 - 96;
				if(size > PROCNE_WHIRLWIND_HEIGHT)
					size = PROCNE_WHIRLWIND_HEIGHT;
				draw[0](canvas,
					&state->sheet[MODULO_THREE(frm / 4) *
							PROCNE_WHIRLWIND_PICTURE +
						(PROCNE_WHIRLWIND_HEIGHT - size) *
							PROCNE_WHIRLWIND_WIDTH],
					32, 64 - size, PROCNE_WHIRLWIND_WIDTH, size);
			}
			if(frm >= PROCNE_ARRIVAL_TICK && frm < PROCNE_TICKS) {
				draw[0](canvas,
					&state->sheet[MODULO_THREE(frm / 4) *
						PROCNE_WHIRLWIND_PICTURE],
					8, 0, PROCNE_WHIRLWIND_WIDTH, PROCNE_WHIRLWIND_HEIGHT);
				draw[0](canvas,
					&state->sheet[MODULO_THREE(frm / 4) *
						PROCNE_WHIRLWIND_PICTURE],
					8, 64, PROCNE_WHIRLWIND_WIDTH, PROCNE_WHIRLWIND_HEIGHT);
			}

			/* The dust spirals in and up. */
			if(frm < PROCNE_ARRIVAL_TICK) {
				point = state->particles;
				for(idx = 0; idx != PROCNE_DUST_MOTES; idx++) {
					if(point[idx].variant == 0) {
						world[0] = point[idx].z * sySin(point[idx].velY);
						world[1] = point[idx].y;
						world[2] = point[idx].z * syCos(point[idx].velY);
						btEffectPosProjectPoint(world, &screen);
						screen.x >>= 1;
						/* DEAD: always true, as a mote restarts once past 48
						   pixels up and climbs little more than 2 pixels a
						   tick; the ROM keeps the test. */
						if(point[idx].y < toFIXED(64))
							draw[0](canvas, state->sheet + PROCNE_DUST,
								screen.x - (PROCNE_DUST_SIZE / 2 + 4),
								screen.y - (PROCNE_DUST_SIZE / 2 + 4),
								PROCNE_DUST_SIZE, PROCNE_DUST_SIZE);
						if(point[idx].z > 24)
							point[idx].z -= 4;
						point[idx].velY += point[idx].velX * 2;
						if(point[idx].velY > TRIG_TURN)
							point[idx].velY -= TRIG_TURN;
						point[idx].velX += PROCNE_DUST_SPIN_GAIN;
						if(point[idx].velX > PROCNE_DUST_SPIN_MAX)
							point[idx].velX = PROCNE_DUST_SPIN_MAX;
						point[idx].velY += TRIG_TURN / 32;
						point[idx].y += point[idx].velY * 2;
						if(point[idx].y > toFIXED(48)) {
							point[idx].y = 0;
							point[idx].z = 100;
							point[idx].velY = 0;
							point[idx].velX = 0;
						}
					} else {
						point[idx].variant--;
					}
				}
			}

			if(frm == PROCNE_ARRIVAL_TICK) {
				Sint32 speed;
				Sint32 ang;

				btBackgroundLoadStub(
					BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
				btBackgroundSetModeStub(
					BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 8);
				btEffectShowLivingTargets(state->args);
				btEffectCreateSprs(PROCNE_SPRITES, SPRITE_PROCNE, 2);
				btResourceLoadAndDecompress(RESOURCE_VIOLET_COMET_SHEET,
					state->sheet + PROCNE_COMET_PICTURES, 1, 1);

				for(idx = 0; idx != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
					idx++) {
					PARTICLES[idx].variant = 0;
				}

				EFFECT_FOR_EACH_TARGET(idx, args) {
					battleObject = btObjectGetStub(args->tgts[idx]);
					tgt = battleObject->obj;
					tgt->y = INTtoFIXED((syRandom() & 15) + 8);
				}

				ProjectionWork.centerX = 72;

				for(idx = 0; idx != PROCNE_GATHERING_SPARKS; idx++) {
					speed = syRandom() & 127;
					ang = (syRandom() & 0x7fff) + DEGtoANG(225) - 1;
					state->particles[idx].x = sySin(ang) * speed;
					state->particles[idx].y = syCos(ang) * speed;
					state->particles[idx].z = toFIXED(syRandom() % 200 - 100);
					state->particles[idx].variant = 0;
				}

				EFFECT_FOR_EACH_TARGET(idx, args) {
					radius[idx] = PROCNE_CORE_RADIUS;
					spin[idx] = PROCNE_SKY_SPIN;
				}
			}

			/* Procne sways, pushed by each comet. */
			if(frm >= PROCNE_ARRIVAL_TICK) {
				scale = ProcneUnitScale;
				size = PROCNE_BOB;
				if(frm - PROCNE_ARRIVAL_TICK <= 64)
					size = PROCNE_BOB * 2;
				wave.x -=
					(sySin((frm - PROCNE_ARRIVAL_TICK) << 7) * PROCNE_SWAY) >>
					6;
				wave.y += (sySin((frm - PROCNE_ARRIVAL_TICK) << 9) * size) >> 6;
				wave.x += drift.x;
				wave.y += drift.y;
				drift.x /= 2;
				drift.y /= 2;

				scale.x = toFIXED(1);
				scale.y = toFIXED(1);
				place.groundHeight = 0;
				/* Sprite i stands lower and higher by the same amount, which
				   cancel on the screen but give it a later draw slot. */
				for(idx = 0; idx != PROCNE_SPRITES; idx++) {
					place.x = toFIXED(ProcneGridX[idx]) + wave.x + toFIXED(40);
					place.height = toFIXED(255) + idx * toFIXED(64);
					place.y = toFIXED(ProcneGridY[idx] + idx * 64) + wave.y;
					fdRenderApplyProjectedPlacementStub(
						(SPR *)state->sprs[idx], &place.x, &scale.x, 0);
				}

				if(frm > 255) {
					Sint32 launch;

					depthOfs[2] = toFIXED(256);
					arrived = 0;
					depthOfs[0] = 0;
					depthOfs[1] = 0;
					syInitMatrix();
					syMatrixTranslateVector(depthOfs);

					/* Until the first comet, the sparks gather on Procne, each
					   a part of its distance a tick; those that have arrived
					   make one spark that grows with their number. */
					if(frm < PROCNE_COMET_TICK) {
						for(idx = 0; idx != PROCNE_GATHERING_SPARKS; idx++) {
							Sint32 xx;
							Sint32 yy;
							Sint32 zz;

							point = &state->particles[idx];
							xx = (point->x >> 8) * (point->x >> 8);
							yy = (point->y >> 8) * (point->y >> 8);
							zz = (point->z >> 8) * (point->z >> 8);
							dist = sySquartIWRAM(xx + yy + zz) >> 9;
							if(dist != 0) {
								btEffectPosProjectPoint(&point->x, &screen);
								/* The canvas is shown at twice its width. */
								screen.x = (FIXEDtoINT(screen.x) >> 1) +
									(FIXEDtoINT(wave.x) >> 1) + 32;
								screen.y = FIXEDtoINT(screen.y) +
									FIXEDtoINT(wave.y) - 4;
								screen.depth = FIXEDtoINT(screen.depth);
								CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
									PROCNE_DEPTH_FAR);
								moteSize = PROCNE_SPARK_SIZE(screen.depth, 6);
								EFFECT_DRAW_MOTE(draw[1], canvas, graphics,
									screen.x, screen.y, moteSize);
								point->x -= point->x / dist;
								point->y -= point->y / dist;
								point->z -= point->z / dist;
							} else {
								arrived++;
							}
						}

						if(arrived > 0) {
							moteSize = arrived / 10 + 1;
							draw[1](canvas,
								graphics +
									BattleEffectMotePictures[moteSize - 1],
								(FIXEDtoINT(wave.x) >> 1) - moteSize / 2 + 32,
								FIXEDtoINT(wave.y) - moteSize - 4, moteSize,
								moteSize * 2);
						}
					}

					/* Each strike throws out sparks. */
					for(slot = 0; slot != PROCNE_STRIKES; slot++) {
						if(frm == ProcneTicks[slot]) {
							Sint32 speed;
							Sint32 ang;

							for(idx = 0; idx != PROCNE_STRIKE_SPARKS; idx++) {
								speed = (syRandom() & 127) + 16;
								ang = syRandom() & TRIG_ANGLE_MASK;
								spark =
									&PARTICLES[slot * PROCNE_STRIKE_SPARK_ROWS +
										idx];
								spark->velX = (sySin(ang) * speed) >> 6;
								spark->velY = -(syCos(ang) * speed) >> 6;
								/* BUG: the random depth speed is stored over
								   velocityY, not in velocityZ as the other
								   bursts store it, and z moves by velocityY:
								   the up-and-down speed just set is lost, and a
								   spark's depth moves with its height. */
#if FIX_PROCNE_SPARK_DEPTH
								spark->velZ = ((syRandom() & 255) - 128) << 10;
#else
								spark->velY = ((syRandom() & 255) - 128) << 10;
#endif
								SET_POSITION(spark, 0, 0, 0);
								spark->variant = (syRandom() & 15) + 64;
							}
						}

						/* Inside the strikes' loop, so the sparks move and fade
						   four steps a tick. */
						if(frm >= ProcneTicks[0]) {
							spark = PARTICLES;
							for(idx = 0; idx !=
								PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
								idx++) {
								if(spark[idx].variant > 0) {
									syCalcPoint(&spark[idx].x, &screen.x);
									screen.x = (FIXEDtoINT(screen.x) >> 1) + 32;
									screen.y = FIXEDtoINT(screen.y) + 56;
									screen.depth = FIXEDtoINT(screen.depth);
									CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
										PROCNE_DEPTH_FAR);
									moteSize =
										PROCNE_SPARK_SIZE(screen.depth, 3);
									EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
										screen.x, screen.y, moteSize);
									spark[idx].x += spark[idx].velX;
									spark[idx].y += spark[idx].velY;
#if FIX_PROCNE_SPARK_DEPTH
									spark[idx].z += spark[idx].velZ;
#else
									spark[idx].z += spark[idx].velY;
#endif
									spark[idx].variant--;
								}
							}
						}
					}

					/* The comets leave Procne one after another; each pushes it
					   back and shakes the screen. */
					for(idx = 0; idx != PROCNE_STRIKES; idx++) {
						comet = &state->particles[idx + PROCNE_COMET_PARTICLES];
						launch =
							idx * PROCNE_COMET_INTERVAL + PROCNE_COMET_TICK;
						if(frm == launch) {
							comet->x = (FIXEDtoINT(wave.x) >> 1) + 32;
							comet->y = FIXEDtoINT(wave.y) - 4;
							/* Where it leaves Procne, kept for the ring of
							   light. */
							comet->velX = comet->x;
							comet->velY = comet->y;

							for(slot = 0; slot != PROCNE_RING_MOTES; slot++) {
								state->particles[slot + PROCNE_RING].variant =
									4;
							}

							drift.x = toFIXED(8);
							drift.y = -toFIXED(2);
							state->shakeFrm = 8;
						}

						if(frm >= launch) {
							Sint32 picture;

							/* The comet grows over its first ticks as it
							   flies. */
							picture = (frm - launch) / 2;
							if(picture > 2)
								picture = 2;
							draw[1](canvas,
								state->sheet + ProcnePictures[picture],
								comet->x,
								comet->y - ProcnePictureBiasY[picture],
								ProcnePictureWidths[picture],
								ProcnePictureHeights[picture]);
							comet->x -= 8;
							comet->y += 2;

							if(frm < launch + 8) {
								/* A ring of light grows where the comet left
								   Procne. */
								syRotZ(-DEGtoANG(11.25));
								syRotY(-DEGtoANG(22.5));

								spark = &state->particles[PROCNE_RING];
								for(slot = 0; slot != PROCNE_RING_MOTES;
									slot++) {
									PARTICLE *step;

									step = &spark[slot];
									world[0] = 0;
									world[1] = step->variant *
										syCos(slot *
											(TRIG_TURN / PROCNE_RING_MOTES));
									world[2] = step->variant *
										sySin(slot *
											(TRIG_TURN / PROCNE_RING_MOTES));
									step->variant += 2;
									btEffectPosProjectPoint(world, &screen);
									screen.x = (FIXEDtoINT(screen.x) >> 1) +
										state
											->particles[idx +
												PROCNE_COMET_PARTICLES]
											.velX;
									screen.y = FIXEDtoINT(screen.y) +
										state
											->particles[idx +
												PROCNE_COMET_PARTICLES]
											.velY;
									screen.depth = FIXEDtoINT(screen.depth);
									CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
										PROCNE_DEPTH_FAR);
									moteSize =
										PROCNE_SPARK_SIZE(screen.depth, 3);
									EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
										screen.x, screen.y, moteSize);
								}
							}
						}
					}
				}
			}

			EFFECT_END_TICK(state);
		}

		EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		EFFECT_FOR_EACH_TARGET(idx, args) {
			MOTION_OBJECT *battler;

			battleObject = btObjectGetStub(args->tgts[idx]);
			battler = battleObject->obj;
			battler->x = home[idx].x;
			battler->z = home[idx].y;
			battler->ang = baseAng[idx];
		}
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		btEffectSetupBlendedDisp();

		for(idx = 0; idx != PROCNE_SPRITES; idx++) {
			fdSprDestroyStub(state->sprs[idx]);
		}

		EFFECT_FINISH_CANVAS();
	}
}

#undef MODULO_THREE
#undef PROCNE_TICKS
#undef PROCNE_ARRIVAL_TICK
#undef PROCNE_DROP_TICK
#undef PROCNE_COMET_TICK
#undef PROCNE_COMET_INTERVAL
#undef PROCNE_SPRITES
#undef PROCNE_WHIRLWIND_WIDTH
#undef PROCNE_WHIRLWIND_HEIGHT
#undef PROCNE_WHIRLWIND_PICTURE
#undef PROCNE_WHIRLWIND_FOOT_HEIGHT
#undef PROCNE_DUST
#undef PROCNE_DUST_SIZE
#undef PROCNE_COMET_PICTURES
#undef PROCNE_DUST_MOTES
#undef PROCNE_DUST_SPIN_GAIN
#undef PROCNE_DUST_SPIN_MAX
#undef PROCNE_CORE_RADIUS
#undef PROCNE_SPIN_GAIN
#undef PROCNE_SKY_SPIN
#undef PROCNE_SWAY
#undef PROCNE_BOB
#undef PROCNE_GATHERING_SPARKS
#undef PROCNE_RING
#undef PROCNE_RING_MOTES
#undef PROCNE_COMET_PARTICLES
#undef PROCNE_STRIKES
#undef PROCNE_STRIKE_SPARKS
#undef PROCNE_STRIKE_SPARK_ROWS
#undef PROCNE_DEPTH_NEAR
#undef PROCNE_DEPTH_FAR
#undef PROCNE_SPARK_SIZE

/* Fireballs rain on the targets and a blast of fire sweeps across them, in the
   LightningBolt colours.

   Abilities: Dragon Driver. */
void btEffectRunDragonDriver(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}

/* Tiamat lands while fireballs rain on the targets, breathes fire across them,
   and bursts away.

   Abilities: Tiamat. */
void btEffectRunTiamat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_TIAMAT);
}

#define DRAGON_FIRE_TICKS		   208
/* The breath of fire, then the flash that ends it. */
#define DRAGON_FIRE_BREATH_TICK	   128
#define DRAGON_FIRE_FLASH_TICK	   160
#define DRAGON_FIRE_FLASH_END_TICK 176
#define DRAGON_FIRE_TIAMAT_SPRITES 8   /* the sprites that draw Tiamat */
#define DRAGON_FIRE_FIREBALLS	   16  /* falling fireballs */
#define DRAGON_FIRE_FIRST_FIREBALL 24  /* the first in state->particles */
#define DRAGON_FIRE_FIRST_SPARK	   40  /* the first spark in state->particles */
#define DRAGON_FIRE_EMBERS		   512 /* embers in PARTICLES */
#define DRAGON_FIRE_BREATH		   24 /* puffs of breath, state->particles 0 to 23 */
#define DRAGON_FIRE_SPARKS		   24 /* sparks, state->particles 40 to 63 */
/* The flames a fireball burns with where it lands, in the sprite sheet. */
#define DRAGON_FIRE_FLAME_PICTURES 12288
/* A puff of breath shows the flash burst's six pictures, four ticks each. */
#define DRAGON_FIRE_PUFF_PICTURES  6
#define DRAGON_FIRE_PUFF_TICKS	   4
/* A landed fireball's flame burns through its six pictures, two ticks each,
   over and over. */
#define DRAGON_FIRE_BURN_PICTURES  6
#define DRAGON_FIRE_BURN_TICKS	   2
#define DRAGON_FIRE_SPARK_LIFE	   48  /* ticks a spark flies */
#define DRAGON_FIRE_EMBER_LIFE	   256 /* ticks an ember rises */
/* The level the canvas is cleared to each tick of the flash, until its last
   two ticks turn it CANVAS_LEVEL_BRIGHTEST. */
#define DRAGON_FIRE_FLASH_LEVEL	   16
/* A spark's heading is a random one from DRAGON_FIRE_HEADING, about 110
   degrees, to an eighth of a turn past it; a puff of breath's from about 110
   degrees the other way. */
#define DRAGON_FIRE_HEADING		   20000

/* Tiamat's pieces (state->sprs), each showing its own picture of the sprite
   sheet: piece n shows animation n. As it breathes, its head, horns, jaw and
   throat show the fire-lit pictures of its open mouth. */
typedef enum TIAMAT_PIECE {
	TIAMAT_HEAD = 0,
	TIAMAT_HORNS = 1,
	TIAMAT_JAW = 3,
	TIAMAT_THROAT = 4,
} TIAMAT_PIECE;

/* The animations of Tiamat's pieces as it breathes fire, the fire-lit pictures
   of its open mouth. */
typedef enum TIAMAT_ANIMATION {
	TIAMAT_HEAD_BREATHING = 8,
	TIAMAT_HORNS_BREATHING,
	TIAMAT_JAW_BREATHING,
	TIAMAT_THROAT_BREATHING,
} TIAMAT_ANIMATION;

/* Fireballs rain on the targets and burn where they land while embers rise from
   the ground; then a blast of fire sweeps across them and the screen flares
   white. Against a two-part enemy Tiamat comes at its own size and stands 8
   pixels further right and lower, with its sparks and breath. */
void btEffectRunDragonFire(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 origin;
	BLITTER_PAIR		 draw;
	void				*canvas;
	BATTLE_CAMERA		*camera;
	BATTLE_EFFECT_STATE *state;
	Sint32				 frm;
	Uint8				*sheet;
	Sint32				 i;

	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	state = RamHeapSlots->battleEff;
	sheet = RamHeapSlots->battleSheet;

	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	btEffectBuildCanvasBlitters(0, draw);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
		state->sheet + DRAGON_FIRE_FLAME_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	if(mode == DRAGON_FIRE_DRAGON_DRIVER) {
		LOAD_BG_PALETTE(RESOURCE_LIGHTNING_BOLT_SHEET);
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	{
		POINT2_D focus;
		Sint32	 dir;
		Sint32	 twoPartTgt;

		twoPartTgt = 0;
		if(btObjectGetStub(args->tgts[0])->obj->recordStorageKind ==
			ACTOR_SPRITE_GROUP)
			twoPartTgt = 1;

		if(mode == DRAGON_FIRE_TIAMAT)
			dir = 1;
		else if(args->side == EFFECT_SIDE_PARTY)
			dir = -1;
		else
			dir = 1;
		if(mode == DRAGON_FIRE_DRAGON_DRIVER) {
			btEffectPosProjectScaledBattler(args->user, &origin);
			/* DEAD: the halved x is replaced below; the ROM keeps the halving
			   and its store. */
			origin.x /= 2;
			origin.y = 66;
			if(args->side == EFFECT_SIDE_PARTY)
				origin.x = 76;
			else
				origin.x = 44;
		}

		if(twoPartTgt == 1) {
			focus.x = toFIXED(220);
			focus.y = -toFIXED(52);
		} else {
			focus.x = toFIXED(212);
			focus.y = -toFIXED(60);
		}
		EFFECT_FREE_PARTICLES(i, state->particles, 64);
		for(i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
			PARTICLE *fireball =
				&state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

			if(dir == 1)
				fireball->x = (syRandom() & 127) + 128;
			else
				fireball->x = (syRandom() & 127) - 128;
			fireball->y = (syRandom() & 7) - 72;
			fireball->variant = -(syRandom() & 31);
		}
		EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
		if(mode == DRAGON_FIRE_TIAMAT) {
			btEffectShowLivingTargets(state->args);
			sySynch(1);
			btEffectCreateSprs(DRAGON_FIRE_TIAMAT_SPRITES, SPRITE_TIAMAT, 2);
		}

		for(frm = 0; frm != DRAGON_FIRE_TICKS; frm++) {
			/* A or B skips to the flash. */
			if((PerWork.repeat & (KEY_A | KEY_B)) && frm > 48 &&
				frm < DRAGON_FIRE_FLASH_TICK) {
				/* What the breath would have done: Tiamat opens its jaws and
				   the targets reel. */
				if(mode == DRAGON_FIRE_TIAMAT) {
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HEAD], TIAMAT_HEAD_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HORNS], TIAMAT_HORNS_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_JAW], TIAMAT_JAW_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_THROAT], TIAMAT_THROAT_BREATHING);
				}
				EFFECT_FOR_EACH_TARGET(i, args) {
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_RED_TINT,
						POSE_DAMAGE, TINT_KEEP, 0);
					btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_HURL);
				}
				frm = DRAGON_FIRE_FLASH_TICK;
			}
			EFFECT_VIEW_FROM_CAMERA(camera);
			if(frm == 178)
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

			if(frm == DRAGON_FIRE_BREATH_TICK)
				state->transferValue = CANVAS_FADE_HALF;
			if(frm == DRAGON_FIRE_FLASH_END_TICK) {
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
				syWordsCopyEntry((void *)BG_PLTT,
					syResourceGetTableEntry(RESOURCE_EMBER_STREAK_SHEET),
					SHEET_PALETTE_BYTES);
			} else if(frm >= DRAGON_FIRE_FLASH_TICK &&
				frm < DRAGON_FIRE_FLASH_END_TICK) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
					CANVAS_FILL_WORD(DRAGON_FIRE_FLASH_LEVEL));
				if(frm > 173)
					state->transferValue =
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
				btPaletteBrightenBgEntries(2, 2, 2);
			}

			/* Embers rise from the ground. */
			if(frm >= 33 && frm < DRAGON_FIRE_FLASH_END_TICK) {
				Sint32 spawned = 0;
				Sint32 limit = 1;

				if(frm > 103)
					limit = 8;
				for(i = 0; i != DRAGON_FIRE_EMBERS; i++) {
					PARTICLE *ember = &PARTICLES[i];

					if(ember->variant == -1) {
						ember->x = toFIXED((syRandom() & 255) - 32);
						ember->y = toFIXED(112);
						ember->velX =
							((syRandom() & 127) + TiamatEmberSpeeds[i & 3])
							<< 9;
						ember->velY =
							-((syRandom() & 127) + TiamatEmberSpeeds[i & 3])
							<< 11;
						ember->variant = 0;
						spawned++;
						if(spawned == limit)
							break;
					}
				}
			}

			/* Sparks fly up from Tiamat's head. */
			if(frm >= 41 && frm < DRAGON_FIRE_BREATH_TICK) {
				Sint32 spawned = 0;

				if(frm & 1) {
					for(i = 0; i != DRAGON_FIRE_SPARKS; i++) {
						PARTICLE *spark =
							&state->particles[DRAGON_FIRE_FIRST_SPARK + i];

						if(spark->variant == -1) {
							Sint32 speed = (syRandom() & 255) + 128;
							Sint32 ang =
								(syRandom() & 0x1fff) + DRAGON_FIRE_HEADING;

							if(mode == DRAGON_FIRE_TIAMAT) {
								if(twoPartTgt == 1) {
									spark->x =
										INTtoFIXED((syRandom() & 7) + 78);
									spark->y = toFIXED(78);
								} else {
									spark->x =
										INTtoFIXED((syRandom() & 7) + 78);
									spark->y = toFIXED(70);
								}
							} else {
								spark->x =
									toFIXED((syRandom() & 7) + origin.x - 8);
								spark->y = toFIXED(origin.y);
							}
							spark->velX = speed * sySin(ang) >> 9;
							spark->velY = speed * syCos(ang) >> 9;
							spark->variant = 0;
							spawned++;
							if(spawned == 1)
								break;
						}
					}
				}
			}
			if(frm == 48)
				sySndPlayStub(SE_RUMBLE);
			if(frm == DRAGON_FIRE_BREATH_TICK)
				sySndPlayStub(SE_CRASH);

			/* Breath: puffs of fire thrown across the targets. */
			if(frm > DRAGON_FIRE_BREATH_TICK &&
				frm < DRAGON_FIRE_FLASH_END_TICK) {
				Sint32 spawned = 0;

				for(i = 0; i != DRAGON_FIRE_BREATH; i++) {
					PARTICLE *puff = &state->particles[i];

					if(puff->variant == -1) {
						Sint32 speed = (syRandom() & 255) + 128;
						Sint32 ang =
							(syRandom() & 0x1fff) - DRAGON_FIRE_HEADING;

						if(mode == DRAGON_FIRE_TIAMAT) {
							if(twoPartTgt == 1) {
								puff->x = toFIXED(72);
								puff->y = toFIXED(72);
							} else {
								puff->x = toFIXED(68);
								puff->y = toFIXED(64);
							}
						} else {
							puff->x = toFIXED(origin.x);
							puff->y = toFIXED(origin.y);
						}
						puff->velX = speed * sySin(ang) >> 6;
						puff->velY = speed * syCos(ang) >> 6;
						puff->variant = 0;
						spawned++;
						if(spawned == 1)
							break;
					}
				}
			}

			if(frm < DRAGON_FIRE_FLASH_END_TICK) {
				for(i = 0; i != DRAGON_FIRE_BREATH; i++) {
					PARTICLE *puff = &state->particles[i];

					if(puff->variant >= 0) {
						draw[0](canvas,
							state->sheet +
								(puff->variant >> 2) *
									PICTURE_BYTES(FLASH_BURST),
							FIXED_INTEGER_PART(puff->x) - FLASH_BURST_WIDTH / 2,
							FIXED_INTEGER_PART(puff->y) -
								FLASH_BURST_HEIGHT / 2,
							FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
						puff->x += puff->velX * dir;
						puff->y += puff->velY;
						puff->variant++;
						if(puff->variant ==
							DRAGON_FIRE_PUFF_PICTURES * DRAGON_FIRE_PUFF_TICKS)
							puff->variant = -1;
					}
				}
			}

			for(i = 0; i != DRAGON_FIRE_SPARKS; i++) {
				PARTICLE *spark =
					&state->particles[DRAGON_FIRE_FIRST_SPARK + i];
				Sint32 size = 1;

				if(spark->variant >= 0) {
					draw[1](canvas, TiamatSparkPixels,
						FIXED_INTEGER_PART(spark->x),
						FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
					spark->x += spark->velX * dir;
					spark->y += spark->velY;
					spark->velY += -toFIXED(1) / 64;
					spark->variant++;
					if(spark->variant == DRAGON_FIRE_SPARK_LIFE)
						spark->variant = -1;
				}
			}

			/* Embers rise, then curve away. */
			if(frm < DRAGON_FIRE_FLASH_END_TICK) {
				for(i = 0; i != DRAGON_FIRE_EMBERS; i++) {
					PARTICLE *ember = &PARTICLES[i];

					if(ember->variant >= 0) {
						Uint8 size = TiamatEmberSizes[i & 3];

						draw[0](canvas,
							sheet + BattleEffectMotePictures[size - 1],
							FIXED_INTEGER_PART(ember->x) - size / 2,
							FIXED_INTEGER_PART(ember->y) - size, size,
							size * 2);
						ember->x += ember->velX * dir;
						ember->y += ember->velY;
						if(frm > DRAGON_FIRE_BREATH_TICK) {
							if(i & 1)
								ember->velX += -toFIXED(1) / 2;
							else
								ember->velX += -toFIXED(1) / 8;
						} else {
							ember->velY += -toFIXED(1) / 64;
						}
						ember->variant++;
						if(ember->variant == DRAGON_FIRE_EMBER_LIFE)
							ember->variant = -1;
					}
				}
			}

			if(frm == DRAGON_FIRE_BREATH_TICK)
				state->shakeFrm = 48;
			if(mode == DRAGON_FIRE_TIAMAT && frm == 48)
				state->shakeFrm = 8;

			/* Tiamat swoops down. */
			if(frm >= 40 && frm <= 47) {
				focus.x += -toFIXED(8);
				focus.y += toFIXED(16);
			}
			if(mode == DRAGON_FIRE_TIAMAT) {
				if(frm == DRAGON_FIRE_BREATH_TICK) {
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HEAD], TIAMAT_HEAD_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HORNS], TIAMAT_HORNS_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_JAW], TIAMAT_JAW_BREATHING);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_THROAT], TIAMAT_THROAT_BREATHING);
				}
				if(frm == DRAGON_FIRE_FLASH_END_TICK) {
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HEAD], TIAMAT_HEAD);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_HORNS], TIAMAT_HORNS);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_JAW], TIAMAT_JAW);
					fdSprStartAnimationStub(
						state->sprs[TIAMAT_THROAT], TIAMAT_THROAT);
				}
				if(twoPartTgt == 1)
					btEffectPlaceFormationSprs(
						SUMMON_FORMATION_TIAMAT_ALTERNATE, focus.x, focus.y);
				else
					btEffectPlaceFormationSprs(
						SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
			}
			if(frm == 138) {
				EFFECT_FOR_EACH_TARGET(i, args) {
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_RED_TINT,
						POSE_DAMAGE, TINT_KEEP, 0);
					btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_HURL);
				}
			}

			if(frm < DRAGON_FIRE_FLASH_END_TICK) {
				for(i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
					PARTICLE *fireball =
						&state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

					if(fireball->y > 55) {
						if(fireball->variant >= 0 &&
							fireball->variant < DRAGON_FIRE_BURN_PICTURES *
									DRAGON_FIRE_BURN_TICKS) {
							Sint32 picture =
								fireball->variant / DRAGON_FIRE_BURN_TICKS;

							draw[0](canvas,
								state->sheet +
									TiamatEmberStreakPictures[picture] +
									DRAGON_FIRE_FLAME_PICTURES,
								fireball->x -
									TiamatEmberStreakWidths[picture] / 2,
								fireball->y + TiamatEmberStreakBiasY[picture],
								TiamatEmberStreakWidths[picture],
								TiamatEmberStreakHeights[picture]);
						}
						fireball->variant++;
						if(fireball->variant ==
							DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_TICKS)
							fireball->variant = 0;
					} else if(fireball->variant == 0) {
						Sint32 size = MOTE_PICTURES;

						fireball->x -= dir * 6;
						fireball->y += 6;
						if(frm <= 47 && fireball->y > 55)
							sySndPlayStub(SE_WHOOSH);
						draw[0](canvas,
							sheet + BattleEffectMotePictures[size - 1],
							fireball->x - size / 2, fireball->y + 30, size,
							size * 2);
					} else {
						fireball->variant++;
					}
				}
			}
			btCameraShake(8, 8);
			EFFECT_END_TICK(state);
		}

		syCloseEvent(btEffectTransferCanvas);
		EFFECT_RELEASE_BLITTERS();
		if(mode == DRAGON_FIRE_TIAMAT) {
			if(twoPartTgt == 1)
				btEffectRunImpactBurst(
					SUMMON_FORMATION_TIAMAT_ALTERNATE, focus.x, focus.y);
			else
				btEffectRunImpactBurst(
					SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
		}
		if(mode == DRAGON_FIRE_TIAMAT) {
			for(i = 0; i != DRAGON_FIRE_TIAMAT_SPRITES; i++)
				fdSprDestroyStub(state->sprs[i]);
		}
	}
	btEffectEndCanvasLayer();
}

#undef DRAGON_FIRE_TICKS
#undef DRAGON_FIRE_BREATH_TICK
#undef DRAGON_FIRE_FLASH_TICK
#undef DRAGON_FIRE_FLASH_END_TICK
#undef DRAGON_FIRE_TIAMAT_SPRITES
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_PICTURES
#undef DRAGON_FIRE_PUFF_PICTURES
#undef DRAGON_FIRE_PUFF_TICKS
#undef DRAGON_FIRE_BURN_PICTURES
#undef DRAGON_FIRE_BURN_TICKS
#undef DRAGON_FIRE_SPARK_LIFE
#undef DRAGON_FIRE_EMBER_LIFE
#undef DRAGON_FIRE_FLASH_LEVEL
#undef DRAGON_FIRE_HEADING
