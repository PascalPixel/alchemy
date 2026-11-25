/// @file
/// Vale and the World Map: the golden ring that rises around someone using
/// Psynergy.

#include "field/field_psynergy_state.h"

/// Sets off a golden ring around the actor: its two halves
/// (field_effect_vertical.c), drawn from the Psynergy's cached tiles.
void Scene_SpawnGoldenRing(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectState *effectState = gFieldEffectState;
  s32 i;

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
  {
    struct Actor *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_RiseRingFrontHalf;
    effectSprite->image.priority = 1;
  }
  {
    struct Actor *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = 1;
    effect->update = FieldEffect_RiseRingBackHalf;
    effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}
