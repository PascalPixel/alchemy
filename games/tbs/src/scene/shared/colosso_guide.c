/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
   what a stage's guide uses to describe the stage. Isaac appears on the stage
   to show how it is crossed, a blinking pointer moves over the screen, and
   Isaac is put back where he stood. */

/* The sprite outline's colours: the field's own, as field_actors.c sets them
   up, and those of the warrior a stage's guide shows. */
#define FIELD_OUTLINE_EDGE	 15
#define FIELD_OUTLINE_FILL	 0
#define WARRIOR_OUTLINE_EDGE 1
#define WARRIOR_OUTLINE_FILL 4

/* Isaac (id) appears at pixel (x, z) on the stage, facing south: he fades in
   through the backgrounds while he stretches from thin and tall to his own
   shape, settles and nods. While he is shown his sprite is outlined in the
   warrior's colours. His position and facing are kept for
   scColossoRestoreWarrior. */
void scColossoFadeInWarrior(Sint32 id, Sint32 x, Sint32 z)
{
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *actor;
	SPR				   *spr;
	Sint32				i;

	system = ActorSystemStateWork;
	actor = fdActorGetById(id);
	system->edge = WARRIOR_OUTLINE_EDGE;
	system->fill = WARRIOR_OUTLINE_FILL;
	ColossoWarriorStartX = actor->x.fixed;
	ColossoWarriorStartZ = actor->z.fixed;
	spr = (SPR *)actor->spr;
	ColossoWarriorStartAngle = actor->facing;
	fdActorSetSprPriority(id, 2);
	actor->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	actor->facing = FACING_SOUTH;
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW | SPRITE_FLAG_OUTLINED);
	fdActorSelectAnimation(actor, ANIM_STILL);
	fdActorSelectAnimation(actor, ANIM_STAND);
	fdActorSetPosFX(id, toFIXED(x), toFIXED(z));
	/* BUG: FACING_SOUTH is passed where an actor id belongs; no actor has it,
	   so the leader does not turn. */
	fdActorFaceActorById(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
		BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3,
		IO_WRITE_HALFWORD);
	spr->image.objMode = OBJ_MODE_SEMI_TRANSPARENT;
	spr->shadow.objMode = OBJ_MODE_SEMI_TRANSPARENT;
	sySndPlay(SE_COLOSSO_WARRIOR_APPEAR);
	for(i = 0; i < BLEND_WEIGHT_MAX; i += 2) {
		actor->scaleX = i * (toFIXED(1) / 16) + toFIXED(1) / 16;
		actor->scaleY = toFIXED(31) / 16 - i * (toFIXED(1) / 16);
		/* BLEND_ALPHA(i + 1, BLEND_WEIGHT_MAX - 1 - i), its bottom weight
		   written first. */
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
			((BLEND_WEIGHT_MAX - 1 - i) << 8) | (i + 1), IO_WRITE_HALFWORD);
		sySynch(1);
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA, BLEND_ALPHA(BLEND_WEIGHT_MAX, 0),
		IO_WRITE_HALFWORD);
	actor->scaleX = toFIXED(17) / 16;
	actor->scaleY = toFIXED(15) / 16;
	fdScriptWait(1);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	fdScriptWait(13);
	spr->image.objMode = OBJ_MODE_NORMAL;
	spr->shadow.objMode = OBJ_MODE_NORMAL;
	fdActorSetAnimationAndWait(id, ANIM_NOD);
	fdScriptWait(20);
}

/* Puts Isaac (id) back where scColossoFadeInWarrior found him: he stands if he
   was climbing, or else turns south, nods and waits half a second; then returns
   to his kept position and facing with his motion stopped, and the outline
   colours are the field's again. */
void scColossoRestoreWarrior(Sint32 id)
{
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *actor;

	system = ActorSystemStateWork;
	actor = fdActorGetById(id);
	if(MainRoutineStateWork.movementMode == 1) { /* climbing */
		MainRoutineStateWork.movementMode = 0;
		fdActorSetAnimation(id, ANIM_STAND);
	} else {
		fdActorFaceDirection(id, FACING_SOUTH, 30);
		fdActorSetAnimation(id, ANIM_NOD);
		fdScriptWait(30);
	}
	system->fill = FIELD_OUTLINE_FILL;
	system->edge = FIELD_OUTLINE_EDGE;
	actor->x.fixed = ColossoWarriorStartX;
	actor->z.fixed = ColossoWarriorStartZ;
	actor->facing = ColossoWarriorStartAngle;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->velX = 0;
	actor->velZ = 0;
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->layer = 0;
	actor->y.fixed = 0;
	actor->groundHeight = 0;
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	fdActorSelectAnimation(actor, ANIM_STILL);
	fdActorSelectAnimation(actor, ANIM_STAND);
	sySynch(1);
}

/* Stops the actor where it is: no velocity and no destination. */
void scActorResetMotionFields(Sint32 actorId)
{
	ACTOR *actor = fdActorGetById(actorId);

	fdActorStartIdle(actor);
	actor->velX = 0;
	actor->velZ = 0;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
}

/* The guide's pointer: one 16x16 sprite centred on a point of the screen, shown
   for POINTER_SHOWN_TICKS of every POINTER_BLINK_TICKS, that can glide from
   point to point. */
#define POINTER_SHOWN_TICKS 13
#define POINTER_BLINK_TICKS 20

/* Loads the pointer's picture into a free VRAM block, once. */
static void scColossoLoadPointer(void)
{
	if(ColossoPointerSlot == -1) {
		ColossoPointerSlot = mnPointingHandLoadIntoFreeSlot();
	}
}

/* The pointer's glide: its steps and the step it is on, where it is now, where
   the glide started and where it ends (screen pixels), its blink counter and
   its flip. */
extern Sint16			  ColossoPointerSteps;
extern Sint16			  ColossoPointerStep;
extern Sint16			  ColossoPointerX;
extern POINTER_COORDINATE ColossoPointerStartX;
extern Sint16			  ColossoPointerEndX;
extern Sint16			  ColossoPointerY;
extern POINTER_COORDINATE ColossoPointerStartY;
extern Sint16			  ColossoPointerEndY;
extern Sint16			  ColossoPointerBlink;
extern Sint16			  ColossoPointerFlip;

/* The pointer's event: glide one step, then draw it while it blinks on (a glide
   keeps it on). */
static void scColossoUpdatePointer(void)
{
	Sint32	tile;
	Sint32	total;
	Sint32	step;
	Sint32 *oam;
	Sint32 *dst;
	Sint32	x;
	Sint32	y;

	oam = ColossoPointerOam;
	tile = VramCacheEntries[ColossoPointerSlot].ofs >> 5;
	total = ColossoPointerSteps;
	if(total != 0) {
		step = ++ColossoPointerStep;
		ColossoPointerX = ColossoPointerStartX.pixels +
			(ColossoPointerEndX - ColossoPointerStartX.signedPixels) *
				(Sint16)step / total;
		ColossoPointerY = ColossoPointerStartY.pixels +
			(ColossoPointerEndY - ColossoPointerStartY.signedPixels) *
				(Sint16)step / total;
		if((Sint16)step >= total)
			ColossoPointerSteps = 0;
		ColossoPointerBlink = 0;
	}
	if(++ColossoPointerBlink <= POINTER_SHOWN_TICKS) {
		Sint32 size = OBJ_SIZE_16X16;

		x = ColossoPointerX;
		y = ColossoPointerY;
		dst = oam;
		*dst++ = 0;
		*dst++ = (y - 8) | OBJ_X(x - 8) | size | ColossoPointerFlip * OBJ_HFLIP;
		*dst = tile | OBJ_PRIORITY(1);
		syOAMAddSprAtDepth(oam, OAM_DEPTH_FRONT);
	} else if(ColossoPointerBlink >= POINTER_BLINK_TICKS) {
		ColossoPointerBlink = 0;
	}
}

/* Shows the pointer at screen pixel (x, y), mirrored by style & 3 (1 left to
   right, 2 top to bottom); every caller passes 0. */
void scColossoShowPointer(Uint32 x, Uint32 y, Uint32 style)
{
	scColossoLoadPointer();

	ColossoPointerX = x;
	ColossoPointerY = y;
	ColossoPointerFlip = (style & 3);
	ColossoPointerBlink = 0;
	ColossoPointerSteps = 0;

	sySetTickEvent(scColossoUpdatePointer, EVENT_PASS_TICK);
}

/* Glides the pointer to screen pixel (x, y) over duration ticks. */
void scColossoMovePointer(Uint32 x, Uint32 y, Uint32 duration)
{
	ColossoPointerEndX = x;
	ColossoPointerEndY = y;
	*(Uint16 *)&ColossoPointerStartX = ColossoPointerX;
	*(Uint16 *)&ColossoPointerStartY = ColossoPointerY;
	ColossoPointerSteps = duration;
	ColossoPointerStep = 0;

	sySetTickEvent(scColossoUpdatePointer, EVENT_PASS_TICK);
}

/* Hides the pointer and frees its VRAM block. */
void scColossoHidePointer(void)
{
	syCloseEvent(scColossoUpdatePointer);
	syVRAMCacheFreeEntry(ColossoPointerSlot);
	ColossoPointerSlot = -1;
}

/* Runs the actor at 2 pixels a tick towards pixel (x, z) and returns at
   once. */
void scColossoWalkActorTo(Sint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		FIXED  speed = toFIXED(2);
		Sint32 unpaused = 0;

		actor->speedLimit = speed;
		actor->acceleration = speed >> 1;
		actor->paused = unpaused;
		fdActorStartIdle(actor);
		fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
		fdActorSetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* Runs the actor at 1.25 pixels a tick to pixel (x, z), waits until it arrives
   and stands it there. */
void scColossoWalkActorToAndStand(Sint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		FIXED  speed = toFIXED(5) / 4;
		Sint32 unpaused = 0;

		actor->speedLimit = speed;
		actor->acceleration = speed >> 1;
		actor->paused = unpaused;
		fdActorStartIdle(actor);
		fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
		fdActorSetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMove(actor);
		fdActorSelectAnimation(actor, ANIM_STAND);
	}
}
