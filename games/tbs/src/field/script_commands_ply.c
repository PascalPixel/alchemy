// Ply cast in the field: eight winged figures spiral into one, which flashes, sheds sparks and
// flies off. Part of script_commands.c.

#include "script_commands_internal.h"

// Per-tick update: sways 4 pixels either side of anchorX, once every 128 ticks.
void fdEffectSwayParticle(ACTOR *actor)
{
	actor->x.fixed = actor->targetX + syMulFX(toFIXED(4), sySin(actor->timer * (TRIG_TURN / 128)));
	actor->timer++;
	actor->timer = (actor->timer + 128) % 128;
}

// Per-tick update: spirals in on a point 10 pixels above the Psynergy's position, at a radius of as
// many pixels as ticks are left; released when timer reaches 0.
void fdEffectUpdateShrinkingOrbitActor(ACTOR *particle)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	VECTOR3             pos;
	Sint16              remainingTicks;
	Sint32              decrementedTimer;

	if (particle != NULL)
	{
		decrementedTimer = (Uint16)particle->timer - 1;
		particle->timer  = decrementedTimer;
		remainingTicks   = decrementedTimer;
		if (remainingTicks != 0)
		{
			pos.x = effectState->x;
			pos.y = effectState->y + toFIXED(10);
			pos.z = effectState->z;
			syVectorAddPolarOffset(toFIXED(remainingTicks),
			                       particle->userValue + remainingTicks * (TRIG_TURN / 32),
			                       (FIXED *)&pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			fdActorStartScriptStub(particle, FieldEffectSharedParticleScript);
		}
	}
}

// Per-tick update: sways 6 pixels either side (a 64-tick cycle) 16 pixels above the Psynergy's
// position, and every 3 ticks sheds a spark that sways as it slowly falls.
void fdEffectRunSparkEmitter(ACTOR *emitter)
{
	FIELD_EFFECT_STATE *effectState;
	ACTOR              *spark;
	Sint32              pos[3];

	effectState = FieldEffectStateWork;
	if (emitter->timer != -1)
	{
		SET_ACTOR_POSITION(emitter,
		                   effectState->x
		                       + syMulFX(toFIXED(6), sySin(emitter->timer * (TRIG_TURN / 64))),
		                   effectState->y + PSYNERGY_EFFECT_HEIGHT,
		                   effectState->z);
		emitter->timer++;
		emitter->timer = (emitter->timer + 64) % 64;
	}
	if (TickCount % 3 == 0)
	{
		// From 2 pixels up, 0 to 6 pixels in a random direction.
		SET_VECTOR(pos, emitter->x.fixed, emitter->y.fixed + toFIXED(2), emitter->z.fixed);
		syVectorAddPolarOffset(syRandom() * 6, syRandom(), pos);
		spark = fdActorSpawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
		// Each falls, 0.007 pixels a tick faster each tick, and is released after 72 ticks, its
		// script's wait.
		if (spark != 0)
		{
			spark->update = fdEffectSwayParticle;
			spark->scaleX = spark->scaleY = toFIXED(3) / 5;
			spark->motionFlags            = ACTOR_FALLS;
			spark->gravity                = toFIXED(0.007);
			spark->timer                  = syRandom() >> 9;
			spark->targetX                = spark->x.fixed;
			fdActorSetSpriteColorModeStub(spark, SPRITE_COLOR_BLUE_TINT);
			spark->wait = 72;
			fdActorStartScriptStub(spark, FieldEffectSharedParticleScript);
		}
	}
}

// Ply's flashing figure: its steps, of 2 ticks each.
#define PLY_FLASH_STEPS 30

// Ply cast on a target, which stops the caster's update once it is done.
void fdPsynergyPlyOnTarget(Sint32 unused)
{
	fdPsynergyPly();
	fdPsynergyStopCasterUpdate();
}

// Eight particles spiral into the centre, where one appears, sheds sparks and flies far away.
void fdPsynergyPly(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR              *target      = effectState->caster;
	ACTOR              *actor;
	SPR                *last;
	Uint8               resourceSlot;
	Sint32              i;

	fdPsynergyFadeToCastColor();

	// The spiralling particles: one a tick, each 1/8 turn on from the last.
	last = 0;
	for (i = 0; i < 8; i++)
	{
		actor = fdActorSpawn(
			SPRITE_WINGED_FIGURE, target->x.fixed, target->y.fixed + toFIXED(32), target->z.fixed);
		if (actor != 0)
		{
			actor->scaleY      = toFIXED(7) / 10;
			actor->scaleX      = toFIXED(7) / 10;
			actor->update      = fdEffectUpdateShrinkingOrbitActor;
			actor->timer       = 120;
			actor->userValue   = i * (TRIG_TURN / 8);
			actor->motionFlags = ACTOR_FLOATS;
			last               = fdSpriteShareVRAMCacheEntry(actor->sprite, last);
		}
		sySynch(1);
	}

	// The particles share one sprite cache; keep its slot to free at the end. last is read with no
	// check that any particle spawned.
	resourceSlot = last->slot;
	sySoundPlayStub(SE_PSYNERGY_START);
	// The last particle spawned 7 ticks ago with 120 ticks to live: wait until the spiral is nearly
	// closed.
	sySynch(110);

	// The one in the centre.
	target = fdActorSpawn(SPRITE_WINGED_FIGURE, 0, 0, 0);
	// FAKEMATCH(temporary): actor, a copy of target, the figure read by both
	// * names.
	actor  = target;
	if (target != 0)
	{
		target->scaleY      = toFIXED(7) / 10;
		target->scaleX      = toFIXED(7) / 10;
		target->x.fixed     = effectState->x;
		target->y.fixed     = effectState->y + toFIXED(16);
		target->z.fixed     = effectState->z;
		target->motionFlags = ACTOR_FLOATS;
		fdActorSetSpriteColorModeStub(target, SPRITE_COLOR_WHITE);
	}

	sySoundPlayStub(SE_PSYNERGY_SPARKLE);
	sySynch(12);
	if (target != 0)
	{
		// Blue every 2 ticks, red one time in 4.
		for (i = 0; i < PLY_FLASH_STEPS; i++)
		{
			// FAKEMATCH(dead): target set again to actor.
			target = actor;
			if (i & 3)
				fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_BLUE_TINT);
			else
				fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_RED_TINT);
			sySynch(2);
		}
	}

	// Sparks.
	fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_NORMAL);
	sySoundPlayStub(MUS_POISON_CURED);
	if (actor != 0)
	{
		target->update = fdEffectRunSparkEmitter;
		target->timer  = 0;
		// Cast on a target (enabled cleared by fdPsynergyDispatch), the sparks fall for 192 ticks
		// instead of 128.
		if (effectState->aimAhead != 0)
			sySynch(128);
		else
			sySynch(192);
	}

	// Fly away: 192 pixels at angle TRIG_TURN * 29 / 32, and release it once it arrives.
	if (target != 0)
	{
		target->timer         = -1;
		target->speedLimit    = toFIXED(5);
		target->acceleration  = toFIXED(2) / 5;
		target->steeringFlags = 0;
		fdActorSetTargetPosFromMagnitudeAngle(target, toFIXED(192), TRIG_TURN * 29 / 32);
		fdActorWaitForMoveStub(target);
		fdActorReleaseStub(target);
	}
	// syVRAMCacheFindFreeEntry returns VRAM_CACHE_ENTRY_COUNT for no entry.
	if (resourceSlot != VRAM_CACHE_ENTRY_COUNT)
		syVRAMCacheFreeEntry(resourceSlot);
	if (effectState->finishCallback != 0)
		effectState->finishCallback();
	fdPsynergyFadeFromCastColor();
}

#undef PLY_FLASH_STEPS
