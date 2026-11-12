/// @file
/// Scene code of overlay 0x6a8, in ROM order, from s19a4's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "call.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

extern const u8 gData_020046f8[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004464(void);

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(0x200)) {
    if (mode == 2)
      Event_PlaySound(188);
    else
      Event_PlaySound(158);
    MapGrid_CopyCellsTo(a, b, c, d, mode, f);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    FieldActor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Actor_WaitForMove(object);
    object->angle = 0xc000;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner,
                                          gData_020046f8);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->returnEntrance);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001a9c(void) { Func_020019a4(0x30, 0x27, 0x39, 0x1a, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001ab8(void) { Func_020019a4(0x31, 0x27, 0x41, 0x18, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001ad4(void) { Func_020019a4(0x32, 0x27, 0x38, 0x13, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001af0(void) { Func_020019a4(0x33, 0x27, 0x3d, 0xd, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001b0c(void) { Func_020019a4(0x33, 0x27, 0x50, 0x10, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001b28(void) { Call6(Func_020019a4, 0x34, 0x27, 0x32, 9, 2, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001b44(void) { Func_020019a4(0x57, 2, 0x50, 7, 1, 2); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001b60(void) { Func_020019a4(0x59, 0, 0x4d, 0x10, 1, 2); }

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02001b7c(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 entrance = work->returnEntrance;
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 cell;
  s32 id;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  cell = object->x >> 16;
  FieldActor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z - 0x40000);
  Actor_WaitForMove(object);
  object->angle = 0xc000;
  Event_PlaySound(128);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  id = gMainRoutineWork.currentOwner;
  if (entrance == 6)
    Actor_SetAnimation(id, 25);
  else
    Actor_SetAnimation(id, 26);
  System_Wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
}
