/* The view bounds, zoom transitions and actors that ride another. Part of
   script_commands.c. */

#include "field/perspective.h"

/* fdActorShowEmoteById's emote: the bubble (EMOTE) in its low byte, of which
   the low four bits pick its animation, then flags (EMOTE_IN_FRONT). */
#define EMOTE_BUBBLE_MASK	 0xff
#define EMOTE_ANIMATION_MASK 15

/* Sets the map coordinates the camera keeps the screen inside. */
void fdMapGridSetCameraBounds(
	Sint32 minX, Sint32 minZ, Sint32 maxX, Sint32 maxZ)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;

	map->minX = minX;
	map->minZ = minZ;
	map->maxX = maxX;
	map->maxZ = maxZ;
}

/* Each frame of a World Map zoom: moves the zoom from zoomFrom toward zoomTo
   over zoomFrm frames. */
void fdWorldMapStepZoom(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint16	  *duration;
	Sint32	  *from;
	Sint32	   offset;
	Sint32	   delta;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
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
		syCloseEvent((EVENT_FUNC)fdWorldMapStepZoom);
	}
}

/* Starts a zoom to zoom (16.16) over frames frames, on the World Map only:
   fdWorldMapStepZoom takes the view's MAP_STATE.zoom to farPlane / zoom. */
void fdWorldMapStartZoom(FIXED zoom, Sint32 frames)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   result;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->mode != ACTOR_SYSTEM_WORLD_MAP)
		return;
	result = syDivFX(zoom, toFIXED(1));
	map->zoomFrom = map->zoomTo;
	map->zoomTo = result;
	map->zoomFrm = frames;
	map->zoomStep = 0;
	sySetEvent(fdWorldMapStepZoom, EVENT_PASS_FRAME + 20);
}

/* Waits for a zoom transition to end (on the World Map only, at most 300
   frames). */
void fdScriptWaitForZoomTransition(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   frames;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->mode == ACTOR_SYSTEM_WORLD_MAP) {
		frames = 0;
		if(map->zoomFrm != 0) {
			for(; frames < 300 && map->zoomFrm != 0; frames++) {
				sySynch(1);
			}
		}
	}
}

/* Per-frame update: keeps the actor on the actor it watches, raised by the
   height of that actor's sprite and 8 pixels more. */
Sint32 fdEffectCopyWatchedActorPos(ACTOR *actor)
{
	ACTOR *host;

	host = (ACTOR *)actor->watchedActor;
	if(host != NULL) {
		actor->motionFlg = 0;
		actor->x.fixed = host->x.fixed;
		actor->y.fixed = host->y.fixed +
			toFIXED((Sint8)fdResourceGetMetadataRecordStubStub(
				((ACTOR_PLACE *)fdFindActorPlacement(actor->userValue))->spr)
					->sprTop) +
			toFIXED(8);
		actor->groundHeight = host->groundHeight;
		actor->z.fixed = host->z.fixed;
	}
	return 0;
}

extern const Uint8 Emote_BubbleScript[];

/* The script command "actor show emote": shows an emote bubble (SPRITE_EMOTE)
   over an actor, then waits the given number of frames. Returns the bubble. */
ACTOR *fdActorShowEmoteById(Sint32 actorId, Sint32 emote, Sint32 frames)
{
	ACTOR *target;
	ACTOR *bubble;

	if((emote & EMOTE_BUBBLE_MASK) == EMOTE_LIGHT_BULB) {
		sySndPlayStub(SE_CHIME);
	}

	target = fdActorLookup(actorId);
	if(target != 0) {
		bubble = fdActorCreateStub(
			SPRITE_EMOTE, target->x.fixed, target->y.fixed, target->z.fixed);
		if(bubble != 0) {
			SPR *sprite;

			fdActorStartScriptStub(bubble, (const Sint32 *)Emote_BubbleScript);
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
		fdScriptWait(frames);
	}
	return bubble;
}

/* Returns 0 while the actor's sprite is an attached effect (SPRITE_RAY_ARC),
   else 1. */
Bool fdActorIsNotAttachedEffect(ACTOR *actor)
{
	SPR	  *sprite;
	Sint32 value;

	if(actor == 0 || actor->sprStorageKind != ACTOR_SPRITE_ONE ||
		(sprite = actor->spr) == 0 || sprite->entries[0] == 0)
		return TRUE;
	value = sprite->entries[0]->anmId;
	if(value != SPRITE_RAY_ARC)
		return TRUE;
	return FALSE;
}
