/* Vale and the World Map: the golden ring that rises around someone using
   Psynergy. */

#include "field/field_psynergy_state.h"

/* Sets off a golden ring around the actor: its two halves
   (field_effect_vertical.c), drawn from the Psynergy's cached tiles. */
void scSpawnGoldenRing(ACTOR *parent)
{
	ACTOR			   *pair[2];
	ACTOR			   *child;
	SPR				   *sprite;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	Sint32				i;

	for(i = 0; i < 2; ++i) {
		child = syFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
			parent->y.fixed, parent->z.fixed);
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			sprite = child->sprite;
			child->motionFlags = 0;
			child->timer = 0;
			child->watchedActor = (ACTOR_OR_EFFECT *)parent;
			if(sprite != NULL) {
				fdSpriteStartAnimation(sprite, 0);
				sprite->flags = 0;
				syVRAMCacheFreeEntry(sprite->slot);
				sprite->slot = effectState->tileSlot;
				sprite->retainCacheEntry = 1;
				sprite->image.tile =
					(VramCacheEntries[sprite->slot].offset / TILE_SIZE_4BPP) &
					OBJ_TILE_MASK;
				sprite->image.fullColor = 0;
				sprite->image.shape = RING_HALF_SHAPE;
				sprite->image.size = RING_HALF_SIZE;
				sprite->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR *effect = pair[0];
		SPR	  *effectSprite = effect->sprite;

		effect->update = scFieldEffectRiseRingFrontHalf;
		effectSprite->image.priority = 1;
	}
	{
		ACTOR *effect = pair[1];
		SPR	  *effectSprite = effect->sprite;

		effectSprite->image.priority = 1;
		effect->update = scFieldEffectRiseRingBackHalf;
		effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	}
}
