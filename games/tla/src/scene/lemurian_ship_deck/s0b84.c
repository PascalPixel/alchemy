#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "scene_tables.h"

extern s32 gData_020024b4;
extern s32 gData_020024b0;
extern const u8 gData_0200218c[];
extern const u8 gData_020021e8[];
extern const u8 gData_0200221c[];

struct Actor *Actor_GetById(s32 id);
s32 Task_Add(void (*func)(void), s32 priority);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void Actor_TurnToFaceEachOther(s32 id, s32 partner, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_Stop(s32 id);
void Actor_Jump(s32 id, s32 speed, s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 flags);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000258(void);
void Func_02000288(void);
void Func_020002cc(void);
void Func_020006e4(void);
void Func_02001f1c(void);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f5c(void);
void Func_02001fc4(s32 id, s32 x, s32 z);
void Func_02001fcc(s32 id, s32 x, s32 z, s32 angle);
void Func_0200201c(const void *message);
void Func_0200202c(s32 id, s32 a, s32 b);
void Func_02002034(s32 id, s32 flags);
void Func_02002044(s32 id, s32 angle);
s32 Func_0200204c(s32 id, s32 emote, s32 frames);
void Func_02002054(s32 id, s32 a);
void Func_0200205c(s32 speed, s32 acceleration);
void Func_0200206c(void);
struct Actor *Func_02002074(void);
void Func_0200207c(s32 countdown);
void Func_020020ac(s32 skip);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);

static __inline__ void AddTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Task_Add(func, priority);
}

/// The scene run from entrance 10: the crew takes its places on deck and the
/// leader is asked to take the tiller; it ends by setting flag 0x8de.
void Func_020002cc(void) {
  s32 angle;
  s32 turn;

  Func_02001f5c();
  Call1((void (*)())Func_020020ac, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02002074()->motionFlags = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(342), PIXELS(48),
        PIXELS(408), 0);
  Call1((void (*)())wait, 1);
  Call6((void (*)())Func_02001f2c, 60, 53, 67, 16, 7, 3);
  Call4((void (*)())Func_02001fcc, gMainRoutineWork.currentOwner, PIXELS(552),
        PIXELS(376), FACING_EAST);
  /* FAKEMATCH: the two-instruction angle set before its call is built ahead
   * of the position arguments; as a constant argument, 4 instructions differ
   * in tla-en. */
  angle = 0xd000;
  Call4((void (*)())Func_02001fcc, 9, PIXELS(558), PIXELS(345), angle);
  Call4((void (*)())Func_02001fcc, 7, PIXELS(534), PIXELS(404), FACING_EAST);
  turn = 0x2000;
  Call4((void (*)())Func_02001fcc, 6, PIXELS(568), PIXELS(408), turn);
  angle = 0x4000;
  Call4((void (*)())Func_02001fcc, 5, PIXELS(546), PIXELS(440), angle);
  Func_02001f1c();
  Call1((void (*)())wait, 1);
  Call2((void (*)())Actor_SetMovementTypeById, 5, (s32)gData_0200218c);
  Call2((void (*)())Actor_SetMovementTypeById, 6, (s32)gData_020021e8);
  Call2((void (*)())Actor_SetMovementTypeById, 9, (s32)gData_0200221c);
  Event_StartScreenTransition();
  gData_020024b4 = 0;
  Value2(Task_Add, (s32)Func_02000258, 0x480);
  gData_020024b0 = 0;
  Value2(Task_Add, (s32)Func_02000288, 0x480);
  Call2((void (*)())Func_0200205c, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(542), PIXELS(44),
        PIXELS(396), 1);
  Func_0200206c();
  Call1((void (*)())FieldEvent_Wait, 80);
  Call2((void (*)())Func_02002044, 7, FACING_NORTHEAST);
  Call1((void (*)())Func_0200201c, (s32)MsgTakeTheTiller);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3(Func_0200204c, gMainRoutineWork.currentOwner, 0x100, 0);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  Value3((s32 (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
         FACING_SOUTHWEST, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Call1((void (*)())Actor_Stop, 5);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())Actor_Stop, 6);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTH + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Value3(Func_0200204c, 6, 0x101, 20);
  Call2((void (*)())Func_02002034, 6, 0);
  Func_02002044(9, turn);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 5, 0x108, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 7, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gMainRoutineWork.currentOwner,
             0) == 0) {
    Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTH + FACING_STEP, 0);
    Value3(Func_0200204c, 9, 0x109, 20);
    Call2((void (*)())Func_02002034, 9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Call3((void (*)())Actor_ArmCallback, 9, FACING_SOUTH + FACING_STEP, 0);
    Value3(Func_0200204c, 9, 0x109, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Call2((void (*)())Func_02002034, 9, 0);
  }
  Value3(Func_0200204c, 7, 0x100, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 6, 0x101, 20);
  Call2((void (*)())Func_02002034, 6, 0);
  Call2((void (*)())Func_02002044, 9, FACING_SOUTHEAST + FACING_STEP);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 5, 0x100, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002044, 9, FACING_SOUTH + FACING_STEP);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Func_02002034, 6, 0);
  Value3(Func_0200204c, 5, 0x108, 0);
  Call2((void (*)())Func_02002044, 5, FACING_NORTHEAST);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002054, 6, 0x102);
  Call2((void (*)())Func_02002044, 6, FACING_SOUTHWEST);
  Call2((void (*)())Func_02002034, 0x4006, 0);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_NORTH, 0);
  Value3(Func_0200204c, 7, 0x101, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3(Func_0200204c, 9, 0x108, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_Jump, 6, 4, 10);
  Call2((void (*)())Func_02002034, 6, 0);
  Call3((void (*)())Actor_Jump, 5, 2, 10);
  Call3((void (*)())Actor_Jump, 5, 4, 10);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call2((void (*)())Func_02002034, 9, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Call1((void (*)())GameFlag_SetBit, 0x8de);
  Call1((void (*)())Func_0200207c, 10);
}

/// Sends actor id to actor 4's spot at a run.
static inline void RunToActor4(s32 id) {
  struct Actor *target;

  Call3((void (*)())Actor_SetSpeedParameters, id, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, id, 2);
  target = Actor_GetById(4);
  if (target != NULL)
    Actor_ResetAndSetPosition(id, ((s16 *)&target->x)[1],
                              ((s16 *)&target->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, id);
  Call3((void (*)())Func_02001fc4, id, 0, 0);
}

/// The scene run from entrance 20: the camera frames actors 4 to 9 around the
/// spot, two callbacks start the bobbing, and after the exchange of looks and
/// emotes the four actors run to actor 4.
void Func_020006e4(void) {
  Func_02001f5c();
  Call1((void (*)())Func_020020ac, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02002074()->motionFlags = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(458), PIXELS(48),
        PIXELS(408), 0);
  Call1((void (*)())Func_0200201c, (s32)MsgHoldOnTo);
  Call4((void (*)())Func_02001fcc, 4, PIXELS(470), PIXELS(392), FACING_EAST);
  Call4((void (*)())Func_020020b4, 6, 0, -24, FACING_SOUTH);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  Call4((void (*)())Func_020020b4, 5, -16, -16, FACING_SOUTHEAST);
#else
  {
    /* FAKEMATCH: the European editions build -16 once in r1 and copy it to r2;
     * as two constants it is built twice, and as one variable (copied, chained
     * or in an array) it takes r2 and is copied to r1 (differ 6). */
    register s32 offset asm("r1") = -16;

    Call4((void (*)())Func_020020b4, 5, offset, offset, FACING_SOUTHEAST);
  }
#endif
  Call4((void (*)())Func_020020b4, 9, -24, 8, FACING_NORTHEAST + FACING_STEP);
  Call4((void (*)())Func_020020b4, 7, -4, 24, FACING_NORTHEAST);
  Call6((void (*)())Func_02001f2c, 60, 53, 67, 16, 7, 3);
  Func_02001f1c();
  Call1((void (*)())wait, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gData_020024b4 = 0;
  Value2(Task_Add, (s32)Func_02000258, 0x480);
  gData_020024b0 = 0;
  Value2(Task_Add, (s32)Func_02000288, 0x480);
  Value3(Func_0200204c, 6, 0x102, 30);
  Call2((void (*)())Func_02002044, 6, FACING_SOUTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Call2((void (*)())Func_02002044, 5, FACING_NORTHEAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Value3(Func_0200204c, 4, 0x102, 30);
  Call2((void (*)())Func_02002044, 4, FACING_SOUTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Func_02002044, 7, FACING_NORTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 0);
  Call2((void (*)())Func_02002044, 5, FACING_SOUTHEAST);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Func_02002044, 5, FACING_NORTHEAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Call2((void (*)())Func_02002044, 6, FACING_SOUTHWEST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Func_02002044, 6, FACING_SOUTH);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Value3(Func_0200204c, 7, 0x109, 30);
  Call2((void (*)())Func_02002044, 7, FACING_NORTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call2((void (*)())Func_02002044, 9, FACING_SOUTHEAST);
  Value3(Func_0200204c, 9, 0x101, 30);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 7, FACING_NORTHWEST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Value3(Func_0200204c, 7, 0x102, 30);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())Actor_TurnToFaceEachOther, 6, 5, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 0);
  Call2((void (*)())Func_02002044, 5, FACING_SOUTHEAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Value3(Func_0200204c, 6, 0x101, 30);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Call2((void (*)())Func_02002044, 7, FACING_NORTHWEST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Value3(Func_0200204c, 9, 0x102, 30);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 9, FACING_NORTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHWEST, 0);
  Call2((void (*)())Func_02002044, 9, FACING_NORTHEAST);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 9, FACING_NORTH);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 4);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_TurnToFaceEachOther, 9, 7, 0);
  Call3((void (*)())Actor_TurnToFaceEachOther, 6, 5, 0);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_Wait, 40);
  RunToActor4(7);
  RunToActor4(5);
  RunToActor4(6);
  RunToActor4(9);
  Call1((void (*)())Func_0200207c, 1);
}

void Func_02000b84(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  AddTask(Func_02000188, 0x480);
  AddTask(Func_020001cc, 0x480);
  if (gMainRoutineWork.entrance == 10)
    Func_020002cc();
  /* FAKEMATCH: the one-pass block keeps the second entrance test a load of its
   * own (ldrsh, cmp #20); without it the two tests share a halfword load and
   * compare shifted (differ 33). */
  do {
  } while (0);
  if (gMainRoutineWork.entrance == 20)
    Func_020006e4();
}
