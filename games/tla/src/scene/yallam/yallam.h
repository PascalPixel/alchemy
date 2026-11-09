#ifndef TLA_YALLAM_H
#define TLA_YALLAM_H

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 MsgDoYouWant[];
extern const u8 MsgRunningAroundIn[];
extern const u8 MsgThereSALot[];
extern const u8 MsgThereSAGame[];
extern const u8 MsgIOnceHeard[];

void Func_020006bc(s32 id);
void Func_02001944(void);
void Func_02002488(void);
void Func_02002490(void);

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
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_02000088(struct Actor *object, struct Actor *target,
                  s32 range, s32 force);
void Func_02001688(void);
void Func_02002448(struct Actor *object, s32 mode);

#endif
