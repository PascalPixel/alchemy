// differ 2: after Func_02004d78 the native loads the event work before it
// starts the offset of the frame count (which it keeps in r6); here the offset
// starts first. A barrier after the load gives that order but cuts the later
// calls from the arguments of Func_02004d78, which then load r1 last (differ 2
// again).
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
  u8 unk1b0[4];
  s32 frames;
};

extern const u8 gData_02004eb8[];

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Camera_FollowActor(s32 actor, s32 target);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 actor, const u8 *action);
s32 Task_Add(void (*func)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020028cc(void);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004d78(s32 a, s32 b);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_02004d78_(s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02004d78(a, b);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02004df8(value, mode);
}

void Func_020029a4(void) {
  struct Actor *object = Actor_GetById(8);

  FieldEvent_Wait(60);
  Func_02004c20();
  Func_02004db8(0);
  Func_02004df8_(0x9999, 1);
  object->scaleX = 0x13333;
  object->scaleY = 0x13333;
  Camera_FollowActor(8, 1);
  wait(1);
  Actor_SetPartAttribute(Actor_GetById(gSequenceParentWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  Actor_SetSpeedParameters_(8, 0x6666, 0x3333);
  object->action = 0;
  Actor_SetMovementTypeById(8, gData_02004eb8);
  Task_Add(Func_020028cc, 0x480);
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 256;
  Func_02004d78_(0x10003, 1);
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->frames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  Func_02004df8_(0x16666, 300);
  FieldEvent_Wait(270);
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->frames = 16;
  *(u16 *)0x05000000 = 0x7fff;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d40(77);
}
