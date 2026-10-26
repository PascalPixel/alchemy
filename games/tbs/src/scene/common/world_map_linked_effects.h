#include "resource.h"
#include "types.h"
#include "animspr.h"
#include "field_effect.h"
#include "vram_cache.h"
#include "fx_work.h"

// Vale and the world map: the particles that rise around a glowing
// actor.

/// Attaches two rising particles to the actor, sharing the effect's
/// cached tiles.
void WorldMap_CreateLinkedEffects(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = Actor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed, parent->y.fixed,
                         parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct Actor *effect = pair[0];
    struct Sprite *effectSprite = effect->sprite;

    effect->update = FieldEffect_AnimateVerticalNegative;
    effectSprite->image.priority = 1;
  }
  {
    struct Actor *effect = pair[1];
    struct Sprite *effectSprite = effect->sprite;

    effectSprite->image.priority = 1;
    effect->update = FieldEffect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}
