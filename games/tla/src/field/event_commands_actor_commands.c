/* @file */
/* Event commands on actors: their callbacks, sprite priority and still */
/* pictures. */
/* Part of event_commands.c. */

#include "field/field_sequence.h"
#include "field/speaker.h"
#include "graphics/glyph.h"

/* Clears an actor's destination, starts its idle script and stands it */
/* still. */
void fdActorClearDestinationAndStand(u32 actorId) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* The event command "actor face direction": the actor turns toward angle */
/* (TRIG_TURN a full turn; fdActorStepAng), then the event waits the given */
/* number of frames. */
void fdActorFaceDirectionById(s32 actorId, s32 angle, s32 frames) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = angle;
		fdActorStartScriptStub(actor, (const s32 *)Actor_StepAngleScript);
		fdEventWait(frames);
	}
}

/* Turns an actor a step toward angle (Actor_StepAngleScript) and waits for */
/* the script to end. */
void fdActorFaceDirectionAndRefreshById(s32 actorId, s32 angle) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = angle;
		fdActorStartScriptStub(actor, (const s32 *)Actor_StepAngleScript);
		fdActorWaitForScriptEndById(actorId);
	}
}

/* The event command "set actor sprite priority": fixes the priority of the */
/* actor's sprite and its shadow, which then no longer follow its depth: */
/* clears ACTOR_PRIORITY_AUTOMATIC. */
void fdActorSetSprPriority(u32 actorId, s32 priority) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL &&
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;

		sprite->image.pri = priority;
		sprite->shadow.pri = priority;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Makes the target actor's sprite show the source actor's graphics (its VRAM */
/* block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(u32 targetId, u32 sourceId) {
	ACTOR *actor;
	SPR *sprite;
	u8 vramBlock;
	u32 tile;

	actor = fdActorGetById(sourceId);
	sprite = actor->spr;
	vramBlock = sprite->slot;
	tile = sprite->image.tile;

	actor = fdActorGetById(targetId);
	sprite = actor->spr;
	sprite->slot = vramBlock;
	sprite->image.tile = tile;
}

/* Stops actor id moving and colliding and hides it. */
void fdActorDisableById(u32 id) {
	ACTOR *actor = fdActorGetById(id);

	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlagsStub(actor, 0);
}

/* Turns actor id's sprite to rotation (1/65536 turns). */
void fdActorSetSprRotationById(s32 id, s32 rotation) {
	ACTOR *actor = fdActorGetById(id);
	SPR *sprite = actor->spr;

	if(actor != NULL && sprite != NULL)
		sprite->rotation = rotation;
}

/* Lays actor id down along angle, a quarter turn rounded: shows its first */
/* frame turned, shifted by the side it falls to, and blocks its cell and the */
/* cell beyond it. */
void fdActorFallOverById(u32 id, u32 angle) {
	FIXED position[3];
	ACTOR *actor = fdActorGetById(id);
	SPR *sprite = actor->spr;

	if(actor != NULL && sprite != NULL) {
		fdActorSelectAnimationStub(actor, ANIM_STILL);
		angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
		position[0] = actor->x.fixed;
		position[2] = actor->z.fixed;
		fdMapGridSetCellBehaviorAtStub(actor->layer, position[0], position[2],
									   CELL_BEHAVIOR_WALL);
		syVectorAddPolarOffset(toFIXED(16), angle, position);
		fdMapGridSetCellBehaviorAtStub(actor->layer, position[0], position[2],
									   CELL_BEHAVIOR_WALL);
		switch(angle) {
		case FACING_EAST:
			sprite->ofsX -= 3;
			sprite->ofsY += 5;
			break;
		case FACING_WEST:
			sprite->ofsX += 2;
			sprite->ofsY += 4;
			break;
		case FACING_NORTH:
			sprite->ofsY += 1;
			break;
		case FACING_SOUTH:
			sprite->ofsY += 5;
			break;
		}
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		sprite->flg = 0;
		sprite->rotation = angle + DEGtoANG(90);
	}
}

/* fdActorFallOverById with an animation of the actor's sprite sheet (-1 for */
/* none) and the fall moving the actor itself half a cell the way it falls. */
void fdActorFallOverWithAnimationById(s32 id, u32 angle, s32 animation) {
	ACTOR *actor = fdActorGetById(id);
	SPR *sprite = actor->spr;
	FIXED position[3];

	if(actor == NULL || sprite == NULL)
		return;
	if(animation != -1)
		fdActorSelectAnimationStub(actor, animation);
	position[0] = actor->x.fixed;
	angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	position[2] = actor->z.fixed;
	fdMapGridSetCellBehaviorAtStub(actor->layer, position[0], position[2],
								   CELL_BEHAVIOR_WALL);
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	fdMapGridSetCellBehaviorAtStub(actor->layer, position[0], position[2],
								   CELL_BEHAVIOR_WALL);
	switch(angle) {
	case FACING_EAST:
		actor->x.fixed += toFIXED(8);
		actor->z.fixed += toFIXED(1);
		sprite->ofsX -= 4;
		sprite->ofsY += 15;
		break;
	case FACING_WEST:
		actor->x.fixed -= toFIXED(8);
		actor->z.fixed += toFIXED(1);
		sprite->ofsX += 3;
		sprite->ofsY += 16;
		break;
	case FACING_NORTH:
		sprite->ofsY += 1;
		break;
	case FACING_SOUTH:
		actor->z.fixed += toFIXED(8);
		sprite->ofsY += 13;
		break;
	}
	actor->collisionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	sprite->flg = 0;
	sprite->rotation = angle + DEGtoANG(90);
}

/* Item 0's icon, a gold coin, which an actor shows for coins. */
#define COIN_ICON_ITEM 0

/* Shows an actor as an item's icon, its sprite's only picture: the item, or */
/* for coins (SCOOPED_COINS) COIN_ICON_ITEM's. */
void fdActorShowItemIconById(s32 id, s32 item) {
	ACTOR *actor = fdActorGetById(id);
	SPR *sprite;
	ICON_TRANSFER *icon;
	s32 tile;
	u8 kind;

	if(actor != NULL) {
		kind = actor->sprStorageKind;
		if(kind == ACTOR_SPRITE_ONE) {
			sprite = actor->spr;
			if(item & SCOOPED_COINS)
				item = COIN_ICON_ITEM;
			icon =
				syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
			btDmaFill32(0, icon->tiles, ICON_TILE_BYTES);
#ifdef UBFIX
			mnIconDrawItemStub(item);
#else
			/* UB: mnIconDrawItem takes the item alone; the call passes the
			 * tiles too. */
			((void (*)(s32, u8 *))mnIconDrawItemStub)(item, icon->tiles);
#endif
			tile = syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, icon->tiles);
			syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			actor->retainSprCache = kind;
			fdSprAnimationClearStub(sprite->entries[0]);
			sprite->entries[0] = NULL;
			sprite->cnt = 0;
			sprite->image.fullCol = 0;
			sprite->image.tile = tile;
			sprite->dirty = 0;
			sprite->flg = 0;
		}
	}
}

/* The event command "set message": the number of the event's next message, */
/* written to the field state's message. */
void fdEventSetMessageId(s32 message) { FieldStateWork->msg = message; }

/* Returns the sprite (the first animation's id) of the actor whose id is */
/* key's under SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor. */
s32 fdActorGetSprById(s32 key) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 result = -1;
	ACTOR *actor = state->actors[key & SPEAKER_ACTOR_MASK];

	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		result = ((SPR *)actor->spr)->entries[0]->anmId;
	return result;
}

/* Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED) */
/* showing spriteId, or -1. */
s32 fdActorFindBySpr(s32 spriteId) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 result = -1;
	s32 index;
	ACTOR *actor;

	for(index = ACTOR_FIRST_PLACED; index < FIELD_ACTOR_COUNT; index++) {
		actor = state->actors[index];
		if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE &&
		   ((SPR *)actor->spr)->entries[0]->anmId == spriteId) {
			result = index;
			break;
		}
	}
	return result;
}
