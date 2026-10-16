// Draft of Func_020029a4 (overlay 64B, 99 instructions), differ 2: the world
// map's opening rise, ported from ☀️'s WorldMap_RunOpeningRise (⚓️ adds
// Func_02004db8(0), scale 0x13333 and exit 77). The one difference: after
// Func_02004d78 the native loads the event work ([r5, #108]) before it starts
// 436 (movs r6, #218); here 436 comes first. Both have sched2 priority 79; the
// constant into saved r6 has no dependence on the call just scheduled, the load
// an anti one, so the constant wins. Measured: the store through an inline
// helper (differ 36, 436 no longer shared), Func_02004d78 through a forwarding
// inline (no change), a one-pass block before or around the store or around the
// call, or an asm volatile after the call (the store matches, but the r5/0x480
// setup before the scheduler call turns, differ 4 to 12). alchemy permute, 5
// minutes: nothing closer.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct EventWork {
  u8 unk000[0x1ac];
  s32 startTransition;
  u8 unk1b0[4];
  s32 transitionFrames;
};

extern const u8 gData_02004eb8[];

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_AttachWorkTargetToObject(s32 actor, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const u8 *action);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020028cc(void);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004d78(s32 a, s32 b);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

void Func_020029a4(void) {
  struct ObjectRuntime *actor;

  actor = Object_GetById(8);
  Battle_WaitMode0(60);
  Func_02004c20();
  Func_02004db8(0);
  Func_02004df8(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  Object_AttachWorkTargetToObject(8, 1);
  WaitFrames(1);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
  ObjectMotion_SetSpeedParameters_(8, 0x6666, 0x3333);
  actor->action = 0;
  ObjectMotion_EnableActionAndSetCallback(8, gData_02004eb8);
  Scheduler_AddOrUpdateCallback(Func_020028cc, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x100;
  Func_02004d78(0x10003, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 32;
  Event_StartScreenTransition();
  Battle_WaitMode0(120);
  Func_02004df8(0x16666, 300);
  Battle_WaitMode0(270);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 16;
  *(u16 *)0x05000000 = 0x7fff;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d40(77);
}
