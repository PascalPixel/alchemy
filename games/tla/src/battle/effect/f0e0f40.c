#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

void Actor_Destroy(struct Actor *object);

void BattleEffect_UpdateDescendingParticlePositiveArc(
    struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct SequenceParentWork *party = &gSequenceParentWork;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->watchedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Actor_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}
