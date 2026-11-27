/* @file */
/* Vale and the World Map: the golden ring that rises around someone using */
/* Psynergy. */

#include "field/field_psynergy_state.h"

/* Sets off a golden ring around the actor: its two halves */
/* (field_effect_vertical.c), drawn from the Psynergy's cached tiles. */
void scSpawnGoldenRing(ACTOR *parent) {
	ACTOR *pair[2];
	ACTOR *child;
	SPR *sprite;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	s32 i;

	for(i = 0; i < 2; ++i) {
		child = syFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
								   parent->y.fixed, parent->z.fixed);
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			sprite = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (ACTOR_OR_EFFECT *)parent;
			if(sprite != NULL) {
				fdSprStartAnimation(sprite, 0);
				sprite->flg = 0;
				syVramCacheFreeEntry(sprite->slot);
				sprite->slot = effectState->tileSlot;
				sprite->retainCacheEntry = 1;
				sprite->image.tile =
					(VramCacheEntries[sprite->slot].ofs / TILE_SIZE_4BPP) &
					OBJ_TILE_MASK;
				sprite->image.fullCol = 0;
				sprite->image.shape = RING_HALF_SHAPE;
				sprite->image.size = RING_HALF_SIZE;
				sprite->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR *effect = pair[0];
		SPR *effectSprite = effect->spr;

		effect->update = scFieldEffectRiseRingFrontHalf;
		effectSprite->image.pri = 1;
	}
	{
		ACTOR *effect = pair[1];
		SPR *effectSprite = effect->spr;

		effectSprite->image.pri = 1;
		effect->update = scFieldEffectRiseRingBackHalf;
		effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}
