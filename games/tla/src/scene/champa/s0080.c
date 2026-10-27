#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "field_event.h"

struct FieldActor *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002b44(s32 door);
void Func_02002b64(s32 skip);
void Func_02002bc4(s32 cue);

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void
ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000080(const void *warp, s32 door, s32 mode) {
  s32 id;

  Func_02002a44();
  Func_02002b64(0);
  Func_02002bc4(158);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 2;
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  Object_SetModeById(gPartyState.currentOwner, 2);
  id = gPartyState.currentOwner;
  if (mode == 0)
    ObjectMotion_CenterOnColumn_(id, 2, -8);
  else
    ObjectMotion_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_WaitTicks(10);
  Func_02002b44(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002a4c();
}

extern s32 gData_020031fc[];
void Func_0200006c(s32 object);
void Func_020029f4(s32 a);

void Func_020000fc(s32 door) {
  s32 *table = gData_020031fc;

  if ((u32)(door - 1) <= 1)
    *(void **)((u8 *)Object_GetById(gPartyState.currentOwner) + 108) =
        Func_0200006c;
  if (door == 7) {
    Func_020029f4(8);
    Func_02000080(table, 7, 1);
  } else {
    Func_02000080(table + 2, door, 0);
  }
}
