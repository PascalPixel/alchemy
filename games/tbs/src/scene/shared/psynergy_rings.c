/// @file
/// Vale in the storm and Venus Lighthouse's aerie: the rings of light around
/// someone using Psynergy in an event.

#include "field_effect_vertical.c"

/// Sets off a golden ring around the actor, its two halves
/// (field_effect_vertical.c) drawn from the Psynergy's cached tiles, with the
/// sound the includer names as PSYNERGY_RINGS_SOUND. Vale's and the World
/// Map's Scene_SpawnGoldenRing (golden_ring.c) is the same ring without the
/// sound, its halves always at sprite priority 1 rather than the actor's.
static void Scene_SpawnPsynergyRings(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectState *effectState = gFieldEffectState;
  s32 i;

  Sound_Play(PSYNERGY_RINGS_SOUND);
  for (i = 0; i < 2; ++i) {
    child = FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                              parent->y.fixed, parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedActor = (union ActorOrEffect *)parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
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
    }
  }
  pair[0]->update = FieldEffect_RiseRingFrontHalf;
  pair[0]->sprite->image.priority = parent->sprite->image.priority;
  pair[1]->sprite->image.priority = parent->sprite->image.priority;
  pair[1]->update = FieldEffect_RiseRingBackHalf;
  pair[1]->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

#undef PSYNERGY_RINGS_SOUND
