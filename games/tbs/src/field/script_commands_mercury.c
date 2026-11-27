/* A Mercury Djinni joining the party. Part of script_commands.c. */

#include "script_commands_internal.h"

#define MERCURY_DROPLETS	   24 /* droplets (one a field effect sprite) */
#define MERCURY_AFTERIMAGE_MAX 8  /* afterimages of the rising Djinni */

/* A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
   bounces twice and hops three times ever higher, then rises 120 pixels out of
   sight trailing eight afterimages; the leader raises its arms as droplets rain
   round it. */
void fdDjinnRunMercuryJoinEffect(Sint32 djinniId)
{
	ACTOR			   *djinni;
	ACTOR			   *copy;
	ACTOR			   *previous;
	SPR				   *sharedSprite;
	FIELD_EFFECT_STATE *effectState;
	ACTOR			  **cursor;
	ACTOR			  **list;
	FIELD_PARTICLE	   *effect;
	Sint32				spriteId;
	Sint32				i;
	Sint32				sharedSlot;
	ACTOR			   *copies[MERCURY_AFTERIMAGE_MAX];
	VECTOR3				position;

	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;

	/* Tint the screen blue. */
	fdEffectInitSlots();
	effectState = FieldEffectStateWork;
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(4, 4, 16));
	sySynch(30);
	/* Unpause the Djinni, so its script and motion run. */
	djinni->paused = 0;
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySynch(20);

	/* Three hops, at 3, 5 and 7 pixels a frame. */
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 3, 14);
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 5, 16);
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 7, 18);
	sySynch(20);

	/* Eight afterimages of the same sprite at 15/16 size, each closing half the
	   distance to the one before it every frame, sharing one VRAM cache entry,
	   as in fdDjinnRunBattleStartHop. */
	spriteId = djinni->spr->entries[0]->anmId;
	sharedSprite = NULL;
	previous = djinni;
	cursor = copies;
	for(i = 0; i < MERCURY_AFTERIMAGE_MAX; i++) {
		copy = fdActorCreateStub(
			spriteId, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
		*cursor++ = copy;
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->motionFlg = 0;
			copy->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			copy->steeringFlg |= ACTOR_FACES_MOTION;
			copy->update = fdEffectHalveDistanceToTarget;
			copy->facing = djinni->facing;
			fdActorSetSprColModeStub(copy, SPRITE_COLOR_BLUE_TINT);
			fdActorSelectAnimationStub(copy, ANIM_STILL);
			fdActorSetSprFlagsStub(copy, 0);
			sharedSprite = fdSprShareVRAMCacheEntry(copy->spr, sharedSprite);
			copy->watchedActor = (union ACTOR_OR_EFFECT *)previous;
			previous = copy;
		}
	}

	/* Rise 8 pixels a frame for 15 frames, then remove the Djinni and its
	   afterimages, and free their shared VRAM cache entry
	   (VRAM_CACHE_ENTRY_COUNT is none). */
	sharedSlot = sharedSprite->slot;
	sySndPlayStub(SE_HIGH_JUMP);
	djinni->motionFlg = 0;
	for(i = 0; i < 15; i++) {
		djinni->y.fixed += toFIXED(8);
		sySynch(1);
	}
	fdActorReleaseStub(djinni);
	list = copies;
	for(i = 0; i < MERCURY_AFTERIMAGE_MAX; i++)
		fdActorReleaseStub(*list++);
	if(sharedSlot != VRAM_CACHE_ENTRY_COUNT)
		syVRAMCacheFreeEntry(sharedSlot);
	sySynch(10);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);
	sySynch(20);

	/* The droplets (SPRITE_DROPLET), one a frame, from where the Djinni was on
	   screen (fdEffectUpdateRandomTargetParticle moves them). */
	position.x = djinni->x.fixed;
	position.y = djinni->y.fixed;
	position.z = djinni->z.fixed;
	fdCameraWorldToScreen(&position);

	effect = effectState->particles;
	for(i = 0; i < MERCURY_DROPLETS; i++) {
		fdParticleInit(effect, SPRITE_DROPLET, position.x, position.z);
		fdParticleSetCallback(effect, fdEffectUpdateRandomTargetParticle);
		fdParticleSetAnimation(effect, 7);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_BLUE_TINT);
		sySynch(1);
		effect++;
	}

	/* After 120 frames every droplet still in use takes its last fall. */
	sySynch(120);
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active)
			effectState->particles[i].state = MERCURY_DROPLET_LAST_FALL;
	}

	/* Restore the colours. */
	sySynch(50);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

#undef MERCURY_DROPLETS
#undef MERCURY_AFTERIMAGE_MAX
