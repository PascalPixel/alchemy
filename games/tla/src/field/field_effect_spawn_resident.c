/* The main image's copy of the overlays' effect spawner (field_effect_spawn.c),
   an object of its own between the script commands: Carry's ring of dust
   (fdEffectSpawnRadialParticleRing) spawns through it. */

#include "field/field_mode_functions.h"
#include "sprite_ids.h"
#include "system/field_effect.h"
#include "field/sprite.h"

extern const Sint32 *FieldEffectDurationScripts[];

/* Places a small effect at (x, y, z) moving at a velocity: a dust cloud, or
   options' type when EFFECT_USE_TYPE asks for it, its script one of
   FieldEffectDurationScripts and its options chosen by the EFFECT_USE_
   flags. */
void fdEffectSpawnResident(Sint32 x, Sint32 y, Sint32 z, Sint32 velX,
	Sint32 velY, Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options)
{
	/* UB: actor 0 is Isaac's, on stage only while he leads; with Felix leading
	   fdActorGetById returns NULL, and the sprite priority below is read
	   through it. UBFIX takes the leader's, as the overlays' copy does. */
#if FIX_RESIDENT_SPAWN_LEADER_ACTOR
	ACTOR *party = fdActorGetById(MainRoutineStateWork.leaderActorId);
#else
	ACTOR *party = fdActorGetById(ACTOR_ISAAC);
#endif
	FIELD_EFFECT *effect;
	FIELD_SPR	 *spr;
	const Sint32 *script;

	if((flg & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)fdActorCreateStub(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)fdActorCreateStub(EFFECT_DEFAULT_TYPE, x, y, z);
	if(effect == 0)
		return;

	spr = effect->spr;
	fdActorSelectAnimationStub((ACTOR *)effect, (flg + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScriptStub(
		(void *)effect, FieldEffectDurationScripts[flg & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	spr->flg = 0;
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velX;
	effect->vel.y = velY;
	effect->vel.z = velZ;
	spr->pri = ((SPR *)party->spr)->image.pri;
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flg & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flg & EFFECT_USE_PALETTE) != 0)
		fdActorSetSprCol((ACTOR *)effect, options->pal);
	if((flg & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		spr->pri = options->pri;
	}
	if((flg & EFFECT_USE_START_SCALE) != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flg & EFFECT_SCALE_TO_TARGET) != 0) {
		script = FieldEffectDurationScripts[flg & EFFECT_SCRIPT_MASK];
		if((flg & EFFECT_USE_START_SCALE) != 0) {
			effect->scaleRateX = (options->tgtScaleX - effect->scaleX) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
			effect->scaleRateY = (options->tgtScaleY - effect->scaleY) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		} else {
			effect->scaleRateX = (options->tgtScaleX - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
			effect->scaleRateY = (options->tgtScaleY - toFIXED(1)) /
				script[EFFECT_SCRIPT_REPEAT_COUNT];
		}
	}
	if((flg & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimationStub((ACTOR *)effect, 1);
		fdActorStartScriptStub((void *)effect, options->script);
	}
	if((flg & EFFECT_USE_ROTATION) != 0)
		spr->rotation = options->rotation;
	if((flg & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flg & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}
