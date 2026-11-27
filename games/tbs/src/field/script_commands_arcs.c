/* The golden rings that swell around a caster and rise up its body: each ring
   is two arcs of SPRITE_GOLDEN_RINGS, its back half behind the caster and its
   front half, the same arc upside down, in front. Part of script_commands.c. */

#include "script_commands_internal.h"

/* A ring's life in ticks, over which its scale runs through half a sine wave:
   it swells from nothing and shrinks again. */
#define CAST_RING_TICKS		  32
#define CAST_RING_SCALE(tick) sySin((tick) * (TRIG_TURN / 2 / CAST_RING_TICKS))
/* The ring is set off every this many ticks while the caster flashes. */
#define CAST_RING_INTERVAL	  16

/* Per-tick update of a ring's back half (arc A): over CAST_RING_TICKS it
   swells and shrinks again while it rises a pixel a tick on the actor it
   follows, 9 pixels in front of it and up to 5 pixels further while small. */
void fdEffectUpdateScaledArcActorA(ACTOR *arc)
{
	ACTOR *anchor;
	Sint32 frm;
	FIXED  scale;

	anchor = (ACTOR *)arc->watchedActor;
	frm = ++arc->tmr;
	if(frm >= CAST_RING_TICKS) {
		fdActorStartScriptStub(arc, FieldEffectSharedParticleScript);
		return;
	}
	scale = CAST_RING_SCALE(frm);
	arc->scaleX = scale;
	arc->scaleY = scale;
	arc->x.fixed = anchor->x.fixed;
	arc->y.fixed += toFIXED(1);
	arc->z.fixed = anchor->z.fixed + (toFIXED(1) - scale) * 5 + toFIXED(9);
}

/* Per-tick update of a ring's front half (arc B): the back half's arc upside
   down, 16 pixels in front of the actor and up to 5 pixels nearer while
   small. */
void fdEffectUpdateScaledArcActorB(ACTOR *arc)
{
	ACTOR *anchor;
	Sint32 frm;
	FIXED  scale;

	anchor = (ACTOR *)arc->watchedActor;
	frm = ++arc->tmr;
	if(frm >= CAST_RING_TICKS) {
		fdActorStartScriptStub(arc, FieldEffectSharedParticleScript);
		return;
	}
	scale = CAST_RING_SCALE(frm);
	arc->scaleX = scale;
	arc->scaleY = -scale;
	arc->x.fixed = anchor->x.fixed;
	arc->y.fixed += toFIXED(1);
	arc->z.fixed = anchor->z.fixed - (toFIXED(1) - scale) * 5 + toFIXED(16);
}

/* Spawns a ring on an actor, its two halves (arcs A and B of
   SPRITE_GOLDEN_RINGS, B upside down); they follow it for CAST_RING_TICKS,
   then release themselves. */
void fdEffectSpawnScaledArcActors(ACTOR *watched)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *src = effectState->caster;
	ACTOR			   *arcs[2];
	ACTOR			   *arc;
	SPR				   *spr;
	Sint32				arcIdx;

	for(arcIdx = 0; arcIdx <= 1; arcIdx++) {
		arc = fdActorCreateStub(SPRITE_GOLDEN_RINGS, watched->x.fixed,
			watched->y.fixed, watched->z.fixed);
		arcs[arcIdx] = arc;
		if(arc == 0)
			continue;
		arc->groundHeight = watched->groundHeight;
		spr = arc->spr;
		arc->motionFlg = 0;
		arc->tmr = 0;
		arc->watchedActor = (union ACTOR_OR_EFFECT *)watched;
		arc->scaleX = arc->scaleY = toFIXED(1) / 10;
		if(spr == 0)
			continue;
		/* A 16-colour sprite 32 by 16 (wide, size 2), drawn from the Psynergy's
		   tiles. */
		fdSprStartAnimationStub(spr, 0);
		spr->flg = 0;
		syVRAMCacheFreeEntry(spr->slot);
		spr->slot = effectState->tileSlot;
		spr->retainCacheEntry = 1;
		spr->image.tile =
			(VramCacheEntries[spr->slot].ofs / TILE_SIZE_4BPP) & OBJ_TILE_MASK;
		spr->image.fullCol = 0;
		spr->image.shape = RING_HALF_SHAPE;
		spr->image.size = RING_HALF_SIZE;
		spr->entries[0]->picture = 0;
	}
	/* The front half in front of everything, the back half under the caster. */
	/* BUG: allocation failures above leave a missing arc or sprite here. */
	arcs[0]->update = fdEffectUpdateScaledArcActorB;
	arcs[0]->spr->image.pri = 0;
	arcs[1]->update = fdEffectUpdateScaledArcActorA;
	arcs[1]->spr->image.pri = src->spr->image.pri;
	arcs[1]->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

/* Per-tick update: flashes an actor white two ticks in four and sets off a
   ring around it every CAST_RING_INTERVAL ticks. */
void fdEffectFlickerActorAndSpawnArcs(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & (CAST_RING_INTERVAL - 1)) == 0) {
		fdEffectSpawnScaledArcActors(actor);
	}
}

/* Per-tick update: cycles an actor through its own colours and the five with
   channels swapped, a new one every two ticks, and sets off a ring around it
   every CAST_RING_INTERVAL ticks. */
void fdEffectCycleActorAndSpawnArcs(ACTOR *actor)
{
	if((TickCount & 1) != 0) {
		Sint32 colIdx = (TickCount >> 1) % (SPRITE_COLOR_BRG + 1);

		fdActorSetSprColModeStub(actor, colIdx);
	}
	if((TickCount & (CAST_RING_INTERVAL - 1)) == 0) {
		fdEffectSpawnScaledArcActors(actor);
	}
}

#undef CAST_RING_TICKS
#undef CAST_RING_SCALE
#undef CAST_RING_INTERVAL
