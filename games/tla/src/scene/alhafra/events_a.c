#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void FieldEvent_WaitTicks(s32 ticks);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001bc4(s32 script, s32 x, s32 z);
void Func_02001be4(void);
void Func_02001bec(void);
void Func_02001cd4(s32 countdown);
void Func_02001cfc(s32 a);
void Func_02001d4c(s32 a);

extern const u8 gData_02001e48[];
extern const u8 gData_02002100[];
extern const u8 gData_02002358[];
extern const u8 gData_020025b0[];
extern struct DoorStep gData_0200277c[];

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(0x8a7)) {
    if (GameFlag_Test(0x90f) == 0)
      return gData_020025b0;
    return gData_02002358;
  }
  if (GameFlag_Test(0x8ab))
    return gData_02002100;
  return gData_02001e48;
}

void Func_02000098(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02001be4();
  Func_02001cfc(0);
  Func_02001d4c(158);
  Func_02001bc4(step->script, step->x, step->z);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 2;
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  Object_SetModeById(gPartyState.currentOwner, 2);
  Call3((void (*)())ObjectMotion_CenterOnColumn, gPartyState.currentOwner, 2,
        -8);
  FieldEvent_WaitTicks(10);
  Func_02001cd4(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001bec();
}

void Func_0200010c(s32 index) {
  struct DoorStep *table = gData_0200277c;

  /* FAKEMATCH: a one-pass block loads the table address before the index copy
   * (two reordered rows). */
  do {
  } while (0);
  Func_02000098(&table[index], index, 0);
}
