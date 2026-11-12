/// @file
/// The scene code of overlay 0x682 that its scene tables belong to, in ROM order; the rest of the overlay is in champa/.

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
#include "event_inline.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_event.h"

void Func_02002a44(void);
void Func_02002a4c(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s16 gData_02002c5c[];
extern s16 gData_02002efc[];

const s16 *Func_0200004c(void) {
  if (Event_GetFlag(0x97f))
    return gData_02002efc;
  return gData_02002c5c;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200006c(s32 a0) {
  FIELD_AT(a0, s32, 24) = (FIELD_AT(a0, s32, 24) + -1536);
  FIELD_AT(a0, s32, 28) = (FIELD_AT(a0, s32, 28) + -1536);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000080(const void *warp, s32 door, s32 mode) {
  s32 id;

  Func_02002a44();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  ((struct FieldActor *)Actor_Get(gMainRoutineWork.currentOwner))->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002a4c();
}

extern s32 gData_020031fc[];
void Func_020029f4(s32 a);

void Func_020000fc(s32 door) {
  s32 *table = gData_020031fc;

  if ((u32)(door - 1) <= 1)
    *(void **)((u8 *)((struct FieldActor *)Actor_Get(gMainRoutineWork.currentOwner)) + 108) =
        Func_0200006c;
  if (door == 7) {
    Func_020029f4(8);
    Func_02000080(table, 7, 1);
  } else {
    Func_02000080(table + 2, door, 0);
  }
}

void Func_0200014c(void) {
  s32 t2;
  s32 t1;

  TreasureSpot_SetPosition(0x40, 0x1880000, 0x2380000);
  t1 = ((s32)Actor_Get(64));
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  t2 = ((s32)Actor_Get(64));
  FIELD_AT(t2, s32, 12) = 0x200000;
}

void Func_02000178(void) { TreasureSpot_SetPosition(0x40, 0, 0); }
