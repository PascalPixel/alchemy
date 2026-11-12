/// @file
/// The scene code of overlay 0x64d that its scene tables belong to, in ROM order; the rest of the overlay is in venus_lighthouse_entrance/.

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
#include "event_inline.h"

void Func_020023a4(void *object);
void Func_0200260c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct Actor *object, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void VenusLighthouseEntrance_RunActorSparkBurst(void *object) {
  Func_02002a34(8, 0x4084);
  Func_02002a44();
  Func_02002a5c(Actor_Get(5), 2);
  FieldEvent_Wait(20);
  Func_020023a4(object);
  Func_02002a5c(Actor_Get(5), 0);
  Func_02002a54();
  Func_02002a4c();
  Field_StartPaletteTransition(8);
}

void VenusLighthouseEntrance_UpdateSparkTrail(struct Actor *object) {
  Func_0200260c(object, object->x, object->y, object->z);
  if (++object->action % 8 == 0)
    Event_PlaySound(125);
}

void Func_020026f4(void);
void VenusLighthouseEntrance_StopSparks(void);

/// The spark trail while the actor steps 96 out and 96 back.
void Func_020000c8(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  Func_020026f4();
  object->update = VenusLighthouseEntrance_UpdateSparkTrail;
  object->priorityFlags = 0;
  {
    s32 a = actor;
    /* FAKEMATCH(pin): the 96 pinned to r1; as
     * Actor_CommitPositionAndActivate(a, 96, dz), the pin below kept, 2 of 35
     * instructions differ in tla-en. */
    register s32 dx asm("r1") = 96;
    /* FAKEMATCH(pin): the -96 pinned to r2, the same call's second; as
     * Actor_CommitPositionAndActivate(actor, 96, -96) with neither pin nor the
     * copy, 23 of 35 instructions differ in tla-en. */
    register s32 dz asm("r2") = -96;

    Actor_CommitPositionAndActivate(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is negated
     * after the actor is copied; as Actor_CommitPositionAndActivate(
     * actor, -96, 96), 2 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    s32 dx = -96;

    Actor_CommitPositionAndActivate(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopSparks();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }
