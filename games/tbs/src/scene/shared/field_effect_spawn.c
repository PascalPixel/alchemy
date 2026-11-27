/* @file */
/* Twenty-four scenes, and the main image's field Psynergy */
/* (event_commands_whirlwind.c, with EFFECT_SPAWN_RESIDENT): fdEffectSpawn, */
/* which places a small effect moving at a velocity, its script one of */
/* FieldEffect_DurationScripts and its options chosen by the EFFECT_USE_ */
/* flags. */

#include "field/field_effect.h"
#include "stubs.h"

/* An effect's update function: move by its velocity, and grow and spin at its
 */
/* rates. */
void scFieldEffectUpdateMotionScaleAndSpin(ACTOR *actor) {
	FIELD_EFFECT *effect = (FIELD_EFFECT *)actor;
	effect->x += effect->vel.x;
	effect->y += effect->vel.y;
	effect->z += effect->vel.z;
	effect->scaleX += effect->scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->spr->rotation += effect->spin;
}

extern const s32 *FieldEffect_DurationScripts[];

/* Sets the effect moving at its velocity, drawn at the leader's sprite */
/* priority, with no growth or spin until the options give them. */
static inline void scFieldEffectInitMotion(FIELD_EFFECT *effect, SPR *sprite,
										   ACTOR *leader, s32 velocityX,
										   s32 velocityY, s32 velocityZ) {
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	sprite->image.pri = leader->spr->image.pri;
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;
}

/* Defines `name`, which places a field effect at (x, y, z) moving at the */
/* velocity given, with its duration script, and applies the options that */
/* `flags` asks for; `create`, `selectAnimation` and `startScript` are the */
/* actor functions it reaches them through, called directly in an overlay and */
/* through stub calls in the main image. */
#define EFFECT_SPAWN(name, create, selectAnimation, startScript)               \
	void name(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,               \
			  s32 velocityZ, u32 flags, const EFFECT_OPTIONS *options) {       \
		ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);                        \
		FIELD_EFFECT *effect;                                                  \
		SPR *sprite;                                                           \
		const s32 *script;                                                     \
                                                                               \
		if((flags & EFFECT_USE_TYPE) != 0 && options != NULL)                  \
			effect = (FIELD_EFFECT *)create(options->type, x, y, z);           \
		else                                                                   \
			effect = (FIELD_EFFECT *)create(EFFECT_DEFAULT_TYPE, x, y, z);     \
		if(effect == NULL)                                                     \
			return;                                                            \
                                                                               \
		sprite = effect->spr;                                                  \
		selectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);             \
		startScript((void *)effect,                                            \
					FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);  \
		effect->motionFlg = 0;                                                 \
		sprite->flg = 0;                                                       \
		effect->update = scFieldEffectUpdateMotionScaleAndSpin;                \
		scFieldEffectInitMotion(effect, sprite, leader, velocityX, velocityY,  \
								velocityZ);                                    \
                                                                               \
		if((flags & EFFECT_OPTION_MASK) == 0 || options == NULL)               \
			return;                                                            \
		if((flags & EFFECT_USE_PALETTE) != 0)                                  \
			fdActorSetSprCol((ACTOR *)effect, options->pal);                   \
		if((flags & EFFECT_USE_PRIORITY) != 0) {                               \
			effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;                       \
			sprite->image.pri = options->pri;                                  \
		}                                                                      \
		if((flags & EFFECT_USE_START_SCALE) != 0) {                            \
			effect->scaleX = options->startScaleX;                             \
			effect->scaleY = options->startScaleY;                             \
		}                                                                      \
		if((flags & EFFECT_SCALE_TO_TARGET) != 0) {                            \
			script = FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];  \
			if((flags & EFFECT_USE_START_SCALE) != 0) {                        \
				effect->scaleRateX = (options->tgtScaleX - effect->scaleX) /   \
									 script[EFFECT_SCRIPT_REPEAT_COUNT];       \
				effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /   \
									 script[EFFECT_SCRIPT_REPEAT_COUNT];       \
			}                                                                  \
			else {                                                             \
				effect->scaleRateX = (options->tgtScaleX - FIXED_ONE) /        \
									 script[EFFECT_SCRIPT_REPEAT_COUNT];       \
				effect->scaleRateY = (options->tgtScaleY - FIXED_ONE) /        \
									 script[EFFECT_SCRIPT_REPEAT_COUNT];       \
			}                                                                  \
		}                                                                      \
		if((flags & EFFECT_USE_SCRIPT) != 0) {                                 \
			selectAnimation(effect, 1);                                        \
			startScript((void *)effect, options->script);                      \
		}                                                                      \
		if((flags & EFFECT_USE_ROTATION) != 0)                                 \
			sprite->rotation = options->rotation;                              \
		if((flags & EFFECT_USE_SPIN) != 0)                                     \
			effect->spin = options->spin;                                      \
		if((flags & EFFECT_USE_UPDATE) != 0)                                   \
			effect->update = options->update;                                  \
	}

#ifdef EFFECT_SPAWN_RESIDENT
EFFECT_SPAWN(fdEffectSpawnResident, fdActorCreateStub,
			 fdActorSelectAnimationStub, fdActorStartScriptStub)
#else
EFFECT_SPAWN(fdEffectSpawn, syFieldActorCreate, fdActorSelectAnimation,
			 fdActorStartScript)
#endif
