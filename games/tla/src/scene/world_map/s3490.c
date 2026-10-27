#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200343c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d40(s32 value);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void ObjectMotion_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004df8(value, mode);
}

void Func_02003490(void) {
  Func_02004c20();
  Func_02004db8(0);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Object_GetById(gPartyState.currentOwner)->update = Func_0200343c;
  Object_GetById(gPartyState.currentOwner)->acceleration = 0x2000;
  Object_GetById(gPartyState.currentOwner)->speedLimit = 0x20000;
  WaitFrames(5);
  Func_02004df8_(0xc000, 60);
  Event_StartScreenTransition();
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14537, 8577);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14465, 8662);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14452, 8766);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14387, 8828);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14331, 8888);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14284, 8904);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14255, 8870);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14255, 8831);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14255, 8772);
  Func_02004df8_(0x14000, 120);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14275, 8736);
  ObjectMotion_SetPositionAndCommit_(gPartyState.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  Func_02004d40(78);
}
