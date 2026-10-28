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
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "tla_edition.h"
#include "scene_ids.h"
#include "edition.h"

#define Actor_SetPartAttribute                                                \
  ((void (*)(struct FieldActor *, s32))Actor_SetPartAttribute)

enum { ENTRANCE_OPENING = 99 };

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

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
  Import_EventWait(10);
  Field_DispatchTypeHandler(1);
  Func_02001c8c();
  Func_02001c94();
}

struct AerieEventWork {
  u8 unk000[0x1ac];
  s32 mode;
};

// The international editions' form, which also saves this scene and entrance
// 22, is still on the listing.
#if (LANGUAGE == LANGUAGE_JAPANESE)
s32 VenusLighthouse_Start(void) {
  ((struct AerieEventWork *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (gMainRoutineWork.entrance == ENTRANCE_OPENING) {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Func_02000b2c();
    Func_02000d30();
    Import_EventEnd();
  } else {
    Actor_SetPartAttribute(Import_ActorGet(10), 6);
    Actor_SetPartAttribute(Import_ActorGet(17), 6);
    Actor_SetPartAttribute(Import_ActorGet(18), 6);
    Actor_SetPartAttribute(Import_ActorGet(19), 6);
    Actor_SetPartAttribute(Import_ActorGet(20), 6);
    Actor_SetPartAttribute(Import_ActorGet(21), 6);
    Actor_SetPartAttribute(Import_ActorGet(22), 6);
    Actor_SetPartAttribute(Import_ActorGet(23), 6);
    Actor_SetPartAttribute(Import_ActorGet(24), 6);
    Actor_SetPartAttribute(Import_ActorGet(25), 6);
  }
  return 0;
}
#endif
