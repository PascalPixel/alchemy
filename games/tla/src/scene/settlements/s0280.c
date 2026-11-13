/// @file
/// Scene code of overlay 0x690, in ROM order.

#include "../shared/routines.h"
#include "../shared/settlements.h"
#include "field_event.h"
#include "field.h"
#include "module/scene/settlements.h"
#include "palette_snapshots.h"

void Func_02000280(void) { Func_080d7788(10, 1, 14); }

void Func_02000290(void) { Func_080d7788(11, 2, 14); }

void Func_020002a0(void) { Func_080d7788(23, 3, 19); }

s32 Func_020002b0(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

DEFINE_FUNC_02000058(Func_020002bc)

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u32 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;
  u32 *placements;

  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_02002380;
  placements = 0;
  if (scene == (s32)&SceneId_Ov690Room3)
    placements = gData_020023a0;
  return placements;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"
#include "io_reg.h"
#include "version.h"

extern u8 gData_020024ac[];
extern u8 gData_020024dc[];
extern u8 gData_02002524[];
extern u8 gData_02002554[];
extern u8 gData_020025cc[];
extern u8 gData_02002614[];
extern u8 gData_020027ac[];
extern u8 gData_020027dc[];
extern u8 gData_02002854[];

u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gData_020024dc;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gData_02002524;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gData_02002554;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_020025cc;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gData_020027dc;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gData_02002854;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gData_02002614;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gData_020027ac;
  return gData_020024ac;
}

void Func_02000404(void) {
  s32 mode;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0xf5d);
  Func_02002130(193, 3);
  mode = 0;
  Party_GiveItem(193, mode);
  Event_End();
}

void Func_0200043c(s32 actor) {
  s32 msg = (s32)MsgSettlements01;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000488(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Func_020004a8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.currentOwner, 2, 0);
    SetPosCommit(gMainRoutineWork.currentOwner, 88, 296);
  }
  Func_020021a0(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(0xa60);
}

void Func_0200052c(void) {
  s32 next = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->actionFlags &= 0xfe;
  CopyCells(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  FieldActor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_Get(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  FieldActor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  FieldEvent_Wait(10);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(next);
  Event_End();
}

void Func_020005cc(void) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  id = 4;
  Actor_WalkByAndWait(id, 16, 0);
  Func_0200052c();
}

void Func_020005f4(void) {
  s32 id, x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the distances and their negation; as Actor_WalkByAndWait(4, -x,
   * -z), 2 of 18 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_WalkByAndWait(id, -x, -z);
  Func_0200052c();
}

void Func_02000624(void) {
  s32 id, a;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the angle byte and its shift; as Actor_FaceDirection(4, a << 8,
   * 0), 2 of 11 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_FaceDirection(id, a << 8, 0);
  Func_0200052c();
}

void Func_02000644(void) {
  CopyCells2(0, 3, 1, 1, 3, 15);
  Event_SetFlag(0x230);
}

extern u8 gData_0200289c[];
extern u8 gData_020028f0[];
extern u8 gData_02002974[];
extern u8 gData_020029a4[];
extern u8 gData_02002a70[];
extern u8 gData_02002ad0[];
extern u8 gData_02002b3c[];
extern u8 gData_02002b90[];
extern u8 gData_02002c2c[];

u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gData_020028f0;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gData_02002974;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gData_020029a4;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_02002a70;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gData_02002ad0;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gData_02002b3c;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gData_02002b90;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gData_02002c2c;
  return gData_0200289c;
}

void Func_02000714(struct Actor *object) {
  s32 step;

  object->x += object->timer << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->timer += 2;
  if (--*(s32 *)&object->watchedObject == 0)
    Func_02002000(object);
}

void Func_02000758(s32 x, s32 y, s32 z) {
  struct Actor *object = FieldActor_Create(0x11d, x, y, z);

  if (object != NULL) {
    s8 *animation;

    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= 0xfe;
    Actor_SetSpriteColor(object, 7);
    animation = object->sprite;
    animation[9] = (animation[9] & -13) | 4;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = (void (*)(void))Func_02000714;
    Func_02001fe0(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

void Func_020007d8(void) {
  if ((gFrameCount & 63) == 0)
    Func_02000758(0xf80000, 0x300000, 0x1000000);
}

/// Moves the object around its orbit's centre (words 14 and 15) and steps the
/// angle by a random amount.
// As ☀️'s Scene_UpdateFloatingNut does.
s32 Func_020007fc(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Math_Sin(p[12]) * 2;

  if (d > 0)
    d = -d;
  p[2] = p[14] + Math_Cosine(p[12]) * 2;
  p[3] = p[15] + d;
  q[9] = Math_Cosine(p[12] + 0x8000) / 8;
  a = Random16Far();
  b = Random16Far();
  {
    u32 x;
    s32 angle;

    x = (u32)a << 9;
    angle = p[12];
    p[12] = angle + ((x >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  }
  return 0;
}

void Func_02002180(s32 a, s32 b, s32 c, s32 d);

static __inline__ void Settlements_Func_02002180(s32 a, s32 b, s32 c, s32 d) {
  Func_02002180(a, b, c, d);
}

void Func_02000860(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  u8 *transfer;

  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    FieldTask_AddFrame(Func_020007d8, priority);
  }
  Func_02002170();
  Func_02002170();
  Settlements_Func_02002180(1, 0x203, 8, 9);
  if (Event_GetFlag(0xf5d)) {
    Actor_SetPosition(11, 0, 0);
    return;
  }
  actor = (struct OrbitObject *)Actor_Get(11);
  sprite = actor->sprite;
  sprite->flags09Mode = 1;
  sprite->flags05Bit5 = 0;
  sprite->flags09High = 0;
  sprite->state = 0;
  Actor_SetSingleChildFlags((struct Actor *)actor, 0);
  actor->active = 0;
  actor->mode = 0;
  if (Event_GetFlag(0x109) == 0)
    actor->y += 0x100000;
  actor->flags23 &= 0xfe;
  actor->visible = 1;
  transfer = HeapSlot_AllocFar(68, 0x608);
  Item_LoadIcon(193);
  transfer += 0x400;
  Vram_Load(sprite->palette, 128, transfer);
  HeapSlot_Release(68);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Func_020007fc;
  actor->state = 0;
}

extern const u8 gData_02002272[];

void Func_02000e04(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Place(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room7)
    Func_02000860();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    Func_02002140(0);
    Func_02002188(gData_0200226c);
    if (GameFlag_Test(0x206))
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_GetById(10)->priorityFlags |= 2;
    Actor_SetModeById(18, 2);
    Actor_SetModeById(19, 2);
    Actor_SetModeById(21, 2);
    if (GameFlag_Test(0x205))
      Actor_SetModeById(10, 2);
    Func_02000e04();
#if TLA_PATCH_1
    if (GameFlag_Test(0x204)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Place(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4) {
    if (GameFlag_Test(0x230)) {
      CopyCells2(0, 3, 1, 1, 3, 15);
      Actor_SetModeById(9, 4);
    }
    if (GameFlag_Test(0x109) && Flag_Test(0x220)) {
      GameFlag_ClearBit(0x220);
      Func_0200125c();
    }
    Func_02002188(gData_02002272);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room3)
    Func_02002140(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room6)
    Func_02002140(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    if (Flag_Test(0x205)) {
      CopyCells2(9, 11, 1, 1, 11, 12);
      Map_CopyCellPictureIndicesRect(12, 32, 11, 15, 1, 1);
      Map_CopyCellPictureIndicesRect(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (GameFlag_Test(0x205))
      Map_CopyCellAttributeRect(9, 11, 1, 1, 11, 11);
#else
    if (GameFlag_Test(0x204)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Place(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room1) {
    if (GameFlag_Test(0xa60)) {
      Map_CopyCellPictureIndicesRect(5, 46, 5, 47, 1, 1);
      Map_CopyCellPictureIndicesRect(4, 25, 5, 17, 1, 1);
      CopyCells2(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}

#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"
#include "map.h"

void Func_02000c1c(void) { GameFlag_SetBit(0x201); }

void Func_02000c2c(void) { GameFlag_ClearBit(0x201); }

void Func_02000c3c(void) { Func_02001770(); }

void Func_02000c44(void) {
  u32 i;

  Event_Start();
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
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
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
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

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

// The hblank split: above the line Func_02000ed0 sets, BG1 scrolls by one value
// and blending is off; below it, by the other, with BG1 blended.
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

DEFINE_FUNC_02000E90(Func_02000e90, gData_02002ce0, gData_02002ce4,
    gData_02002ce6)

DEFINE_FUNC_02000ED0(Func_02000ed0, Func_02000e30, Func_02000e90)

void Func_02000ef0(void) { Func_02002190(gData_0200226c); }

void Func_02000f00(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 2;
  Func_02002190(gData_0200226c);
}

void Func_0200156c(s32 id);

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
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldActor_WaitForMoveById(10);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
    Actor_SetSpeed(10, 0x26666, 0x13333);
    Actor_SetDestinationOffset(10, 0, 80);
    FieldActor_WaitForMoveById(10);
    Func_0200156c(10);
    if (!Event_GetFlag(0x204)) {
      FieldEvent_Wait(10);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldActor_WaitForMoveById(10);
      Actor_SetSpeed(10, 0x28000, 0x14000);
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
    Event_End();
  } else if (!Flag_Test(0x207) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Flag_Set(0x207);
    Event_End();
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

  Event_Start();
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
  Event_End();
}

void Func_0200155c(void) {
  Func_02002198();
  Func_0200122c();
  Func_0200138c();
}

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
