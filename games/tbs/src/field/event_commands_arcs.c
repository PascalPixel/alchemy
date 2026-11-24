/// @file
/// The golden rings that swell around a caster and rise up its body: each ring
/// is two arcs of SPRITE_GOLDEN_RINGS, its back half behind the caster and its
/// front half, the same arc upside down, in front.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// A ring's life in frames, over which its scale runs through half a sine
/// wave: it swells from nothing and shrinks again.
#define CAST_RING_FRAMES 32
#define CAST_RING_SCALE(frame)                                                 \
  Trig_Sin((frame) * (TRIG_TURN / 2 / CAST_RING_FRAMES))
/// The ring is set off every this many frames while the caster flashes.
#define CAST_RING_INTERVAL 16

/// Per-frame update of a ring's back half (arc A): over CAST_RING_FRAMES it
/// swells and shrinks again while it rises a pixel a frame on the actor it
/// follows, 9 pixels in front of it and up to 5 pixels further while small.
void FieldEffect_UpdateScaledArcActorA(struct Actor *arc) {
  struct Actor *anchor;
  s32 frame;
  FIXED scale;

  anchor = arc->watchedActor;
  frame = ++arc->timer;
  if (frame >= CAST_RING_FRAMES) {
    Actor_StartScriptFar(arc, gFieldEffect_SharedParticleScript);
    return;
  }
  scale = CAST_RING_SCALE(frame);
  arc->scaleX = scale;
  arc->scaleY = scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += FIXED_ONE;
  arc->z.fixed = anchor->z.fixed + (FIXED_ONE - scale) * 5 + toFIXED(9);
}

/// Per-frame update of a ring's front half (arc B): the back half's arc upside
/// down, 16 pixels in front of the actor and up to 5 pixels nearer while
/// small.
void FieldEffect_UpdateScaledArcActorB(struct Actor *arc) {
  struct Actor *anchor;
  s32 frame;
  FIXED scale;

  anchor = arc->watchedActor;
  frame = ++arc->timer;
  if (frame >= CAST_RING_FRAMES) {
    Actor_StartScriptFar(arc, gFieldEffect_SharedParticleScript);
    return;
  }
  scale = CAST_RING_SCALE(frame);
  arc->scaleX = scale;
  arc->scaleY = -scale;
  arc->x.fixed = anchor->x.fixed;
  arc->y.fixed += FIXED_ONE;
  arc->z.fixed = anchor->z.fixed - (FIXED_ONE - scale) * 5 + toFIXED(16);
}

/// Spawns a ring on an actor, its two halves (arcs A and B of
/// SPRITE_GOLDEN_RINGS, B upside down); they follow it for CAST_RING_FRAMES,
/// then release themselves.
void FieldEffect_SpawnScaledArcActors(struct Actor *watched) {
  struct FieldEffectState *effectState = gFieldEffectState;
  struct Actor *source = effectState->caster;
  struct Actor *arcs[2];
  struct Actor *arc;
  struct Sprite *sprite;
  s32 arcIndex;

  for (arcIndex = 0; arcIndex <= 1; arcIndex++) {
    arc = Actor_CreateFar(SPRITE_GOLDEN_RINGS, watched->x.fixed,
                          watched->y.fixed, watched->z.fixed);
    arcs[arcIndex] = arc;
    if (arc == 0)
      continue;
    arc->groundHeight = watched->groundHeight;
    sprite = arc->sprite;
    arc->motionFlags = 0;
    arc->timer = 0;
    arc->watchedActor = watched;
    arc->scaleX = arc->scaleY = FIXED_ONE / 10;
    if (sprite == 0)
      continue;
    // A 16-colour sprite 32 by 16 (wide, size 2), drawn from the Psynergy's
    // tiles.
    Sprite_StartAnimationFar(sprite, 0);
    sprite->flags = 0;
    VramCache_FreeEntry(sprite->slot);
    sprite->slot = effectState->tileSlot;
    sprite->retainCacheEntry = 1;
    sprite->image.tile =
        (gVramCacheEntries[sprite->slot].offset / TILE_SIZE_4BPP) &
        OBJ_TILE_MASK;
    sprite->image.fullColor = 0;
    sprite->image.shape = RING_HALF_SHAPE;
    sprite->image.size = RING_HALF_SIZE;
    sprite->entries[0]->picture = 0;
  }
  // The front half in front of everything, the back half under the caster.
  // BUG: allocation failures above leave a missing arc or sprite here.
  arcs[0]->update = FieldEffect_UpdateScaledArcActorB;
  arcs[0]->sprite->image.priority = 0;
  arcs[1]->update = FieldEffect_UpdateScaledArcActorA;
  arcs[1]->sprite->image.priority = source->sprite->image.priority;
  arcs[1]->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

/// Per-frame update: flashes an actor white two frames in four and sets off a
/// ring around it every CAST_RING_INTERVAL frames.
void FieldEffect_FlickerActorAndSpawnArcs(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & (CAST_RING_INTERVAL - 1)) == 0) {
    FieldEffect_SpawnScaledArcActors(actor);
  }
}

/// Per-frame update: cycles an actor through its own colours and the five
/// with channels swapped, a new one every two frames, and sets off a ring
/// around it every CAST_RING_INTERVAL frames.
void FieldEffect_CycleActorAndSpawnArcs(struct Actor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 colorIndex = (gFrameCount >> 1) % (SPRITE_COLOR_BRG + 1);

    Actor_SetSpriteColorModeFar(actor, colorIndex);
  }
  if ((gFrameCount & (CAST_RING_INTERVAL - 1)) == 0) {
    FieldEffect_SpawnScaledArcActors(actor);
  }
}

#undef CAST_RING_FRAMES
#undef CAST_RING_SCALE
#undef CAST_RING_INTERVAL
