// A Mars Djinni joining the party, and the first of the Venus Djinni's functions, linked after it:
// its shrinking, its falling motes and its rising particles. Part of script_commands.c.

#include "script_commands_internal.h"

// A Mars Djinni joins the party: it turns to the camera, bounces three times, spins ever faster and
// shrinks away in a fountain of 24 sparks (fdEffectUpdateMarsSpark). The leader turns to the camera
// and raises its arms, the sparks swarm it while it pulses for 100 ticks, and the sparks vanish.
void fdDjinnRunMarsJoinEffect(Sint32 djinniId)
{
	VECTOR3             pos;
	FIXED               scale;
	Sint32              i;
	ACTOR              *djinni;
	FIELD_EFFECT_STATE *effectState;
	ACTOR              *leader;
	FIELD_PARTICLE     *effect;

	djinni = fdActorGetById(djinniId);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if (djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = FieldEffectStateWork;
	// Tint the screen red.
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(16, 4, 4));
	sySynch(30);
	// Turn to face the camera (FACING_SOUTH), then bounce three times, a rising note on each.
	fdActorFaceDirectionById(djinniId, FACING_SOUTH, 0);
	sySynch(20);
	sySoundPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySoundPlayStub(SE_NOTE_MID);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySoundPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	// Spin up for 80 ticks, then spin and shrink away.
	sySynch(20);
	sySoundPlayStub(SE_SPIN);
	djinni->update = fdEffectAdvanceAngleWithRamp;
	djinni->timer  = 0;
	sySynch(80);
	djinni->update = fdEffectShrinkActorAndDestroySlow;
	fdActorSelectAnimationStub(djinni, 3);

	// The sparks start where the Djinni is on screen.
	pos.x = djinni->x.fixed;
	pos.y = djinni->y.fixed;
	pos.z = djinni->z.fixed;
	fdCameraWorldToScreen(&pos);

	// One spark a tick into each of the field effect sprites.
	for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++)
	{
		effect = &effectState->particles[i];
		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateMarsSpark);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSpriteSetAnimationColorModesStub(effect->sprite, SPRITE_COLOR_RED_TINT);
		scale          = syRandom() / 3 + toFIXED(1); // 1.0 to 1.33
		effect->scaleY = scale;
		effect->scaleX = scale;
		sySynch(1);
	}

	sySynch(60);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(fdActorGetById(MainRoutineStateWork.leaderActorId),
	                           ANIM_LEADER_ARMS_RAISED);

	// Every spark still in use stops its fountain.
	sySynch(20);
	for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++)
	{
		if (effectState->particles[i].active)
			effectState->particles[i].state = MARS_SPARK_STOPPING;
	}

	// The leader pulses while the sparks swarm it.
	sySynch(60);
	leader->update = fdEffectPulseActorScale;
	sySynch(100);

	// Every spark still in use goes.
	for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++)
	{
		if (effectState->particles[i].active)
			effectState->particles[i].state = MARS_SPARK_LEAVING;
	}

	// The leader stops pulsing.
	sySynch(10);
	leader->update = NULL;
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	// Fade the palette back from the tint and free the field effect sprites.
	sySynch(30);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

// Spins an actor an eighth of a turn and shrinks it by 1/64 of its size a tick, then releases it:
// 53 ticks from full size. The Venus Djinni's joining shrinks the Djinni away with it.
DEFINE_SHRINK_AND_DESTROY(fdEffectShrinkActorAndDestroyFast, -(toFIXED(1) / 64))

// Per-tick update of a Venus Djinni's falling motes: each circles the leader a 32nd of a turn a
// tick on a shrinking spiral (0.4 pixels of radius for each tick left) while it sinks a pixel a
// tick, and is released 20 pixels above the leader's feet.
void fdEffectUpdateDescendingOrbitActor(ACTOR *mote)
{
	VECTOR3 pos;
	ACTOR  *leader;
	Sint32  timer;
	Sint16  ticks;
	FIXED   y;

	leader           = fdActorGetById(MainRoutineStateWork.leaderActorId);
	timer            = mote->orbitFrame - 1;
	mote->orbitFrame = timer;
	ticks            = mote->orbitFrame;
	pos.x            = leader->x.fixed;
	pos.z            = leader->z.fixed;
	syVectorAddPolarOffset(
		ticks * (toFIXED(2) / 5), ticks * (TRIG_TURN / 32) + mote->orbitAngle, (FIXED *)&pos);
	mote->x.fixed = pos.x;
	mote->z.fixed = pos.z;
	y             = mote->y.fixed - toFIXED(1);
	mote->y.fixed = y;
	if (y < leader->y.fixed + toFIXED(20))
		fdActorReleaseStub(mote);
}

// Field effect sprite callback of a Venus Djinni's particles: each starts on screen at a random
// point 10 to 20 pixels round the leader and flies 120 pixels up the screen, over and over, until
// it is set to VENUS_PARTICLE_LAST_RISE and freed once it arrives.
void fdEffectUpdateRadialSpread(FIELD_PARTICLE *effect)
{
	VECTOR3 pos;
	ACTOR  *leader;
	Sint32  state;
	Uint32  random;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state  = effect->state;

	if (state == VENUS_PARTICLE_AIM)
	{
		pos.x  = leader->x.fixed;
		pos.y  = leader->y.fixed;
		pos.z  = leader->z.fixed;

		random = syRandom() * 10 + toFIXED(10);
		syVectorAddPolarOffset(random, syRandom(), (FIXED *)&pos);
		fdCameraWorldToScreen(&pos);

		effect->originX = pos.x;
		effect->originZ = pos.z;
		effect->x       = pos.x;
		effect->z       = pos.z;
		pos.x           = effect->x;
		pos.z           = effect->z;

		syVectorAddPolarOffset(toFIXED(120), FACING_NORTH, (FIXED *)&pos);
		effect->targetX         = pos.x;
		effect->targetZ         = pos.z;
		effect->acceleration    = toFIXED(1);
		effect->speedMax        = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		if ((FrameCount & 1) != 0)
			sySoundPlayStub(SE_CLATTER);
	}
	else if (state == VENUS_PARTICLE_RISING)
	{
		if (fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	}
	else if (state == VENUS_PARTICLE_LAST_RISE)
	{
		if (fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}
