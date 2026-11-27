/* Astral Blast and Shining Star. Part of battle_effect.c. */

#define SHINING_STAR_FRAMES 128 /* frames of Shining Star */
#define ASTRAL_BLAST_FRAMES 60	/* frames of Astral Blast */
#define ASTRAL_BLAST_STARS	3
#define SHINING_STARS		8

/* Astral Blast flies three five-pointed stars at the target; Shining Star drops
   eight that bounce near it. Each outline's corners are projected onto the
   canvas and each edge stamped with motes. */

/* A star outline's corners, its five points and the five between, and the turn
   from one to the next, a tenth of a turn rounded up. */
#define STAR_CORNERS	  10
#define STAR_CORNER_ANGLE ((TRIG_TURN + STAR_CORNERS - 1) / STAR_CORNERS)

/* How fast the camera swings round, slowing from frame 40 to a stop. */
#define ASTRAL_BLAST_TURN_SPEED	   128
/* A new star leaves every 12 frames, spinning a 64th of a turn a frame. */
#define ASTRAL_BLAST_STAR_INTERVAL 12
#define ASTRAL_BLAST_SPIN		   (TRIG_TURN / 64)
/* A star is drawn only while its farthest corner is no deeper than this. */
#define ASTRAL_BLAST_DEPTH_MAX	   399999
/* The motes stamped along each edge of a star. */
#define ASTRAL_BLAST_EDGE_MOTES	   16
#define SHINING_STAR_EDGE_MOTES	   12
/* How far a Shining Star turns each frame, a 32nd of a turn. */
#define SHINING_STAR_SPIN		   (TRIG_TURN / 32)

/* Three pink stars of light fly from above the user at the first target,
   growing and spinning, while the camera swings round; each knocks the target
   back.

   Abilities: Astral Blast. */
void btEffectRunAstralBlast(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT screenPosition;
#if !FIX_UNINITIALIZED_READ
	SCREEN_POINT base;
#endif
	VECTOR3				 scale;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	Sint32		   i;
	Uint8		  *motes;
	BATTLE_CAMERA *camera;
	ACTOR		  *user;
	ACTOR		  *source;
	ACTOR		  *target;
	PARTICLE	  *point;
	PARTICLE	  *star;
	Sint32		   start;
	Sint32		   peak;
	Sint32		   size;
	Sint32		   j;
	Sint32		   k;
	PARTICLE	  *corners;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	user = btObjectGetStub(args->user)->actor;
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_RUNE_SHEET),
		SHEET_PALETTE_BYTES);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);
	fdActorSelectAnimationStub(user, POSE_LEAP);
	fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL * 3);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	source = btObjectGetStub(state->args->user)->actor;
	target = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0, point = state->particles; i != ASTRAL_BLAST_STARS; i++) {
		SET_POSITION(point, source->x.fixed, source->y.fixed + toFIXED(40),
			source->z.fixed);
		if(i == 0)
			point->velX = (target->x.fixed - point->x) / 12;
		else
			point->velX = (target->x.fixed * 2 - point->x) / 12;
		point->velY = (target->y.fixed - point->y + toFIXED(40)) / 12;
		point->velZ = (target->z.fixed - point->z) / 12;
		point->variant = 0;
		point++;
	}

	/* Each star's ten corners on the canvas, kept in PARTICLES' velocities. */
	corners = PARTICLES;
	for(frame = 0; frame != ASTRAL_BLAST_FRAMES; frame++) {
		if(frame <= 47) {
			BATTLE_CAMERA *view = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
			Sint32		   speed;

			if(frame <= 39)
				speed = ASTRAL_BLAST_TURN_SPEED;
			else
				speed = 768 - frame * 16;
			if(state->args->side == EFFECT_SIDE_ENEMIES)
				view->yaw -= speed;
			else
				view->yaw += speed;
		}
		for(i = 0; i != ASTRAL_BLAST_STARS; i++) {
			start = i * ASTRAL_BLAST_STAR_INTERVAL;
			if(frame >= start) {
				star = &state->particles[i];
				size = (frame - start) / 4 + 2;
				if(size > MOTE_SIZE_MAX)
					size = MOTE_SIZE_MAX;
				EFFECT_VIEW_FROM_CAMERA(camera);
				syMatrixTranslateVector(btParticleCoordinates(star));
				peak = 0;
				for(j = 0; j != STAR_CORNERS; j++) {
					Sint32 depth;

					syPushMatrix();
					syRotZ((frame - start) * ASTRAL_BLAST_SPIN);
					syRotY(DEGtoANG(90));
					scale.x =
						(frame - start) * (toFIXED(1) / 16) + toFIXED(1) / 16;
					if(scale.x > toFIXED(1))
						scale.x = toFIXED(1);
					scale.y = scale.x;
					scale.z = scale.x;
					syMatrixScaleVector(syVector3Coordinates(&scale));
					syRotZ(j * STAR_CORNER_ANGLE);
					depth =
						btProjectionPoint(&Star_Points[j & 1], &screenPosition);
					if(peak < depth)
						peak = depth;
					screenPosition.x >>= 1;
#if !FIX_UNINITIALIZED_READ
					/* UB: these first stores read base, which is never set; the
					   next two overwrite them. */
					corners[i * STAR_CORNERS + j].velX =
						screenPosition.x + base.x;
					corners[i * STAR_CORNERS + j].velY =
						screenPosition.y + base.y;
#endif
					corners[i * STAR_CORNERS + j].velX = screenPosition.x;
					corners[i * STAR_CORNERS + j].velY = screenPosition.y;
					syPopMatrix();
				}
				if(peak <= ASTRAL_BLAST_DEPTH_MAX) {
					for(j = 0; j != STAR_CORNERS; j++) {
						PARTICLE *from =
							&((PARTICLE *)PARTICLES)[j + i * STAR_CORNERS];
						PARTICLE *to =
							&((PARTICLE *)PARTICLES)[(j + 1) % STAR_CORNERS +
								i * STAR_CORNERS];

						for(k = 0; k != ASTRAL_BLAST_EDGE_MOTES; k++) {
							Sint32 x = from->velX +
								(to->velX - from->velX) * k /
									ASTRAL_BLAST_EDGE_MOTES;
							Sint32 y = from->velY +
								(to->velY - from->velY) * k /
									ASTRAL_BLAST_EDGE_MOTES;

							EFFECT_DRAW_MOTE(
								draw[0], canvas, motes, x, y, size);
						}
					}
				}
				star->x += star->velX;
				star->y += star->velY;
				star->z += star->velZ;
				/* Each star knocks the target back and shakes the screen. */
				if(frame == start + i + 10) {
					target->acceleration = toFIXED(2);
					target->speedLimit = toFIXED(8);
					target->velY = toFIXED(5);
					target->gravity = BATTLE_UNIT_GRAVITY;
					target->steeringFlg = 0;
					fdActorStartIdleStub(target);
					if(target->x.fixed < 0)
						fdActorSetMoveTargetStub(target,
							target->x.fixed - toFIXED(40), 0, target->z.fixed);
					else
						fdActorSetMoveTargetStub(target,
							target->x.fixed + toFIXED(40), 0, target->z.fixed);
					if(i == ASTRAL_BLAST_STARS - 1) {
						EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					} else {
						sySndPlayStub(SE_HEAVY_IMPACT);
						btEffectTintBattler(state->args->tgts[0],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
					}
					state->shakeFrm = 4;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS();
}

/* Eight spinning star outlines fall from above the user and bounce on the
   ground near the first target, each bounce flashing the targets.

   Abilities: Shining Star. */
void btEffectRunShiningStar(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	Sint32		   i;
	Uint8		  *motes;
	BATTLE_CAMERA *camera;
	ACTOR		  *source;
	ACTOR		  *target;
	PARTICLE	  *point;
	PARTICLE	  *star;
	SCREEN_POINT   screenPosition;
	Sint32		   size;
	Sint32		   j;
	Sint32		   k;
	PARTICLE	  *corners;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_RUNE_SHEET),
		SHEET_PALETTE_BYTES);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_YELLOW_GLOW_SHEET), motes);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	state->transferMode = CANVAS_COPY_AND_FADE;
	state->transferValue = CANVAS_FADE_HALF;
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	source = btObjectGetStub(state->args->user)->actor;
	target = btObjectGetStub(state->args->tgts[0])->actor;
	for(i = 0, point = state->particles; i != SHINING_STARS; i++) {
		Sint32 delay = i * 8;

		SET_POSITION(point, source->x.fixed / 2, source->y.fixed + toFIXED(120),
			source->z.fixed);
		point->velX =
			(target->x.fixed + toFIXED((Sint32)((syRandom() & 0x7f) - 64)) -
				point->x) /
			12;
		point->velY = (target->y.fixed - point->y + toFIXED(20)) / 12;
		point->velZ = (target->z.fixed - point->z) / 12;
		point->variant = (syRandom() & 15) + delay;
		point++;
	}

	/* LEFTOVER(template): size holds 2, as btEffectRunAstralBlast
	 * (battle_effect_stars.c) sizes the same edge motes by frame; as 2 at the
	 * draw. */
	size = 2;
	corners = PARTICLES;
	for(frame = 0; frame != SHINING_STAR_FRAMES; frame++) {
		/* The stars run through the colours of the rainbow. */
		btEffectUpdatePhasePalette(
			frame, TRIG_TURN - DEGtoANG(120), DEGtoANG(120), 0);
		if(frame == 96)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		for(i = 0; i != SHINING_STARS; i++) {
			star = &state->particles[i];
			if(frame >= star->variant) {
				EFFECT_VIEW_FROM_CAMERA(camera);
				btProjectionPoint(star, &screenPosition);
				screenPosition.x >>= 1;
				if(screenPosition.x >= -8 &&
					screenPosition.x <= CANVAS_SIZE - 1 &&
					screenPosition.y <= CANVAS_SIZE - 1 &&
					screenPosition.y >= -8) {
					for(j = 0; j != STAR_CORNERS; j++) {
						corners[i * STAR_CORNERS + j].velX = screenPosition.x +
							FIXEDtoINT((sySin(j * STAR_CORNER_ANGLE -
											(frame - star->variant) *
												SHINING_STAR_SPIN) *
										   Star_Radii[j & 1]) /
								2);
						corners[i * STAR_CORNERS + j].velY = screenPosition.y -
							FIXEDtoINT(syCos(j * STAR_CORNER_ANGLE -
										   (frame - star->variant) *
											   SHINING_STAR_SPIN) *
								Star_Radii[j & 1]);
					}
					for(j = 0; j != STAR_CORNERS; j++) {
						PARTICLE *from = &corners[j + i * STAR_CORNERS];
						PARTICLE *to =
							&corners[i * STAR_CORNERS + (j + 1) % STAR_CORNERS];

						for(k = 0; k != SHINING_STAR_EDGE_MOTES; k++) {
							Sint32 x = from->velX +
								(to->velX - from->velX) * k /
									SHINING_STAR_EDGE_MOTES;
							Sint32 y = from->velY +
								(to->velY - from->velY) * k /
									SHINING_STAR_EDGE_MOTES;

							EFFECT_DRAW_MOTE(
								draw[0], canvas, motes, x, y, size);
						}
					}
				}
				if(star->y <= toFIXED(30) - 1) {
					star->velY = -star->velY;
					star->velX /= 2;
					star->velZ /= 2;
					state->shakeFrm = 4;
					sySndPlayStub(SE_HEAVY_IMPACT);
					EFFECT_FOR_EACH_TARGET(j, state->args)
						btEffectTintBattler(state->args->tgts[j],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
				}
				star->x += star->velX;
				star->y += star->velY;
				star->z += star->velZ;
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef SHINING_STAR_FRAMES
#undef ASTRAL_BLAST_FRAMES
#undef ASTRAL_BLAST_STARS
#undef SHINING_STARS
#undef STAR_CORNERS
#undef STAR_CORNER_ANGLE
#undef ASTRAL_BLAST_TURN_SPEED
#undef ASTRAL_BLAST_STAR_INTERVAL
#undef ASTRAL_BLAST_SPIN
#undef ASTRAL_BLAST_DEPTH_MAX
#undef ASTRAL_BLAST_EDGE_MOTES
#undef SHINING_STAR_EDGE_MOTES
#undef SHINING_STAR_SPIN
