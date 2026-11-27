/* Field effect sprites: the particles field Psynergy moves and draws. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* Updates one field effect sprite: counts its age, runs its callback (after any
   delay), moves it and draws it. */
void fdParticleUpdate(FIELD_PARTICLE *effect)
{
	if(effect->active != 0) {
		effect->age++;
		if(effect->callbackDelay != 0)
			effect->callbackDelay--;
		else if(effect->callback != 0)
			effect->callback(effect);
		/* A callback may release its own slot; do not move or draw it
		   afterward. */
		if(effect->active != 0) {
			if(effect->updateMotion != 0)
				fdParticleUpdateMotion(effect);
			if(effect->render != 0)
				fdEffectDrawScaledActor(effect);
		}
	}
}

/* Draws a field effect sprite at its screen position and scale, when it is near
   the screen: x from -32 to 272 pixels and z from -32 to 224, both
   exclusive. */
void fdEffectDrawScaledActor(FIELD_PARTICLE *particle)
{
	SPR			 *sprite;
	FIXED		  offset;
	SPR_SCALE	  scale;
	SPR_PLACEMENT pos;

	sprite = particle->sprite;
	offset = 0;
	if(particle->flags & FIELD_PARTICLE_LIFTED_BY_Z)
		offset = toFIXED(508) - particle->z;
	scale.x = syMulFX(particle->scaleX, sprite->scale);
	scale.y = syMulFX(particle->scaleY, sprite->scale);
	pos.x = particle->x;
	pos.height = offset;
	pos.y = particle->z + offset;
	pos.groundHeight = 0;
	if(particle->x > -toFIXED(32) && particle->x < toFIXED(272) &&
		particle->z > -toFIXED(32) && particle->z < toFIXED(224))
		fdRenderApplyProjectedPlacementStub(sprite, &pos, &scale, 0);
}

/* Moves a field effect sprite toward its target, turning at most turnStepMax a
   tick when turnStepLimited is set and speeding up by acceleration to
   speedMax. One that stops at its target lands on it once within 8 pixels. */
void fdParticleUpdateMotion(FIELD_PARTICLE *effect)
{
	Sint32 deltaX;
	Sint32 deltaZ;
	FIXED  distance;
	Sint32 pixelX;
	Sint32 pixelZ;
	Sint32 speed;
	Sint16 angle;
	Sint32 heading;
	Sint32 facing;
	Sint16 turn;

	if(effect->targetX == FIELD_EFFECT_SPRITE_NO_TARGET)
		return;
	deltaX = effect->targetX - effect->x;
	deltaZ = effect->targetZ - effect->z;
	/* The distance in whole pixels first; under 128 pixels, the exact 16.16 one
	   decides whether it is within 8 pixels. */
	if(effect->stopAtTarget != 0) {
		pixelX = deltaX / toFIXED(1);
		pixelZ = deltaZ / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(pixelX * pixelX + pixelZ * pixelZ));
		if(distance < toFIXED(128))
			distance =
				sySquartFX(syMulFX(deltaX, deltaX) + syMulFX(deltaZ, deltaZ));
		if(distance <= toFIXED(8)) {
			fdParticleSetPos(effect, effect->targetX, effect->targetZ);
			return;
		}
	}
	angle = syAtan(deltaZ, deltaX);
	if(effect->turnStepLimited != 0) {
		heading = effect->heading;
		turn = angle - heading;
		if((turn >= 0 ? turn : -turn) >= effect->turnStepMax) {
			if(turn < 0) {
				if(-turn > effect->turnStepMax)
					turn = -effect->turnStepMax;
			} else if(turn > effect->turnStepMax) {
				turn = effect->turnStepMax;
			}
			angle = turn + heading;
		}
	}
	facing = (Uint16)angle;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if(speed > effect->speedMax)
		speed = effect->speedMax;
	angle = facing;
	effect->speed = speed;
	effect->x += syMulFX(syCos(angle), speed);
	effect->z += syMulFX(sySin(angle), speed);
}

/* Returns 1 while a field effect sprite that stops at its target still has one
   (it has not arrived), 0 once it has arrived or when it never stops. */
Uint32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect)
{
	if(effect->stopAtTarget == 0) {
		return 0;
	}
	if(effect->targetX != FIELD_EFFECT_SPRITE_NO_TARGET)
		return 1;
	return 0;
}

/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z)
{
	effect->targetX = FIELD_EFFECT_SPRITE_NO_TARGET;
	effect->targetZ = FIELD_EFFECT_SPRITE_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode)
{
	fdSpriteStartAnimationStub(effect->sprite, mode);
}

/* Gives a field effect sprite its per-tick callback, starting at state 0. */
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK callback)
{
	effect->callback = callback;
	effect->callbackDelay = 0;
	effect->age = 0;
	effect->state = 0;
}

/* Gives a field effect sprite a new sprite of kind at (x, z): stopping at its
   target, top speed 2, acceleration and scale 1, drawn, animation 1. */
void fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, FIXED z)
{
	SPR *sprite;

	DMA_FILL32(0, effect, sizeof *effect);
	sprite = fdSpriteCreateStub(kind);
	effect->sprite = sprite;
	if(sprite != NULL)
		sprite->image.priority = 0;
	fdParticleSetPos(effect, x, z);
	effect->scaleX = toFIXED(1);
	effect->speedMax = toFIXED(2);
	effect->scaleY = toFIXED(1);
	effect->acceleration = toFIXED(1);
	effect->originX = x;
	effect->originZ = z;
	/* BUG: fdSpriteCreateStub can return NULL, but this and the animation setup
	   below still require a sprite. Callers also immediately use it, so
	   guarding this store alone would not establish allocation-failure
	   recovery. */
	{
		SPR *created = effect->sprite;
		created->flags = 0;
	}
	effect->stopAtTarget = 1;
	effect->turnStepLimited = 1;
	effect->updateMotion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->randomValue = syRandom();
	effect->flags = FIELD_PARTICLE_LIFTED_BY_Z;
	fdParticleSetAnimation(effect, 1);
}

/* Frees a field effect sprite and its sprite, clearing it to zero
   (inactive). */
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect)
{
	if(effect->sprite)
		fdSpriteDestroyStub(effect->sprite);
	DMA_FILL32(0, effect, sizeof *effect);
}
