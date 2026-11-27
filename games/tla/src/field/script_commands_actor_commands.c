// Script commands on actors: their callbacks, sprite priority and still pictures. Part of
// script_commands.c.

#include "field/field_mode.h"
#include "field/speaker.h"
#include "graphics/glyph.h"

// Clears an actor's destination, starts its idle script and stands it still.
void fdActorClearDestinationAndStand(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

// The script command "actor face direction": the actor turns toward angle (TRIG_TURN a full turn;
// fdActorStepAngle), then the script waits the given number of ticks.
void fdActorFaceDirectionById(Sint32 actorId, Sint32 angle, Sint32 ticks)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->timer = angle;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdScriptWait(ticks);
	}
}

// Turns an actor a step toward angle (ActorStepAngleScript) and waits for the script to end.
void fdActorFaceDirectionAndRefreshById(Sint32 actorId, Sint32 angle)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->timer = angle;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdActorWaitForScriptEndById(actorId);
	}
}

// The script command "set actor sprite priority": fixes the priority of the actor's sprite and its
// shadow, which then no longer follow its depth: clears ACTOR_PRIORITY_AUTOMATIC.
void fdActorSetSpritePriority(Uint32 actorId, Sint32 priority)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL && (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
	{
		SPR *sprite              = actor->sprite;

		sprite->image.priority   = priority;
		sprite->shadow.priority  = priority;
		actor->priorityFlags    &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

// Makes the target actor's sprite show the source actor's graphics (its VRAM block and tile).
// Neither actor is checked for NULL.
void fdActorCopyGraphics(Uint32 targetId, Uint32 srcId)
{
	ACTOR *actor;
	SPR   *sprite;
	Uint8  vramBlock;
	Uint32 tile;

	actor              = fdActorGetById(srcId);
	sprite             = actor->sprite;
	vramBlock          = sprite->slot;
	tile               = sprite->image.tile;

	actor              = fdActorGetById(targetId);
	sprite             = actor->sprite;
	sprite->slot       = vramBlock;
	sprite->image.tile = tile;
}

// Stops actor id moving and colliding and hides it.
void fdActorDisableById(Uint32 id)
{
	ACTOR *actor          = fdActorGetById(id);

	actor->motionFlags    = 0;
	actor->collisionFlags = 0;
	fdActorSetSpriteFlagsStub(actor, 0);
}

// Turns actor id's sprite to rotation (1/65536 turns).
void fdActorSetSpriteRotationById(Sint32 id, Sint32 rotation)
{
	ACTOR *actor  = fdActorGetById(id);
	SPR   *sprite = actor->sprite;

	if (actor != NULL && sprite != NULL)
		sprite->rotation = rotation;
}

// Lays actor id down along angle, a quarter turn rounded: shows its first tick turned, shifted by
// the side it falls to, and blocks its metatile and the metatile beyond it.
void fdActorFallOverById(Uint32 id, Uint32 angle)
{
	FIXED  pos[3];
	ACTOR *actor  = fdActorGetById(id);
	SPR   *sprite = actor->sprite;

	if (actor != NULL && sprite != NULL)
	{
		fdActorSelectAnimationStub(actor, ANIM_STILL);
		angle  = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
		pos[0] = actor->x.fixed;
		pos[2] = actor->z.fixed;
		fdMapGridSetMetatileBehaviorAtStub(actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
		syVectorAddPolarOffset(toFIXED(16), angle, pos);
		fdMapGridSetMetatileBehaviorAtStub(actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
		switch (angle)
		{
			case FACING_EAST:
				sprite->offsetX -= 3;
				sprite->offsetY += 5;
				break;
			case FACING_WEST:
				sprite->offsetX += 2;
				sprite->offsetY += 4;
				break;
			case FACING_NORTH: sprite->offsetY += 1; break;
			case FACING_SOUTH: sprite->offsetY += 5; break;
		}
		actor->collisionFlags  = 0;
		actor->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
		sprite->flags          = 0;
		sprite->rotation       = angle + DEGtoANG(90);
	}
}

// fdActorFallOverById with an animation of the actor's sprite sheet (-1 for none) and the fall
// moving the actor itself half a metatile the way it falls.
void fdActorFallOverWithAnimationById(Sint32 id, Uint32 angle, Sint32 animation)
{
	ACTOR *actor  = fdActorGetById(id);
	SPR   *sprite = actor->sprite;
	FIXED  pos[3];

	if (actor == NULL || sprite == NULL)
		return;
	if (animation != -1)
		fdActorSelectAnimationStub(actor, animation);
	pos[0] = actor->x.fixed;
	angle  = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	pos[2] = actor->z.fixed;
	fdMapGridSetMetatileBehaviorAtStub(actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
	syVectorAddPolarOffset(toFIXED(16), angle, pos);
	fdMapGridSetMetatileBehaviorAtStub(actor->layer, pos[0], pos[2], METATILE_BEHAVIOR_WALL);
	switch (angle)
	{
		case FACING_EAST:
			actor->x.fixed  += toFIXED(8);
			actor->z.fixed  += toFIXED(1);
			sprite->offsetX -= 4;
			sprite->offsetY += 15;
			break;
		case FACING_WEST:
			actor->x.fixed  -= toFIXED(8);
			actor->z.fixed  += toFIXED(1);
			sprite->offsetX += 3;
			sprite->offsetY += 16;
			break;
		case FACING_NORTH: sprite->offsetY += 1; break;
		case FACING_SOUTH:
			actor->z.fixed  += toFIXED(8);
			sprite->offsetY += 13;
			break;
	}
	actor->collisionFlags  = 0;
	actor->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
	sprite->flags          = 0;
	sprite->rotation       = angle + DEGtoANG(90);
}

// Item 0's icon, a gold coin, which an actor shows for coins.
#define COIN_ICON_ITEM 0

// Shows an actor as an item's icon, its sprite's only picture: the item, or for coins
// (SCOOPED_COINS) COIN_ICON_ITEM's.
void fdActorShowItemIconById(Sint32 id, Sint32 item)
{
	ACTOR         *actor = fdActorGetById(id);
	SPR           *sprite;
	ICON_TRANSFER *icon;
	Sint32         tile;
	Uint8          kind;

	if (actor != NULL)
	{
		kind = actor->spriteStorageKind;
		if (kind == ACTOR_SPRITE_ONE)
		{
			sprite = actor->sprite;
			if (item & SCOOPED_COINS)
				item = COIN_ICON_ITEM;
			icon = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
			DMA_FILL32(0, icon->tiles, ICON_TILE_BYTES);
#if FIX_FUNCTION_SIGNATURES
			mnIconDrawItemStub(item);
#else
			// UB: mnIconDrawItem takes the item alone; the call passes the tiles too.
			((void (*)(Sint32, Uint8 *))mnIconDrawItemStub)(item, icon->tiles);
#endif
			tile = syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, icon->tiles);
			syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			actor->retainSpriteCache = kind;
			fdSpriteAnimationClearStub(sprite->entries[0]);
			sprite->entries[0]      = NULL;
			sprite->count           = 0;
			sprite->image.fullColor = 0;
			sprite->image.tile      = tile;
			sprite->dirty           = 0;
			sprite->flags           = 0;
		}
	}
}

// The script command "set message": the number of the script's next message, written to the field
// state's message.
void fdScriptSetMessageId(Sint32 msg)
{
	FieldStateWork->msg = msg;
}

// Returns the sprite (the first animation's id) of the actor whose id is key's under
// SPEAKER_ACTOR_MASK, or -1 if there is no such animated actor.
Sint32 fdActorGetSpriteById(Sint32 key)
{
	FIELD_STATE *state  = RamHeapSlots->fieldState;
	Sint32       result = -1;
	ACTOR       *actor  = state->actors[key & SPEAKER_ACTOR_MASK];

	if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
		result = ((SPR *)actor->sprite)->entries[0]->animationId;
	return result;
}

// Returns the first of the scene's placed actors (from ACTOR_FIRST_PLACED) showing spriteId, or -1.
Sint32 fdActorFindBySprite(Sint32 spriteId)
{
	FIELD_STATE *state  = RamHeapSlots->fieldState;
	Sint32       result = -1;
	Sint32       index;
	ACTOR       *actor;

	for (index = ACTOR_FIRST_PLACED; index < FIELD_ACTOR_COUNT; index++)
	{
		actor = state->actors[index];
		if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE
		    && ((SPR *)actor->sprite)->entries[0]->animationId == spriteId)
		{
			result = index;
			break;
		}
	}
	return result;
}
