/* Cyclone and the whirlwinds: the 96-particle burst, the leader carried or
   lifted to a Cyclone metatile, Cyclone itself, an actor leaving or arriving in
   a whirlwind (fdActorDepartInRing, fdActorArriveInRing), then Blaze's jet of
   flame and the smoke it leaves. Part of field_psynergy.c. */

/* The 96-particle burst's event (PARTICLE_BURST_STATE in
   HEAP_SLOT_BATTLE_EFFECT, started by fdParticleBurstStart). Each particle
   whose delay counter is between 0 and 63 shows the picture (delay / 8) % 6 of
   the burst's tiles 48 pixels out along its heading, then turns by its spin,
   rises by its speed and spins eight units faster; every counter advances. The
   state's flash flag is set on tick 30 only, and the burst stops being active
   on tick 112. */
void fdParticleBurstUpdate(void)
{
	PARTICLE_BURST_STATE *state;
	BURST_PARTICLE		 *particle;
	EFFECT_SPR			 *spr;
	Sint32				  pos[3];
	Sint32				  i;

	state = (PARTICLE_BURST_STATE *)RamHeapSlots->battleEff;
	particle = state->particles;

	for(i = 0; i < PARTICLE_BURST_COUNT; i++, particle++) {
		spr = &state->sprs[i];
		if((Uint32)particle->delay < 64) {
			spr->oam.idx = state->tile + (particle->delay >> 3) % 6 * 2;
			pos[0] = particle->x;
			pos[1] = particle->y;
			pos[2] = particle->z;
			syVectorAddPolarOffset(toFIXED(48), particle->heading, pos);
			fdCameraWorldToScreen(pos);
			spr->x = pos[0];
			spr->y = pos[2];
			fdEffectSprDraw(spr);
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

/* Ends the particle burst (fdParticleBurstStart): stops its event and frees its
   tiles. */
void fdParticleBurstEnd(void)
{
	PARTICLE_BURST_STATE *state = RamHeapSlots->battleEff;

	syCloseEvent((Uint32)fdParticleBurstUpdate);
	syVRAMCacheFreeEntry(state->cacheEntry);
}

/* The ticks the whirlwind takes to carry or lift the leader. */
#define CYCLONE_CARRY_TICKS 128

/* How far the whirlwind turns the leader round each tick of the ride. */
#define CYCLONE_CARRY_TURN_STEP (TRIG_TURN * 5 / 256)

/* Event of the Cyclone: the whirlwind carries the leader over 128 ticks from
   where it stood to the nearby CYCLONE_METATILE_CARRY metatile (a Cyclone
   teleporter), circling and arcing up and down on the way, its follower with
   it. */
void fdCycloneCarryLeader(void)
{
	FIELD_EFFECT_STATE *effect = RamHeapSlots->fieldEffState;
	CYCLONE_STATE	   *state = RamHeapSlots->battleEff;
	ACTOR			   *owner = effect->caster;
	Sint32				tmr = state->tmr;
	Sint32				pos[3];

	switch(state->state) {
	case 0:
		state->radius = sySin(tmr * (TRIG_TURN / 2 / CYCLONE_CARRY_TICKS)) << 5;
		state->ang += CYCLONE_CARRY_TURN_STEP;
		pos[0] = state->from[0] +
			(state->to[0] - state->from[0]) * tmr / CYCLONE_CARRY_TICKS;
		pos[1] = state->from[1] +
			(state->to[1] - state->from[1]) * tmr / CYCLONE_CARRY_TICKS +
			(sySin(tmr * (TRIG_TURN / 2 / CYCLONE_CARRY_TICKS)) << 6);
		pos[2] = state->from[2] +
			(state->to[2] - state->from[2]) * tmr / CYCLONE_CARRY_TICKS;
		syVectorAddPolarOffset(state->radius, state->ang, pos);
		SET_ACTOR_POSITION(owner, pos[0], pos[1], pos[2]);
		if(tmr >= CYCLONE_CARRY_TICKS) {
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

/* fdCycloneCarryLeader for a CYCLONE_METATILE_LIFT metatile: the leader is
   carried there 32 pixels up, circling, without the arc. */
void fdCycloneLiftLeader(void)
{
	FIELD_EFFECT_STATE *effect = RamHeapSlots->fieldEffState;
	CYCLONE_STATE	   *state = RamHeapSlots->battleEff;
	ACTOR			   *owner = effect->caster;
	Sint32				tmr = state->tmr;
	Sint32				pos[3];

	switch(state->state) {
	case 0:
		state->radius = sySin(tmr * (TRIG_TURN / 2 / CYCLONE_CARRY_TICKS)) << 5;
		state->ang += CYCLONE_CARRY_TURN_STEP;
		pos[0] = state->from[0] +
			(state->to[0] - state->from[0]) * tmr / CYCLONE_CARRY_TICKS;
		pos[1] = state->from[1] +
			(state->to[1] - state->from[1]) * tmr / CYCLONE_CARRY_TICKS;
		pos[2] = state->from[2] +
			(state->to[2] - state->from[2]) * tmr / CYCLONE_CARRY_TICKS;
		syVectorAddPolarOffset(state->radius, state->ang, pos);
		SET_ACTOR_POSITION(owner, pos[0], pos[1], pos[2]);
		if(tmr >= CYCLONE_CARRY_TICKS) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case 1:
		owner->y.fixed -= toFIXED(2) / 5;
		if(effect->sceneVariant == 1) {
			if(tmr == 40)
				state->state = FIELD_PSYNERGY_FINISHED;
		} else if(tmr == 40) {
			state->state++;
			state->tmr = -1;
		}
		break;
	case 2:
		owner->y.fixed += toFIXED(19) / 5;
		if(tmr == 40)
			state->state = FIELD_PSYNERGY_FINISHED;
		break;
	}
	state->tmr++;
	state->follower->x.fixed = owner->x.fixed;
	state->follower->y.fixed = owner->y.fixed;
	state->follower->z.fixed = owner->z.fixed;
}

/* Per-tick update: turns an actor 1/16 turn a tick. */
void fdActorUpdateSpin(MOTION_OBJECT *object)
{
	object->ang += TRIG_TURN / 16;
}

/* Cyclone cast on a target: runs fdPsynergyCyclone. */
void fdPsynergyCycloneOnTarget(void)
{
	fdPsynergyCyclone();
}

/* The ticks the Cyclone's whirlwind takes to rise to full height, and the
   ticks its whirl of wind is drawn for, then whirls on as the Cyclone acts. */
#define CYCLONE_WHIRLWIND_RISE_TICKS 20
#define CYCLONE_WHIRL_TICKS			 60
#define CYCLONE_SPIN_TICKS			 16
/* A whirlwind widens or narrows by WHIRLWIND_RESIZE_STEP of its width a tick,
   and an actor by the same of its size: the ticks for the whole whirlwind and
   for half an actor. */
#define WHIRLWIND_RESIZE_STEP		 (toFIXED(1) / 32)
#define WHIRLWIND_RESIZE_TICKS		 32
#define ACTOR_RESIZE_TICKS			 16
/* The whirl's arcs spread by a step that keeps CYCLONE_SPREAD_KEPT of every
   CYCLONE_SPREAD_SCALE parts of itself each tick. */
#define CYCLONE_SPREAD_KEPT			 123
#define CYCLONE_SPREAD_SCALE		 128

/* Cyclone ("Conjure wind to scatter weeds."): the caster spins in a whirlwind
   of blue and white drawn on the canvas sprite, and the weeds on the metatiles
   round it (fdPsynergyGatherNearbyMetatiles) are scattered. */
void fdPsynergyCyclone(void)
{
	CYCLONE_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(CYCLONE_STATE));
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	FIELD_EFFECT_STATE *effect = RamHeapSlots->fieldEffState;
	ACTOR			   *owner = effect->caster;
	Sint16			   *points = &field->psynergyMetatiles[0];
	FIXED				spread = toFIXED(4);
	Sint32				step = toFIXED(7) / 8;
	CANVAS_SPR_STATE   *canvas;
	ACTOR			   *whirlwind;
	Uint16			   *offsets;
	Uint8			   *pixels;
	Uint16			   *palette;
	FIXED				pos[3];
	Sint32				mode;
	Sint32				frm;
	Sint32				i, j;
	Sint32				blue, c;
	Sint32				point;

	fdPsynergyFadeToCastCol();
	fdPsynergyGatherNearbyMetatiles();
	fdBattleEffectSetObjectUpdateMode(effect->caster, CASTER_LOOK_NORMAL);
	owner->update = fdActorUpdateSpin;
	owner->tmr = owner->facing;
	/* The whirl's colours: blue full, red and green rising from half to
	   full. */
	palette = (Uint16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
	blue = RGB(0, 0, RGB_LEVEL_MAX);
	for(j = 0; j < PALETTE_COLORS; j++) {
		c = j + (RGB_LEVEL_MAX + 1) / 2;
		syIOWrite16(RGB(0, c, 0) | blue | c, palette);
		palette++;
	}
	fdBattleEffectStartCanvasSpr();
	canvas = RamHeapSlots->canvasSpr;
	pos[0] = effect->caster->x.fixed;
	pos[1] = effect->caster->y.fixed;
	pos[2] = effect->caster->z.fixed;
	fdCameraWorldToScreen(pos);
	canvas->x = (Sint16)FIXEDtoINT(pos[0]);
	canvas->y = (Sint16)FIXEDtoINT(pos[2]);
	canvas->pri = 1;
	canvas->depth = OAM_DEPTH_WINDOW;
	canvas->mode = CANVAS_SPRITE_KEPT;
	canvas->scaleX = toFIXED(3) / 2;
	canvas->scaleY = toFIXED(3) / 2;
	fdEffectSprInit(&canvas->spr, 32, 32, OBJ_SIZE_64X64, canvas->tile);
	canvas->spr.oam.fullCol = 1;
	canvas->spr.oam.pal = 0;
	offsets = RamHeapSlots->canvasMap;
	pixels = RamHeapSlots->battleCanvas;
	whirlwind = fdActorSpawn(SPRITE_WHIRLWIND, owner->x.fixed,
		owner->y.fixed - toFIXED(6), owner->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = toFIXED(3) / 2;
	whirlwind->scaleY = 0;
	sySynch(1);
	sySndPlayStub(SE_RUSH);
	for(i = 0; i < CYCLONE_WHIRLWIND_RISE_TICKS; i++) {
		whirlwind->scaleY = toFIXED(1) * i / CYCLONE_WHIRLWIND_RISE_TICKS;
		sySynch(1);
	}
	for(i = 0; i < CYCLONE_WHIRL_TICKS; i++) {
		for(j = 0; j < 4; j++)
			fdCyclonePlotArc(
				FIXEDtoINT((Sint32)(spread + syRandom() * 4 - toFIXED(j))),
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
		mode = CYCLONE_METATILE_LIFT;
	else if(field->cycloneCarryX != -1)
		mode = CYCLONE_METATILE_CARRY;
	else {
		point = points[0];
		mode = point != -1;
	}

	if(mode == CYCLONE_METATILE_LIFT) {
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_TICKS; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorSetPosAndResetMotionStub(
			owner, owner->x.fixed, owner->y.fixed, owner->z.fixed);
		fdActorSetSprFlagsStub(owner, 0);
		fdActorSetSprPriority(effect->casterId, 1);
		owner->motionFlg = 0;
		frm = 0;
		sySynch(1);
		state->to[0] = toFIXED(field->cycloneLiftX);
		state->to[1] = owner->y.fixed + toFIXED(32);
		state->to[2] = toFIXED(field->cycloneLiftZ);
		sySetEvent(fdCycloneLiftLeader, EVENT_PASS_TICK);
		while(state->state != FIELD_PSYNERGY_FINISHED) {
			if(frm == 30)
				fdPsynergyRunTouchedScripts();
			frm++;
			sySynch(1);
		}
		syCloseEvent(fdCycloneLiftLeader);
		if(effect->sceneVariant == 0)
			owner->scaleX = 0;
		whirlwind->scaleX = 0;
		if(state->kind)
			fdParticleBurstEnd();
	} else if(mode == CYCLONE_METATILE_CARRY) {
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_TICKS; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdActorSetPosAndResetMotionStub(
			owner, owner->x.fixed, owner->y.fixed, owner->z.fixed);
		fdActorSetSprFlagsStub(owner, 0);
		owner->motionFlg = 0;
		sySynch(1);
		state->to[0] = toFIXED(field->cycloneCarryX);
		state->to[1] = owner->y.fixed;
		state->to[2] = toFIXED(field->cycloneCarryZ);
		sySetEvent(fdCycloneCarryLeader, EVENT_PASS_TICK);
		frm = 0;
		while(state->state != FIELD_PSYNERGY_FINISHED) {
			if(frm == 30)
				fdPsynergyRunTouchedScripts();
			frm++;
			sySynch(1);
		}
		syCloseEvent(fdCycloneCarryLeader);
		for(i = 0; i < ACTOR_RESIZE_TICKS; i++) {
			owner->scaleX += -WHIRLWIND_RESIZE_STEP;
			owner->scaleY = owner->scaleX;
			sySynch(1);
		}
		owner->scaleX = 0;
		for(i = 0; i < WHIRLWIND_RESIZE_TICKS; i++) {
			whirlwind->scaleX += -WHIRLWIND_RESIZE_STEP;
			whirlwind->scaleY += -(toFIXED(1) / 128);
			sySynch(1);
		}
		whirlwind->scaleX = 0;
		if(state->kind)
			fdParticleBurstEnd();
	} else if(mode == CYCLONE_METATILE_BURST) {
		sySndPlayStub(SE_WHOOSH);
		fdActorSelectAnimationStub(whirlwind, 6);
		if(state->kind)
			fdParticleBurstStart();
		for(i = 0; i < CYCLONE_SPIN_TICKS; i++) {
			canvas->ang += TRIG_TURN / 64;
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
			sySynch(1);
		}
		owner->update = NULL;
		owner->facing = owner->tmr;
		frm = 0;
		while(state->cnt != 0) {
			if(frm == 30)
				fdPsynergyRunTouchedScripts();
			sySynch(1);
			frm++;
		}
		if(state->kind)
			fdParticleBurstEnd();
	} else {
		sySndPlayStub(SE_WHOOSH);
		fdActorSelectAnimationStub(whirlwind, 6);
		for(i = 0; i < CYCLONE_SPIN_TICKS; i++) {
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

/* The ticks fdActorDepartInRing's actor circles in its whirlwind. */
#define DEPART_TICKS 128

/* The actor leaves in a whirlwind (SPRITE_WHIRLWIND): as the screen transition
   starts it circles inside it for DEPART_TICKS, from 96 pixels up and 32 out
   down to where it stood, then the whirlwind vanishes with a whoosh and the
   camera follows the actor again. */
void fdActorDepartInRing(Sint32 actorId)
{
	ACTOR *actor = fdActorGetById(actorId);
	Sint32 angle = actor->facing;
	Uint8  motionFlags = actor->motionFlg;
	ACTOR *whirlwind;
	FIXED  x, y, z;
	Sint32 spin;
	FIXED  position[3];
	Sint32 i;

	fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	actor->motionFlg = 0;
	actor->update = fdActorUpdateSpin;
	whirlwind = fdActorSpawn(
		SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = toFIXED(3) / 2;
	whirlwind->scaleY = toFIXED(1);
	x = actor->x.fixed;
	y = actor->y.fixed;
	z = actor->z.fixed;
	spin = 0;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	for(i = 0; i < DEPART_TICKS; i++) {
		position[0] = x;
		position[1] = y + syCos(i * (TRIG_TURN / 4 / DEPART_TICKS)) * 96;
		position[2] = z;
		syVectorAddPolarOffset(
			syCos(i * (TRIG_TURN / 4 / DEPART_TICKS)) * 32, spin, position);
		SET_ACTOR_POSITION(actor, position[0], position[1], position[2]);
		spin += TRIG_TURN / 64;
		SET_ACTOR_POSITION(whirlwind, actor->x.fixed,
			actor->y.fixed - toFIXED(6), actor->z.fixed);
		sySynch(1);
	}
	fdActorSetPosAndResetMotionStub(actor, x, y, z);
	sySndPlayStub(SE_WHOOSH);
	fdActorSelectAnimationStub(whirlwind, 6);
	fdScriptWait(15);
	fdActorReleaseStub(whirlwind);
	actor->update = NULL;
	actor->facing = angle;
	actor->motionFlg = motionFlags;
	sySynch(1);
	fdCameraFollowActor(actorId, 1);
}

/* The actor arrives in a whirlwind, fdActorDepartInRing's counterpart: once the
   screen transition is done, a whirlwind (SPRITE_WHIRLWIND) widens on the
   unseen, spinning actor for WHIRLWIND_RESIZE_TICKS, then vanishes with a
   whoosh as the actor grows back from half its size and shows again. */
void fdActorArriveInRing(Sint32 actorId)
{
	ACTOR *actor = fdActorGetById(actorId);
	Sint32 ang = actor->facing;
	ACTOR *whirlwind;
	Sint32 i;

	actor->update = fdActorUpdateSpin;
	actor->scaleX = 0;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSprFlagsStub(fdActorGetById(actorId), 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	whirlwind = fdActorSpawn(
		SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	fdActorSelectAnimationStub(whirlwind, 2);
	whirlwind->scaleX = toFIXED(1) / 2;
	whirlwind->scaleY = toFIXED(3) / 4;
	for(i = 0; i < WHIRLWIND_RESIZE_TICKS; i++) {
		whirlwind->scaleX += WHIRLWIND_RESIZE_STEP;
		whirlwind->scaleY += toFIXED(1) / 128;
		sySynch(1);
	}
	whirlwind->scaleX = toFIXED(3) / 2;
	whirlwind->scaleY = toFIXED(1);
	sySndPlayStub(SE_WHOOSH);
	fdActorSelectAnimationStub(whirlwind, 6);
	actor->scaleY = actor->scaleX = toFIXED(1) / 2;
	for(i = 0; i < ACTOR_RESIZE_TICKS; i++) {
		actor->scaleY = actor->scaleX = actor->scaleX + WHIRLWIND_RESIZE_STEP;
		sySynch(1);
	}
	fdActorSetSprFlagsStub(fdActorGetById(actorId), SPRITE_FLAG_SHADOW);
	fdScriptWait(10);
	fdActorReleaseStub(whirlwind);
	actor->update = NULL;
	actor->facing = ang;
}

/* What Blaze does (BLAZE_STATE.mode). */
typedef enum BLAZE_MODE {
	BLAZE_FLICKER = 0, /* cast at nothing with no torch in reach: a flicker */
	/* cast at nothing: lights the nearest torch in reach */
	BLAZE_LIGHT_TORCH = 1,
	BLAZE_SPRAY = 2, /* cast at a target: the jet of flame */
	/* The jet, faster and longer (FIELD_STATE.blazeLongSpray). */
	BLAZE_LONG_SPRAY = 3
} BLAZE_MODE;

/* The flames of Blaze's jet, and the smoke puffs they leave. */
#define BLAZE_FLAMES		   16
/* The ticks a flame of the jet lives, in BLAZE_LONG_SPRAY and otherwise. */
#define BLAZE_LONG_FLAME_TICKS 33
#define BLAZE_FLAME_TICKS	   25

/* Blaze's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the flame tiles, 16 flames
   and 16 smoke puffs with their motion, where the jet starts and where it
   points, and the actor it has reached. */
typedef struct BLAZE_STATE {
	Uint8					unused[4096]; /* no code names it */
	EFFECT_SPR				flames[BLAZE_FLAMES];
	FIELD_PSYNERGY_PARTICLE flameMotion[BLAZE_FLAMES];
	EFFECT_SPR				puffs[BLAZE_FLAMES];
	FIELD_PSYNERGY_PARTICLE puffMotion[BLAZE_FLAMES];
	Sint32					origin[3];
	Uint16					flameTile;
	Uint16					puffTile;
	Uint16					ang;
	Uint16					tgtAng;
	Sint16					hitObj;
	Sint8					hit;
	Sint8					emitting;
	Sint32					speed;
	Sint32					lifetime;
	Sint32					gravity;
	Sint8					mode; /* BLAZE_MODE */
} BLAZE_STATE;

/* The tick callback of fdPsynergyBlaze's spray. Every fourth tick while it is
   emitting, a free one of sixteen flames starts at the origin with the mode's
   speed (BLAZE_LONG_SPRAY faster, longer and lighter) along the spray's angle.
   Each live flame grows, cycles its four tiles, is drawn at its position, and
   dies after the mode's lifetime, on reaching an object of the script state's
   list (whose index is kept and which stops the spray for the tick) or on
   something solid; an odd flame that dies leaves one of sixteen puffs of smoke
   that rise for 32 ticks. The angle then turns toward the scene's facing by at
   most 1/64 turn a tick. */
void fdBlazeUpdateSpray(void)
{
	BLAZE_STATE				*state = (BLAZE_STATE *)RamHeapSlots->battleEff;
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	FIELD_PSYNERGY_PARTICLE *puff;
	Sint32					 pos[3];
	Sint32					 i;
	Sint32					 j;
	Uint16					 tgt;
	Sint32					 turn;

	if(state->emitting != 0 && (TickCount & 3) == 0) {
		particle = state->flameMotion;
		for(i = 0; i < BLAZE_FLAMES; i++, particle++) {
			if(particle->tmr == -1) {
				spr = &state->flames[i];
				if(scene->tgt != NULL)
					spr->depth = fdActorGetSprDepth(scene->tgt);
				else
					spr->depth = fdActorGetSprDepth(scene->caster);
				if(state->mode == BLAZE_LONG_SPRAY) {
					state->speed = toFIXED(12) / 5;
					state->lifetime = BLAZE_LONG_FLAME_TICKS;
					state->gravity = toFIXED(3) / 32;
				} else {
					state->speed = toFIXED(9) / 5;
					state->lifetime = BLAZE_FLAME_TICKS;
					state->gravity = toFIXED(1) / 8;
				}
				SET_POSITION(particle, state->origin[0], state->origin[1],
					state->origin[2]);
				particle->velX = 0;
				particle->velY = -(toFIXED(6) / 5);
				particle->velZ = 0;
				syVectorAddPolarOffset(
					state->speed, state->ang, &particle->velX);
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
		spr = &state->flames[i];
		spr->scaleY = spr->scaleX =
			particle->tmr * (toFIXED(1) / 25) + toFIXED(1) / 2;
		spr->oam.idx = state->flameTile + (((particle->tmr >> 1) + i) & 3) * 32;
		if(state->ang > DEGtoANG(180))
			spr->depth--;
		else
			spr->depth++;
		fdEffectSprDrawAtWorldPos(spr, particle);
		if(++particle->tmr == state->lifetime)
			particle->tmr = -1;
		for(j = 0; j < FIELD_ACTOR_COUNT; j++) {
			ACTOR *other = field->actors[j];
			if(other != NULL && other->script != NULL &&
				(other->collisionFlg & ACTOR_SOLID) && other != scene->caster &&
				other != scene->tgt &&
				fdPsynergyCheckOverlap(
					&particle->x, 2, &other->x.fixed, other->radius - 2) >= 0) {
				state->hit = 1;
				state->hitObj = j;
				particle->tmr = -1;
				break;
			}
		}
		if(FIELD_METATILE(particle->x) != FIELD_METATILE(state->origin[0]) ||
			FIELD_METATILE(particle->z) != FIELD_METATILE(state->origin[2])) {
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
			syVectorAddPolarOffset(
				toFIXED(10), state->ang + DEGtoANG(180), pos);
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
			spr = &state->puffs[i];
			spr->oam.idx = state->puffTile + ((particle->tmr >> 2) & 7) * 8;
			pos[0] = particle->x;
			pos[1] = particle->y;
			pos[2] = particle->z;
			fdCameraWorldToScreen(pos);
			spr->x = pos[0];
			spr->y = pos[2];
			fdEffectSprDraw(spr);
			particle->y += toFIXED(4) / 5;
			if(++particle->tmr == 32)
				particle->tmr = -1;
		}
	}

	tgt = scene->ang8 - DEGtoANG(90);
	turn = (Sint16)(state->tgtAng - state->ang);
	if((Uint16)(state->tgtAng - tgt) - (Uint16)(state->ang - tgt) ==
		TRIG_TURN / 2) {
		state->ang = state->ang + TRIG_TURN / 64;
	} else {
		if(turn > TRIG_TURN / 64)
			turn = TRIG_TURN / 64;
		if(turn < -(TRIG_TURN / 64))
			turn = -(TRIG_TURN / 64);
		state->ang = state->ang + turn;
	}
}

#undef CYCLONE_WHIRLWIND_RISE_TICKS
#undef CYCLONE_WHIRL_TICKS
#undef CYCLONE_SPIN_TICKS
#undef WHIRLWIND_RESIZE_STEP
#undef WHIRLWIND_RESIZE_TICKS
#undef ACTOR_RESIZE_TICKS
#undef DEPART_TICKS
#undef CYCLONE_SPREAD_KEPT
#undef CYCLONE_SPREAD_SCALE
#undef BLAZE_LONG_FLAME_TICKS
#undef BLAZE_FLAME_TICKS

/* Blaze cast on a target: runs fdPsynergyBlaze. */
void fdPsynergyBlazeOnTarget(void)
{
	fdPsynergyBlaze();
}
