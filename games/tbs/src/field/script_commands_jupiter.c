/* A Jupiter Djinni joining the party, and the first of the Mars Djinni's
   functions, linked after it: its shrinking and its sparks. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* A Jupiter Djinni joins the party: it bounces three times, spins, leaps out of
   sight and bursts into a light particle for each of the effect state's, which
   circle and close in on the leader, who turns to the camera and raises its
   arms (ANIM_LEADER_ARMS_RAISED). */
void fdDjinnRunJupiterJoinEffect(Sint32 djinniId)
{
	VECTOR3				pos;
	ACTOR			   *djinni;
	FIELD_EFFECT_STATE *effectState;
	FIELD_PARTICLE	   *effect;
	FIXED				scale;
	Sint32				i;

	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;
	fdEffectInitSlots();
	effectState = FieldEffectStateWork;
	/* Tint the screen yellow. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(12, 12, 4));
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_MID);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySynch(20);

	/* Spin, then leap high. */
	sySndPlayStub(SE_SPIN);
	djinni->update = fdEffectAdvanceSpinAng;
	sySynch(40);
	sySndPlayStub(SE_HIGH_JUMP);
	fdActorJump(djinniId, 12, 22);

	/* The light particles, from where the Djinni is on screen. */
	pos.x = djinni->x.fixed;
	pos.y = djinni->y.fixed;
	pos.z = djinni->z.fixed;
	fdCameraWorldToScreen(&pos);
	fdActorReleaseStub(djinni);
	sySndPlayStub(SE_SCATTER);
	effect = effectState->particles;
	/* One a tick, each in a random colour mode from SPRITE_COLOR_NORMAL to
	   SPRITE_COLOR_GREY. */
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectRunAngledApproachPhases);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModesStub(
			effect->spr, (syRandom() * (SPRITE_COLOR_GREY + 1)) >> 16);
		scale = syRandom() / 3 + toFIXED(1); /* 1.0 to 1.33 */
		effect->scaleY = scale;
		effect->scaleX = scale;
		sySynch(1);
		effect++;
	}

	sySynch(60);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);
	sySynch(40);
	sySndPlayStub(SE_SCATTER);
	sySynch(100);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Per-tick update: makes an actor pulse, switching its scale between the two
   values of FieldEffectPulseScales every 4 ticks. */
void fdEffectPulseActorScale(ACTOR *actor)
{
	const Uint32 *tbl = FieldEffectPulseScales;
	Uint32		  idx = ((Uint32)FrameCount >> 2) & 1;
	Uint32		  scale = tbl[idx];
	actor->scaleX = scale;
	actor->scaleY = scale;
}

/* Per-tick update: spins an actor ever faster, from 1/16 turn a tick up to
   just over 1/8 (DEGtoANG(45) + 64), 80 more each tick for 52 ticks. */
void fdEffectAdvanceAngWithRamp(ACTOR *actor)
{
	Uint32 step;

	step = actor->spinRampFrm * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP) {
		actor->spinRampFrm = actor->spinRampFrm + 1;
	}
}

/* Spins an actor an eighth of a turn and shrinks it by 7/1024 of its size a
   tick, then releases it: 119 ticks from full size. The Mars Djinni's joining
   shrinks the Djinni away with it. */
DEFINE_SHRINK_AND_DESTROY(
	fdEffectShrinkActorAndDestroySlow, -(toFIXED(7) / 1024))

/* Field effect sprite callback of a Mars Djinni's sparks (enum
   MARS_SPARK_STATE): the spark flies 120 pixels up the screen (give or take
   3/32 turn) from its origin at up to 5 pixels a tick and starts over, until
   it stops where it arrives, hidden; 40 ticks later it swarms to points within
   4 pixels of one 20 pixels over the leader, until it is freed once it
   arrives. */
void fdEffectUpdateMarsSpark(FIELD_PARTICLE *effect)
{
	ACTOR  *leader;
	VECTOR3 pos;
	Sint32	state;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == MARS_SPARK_AIM) {
		effect->x = effect->originX;
		effect->z = effect->originZ;
		pos.x = effect->x;
		pos.z = effect->z;
		syVectorAddPolarOffset(toFIXED(120),
			syRandom16Below(TRIG_TURN * 3 / 32) -
				syRandom16Below(TRIG_TURN * 3 / 32) + FACING_NORTH,
			(FIXED *)&pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->acceleration = toFIXED(5);
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		/* At the leader's sprite priority, until MARS_SPARK_RISING brings it in
		   front (priority 0, z lifting the sprite) after 3 ticks. */
		((SPR *)effect->spr)->image.pri = leader->spr->image.pri;
		effect->flg = 0;
		effect->age = 0;
		if((FrameCount & 1) != 0)
			sySndPlayStub(SE_HEAVY_IMPACT);
	} else if(state == MARS_SPARK_RISING) {
		if(effect->age == 3) {
			((SPR *)effect->spr)->image.pri = 0;
			effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
		}
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == MARS_SPARK_STOPPING) {
		if(fdParticleHasPendingTarget(effect) == 0) {
			effect->originX = effect->x;
			effect->originZ = effect->z;
			((SPR *)effect->spr)->image.pri = 0;
			effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
			effect->render = 0;
			effect->state++;
			effect->funcDelay = 40;
		}
	} else if(state == MARS_SPARK_AIM_LEADER) {
		effect->render = 1;
		effect->x = effect->originX;
		effect->z = effect->originZ;
		pos.x = leader->x.fixed;
		pos.y = leader->y.fixed + toFIXED(20);
		pos.z = leader->z.fixed;
		fdCameraWorldToScreen(&pos);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->state++;
		if((FrameCount & 1) != 0)
			sySndPlayStub(SE_CRASH);
	} else if(state == MARS_SPARK_SWARMING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == MARS_SPARK_LEAVING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}
