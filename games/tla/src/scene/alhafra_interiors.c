/// @file
/// The scene code of overlay 0x666 that its scene tables belong to, in ROM order; the rest of the overlay is in alhafra_interiors/.

#include "system/main_routine_work.h"

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "alhafra_interiors/alhafra_interiors.h"
#include "event_inline.h"

u8 *Func_0200004c(void) {
  if (Event_GetFlag(0x8a7))
    return gData_0200116c;
  if (Event_GetFlag(0x8ab))
    return gData_02000f14;
  return gData_02000cbc;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000a94(void);
void Func_02000b84(s32 a, s32 b);
void Func_02000b7c(void);
void Func_02000a9c(void);
extern u8 MsgAlhafraInteriors31[];

void Func_02000084(void) {
  Event_SetFlag(0x8b2);
  Func_02000a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors31);
  Func_02000b84(0x1f, 1);
  Actor_WalkToAndWait(4, 452, 335);
  Actor_FaceDirection(4, 0x8000, 0);
  Func_02000b7c();
  Actor_FaceDirection(31, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 4);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_RunRepeatedMotion(31, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_FaceDirection(31, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  Func_02000b7c();
  Func_02000a9c();
}

