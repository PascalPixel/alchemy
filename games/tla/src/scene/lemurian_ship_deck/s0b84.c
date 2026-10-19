#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"

extern s32 gData_020024b4;
extern s32 gData_020024b0;
extern const u8 gData_0200218c[];
extern const u8 gData_020021e8[];
extern const u8 gData_0200221c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void GameFlag_SetBit(s32 flag);
void Object_LinkPair(s32 id, s32 partner, s32 flags);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 flags);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
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
struct ObjectRuntime *Func_02002074(void);
void Func_0200207c(s32 countdown);
void Func_020020ac(s32 skip);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);

static __inline__ void AddCallback(void (*callback)(void), s32 priority) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Scheduler_AddOrUpdateCallback(callback, priority);
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
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1560000, 0x300000, 0x1980000,
        0);
  Call1((void (*)())WaitFrames, 1);
  Call6((void (*)())Func_02001f2c, 60, 53, 67, 16, 7, 3);
  Call4((void (*)())Func_02001fcc, gPartyState.currentOwner, 0x2280000,
        0x1780000, 0);
  /* FAKEMATCH: the two-instruction angle set before its call is built ahead
   * of the position arguments; as a constant argument, 4 instructions differ
   * in tla-en. */
  angle = 0xd000;
  Call4((void (*)())Func_02001fcc, 9, 0x22e0000, 0x1590000, angle);
  Call4((void (*)())Func_02001fcc, 7, 0x2160000, 0x1940000, 0);
  turn = 0x2000;
  Call4((void (*)())Func_02001fcc, 6, 0x2380000, 0x1980000, turn);
  angle = 0x4000;
  Call4((void (*)())Func_02001fcc, 5, 0x2220000, 0x1b80000, angle);
  Func_02001f1c();
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 5,
        (s32)gData_0200218c);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 6,
        (s32)gData_020021e8);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 9,
        (s32)gData_0200221c);
  Event_StartScreenTransition();
  gData_020024b4 = 0;
  Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000258, 0x480);
  gData_020024b0 = 0;
  Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000288, 0x480);
  Call2((void (*)())Func_0200205c, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x21e0000, 0x2c0000, 0x18c0000,
        1);
  Func_0200206c();
  Call1((void (*)())FieldEvent_WaitFrames, 80);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call1((void (*)())Func_0200201c, (s32)MsgTakeTheTiller);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3(Func_0200204c, gPartyState.currentOwner, 0x100, 0);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  Value3((s32 (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x6000,
         20);
  Call2((void (*)())Func_02002034, 7, 0);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 5);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 9);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 6);
  Call1((void (*)())WaitFrames, 1);
  Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Value3(Func_0200204c, 6, 0x101, 20);
  Call2((void (*)())Func_02002034, 6, 0);
  Func_02002044(9, turn);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 5, 0x108, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 7, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gPartyState.currentOwner, 0) ==
      0) {
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Value3(Func_0200204c, 9, 0x109, 20);
    Call2((void (*)())Func_02002034, 9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Value3(Func_0200204c, 9, 0x109, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Call2((void (*)())Func_02002034, 9, 0);
  }
  Value3(Func_0200204c, 7, 0x100, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 6, 0x101, 20);
  Call2((void (*)())Func_02002034, 6, 0);
  Call2((void (*)())Func_02002044, 9, 0x3000);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3(Func_0200204c, 5, 0x100, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002044, 9, 0x5000);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Func_02002034, 6, 0);
  Value3(Func_0200204c, 5, 0x108, 0);
  Call2((void (*)())Func_02002044, 5, 0xe000);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002054, 6, 0x102);
  Call2((void (*)())Func_02002044, 6, 0x6000);
  Call2((void (*)())Func_02002034, 0x4006, 0);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0xc000,
        0);
  Value3(Func_0200204c, 7, 0x101, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3(Func_0200204c, 9, 0x108, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())ObjectMotion_Launch, 6, 4, 10);
  Call2((void (*)())Func_02002034, 6, 0);
  Call3((void (*)())ObjectMotion_Launch, 5, 2, 10);
  Call3((void (*)())ObjectMotion_Launch, 5, 4, 10);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call2((void (*)())Func_02002034, 9, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gPartyState.renderMode = 7;
  gPartyState.ownerOverride = 1;
  Call1((void (*)())GameFlag_SetBit, 0x8de);
  Call1((void (*)())Func_0200207c, 10);
}

/// Sends actor id to actor 4's spot at a run.
static inline void RunToActor4(s32 id) {
  struct ObjectRuntime *target;

  Call3((void (*)())ObjectMotion_SetSpeedParameters, id, 0x13333, 0x9999);
  Call2((void (*)())Object_SetModeById, id, 2);
  target = Object_GetById(4);
  if (target != NULL)
    ObjectMotion_ResetAndSetPosition(id, ((s16 *)&target->x)[1],
                                     ((s16 *)&target->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, id);
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
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1ca0000, 0x300000, 0x1980000,
        0);
  Call1((void (*)())Func_0200201c, (s32)MsgHoldOnTo);
  Call4((void (*)())Func_02001fcc, 4, 0x1d60000, 0x1880000, 0);
  Call4((void (*)())Func_020020b4, 6, 0, -24, 0x4000);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  Call4((void (*)())Func_020020b4, 5, -16, -16, 0x2000);
#else
  {
    /* FAKEMATCH: the European editions build -16 once in r1 and copy it to r2;
     * as two constants it is built twice, and as one variable (copied, chained
     * or in an array) it takes r2 and is copied to r1 (differ 6). */
    register s32 offset asm("r1") = -16;

    Call4((void (*)())Func_020020b4, 5, offset, offset, 0x2000);
  }
#endif
  Call4((void (*)())Func_020020b4, 9, -24, 8, 0xf000);
  Call4((void (*)())Func_020020b4, 7, -4, 24, 0xe000);
  Call6((void (*)())Func_02001f2c, 60, 53, 67, 16, 7, 3);
  Func_02001f1c();
  Call1((void (*)())WaitFrames, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gData_020024b4 = 0;
  Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000258, 0x480);
  gData_020024b0 = 0;
  Value2(Scheduler_AddOrUpdateCallback, (s32)Func_02000288, 0x480);
  Value3(Func_0200204c, 6, 0x102, 30);
  Call2((void (*)())Func_02002044, 6, 0x4000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Call2((void (*)())Func_02002044, 5, 0xe000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Value3(Func_0200204c, 4, 0x102, 30);
  Call2((void (*)())Func_02002044, 4, 0x4000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Func_02002044, 7, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x4000, 0);
  Call2((void (*)())Func_02002044, 5, 0x2000);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002044, 5, 0xe000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Call2((void (*)())Func_02002044, 6, 0x6000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Func_02002044, 6, 0x4000);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Value3(Func_0200204c, 7, 0x109, 30);
  Call2((void (*)())Func_02002044, 7, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call2((void (*)())Func_02002044, 9, 0x2000);
  Value3(Func_0200204c, 9, 0x101, 30);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3(Func_0200204c, 7, 0x102, 30);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())Object_LinkPair, 6, 5, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x4000, 0);
  Call2((void (*)())Func_02002044, 5, 0x2000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3(Func_0200204c, 6, 0x101, 30);
  Call3((void (*)())Func_0200202c, 6, 0, 5);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 5, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_0200202c, 5, 0, 5);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 7, 0, 5);
  Value3(Func_0200204c, 9, 0x102, 30);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 9, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x6000, 0);
  Call2((void (*)())Func_02002044, 9, 0xe000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Func_02002044, 9, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 4);
  Call3((void (*)())Func_0200202c, 9, 0, 5);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Object_LinkPair, 9, 7, 0);
  Call3((void (*)())Object_LinkPair, 6, 5, 0);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 9, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  RunToActor4(7);
  RunToActor4(5);
  RunToActor4(6);
  RunToActor4(9);
  Call1((void (*)())Func_0200207c, 1);
}

void Func_02000b84(void) {
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 0x20;
  AddCallback(Func_02000188, 0x480);
  AddCallback(Func_020001cc, 0x480);
  if (gPartyState.entrance == 10)
    Func_020002cc();
  /* FAKEMATCH: the one-pass block keeps the second entrance test a load of its
   * own (ldrsh, cmp #20); without it the two tests share a halfword load and
   * compare shifted (differ 33). */
  do {
  } while (0);
  if (gPartyState.entrance == 20)
    Func_020006e4();
}
