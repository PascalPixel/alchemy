/* @file */
/* The Procne summon. */
/* Part of battle_effect.c. */

#define PROCNE_FRAMES 400 /* frames of Procne */
/* Procne appears in the violet sky, and the targets it lifted drop. */
#define PROCNE_ARRIVAL_FRAME 160
#define PROCNE_DROP_FRAME	 395
/* The first comet leaves Procne, and the next each 12 frames after. */
#define PROCNE_COMET_FRAME	  330
#define PROCNE_COMET_INTERVAL 12

#define PROCNE_SPRITES 9 /* Procne's sprites, on Procne_GridX and _GridY */

/* The whirlwind sheet: three whirlwind pictures, shown 4 frames each, then the
 */
/* dust. */
#define PROCNE_WHIRLWIND_WIDTH	48
#define PROCNE_WHIRLWIND_HEIGHT 64
#define PROCNE_WHIRLWIND_PICTURE                                               \
	(PROCNE_WHIRLWIND_WIDTH * PROCNE_WHIRLWIND_HEIGHT)
/* The whirlwind rises in two pieces: its foot, a picture's upper rows, then a
 */
/* whole picture above it. */
#define PROCNE_WHIRLWIND_FOOT_HEIGHT 48
#define PROCNE_DUST					 (3 * PROCNE_WHIRLWIND_PICTURE)
#define PROCNE_COMET_PICTURES		 13824

#define PROCNE_DUST_MOTES 24 /* dust motes that swirl up the whirlwind */
/* How much faster a dust mote circles the whirlwind each frame, up to */
/* PROCNE_DUST_SPIN_MAX, in 1/65536 turns a frame. */
#define PROCNE_DUST_SPIN_GAIN 50
#define PROCNE_DUST_SPIN_MAX  (TRIG_TURN / 16)
/* The distance from the whirlwind's centre, in half pixels, that the lifted */
/* targets spiral in to. */
#define PROCNE_CORE_RADIUS 16
/* How much faster the lifted targets circle each frame, in 1/65536 turns a */
/* frame, and how fast they circle once Procne arrives. */
#define PROCNE_SPIN_GAIN 48
#define PROCNE_SKY_SPIN	 6000

/* Procne's sway: sixty-fourths of the wave's sine, as 16.16 pixels a frame, */
/* across and up and down; it bobs twice as far over its first 64 frames. */
#define PROCNE_SWAY 32
#define PROCNE_BOB	16

/* Procne's particles in state->particles: the sparks that gather on it, the */
/* ring of light each comet leaves, then one per comet. */
#define PROCNE_GATHERING_SPARKS 32
#define PROCNE_RING				PROCNE_GATHERING_SPARKS
#define PROCNE_RING_MOTES		28
#define PROCNE_COMET_PARTICLES	(PROCNE_RING + PROCNE_RING_MOTES)

/* The four strikes, at Procne_Frames: each throws out its sparks from its */
/* own rows of PARTICLES. */
#define PROCNE_STRIKES			 4
#define PROCNE_STRIKE_SPARKS	 96
#define PROCNE_STRIKE_SPARK_ROWS 128

/* The depths a spark is drawn between. */
#define PROCNE_DEPTH_NEAR 170
#define PROCNE_DEPTH_FAR  350
/* A spark's mote size at `depth`: `largest` at PROCNE_DEPTH_NEAR, down to 1 */
/* at PROCNE_DEPTH_FAR. */
#define PROCNE_SPARK_SIZE(depth, largest)                                      \
	((largest) -                                                               \
	 ((depth) - PROCNE_DEPTH_NEAR) /                                           \
		 ((PROCNE_DEPTH_FAR - PROCNE_DEPTH_NEAR) / ((largest) - 1)))

/* A whirlwind lifts the targets and spins them while dust swirls up it; then */
/* Procne, a great bird in a violet sky, gathers sparks and strikes them with */
/* four comets, each throwing out a burst of sparks. A or B skips to Procne's */
/* arrival, then to the drop. */
/* */
/* Abilities: Procne. */
void btEffectRunProcne(BATTLE_EFFECT_PARM *args) {
	static SPR_SCALE unitScale = {{{FIXED_ONE, FIXED_ONE}}};
	void **heap;
	void *canvas;
	BATTLE_CAMERA *camera;
	BATTLE_EFFECT_STATE *state;
	BLITTER_PAIR draw;
	s32 frame;
	u8 *motes;
	POINT2_D procne;
	POINT2_D drift;
	ACTOR *actor;
	BATTLE_OBJECT *battleObject;
	PARTICLE *point;
	PARTICLE *spark;
	PARTICLE *comet;
	u8 *resource;
	u8 *image;
	s32 index;
	s32 slot;
	s32 size;
	s32 moteSize;
	s32 dist;
	s32 arrived;
	s32 width = PROCNE_WHIRLWIND_WIDTH;
	s32 footHeight = PROCNE_WHIRLWIND_FOOT_HEIGHT;
	s32 heading[8];
	s32 radius[8];
	s32 spin[8];
	POINT2_D home[8];
	s32 homeFacing[8];
	VECTOR3 world;
	SCR_POINT screen;
	VECTOR3 depthOffset;
	SPR_PLACEMENT place;
	SPR_SCALE scale;

	heap = &HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	canvas = heap[0];
	camera = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_CAMERA);
	state =
		HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
	motes =
		HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
	state->args = args;

	btEffectStartCanvasLayer(0);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	draw[0] = (DRAW_RECTANGLE)HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS,
											 HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[1] = (DRAW_RECTANGLE)HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS,
											 HEAP_SLOT_RAM_CODE_PAIRED);

	resource = syResourceGetTableEntry(RESOURCE_WHIRLWIND_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	fdDecompressTaggedLz(resource, state->sheet);
	fdDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET),
						 motes);

	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	procne.x = toFIXED(128);
	procne.y = toFIXED(40);
	drift.x = 0;
	drift.y = 0;

	/* Each target's place, for the whirlwind to turn it. */
	EFFECT_FOR_EACH_TARGET(index, state->args) {
		battleObject = btObjectGetStub(state->args->tgts[index]);
		actor = battleObject->actor;
		home[index].x = actor->x.fixed;
		home[index].y = actor->z.fixed;
		homeFacing[index] = actor->facing;
		heading[index] = (u16)syAtan(actor->x.fixed, actor->z.fixed);
		radius[index] =
			sySquart((actor->x.fixed >> 8) * (actor->x.fixed >> 8) +
					 (actor->z.fixed >> 8) * (actor->z.fixed >> 8)) >>
			7;
		spin[index] = 0;
		actor->gravity = 0;
	}

	for(index = 0; index != ARRAY_COUNT(state->particles); index++) {
		point = &state->particles[index];
		point->z = 120;
		point->y = 0;
		syRandom(); /* drawn and thrown away */
		point->velY = 0;
		point->velX = 0;
		point->variant = syRandom() & 63;
	}
	for(frame = 0; frame != PROCNE_FRAMES; frame++) {
		if((KeysRepeat & (KEY_A | KEY_B)) != 0) {
			if(frame < PROCNE_ARRIVAL_FRAME)
				frame = PROCNE_ARRIVAL_FRAME;
			else if(frame < PROCNE_DROP_FRAME)
				frame = PROCNE_DROP_FRAME;
		}

		EFFECT_VIEW_FROM_CAMERA(camera);

		if(frame == 16)
			sySndPlayStub(SE_RUMBLE);
		if(frame == 256)
			sySndPlayStub(SE_SPIN);
		if(frame == 334)
			sySndPlayStub(SE_POWER_UP);
		if(frame == 347)
			sySndPlayStub(SE_POWER_UP);
		if(frame == 359)
			sySndPlayStub(SE_POWER_UP);
		if(frame == 372)
			sySndPlayStub(SE_POWER_UP);

		/* The targets, one every 16 frames, spiral in toward the centre and
		 * rise, */
		/* spinning faster; each strike flashes them. */
		EFFECT_FOR_EACH_TARGET(index, state->args) {
			if(frame > index * 16) {
				battleObject = btObjectGetStub(state->args->tgts[index]);
				actor = battleObject->actor;
				actor->x.fixed = (radius[index] * sySin(heading[index])) >> 1;
				actor->z.fixed = (radius[index] * syCos(heading[index])) >> 1;

				if(frame < PROCNE_ARRIVAL_FRAME) {
					if(frame > index * 16 + 16)
						spin[index] += PROCNE_SPIN_GAIN;
					if(radius[index] <= 31)
						actor->y.fixed += toFIXED(6);
					else
						actor->y.fixed += toFIXED(1) / 2;
					if(actor->y.fixed > toFIXED(124))
						actor->y.fixed = toFIXED(124);
				}

				/* DEAD: always true, as the effect ends at frame 399; the ROM
				 * keeps */
				/* the test. */
				if(frame <= 490) {
					heading[index] += spin[index];
					if(heading[index] > TRIG_TURN)
						heading[index] -= TRIG_TURN;
					actor->facing += spin[index] / 4;
				}

				if(frame == PROCNE_DROP_FRAME)
					actor->gravity = BATTLE_UNIT_GRAVITY;

				for(slot = 0; slot != PROCNE_STRIKES; slot++) {
					if(frame == Procne_Frames[slot]) {
						actor->velY = 0;
						btEffectTintBattler(state->args->tgts[index],
											SPRITE_COLOR_WHITE, POSE_DAMAGE,
											index, 8);
					}
				}

				if(radius[index] > PROCNE_CORE_RADIUS)
					radius[index] -= 2;
			}
		}

		/* The whirlwind's foot grows from frame 16, its top from 48; once
		 * Procne */
		/* arrives it stands two pictures high. */
		if(frame >= 16 && frame < PROCNE_ARRIVAL_FRAME) {
			size = frame * 2 - 32;
			if(size > footHeight)
				size = footHeight;
			draw[0](canvas,
					&state->sheet[frame / 4 % 3 * PROCNE_WHIRLWIND_PICTURE +
								  (footHeight - size) * PROCNE_WHIRLWIND_WIDTH],
					32, 112 - size, width, size);
		}

		if(frame >= 48 && frame < PROCNE_ARRIVAL_FRAME) {
			size = frame * 2 - 96;
			if(size > PROCNE_WHIRLWIND_HEIGHT)
				size = PROCNE_WHIRLWIND_HEIGHT;
			draw[0](canvas,
					&state->sheet[frame / 4 % 3 * PROCNE_WHIRLWIND_PICTURE +
								  (PROCNE_WHIRLWIND_HEIGHT - size) *
									  PROCNE_WHIRLWIND_WIDTH],
					32, 64 - size, width, size);
		}

		if(frame >= PROCNE_ARRIVAL_FRAME && frame < PROCNE_FRAMES) {
			image = state->sheet + frame / 4 % 3 * PROCNE_WHIRLWIND_PICTURE;
			draw[0](canvas, image, 8, 0, width, PROCNE_WHIRLWIND_HEIGHT);
			draw[0](canvas, image, 8, 64, width, PROCNE_WHIRLWIND_HEIGHT);
		}

		/* The dust spirals in and up. */
		if(frame < PROCNE_ARRIVAL_FRAME) {
			point = state->particles;
			for(index = 0; index != PROCNE_DUST_MOTES; index++) {
				if(point[index].variant == 0) {
					world.x = point[index].z * sySin(point[index].velY);
					world.y = point[index].y;
					world.z = point[index].z * syCos(point[index].velY);
					btProjectionPoint(&world.x, &screen);
					screen.x >>= 1;
					/* DEAD: always true, as a mote restarts once past 48 pixels
					 * up and */
					/* climbs little more than 2 pixels a frame; the ROM keeps
					 * the test. */
					if(point[index].y < toFIXED(64)) {
						draw[0](canvas, state->sheet + PROCNE_DUST,
								screen.x - 12, screen.y - 12, 16, 16);
					}
					if(point[index].z > 24)
						point[index].z -= 4;
					point[index].velY += point[index].velX * 2;
					if(point[index].velY > TRIG_TURN)
						point[index].velY -= TRIG_TURN;
					point[index].velX += PROCNE_DUST_SPIN_GAIN;
					if(point[index].velX > PROCNE_DUST_SPIN_MAX)
						point[index].velX = PROCNE_DUST_SPIN_MAX;
					point[index].velY += TRIG_TURN / 32;
					point[index].y += point[index].velY * 2;
					if(point[index].y > toFIXED(48)) {
						point[index].y = 0;
						point[index].z = 100;
						point[index].velY = 0;
						point[index].velX = 0;
					}
				}
				else {
					point[index].variant--;
				}
			}
		}

		if(frame == PROCNE_ARRIVAL_FRAME) {
			s32 speed;
			s32 angle;

			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
								 RESOURCE_VIOLET_SKY_BACKDROP, 0);
			btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED,
									RESOURCE_VIOLET_SKY_BACKDROP, 8);
			btEffectShowLivingTargets(state->args);
			btEffectCreateSprs(PROCNE_SPRITES, SPRITE_PROCNE, 2);

			resource = syResourceGetTableEntry(RESOURCE_VIOLET_COMET_SHEET);
			syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
			resource += SHEET_PALETTE_BYTES;
			fdDecompressTaggedLz(resource,
								 state->sheet + PROCNE_COMET_PICTURES);

			for(index = 0; index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
				index++) {
				PARTICLES[index].variant = 0;
			}

			EFFECT_FOR_EACH_TARGET(index, state->args) {
				battleObject = btObjectGetStub(state->args->tgts[index]);
				actor = battleObject->actor;
				actor->y.fixed = INTtoFIXED((syRandom() & 15) + 8);
			}

			ProjectionWork.centerX = 72;

			for(index = 0; index != PROCNE_GATHERING_SPARKS; index++) {
				speed = syRandom() & 127;
				angle = (syRandom() & 0x7fff) + DEGtoANG(225) - 1;
				state->particles[index].x = sySin(angle) * speed;
				state->particles[index].y = syCos(angle) * speed;
				state->particles[index].z = INTtoFIXED(syRandom() % 200 - 100);
				state->particles[index].variant = 0;
			}

			EFFECT_FOR_EACH_TARGET(index, state->args) {
				radius[index] = PROCNE_CORE_RADIUS;
				spin[index] = PROCNE_SKY_SPIN;
			}
		}

		/* Procne sways, pushed by each comet. */
		if(frame >= PROCNE_ARRIVAL_FRAME) {
			scale = unitScale;
			size = PROCNE_BOB;
			if(frame - PROCNE_ARRIVAL_FRAME <= 64)
				size = PROCNE_BOB * 2;

			procne.x -=
				(sySin((frame - PROCNE_ARRIVAL_FRAME) << 7) * PROCNE_SWAY) >> 6;
			procne.y +=
				(sySin((frame - PROCNE_ARRIVAL_FRAME) << 9) * size) >> 6;
			procne.x += drift.x;
			procne.y += drift.y;
			drift.x /= 2;
			drift.y /= 2;

			scale.x = FIXED_ONE;
			scale.y = FIXED_ONE;
			place.groundHeight = 0;
			/* Sprite i stands lower and higher by the same amount, which cancel
			 * on */
			/* the screen but give it a later draw slot. */
			for(index = 0; index != PROCNE_SPRITES; index++) {
				place.x =
					INTtoFIXED(Procne_GridX[index]) + procne.x + toFIXED(40);
				place.height = toFIXED(255) + index * toFIXED(64);
				place.y =
					INTtoFIXED(Procne_GridY[index] + index * 64) + procne.y;
				fdRenderApplyProjectedPlacementStub(state->sprs[index], &place,
													&scale, 0);
			}

			if(frame > 255) {
				s32 launch;

				depthOffset.z = toFIXED(256);
				arrived = 0;
				depthOffset.x = 0;
				depthOffset.y = 0;
				syInitMatrix();
				syMatrixTranslateVector(syVector3Coordinates(&depthOffset));

				/* Until the first comet, the sparks gather on Procne, each a
				 * part of */
				/* its distance a frame; those that have arrived make one spark
				 * that */
				/* grows with their number. */
				if(frame < PROCNE_COMET_FRAME) {
					for(index = 0; index != PROCNE_GATHERING_SPARKS; index++) {
						s32 xx;
						s32 yy;
						s32 zz;

						point = &state->particles[index];
						xx = (point->x >> 8) * (point->x >> 8);
						yy = (point->y >> 8) * (point->y >> 8);
						zz = (point->z >> 8) * (point->z >> 8);
						dist = sySquart(xx + yy + zz) >> 9;
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
						}
						else {
							arrived++;
						}
					}

					if(arrived > 0) {
						moteSize = arrived / 10 + 1;
						draw[1](canvas,
								motes + BattleEffect_MotePictures[moteSize - 1],
								(FIXEDtoINT(procne.x) >> 1) - moteSize / 2 + 32,
								FIXEDtoINT(procne.y) - moteSize - 4, moteSize,
								moteSize * 2);
					}
				}

				/* Each strike throws out sparks. */
				for(slot = 0; slot != PROCNE_STRIKES; slot++) {
					if(frame == Procne_Frames[slot]) {
						s32 speed;
						s32 angle;

						for(index = 0; index != PROCNE_STRIKE_SPARKS; index++) {
							speed = (syRandom() & 127) + 16;
							angle = syRandom() & TRIG_ANGLE_MASK;
							spark = &PARTICLES[slot * PROCNE_STRIKE_SPARK_ROWS +
											   index];
							spark->velX = (sySin(angle) * speed) >> 6;
							spark->velY = -(syCos(angle) * speed) >> 6;
							/* BUG: the random depth speed is stored over
							 * velocityY, not in */
							/* velocityZ as the other bursts store it, and z
							 * moves by */
							/* velocityY: the up-and-down speed just set is
							 * lost, and a */
							/* spark's depth moves with its height. */
#ifdef BUGFIX
							spark->velZ = ((syRandom() & 255) - 128) << 10;
#else
							spark->velY = ((syRandom() & 255) - 128) << 10;
#endif
							SET_POSITION(spark, 0, 0, 0);
							spark->variant = (syRandom() & 15) + 64;
						}
					}

					/* Inside the strikes' loop, so the sparks move and fade
					 * four */
					/* steps a frame. */
					if(frame >= Procne_Frames[0]) {
						spark = PARTICLES;
						for(index = 0;
							index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
							index++) {
							if(spark[index].variant > 0) {
								btProjectionPoint(&spark[index].x, &screen);
								screen.x = (FIXEDtoINT(screen.x) >> 1) + 32;
								screen.y = FIXEDtoINT(screen.y) + 56;
								screen.depth = FIXEDtoINT(screen.depth);
								CLAMP(screen.depth, PROCNE_DEPTH_NEAR,
									  PROCNE_DEPTH_FAR);
								moteSize = PROCNE_SPARK_SIZE(screen.depth, 3);
								EFFECT_DRAW_MOTE(draw[0], canvas, motes,
												 screen.x, screen.y, moteSize);
								spark[index].x += spark[index].velX;
								spark[index].y += spark[index].velY;
#ifdef BUGFIX
								spark[index].z += spark[index].velZ;
#else
								spark[index].z += spark[index].velY;
#endif
								spark[index].variant--;
							}
						}
					}
				}

				/* The comets leave Procne one after another; each pushes it
				 * back */
				/* and shakes the screen. */
				for(index = 0; index != PROCNE_STRIKES; index++) {
					comet = &state->particles[index + PROCNE_COMET_PARTICLES];
					launch = index * PROCNE_COMET_INTERVAL + PROCNE_COMET_FRAME;
					if(frame == launch) {
						comet->x = (FIXEDtoINT(procne.x) >> 1) + 32;
						comet->y = FIXEDtoINT(procne.y) - 4;
						/* Where it leaves Procne, kept for the ring of light.
						 */
						comet->velX = (FIXEDtoINT(procne.x) >> 1) + 32;
						comet->velY = FIXEDtoINT(procne.y) - 4;

						for(slot = 0; slot != PROCNE_RING_MOTES; slot++) {
							state->particles[slot + PROCNE_RING].variant = 4;
						}

						drift.x = toFIXED(8);
						drift.y = -toFIXED(2);
						state->shakeFrm = 8;
					}

					if(frame >= launch) {
						s32 picture;

						/* The comet grows over its first frames as it flies. */
						picture = (frame - launch) / 2;
						if(picture > 2)
							picture = 2;
						draw[1](canvas, state->sheet + Procne_Pictures[picture],
								comet->x,
								comet->y - Procne_PictureBiasY[picture],
								Procne_PictureWidths[picture],
								Procne_PictureHeights[picture]);
						comet->x -= 8;
						comet->y += 2;

						if(frame < launch + 8) {
							/* A ring of light grows where the comet left
							 * Procne. */
							syRotZ(-DEGtoANG(11.25));
							syRotY(-DEGtoANG(22.5));

							spark = &state->particles[PROCNE_RING];
							for(slot = 0; slot != PROCNE_RING_MOTES; slot++) {
								PARTICLE *mote;

								mote = &spark[slot];
								world.x = 0;
								world.y = mote->variant *
										  syCos(slot * (TRIG_TURN /
														PROCNE_RING_MOTES));
								world.z = mote->variant *
										  sySin(slot * (TRIG_TURN /
														PROCNE_RING_MOTES));
								mote->variant += 2;
								btProjectionPoint(&world.x, &screen);
								screen.x =
									(FIXEDtoINT(screen.x) >> 1) +
									state
										->particles[index +
													PROCNE_COMET_PARTICLES]
										.velX;
								screen.y =
									FIXEDtoINT(screen.y) +
									state
										->particles[index +
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

		EFFECT_END_FRAME(state);
	}

	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

	/* Put the targets back. */
	EFFECT_FOR_EACH_TARGET(index, state->args) {
		ACTOR *actor;

		battleObject = btObjectGetStub(state->args->tgts[index]);
		actor = battleObject->actor;
		actor->x.fixed = home[index].x;
		actor->z.fixed = home[index].y;
		actor->facing = homeFacing[index];
	}

	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;

	btEffectSetupBlendedDisp();

	for(index = 0; index != PROCNE_SPRITES; index++) {
		fdSprDestroyStub(state->sprs[index]);
	}

	EFFECT_FINISH_CANVAS();
}

#undef PROCNE_FRAMES
#undef PROCNE_ARRIVAL_FRAME
#undef PROCNE_DROP_FRAME
#undef PROCNE_COMET_FRAME
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
