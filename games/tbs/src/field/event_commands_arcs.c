/* @file */
/* The golden rings that swell around a caster and rise up its body: each ring
 */
/* is two arcs of SPRITE_GOLDEN_RINGS, its back half behind the caster and its
 */
/* front half, the same arc upside down, in front. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* A ring's life in frames, over which its scale runs through half a sine */
/* wave: it swells from nothing and shrinks again. */
#define CAST_RING_FRAMES 32
#define CAST_RING_SCALE(frame)                                                 \
	sySin((frame) * (TRIG_TURN / 2 / CAST_RING_FRAMES))
/* The ring is set off every this many frames while the caster flashes. */
#define CAST_RING_INTERVAL 16

/* Per-frame update of a ring's back half (arc A): over CAST_RING_FRAMES it */
/* swells and shrinks again while it rises a pixel a frame on the actor it */
/* follows, 9 pixels in front of it and up to 5 pixels further while small. */
void fdEffectUpdateScaledArcActorA(ACTOR *arc) {
	ACTOR *anchor;
	s32 frame;
	FIXED scale;

	anchor = (ACTOR *)arc->watchedActor;
	frame = ++arc->tmr;
	if(frame >= CAST_RING_FRAMES) {
		fdActorStartScriptStub(arc, FieldEffect_SharedParticleScript);
		return;
	}
	scale = CAST_RING_SCALE(frame);
	arc->scaleX = scale;
	arc->scaleY = scale;
	arc->x.fixed = anchor->x.fixed;
	arc->y.fixed += FIXED_ONE;
	arc->z.fixed = anchor->z.fixed + (FIXED_ONE - scale) * 5 + toFIXED(9);
}

/* Per-frame update of a ring's front half (arc B): the back half's arc upside
 */
/* down, 16 pixels in front of the actor and up to 5 pixels nearer while */
/* small. */
void fdEffectUpdateScaledArcActorB(ACTOR *arc) {
	ACTOR *anchor;
	s32 frame;
	FIXED scale;

	anchor = (ACTOR *)arc->watchedActor;
	frame = ++arc->tmr;
	if(frame >= CAST_RING_FRAMES) {
		fdActorStartScriptStub(arc, FieldEffect_SharedParticleScript);
		return;
	}
	scale = CAST_RING_SCALE(frame);
	arc->scaleX = scale;
	arc->scaleY = -scale;
	arc->x.fixed = anchor->x.fixed;
	arc->y.fixed += FIXED_ONE;
	arc->z.fixed = anchor->z.fixed - (FIXED_ONE - scale) * 5 + toFIXED(16);
}

/* Spawns a ring on an actor, its two halves (arcs A and B of */
/* SPRITE_GOLDEN_RINGS, B upside down); they follow it for CAST_RING_FRAMES, */
/* then release themselves. */
void fdEffectSpawnScaledArcActors(ACTOR *watched) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *source = effectState->caster;
	ACTOR *arcs[2];
	ACTOR *arc;
	SPR *sprite;
	s32 arcIndex;

	for(arcIndex = 0; arcIndex <= 1; arcIndex++) {
		arc = fdActorCreateStub(SPRITE_GOLDEN_RINGS, watched->x.fixed,
								watched->y.fixed, watched->z.fixed);
		arcs[arcIndex] = arc;
		if(arc == 0)
			continue;
		arc->groundHeight = watched->groundHeight;
		sprite = arc->spr;
		arc->motionFlg = 0;
		arc->tmr = 0;
		arc->watchedActor = (union ACTOR_OR_EFFECT *)watched;
		arc->scaleX = arc->scaleY = FIXED_ONE / 10;
		if(sprite == 0)
			continue;
		/* A 16-colour sprite 32 by 16 (wide, size 2), drawn from the Psynergy's
		 */
		/* tiles. */
		fdSprStartAnimationStub(sprite, 0);
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
	/* The front half in front of everything, the back half under the caster. */
	/* BUG: allocation failures above leave a missing arc or sprite here. */
	arcs[0]->update = fdEffectUpdateScaledArcActorB;
	arcs[0]->spr->image.pri = 0;
	arcs[1]->update = fdEffectUpdateScaledArcActorA;
	arcs[1]->spr->image.pri = source->spr->image.pri;
	arcs[1]->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

/* Per-frame update: flashes an actor white two frames in four and sets off a */
/* ring around it every CAST_RING_INTERVAL frames. */
void fdEffectFlickerActorAndSpawnArcs(ACTOR *actor) {
	if((FrameCount & 2) != 0) {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_WHITE);
	}
	else {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	if((FrameCount & (CAST_RING_INTERVAL - 1)) == 0) {
		fdEffectSpawnScaledArcActors(actor);
	}
}

/* Per-frame update: cycles an actor through its own colours and the five */
/* with channels swapped, a new one every two frames, and sets off a ring */
/* around it every CAST_RING_INTERVAL frames. */
void fdEffectCycleActorAndSpawnArcs(ACTOR *actor) {
	if((FrameCount & 1) != 0) {
		s32 colorIndex = (FrameCount >> 1) % (SPRITE_COLOR_BRG + 1);

		fdActorSetSprColModeStub(actor, colorIndex);
	}
	if((FrameCount & (CAST_RING_INTERVAL - 1)) == 0) {
		fdEffectSpawnScaledArcActors(actor);
	}
}

#undef CAST_RING_FRAMES
#undef CAST_RING_SCALE
#undef CAST_RING_INTERVAL
