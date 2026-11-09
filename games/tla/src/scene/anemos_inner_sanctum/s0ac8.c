#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "event_inline.h"

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
void Event_WaitTransitionFrames();
void Func_0200088c();
void Func_020009b8();
void Func_02000a78();
void Func_020032e8();
void Func_020032f8();
void Func_02003330();
void Func_02003338();
void Func_02003380();
void Func_02003398();
void Func_020033a0();
void Func_02003430();
void Actor_SetSingleChildFlags();
s32 Random16Far();
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
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, x, z);
  FieldCamera_MoveTo(x << 16, -1, z << 16, 1);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  actor = ((struct Actor *)Import_ActorCreate((u16)type, leader->x,
                                              leader->y + 0x80000, leader->z));
  actor->motionFlags = 0;
  actor->scaleY = 0x1999;
  actor->scaleX = 0x1999;
  Actor_SetSingleChildFlags((s32)actor, 0);
  Func_020032e8(actor, gData_020035cc);
  Animation_SetStateFlags(actor, 1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(60);
  Func_02003338();
  alpha = alpha & 255;
  REG_BLDALPHA = 0x1000;
  t0 = x >> 4;
  t1 = z >> 4;
  column = t0;
  row = t1;
  MapGrid_CopyCellsTo(t0 + 63, t1 + 23, t0 + 63, t1 - 3, 3, 5);
  i = 0;
  if (alpha != 0) {
  fadeIn:
    REG_BLDALPHA = i | 0x1000;
    System_Wait(8);
    i++;
    if (i != alpha)
      goto fadeIn;
  }
  Func_02003330();
  Event_PlaySound(246);
  FieldEvent_Wait(48);

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
      Event_PlaySound(195);
    i++;
    FieldEvent_Wait(8);
  }

  if (count != 18) {
    FieldEvent_Wait(96);
    Func_020032e8(actor, gData_02003620);
    Func_02003338();
    alpha = REG_BLDALPHA & 255;
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(60);
    for (i = alpha; i != 0;) {
      REG_BLDALPHA = i | 0x1000;
      i--;
      System_Wait(8);
    }
    for (i = 0; i != count;) {
      s32 slot = order[i];

      i++;
      MapGrid_CopyCellsTo(
          64, 0, ((leader->x + (gData_020036fc[slot][0] << 16)) >> 20) + 64,
          (leader->z + (gData_020036fc[slot][1] << 16)) >> 20, 1, 1);
    }
    MapGrid_CopyCellsTo(64, 0, column + 63, row - 3, 3, 5);
    Func_02003330();
    Event_WaitTransitionFrames();
  } else {
    Event_SetFlag((s16)work->room + 0xa55);
    Func_020032e8(actor, gData_02003620);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x40000, 0x8000);
    FieldCamera_MoveTo(0x2180000, -1, 0x780000, 1);
    Func_02003430();
    Event_PlaySound(220);
    {
      s32 a = targetX << 16;
      s32 b = targetZ << 16;

      Func_020009b8(a, b, part, 0x800);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0x900);
      FieldEvent_Wait(10);
      Func_020009b8(a, b, part, 0xa00);
    }
    FieldEvent_Wait(12);
    System_Wait(16);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(32);
    Event_PlaySound(0x23e);
    Call3((void (*)())Func_02000a78, targetX, targetZ, part);
    FieldEvent_Wait(80);
    if (GameFlag_Test(0xa87) && GameFlag_Test(0xa88) && GameFlag_Test(0xa89) &&
        GameFlag_Test(0xa8a)) {
      Event_SetFlag(0xa8b);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(52, 7, 33, 7, 1, 2);
      System_Wait(5);
      MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
      Func_02003380(0x2180000, 0x880000, 0, 0);
      System_Wait(60);
    }
  }

  Func_020032f8(actor);
  Event_SetFlag(0x200);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003430();
  FieldEvent_Wait(60);
  Func_020033a0();
}
