// The golden rings that swell around a caster and rise up its body: each ring is two arcs of
// SPRITE_GOLDEN_RINGS, its back half behind the caster and its front half, the same arc upside
// down, in front. Part of script_commands.c.

#include "script_commands_internal.h"

// A ring's life in ticks, over which its scale runs through half a sine wave: it swells from
// nothing and shrinks again.
#define CAST_RING_TICKS       32
#define CAST_RING_SCALE(tick) sySin((tick) * (TRIG_TURN / 2 / CAST_RING_TICKS))
// The ring is set off every this many ticks while the caster flashes.
#define CAST_RING_INTERVAL    16

// Per-tick update of a ring's back half (arc A): over CAST_RING_TICKS it swells and shrinks again
// while it rises a pixel a tick on the actor it follows, 9 pixels in front of it and up to 5 pixels
// further while small.
void fdEffectUpdateScaledArcActorA(ACTOR *arc)
{
	ACTOR *anchor;
	Sint32 frame;
	FIXED  scale;

	anchor = (ACTOR *)arc->watchedActor;
	frame  = ++arc->timer;
	if (frame >= CAST_RING_TICKS)
	{
		fdActorStartScriptStub(arc, FieldEffectSharedParticleScript);
		return;
	}
	scale         = CAST_RING_SCALE(frame);
	arc->scaleX   = scale;
	arc->scaleY   = scale;
	arc->x.fixed  = anchor->x.fixed;
	arc->y.fixed += toFIXED(1);
	arc->z.fixed  = anchor->z.fixed + (toFIXED(1) - scale) * 5 + toFIXED(9);
}

// Per-tick update of a ring's front half (arc B): the back half's arc upside down, 16 pixels in
// front of the actor and up to 5 pixels nearer while small.
void fdEffectUpdateScaledArcActorB(ACTOR *arc)
{
	ACTOR *anchor;
	Sint32 frame;
	FIXED  scale;

	anchor = (ACTOR *)arc->watchedActor;
	frame  = ++arc->timer;
	if (frame >= CAST_RING_TICKS)
	{
		fdActorStartScriptStub(arc, FieldEffectSharedParticleScript);
		return;
	}
	scale         = CAST_RING_SCALE(frame);
	arc->scaleX   = scale;
	arc->scaleY   = -scale;
	arc->x.fixed  = anchor->x.fixed;
	arc->y.fixed += toFIXED(1);
	arc->z.fixed  = anchor->z.fixed - (toFIXED(1) - scale) * 5 + toFIXED(16);
}

// Spawns a ring on an actor, its two halves (arcs A and B of SPRITE_GOLDEN_RINGS, B upside down);
// they follow it for CAST_RING_TICKS, then release themselves.
void fdEffectSpawnScaledArcActors(ACTOR *watched)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR              *src         = effectState->caster;
	ACTOR              *arcs[2];
	ACTOR              *arc;
	SPR                *sprite;
	Sint32              arcIndex;

	for (arcIndex = 0; arcIndex <= 1; arcIndex++)
	{
		arc = fdActorCreateStub(
			SPRITE_GOLDEN_RINGS, watched->x.fixed, watched->y.fixed, watched->z.fixed);
		arcs[arcIndex] = arc;
		if (arc == 0)
			continue;
		arc->groundHeight = watched->groundHeight;
		sprite            = arc->sprite;
		arc->motionFlags  = 0;
		arc->timer        = 0;
		arc->watchedActor = (union ACTOR_OR_EFFECT *)watched;
		arc->scaleX = arc->scaleY = toFIXED(1) / 10;
		if (sprite == 0)
			continue;
		// A 16-colour sprite 32 by 16 (wide, size 2), drawn from the Psynergy's tiles.
		fdSpriteStartAnimationStub(sprite, 0);
		sprite->flags = 0;
		syVRAMCacheFreeEntry(sprite->slot);
		sprite->slot             = effectState->tileSlot;
		sprite->retainCacheEntry = 1;
		sprite->image.tile =
			(VramCacheEntries[sprite->slot].offset / TILE_SIZE_4BPP) & OBJ_TILE_MASK;
		sprite->image.fullColor     = 0;
		sprite->image.shape         = RING_HALF_SHAPE;
		sprite->image.size          = RING_HALF_SIZE;
		sprite->entries[0]->picture = 0;
	}
	// The front half in front of everything, the back half under the caster. BUG: allocation
	// failures above leave a missing arc or sprite here.
	arcs[0]->update                 = fdEffectUpdateScaledArcActorB;
	arcs[0]->sprite->image.priority = 0;
	arcs[1]->update                 = fdEffectUpdateScaledArcActorA;
	arcs[1]->sprite->image.priority = src->sprite->image.priority;
	arcs[1]->priorityFlags          = ACTOR_PRIORITY_UNDERFOOT;
}

// Per-tick update: flashes an actor white two ticks in four and sets off a ring around it every
// CAST_RING_INTERVAL ticks.
void fdEffectFlickerActorAndSpawnArcs(ACTOR *actor)
{
	if ((TickCount & 2) != 0)
	{
		fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_WHITE);
	}
	else
	{
		fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	if ((TickCount & (CAST_RING_INTERVAL - 1)) == 0)
	{
		fdEffectSpawnScaledArcActors(actor);
	}
}

// Per-tick update: cycles an actor through its own colours and the five with channels swapped, a
// new one every two ticks, and sets off a ring around it every CAST_RING_INTERVAL ticks.
void fdEffectCycleActorAndSpawnArcs(ACTOR *actor)
{
	if ((TickCount & 1) != 0)
	{
		Sint32 colorIndex = (TickCount >> 1) % (SPRITE_COLOR_BRG + 1);

		fdActorSetSpriteColorModeStub(actor, colorIndex);
	}
	if ((TickCount & (CAST_RING_INTERVAL - 1)) == 0)
	{
		fdEffectSpawnScaledArcActors(actor);
	}
}

#undef CAST_RING_TICKS
#undef CAST_RING_SCALE
#undef CAST_RING_INTERVAL
