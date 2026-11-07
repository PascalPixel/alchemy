#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern const u8 MsgByReachingThis[];
extern const u8 gData_020012a4[];
struct Actor *Actor_GetById(u32 id);
void Func_0200119c(void);
void Func_020011a4(void);
void Func_0200125c(s32 skip);
void Func_02001244(s32 a, s32 b);
s32 Func_0200123c(s32 a, s32 b);
s32 Func_0200124c(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001284(s32 cue);
void FieldEvent_Wait(s32 frames);
void Func_020011e4(const u8 *message);
void Func_0200118c(s32 line, s32 a, s32 b);
void Func_020011cc(s32 actor, s32 x, s32 z);
struct Actor *Func_0200115c(s32 type, s32 x, s32 y, s32 z);
void Func_02001154(struct Actor *object, const u8 *script);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020009bc(void);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Func_020011d4(s32 actor, s32 target);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_020011ec(s32 actor, s32 a);
s32 Func_020011fc(s32 actor, s32 emote, s32 frames);
void GameFlag_SetBit(s32 flag);
s32 Func_02001264(void);
void Func_02001214(s32 actor, s32 a);

union EventLine {
  s32 word;
  u16 line;
};

#define TALK()                                                                 \
  Call3(Func_0200118c, (s16)((union EventLine *)work)->line++, 0, -3)

void Func_020009d4(void) {
  struct Actor *follower = Actor_GetById(9);
  struct Actor *statue = Actor_GetById(0x40);
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *glow, *leader;
  s32 speed;

  Func_0200119c();
  Func_0200125c(0);
  Call2(Func_02001244, 0x10000, 0);
  Value2(Func_0200123c, 0x403108, 1);
  Func_0200124c(60);
  Call4(Camera_MoveWithinMapBounds, 0xa80000, -1, 0x9e0000, 1);
  Func_02001284(26);
  FieldEvent_Wait(60);
  Actor_SetModeById(8, 2);
  Func_02001284(0x240);
  FieldEvent_Wait(60);
  Func_020011e4((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Call3(Func_020011cc, 0x40, 0xa80000, 0x980000);
  statue->motionFlags = 6;
  statue->terrainHeight += 0x100000;
  Func_02001284(220);
  glow = Func_0200115c(412, 0, 0, 0);
  Func_02001154(glow, gData_020012a4);
  Animation_ApplyChildValues(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + 0x80000;
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = Func_020009bc;
  FieldEvent_Wait(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  Actor_ResetAndSetPosition(gMainRoutineWork.currentOwner, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_020011d4(9, gMainRoutineWork.currentOwner);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_ResetAndSetPosition(9, 184, 188);
  Actor_WaitForMoveById(9);
  Call3(Actor_ArmCallback, 9, 0x8000, 0);
  Func_020011ec(9, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xc000, 0);
  TALK();
  Func_020011fc(9, 258, 0);
  Call3(Actor_ArmCallback, 9, 0xb000, 0);
  FieldEvent_Wait(30);
  Func_020011ec(9, 0);
  TALK();
  Value3(Func_020011fc, 9, 261, 0);
  FieldEvent_Wait(30);
  TALK();
  Func_020011fc(9, 265, 0);
  FieldEvent_Wait(30);
  Func_020011ec(9, 0);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 0);
  FieldEvent_Wait(10);
  Value3(Func_020011fc, gMainRoutineWork.currentOwner, 257, 0);
  Func_020011fc(9, 257, 0);
  FieldEvent_Wait(60);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Call3(Actor_ArmCallback, 9, 0xb000, 0);
  Value3(Func_020011fc, gMainRoutineWork.currentOwner, 261, 0);
  Func_020011fc(9, 261, 0);
  FieldEvent_Wait(60);
  TALK();
  FieldEvent_Wait(40);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(40);
  Actor_SetModeById(9, 3);
  Func_020011ec(9, 0);
  Actor_SetModeById(8, 2);
  TALK();
  FieldEvent_Wait(30);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Func_020011cc(9, 0, 0);
  Actor_WaitForMoveById(9);
  GameFlag_SetBit(0x9ce);
  Func_02001264();
  Call2(Func_0200123c, speed, 0);
  Func_0200124c(60);
  Func_02001214(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(60);
  Func_020011a4();
}
