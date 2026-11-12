/// @file
/// The scene code of overlay 0x6a5 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_aerie_beacon/.

#include "types.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000038(s32 a0) {
  Event_PlaySound(0x98);
  Actor_SetSingleChildFlags(a0, 0);
  return 0;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000050(s32 a0) {
  Actor_SetSingleChildFlags(a0, 1);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "../field/object/park_record.c"
