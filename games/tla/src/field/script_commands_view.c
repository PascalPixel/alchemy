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

/* Each tick of a World Map zoom: moves the zoom from zoomFrom toward zoomTo
   over zoomFrm ticks. */
void fdWorldMapStepZoom(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint16	  *duration;
	Sint32	  *from;
	Sint32	   ofs;
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
	ofs = *from + delta * map->zoomStep / *duration;
	map->zoom = syMulFX(map->farPlane, ofs);
	ScanlinePitch = ((WORLD_MAP_VIEW *)map)->pitch + 1;
	if(map->zoomStep == *duration) {
		*duration = 0;
		syCloseEvent((EVENT_FUNC)fdWorldMapStepZoom);
	}
}

/* Starts a zoom to zoom (16.16) over ticks ticks, on the World Map only:
   fdWorldMapStepZoom takes the view's MAP_STATE.zoom to farPlane / zoom. */
void fdWorldMapStartZoom(FIXED zoom, Sint32 ticks)
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
	map->zoomFrm = ticks;
	map->zoomStep = 0;
	sySetEvent(fdWorldMapStepZoom, EVENT_PASS_TICK + 20);
}

/* Waits for a zoom transition to end (on the World Map only, at most 300
   ticks). */
void fdScriptWaitForZoomTransition(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   ticks;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->mode == ACTOR_SYSTEM_WORLD_MAP) {
		ticks = 0;
		if(map->zoomFrm != 0) {
			for(; ticks < 300 && map->zoomFrm != 0; ticks++) {
				sySynch(1);
			}
		}
	}
}

/* Per-tick update: keeps the actor on the actor it watches, raised by the
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

extern const Uint8 EmoteBubbleScript[];

/* The script command "actor show emote": shows an emote bubble (SPRITE_EMOTE)
   over an actor, then waits the given number of ticks. Returns the bubble. */
ACTOR *fdActorShowEmoteById(Sint32 actorId, Sint32 emote, Sint32 ticks)
{
	ACTOR *tgt;
	ACTOR *bubble;

	if((emote & EMOTE_BUBBLE_MASK) == EMOTE_LIGHT_BULB) {
		sySndPlayStub(SE_CHIME);
	}

	tgt = fdActorLookup(actorId);
	if(tgt != 0) {
		bubble = fdActorCreateStub(
			SPRITE_EMOTE, tgt->x.fixed, tgt->y.fixed, tgt->z.fixed);
		if(bubble != 0) {
			SPR *spr;

			fdActorStartScriptStub(bubble, (const Sint32 *)EmoteBubbleScript);
			fdActorSelectAnimationStub(bubble, emote & EMOTE_ANIMATION_MASK);
			bubble->motionFlg = 0;
			bubble->tmr = 0;
			bubble->userValue = actorId;
			bubble->update = (void (*)(ACTOR *))fdEffectCopyWatchedActorPos;
			bubble->watchedActor = (union FIELD_OBJECT *)tgt;
			spr = bubble->spr;
			spr->flg = 0;

			/* The bubble takes sprite priority 1, or the actor's. */
			if((emote & EMOTE_IN_FRONT) != 0)
				((PLACED_SPR *)spr)->pri = 1;
			else
				((PLACED_SPR *)spr)->pri = ((PLACED_SPR *)tgt->spr)->pri;
		}
		fdScriptWait(ticks);
	}
	return bubble;
}

/* Returns 0 while the actor's sprite is an attached effect (SPRITE_RAY_ARC),
   else 1. */
Bool fdActorIsNotAttachedEffect(ACTOR *actor)
{
	SPR	  *spr;
	Sint32 value;

	if(actor == 0 || actor->sprStorageKind != ACTOR_SPRITE_ONE ||
		(spr = actor->spr) == 0 || spr->entries[0] == 0)
		return TRUE;
	value = spr->entries[0]->anmId;
	if(value != SPRITE_RAY_ARC)
		return TRUE;
	return FALSE;
}
