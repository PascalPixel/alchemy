#include "resource.h"
#include "types.h"
#include "animspr.h"
#include "field_effect.h"
#include "vram_block.h"
#include "b5_context.h"
#include "fx_scene.h"

// Vale and the world map: the particles that rise around a glowing actor.

// Attach two rising particles to the actor, sharing the effect's cached tiles.
void WorldMap_CreateLinkedEffects(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct BattleFxScene *work = gEffectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        AnimationObjects_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  {
    struct FieldActor *effect = pair[0];
    struct AnimationObject *effectSprite = effect->sprite;

    effect->update = Effect_AnimateVerticalNegative;
    effectSprite->image.priority = 1;
  }
  {
    struct FieldActor *effect = pair[1];
    struct AnimationObject *effectSprite = effect->sprite;

    effectSprite->image.priority = 1;
    effect->update = Effect_AnimateVerticalPositive;
    effect->priorityFlags = 2;
  }
}
