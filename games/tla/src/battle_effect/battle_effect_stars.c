/* Astral Blast and Shining Star. Part of battle_effect.c. */

#define SHINING_STAR_TICKS 128
#define ASTRAL_BLAST_TICKS 60

/* Astral Blast flies three five-pointed stars at the target; Shining Star drops
   eight that bounce near it. Each outline's corners are projected onto the
   canvas, kept in the map cell buffer, and each edge stamped with motes. */

/* A star outline's corners, its five points and the five between, and the turn
   from one to the next, a tenth of a turn rounded up. */
#define STAR_CORNERS	  10
#define STAR_CORNER_ANGLE ((TRIG_TURN + STAR_CORNERS - 1) / STAR_CORNERS)

/* How fast the camera swings round, in 1/65536 turns a tick, slowing from
   tick 40 to a stop. */
#define ASTRAL_BLAST_TURN_SPEED	   128
/* A new star leaves every 12 ticks, spinning a 64th of a turn a tick. */
#define ASTRAL_BLAST_STAR_INTERVAL 12
#define ASTRAL_BLAST_SPIN		   (TRIG_TURN / 64)
/* A star is drawn only while its farthest corner is no deeper than this. */
#define ASTRAL_BLAST_DEPTH_MAX	   399999
/* The motes stamped along each edge of a star. */
#define ASTRAL_BLAST_EDGE_MOTES	   16
#define SHINING_STAR_EDGE_MOTES	   12
/* How far a Shining Star turns each tick, a 32nd of a turn. */
#define SHINING_STAR_SPIN		   (TRIG_TURN / 32)

/* Three pink stars of light fly from above the user at the first target,
   growing and spinning, while the camera swings round; each knocks the target
   back.

   Abilities: Astral Blast. */
void btEffectRunAstralBlast(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT screenPos;
#if !FIX_UNINITIALIZED_READ
	SCREEN_POINT base;
#endif
	FIXED				 scale[3];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	Sint32		   i;
	Uint8		  *graphics;
	BATTLE_CAMERA *camera;
	ACTOR		  *caster;
	ACTOR		  *src;
	ACTOR		  *tgt;
	PARTICLE	  *point;
	PARTICLE	  *star;
	Sint32		   start;
	Sint32		   peak;
	Sint32		   size;
	Sint32		   j;
	Sint32		   k;
	PARTICLE	  *corners;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	graphics = Ram_HeapSlots->battleSheet;
	camera = Ram_HeapSlots->cameraState;
	caster = (ACTOR *)btObjectGetStub(args->user)->obj;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_RUNE_SHEET), SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	fdActorSelectAnimationStub(caster, POSE_LEAP);
	fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 3);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	draw[0] = Ram_HeapSlots->ramCode;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	/* The stars leave from above the caster and reach the target in twelve
	   ticks; the second and third aim past it. */
	src = (ACTOR *)btObjectGetStub(args->user)->obj;
	tgt = (ACTOR *)btObjectGetStub(args->tgts[0])->obj;
	for(i = 0, point = state->particles; i != 3; i++) {
		SET_POSITION(
			point, src->x.fixed, src->y.fixed + toFIXED(40), src->z.fixed);
		if(i == 0)
			point->velX = (tgt->x.fixed - point->x) / 12;
		else
			point->velX = (tgt->x.fixed * 2 - point->x) / 12;
		point->velY = (tgt->y.fixed - point->y + toFIXED(40)) / 12;
		point->velZ = (tgt->z.fixed - point->z) / 12;
		point->variant = 0;
		point++;
	}

	corners = (PARTICLE *)MapGridMetatileBuffer;
	for(frm = 0; frm != ASTRAL_BLAST_TICKS; frm++) {
		if(frm <= 47) {
			Sint32 speed;

			if(frm <= 39)
				speed = ASTRAL_BLAST_TURN_SPEED;
			else
				speed = 768 - frm * 16;
			if(args->side == EFFECT_SIDE_ENEMIES)
				camera->yaw -= speed;
			else
				camera->yaw += speed;
		}

		for(i = 0; i != 3; i++) {
			start = i * ASTRAL_BLAST_STAR_INTERVAL;
			if(frm >= start) {
				star = &state->particles[i];
				size = (frm - start) / 4 + 2;
				if(size > MOTE_SIZE_MAX)
					size = MOTE_SIZE_MAX;
				EFFECT_VIEW_FROM_CAMERA(camera);
				syMatrixTranslateVector(btParticleCoordinates(star));
				peak = 0;
				for(j = 0; j != STAR_CORNERS; j++) {
					Sint32 depth;

					syPushMatrix();
					syRotZ((frm - start) * ASTRAL_BLAST_SPIN);
					syRotY(DEGtoANG(90));
					scale[0] =
						(frm - start) * (toFIXED(1) / 16) + toFIXED(1) / 16;
					if(scale[0] > toFIXED(1))
						scale[0] = toFIXED(1);
					scale[1] = scale[0];
					scale[2] = scale[0];
					syMatrixScaleVector(scale);
					syRotZ(j * STAR_CORNER_ANGLE);
					depth = btEffectPosProjectPoint(
						&StarPoints[(j & 1) * 3], &screenPos);
					if(peak < depth)
						peak = depth;
					screenPos.x >>= 1;
#if !FIX_UNINITIALIZED_READ
					/* UB: these first stores read base, which is never set; the
					   next two overwrite them. */
					corners[i * STAR_CORNERS + j].velX = screenPos.x + base.x;
					corners[i * STAR_CORNERS + j].velY = screenPos.y + base.y;
#endif
					corners[i * STAR_CORNERS + j].velX = screenPos.x;
					corners[i * STAR_CORNERS + j].velY = screenPos.y;
					syPopMatrix();
				}
				if(peak <= ASTRAL_BLAST_DEPTH_MAX) {
					/* Each edge, from corner to corner, read from the buffer
					   itself. */
					for(j = 0; j != STAR_CORNERS; j++) {
						PARTICLE *from = &((PARTICLE *)
								MapGridMetatileBuffer)[j + i * STAR_CORNERS];
						PARTICLE *to = &((PARTICLE *)
								MapGridMetatileBuffer)[(j + 1) % STAR_CORNERS +
							i * STAR_CORNERS];

						for(k = 0; k != ASTRAL_BLAST_EDGE_MOTES; k++) {
							Sint32 x = from->velX +
								(to->velX - from->velX) * k /
									ASTRAL_BLAST_EDGE_MOTES;
							Sint32 y = from->velY +
								(to->velY - from->velY) * k /
									ASTRAL_BLAST_EDGE_MOTES;

							EFFECT_DRAW_MOTE(
								draw[0], canvas, graphics, x, y, size);
						}
					}
				}
				star->x += star->velX;
				star->y += star->velY;
				star->z += star->velZ;
				/* Each star knocks the target back and shakes the screen. */
				if(frm == start + i + 10) {
					tgt->acceleration = toFIXED(2);
					tgt->speedLimit = toFIXED(8);
					tgt->velY = toFIXED(5);
					tgt->gravity = BATTLE_UNIT_GRAVITY;
					tgt->steeringFlg = 0;
					fdActorStartIdleStub(tgt);
					if(tgt->x.fixed < 0)
						fdActorSetMoveTargetStub(
							tgt, tgt->x.fixed - toFIXED(40), 0, tgt->z.fixed);
					else
						fdActorSetMoveTargetStub(
							tgt, tgt->x.fixed + toFIXED(40), 0, tgt->z.fixed);
					if(i == 2) {
						EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					} else {
						sySndPlayStub(SE_HEAVY_IMPACT);
						btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
							POSE_DAMAGE, 0, 8);
					}
					state->shakeFrm = 4;
				}
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
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
	Sint32				 frm;
	/* LEFTOVER(frame): draw[1], the idle half of the pair. */
	BLITTER_PAIR   draw;
	Sint32		   i;
	Uint8		  *graphics;
	BATTLE_CAMERA *camera;
	MOTION_OBJECT *src;
	MOTION_OBJECT *tgt;
	PARTICLE	  *point;
	PARTICLE	  *star;
	SCREEN_POINT   screenPos;
	Sint32		   size;
	Sint32		   j;
	Sint32		   k;
	PARTICLE	  *corners;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	graphics = Ram_HeapSlots->battleSheet;
	camera = Ram_HeapSlots->cameraState;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_RUNE_SHEET), SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	/* Each star leaves from above the caster on its own course to the target,
	   the later ones after a longer delay (held in variant). */
	src = btObjectGetStub(args->user)->obj;
	tgt = btObjectGetStub(args->tgts[0])->obj;
	for(i = 0, point = state->particles; i != 8; i++) {
		Sint32 delay = i * 8;

		SET_POSITION(point, src->x / 2, src->y + toFIXED(120), src->z);
		point->velX =
			(tgt->x + toFIXED((Sint32)((syRandom() & 0x7f) - 64)) - point->x) /
			12;
		point->velY = (tgt->y - point->y + toFIXED(20)) / 12;
		point->velZ = (tgt->z - point->z) / 12;
		point->variant = (syRandom() & 15) + delay;
		point++;
	}

	/* LEFTOVER(template): size holds 2, as btEffectRunAstralBlast
	 * (battle_effect_stars.c) sizes the same edge motes by tick; as 2 at the
	 * draw. */
	size = 2;
	corners = (PARTICLE *)MapGridMetatileBuffer;
	for(frm = 0; frm != SHINING_STAR_TICKS; frm++) {
		/* The stars run through the colours of the rainbow. */
		btEffectUpdatePhasePalette(
			frm, TRIG_TURN - DEGtoANG(120), DEGtoANG(120), 0);
		if(frm == 96)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		for(i = 0; i != 8; i++) {
			star = &state->particles[i];
			if(frm >= star->variant) {
				EFFECT_VIEW_FROM_CAMERA(camera);
				btEffectPosProjectPoint(star, &screenPos);
				screenPos.x >>= 1;
				if(screenPos.x >= -8 && screenPos.x <= CANVAS_SIZE - 1 &&
					screenPos.y <= CANVAS_SIZE - 1 && screenPos.y >= -8) {
					for(j = 0; j != STAR_CORNERS; j++) {
						corners[i * STAR_CORNERS + j].velX = screenPos.x +
							FIXEDtoINT((sySin(j * STAR_CORNER_ANGLE -
											(frm - star->variant) *
												SHINING_STAR_SPIN) *
										   StarRadii[j & 1]) /
								2);
						corners[i * STAR_CORNERS + j].velY = screenPos.y -
							FIXEDtoINT(
								syCos(j * STAR_CORNER_ANGLE -
									(frm - star->variant) * SHINING_STAR_SPIN) *
								StarRadii[j & 1]);
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
								draw[0], canvas, graphics, x, y, size);
						}
					}
				}
				/* At the ground the star bounces up at half its sideways
				   speed. */
				if(star->y <= toFIXED(30) - 1) {
					star->velY = -star->velY;
					star->velX /= 2;
					star->velZ /= 2;
					state->shakeFrm = 4;
					sySndPlayStub(SE_HEAVY_IMPACT);
					for(j = 0; j != args->tgtCnt; j++)
						btEffectTintBattler(args->tgts[j], SPRITE_COLOR_WHITE,
							POSE_DAMAGE, j, 8);
				}
				star->x += star->velX;
				star->y += star->velY;
				star->z += star->velZ;
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef SHINING_STAR_TICKS
#undef ASTRAL_BLAST_TICKS
#undef STAR_CORNERS
#undef STAR_CORNER_ANGLE
#undef ASTRAL_BLAST_TURN_SPEED
#undef ASTRAL_BLAST_STAR_INTERVAL
#undef ASTRAL_BLAST_SPIN
#undef ASTRAL_BLAST_DEPTH_MAX
#undef ASTRAL_BLAST_EDGE_MOTES
#undef SHINING_STAR_EDGE_MOTES
#undef SHINING_STAR_SPIN
