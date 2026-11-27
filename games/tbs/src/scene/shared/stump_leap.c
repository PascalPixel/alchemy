// The stump Move pushes off a ledge into the water, in Goma Cave Entrance and Goma Cave.
//
// The including scene defines where it lands (STUMP_LANDING_X and _Z), where it bounces on to and
// sinks (STUMP_RESTING_X and _Z), and the height of the ground it comes down to
// (STUMP_FLOOR_HEIGHT, 16.16).

// Spins the leaping stump a thirty-second of a turn each tick.
static void scSpinStump(ACTOR *actor)
{
	SPR *sprite       = actor->sprite;

	sprite->rotation -= TRIG_TURN / 32;
}

#include "actor_wait_below_height.c"

// The stump tips over the ledge, leaps spinning into the water, lands with a shake and a ring of
// dust, bounces once and sinks with a splash.
static void scLeapStump(Sint32 stumpId)
{
	ACTOR         *stump;
	Uint32         i;
	VECTOR3        direction;
	EFFECT_OPTIONS options;

	stump              = fdActorGet(stumpId);
	stump->motionFlags = 0;
	for (i = 0; i < STUMP_TIP_TICKS; i++)
	{
		sySynch(1);
		stump->sprite->rotation -= TRIG_TURN / 256;
		stump->x.fixed          -= syCos(stump->sprite->rotation) / 2;
		stump->targetX           = ACTOR_NO_TARGET;
	}
	stump->update = scSpinStump;
	fdActorSetSpeed(stumpId, 3);
	fdActorSetDestination(stumpId, STUMP_LANDING_X, STUMP_LANDING_Z);
	stump->gravity     = toFIXED(4) / 5;
	stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	stump->layer       = 0;
	fdActorWaitForMoveById(stumpId);
	scActorWaitBelowHeight(stump, STUMP_FLOOR_HEIGHT);
	sySoundPlay(SE_THUMP);
	fdMapGridSetShake(5, 5, 1);
	sySoundPlay(SE_RUMBLE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	// The puffs of dust, one in each of the sixteen facings.
	for (i = 0; i < STUMP_LANDING_PUFFS; i++)
	{
		direction.x  = syCos(i * FACING_STEP);
		direction.y  = 0;
		direction.z  = sySin(i * FACING_STEP);
		direction.x -= direction.x / 4;
		direction.z -= direction.z / 2;
		fdEffectSpawn(stump->x.fixed,
		              stump->y.fixed,
		              stump->z.fixed,
		              direction.x,
		              direction.y,
		              direction.z,
		              0,
		              NULL);
	}
	stump->velocityY = toFIXED(5);
	fdActorSetDestination(stumpId, STUMP_RESTING_X, STUMP_RESTING_Z);
	fdActorWaitForMoveById(stumpId);
	scActorWaitBelowHeight(stump, STUMP_FLOOR_HEIGHT);
	stump->update           = NULL;
	stump->sprite->rotation = TRIG_TURN / 16;
	options.type            = SPRITE_SPLASH;
	options.startScaleX     = toFIXED(1) / 2;
	options.startScaleY     = toFIXED(4) / 5;
	options.targetScaleX    = toFIXED(3) / 2;
	options.targetScaleY    = toFIXED(6) / 5;
	fdEffectSpawn(stump->x.fixed,
	              stump->y.fixed,
	              stump->z.fixed,
	              0,
	              0,
	              0,
	              EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
	              &options);
	sySoundPlay(SE_CAST);
	fdActorSetAnimation(stumpId, STUMP_ANIMATION_SPLASH);
	fdMapGridWaitForShakeToSettle();
}
