#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);
void FieldEvent_WaitFrames(s32 frames);
void Import_SoundPlay(s32 cue);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void VenusLighthouseEntrance_RunActorSparkBurst(void *object) {
  Func_02002a34(8, 0x4084);
  Func_02002a44();
  Func_02002a5c(Actor_GetById(5), 2);
  FieldEvent_WaitFrames(20);
  Func_020023a4(object);
  Func_02002a5c(Actor_GetById(5), 0);
  Func_02002a54();
  Func_02002a4c();
  Field_StartPaletteTransition(8);
}

void VenusLighthouseEntrance_UpdateSparkTrail(struct Actor *object) {
  Func_0200260c(object, object->x, object->y, object->z);
  if (++object->action % 8 == 0)
    Import_SoundPlay(125);
}
