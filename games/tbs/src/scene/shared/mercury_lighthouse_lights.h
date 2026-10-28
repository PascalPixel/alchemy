#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "dma.h"
#include "ram_buffer.h"
#include "event.h"
#include "scene_tables.h"
#include "actor.h"
#include "sprite.h"
#include "map_grid.h"
#include "io_reg.h"
#include "flags.h"

// Mercury Lighthouse and its entrance: the lights that carry the leader over
// the pool (mercury_lighthouse_pillar.h).

/// Keeps the lights' state st in HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS for the
/// floor's region, the pool's cell behavior. On an ordinary arrival the
/// leader has no lights. When the party returns to where it was, the column of
/// lights over the leader and the light under them in the pool are made again.
void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st) {
  struct Actor *actor;
  struct Actor *obj;
  struct Sprite *animation;
  struct MapGridCell *cell;
  s32 z;
  s32 flag;

  *(struct MercuryLighthouseLights **)HeapSlot_AllocEwram(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(st)) = st;
  flag = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Dma_Fill32(flag, st, sizeof(*st));
    st->poolBehavior = region;
    return;
  }
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  z = actor->z.fixed;
  cell = (struct MapGridCell *)Ram_MapGridCellBuffer + ((z / PIXELS(16)) << 7) +
         actor->x.fixed / PIXELS(16);
  if (st->levels != 0 && st->pillar != 0) {
    obj = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed,
                            actor->y.fixed + PIXELS(24), z);
    if (obj != 0) {
      obj->groundHeight = actor->groundHeight;
      animation = obj->sprite;
      Actor_StartScript(obj, gMercuryLighthouse_PillarScript);
      obj->watchedActor = actor;
      obj->motionFlags = 4;
      obj->y.fixed += -PIXELS(1) / 2;
      if (animation != 0) {
        Sprite_StartAnimation(animation, 6 - st->levels);
        animation->flags = 0;
        animation->image.priority = 1;
      }
      st->pillar = obj;
    }
  } else {
    st->pillar = 0;
  }
  if (cell->behavior == region && st->stepEffect != 0) {
    obj = FieldActor_Create(SPRITE_GOLDEN_RINGS, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
    if (obj == 0)
      return;
    obj->groundHeight = actor->groundHeight;
    animation = obj->sprite;
    Actor_StartScript(obj, gMercuryLighthouse_RampScript);
    obj->motionFlags = 0;
    obj->timer = 0;
    obj->priorityFlags = 2;
    obj->speedLimit = PIXELS(4);
    if (animation != 0) {
      Sprite_StartAnimation(animation, 6);
      animation->flags = 0;
    }
    st->stepEffect = obj;
  } else {
    st->stepEffect = 0;
  }
}
