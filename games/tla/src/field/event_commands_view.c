/* @file */
/* The view bounds, zoom transitions and actors that ride another. */
/* Part of event_commands.c. */

#include "field/perspective.h"

/* fdActorShowEmoteById's emote: the bubble (EMOTE) in its low byte, of */
/* which the low four bits pick its animation, then flags (EMOTE_IN_FRONT). */
#define EMOTE_BUBBLE_MASK	 0xff
#define EMOTE_ANIMATION_MASK 15

/* Sets the map coordinates the camera keeps the screen inside. */
void fdMapGridSetCameraBounds(s32 minX, s32 minZ, s32 maxX, s32 maxZ) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;

	map->minX = minX;
	map->minZ = minZ;
	map->maxX = maxX;
	map->maxZ = maxZ;
}

/* Each frame of a World Map zoom: moves the zoom from zoomFrom toward zoomTo */
/* over zoomFrm frames. */
void fdWorldMapStepZoom(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s16 *duration;
	s32 *from;
	s32 offset;
	s32 delta;

	if(((FIELD_STATE *)syHeapSlotAllocEwram(HEAP_SLOT_FIELD,
											sizeof(FIELD_STATE)))
		   ->camera->paused != 0)
		return;
	duration = &map->zoomFrm;
	if(*duration == 0)
		return;
	from = &map->zoomFrom;
	delta = map->zoomTo - *from;
	map->zoomStep++;
	offset = *from + delta * map->zoomStep / *duration;
	map->zoom = syMulFX(map->farPlane, offset);
	ScanlinePitch = ((WORLD_MAP_VIEW *)map)->pitch + 1;
	if(map->zoomStep == *duration) {
		*duration = 0;
		syCloseEvent((TASK_FUNC)fdWorldMapStepZoom);
	}
}

/* Starts a zoom to zoom (16.16) over frames frames, on the World Map only: */
/* fdWorldMapStepZoom takes the view's MAP_STATE.zoom to farPlane / zoom. */
void fdWorldMapStartZoom(FIXED zoom, s32 frames) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 result;

	if(((FIELD_STATE *)syHeapSlotAllocEwram(HEAP_SLOT_FIELD,
											sizeof(FIELD_STATE)))
		   ->mode != ACTOR_SYSTEM_WORLD_MAP)
		return;
	result = syDivFX(zoom, FIXED_ONE);
	map->zoomFrom = map->zoomTo;
	map->zoomTo = result;
	map->zoomFrm = frames;
	map->zoomStep = 0;
	sySetEvent(fdWorldMapStepZoom, TASK_PASS_FRAME + 20);
}

/* Waits for a zoom transition to end (on the World Map only, at most 300 */
/* frames). */
void fdEventWaitForZoomTransition(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 frames;

	if(((FIELD_STATE *)syHeapSlotAllocEwram(HEAP_SLOT_FIELD,
											sizeof(FIELD_STATE)))
		   ->mode == ACTOR_SYSTEM_WORLD_MAP) {
		frames = 0;
		if(map->zoomFrm != 0) {
			for(; frames < 300 && map->zoomFrm != 0; frames++) {
				sySynch(1);
			}
		}
	}
}

/* Per-frame update: keeps the actor on the actor it watches, raised by the */
/* height of that actor's sprite and 8 pixels more. */
s32 fdEffectCopyWatchedActorPos(ACTOR *actor) {
	ACTOR *host;

	host = (ACTOR *)actor->watchedActor;
	if(host != NULL) {
		actor->motionFlg = 0;
		actor->x.fixed = host->x.fixed;
		actor->y.fixed =
			host->y.fixed +
			toFIXED((s8)syResourceGetMetadataRecordFarStub(
						((ACTOR_PLACE *)scFindActorPlacement(actor->userValue))
							->spr)
						->sprTop) +
			toFIXED(8);
		actor->groundHeight = host->groundHeight;
		actor->z.fixed = host->z.fixed;
	}
	return 0;
}

extern const u8 Emote_BubbleScript[];

/* The event command "actor show emote": shows an emote bubble (SPRITE_EMOTE) */
/* over an actor, then waits the given number of frames. Returns the bubble. */
ACTOR *fdActorShowEmoteById(s32 actorId, s32 emote, s32 frames) {
	ACTOR *target;
	ACTOR *bubble;

	if((emote & EMOTE_BUBBLE_MASK) == EMOTE_LIGHT_BULB) {
		sySndPlayStub(SE_CHIME);
	}

	target = fdActorLookup(actorId);
	if(target != 0) {
		bubble = fdActorCreateStub(SPRITE_EMOTE, target->x.fixed,
								   target->y.fixed, target->z.fixed);
		if(bubble != 0) {
			SPR *sprite;

			fdActorStartScriptStub(bubble, (const s32 *)Emote_BubbleScript);
			fdActorSelectAnimationStub(bubble, emote & EMOTE_ANIMATION_MASK);
			bubble->motionFlg = 0;
			bubble->tmr = 0;
			bubble->userValue = actorId;
			bubble->update = (void (*)(ACTOR *))fdEffectCopyWatchedActorPos;
			bubble->watchedActor = (union FIELD_OBJECT *)target;
			sprite = bubble->spr;
			sprite->flg = 0;

			/* The bubble takes sprite priority 1, or the actor's. */
			if((emote & EMOTE_IN_FRONT) != 0)
				((PLACED_SPR *)sprite)->pri = 1;
			else
				((PLACED_SPR *)sprite)->pri = ((PLACED_SPR *)target->spr)->pri;
		}
		fdEventWait(frames);
	}
	return bubble;
}

/* Returns 0 while the actor's sprite is an attached effect */
/* (SPRITE_RAY_ARC), else 1. */
s32 fdActorIsNotAttachedEffect(ACTOR *actor) {
	SPR *sprite;
	s32 value;

	if(actor == 0 || actor->sprStorageKind != ACTOR_SPRITE_ONE ||
	   (sprite = actor->spr) == 0 || sprite->entries[0] == 0)
		return 1;
	value = sprite->entries[0]->anmId;
	if(value != SPRITE_RAY_ARC)
		return 1;
	return 0;
}
