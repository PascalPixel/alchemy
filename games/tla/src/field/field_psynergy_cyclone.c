/* @file */
/* Cyclone and the whirlwinds: the 96-particle burst, the leader carried or */
/* lifted to a Cyclone cell, Cyclone itself, an actor leaving or arriving in a
 */
/* whirlwind (fdActorDepartInRing, fdActorArriveInRing), then Blaze's jet of */
/* flame and the smoke it leaves. */
/* Part of field_psynergy.c. */

/* The 96-particle burst's task (PARTICLE_BURST_STATE in
 * HEAP_SLOT_BATTLE_EFFECT,
 */
/* started by fdParticleBurstStart). Each particle whose delay counter is */
/* between 0 and 63 shows the picture (delay / 8) % 6 of the burst's tiles 48 */
/* pixels out along its heading, then turns by its spin, rises by its speed and
 */
/* spins eight units faster; every counter advances. The state's flash flag is
 */
/* set on frame 30 only, and the burst stops being active on frame 112. */
void fdParticleBurstUpdate(void) {
	PARTICLE_BURST_STATE *state;
	BURST_PARTICLE *particle;
	EFFECT_SPR *sprite;
	s32 position[3];
	s32 i;

	state = (PARTICLE_BURST_STATE *)Ram_HeapSlots->battleEff;
	particle = state->particles;

	for(i = 0; i < PARTICLE_BURST_COUNT; i++, particle++) {
		sprite = &state->sprs[i];
		if((u32)particle->delay < 64) {
			sprite->oam.idx = state->tile + (particle->delay >> 3) % 6 * 2;
			position[0] = particle->x;
			position[1] = particle->y;
			position[2] = particle->z;
			syVectorAddPolarOffset(toFIXED(48), particle->heading, position);
			fdCameraWorldToScr(position);
			sprite->x = position[0];
			sprite->y = position[2];
			fdEffectSprDraw(sprite);
			particle->heading += particle->spin;
			particle->y += particle->speed;
			particle->spin =
				particle->spin >= 0 ? particle->spin + 8 : particle->spin - 8;
		}
		particle->delay++;
	}
	state->flash = 0;
	if(state->frm == 30)
		state->flash = 1;
	if(state->frm == 112)
		state->active = 0;
	state->frm++;
}

/* Ends the particle burst (fdParticleBurstStart): stops its task and frees its
 */
/* tiles. */
void fdParticleBurstEnd(void) {
	PARTICLE_BURST_STATE *state = Ram_HeapSlots->battleEff;

	syCloseEvent((u32)fdParticleBurstUpdate);
	syVramCacheFreeEntry(state->cacheEntry);
}

/* The frames the whirlwind takes to carry or lift the leader. */
#define CYCLONE_CARRY_FRAMES 128

/* How far the whirlwind turns the leader round each frame of the ride. */
#define CYCLONE_CARRY_TURN_STEP (TRIG_TURN * 5 / 256)

/* Task of the Cyclone: the whirlwind carries the leader over 128 frames from */
/* where it stood to the nearby CYCLONE_CELL_CARRY cell (a Cyclone teleporter),
 */
/* circling and arcing up and down on the way, its follower with it. */
void fdCycloneCarryLeader(void) {
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	CYCLONE_STATE *state = Ram_HeapSlots->battleEff;
	ACTOR *owner = effect->caster;
	s32 timer = state->tmr;
	s32 position[3];

	switch(state->state) {
	case 0:
		state->radius = sySin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
						<< 5;
		state->ang += CYCLONE_CARRY_TURN_STEP;
		position[0] = state->from[0] + (state->to[0] - state->from[0]) * timer /
										   CYCLONE_CARRY_FRAMES;
		position[1] =
			state->from[1] +
			(state->to[1] - state->from[1]) * timer / CYCLONE_CARRY_FRAMES +
			(sySin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES)) << 6);
		position[2] = state->from[2] + (state->to[2] - state->from[2]) * timer /
										   CYCLONE_CARRY_FRAMES;
		syVectorAddPolarOffset(state->radius, state->ang, position);
		SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
		if(timer >= CYCLONE_CARRY_FRAMES) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case 1:
		state->state = FIELD_PSYNERGY_FINISHED;
		break;
	}
	state->tmr++;
	state->follower->x.fixed = owner->x.fixed;
	state->follower->y.fixed = owner->y.fixed;
	state->follower->z.fixed = owner->z.fixed;
}

/* fdCycloneCarryLeader for a CYCLONE_CELL_LIFT cell: the leader is carried */
/* there 32 pixels up, circling, without the arc. */
void fdCycloneLiftLeader(void) {
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	CYCLONE_STATE *state = Ram_HeapSlots->battleEff;
	ACTOR *owner = effect->caster;
	s32 timer = state->tmr;
	s32 position[3];

	switch(state->state) {
	case 0:
		state->radius = sySin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
						<< 5;
		state->ang += CYCLONE_CARRY_TURN_STEP;
		position[0] = state->from[0] + (state->to[0] - state->from[0]) * timer /
										   CYCLONE_CARRY_FRAMES;
		position[1] = state->from[1] + (state->to[1] - state->from[1]) * timer /
										   CYCLONE_CARRY_FRAMES;
		position[2] = state->from[2] + (state->to[2] - state->from[2]) * timer /
										   CYCLONE_CARRY_FRAMES;
		syVectorAddPolarOffset(state->radius, state->ang, position);
		SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
		if(timer >= CYCLONE_CARRY_FRAMES) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case 1:
		owner->y.fixed -= toFIXED(2) / 5;
		if(effect->sceneVariant == 1) {
			if(timer == 40)
				state->state = FIELD_PSYNERGY_FINISHED;
		}
		else if(timer == 40) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case 2:
		owner->y.fixed += toFIXED(19) / 5;
		if(timer == 40)
			state->state = FIELD_PSYNERGY_FINISHED;
		break;
	}
	state->tmr++;
	state->follower->x.fixed = owner->x.fixed;
	state->follower->y.fixed = owner->y.fixed;
	state->follower->z.fixed = owner->z.fixed;
}

/* Per-frame update: turns an actor 1/16 turn a frame. */
void fdActorUpdateSpin(MOTION_OBJECT *object) { object->ang += TRIG_TURN / 16; }

/* Cyclone cast on a target: runs fdPsynergyCyclone. */
void fdPsynergyCycloneOnTarget(void) { fdPsynergyCyclone(); }

/* The frames the Cyclone's whirlwind takes to rise to full height, and the */
/* frames its whirl of wind is drawn for, then whirls on as the Cyclone acts. */
#define CYCLONE_WHIRLWIND_RISE_FRAMES 20
#define CYCLONE_WHIRL_FRAMES		  60
#define CYCLONE_SPIN_FRAMES			  16
/* A whirlwind widens or narrows by WHIRLWIND_RESIZE_STEP of its width a */
/* frame, and an actor by the same of its size: the frames for the whole */
/* whirlwind and for half an actor. */
#define WHIRLWIND_RESIZE_STEP	(FIXED_ONE / 32)
#define WHIRLWIND_RESIZE_FRAMES 32
#define ACTOR_RESIZE_FRAMES		16
/* The whirl's arcs spread by a step that keeps CYCLONE_SPREAD_KEPT of every */
/* CYCLONE_SPREAD_SCALE parts of itself each frame. */
#define CYCLONE_SPREAD_KEPT	 123
#define CYCLONE_SPREAD_SCALE 128

/* Cyclone ("Conjure wind to scatter weeds."): the caster spins in a whirlwind
 */
/* of blue and white drawn on the canvas sprite, and the weeds on the cells */
/* round it (fdPsynergyGatherNearbyCells) are scattered. */
void fdPsynergyCyclone(void) {
	CYCLONE_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(CYCLONE_STATE));
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	ACTOR *owner = effect->caster;
	s16 *points = &field->psynergyCells[0];
	FIXED spread = toFIXED(4);
	s32 step = toFIXED(7) / 8;
	CANVAS_SPR_STATE *canvas;
	ACTOR *whirlwind;
	u16 *offsets;
	u8 *pixels;
	u16 *palette;
	FIXED pos[3];
	s32 mode;
	s32 frame;
	s32 i, j;
	s32 blue, c;
	s32 point;

	fdPsynergyFadeToCastCol();
	fdPsynergyGatherNearbyCells();
	fdBattleEffectSetObjectUpdateMode(effect->caster, CASTER_LOOK_NORMAL);
	owner->update = fdActorUpdateSpin;
	owner->tmr = owner->facing;
	/* The whirl's colours: blue full, red and green rising from half to full.
	 */
	palette = (u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
	blue = RGB(0, 0, RGB_LEVEL_MAX);
	for(j = 0; j < PALETTE_COLORS; j++) {
		c = j + (RGB_LEVEL_MAX + 1) / 2;
		syIoWrite16(RGB(0, c, 0) | blue | c, palette);
		palette++;
	}
	fdBattleEffectStartCanvasSpr();
	canvas = Ram_HeapSlots->canvasSpr;
	pos[0] = effect->caster->x.fixed;
	pos[1] = effect->caster->y.fixed;
	pos[2] = effect->caster->z.fixed;
	fdCameraWorldToScr(pos);
	canvas->x = (s16)FIXEDtoINT(pos[0]);
	canvas->y = (s16)FIXEDtoINT(pos[2]);
	canvas->pri = 1;
	canvas->depth = OAM_DEPTH_WINDOW;
	canvas->mode = CANVAS_SPRITE_KEPT;
	canvas->scaleX = FIXED_ONE * 3 / 2;
	canvas->scaleY = FIXED_ONE * 3 / 2;
	fdEffectSprInit(&canvas->spr, 32, 32, OBJ_SIZE_64X64, canvas->tile);
	canvas->spr.oam.fullCol = 1;
	canvas->spr.oam.pal = 0;
	offsets = Ram_HeapSlots->canvasMap;
	pixels = Ram_HeapSlots->battleCanvas;
	whirlwind = fdActorSpawn(SPRITE_WHIRLWIND, owner->x.fixed,
							 owner->y.fixed - toFIXED(6), owner->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = FIXED_ONE * 3 / 2;
	whirlwind->scaleY = 0;
	sySynch(1);
	sySndPlayStub(SE_RUSH);
	for(i = 0; i < CYCLONE_WHIRLWIND_RISE_FRAMES; i++) {
		whirlwind->scaleY = FIXED_ONE * i / CYCLONE_WHIRLWIND_RISE_FRAMES;
		sySynch(1);
	}
	for(i = 0; i < CYCLONE_WHIRL_FRAMES; i++) {
		for(j = 0; j < 4; j++)
			fdCyclonePlotArc(
				FIXEDtoINT((s32)(spread + syRandom() * 4 - INTtoFIXED(j))),
				syRandom(), offsets, pixels);
		spread += step;
		if(spread > toFIXED(28))
			spread = toFIXED(28);
		step = step * CYCLONE_SPREAD_KEPT / CYCLONE_SPREAD_SCALE;
		canvas->ang += TRIG_TURN / 64;
		canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
		sySynch(1);
	}
	fdPsynergyFadeFromCastCol();
	state->follower = whirlwind;
	state->from[0] = owner->x.fixed;
	state->from[1] = owner->y.fixed;
	state->from[2] = owner->z.fixed;
	state->to[0] = owner->x.fixed;
	state->to[1] = owner->y.fixed;
	state->to[2] = owner->z.fixed;
	state->ang = 0;
	state->state = 0;
	state->tmr = 0;
	state->state = 0;
	if(field->cycloneLiftX != -1)
		mode = CYCLONE_CELL_LIFT;
	else if(field->cycloneCarryX != -1)
		mode = CYCLONE_CELL_CARRY;
	else {
		point = points[0];
		mode = point != -1;
	}

	if(mode == CYCLONE_CELL_LIFT) {
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorSetPosAndResetMotionStub(owner, owner->x.fixed, owner->y.fixed,
										owner->z.fixed);
		fdActorSetSprFlagsStub(owner, 0);
		fdActorSetSprPriority(effect->casterId, 1);
		owner->motionFlg = 0;
		frame = 0;
		sySynch(1);
		state->to[0] = toFIXED(field->cycloneLiftX);
		state->to[1] = owner->y.fixed + toFIXED(32);
		state->to[2] = toFIXED(field->cycloneLiftZ);
		sySetEvent(fdCycloneLiftLeader, TASK_PASS_FRAME);
		while(state->state != FIELD_PSYNERGY_FINISHED) {
			if(frame == 30)
				fdPsynergyRunTouchedEvents();
			frame++;
			sySynch(1);
		}
		syCloseEvent(fdCycloneLiftLeader);
		if(effect->sceneVariant == 0)
			owner->scaleX = 0;
		whirlwind->scaleX = 0;
		if(state->kind)
			fdParticleBurstEnd();
	}
	else if(mode == CYCLONE_CELL_CARRY) {
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorSetPosAndResetMotionStub(owner, owner->x.fixed, owner->y.fixed,
										owner->z.fixed);
		fdActorSetSprFlagsStub(owner, 0);
		owner->motionFlg = 0;
		sySynch(1);
		state->to[0] = toFIXED(field->cycloneCarryX);
		state->to[1] = owner->y.fixed;
		state->to[2] = toFIXED(field->cycloneCarryZ);
		sySetEvent(fdCycloneCarryLeader, TASK_PASS_FRAME);
		frame = 0;
		while(state->state != FIELD_PSYNERGY_FINISHED) {
			if(frame == 30)
				fdPsynergyRunTouchedEvents();
			frame++;
			sySynch(1);
		}
		syCloseEvent(fdCycloneCarryLeader);
		for(i = 0; i < ACTOR_RESIZE_FRAMES; i++) {
			owner->scaleX += -WHIRLWIND_RESIZE_STEP;
			owner->scaleY = owner->scaleX;
			sySynch(1);
		}
		owner->scaleX = 0;
		for(i = 0; i < WHIRLWIND_RESIZE_FRAMES; i++) {
			whirlwind->scaleX += -WHIRLWIND_RESIZE_STEP;
			whirlwind->scaleY += -(FIXED_ONE / 128);
			sySynch(1);
		}
		whirlwind->scaleX = 0;
		if(state->kind)
			fdParticleBurstEnd();
	}
	else if(mode == CYCLONE_CELL_BURST) {
		sySndPlayStub(SE_WHOOSH);
		fdActorSelectAnimationStub(whirlwind, 6);
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		owner->update = NULL;
		owner->facing = owner->tmr;
		frame = 0;
		while(state->cnt != 0) {
			if(frame == 30)
				fdPsynergyRunTouchedEvents();
			sySynch(1);
			frame++;
		}
		if(state->kind)
			fdParticleBurstEnd();
	}
	else {
		sySndPlayStub(SE_WHOOSH);
		fdActorSelectAnimationStub(whirlwind, 6);
		for(i = 0; i < CYCLONE_SPIN_FRAMES; i++) {
			owner->facing += FACING_STEP;
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
	}
	sySndPlayStub(SE_WHOOSH);
	fdActorReleaseStub(whirlwind);
	owner->update = NULL;
	owner->facing = owner->tmr;
	fdBattleEffectEndCanvasSpr();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* The frames fdActorDepartInRing's actor circles in its whirlwind. */
#define DEPART_FRAMES 128

/* The actor leaves in a whirlwind (SPRITE_WHIRLWIND): as the screen */
/* transition starts it circles inside it for DEPART_FRAMES, from 96 pixels up
 */
/* and 32 out down to where it stood, then the whirlwind vanishes with a whoosh
 */
/* and the camera follows the actor again. */
void fdActorDepartInRing(s32 actorId) {
	ACTOR *actor = fdActorGetById(actorId);
	s32 angle = actor->facing;
	u8 motionFlags = actor->motionFlg;
	ACTOR *whirlwind;
	FIXED x, y, z;
	s32 spin;
	FIXED position[3];
	s32 i;

	fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	actor->motionFlg = 0;
	actor->update = fdActorUpdateSpin;
	whirlwind = fdActorSpawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
							 actor->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = FIXED_ONE * 3 / 2;
	whirlwind->scaleY = FIXED_ONE;
	x = actor->x.fixed;
	y = actor->y.fixed;
	z = actor->z.fixed;
	spin = 0;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	for(i = 0; i < DEPART_FRAMES; i++) {
		position[0] = x;
		position[1] = y + syCos(i * (TRIG_TURN / 4 / DEPART_FRAMES)) * 96;
		position[2] = z;
		syVectorAddPolarOffset(syCos(i * (TRIG_TURN / 4 / DEPART_FRAMES)) * 32,
							   spin, position);
		SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
		spin += TRIG_TURN / 64;
		SET_ACTOR_POSITION(whirlwind, actor->x.fixed,
						   actor->y.fixed - toFIXED(6), actor->z.fixed);
		sySynch(1);
	}
	fdActorSetPosAndResetMotionStub(actor, x, y, z);
	sySndPlayStub(SE_WHOOSH);
	fdActorSelectAnimationStub(whirlwind, 6);
	fdEventWait(15);
	fdActorReleaseStub(whirlwind);
	actor->update = NULL;
	actor->facing = angle;
	actor->motionFlg = motionFlags;
	sySynch(1);
	fdCameraFollowActor(actorId, 1);
}

/* The actor arrives in a whirlwind, fdActorDepartInRing's counterpart: once */
/* the screen transition is done, a whirlwind (SPRITE_WHIRLWIND) widens on the
 */
/* unseen, spinning actor for WHIRLWIND_RESIZE_FRAMES, then vanishes with a */
/* whoosh as the actor grows back from half its size and shows again. */
void fdActorArriveInRing(s32 actorId) {
	ACTOR *actor = fdActorGetById(actorId);
	s32 angle = actor->facing;
	ACTOR *whirlwind;
	s32 i;

	actor->update = fdActorUpdateSpin;
	actor->scaleX = 0;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetSprFlagsStub(fdActorGetById(actorId), 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	whirlwind = fdActorSpawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
							 actor->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = FIXED_ONE / 2;
	whirlwind->scaleY = FIXED_ONE * 3 / 4;
	for(i = 0; i < WHIRLWIND_RESIZE_FRAMES; i++) {
		whirlwind->scaleX += WHIRLWIND_RESIZE_STEP;
		whirlwind->scaleY += FIXED_ONE / 128;
		sySynch(1);
	}
	whirlwind->scaleX = FIXED_ONE * 3 / 2;
	whirlwind->scaleY = FIXED_ONE;
	sySndPlayStub(SE_WHOOSH);
	fdActorSelectAnimationStub(whirlwind, 6);
	actor->scaleY = actor->scaleX = FIXED_ONE / 2;
	for(i = 0; i < ACTOR_RESIZE_FRAMES; i++) {
		actor->scaleY = actor->scaleX = actor->scaleX + WHIRLWIND_RESIZE_STEP;
		sySynch(1);
	}
	fdActorSetSprFlagsStub(fdActorGetById(actorId), SPRITE_FLAG_SHADOW);
	fdEventWait(10);
	fdActorReleaseStub(whirlwind);
	actor->update = NULL;
	actor->facing = angle;
}

/* What Blaze does (BLAZE_STATE.mode). */
typedef enum BLAZE_MODE {
	BLAZE_FLICKER = 0, /* cast at nothing with no torch in reach: a flicker */
	BLAZE_LIGHT_TORCH =
		1,			 /* cast at nothing: lights the nearest torch in reach */
	BLAZE_SPRAY = 2, /* cast at a target: the jet of flame */
	/* The jet, faster and longer (FIELD_STATE.blazeLongSpray). */
	BLAZE_LONG_SPRAY = 3
} BLAZE_MODE;

/* The flames of Blaze's jet, and the smoke puffs they leave. */
#define BLAZE_FLAMES 16
/* The frames a flame of the jet lives, in BLAZE_LONG_SPRAY and otherwise. */
#define BLAZE_LONG_FLAME_FRAMES 33
#define BLAZE_FLAME_FRAMES		25

/* Blaze's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the flame tiles, 16 */
/* flames and 16 smoke puffs with their motion, where the jet starts and where
 */
/* it points, and the actor it has reached. */
typedef struct BLAZE_STATE {
	u8 unused[4096]; /* no code names it */
	EFFECT_SPR flames[BLAZE_FLAMES];
	FIELD_PSYNERGY_PARTICLE flameMotion[BLAZE_FLAMES];
	EFFECT_SPR puffs[BLAZE_FLAMES];
	FIELD_PSYNERGY_PARTICLE puffMotion[BLAZE_FLAMES];
	s32 origin[3];
	u16 flameTile;
	u16 puffTile;
	u16 ang;
	u16 tgtAng;
	s16 hitObj;
	s8 hit;
	s8 emitting;
	s32 speed;
	s32 lifetime;
	s32 gravity;
	s8 mode; /* BLAZE_MODE */
} BLAZE_STATE;

/* The frame callback of fdPsynergyBlaze's spray. Every fourth frame while */
/* it is emitting, a free one of sixteen flames starts at the origin with the */
/* mode's speed (BLAZE_LONG_SPRAY faster, longer and lighter) along the */
/* spray's angle. */
/* Each live flame grows, cycles its four tiles, is drawn at its position, and
 */
/* dies after the mode's lifetime, on reaching an object of the event state's */
/* list (whose index is kept and which stops the spray for the frame) or on */
/* something solid; an odd flame that dies leaves one of sixteen puffs of smoke
 */
/* that rise for 32 frames. The angle then turns toward the scene's facing by */
/* at most 1/64 turn a frame. */
void fdBlazeUpdateSpray(void) {
	BLAZE_STATE *state = (BLAZE_STATE *)Ram_HeapSlots->battleEff;
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	EFFECT_SPR *sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	FIELD_PSYNERGY_PARTICLE *puff;
	s32 pos[3];
	s32 i;
	s32 j;
	u16 target;
	s32 turn;

	if(state->emitting != 0 && (FrameCount & 3) == 0) {
		particle = state->flameMotion;
		for(i = 0; i < BLAZE_FLAMES; i++, particle++) {
			if(particle->tmr == -1) {
				sprite = &state->flames[i];
				if(scene->tgt != NULL)
					sprite->depth = fdActorGetSprDepth(scene->tgt);
				else
					sprite->depth = fdActorGetSprDepth(scene->caster);
				if(state->mode == BLAZE_LONG_SPRAY) {
					state->speed = toFIXED(12) / 5;
					state->lifetime = BLAZE_LONG_FLAME_FRAMES;
					state->gravity = toFIXED(3) / 32;
				}
				else {
					state->speed = toFIXED(9) / 5;
					state->lifetime = BLAZE_FLAME_FRAMES;
					state->gravity = toFIXED(1) / 8;
				}
				SET_POSITION(particle, state->origin[0], state->origin[1],
							 state->origin[2]);
				particle->velX = 0;
				particle->velY = -(toFIXED(6) / 5);
				particle->velZ = 0;
				syVectorAddPolarOffset(state->speed, state->ang,
									   &particle->velX);
				particle->tmr = 0;
				break;
			}
		}
	}

	state->hit = 0;
	particle = state->flameMotion;
	for(i = 0; i < BLAZE_FLAMES; i++, particle++) {
		if(particle->tmr == -1)
			continue;
		sprite = &state->flames[i];
		sprite->scaleY = sprite->scaleX =
			particle->tmr * (FIXED_ONE / 25) + FIXED_ONE / 2;
		sprite->oam.idx =
			state->flameTile + (((particle->tmr >> 1) + i) & 3) * 32;
		if(state->ang > DEGtoANG(180))
			sprite->depth--;
		else
			sprite->depth++;
		fdEffectSprDrawAtWorldPos(sprite, particle);
		if(++particle->tmr == state->lifetime)
			particle->tmr = -1;
		for(j = 0; j < FIELD_ACTOR_COUNT; j++) {
			ACTOR *other = field->actors[j];
			if(other != NULL && other->script != NULL &&
			   (other->collisionFlg & ACTOR_SOLID) && other != scene->caster &&
			   other != scene->tgt &&
			   fdPsynergyCheckOverlap(&particle->x, 2, &other->x.fixed,
									  other->radius - 2) >= 0) {
				state->hit = 1;
				state->hitObj = j;
				particle->tmr = -1;
				break;
			}
		}
		if(FIELD_CELL(particle->x) != FIELD_CELL(state->origin[0]) ||
		   FIELD_CELL(particle->z) != FIELD_CELL(state->origin[2])) {
			pos[0] = particle->x;
			pos[1] = particle->y;
			pos[2] = particle->z;
			if(fdMapGridCheckCollisionStub(scene->caster, pos) > 0)
				particle->tmr = -1;
		}
		if(particle->tmr == -1 && (i & 1)) {
			pos[0] = particle->x;
			pos[1] = particle->y - (syRandom() << 2);
			pos[2] = particle->z;
			syVectorAddPolarOffset(toFIXED(10), state->ang + DEGtoANG(180),
								   pos);
			syVectorAddPolarOffset(toFIXED(12), syRandom(), pos);
			puff = &state->puffMotion[i];
			SET_POSITION(puff, pos[0], pos[1], pos[2]);
			puff->tmr = 0;
		}
		if(state->hit != 0)
			break;
		fdAdvanceWithGravity3D(&particle->x, 64, state->gravity);
	}

	particle = state->puffMotion;
	for(i = 0; i < BLAZE_FLAMES; i++, particle++) {
		if(particle->tmr >= 0) {
			sprite = &state->puffs[i];
			sprite->oam.idx = state->puffTile + ((particle->tmr >> 2) & 7) * 8;
			pos[0] = particle->x;
			pos[1] = particle->y;
			pos[2] = particle->z;
			fdCameraWorldToScr(pos);
			sprite->x = pos[0];
			sprite->y = pos[2];
			fdEffectSprDraw(sprite);
			particle->y += toFIXED(4) / 5;
			if(++particle->tmr == 32)
				particle->tmr = -1;
		}
	}

	target = scene->ang8 - DEGtoANG(90);
	turn = (s16)(state->tgtAng - state->ang);
	if((u16)(state->tgtAng - target) - (u16)(state->ang - target) ==
	   TRIG_TURN / 2) {
		state->ang = state->ang + TRIG_TURN / 64;
	}
	else {
		if(turn > TRIG_TURN / 64)
			turn = TRIG_TURN / 64;
		if(turn < -(TRIG_TURN / 64))
			turn = -(TRIG_TURN / 64);
		state->ang = state->ang + turn;
	}
}

#undef CYCLONE_WHIRLWIND_RISE_FRAMES
#undef CYCLONE_WHIRL_FRAMES
#undef CYCLONE_SPIN_FRAMES
#undef WHIRLWIND_RESIZE_STEP
#undef WHIRLWIND_RESIZE_FRAMES
#undef ACTOR_RESIZE_FRAMES
#undef DEPART_FRAMES
#undef CYCLONE_SPREAD_KEPT
#undef CYCLONE_SPREAD_SCALE
#undef BLAZE_LONG_FLAME_FRAMES
#undef BLAZE_FLAME_FRAMES

/* Blaze cast on a target: runs fdPsynergyBlaze. */
void fdPsynergyBlazeOnTarget(void) { fdPsynergyBlaze(); }
