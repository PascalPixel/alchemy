/* @file */
/* Avoid: the caster spins among falling arcs and flashes, and monsters attack
 */
/* less often until it wears off. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* End of the Psynergy: switches the caster's sprite between SPRITE_FLAG_SHADOW
 */
/* | SPRITE_FLAG_OUTLINED and SPRITE_FLAG_SHADOW on a 5-frame cycle, so it */
/* flashes as a dark outline. */
void fdEffectBlinkAvoidCaster(void) {
	ACTOR *caster;
	SPR *sprite;
	SPR_ANIMATION *entry;
	s32 phase;

	caster = fdActorLookup(MainRoutineStateWork.leaderActorId);
	sprite = caster->spr;
	entry = sprite->entries[0];
	phase = FrameCount % 5;
	if(phase == 0) {
		sprite->dirty = 1;
		sprite->flg = SPRITE_FLAG_SHADOW | SPRITE_FLAG_OUTLINED;
	}
	else if(phase == 2) {
		entry->colMode = SPRITE_COLOR_NORMAL; /* restore its colours */
		sprite->dirty = 1;
		sprite->flg = SPRITE_FLAG_SHADOW;
	}
}

/* The caster during the Psynergy: sheds a pair of falling arcs every 5 frames
 */
/* (7 on the World Map), and spins in phase 1, 3/64 turn a frame. */
void fdEffectUpdateAvoidCaster(ACTOR *caster) {
	s16 phase;
	s32 elapsedFrames;

	phase = caster->arcPhase;
	elapsedFrames = caster->arcFrm++;

	if(MainRoutineStateWork.mapId == MAP_INDEX_WORLD) {
		if(elapsedFrames % 7 == 0)
			fdEffectSpawnDescendingArcParticles(caster);
	}
	else if(elapsedFrames % 5 == 0) {
		fdEffectSpawnDescendingArcParticles(caster);
	}

	if(phase == 1)
		caster->facing += TRIG_TURN * 3 / 64;
}

/* Says that Avoid, or the Sacred Feather, has worn off: "Avoid's effects wore
 */
/* off." Clears MainRoutineStateWork.avoidDistance. Reached through */
/* fdPsynergyRunExtraHandler when the Psynergy request the leader's walking */
/* raises as Avoid runs out (field_sequence_walking.c) is handled. */
void fdPsynergyShowAvoidWoreOff(void) {
	MainRoutineStateWork.avoidDistance = 0;
	if(MainRoutineStateWork.avoidByItem == 0) {
		mnMessageArgumentPushStub(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
		mnFieldMessageShowStub(MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
		return;
	}
	mnMessageArgumentPushStub(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
	mnFieldMessageShowStub(MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
}

#define AVOID_FLASHES	   20 /* white flashes, 5 frames each */
#define AVOID_BLINK_FRAMES 70 /* the outline flashes */
#define AVOID_NOTE_FRAME   15 /* SE_NOTE_MID, into the outline's flashing */

/* The caster spins among falling arcs and flashes white, then flashes as an */
/* outline, and encounters are kept away until Avoid wears off. */
void fdPsynergyAvoid(void) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *actor;
	SPR *sprite;
	SPR_ANIMATION *entry;
	u32 angle;
	s32 slot;
	s32 count;

	effectState = FieldEffectStateWork;
	actor = effectState->caster;
	sprite = actor->spr;
	entry = sprite->entries[0];
	angle = actor->facing;
	slot = syVramCacheFindFreeEntry();
	/* The arcs' tiles (256 bytes) stay loaded until the Psynergy's end; the */
	/* distance is 2,400 pixels in 16.16. */
	effectState->arcTileSlot = slot;
	syVramCacheLoad((s16)slot, 256, FieldEffect_ArcSparkTiles);
	MainRoutineStateWork.avoidDistance = toFIXED(2400);
	MainRoutineStateWork.avoidByItem = gmFlagGetStub(FLAG_PSYNERGY_FROM_ITEM);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	actor->update = fdEffectUpdateAvoidCaster;
	actor->arcPhase = 0;
	actor->arcFrm = 0;
	sySndPlayStub(SE_SPIN);
	sySynch(15);
	actor->arcPhase = 1;
	sySynch(10);
	/* Flash white for 2 frames in every 5. */
	for(count = 0; count < AVOID_FLASHES; count++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sySynch(2);
		sprite->dirty = 1;
		entry->colMode = SPRITE_COLOR_NORMAL;
		sprite->flg = SPRITE_FLAG_SHADOW;
		sySynch(3);
	}
	actor->update = NULL;
	actor->facing = angle;
	sySetEvent(fdEffectBlinkAvoidCaster, TASK_PASS_FRAME);
	sySynch(AVOID_NOTE_FRAME);
	sySndPlayStub(SE_NOTE_MID);
	sySynch(AVOID_BLINK_FRAMES - AVOID_NOTE_FRAME);
	syCloseEvent(fdEffectBlinkAvoidCaster);
	/* Back to the caster's own look: outlined while Cloak is on, else with its
	 */
	/* shadow. */
	if(MainRoutineStateWork.cloaked != 0)
		fdActorSetSprFlagsStub(actor, SPRITE_FLAG_OUTLINED);
	else
		fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	syVramCacheFreeEntry(effectState->arcTileSlot);
	/* "Monsters won't attack so often now." */
	mnFieldMessageShowStub(MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}

#undef AVOID_FLASHES
#undef AVOID_BLINK_FRAMES
#undef AVOID_NOTE_FRAME
