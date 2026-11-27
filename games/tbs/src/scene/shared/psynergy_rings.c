/* Vale in the storm and Venus Lighthouse's aerie: the rings of light around
   someone using Psynergy in a script. */

#include "field_effect_vertical.c"

/* Sets off a golden ring around the actor, its two halves
   (field_effect_vertical.c) drawn from the Psynergy's cached tiles, with the
   sound the includer names as PSYNERGY_RINGS_SOUND. Vale's and the World Map's
   scSpawnGoldenRing (golden_ring.c) is the same ring without the sound, its
   halves always at sprite priority 1 rather than the actor's. */
static void scSpawnPsynergyRings(ACTOR *parent)
{
	ACTOR			   *pair[2];
	ACTOR			   *child;
	SPR				   *spr;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	Sint32				i;

	sySndPlay(PSYNERGY_RINGS_SOUND);
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
	pair[0]->update = scFieldEffectRiseRingFrontHalf;
	pair[0]->spr->image.pri = parent->spr->image.pri;
	pair[1]->spr->image.pri = parent->spr->image.pri;
	pair[1]->update = scFieldEffectRiseRingBackHalf;
	pair[1]->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

#undef PSYNERGY_RINGS_SOUND
