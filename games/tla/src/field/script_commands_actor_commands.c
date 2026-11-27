/* Script commands on actors: their callbacks, sprite priority and still
   pictures. Part of script_commands.c. */

#include "field/field_mode.h"
#include "field/speaker.h"
#include "graphics/glyph.h"

/* Clears an actor's destination, starts its idle script and stands it still. */
void fdActorClearDestinationAndStand(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* The script command "actor face direction": the actor turns toward angle
   (TRIG_TURN a full turn; fdActorStepAng), then the script waits the given
   number of ticks. */
void fdActorFaceDirectionById(Sint32 actorId, Sint32 ang, Sint32 ticks)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = ang;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdScriptWait(ticks);
	}
}

/* Turns an actor a step toward angle (ActorStepAngleScript) and waits for the
   script to end. */
void fdActorFaceDirectionAndRefreshById(Sint32 actorId, Sint32 angle)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = angle;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdActorWaitForScriptEndById(actorId);
	}
}

/* The script command "set actor sprite priority": fixes the priority of the
   actor's sprite and its shadow, which then no longer follow its depth: clears
   ACTOR_PRIORITY_AUTOMATIC. */
void fdActorSetSprPriority(Uint32 actorId, Sint32 priority)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;

		sprite->image.pri = priority;
		sprite->shadow.pri = priority;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Makes the target actor's sprite show the source actor's graphics (its VRAM
   block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(Uint32 tgtId, Uint32 srcId)
{
	ACTOR *actor;
	SPR	  *spr;
	Uint8  vramBlock;
	Uint32 tile;

	actor = fdActorGetById(srcId);
	spr = actor->spr;
	vramBlock = spr->slot;
	tile = spr->image.tile;

	actor = fdActorGetById(tgtId);
	spr = actor->spr;
	spr->slot = vramBlock;
	spr->image.tile = tile;
}

/* Stops actor id moving and colliding and hides it. */
void fdActorDisableById(Uint32 id)
{
	ACTOR *actor = fdActorGetById(id);

	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlagsStub(actor, 0);
}

/* Turns actor id's sprite to rotation (1/65536 turns). */
void fdActorSetSprRotationById(Sint32 id, Sint32 rotation)
{
	ACTOR *actor = fdActorGetById(id);
	SPR	  *spr = actor->spr;

	if(actor != NULL && spr != NULL)
		spr->rotation = rotation;
}

/* Lays actor id down along angle, a quarter turn rounded: shows its first tick
   turned, shifted by the side it falls to, and blocks its metatile and the
   metatile beyond it. */
void fdActorFallOverById(Uint32 id, Uint32 ang)
{
	FIXED  pos[3];
	ACTOR *actor = fdActorGetById(id);
	SPR	  *spr = actor->spr;

	if(actor != NULL && spr != NULL) {
		fdActorSelectAnimationStub(actor, ANIM_STILL);
		ang = (ang + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
		pos[0] = actor->x.fixed;
		pos[2] = actor->z.fixed;
		fdMapGridSetMetatileBehaviorAtStub(
			actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
		syVectorAddPolarOffset(toFIXED(16), ang, pos);
		fdMapGridSetMetatileBehaviorAtStub(
			actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
		switch(ang) {
		case FACING_EAST:
			spr->ofsX -= 3;
			spr->ofsY += 5;
			break;
		case FACING_WEST:
			spr->ofsX += 2;
			spr->ofsY += 4;
			break;
		case FACING_NORTH:
			spr->ofsY += 1;
			break;
		case FACING_SOUTH:
			spr->ofsY += 5;
			break;
		}
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		spr->flg = 0;
		spr->rotation = ang + DEGtoANG(90);
	}
}

/* fdActorFallOverById with an animation of the actor's sprite sheet (-1 for
   none) and the fall moving the actor itself half a metatile the way it
   falls. */
void fdActorFallOverWithAnimationById(Sint32 id, Uint32 ang, Sint32 anm)
{
	ACTOR *actor = fdActorGetById(id);
	SPR	  *spr = actor->spr;
	FIXED  pos[3];

	if(actor == NULL || spr == NULL)
		return;
	if(anm != -1)
		fdActorSelectAnimationStub(actor, anm);
	pos[0] = actor->x.fixed;
	ang = (ang + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	pos[2] = actor->z.fixed;
	fdMapGridSetMetatileBehaviorAtStub(
		actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
	syVectorAddPolarOffset(toFIXED(16), ang, pos);
	fdMapGridSetMetatileBehaviorAtStub(
		actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
	switch(ang) {
	case FACING_EAST:
		actor->x.fixed += toFIXED(8);
		actor->z.fixed += toFIXED(1);
		spr->ofsX -= 4;
		spr->ofsY += 15;
		break;
	case FACING_WEST:
		actor->x.fixed -= toFIXED(8);
		actor->z.fixed += toFIXED(1);
		spr->ofsX += 3;
		spr->ofsY += 16;
		break;
	case FACING_NORTH:
		spr->ofsY += 1;
		break;
	case FACING_SOUTH:
		actor->z.fixed += toFIXED(8);
		spr->ofsY += 13;
		break;
	}
	actor->collisionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	spr->flg = 0;
	spr->rotation = ang + DEGtoANG(90);
}

/* Item 0's icon, a gold coin, which an actor shows for coins. */
#define COIN_ICON_ITEM 0

/* Shows an actor as an item's icon, its sprite's only picture: the item, or for
   coins (SCOOPED_COINS) COIN_ICON_ITEM's. */
void fdActorShowItemIconById(Sint32 id, Sint32 item)
{
	ACTOR		  *actor = fdActorGetById(id);
	SPR			  *spr;
	ICON_TRANSFER *icon;
	Sint32		   tile;
	Uint8		   kind;

	if(actor != NULL) {
		kind = actor->sprStorageKind;
		if(kind == ACTOR_SPRITE_ONE) {
			spr = actor->spr;
			if(item & SCOOPED_COINS)
				item = COIN_ICON_ITEM;
			icon =
				syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
			DMA_FILL32(0, icon->tiles, ICON_TILE_BYTES);
#if FIX_FUNCTION_SIGNATURES
			mnIconDrawItemStub(item);
#else
			/* UB: mnIconDrawItem takes the item alone; the call passes the
			   tiles too. */
			((void (*)(Sint32, Uint8 *))mnIconDrawItemStub)(item, icon->tiles);
#endif
			tile = syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, icon->tiles);
			syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			actor->retainSprCache = kind;
			fdSprAnimationClearStub(spr->entries[0]);
			spr->entries[0] = NULL;
			spr->cnt = 0;
			spr->image.fullCol = 0;
			spr->image.tile = tile;
			spr->dirty = 0;
			spr->flg = 0;
		}
	}
}

/* The script command "set message": the number of the script's next message,
   written to the field state's message. */
void fdScriptSetMessageId(Sint32 msg)
{
	FieldStateWork->msg = msg;
}

/* Returns the sprite (the first animation's id) of the actor whose id is key's
   under SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor. */
Sint32 fdActorGetSprById(Sint32 key)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 result = -1;
	ACTOR		*actor = state->actors[key & SPEAKER_ACTOR_MASK];

	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		result = ((SPR *)actor->spr)->entries[0]->anmId;
	return result;
}

/* Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED)
   showing sprId, or -1. */
Sint32 fdActorFindBySpr(Sint32 sprId)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 result = -1;
	Sint32		 idx;
	ACTOR		*actor;

	for(idx = ACTOR_FIRST_PLACED; idx < FIELD_ACTOR_COUNT; idx++) {
		actor = state->actors[idx];
		if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE &&
			((SPR *)actor->spr)->entries[0]->anmId == sprId) {
			result = idx;
			break;
		}
	}
	return result;
}
