/* Twenty-four scenes, and the main image's field Psynergy
   (script_commands_whirlwind.c, with EFFECT_SPAWN_RESIDENT): fdEffectSpawn,
   which places a small effect moving at a velocity, its script one of
   FieldEffectDurationScripts and its options chosen by the EFFECT_USE_
   flags. */

#include "field/field_effect.h"
#include "stubs.h"

/* An effect's update function: move by its velocity, and grow and spin at its
   rates. */
void scFieldEffectUpdateMotionScaleAndSpin(ACTOR *actor)
{
	FIELD_EFFECT *effect = (FIELD_EFFECT *)actor;
	effect->x += effect->velocity.x;
	effect->y += effect->velocity.y;
	effect->z += effect->velocity.z;
	effect->scaleX += effect->scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->sprite->rotation += effect->spin;
}

extern const Sint32 *FieldEffectDurationScripts[];

/* Sets the effect moving at its velocity, drawn at the leader's sprite
   priority, with no growth or spin until the options give them. */
static inline void scFieldEffectInitMotion(FIELD_EFFECT *effect, SPR *sprite,
	ACTOR *leader, Sint32 velocityX, Sint32 velocityY, Sint32 velocityZ)
{
	effect->velocity.x = velocityX;
	effect->velocity.y = velocityY;
	effect->velocity.z = velocityZ;
	sprite->image.priority = leader->sprite->image.priority;
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;
}

/* Defines `name`, which places a field effect at (x, y, z) moving at the
   velocity given, with its duration script, and applies the options that
   `flags` asks for; `create`, `selectAnimation` and `startScript` are the actor
   functions it reaches them through, called directly in an overlay and through
   stub calls in the main image. */
#define EFFECT_SPAWN(name, create, selectAnimation, startScript) \
	void name(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX, \
		Sint32 velocityY, Sint32 velocityZ, Uint32 flags, \
		const EFFECT_OPTIONS *options) \
	{ \
		ACTOR		 *leader = fdActorGet(ACTOR_PARTY_LEADER); \
		FIELD_EFFECT *effect; \
		SPR			 *sprite; \
		const Sint32 *script; \
\
		if((flags & EFFECT_USE_TYPE) != 0 && options != NULL) \
			effect = (FIELD_EFFECT *)create(options->type, x, y, z); \
		else \
			effect = (FIELD_EFFECT *)create(EFFECT_DEFAULT_TYPE, x, y, z); \
		if(effect == NULL) \
			return; \
\
		sprite = effect->sprite; \
		selectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK); \
		startScript((void *)effect, \
			FieldEffectDurationScripts[flags & EFFECT_SCRIPT_MASK]); \
		effect->motionFlags = 0; \
		sprite->flags = 0; \
		effect->update = scFieldEffectUpdateMotionScaleAndSpin; \
		scFieldEffectInitMotion( \
			effect, sprite, leader, velocityX, velocityY, velocityZ); \
\
		if((flags & EFFECT_OPTION_MASK) == 0 || options == NULL) \
			return; \
		if((flags & EFFECT_USE_PALETTE) != 0) \
			fdActorSetSpriteColor((ACTOR *)effect, options->palette); \
		if((flags & EFFECT_USE_PRIORITY) != 0) { \
			effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC; \
			sprite->image.priority = options->priority; \
		} \
		if((flags & EFFECT_USE_START_SCALE) != 0) { \
			effect->scaleX = options->startScaleX; \
			effect->scaleY = options->startScaleY; \
		} \
		if((flags & EFFECT_SCALE_TO_TARGET) != 0) { \
			script = FieldEffectDurationScripts[flags & EFFECT_SCRIPT_MASK]; \
			if((flags & EFFECT_USE_START_SCALE) != 0) { \
				effect->scaleRateX = \
					(options->targetScaleX - effect->scaleX) / \
					script[EFFECT_SCRIPT_REPEAT_COUNT]; \
				effect->scaleRateY = \
					(options->targetScaleY - effect->scaleY) / \
					script[EFFECT_SCRIPT_REPEAT_COUNT]; \
			} else { \
				effect->scaleRateX = (options->targetScaleX - toFIXED(1)) / \
					script[EFFECT_SCRIPT_REPEAT_COUNT]; \
				effect->scaleRateY = (options->targetScaleY - toFIXED(1)) / \
					script[EFFECT_SCRIPT_REPEAT_COUNT]; \
			} \
		} \
		if((flags & EFFECT_USE_SCRIPT) != 0) { \
			selectAnimation(effect, 1); \
			startScript((void *)effect, options->script); \
		} \
		if((flags & EFFECT_USE_ROTATION) != 0) \
			sprite->rotation = options->rotation; \
		if((flags & EFFECT_USE_SPIN) != 0) \
			effect->spin = options->spin; \
		if((flags & EFFECT_USE_UPDATE) != 0) \
			effect->update = options->update; \
	}

#ifdef EFFECT_SPAWN_RESIDENT
EFFECT_SPAWN(fdEffectSpawnResident, fdActorCreateStub,
	fdActorSelectAnimationStub, fdActorStartScriptStub)
#else
EFFECT_SPAWN(fdEffectSpawn, syFieldActorCreate, fdActorSelectAnimation,
	fdActorStartScript)
#endif
