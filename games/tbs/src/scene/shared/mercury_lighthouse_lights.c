/// @file
/// Mercury Lighthouse and its entrance: the lights that carry the leader over
/// the pool (mercury_lighthouse_pillar.c).

#include "system/dma.h"
#include "system/ram_buffer.h"

/// SPRITE_GOLDEN_RINGS's animations for the pool's lights (golden_rings.tsv).
enum GoldenRingsAnimation {
  /// The back half of a ring, an arc bowed upward: the charging spark behind
  /// the leader.
  GOLDEN_RINGS_ANIMATION_BACK_ARC = 1,
  /// The front half, bowed downward: the charging spark in front.
  GOLDEN_RINGS_ANIMATION_FRONT_ARC = 2,
  /// The column of light while it holds three lights: three small rings
  /// turning. 4 shows two rings and 5 one (GOLDEN_RINGS_ANIMATION_COLUMN).
  GOLDEN_RINGS_ANIMATION_THREE_LIGHTS = 3,
  GOLDEN_RINGS_ANIMATION_ONE_LIGHT = 5,
  /// A whole ring, shimmering: the light that holds the leader up in the
  /// pool.
  GOLDEN_RINGS_ANIMATION_POOL_LIGHT = 6,
  /// A whole ring breaking into dots: a light going out.
  GOLDEN_RINGS_ANIMATION_LIGHT_OUT = 7
};

/// The column of light's animation while it holds this many lights, 1 to 3.
#define GOLDEN_RINGS_ANIMATION_COLUMN(lights)                                  \
  (GOLDEN_RINGS_ANIMATION_ONE_LIGHT + 1 - (lights))

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
        Sprite_StartAnimation(sprite,
                              GOLDEN_RINGS_ANIMATION_COLUMN(lights->levels));
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
      Sprite_StartAnimation(sprite, GOLDEN_RINGS_ANIMATION_POOL_LIGHT);
      sprite->flags = 0;
    }
    lights->stepEffect = light;
  } else {
    lights->stepEffect = 0;
  }
}
