#include "local.h"
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"
#include "map.h"

void Func_02000c1c(void) { GameFlag_SetBit(0x201); }

void Func_02000c2c(void) { GameFlag_ClearBit(0x201); }

void Func_02000c3c(void) { Func_02001770(); }

extern char SceneId_Ov690Room4;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void CenterOn(s32 id, s32 column, s32 offset) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which sets the
   * arguments up in their own order; as a direct Actor_CenterOnColumn call,
   * 2 of 74 instructions differ in tla-en (the offset is negated after the
   * column). */
  Actor_CenterOnColumn(id, column, offset);
}

void Func_02000c44(void) {
  u32 i;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop carries no loop notes: the bytes rebuild the zero on every
  // pass, and a for loop hoists it.
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4)
    CopyCells(33, 30, 23, 14, 1, 2);
  else
    CopyCells(27, 29, 40, 16, 1, 2);
  Event_PlaySound(158);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  CenterOn(gMainRoutineWork.currentOwner, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002070();
}

void Func_02000cfc(void) {
  s32 b = 8;
  s32 c = 9;

  /* FAKEMATCH(barrier): the empty asm on the two coordinates, so they load
   * before the first argument as the native has; as Func_02002178(1, 8, 9),
   * 2 of 10 instructions differ in tla-en (the first argument loads first).
   */
  __asm__ volatile("" : "+r"(b), "+r"(c));
  Func_02002178(1, b, c);
  Event_SetFlag(0x203);
}

extern s32 gData_02002c5c;
extern u16 gData_02002c60[];
extern u16 gData_02002c80[];

struct PaletteCycleWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xca4 - 0x168];
  s8 unkca4;
};

void Func_02000d14(void) {
  struct PaletteCycleWork *work =
      (struct PaletteCycleWork *)Ram_HeapSlots->eventWork;
  s32 *frame;
  s32 phase, i;

  if (work->unk166 != 0 || work->unkca4 != 0)
    return;
  frame = &gData_02002c5c;
  if (*frame == -1) {
    u16 *colors = gData_02002c60;
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    copy(colors, (void *)0x5000060, 32);
    copy(gData_02002c80, colors, 32);
    *frame = 0;
  }
  phase = (*frame / 8) % 4;
  for (i = 0; i <= 3; i++)
    gData_02002c80[12 + (phase + i) % 4] = gData_02002c60[12 + i];
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)gData_02002c80;
      q->count = count + 1;
      *entry++ = 0x5000060;
      *entry = 0x84000008;
    }
    IRQ_UNLOCK(saved);
  }
  gData_02002c5c++;
}

void Func_02000e04(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 514;
  FieldTask_AddFrame(Func_02000d14, 0x480);
  Func_02002168(170);
  Func_02000ed0();
}

// The hblank split: above the line Func_02000ed0 sets, BG1 scrolls by one value and
// blending is off; below it, by the other, with BG1 blended.
#include "io_reg.h"
#include "system/io_write.h"

/// The hblank split: above the line Func_02000ed0 sets, BG1 scrolls by one
/// value and blending is off; below it, by the other, with BG1 blended.
void Func_02000e30(void) {
  if (REG_VCOUNT < gData_02002ce0) {
    Io_Write16(gData_02002ce4, &REG_BG1HOFS);
    REG_BLDCNT = 0;
  } else {
    Io_Write16(gData_02002ce6, &REG_BG1HOFS);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

void Func_02000e90(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  s16 y = layer->y >> 16;

  gData_02002ce0 = 1448 - y;
  gData_02002ce4 = layer->x >> 16;
  gData_02002ce6 = (s16)(layer->x >> 16) - (gFrameCount >> 2);
}

void Func_02000ed0(void) {
  s32 priority;

  Func_02001fa8(1, 0, Func_02000e30);
  priority = 0xc80;
  FieldTask_AddFrame(Func_02000e90, priority);
}

void Func_02000ef0(void) { Func_02002190(gData_0200226c); }

void Func_02000f00(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 2;
  Func_02002190(gData_0200226c);
}

void Func_0200156c(s32 id);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Place(s32 id, s32 x, s32 z) { Actor_SetHorizontalPositionWithTerrain(id, x, z); }

void Func_02000f2c(void) {
  struct Actor *object = Actor_GetById(9);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;

  if (!GameFlag_Test(0x204) && column == 11 && row == 12) {
    object->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      object->y -= 0x20000;
      Event_Wait(1);
    }
    Func_0200156c(9);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Place(21, PIXELS(184), PIXELS(184));
    Actor_GetById(21)->motionFlags = 0;
    Actor_GetById(21)->y -= 0x400000;
    CopyCells2(9, 11, 1, 1, 11, 12);
    GameFlag_SetBit(0x204);
  }
}

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which keeps
   * each call's own constants; as direct Actor_SetSpeedParameters calls, 38
   * of 199 instructions differ in tla-en. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_02000fd0(void) {
  struct Actor *object = Actor_GetById(10);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    object->motionFlags = 0;
    // A goto loop carries no loop notes: as a for loop its two uses of the
    // object count double and the object takes the row's register.
    i = 8;
  sink:
    FieldEvent_Wait(1);
    object->y -= 0x20000;
    i--;
    if (i > 0)
      goto sink;
    Func_0200156c(10);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(0xc00000, -1, 0xe00000, 1);
    Speed(10, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldActor_WaitForMoveById(10);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
    Speed(10, 0x26666, 0x13333);
    Actor_SetDestinationOffset(10, 0, 80);
    FieldActor_WaitForMoveById(10);
    Func_0200156c(10);
    if (!Event_GetFlag(0x204)) {
      FieldEvent_Wait(10);
      Speed(10, 0xcccc, 0x6666);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldActor_WaitForMoveById(10);
      Speed(10, 0x28000, 0x14000);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(10, 0, 0);
      Event_SetFlag(0x206);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(10, 0, 12);
      FieldActor_WaitForMoveById(10);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(10, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(0x205);
    }
    Func_02002070();
  } else if (!Flag_Test(0x207) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Flag_Set(0x207);
    Func_02002070();
  }
}

void Func_020011bc(void) {
  Func_02002198();
  Func_02000f2c();
}

void Func_020011c8(void) {
  Func_02002198();
  Func_02000fd0();
}

void Func_020011d4(void) {
  Func_02002198();
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags += 254;
  Func_02000f2c();
  Func_02000fd0();
}

void Func_02001200(void) {
  MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 0);
  MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 10);
}

void Func_0200122c(void) {
  MapGrid_CopyCellAttributes(64, 0, 10, 10, 11, 73);
  MapGrid_CopyCellAttributes(64, 10, 10, 10, 42, 73);
}

void Func_0200125c(void) {
  if (!Event_GetFlag(0x221) && !Flag_Test(0x220)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    CopyCells2(42, 11, 10, 4, 11, 75);
    Event_SetFlag(0x220);
  }
}

void Func_020012c0(void) {
  if (Flag_Test(0x220)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(0x220);
  }
}

extern const u8 gData_02002272[];

void Func_02001304(void) {
  struct Actor *object = Actor_Get(10);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  Func_02001200();
  Func_02002190(gData_02002272);
  if (x == 12 && z == 15) {
    MapGrid_CopyCellAttributes(42, 3, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 13 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 16 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else {
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  }
}

extern const u8 gData_0200232c[];

void Func_02001620(s32 id);

#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    object->y -= 0x20000;                                                      \
    i--;                                                                       \
  } while (i > 0)

void Func_0200138c(void) {
  struct Actor *object;
  s32 column;
  s32 row;
  s32 i;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_FollowActor(10, 1);
  object = Actor_Get(10);
  column = object->x >> 20;
  row = object->z >> 20;
  if (column == 12 && row == 15) {
    Event_SetFlag(0x221);
    object->motionFlags = 0;
    SINK(32);
    Func_02001620(10);
    FieldEvent_Wait(20);
  } else if (!Flag_Test(0x205) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Flag_Set(0x205);
    Event_SetFlag(0x221);
    SINK(48);
    Func_02001620(10);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    Event_SetFlag(0x221);
    SINK(32);
    Func_02001620(10);
    if (!Event_GetFlag(102)) {
      Actor_SetPosition(11, 0, 0);
      Actor_SetScript(10, gData_0200232c);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(0x204) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16);
    Func_02001620(10);
    FieldEvent_Wait(20);
    Event_SetFlag(0x204);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Func_02002070();
}

void Func_0200155c(void) {
  Func_02002198();
  Func_0200122c();
  Func_0200138c();
}

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SettlementsSpawn {
  s32 unk00;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

extern const u8 gData_02002310[];

void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct SettlementsSpawn *spawn);

void Func_0200156c(s32 id) {
  /* FAKEMATCH(dead): an unused local that only sizes the frame: its twelve
   * bytes are the gap the native stack frame keeps above the spawn record,
   * where Func_02001620 holds its velocity; without it, 4 of 41
   * instructions differ in tla-en. */
  struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct Actor *object = Actor_Get(id);

  spawn.kind = 0x115;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0xcccc;
  spawn.speed = 0x10000;
  spawn.acceleration = 0x10000;
  spawn.script = gData_02002310;
  Event_PlaySound(154);
  Func_020000b8(object->x, object->y, object->z, 0, 0, 0, 0x1c0000, &spawn);
}
