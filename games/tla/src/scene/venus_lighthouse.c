/// @file
/// The scene code of overlay 0x64e that its scene tables belong to, in ROM
/// order; the rest of the overlay is in venus_lighthouse/.

#include "shared/push.h"
#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

extern s32 gData_02001d8c[];

void Import_ActorSelectAnimation(struct StagedActor *object, s32 mode);
s32 MapGrid_CheckCollision(struct StagedActor *object,
                           struct FieldPosition *position);

DEFINE_BLOCK_PUSH_ONTO(Func_0200038c, gData_02001d8c, MapGrid_CheckCollision,
                       Import_ActorSelectAnimation)

#include "../field/map/cell_fill_byte.c"

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "scene_tables.h"
#include "frame.h"

void Func_0200056c(void) {}

#define FieldActor_SetSpriteColor ((void (*)(void *, s32))Actor_SetSpriteColor)

s32 OverlayObject_UpdateEveryFourFrames(void *obj) {
  if ((gFrameCount & 3) == 0)
    FieldActor_SetSpriteColor(obj, 7);
  else
    FieldActor_SetSpriteColor(obj, 0);

  if ((gFrameCount & 7) == 0)
    Event_PlaySound(138);
  return 0;
}

void Func_02000150(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

/// Emits a rising particle near the actor every eight frames.
s32 VenusLighthouse_EmitRisingParticles(struct SceneEffectSource *actor) {
  struct SceneEffectParams options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    goto done;
  options.priority = 3 - (s32)((u32)(Random16Far() * 2) >> 16);
  options.startScaleX = 0x6666;
  options.startScaleY = 0x6666;
  options.palette = 14;
  particleX = actor->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  particleY = actor->y + ((32 - (s32)((u32)(Random16Far() * 32) >> 16)) << 16);
  riseSpeed =
      ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x00050000) / 10;
  Func_02000150(particleX, particleY, actor->z, 0, riseSpeed, framePhase,
                0x000b0000, &options);
done:
  return 0;
}

#define Actor_SetSpriteFlags ((void (*)(void *, s32))Actor_SetSpriteFlags)

extern const struct ActorPlacement gActorPlacementsEntrance99[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 SceneActor_ApplyCounterLowBitsAsMode(struct Actor *actor) {
  FieldActor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
    return gActorPlacementsEntrance99;
  return gActorPlacements;
}
