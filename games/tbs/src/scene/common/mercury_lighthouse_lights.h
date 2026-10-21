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
#include "field_event.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "animspr.h"
#include "map.h"
#include "io_reg.h"
#include "flags.h"

// Mercury Lighthouse and its entrance: the lights that carry the leader over
// the pool (mercury_lighthouse_pillar.h).

/// Keeps the lights' state st in HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS for the
/// floor's region, the pool's collision code. On an ordinary entrance the
/// leader has no lights. When the party returns to where it was, the column of
/// lights over the leader and the light under them in the pool are made again.
void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st) {
  struct FieldActor *actor;
  struct FieldActor *obj;
  struct AnimationObject *anim;
  struct MapCell *cell;
  s32 z;
  s32 flag;

  *(struct MercuryLighthouseLights **)Runtime_AllocateBlock(
      HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(st)) = st;
  flag = Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Dma_Fill32(flag, st, sizeof(*st));
    st->terrainCode = region;
    return;
  }
  actor = Engine_ActorLookup(gGameState.selectedActor);
  z = actor->z.fixed;
  cell = (struct MapCell *)Ram_MapCellBuffer + ((z / 0x100000) << 7) +
         actor->x.fixed / 0x100000;
  if (st->levels != 0 && st->pillar != 0) {
    obj = Engine_ObjectCreate(26, actor->x.fixed, actor->y.fixed + 0x180000, z);
    if (obj != 0) {
      obj->terrainHeight = actor->terrainHeight;
      anim = obj->sprite;
      Engine_ObjectSetScript(obj, gMercuryLighthouse_PillarScript);
      obj->linkedObject = actor;
      obj->motionFlags = 4;
      obj->y.fixed += -0x8000;
      if (anim != 0) {
        AnimationObjects_SelectAnimation(anim, 6 - st->levels);
        anim->flags = 0;
        anim->image.priority = 1;
      }
      st->pillar = obj;
    }
  } else {
    st->pillar = 0;
  }
  if (cell->collisionCode == region && st->stepEffect != 0) {
    obj =
        Engine_ObjectCreate(26, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    if (obj == 0)
      return;
    obj->terrainHeight = actor->terrainHeight;
    anim = obj->sprite;
    Engine_ObjectSetScript(obj, gMercuryLighthouse_RampScript);
    obj->motionFlags = 0;
    obj->timer = 0;
    obj->priorityFlags = 2;
    obj->speed = 0x40000;
    if (anim != 0) {
      AnimationObjects_SelectAnimation(anim, 6);
      anim->flags = 0;
    }
    st->stepEffect = obj;
  } else {
    st->stepEffect = 0;
  }
}
