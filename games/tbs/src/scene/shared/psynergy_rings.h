/// @file
/// Vale in the storm and Venus Lighthouse's aerie: the rings of light around
/// someone using Psynergy in an event.
#include "types.h"
#include "actor.h"
#include "sprite.h"
#include "field_effect.h"
#include "vram_cache.h"
#include "field_psynergy_work.h"
#include "battle_sprite_id.h"
#include "field_effect_vertical.h"

/// Spawns a pair of rings of light on the actor, one rising and one falling
/// (field_effect_vertical.h), sharing the field effect's cached tiles, with
/// the sound the includer names as PSYNERGY_RINGS_SOUND.
/// ★ Named from what the player sees: rings of light rise and fall around the
/// elders holding the Boulder and around Sheba at the aerie
/// (ValePrologue_WatchBoulderFall,
/// VenusLighthouseAerie_ConfrontSaturosAndMenardi)
static void Scene_SpawnPsynergyRings(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
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
      child->watchedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->picture = 0;
      }
    }
  }
  pair[0]->update = FieldEffect_AnimateVerticalNegative;
  pair[0]->sprite->image.priority = parent->sprite->image.priority;
  pair[1]->sprite->image.priority = parent->sprite->image.priority;
  pair[1]->update = FieldEffect_AnimateVerticalPositive;
  pair[1]->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
}

#undef PSYNERGY_RINGS_SOUND
