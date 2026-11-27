/* The view bounds, zoom transitions and actors that ride another. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* The high byte of the first sprite part's OAM attribute 2, which holds its
   priority and palette. */
#define SPRITE_ATTR2_HIGH(sprite) (((Uint8 *)(sprite))[9])
/* The priority's bits in that byte. */
#define ATTR2_HIGH_PRIORITY_MASK  (OBJ_PRIORITY(3) >> 8)

/* fdActorShowEmoteById's emote: the bubble (EMOTE) in the bits below its flags
   (EMOTE_IN_FRONT), of which the low four pick its animation. */
#define EMOTE_BUBBLE_MASK	 (EMOTE_IN_FRONT - 1)
#define EMOTE_ANIMATION_MASK 15

/* fdEffectSetAttached's flags: the ATTACHED_EFFECT_FLAG below its
   ATTACHED_EFFECT_IN_FRONT. */
#define ATTACHED_EFFECT_KIND_MASK 3

/* The script command "set view bounds": the camera keeps the screen inside
   these map coordinates (fdCameraMoveWithinMapBounds). */
void fdMapGridSetCameraBounds(
	Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];

	grid->minX = minX;
	grid->minY = minY;
	grid->maxX = maxX;
	grid->maxY = maxY;
}

/* Event: moves the zoom one step from its previous value toward its target, and
   removes itself on the last step. */
UNREAD_RESULT fdWorldMapStepZoom(void)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	Sint32		   *from; /* the zoom the transition starts from */
	Sint32			delta;
	Sint32			zoom;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->camera->paused != 0)
		return;
	if(perspective->zoomTransitionFrm == 0)
		return;
	from = &perspective->prevZoom;
	delta = perspective->zoom - *from;
	perspective->zoomTransitionStep++;
	zoom = *from +
		delta * perspective->zoomTransitionStep /
			perspective->zoomTransitionFrm;
	perspective->distance = syMulFX(perspective->farPlane, zoom);
	ScanlinePitch = perspective->pitch + 1;
	if(perspective->zoomTransitionStep == perspective->zoomTransitionFrm) {
		perspective->zoomTransitionFrm = 0;
		syCloseEvent((EVENT_FUNC)fdWorldMapStepZoom);
	}
}

/* Starts a zoom to zoom (16.16) over frames frames, on the World Map only. */
void fdWorldMapStartZoom(FIXED zoom, Sint32 frames)
{
	WORLD_MAP_VIEW *perspective = MapGridState[0];
	FIXED			inverse;

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->mode != ACTOR_SYSTEM_WORLD_MAP)
		return;
	{
		inverse = syDivFX(zoom, toFIXED(1)); /* 1 / zoom */
	}
	perspective->prevZoom = perspective->zoom;
	perspective->zoom = inverse;
	perspective->zoomTransitionFrm = frames;
	perspective->zoomTransitionStep = 0;
	sySetEvent((EVENT_FUNC)fdWorldMapStepZoom, EVENT_PASS_FRAME + 20);
}

/* Waits for a zoom transition to end (on the World Map only, at most 300
   frames). */
void fdScriptWaitForZoomTransition(void)
{
	Sint32			frames;
	WORLD_MAP_VIEW *perspective = MapGridState[0];

	if(((FIELD_STATE *)syHeapSlotAllocEWRAM(
			HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->mode == ACTOR_SYSTEM_WORLD_MAP) {
		frames = 0;
		if(perspective->zoomTransitionFrm != 0) {
			for(; frames < 300 && perspective->zoomTransitionFrm != 0;
				frames++) {
				sySynch(1);
			}
		}
	}
}

/* Per-frame update of an emote bubble or attached effect: keeps it on the actor
   it rides (its watched actor, whose id is its userValue), raised by that
   actor's sprite height (its metadata's sprTop) and 8 pixels. */
UNREAD_RESULT fdEffectCopyWatchedActorPos(ACTOR *actor)
{
	ACTOR *host;

	host = (ACTOR *)actor->watchedActor;
	if(host != NULL) {
		actor->motionFlg = 0;
		actor->x.fixed = host->x.fixed;
		actor->y.fixed = host->y.fixed +
			toFIXED((Sint8)syResourceGetMetadataRecordStub(
				(Sint16)fdFindActorPlacement(actor->userValue)->spr)
					->sprTop) +
			toFIXED(8);
		actor->groundHeight = host->groundHeight;
		actor->z.fixed = host->z.fixed;
	}
#if !FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* The script command "actor show emote": shows an emote bubble (SPRITE_EMOTE)
   over an actor, then waits the given number of frames. */
void fdActorShowEmoteById(Sint32 actorId, Sint32 emote, Sint32 frames)
{
	ACTOR *target;

	if((emote & EMOTE_BUBBLE_MASK) == EMOTE_LIGHT_BULB) {
		sySndPlayStub(SE_CHIME);
	}

	target = fdActorLookup(actorId);
	if(target != 0) {
		ACTOR *bubble = fdActorCreateStub(
			SPRITE_EMOTE, target->x.fixed, target->y.fixed, target->z.fixed);

		if(bubble != 0) {
			SPR *sprite;

			fdActorStartScriptStub(bubble, Emote_BubbleScript);
			fdActorSelectAnimationStub(bubble, emote & EMOTE_ANIMATION_MASK);
			bubble->motionFlg = 0;
			bubble->tmr = 0;
			bubble->userValue = actorId;
			sprite = bubble->spr;
#if FIX_FUNCTION_SIGNATURES
			bubble->update = fdEffectCopyWatchedActorPos;
#else
			bubble->update = (void (*)(ACTOR *))fdEffectCopyWatchedActorPos;
#endif
			sprite->flg = 0;
			bubble->watchedActor = (union ACTOR_OR_EFFECT *)target;

			/* The bubble takes sprite priority 1, or the actor's: -(m + 1) is
			   ~m, the bits besides the priority. */
			if((emote & EMOTE_IN_FRONT) != 0) {
				Sint32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;
				Uint8  attr = SPRITE_ATTR2_HIGH(sprite);

				mask = -mask;
				mask &= attr;
				mask |= OBJ_PRIORITY(1) >> 8;
				SPRITE_ATTR2_HIGH(sprite) = mask;
			} else {
				Sint32 priority = ATTR2_HIGH_PRIORITY_MASK;
				Uint8  src = SPRITE_ATTR2_HIGH(target->spr);
				Uint8  dst;
				Sint32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;

				priority &= src;
				dst = SPRITE_ATTR2_HIGH(sprite);
				mask = -mask;
				mask &= dst;
				mask |= priority;
				SPRITE_ATTR2_HIGH(sprite) = mask;
			}
		}
		fdScriptWait(frames);
	}
}

/* The script command "actor set attached effect", by ATTACHED_EFFECT_FLAG:
   removes the actor attached to an actor, attaches one (SPRITE_ATTACHED_EFFECT,
   animation 1) or shows a one-off one (animation 2) that ends by itself
   (Actor_WaitScript, 40 frames). */
void fdEffectSetAttached(Sint32 actorId, Sint32 flags)
{
	ACTOR *actor = fdActorLookup(actorId);
	ACTOR *effect;
	SPR	  *visual;
	Sint32 mode;

	effect = 0;
	visual = 0;

	if(actor == 0)
		return;

	if((flags & ATTACHED_EFFECT_KIND_MASK) != 0) {
		if((flags & ATTACHED_EFFECT_KIND_MASK) == ATTACHED_EFFECT_ONCE ||
			actor->watchedActor == 0) {
			effect = fdActorCreateStub(SPRITE_ATTACHED_EFFECT, actor->x.fixed,
				actor->y.fixed, actor->z.fixed);
		}
	} else {
		effect = (ACTOR *)actor->watchedActor;
		if(effect == 0)
			return;
		fdActorReleaseStub(effect);
		actor->watchedActor = 0;
		return;
	}

	if(effect == 0)
		return;

	mode = flags & ATTACHED_EFFECT_KIND_MASK;
	switch(mode) {
	case ATTACHED_EFFECT_ATTACH:
		fdActorSelectAnimationStub(effect, 1);
		actor->watchedActor = (union ACTOR_OR_EFFECT *)effect;
		effect->tmr = 1;
		break;
	case ATTACHED_EFFECT_ONCE:
		fdActorSelectAnimationStub(effect, 2);
		fdActorStartScriptStub(effect, (const Sint32 *)Actor_WaitScript);
		effect->tmr = 1;
		break;
	}

	effect->userValue = actorId;
	effect->motionFlg = 0;
	effect->update = (void (*)(ACTOR *))fdEffectCopyWatchedActorPos;
	visual = effect->spr;
	visual->flg = 0;
	effect->watchedActor = (union ACTOR_OR_EFFECT *)actor;

	/* The effect takes sprite priority 1, or the actor's, as an emote bubble
	   does. */
	if(flags & ATTACHED_EFFECT_IN_FRONT) {
		Sint32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;
		Uint8  attr = SPRITE_ATTR2_HIGH(visual);

		mask = -mask;
		mask &= attr;
		mask |= OBJ_PRIORITY(1) >> 8;
		SPRITE_ATTR2_HIGH(visual) = mask;
	} else {
		Sint32 priority = ATTR2_HIGH_PRIORITY_MASK;
		Uint8  src = SPRITE_ATTR2_HIGH(actor->spr);
		Uint8  dst;
		Sint32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;

		priority &= src;
		dst = SPRITE_ATTR2_HIGH(visual);
		mask = -mask;
		mask &= dst;
		mask |= priority;
		SPRITE_ATTR2_HIGH(visual) = mask;
	}
}

#undef SPRITE_ATTR2_HIGH
#undef ATTR2_HIGH_PRIORITY_MASK
#undef EMOTE_BUBBLE_MASK
#undef EMOTE_ANIMATION_MASK
#undef ATTACHED_EFFECT_KIND_MASK
