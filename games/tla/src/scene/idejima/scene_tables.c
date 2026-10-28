#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "scene_tables.h"

enum { ENTRANCE_WAKE = 2, ENTRANCE_WAKE_AGAIN = 4 };

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gIdejimaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gIdejimaExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gIdejimaPlacements;
}

const struct Trigger *Scene_GetEvents(void) {
  switch (gMainRoutineWork.entrance) {
  case 1:
  case 3:
    return gIdejimaEventsEntrance1;
  case ENTRANCE_WAKE:
  case ENTRANCE_WAKE_AGAIN:
    return gIdejimaEventsWake;
  default:
    return gIdejimaEvents;
  }
}
