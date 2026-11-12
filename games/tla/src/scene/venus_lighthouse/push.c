/// @file
/// Scene code of overlay 0x64e, in ROM order, from push's offset up to the next object the overlay links.

#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "tla_edition.h"
#include "scene_ids.h"
#include "version.h"

void SceneState_RunActor13AtColumn42Setup(void) {
  struct FieldActor *obj;
  s32 val;
  s32 a;
  s32 b;

  obj = Actor_Get(10);
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  if (obj->x >> 20 == 42) {
    FieldEvent_Wait(30);
    Event_PlaySound(188);
    obj->motionFlags = 0;
    val = 0xfffe0000;
    obj->terrainHeight = val;
    obj->y = val;
    Event_SetFlag(0x200);
    a = 3;
    b = 5;
    MapGrid_CopyCellsTo(44, 117, 41, 117, a, b);
  }
  Import_EventEnd();
}

void VenusLighthouse_ReadRelief(void) {
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
  Import_EventEnd();
}

void VenusLighthouse_ReadTrueHeart(void) {
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
  Import_EventEnd();
}

void SceneState_ApplySixRectsAfter161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(35, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(35, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(99, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(57, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(57, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(36, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(36, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(100, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(53, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(53, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(117, 55, 110, 55, a, a);
}

void Func_02000b2c(void);

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
  FieldEvent_Wait(10);
  Field_DispatchTypeHandler(1);
  Func_02001c8c();
  Func_02001c94();
}

struct AerieEventWork {
  u8 unk000[0x1ac];
  s32 mode;
};

// The Japanese form; from patch 1, the form in s0a54.c also saves this scene
// and entrance 22.
#if !TLA_PATCH_1
s32 VenusLighthouse_Start(void) {
  ((struct AerieEventWork *)Ram_HeapSlots->eventWork)->mode = 0x204;
  if (gMainRoutineWork.entrance == ENTRANCE_OPENING) {
    Import_EventStart();
    FieldEvent_ResolvePendingActions(0);
    Func_02000b2c();
    Func_02000d30();
    Import_EventEnd();
  } else {
    Actor_SetSpriteColor(Actor_Get(10), 6);
    Actor_SetSpriteColor(Actor_Get(17), 6);
    Actor_SetSpriteColor(Actor_Get(18), 6);
    Actor_SetSpriteColor(Actor_Get(19), 6);
    Actor_SetSpriteColor(Actor_Get(20), 6);
    Actor_SetSpriteColor(Actor_Get(21), 6);
    Actor_SetSpriteColor(Actor_Get(22), 6);
    Actor_SetSpriteColor(Actor_Get(23), 6);
    Actor_SetSpriteColor(Actor_Get(24), 6);
    Actor_SetSpriteColor(Actor_Get(25), 6);
  }
  return 0;
}
#endif
