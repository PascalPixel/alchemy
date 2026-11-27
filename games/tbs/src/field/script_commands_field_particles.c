/* Field effect sprites: the particles field Psynergy moves and draws. Part of
   script_commands.c. */

#include "script_commands_internal.h"

#include "field/particle_update.c"

/* Draws a field effect sprite at its screen position and scale, when it is near
   the screen: x from -32 to 272 pixels and z from -32 to 224, both
   exclusive. */
void fdEffectDrawScaledActor(FIELD_PARTICLE *particle)
{
	SPR			 *spr;
	FIXED		  ofs;
	SPR_SCALE	  scale;
	SPR_PLACEMENT pos;

	spr = particle->spr;
	ofs = 0;
	if(particle->flg & FIELD_PARTICLE_LIFTED_BY_Z)
		ofs = toFIXED(508) - particle->z;
	scale.x = syMulFX(particle->scaleX, spr->scale);
	scale.y = syMulFX(particle->scaleY, spr->scale);
	pos.x = particle->x;
	pos.height = ofs;
	pos.y = particle->z + ofs;
	pos.groundHeight = 0;
	if(particle->x > -toFIXED(32) && particle->x < toFIXED(272) &&
		particle->z > -toFIXED(32) && particle->z < toFIXED(224))
		fdRenderApplyProjectedPlacementStub(spr, &pos, &scale, 0);
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
	Sint16 ang;
	Sint32 heading;
	Sint32 facing;
	Sint16 turn;

	if(effect->tgtX == FIELD_EFFECT_SPRITE_NO_TARGET)
		return;
	deltaX = effect->tgtX - effect->x;
	deltaZ = effect->tgtZ - effect->z;
	/* The distance in whole pixels first; under 128 pixels, the exact 16.16 one
	   decides whether it is within 8 pixels. */
	if(effect->stopAtTgt != 0) {
		pixelX = deltaX / toFIXED(1);
		pixelZ = deltaZ / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(pixelX * pixelX + pixelZ * pixelZ));
		if(distance < toFIXED(128))
			distance =
				sySquartFX(syMulFX(deltaX, deltaX) + syMulFX(deltaZ, deltaZ));
		if(distance <= toFIXED(8)) {
			fdParticleSetPos(effect, effect->tgtX, effect->tgtZ);
			return;
		}
	}
	ang = syAtan(deltaZ, deltaX);
	if(effect->turnStepLimited != 0) {
		heading = effect->heading;
		turn = ang - heading;
		if((turn >= 0 ? turn : -turn) >= effect->turnStepMax) {
			if(turn < 0) {
				if(-turn > effect->turnStepMax)
					turn = -effect->turnStepMax;
			} else if(turn > effect->turnStepMax) {
				turn = effect->turnStepMax;
			}
			ang = turn + heading;
		}
	}
	facing = (Uint16)ang;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if(speed > effect->speedMax)
		speed = effect->speedMax;
	ang = facing;
	effect->speed = speed;
	effect->x += syMulFX(syCos(ang), speed);
	effect->z += syMulFX(sySin(ang), speed);
}

/* Returns 1 while a field effect sprite that stops at its target still has one
   (it has not arrived), 0 once it has arrived or when it never stops. */
Uint32 fdParticleHasPendingTarget(FIELD_PARTICLE *effect)
{
	if(effect->stopAtTgt == 0) {
		return 0;
	}
	if(effect->tgtX != FIELD_EFFECT_SPRITE_NO_TARGET)
		return 1;
	return 0;
}

/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPos(FIELD_PARTICLE *effect, Sint32 x, Sint32 z)
{
	effect->tgtX = FIELD_EFFECT_SPRITE_NO_TARGET;
	effect->tgtZ = FIELD_EFFECT_SPRITE_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

#include "field/particle_set_animation.c"

/* Gives a field effect sprite a new sprite of kind at (x, z): stopping at its
   target, top speed 2, acceleration and scale 1, drawn, animation 1. */
void fdParticleInit(FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, FIXED z)
{
	SPR *spr;

	DMA_FILL32(0, effect, sizeof *effect);
	spr = fdSprCreateStub(kind);
	effect->spr = spr;
	if(spr != NULL)
		spr->image.pri = 0;
	fdParticleSetPos(effect, x, z);
	effect->scaleX = toFIXED(1);
	effect->speedMax = toFIXED(2);
	effect->scaleY = toFIXED(1);
	effect->acceleration = toFIXED(1);
	effect->originX = x;
	effect->originZ = z;
	/* BUG: fdSprCreateStub can return NULL, but this and the animation setup
	   below still require a sprite. Callers also immediately use it, so
	   guarding this store alone would not establish allocation-failure
	   recovery. */
	{
		SPR *created = effect->spr;
		created->flg = 0;
	}
	effect->stopAtTgt = 1;
	effect->turnStepLimited = 1;
	effect->updateMotion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->randomValue = syRandom();
	effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
	fdParticleSetAnimation(effect, 1);
}

#include "field/effect_clear_owned_slot.c"
