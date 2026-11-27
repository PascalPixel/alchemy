/* The effect spawner each field overlay keeps its own copy of, the twin of The
   Broken Seal's EFFECT_SPAWN (scene/shared/field_effect_spawn.c).
   system/field_effect.h names the copy, the import that creates its actor and
   the duration scripts it picks from: EFFECT_SPAWN, EFFECT_CREATE and
   EFFECT_SCRIPTS. The including scene gives only EFFECT_SET_MODE and
   EFFECT_SET_SCRIPT, the functions it calls to choose the effect's animation
   and start its script. A file that also defines EFFECT_SPRITE_PRIORITY creates
   the actor as a script does (scFieldActorCreate) and gives the sprite the
   requested priority itself before fdSetActorSprPriority. */
#include "field/field_functions.h"
#include "field/field_mode_functions.h"
#include "system/field_effect.h"
#include "sprite_ids.h"

extern ACTOR		*EFFECT_CREATE(Sint32 type, Sint32 x, Sint32 y, Sint32 z);
extern const Sint32 *EFFECT_SCRIPTS[];

void EFFECT_SPAWN(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options)
{
	ACTOR		 *party = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	FIELD_SPR	 *spr;
	const Sint32 *script;
	Uint32		  start;

#ifdef EFFECT_SPRITE_PRIORITY
	if((flg & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)scFieldActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)scFieldActorCreate(EFFECT_DEFAULT_TYPE, x, y, z);
#else
	if((flg & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)EFFECT_CREATE(options->type, x, y, z);
	else
		effect = (FIELD_EFFECT *)EFFECT_CREATE(EFFECT_DEFAULT_TYPE, x, y, z);
#endif
	if(effect == 0)
		return;

	spr = effect->spr;
	EFFECT_SET_MODE((void *)effect, (flg + 1) & EFFECT_SCRIPT_MASK);
	EFFECT_SET_SCRIPT((void *)effect, EFFECT_SCRIPTS[flg & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velX;
	effect->vel.y = velY;
	effect->vel.z = velZ;
	fdSetActorSprPriority(effect, ((FIELD_SPR *)party->spr)->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flg & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flg & EFFECT_USE_PALETTE) != 0)
		fdActorSetSprCol(effect, options->pal);
	if((flg & EFFECT_USE_PRIORITY) != 0) {
#ifdef EFFECT_SPRITE_PRIORITY
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		spr->pri = options->pri;
#else
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
#endif
		fdSetActorSprPriority(effect, options->pri);
	}
	start = flg & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flg & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED  tgtX;
		Sint32 cnt;

		script = EFFECT_SCRIPTS[flg & EFFECT_SCRIPT_MASK];
		tgtX = options->tgtScaleX;
		cnt = script[EFFECT_SCRIPT_REPEAT_COUNT];
		if(start != 0) {
			effect->scaleRateX = (tgtX - effect->scaleX) / cnt;
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		} else {
			effect->scaleRateX = (tgtX - toFIXED(1)) / cnt;
			effect->scaleRateY = (options->tgtScaleY - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flg & EFFECT_USE_SCRIPT) != 0) {
		EFFECT_SET_MODE((void *)effect, 1);
		EFFECT_SET_SCRIPT((void *)effect, options->script);
	}
	if((flg & EFFECT_USE_ROTATION) != 0)
		spr->rotation = options->rotation;
	if((flg & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flg & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}
