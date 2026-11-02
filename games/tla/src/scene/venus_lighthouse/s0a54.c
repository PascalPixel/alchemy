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

#define Actor_SetPartAttribute                                                 \
  ((void (*)(struct FieldActor *, s32))Actor_SetPartAttribute)

enum { ENTRANCE_OPENING = 99 };

struct AerieEventWork {
  u8 unk000[0x1ac];
  s32 mode;
};

// The Japanese edition has no form of this function here.
#if (LANGUAGE != LANGUAGE_JAPANESE)
s32 VenusLighthouse_Start(void) {
  struct MainRoutineWork *routine;

  routine = &gMainRoutineWork;
  ((struct AerieEventWork *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (routine->entrance == ENTRANCE_OPENING) {
    // The international editions also save this scene and entrance 22.
    routine->savedScene = (s32)&SceneId_VenusLighthouse;
    routine->savedEntrance = 22;
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
