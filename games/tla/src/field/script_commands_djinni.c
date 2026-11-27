/* A Djinni's battle start and its joining the party, the field effect sprites'
   slots, and a Jupiter Djinni's light. Part of script_commands.c. */

#include "game/stubs.h"

#define DJINNI_AFTERIMAGES 8 /* afterimages of the leaping Djinni */

/* The ticks a Jupiter Djinni's light widens, and later closes in over, so that
   it ends where it began; and the ticks it circles and drifts. */
#define DJINNI_LIGHT_RADIUS_TICKS	60
#define DJINNI_LIGHT_CIRCLING_TICKS 40
#define DJINNI_LIGHT_DRIFTING_TICKS 40

/* The states of a Jupiter Djinni's light particle, in order. */
typedef enum DJINNI_LIGHT_STATE {
	DJINNI_LIGHT_WIDENING = 0, /* circles as its radius grows */
	DJINNI_LIGHT_CIRCLING = 1,
	DJINNI_LIGHT_DRIFTING = 2, /* its centre drifts toward the leader */
	DJINNI_LIGHT_CLOSING = 3,  /* circles as its radius shrinks */
	DJINNI_LIGHT_DONE = 4	   /* frees its slot */
} DJINNI_LIGHT_STATE;

/* Per-tick update: spins an actor 1/8 turn a tick. */
void fdEffectAdvanceSpinAng(ACTOR *actor)
{
	actor->facing += DEGtoANG(45);
}

/* Per-tick update: halves the actor's distance to its watched actor along x, y
   and z. */
void fdActorMoveHalfwayTowardTarget(ACTOR *actor)
{
	ACTOR *tgt = (ACTOR *)actor->watchedActor;

	actor->x.fixed += (tgt->x.fixed - actor->x.fixed) / 2;
	{
		Sint32 pos = actor->y.fixed;

		pos += (tgt->y.fixed - pos) / 2;
		actor->y.fixed = pos;
	}
	{
		FIXED pos = actor->z.fixed;

		pos += (tgt->z.fixed - pos) / 2;
		actor->z.fixed = pos;
	}
}

/* The sound of the Djinni's leap: SE_WHOOSH in the Japanese edition,
   SE_HIGH_JUMP from it. */
#define LEAP_SOUND WHEN(DJINNI_LEAP_HIGH_JUMP_SOUND, SE_HIGH_JUMP, SE_WHOOSH)

/* A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
   the leader with an anger vein over it, jumps three times where it stands,
   spinning and each time higher, then leaps 16 pixels at the leader trailing
   eight afterimages tinted for its element, and stops once it has come down
   within 20 pixels of the leader's height. */
void fdDjinnRunBattleStartHop(Sint32 djinniId)
{
	ACTOR		 *djinni;
	ACTOR		 *leader;
	ACTOR		 *copy;
	ACTOR		 *prev;
	void		 *sharedSpr;
	Sint32		  dir;
	Sint32		  back;
	Sint32		  spr;
	Sint32		  tint;
	Sint32		  i;
	UNUSED ACTOR *copies[DJINNI_AFTERIMAGES];

	djinni = fdActorGetById(djinniId);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	dir = (leader->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	fdScriptStart();
	sySynch(10);
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySynch(10);
	back = dir + DEGtoANG(180);
	fdActorFaceDirectionById(djinniId, back, 0);
	sySynch(10);
	djinni->facing = back;
	fdActorShowEmoteById(djinniId, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	fdActorGetById(djinniId)->steeringFlg &= ~ACTOR_FACES_MOTION;
	djinni->motionFlg = ACTOR_FALLS;
	djinni->update = (OBJECT_UPDATE)fdEffectAdvanceSpinAng;
#if DJINNI_SPIN_JUMP_SOUNDS
	sySndPlayStub(SE_JUMP);
#endif
	djinni->gravity = toFIXED(1) / 4;
	djinni->velY = toFIXED(9) / 2;
	sySynch(33);
	((SPR *)djinni->spr)->image.pri = 0;
#if DJINNI_SPIN_JUMP_SOUNDS
	sySndPlayStub(SE_JUMP);
#endif
	djinni->velY = toFIXED(4.8);
	sySynch(35);
#if DJINNI_SPIN_JUMP_SOUNDS
	sySndPlayStub(SE_JUMP);
#endif
	djinni->velY = toFIXED(4.9);
	sySynch(38);

	djinni->update = NULL;
	djinni->facing = back;
	/* Afterimages of the same sprite at AFTERIMAGE_SCALE, each closing half the
	   distance to the one before it every tick, tinted by the Djinni's
	   sprite. */
	spr = ((SPR *)djinni->spr)->entries[0]->anmId;
	tint = SPRITE_COLOR_BLUE_TINT;
	if(spr == SPRITE_VENUS_DJINNI)
		tint = SPRITE_COLOR_RBG;
	if(spr == SPRITE_MARS_DJINNI)
		tint = SPRITE_COLOR_RED_TINT;
	/* DEAD: tint is already SPRITE_COLOR_BLUE_TINT for SPRITE_MERCURY_DJINNI;
	   the ROM keeps the test and the store. */
	if(spr == SPRITE_MERCURY_DJINNI)
		tint = SPRITE_COLOR_BLUE_TINT;
	sharedSpr = NULL;
	prev = djinni;
	for(i = 0; i < DJINNI_AFTERIMAGES; i++) {
		copy = fdActorCreateStub(
			spr, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
		/* DEAD: nothing reads copies; the ROM keeps the store and its slot. */
		copies[i] = copy;
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->motionFlg = 0;
			copy->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			copy->steeringFlg |= ACTOR_FACES_MOTION;
			copy->update = fdActorMoveHalfwayTowardTarget;
			copy->facing = djinni->facing;
			((SPR *)copy->spr)->image.pri = 0;
			fdActorSetSprColModeStub(copy, tint);
			fdActorSelectAnimationStub(copy, ANIM_STILL);
			fdActorSetSprFlagsStub(copy, 0);
			sharedSpr = fdSprShareVRAMCacheEntry(copy->spr, sharedSpr);
			copy->watchedActor = (union FIELD_OBJECT *)prev;
			prev = copy;
		}
	}

	djinni->speedLimit = toFIXED(0.3);
	djinni->acceleration = toFIXED(0.3);
	fdActorSetTargetPosFromMagnitudeAng(
		djinni, toFIXED(16), dir + DEGtoANG(180));
	sySndPlayStub(LEAP_SOUND);
	fdActorJump(djinniId, 6, 5);
	sySynch(24);
	for(i = 0; i < 120; i++) {
		if(ABS(djinni->y.fixed - leader->y.fixed) < toFIXED(20))
			break;
		sySynch(1);
	}
	djinni->motionFlg = 0;
	djinni->velX = 0;
	djinni->velZ = 0;
	djinni->velY = 0;
	djinni->tgtX = ACTOR_NO_TARGET;
	djinni->tgtZ = ACTOR_NO_TARGET;
	djinni->tgtY = ACTOR_NO_TARGET;
	fdActorSelectAnimationStub(djinni, ANIM_STILL);
	((SPR *)djinni->spr)->image.pri = 2;
#if DJINNI_HOP_RESTORES_GRAVITY
	djinni->gravity = toFIXED(1);
#endif
	fdScriptEnd();
}

/* A Djinni joins. The element's last Djinni stands for the one of The Broken
   Seal's chosen for it (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). While
   FLAG_DJINN_LESSON is set it joins the leader by its flag alone, otherwise the
   party member with the fewest; if it found one, the element's join effect
   plays at the actor (none for actor -1), the Djinni is added, and the actor's
   script slot is cleared (in every edition but the Japanese; the Japanese
   release leaves it set). */
void fdDjinnJoinParty(Sint32 actor, Sint32 element, Sint32 djinni)
{
	Sint32 owner;

	if(djinni == DJINN_PER_ELEMENT - 1) {
		Sint32 recorded = SceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];

		if(recorded == 0)
			return;
		djinni = recorded - 1;
	}
	if(syGameFlagGet(FLAG_DJINN_LESSON)) {
		owner = MainRoutineStateWork.leaderActorId;
		syGameFlagSet(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinni));
	} else {
		owner = gmDjinnGiveToPartyStub(element, djinni);
	}
	if(owner < 0)
		return;
	fdScriptStart();
	if(actor != -1) {
		if(element == ELEMENT_VENUS)
			fdDjinnRunVenusJoinEffect(actor);
		else if(element == ELEMENT_MERCURY)
			fdDjinnRunMercuryJoinEffect(actor);
		else if(element == ELEMENT_MARS)
			fdDjinnRunMarsJoinEffect(actor);
		else if(element == ELEMENT_JUPITER)
			fdDjinnRunJupiterJoinEffect(actor);
	}
	fdWorldMapFreeze();
	mnDjinnShowJoinedMessageStub(owner, element, djinni);
	fdWorldMapReloadPendingAreaGraphics();
#if DJINNI_JOIN_CLEARS_ACTOR_SLOT
	FieldStateWork->actors[actor] = NULL;
#endif
	fdScriptEnd();
}

/* Puts a Djinni's actor back where its battle started from: for a pending
   Djinni battle (pendingDjinni from DJINNI_BATTLE_SCRIPT_FIRST, one for each
   Djinni, FLAG_KEEP_PARTY_POSITION set, the battle over) the scene's actor
   placed for that Djinni (condition = its flag) goes back to its placement's
   position, or 32 pixels north of the leader when the script has DJINN_NO_HOP
   set, and stands. fdModeRun calls it as it rebuilds the scene, only while a
   script is still pending after fdDjinnResolvePending's first call, that is
   after a won battle; the second call then makes this actor join the party. */
void fdDjinnRestoreActorAfterBattle(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	Uint32				raw = (Uint16)party->pendingDjinni;
	Sint32				flags = party->pendingDjinni & DJINN_PENDING_KIND_MASK;
	Uint32				scene = DJINN_PENDING_SCRIPT_MASK;
	Sint32				id;

	scene &= raw;
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
		flags = scene & DJINN_NO_HOP;
		scene &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
		if(scene - DJINNI_BATTLE_SCRIPT_FIRST <=
				ELEMENT_COUNT * DJINN_PER_ELEMENT &&
			party->modeResult > 0) {
			Sint32 *leaderId;

			for(id = ACTOR_FIRST_PLACED, leaderId = &party->leaderActorId;
				id <= FIELD_ACTOR_COUNT - 1; id++) {
				ACTOR_PLACE *placement =
					(ACTOR_PLACE *)fdFindActorPlacement(id);
				ACTOR *actor;

				if(placement == NULL ||
					placement->condition - FLAG_DJINN_FIRST !=
						scene - DJINNI_BATTLE_SCRIPT_FIRST)
					continue;
				actor = fdActorLookup(id);
				if(actor == NULL)
					continue;
				if(flags == 0) {
					actor->groundHeight = flags;
					actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
					fdActorSetPosAndResetMotionStub(
						actor, placement->x, placement->y, placement->z);
				} else {
					ACTOR *leader = fdActorLookup(*leaderId);

					fdActorSetPosAndResetMotionStub(actor, leader->x.fixed,
						leader->y.fixed, leader->z.fixed - toFIXED(32));
				}
				fdActorSelectAnimationStub(actor, ANIM_STAND);
			}
		}
	}
}

/* Settles a pending Djinni battle as a scene loads, from the battle's result in
   modeResult (the enemies defeated, BATTLE_OUTCOME_FLED when the party fled, -1
   when it lost), as The Broken Seal's; a mimic chest's script
   (CONDITION_FLAG_SET) falls through to the clear. */
void fdDjinnResolvePending(Sint32 shown)
{
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	Sint16			   *pending = &state->pendingDjinni;
	Sint32				kind = *pending & DJINN_PENDING_KIND_MASK;
	Sint32				flag = *pending & DJINN_PENDING_SCRIPT_MASK;

	if(shown == 0) {
		if(kind == 0) {
			flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
			if(flag < DJINNI_BATTLE_SCRIPT_FIRST ||
				flag > DJINNI_BATTLE_SCRIPT_FIRST +
						ELEMENT_COUNT * DJINN_PER_ELEMENT)
				return;
			if(state->modeResult > 0 &&
				state->modeResult != BATTLE_OUTCOME_FLED)
				return;
			syGameFlagSet(
				flag - (DJINNI_BATTLE_SCRIPT_FIRST - FLAG_DJINN_HIDING_FIRST));
			*pending = kind;
			return;
		} else if(kind == CONDITION_FLAG_SET) {
			if(state->modeResult > 0 &&
				state->modeResult != BATTLE_OUTCOME_FLED)
				syGameFlagSet(flag);
		} else
			return;
	} else if(kind == 0) {
		flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
		if(flag >= DJINNI_BATTLE_SCRIPT_FIRST &&
			flag <= DJINNI_BATTLE_SCRIPT_FIRST +
					ELEMENT_COUNT * DJINN_PER_ELEMENT) {
			flag &= DJINN_PENDING_BATTLE_SCRIPT_MASK;
			if(state->modeResult > 0) {
				Sint32 djinn = flag - DJINNI_BATTLE_SCRIPT_FIRST;
				Sint32 element = djinn / DJINN_PER_ELEMENT;
				Sint32 index = djinn % DJINN_PER_ELEMENT;
				Sint32 actor;

				for(actor = ACTOR_FIRST_PLACED; actor <= FIELD_ACTOR_COUNT - 1;
					actor++) {
					ACTOR_PLACE *found =
						(ACTOR_PLACE *)fdFindActorPlacement(actor);
					if(found != 0 &&
						found->condition - FLAG_DJINN_FIRST ==
							flag - DJINNI_BATTLE_SCRIPT_FIRST) {
						sySynch(40);
						fdDjinnJoinParty(actor, element, index);
						break;
					}
				}
			}
		}
	}
	MainRoutineStateWork.pendingDjinni = 0;
}

/* Event: updates the field effect sprites. */
void fdEffectUpdateAllSlots(void)
{
	FIELD_EFFECT_STATE *scene;
	Sint32				i;

	scene = Ram_HeapSlots->fieldEffState;
	for(i = 0; i < (Sint32)ARRAY_COUNT(scene->particles); i++)
		fdParticleUpdate(&scene->particles[i]);
}

/* Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and
   starts their update event. */
void fdEffectInitSlots(void)
{
	void *state;

	state = syHeapSlotAlloc(HEAP_SLOT_FIELD_EFFECT, sizeof(FIELD_EFFECT_STATE));
	DMA_FILL32(0, state, sizeof(FIELD_EFFECT_STATE));
	sySetEvent((Sint32)fdEffectUpdateAllSlots, EVENT_PASS_TICK);
}

/* Stops the field effect sprites: removes their event, frees every slot still
   in use and releases their heap block, the field Psynergy's state with it. */
void fdEffectReleaseSlots(void)
{
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_PARTICLE	   *slot;
	Sint32				i;

	syCloseEvent(fdEffectUpdateAllSlots);
	for(i = 0; i < (Sint32)ARRAY_COUNT(scene->particles); i++) {
		slot = &scene->particles[i];
		if(slot->active != 0)
			fdEffectClearOwnedSlot(slot);
	}
	syHeapSlotRelease(HEAP_SLOT_FIELD_EFFECT);
	sySynch(1);
}

/* Per-tick update of a joining Venus or Jupiter Djinni: spins it an eighth of
   a turn, as fdEffectAdvanceSpinAng does. */
void fdDjinnAdvanceSpinAng(ACTOR *djinni)
{
	djinni->facing += DEGtoANG(45);
}

/* Field effect sprite callback of a Jupiter Djinni's light particles: each
   circles its centre while it widens, circles, drifts toward a point 20 pixels
   over the leader and closes in, and is freed. It turns 1/32 turn a tick
   throughout, and its radius grows, then shrinks, by 1 pixel a tick. The
   slot's origin is the centre, in screen coordinates
   (fdCameraWorldToScreen). */
void fdEffectRunAngledApproachPhases(FIELD_PARTICLE *particle)
{
	ACTOR  *tgt;
	VECTOR3 pos;

	tgt = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(particle->state == DJINNI_LIGHT_WIDENING) {
		particle->orbitRadius += 1;
		particle->orbitAng += 1;
		if((Sint16)particle->age == DJINNI_LIGHT_RADIUS_TICKS) {
			particle->age = 0;
			particle->state += 1;
		}
	} else if(particle->state == DJINNI_LIGHT_CIRCLING) {
		particle->orbitAng += 1;
		if((Sint16)particle->age == DJINNI_LIGHT_CIRCLING_TICKS) {
			particle->age = 0;
			particle->state += 1;
		}
	} else if(particle->state == DJINNI_LIGHT_DRIFTING) {
		particle->orbitAng += 1;
		SET_POSITION(
			&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(20), tgt->z.fixed);
		/* The centre moves 1/8 of the way to the leader's screen point a
		   tick. */
		fdCameraWorldToScreen(&pos);
		particle->originX += (pos.x - particle->originX) / 8;
		particle->originZ += (pos.z - particle->originZ) / 8;
		if((Sint16)particle->age == DJINNI_LIGHT_DRIFTING_TICKS) {
			particle->age = 0;
			particle->state += 1;
		}
	} else if(particle->state == DJINNI_LIGHT_CLOSING) {
		particle->orbitRadius -= 1;
		particle->orbitAng += 1;
		if((Sint16)particle->age == DJINNI_LIGHT_RADIUS_TICKS) {
			particle->age = 0;
			particle->state += 1;
		}
	} else if(particle->state == DJINNI_LIGHT_DONE) {
		fdEffectClearOwnedSlot(particle);
	}
	pos.x = particle->originX;
	pos.z = particle->originZ;
	syVectorAddPolarOffset(toFIXED(particle->orbitRadius),
		particle->orbitAng * (TRIG_TURN / 32), &pos);
	particle->x = pos.x;
	particle->z = pos.z;
}

#undef DJINNI_AFTERIMAGES
#undef LEAP_SOUND
#undef DJINNI_LIGHT_RADIUS_TICKS
#undef DJINNI_LIGHT_CIRCLING_TICKS
#undef DJINNI_LIGHT_DRIFTING_TICKS
