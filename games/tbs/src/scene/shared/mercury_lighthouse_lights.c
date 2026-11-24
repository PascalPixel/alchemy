/// @file
/// Mercury Lighthouse and its entrance: the lights that carry the leader over
/// the pool (mercury_lighthouse_pillar.c).

#include "system/dma.h"
#include "system/ram_buffer.h"

/// Sets up the lights for a floor whose pool is the cell behavior region,
/// keeping their state for the pool's events. On an ordinary arrival the
/// leader has no lights. When the party returns to where it was, as after a
/// battle, the column of light over the leader and the light under them in
/// the pool are made again.
void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *lights) {
  struct Actor *actor;
  struct Actor *light;
  struct Sprite *sprite;
  struct MapGridCell *cell;
  FIXED z;
  s32 flag;

  *(struct MercuryLighthouseLights **)HeapSlot_AllocEwram(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(lights)) = lights;
  flag = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Dma_Fill32(flag, lights, sizeof(*lights));
    lights->poolBehavior = region;
    return;
  }
  actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  z = actor->z.fixed;
  cell = (struct MapGridCell *)Ram_MapGridCellBuffer +
         (z / toFIXED(16)) * MAPGRID_SIZE + actor->x.fixed / toFIXED(16);
  if (lights->levels != 0 && lights->pillar != 0) {
    light = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed,
                              actor->y.fixed + toFIXED(24), z);
    if (light != NULL) {
      light->groundHeight = actor->groundHeight;
      sprite = light->sprite;
      Actor_StartScript(light, gMercuryLighthouse_PillarScript);
      light->watchedActor = actor;
      light->motionFlags = ACTOR_FLOATS;
      light->y.fixed -= toFIXED(1) / 2;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 6 - lights->levels);
        sprite->flags = 0;
        sprite->image.priority = 1;
      }
      lights->pillar = light;
    }
  } else {
    lights->pillar = 0;
  }
  if (cell->behavior == region && lights->stepEffect != 0) {
    light = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed,
                              actor->y.fixed, actor->z.fixed);
    if (light == NULL)
      return;
    light->groundHeight = actor->groundHeight;
    sprite = light->sprite;
    Actor_StartScript(light, gMercuryLighthouse_GrowLightScript);
    light->motionFlags = 0;
    light->timer = 0;
    light->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    light->speedLimit = toFIXED(4);
    if (sprite != NULL) {
      Sprite_StartAnimation(sprite, 6);
      sprite->flags = 0;
    }
    lights->stepEffect = light;
  } else {
    lights->stepEffect = 0;
  }
}
