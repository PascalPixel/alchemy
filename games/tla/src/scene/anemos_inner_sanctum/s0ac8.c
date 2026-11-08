#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"

struct Work {
  u8 pad[340];
  u16 room;
};

struct Globals {
  u8 pad[108];
  struct Work *work;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

void Animation_SetStateFlags();
void Camera_FollowActor();
void Camera_MoveWithinMapBounds();
struct Actor *Import_ActorCreate();
void Event_WaitTransitionFrames();
void FieldEvent_Wait();
void Func_0200088c();
void Func_020009b8();
void Func_02000a78();
void Func_020032e8();
void Func_020032f8();
void Func_02003328();
void Func_02003330();
void Func_02003338();
void Func_02003380();
void Func_02003398();
void Func_020033a0();
void Func_02003400();
void Func_02003420();
void Func_02003430();
void Func_02003458();
void Func_02003460();
void Func_02003468();
void Func_02003488();
void Func_020034b8();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Actor_SetSingleChildFlags();
void Actor_WaitForMoveById();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
void Actor_SetModeById();
s32 Random16Far();
void wait();
extern u8 gData_020035cc[];
extern u8 gData_02003620[];
extern s32 gData_020036fc[][2];

void Func_02000ac8(void) {
  struct Work *work = ((struct Globals *)0x3000000)->work;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  volatile u16 alpha = REG_BLDALPHA;
  s32 x, z;
  s32 targetX = 0;
  s32 targetZ = 0;
  s16 type = work->room + 193;
  s16 room = work->room;
  s32 index = room - 50;
  s32 base;
  u8 count;
  s32 part;
  s32 column, row, t0, t1;
  s32 i;
  u8 last;
  struct Actor *actor;
  u8 found[20];
  u8 order[20];
  struct Vector3 from;
  struct Vector3 to;

  x = ((index & 1) << 8) + 408;
  base = 0;
  count = 0;
  part = 0;
  z = (index & 2) * 3 * 32 + 168;
  switch (room) {
  case 50:
    base = 48;
    targetX = 0x208;
    targetZ = 48;
    if (GameFlag_Test(0xa87))
      goto seen;
    break;
  case 51:
    part = 3;
    base = 68;
    targetX = 0x228;
    targetZ = 48;
    if (GameFlag_Test(0xa88))
      goto seen;
    break;
  case 52:
    part = 10;
    base = 88;
    targetX = 0x1f8;
    targetZ = 40;
    if (GameFlag_Test(0xa89))
      goto seen;
    break;
  case 53:
    part = 2;
    base = 108;
    targetX = 0x238;
    targetZ = 40;
    if (GameFlag_Test(0xa8a)) {
    seen:
      GameFlag_SetBit(0x200);
      return;
    }
    break;
  }

  i = (u16)base;
  if (i != (u16)base + 18) {
  scan:
    if (GameFlag_Test(i)) {
      found[count] = i - base;
      count++;
    }
    i++;
    if (i != (u16)base + 18)
      goto scan;
  }
  if (count == 0)
    return;

  Func_02003398();
  Call1((void (*)())Func_02003488, 0);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0xcccc, 0x6666);
  Call2((void (*)())Func_02003420, 0xcccc, 0x1999);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, x, z);
  Call4((void (*)())Camera_MoveWithinMapBounds, x << 16, -1, z << 16, 1);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Call2((void (*)())Func_02003400, gMainRoutineWork.currentOwner, 0x4000);
  Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 28);
  actor =
      Import_ActorCreate((u16)type, leader->x, leader->y + 0x80000, leader->z);
  actor->motionFlags = 0;
  actor->scaleY = 0x1999;
  actor->scaleX = 0x1999;
  Call2((void (*)())Actor_SetSingleChildFlags, (s32)actor, 0);
  Call2((void (*)())Func_020032e8, (s32)actor, (s32)gData_020035cc);
  Call2((void (*)())Animation_SetStateFlags, (s32)actor, 1);
  Call2((void (*)())Func_02003460, 0x10000, 0);
  Call2((void (*)())Func_02003458, 0x10003, 1);
  Call1((void (*)())Func_02003468, 60);
  Func_02003338();
  alpha = alpha & 255;
  REG_BLDALPHA = 0x1000;
  t0 = x >> 4;
  t1 = z >> 4;
  column = t0;
  row = t1;
  Call6((void (*)())Func_02003328, t0 + 63, t1 + 23, t0 + 63, t1 - 3, 3, 5);
  i = 0;
  if (alpha != 0) {
  fadeIn:
    REG_BLDALPHA = i | 0x1000;
    Call1((void (*)())wait, 8);
    i++;
    if (i != alpha)
      goto fadeIn;
  }
  Func_02003330();
  Call1((void (*)())Func_020034b8, 246);
  Call1((void (*)())FieldEvent_Wait, 48);

  last = count - 1;
  for (i = 0; i != count; i++) {
    s32 pick = Random16Far();
    pick &= last;

    order[i] = found[pick];
    found[pick] = found[last];
    last--;
  }
  for (i = 0; i != count;) {
    s32 slot;

    from.x = leader->x + ((Random16Far() & 31) << 16) - 0x100000;
    from.y = leader->y + ((Random16Far() & 15) << 16) + 0x80000;
    from.z = leader->z + ((Random16Far() & 15) << 16) - 0x80000;
    slot = order[i];
    to.x = leader->x + (gData_020036fc[slot][0] << 16);
    to.y = leader->y;
    to.z = leader->z + (gData_020036fc[slot][1] << 16);
    Func_0200088c(&from, &to, part);
    if (i & 1)
      Call1((void (*)())Func_020034b8, 195);
    i++;
    Call1((void (*)())FieldEvent_Wait, 8);
  }

  if (count != 18) {
    Call1((void (*)())FieldEvent_Wait, 96);
    Call2((void (*)())Func_020032e8, (s32)actor, (s32)gData_02003620);
    Func_02003338();
    alpha = REG_BLDALPHA & 255;
    Call2((void (*)())Func_02003458, 0x10000, 1);
    Call1((void (*)())Func_02003468, 60);
    for (i = alpha; i != 0;) {
      REG_BLDALPHA = i | 0x1000;
      i--;
      Call1((void (*)())wait, 8);
    }
    for (i = 0; i != count;) {
      s32 slot = order[i];

      i++;
      Call6((void (*)())Func_02003328, 64, 0,
            ((leader->x + (gData_020036fc[slot][0] << 16)) >> 20) + 64,
            (leader->z + (gData_020036fc[slot][1] << 16)) >> 20, 1, 1);
    }
    Call6((void (*)())Func_02003328, 64, 0, column + 63, row - 3, 3, 5);
    Func_02003330();
    Event_WaitTransitionFrames();
  } else {
    Call1((void (*)())GameFlag_SetBit, (s16)work->room + 0xa55);
    Call2((void (*)())Func_020032e8, (s32)actor, (s32)gData_02003620);
    Call1((void (*)())FieldEvent_Wait, 60);
    Call2((void (*)())Func_02003420, 0x40000, 0x8000);
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x2180000, -1, 0x780000, 1);
    Func_02003430();
    Call1((void (*)())Func_020034b8, 220);
    {
      s32 a = targetX << 16;
      s32 b = targetZ << 16;

      Func_020009b8(a, b, part, 0x800);
      Call1((void (*)())FieldEvent_Wait, 10);
      Func_020009b8(a, b, part, 0x900);
      Call1((void (*)())FieldEvent_Wait, 10);
      Func_020009b8(a, b, part, 0xa00);
    }
    Call1((void (*)())FieldEvent_Wait, 12);
    Call1((void (*)())wait, 16);
    Call2((void (*)())Func_02003458, 0x7fff, 0);
    Call1((void (*)())Func_02003468, 1);
    Call1((void (*)())wait, 1);
    Call2((void (*)())Func_02003458, 0x10000, 0);
    Call1((void (*)())Func_02003468, 32);
    Call1((void (*)())Func_020034b8, 0x23e);
    Call3((void (*)())Func_02000a78, targetX, targetZ, part);
    Call1((void (*)())FieldEvent_Wait, 80);
    if (GameFlag_Test(0xa87) && GameFlag_Test(0xa88) && GameFlag_Test(0xa89) &&
        GameFlag_Test(0xa8a)) {
      Call1((void (*)())GameFlag_SetBit, 0xa8b);
      Call1((void (*)())Func_020034b8, 188);
      Call6((void (*)())Func_02003328, 52, 7, 33, 7, 1, 2);
      Call1((void (*)())wait, 5);
      Call6((void (*)())Func_02003328, 54, 7, 33, 7, 1, 2);
      Call4((void (*)())Func_02003380, 0x2180000, 0x880000, 0, 0);
      Call1((void (*)())wait, 60);
    }
  }

  Call1((void (*)())Func_020032f8, (s32)actor);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  Call2((void (*)())Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
  Func_02003430();
  Call1((void (*)())FieldEvent_Wait, 60);
  Func_020033a0();
}
