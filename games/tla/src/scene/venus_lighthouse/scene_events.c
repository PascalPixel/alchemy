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
#include "field_scene.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "tla_edition.h"
#include "scene_ids.h"
#include "edition.h"

#define Object_SetPartAttribute                                                \
  ((void (*)(struct FieldActor *, s32))Object_SetPartAttribute)

enum { ENTRANCE_OPENING = 99 };

const struct SceneEvent *Scene_GetEvents(void) { return gSceneEvents; }

s32 VenusLighthouse_Idle(void) { return 0; }

struct SetupWork {
  u8 unk00[0x24];
  s32 (*idle)(void);
};

void VenusLighthouse_RunSetupStep(void) {
  Func_02001c7c(140, 1);
  Func_02001c84(4, 10);
  ((struct SetupWork *)Ram_HeapSlots->effectWork)->idle = VenusLighthouse_Idle;
  Func_02001c9c();
  Engine_EventWait(10);
  Field_DispatchTypeHandler(1);
  Func_02001c8c();
  Func_02001c94();
}

struct AerieEventWork {
  u8 unk000[0x1ac];
  s32 mode;
};

s32 VenusLighthouse_Start(void) {
  ((struct AerieEventWork *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (gPartyState.entrance == ENTRANCE_OPENING) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    // The international editions also save this scene and entrance 22.
    {
      s32 scene = (s32)&SceneId_VenusLighthouse;

      /* FAKEMATCH: the native loads the scene id before it builds the
       * saved-scene offset (one reordered row without). */
      __asm__ volatile("");
      gPartyState.savedScene = scene;
    }
    gPartyState.savedEntrance = 22;
#endif
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Func_02000b2c();
    Func_02000d30();
    Engine_EventEnd();
  } else {
    Object_SetPartAttribute(Engine_ActorGet(10), 6);
    Object_SetPartAttribute(Engine_ActorGet(17), 6);
    Object_SetPartAttribute(Engine_ActorGet(18), 6);
    Object_SetPartAttribute(Engine_ActorGet(19), 6);
    Object_SetPartAttribute(Engine_ActorGet(20), 6);
    Object_SetPartAttribute(Engine_ActorGet(21), 6);
    Object_SetPartAttribute(Engine_ActorGet(22), 6);
    Object_SetPartAttribute(Engine_ActorGet(23), 6);
    Object_SetPartAttribute(Engine_ActorGet(24), 6);
    Object_SetPartAttribute(Engine_ActorGet(25), 6);
  }
  return 0;
}
