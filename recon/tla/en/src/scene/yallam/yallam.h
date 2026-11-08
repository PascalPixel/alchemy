#ifndef TLA_YALLAM_H
#define TLA_YALLAM_H

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 MsgDoYouWant[];
extern const u8 MsgRunningAroundIn[];
extern const u8 MsgThereSALot[];
extern const u8 MsgThereSAGame[];
extern const u8 MsgIOnceHeard[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Func_020006bc(s32 id);
void Func_02001944(void);
void Func_02002488(void);
void Func_02002490(void);
void Func_02002520(const u8 *message);
void Func_02002538(s32 speaker, s32 flags);
void Func_02002590(s32 skip);

struct YallamSaved {
  s32 action;
  s32 mode;
  s32 byte98;
  s32 byte99;
};

struct YallamStop {
  s32 a;
  u16 b;
  u16 c;
};

extern struct YallamSaved gData_02002fc0[5];
extern const struct YallamStop gData_02002ce4[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
s32 Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void Actor_CenterOnColumn(s32 actor, s32 x, s32 z);
void FieldEvent_Wait(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_02000088(struct Actor *object, struct Actor *target,
                  s32 range, s32 force);
void Func_02001688(void);
void Func_02002448(struct Actor *object, s32 mode);
void Func_02002460(s32 a, s32 b, s32 c);
void Func_02002578(s32 index);
void Func_020025a0(s32 cue);

#endif
