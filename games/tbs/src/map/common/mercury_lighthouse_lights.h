#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"
#include "script_layer.h"
#include "map_tables.h"
#include "object_runtime.h"
#include "animspr.h"
#include "map_grid.h"
#include "io_reg.h"
#include "flags.h"

// Mercury Lighthouse and its entrance: the lights that carry the leader over
// the pool (mercury_lighthouse_pillar.h).

/// Keeps the lights' state st in HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS for the
/// floor's region, the pool's metatile behavior. On an ordinary arrival the
/// leader has no lights. When the party returns to where it was, the column of
/// lights over the leader and the light under them in the pool are made again.
void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *obj;
  struct AnimationObject *animation;
  struct MapGridCell *cell;
  s32 z;
  s32 flag;

  *(struct MercuryLighthouseLights **)AllocHeapSlotEwram(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(st)) = st;
  flag = Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Dma_Fill32(flag, st, sizeof(*st));
    st->terrainCode = region;
    return;
  }
  objectEvent = Engine_ObjectEventLookup(gGameState.leaderLocalId);
  z = objectEvent->z.fixed;
  cell = (struct MapGridCell *)Ram_MapGridCellBuffer + ((z / 0x100000) << 7) +
         objectEvent->x.fixed / 0x100000;
  if (st->levels != 0 && st->pillar != 0) {
    obj = Engine_ObjectCreate(26, objectEvent->x.fixed,
                              objectEvent->y.fixed + 0x180000, z);
    if (obj != 0) {
      obj->groundHeight = objectEvent->groundHeight;
      animation = obj->sprite;
      Engine_ObjectSetScript(obj, gMercuryLighthouse_PillarScript);
      obj->linkedObject = objectEvent;
      obj->motionFlags = 4;
      obj->y.fixed += -0x8000;
      if (animation != 0) {
        AnimationObject_SelectAnimation(animation, 6 - st->levels);
        animation->flags = 0;
        animation->image.priority = 1;
      }
      st->pillar = obj;
    }
  } else {
    st->pillar = 0;
  }
  if (cell->behavior == region && st->stepEffect != 0) {
    obj = Engine_ObjectCreate(26, objectEvent->x.fixed, objectEvent->y.fixed,
                              objectEvent->z.fixed);
    if (obj == 0)
      return;
    obj->groundHeight = objectEvent->groundHeight;
    animation = obj->sprite;
    Engine_ObjectSetScript(obj, gMercuryLighthouse_RampScript);
    obj->motionFlags = 0;
    obj->timer = 0;
    obj->priorityFlags = 2;
    obj->speed = 0x40000;
    if (animation != 0) {
      AnimationObject_SelectAnimation(animation, 6);
      animation->flags = 0;
    }
    st->stepEffect = obj;
  } else {
    st->stepEffect = 0;
  }
}
