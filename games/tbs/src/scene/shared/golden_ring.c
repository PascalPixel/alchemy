/* Vale and the World Map: the golden ring that rises around someone using
   Psynergy. */

#include "field/field_psynergy_state.h"

/* Sets off a golden ring around the actor: its two halves
   (field_effect_vertical.c), drawn from the Psynergy's cached tiles. */
void scSpawnGoldenRing(ACTOR *parent)
{
	ACTOR			   *pair[2];
	ACTOR			   *child;
	SPR				   *spr;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	Sint32				i;

	for(i = 0; i < 2; ++i) {
		child = syFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
			parent->y.fixed, parent->z.fixed);
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			spr = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (ACTOR_OR_EFFECT *)parent;
			if(spr != NULL) {
				fdSprStartAnimation(spr, 0);
				spr->flg = 0;
				syVRAMCacheFreeEntry(spr->slot);
				spr->slot = effectState->tileSlot;
				spr->retainCacheEntry = 1;
				spr->image.tile =
					(VramCacheEntries[spr->slot].ofs / TILE_SIZE_4BPP) &
					OBJ_TILE_MASK;
				spr->image.fullCol = 0;
				spr->image.shape = RING_HALF_SHAPE;
				spr->image.size = RING_HALF_SIZE;
				spr->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR *effect = pair[0];
		SPR	  *effectSpr = effect->spr;

		effect->update = scFieldEffectRiseRingFrontHalf;
		effectSpr->image.pri = 1;
	}
	{
		ACTOR *effect = pair[1];
		SPR	  *effectSpr = effect->spr;

		effectSpr->image.pri = 1;
		effect->update = scFieldEffectRiseRingBackHalf;
		effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}
