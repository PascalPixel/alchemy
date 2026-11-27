/* The Procne summon. Part of battle_effect.c. */

#define PROCNE_TICKS		  400 /* ticks of Procne */
/* Procne appears in the violet sky, and the targets it lifted drop. */
#define PROCNE_ARRIVAL_TICK	  160
#define PROCNE_DROP_TICK	  395
/* The first comet leaves Procne, and the next each 12 ticks after. */
#define PROCNE_COMET_TICK	  330
#define PROCNE_COMET_INTERVAL 12

#define PROCNE_SPRITES 9 /* Procne's sprites, on ProcneGridX, ProcneGridY */

/* The whirlwind sheet: three whirlwind pictures, shown 4 ticks each, then the
   dust. */
#define PROCNE_WHIRLWIND_WIDTH	48
#define PROCNE_WHIRLWIND_HEIGHT 64
#define PROCNE_WHIRLWIND_PICTURE \
	(PROCNE_WHIRLWIND_WIDTH * PROCNE_WHIRLWIND_HEIGHT)
/* The whirlwind rises in two pieces: its foot, a picture's upper rows, then a
   whole picture above it. */
#define PROCNE_WHIRLWIND_FOOT_HEIGHT 48
#define PROCNE_DUST					 (3 * PROCNE_WHIRLWIND_PICTURE)
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
		((depth) - PROCNE_DEPTH_NEAR) / \
			((PROCNE_DEPTH_FAR - PROCNE_DEPTH_NEAR) / ((largest) - 1)))

/* A whirlwind lifts the targets and spins them while dust swirls up it; then
   Procne, a great bird in a violet sky, gathers sparks and strikes them with
   four comets, each throwing out a burst of sparks. A or B skips to Procne's
   arrival, then to the drop.

   Abilities: Procne. */
void btEffectRunProcne(BATTLE_EFFECT_PARM *args)
{
	static SPR_SCALE	 unitScale = {{{toFIXED(1), toFIXED(1)}}};
	void			   **heap;
	void				*canvas;
	BATTLE_CAMERA		*camera;
	BATTLE_EFFECT_STATE *state;
	BLITTER_PAIR		 draw;
	Sint32				 frm;
	Uint8				*motes;
	POINT2_D			 procne;
	POINT2_D			 drift;
	ACTOR				*actor;
	BATTLE_OBJECT		*battleObject;
	PARTICLE			*point;
	PARTICLE			*spark;
	PARTICLE			*comet;
	Uint8				*resource;
	Uint8				*image;
	Sint32				 idx;
	Sint32				 slot;
	Sint32				 size;
	Sint32				 moteSize;
	Sint32				 dist;
	Sint32				 arrived;
	Sint32				 width = PROCNE_WHIRLWIND_WIDTH;
	Sint32				 footHeight = PROCNE_WHIRLWIND_FOOT_HEIGHT;
	Sint32				 heading[8];
	Sint32				 radius[8];
	Sint32				 spin[8];
	POINT2_D			 home[8];
	Sint32				 homeFacing[8];
	VECTOR3				 world;
	SCREEN_POINT		 screen;
	VECTOR3				 depthOfs;
	SPR_PLACEMENT		 place;
	SPR_SCALE			 scale;

	heap = &HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	canvas = heap[0];
	camera = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_CAMERA);
	state =
		HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
	motes =
		HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
	state->args = args;

	btEffectStartCanvasLayer(0);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = (DRAW_RECTANGLE)HEAP_SLOT_FROM(
		heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[1] = (DRAW_RECTANGLE)HEAP_SLOT_FROM(
		heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_RAM_CODE_PAIRED);

	resource = syResourceGetTableEntry(RESOURCE_WHIRLWIND_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	procne.x = toFIXED(128);
	procne.y = toFIXED(40);
	drift.x = 0;
	drift.y = 0;

	/* Each target's place, for the whirlwind to turn it. */
	EFFECT_FOR_EACH_TARGET(idx, state->args) {
		battleObject = btObjectGetStub(state->args->tgts[idx]);
		actor = battleObject->actor;
		home[idx].x = actor->x.fixed;
		home[idx].y = actor->z.fixed;
		homeFacing[idx] = actor->facing;
		heading[idx] = (Uint16)syAtan(actor->x.fixed, actor->z.fixed);
		radius[idx] =
			sySquartIWRAM((actor->x.fixed >> 8) * (actor->x.fixed >> 8) +
				(actor->z.fixed >> 8) * (actor->z.fixed >> 8)) >>
			7;
		spin[idx] = 0;
		actor->gravity = 0;
	}

	for(idx = 0; idx != ARRAY_COUNT(state->particles); idx++) {
		point = &state->particles[idx];
		point->z = 120;
		point->y = 0;
		syRandom(); /* drawn and thrown away */
		point->velY = 0;
		point->velX = 0;
		point->variant = syRandom() & 63;
	}
	for(frm = 0; frm != PROCNE_TICKS; frm++) {
		if((PerRepeat & (KEY_A | KEY_B)) != 0) {
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
		EFFECT_FOR_EACH_TARGET(idx, state->args) {
			if(frm > idx * 16) {
				battleObject = btObjectGetStub(state->args->tgts[idx]);
				actor = battleObject->actor;
				actor->x.fixed = (radius[idx] * sySin(heading[idx])) >> 1;
				actor->z.fixed = (radius[idx] * syCos(heading[idx])) >> 1;

				if(frm < PROCNE_ARRIVAL_TICK) {
					if(frm > idx * 16 + 16)
						spin[idx] += PROCNE_SPIN_GAIN;
					if(radius[idx] <= 31)
						actor->y.fixed += toFIXED(6);
					else
						actor->y.fixed += toFIXED(1) / 2;
					if(actor->y.fixed > toFIXED(124))
						actor->y.fixed = toFIXED(124);
				}

				/* DEAD: always true, as the effect ends at tick 399; the ROM
				   keeps the test. */
				if(frm <= 490) {
					heading[idx] += spin[idx];
					if(heading[idx] > TRIG_TURN)
						heading[idx] -= TRIG_TURN;
					actor->facing += spin[idx] / 4;
				}

				if(frm == PROCNE_DROP_TICK)
					actor->gravity = BATTLE_UNIT_GRAVITY;

				for(slot = 0; slot != PROCNE_STRIKES; slot++) {
					if(frm == ProcneTicks[slot]) {
						actor->velY = 0;
						btEffectTintBattler(state->args->tgts[idx],
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
			if(size > footHeight)
				size = footHeight;
			draw[0](canvas,
				&state->sheet[frm / 4 % 3 * PROCNE_WHIRLWIND_PICTURE +
					(footHeight - size) * PROCNE_WHIRLWIND_WIDTH],
				32, 112 - size, width, size);
		}

		if(frm >= 48 && frm < PROCNE_ARRIVAL_TICK) {
			size = frm * 2 - 96;
			if(size > PROCNE_WHIRLWIND_HEIGHT)
				size = PROCNE_WHIRLWIND_HEIGHT;
			draw[0](canvas,
				&state->sheet[frm / 4 % 3 * PROCNE_WHIRLWIND_PICTURE +
					(PROCNE_WHIRLWIND_HEIGHT - size) * PROCNE_WHIRLWIND_WIDTH],
				32, 64 - size, width, size);
		}

		if(frm >= PROCNE_ARRIVAL_TICK && frm < PROCNE_TICKS) {
			image = state->sheet + frm / 4 % 3 * PROCNE_WHIRLWIND_PICTURE;
			draw[0](canvas, image, 8, 0, width, PROCNE_WHIRLWIND_HEIGHT);
			draw[0](canvas, image, 8, 64, width, PROCNE_WHIRLWIND_HEIGHT);
		}

		/* The dust spirals in and up. */
		if(frm < PROCNE_ARRIVAL_TICK) {
			point = state->particles;
			for(idx = 0; idx != PROCNE_DUST_MOTES; idx++) {
				if(point[idx].variant == 0) {
					world.x = point[idx].z * sySin(point[idx].velY);
					world.y = point[idx].y;
					world.z = point[idx].z * syCos(point[idx].velY);
					btProjectionPoint(&world.x, &screen);
					screen.x >>= 1;
					/* DEAD: always true, as a mote restarts once past 48 pixels
					   up and climbs little more than 2 pixels a tick; the ROM
					   keeps the test. */
					if(point[idx].y < toFIXED(64)) {
						draw[0](canvas, state->sheet + PROCNE_DUST,
							screen.x - 12, screen.y - 12, 16, 16);
					}
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

			resource = syResourceGetTableEntry(RESOURCE_VIOLET_COMET_SHEET);
			syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
			resource += SHEET_PALETTE_BYTES;
			syDecompressTaggedLZ(
				resource, state->sheet + PROCNE_COMET_PICTURES);

			for(idx = 0; idx != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
				idx++) {
				PARTICLES[idx].variant = 0;
			}

			EFFECT_FOR_EACH_TARGET(idx, state->args) {
				battleObject = btObjectGetStub(state->args->tgts[idx]);
				actor = battleObject->actor;
				actor->y.fixed = INTtoFIXED((syRandom() & 15) + 8);
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

			EFFECT_FOR_EACH_TARGET(idx, state->args) {
				radius[idx] = PROCNE_CORE_RADIUS;
				spin[idx] = PROCNE_SKY_SPIN;
			}
		}

		/* Procne sways, pushed by each comet. */
		if(frm >= PROCNE_ARRIVAL_TICK) {
			scale = unitScale;
			size = PROCNE_BOB;
			if(frm - PROCNE_ARRIVAL_TICK <= 64)
				size = PROCNE_BOB * 2;

			procne.x -=
				(sySin((frm - PROCNE_ARRIVAL_TICK) << 7) * PROCNE_SWAY) >> 6;
			procne.y += (sySin((frm - PROCNE_ARRIVAL_TICK) << 9) * size) >> 6;
			procne.x += drift.x;
			procne.y += drift.y;
			drift.x /= 2;
			drift.y /= 2;

			scale.x = toFIXED(1);
			scale.y = toFIXED(1);
			place.groundHeight = 0;
			/* Sprite i stands lower and higher by the same amount, which cancel
			   on the screen but give it a later draw slot. */
			for(idx = 0; idx != PROCNE_SPRITES; idx++) {
				place.x = toFIXED(ProcneGridX[idx]) + procne.x + toFIXED(40);
				place.height = toFIXED(255) + idx * toFIXED(64);
				place.y = toFIXED(ProcneGridY[idx] + idx * 64) + procne.y;
				fdRenderApplyProjectedPlacementStub(
					state->sprs[idx], &place, &scale, 0);
			}

			if(frm > 255) {
				Sint32 launch;

				depthOfs.z = toFIXED(256);
				arrived = 0;
				depthOfs.x = 0;
				depthOfs.y = 0;
				syInitMatrix();
				syMatrixTranslateVector(syVector3Coordinates(&depthOfs));

				/* Until the first comet, the sparks gather on Procne, each a
				   part of its distance a tick; those that have arrived make
				   one spark that grows with their number. */
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
							btProjectionPoint(&point->x, &screen);
							/* The canvas is shown at twice its width. */
							screen.x = (FIXEDtoINT(screen.x) >> 1) +
								(FIXEDtoINT(procne.x) >> 1) + 32;
							screen.y =
								FIXEDtoINT(screen.y) + FIXEDtoINT(procne.y) - 4;
							screen.depth = FIXEDtoINT(screen.depth);
							CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
								PROCNE_DEPTH_FAR);
							moteSize = PROCNE_SPARK_SIZE(screen.depth, 6);
							EFFECT_DRAW_MOTE(draw[1], canvas, motes, screen.x,
								screen.y, moteSize);
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
							motes + BattleEffectMotePictures[moteSize - 1],
							(FIXEDtoINT(procne.x) >> 1) - moteSize / 2 + 32,
							FIXEDtoINT(procne.y) - moteSize - 4, moteSize,
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
							spark = &PARTICLES[slot * PROCNE_STRIKE_SPARK_ROWS +
								idx];
							spark->velX = (sySin(ang) * speed) >> 6;
							spark->velY = -(syCos(ang) * speed) >> 6;
							/* BUG: the random depth speed is stored over
							   velocityY, not in velocityZ as the other bursts
							   store it, and z moves by velocityY: the
							   up-and-down speed just set is lost, and a spark's
							   depth moves with its height. */
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
						for(idx = 0;
							idx != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
							idx++) {
							if(spark[idx].variant > 0) {
								btProjectionPoint(&spark[idx].x, &screen);
								screen.x = (FIXEDtoINT(screen.x) >> 1) + 32;
								screen.y = FIXEDtoINT(screen.y) + 56;
								screen.depth = FIXEDtoINT(screen.depth);
								CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
									PROCNE_DEPTH_FAR);
								moteSize = PROCNE_SPARK_SIZE(screen.depth, 3);
								EFFECT_DRAW_MOTE(draw[0], canvas, motes,
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
					launch = idx * PROCNE_COMET_INTERVAL + PROCNE_COMET_TICK;
					if(frm == launch) {
						comet->x = (FIXEDtoINT(procne.x) >> 1) + 32;
						comet->y = FIXEDtoINT(procne.y) - 4;
						/* Where it leaves Procne, kept for the ring of
						   light. */
						comet->velX = (FIXEDtoINT(procne.x) >> 1) + 32;
						comet->velY = FIXEDtoINT(procne.y) - 4;

						for(slot = 0; slot != PROCNE_RING_MOTES; slot++) {
							state->particles[slot + PROCNE_RING].variant = 4;
						}

						drift.x = toFIXED(8);
						drift.y = -toFIXED(2);
						state->shakeFrm = 8;
					}

					if(frm >= launch) {
						Sint32 picture;

						/* The comet grows over its first ticks as it flies. */
						picture = (frm - launch) / 2;
						if(picture > 2)
							picture = 2;
						draw[1](canvas, state->sheet + ProcnePictures[picture],
							comet->x, comet->y - ProcnePictureBiasY[picture],
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
							for(slot = 0; slot != PROCNE_RING_MOTES; slot++) {
								PARTICLE *mote;

								mote = &spark[slot];
								world.x = 0;
								world.y = mote->variant *
									syCos(
										slot * (TRIG_TURN / PROCNE_RING_MOTES));
								world.z = mote->variant *
									sySin(
										slot * (TRIG_TURN / PROCNE_RING_MOTES));
								mote->variant += 2;
								btProjectionPoint(&world.x, &screen);
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
								moteSize = PROCNE_SPARK_SIZE(screen.depth, 3);
								EFFECT_DRAW_MOTE(draw[0], canvas, motes,
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

	/* Put the targets back. */
	EFFECT_FOR_EACH_TARGET(idx, state->args) {
		ACTOR *actor;

		battleObject = btObjectGetStub(state->args->tgts[idx]);
		actor = battleObject->actor;
		actor->x.fixed = home[idx].x;
		actor->z.fixed = home[idx].y;
		actor->facing = homeFacing[idx];
	}

	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;

	btEffectSetupBlendedDisp();

	for(idx = 0; idx != PROCNE_SPRITES; idx++) {
		fdSprDestroyStub(state->sprs[idx]);
	}

	EFFECT_FINISH_CANVAS();
}

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
