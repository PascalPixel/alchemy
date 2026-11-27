/* @file */
/* The effect spawner each field overlay keeps its own copy of, the twin of The
 */
/* Broken Seal's EFFECT_SPAWN (scene/shared/field_effect_spawn.c). */
/* system/field_effect.h names the copy, the import that creates its actor and
 */
/* the duration scripts it picks from: EFFECT_SPAWN, EFFECT_CREATE and */
/* EFFECT_SCRIPTS. The including scene gives only EFFECT_SET_MODE and */
/* EFFECT_SET_SCRIPT, the functions it calls to choose the effect's animation */
/* and start its script. A file that also defines EFFECT_SPRITE_PRIORITY */
/* creates the actor as an event does (scFieldActorCreate) and gives the sprite
 */
/* the requested priority itself before scSetActorSprPriority. */
#include "field/field_functions.h"
#include "field/field_sequence_functions.h"
#include "system/field_effect.h"
#include "sprite_ids.h"

extern ACTOR *EFFECT_CREATE(s32 type, s32 x, s32 y, s32 z);
extern const s32 *EFFECT_SCRIPTS[];

void EFFECT_SPAWN(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
				  s32 velocityZ, u32 flags, const EFFECT_OPTIONS *options) {
	ACTOR *party = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	FIELD_SPR *sprite;
	const s32 *script;
	u32 start;

#ifdef EFFECT_SPRITE_PRIORITY
	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)scFieldActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)scFieldActorCreate(EFFECT_DEFAULT_TYPE, x, y, z);
#else
	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)EFFECT_CREATE(options->type, x, y, z);
	else
		effect = (FIELD_EFFECT *)EFFECT_CREATE(EFFECT_DEFAULT_TYPE, x, y, z);
#endif
	if(effect == 0)
		return;

	sprite = effect->spr;
	EFFECT_SET_MODE((void *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	EFFECT_SET_SCRIPT((void *)effect,
					  EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	scSetActorSprPriority(effect, ((FIELD_SPR *)party->spr)->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flags & EFFECT_USE_PALETTE) != 0)
		fdActorSetSprCol(effect, options->pal);
	if((flags & EFFECT_USE_PRIORITY) != 0) {
#ifdef EFFECT_SPRITE_PRIORITY
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->pri = options->pri;
#else
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
#endif
		scSetActorSprPriority(effect, options->pri);
	}
	start = flags & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flags & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED targetX;
		s32 count;

		script = EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK];
		targetX = options->tgtScaleX;
		count = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (targetX - effect->scaleX) / count;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
		else {
			effect->scaleRateX = (targetX - FIXED_ONE) / count;
			effect->scaleRateY = (options->tgtScaleY - FIXED_ONE) /
								 script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flags & EFFECT_USE_SCRIPT) != 0) {
		EFFECT_SET_MODE((void *)effect, 1);
		EFFECT_SET_SCRIPT((void *)effect, options->script);
	}
	if((flags & EFFECT_USE_ROTATION) != 0)
		sprite->rotation = options->rotation;
	if((flags & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flags & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}
