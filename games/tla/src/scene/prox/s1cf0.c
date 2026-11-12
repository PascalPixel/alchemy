/// @file
/// Scene code of overlay 0x6a8, in ROM order, from s1cf0's offset up to the next object the overlay links.

#include "local.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "io_reg.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
void Func_0200457c(void);
s32 Func_0200446c();
extern u8 MsgProx80[];

void Func_02001cf0(void) {
  s32 t83;
  s32 t144;
  s32 t159;
  s32 t202;
  s32 t207;
  s32 angleC000;
  s32 angleB000;
  s32 angle3000;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_Stop(0x14);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(184), PIXELS(484));
  /* FAKEMATCH(temporary): the three facings held in locals set before their
   * first placement, where as constants each placement rebuilds them and the
   * later turns cannot share them; as the constants, 76 of 936 instructions
   * differ in tla-en. */
  angleC000 = 0xc000;
  angleB000 = 0xb000;
  Actor_SetPositionAndFacing(0, PIXELS(168), PIXELS(484), angleC000);
  Actor_SetPositionAndFacing(27, PIXELS(200), PIXELS(484), angleB000);
  angle3000 = 0x3000;
  Actor_SetPositionAndFacing(17, PIXELS(138), PIXELS(412), angle3000);
  Actor_SetPositionAndFacing(20, PIXELS(220), PIXELS(600), angleB000);
  Actor_SetPosition(5, PIXELS(184), PIXELS(684));
  System_Wait(1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(448), 1);
  Func_0200457c();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgProx80);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(26, 0x5000, 0);
  Prox_Jump(26, 0x108, 20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(0, 0, 20);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x108, 0);
  Prox_Jump(0, 0x108, 40);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(19, 4);
  Event_ShowMessage(0x4013, 0);
  Prox_Jump(0, 0x109, 20);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(27, FACING_WEST, 20);
  Prox_Jump(27, 0x108, 0);
  Event_ShowMessage(27, 0);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Prox_Jump(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirectionAndWait(26, 0x3000);
  Actor_SetAnimation(26, 4);
  Event_ShowMessage(26, 0);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(0x13, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x4013, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(26, 0x5000, 0);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_ShowEmote(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_ShowEmote(25, 0x103, 20);
  Event_ShowMessage(25, 0);
  Actor_ShowEmote(27, 0x100, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(26, 0x3000, 0);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_ShowEmote(25, 0x105, 20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(26, 4);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0x14);
  Actor_FaceDirectionAndWait(0, 0);
  Actor_ShowEmote(0, 0x108, 0);
  Event_OpenMessage(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t83 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t83 == 0) {
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessage(26, 0);
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId++;
    Actor_SetAnimationAndWait(26, 4);
    Event_ShowMessage(26, 0);
  }
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Prox_Jump(25, 0x108, 20);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(27, 0xb000, 20);
  Actor_SetAnimationAndWait(27, 3);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirectionAndWait(0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(0x4000, 0);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirectionAndWait(25, FACING_WEST);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Event_ShowMessage(25, 0);
  Prox_Jump(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x101, 0);
  Prox_Jump(0, 0x101, 0);
  Prox_Jump(26, 0x101, 0);
  Actor_ShowEmote(25, 0x101, 40);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(26, 3);
  Actor_SetAnimation(25, 3);
  Actor_FaceDirection(27, FACING_WEST, 0);
  Actor_ShowEmote(27, 0x108, 20);
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(26, 0x109, 20);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0x14);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(0x1b, 0x102);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(27, 0x5000);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirectionAndWait(0, 0);
  Prox_MessageOpen(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t144 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t144 == 0) {
    FieldCamera_SetSpeed(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(512), 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(0x4000, 0);
    Event_ShowMessage(0x2005, 0);
    EVENT_WORK->messageId += 2;
  } else {
    EVENT_WORK->messageId += 2;
    FieldCamera_SetSpeed(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(512), 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(0x4000, 0);
    Event_ShowMessage(0x2005, 0);
  }
  t159 = ((s32)FieldCamera_GetActor());
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  FieldCamera_MoveTo(PIXELS(158), -1, PIXELS(432), 1);
  Func_0200457c();
  FieldActor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(172), -1, PIXELS(520), 1);
  FieldActor_WatchAndSetScript(0, 0x11);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, 0x11);
  FieldActor_WatchAndSetScript(0x1b, 0x11);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_WalkToAndWait(17, 140, 478);
  Actor_WalkToAndWait(17, 150, 540);
  Actor_WalkToAndWait(17, 160, 560);
  Actor_WalkToAndWait(17, 190, 640);
  FieldActor_Stop(0);
  FieldActor_Stop(gMainRoutineWork.currentOwner);
  FieldActor_Stop(0x1b);
  System_Wait(1);
  Actor_SetPosition(17, 0, 0);
  FieldEvent_Wait(10);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(448), 1);
  Actor_WalkToAndWait(0, 168, 484);
  Actor_FaceDirectionAndWait(0, 0);
  Event_ShowMessage(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_SetAnimationAndWait(25, 3);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(27, 3);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_SetSpeed(27, 0x19999, 0xcccc);
  Actor_SetAnimation(0, 2);
  t202 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t202 != 0) {
    Actor_SetDestination(0, FIELD_AT(t202, s16, 10),
                              FIELD_AT(t202, s16, 18));
  }
  FieldActor_WaitForMoveById(0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetAnimation(27, 2);
  t207 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t207 != 0) {
    Actor_SetDestination(27, FIELD_AT(t207, s16, 10),
                              FIELD_AT(t207, s16, 18));
  }
  FieldActor_WaitForMoveById(27);
  Actor_SetPosition(27, 0, 0);
  {
    struct EventWork *work = EVENT_WORK;

    work->screenEffect = 0x209;
    work->delay = 0x10;
  }
  Event_ClearFlag(0x12f);
  Event_SetFlag(0x9c5);
  Func_0200446c();
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Actor_SetWideSprite(s32 id, s32 wide);
void Func_020018e0(void);
void Func_020027e4(void);
void Func_02002878(void);
void Func_020029c8(void);
void Func_02002a38(void);
void Func_0200440c(s32 layer);
void Func_02004414(s32 layer);
void Func_020045dc(s32 a);
void Func_0200461c(s32 a);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

s32 Func_02002614(void) {
  s32 scene;

  if (Flag_Test(0xa3d)) {
    GameFlag_SetBit(0x17c);
    GameFlag_SetBit(0x144);
  }
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov6a8Room4) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_020029c8();
  } else if (scene == (s32)&SceneId_Ov6a8Room5) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_02002a38();
  } else if (scene == (s32)&SceneId_Ov6a8Room6) {
    s32 *effect =
        &((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect;
    /* FAKEMATCH(pin): the call's argument pinned to the r0 it is passed in, so
     * it loads before the store as the native does; in the plain order the
     * store comes first; as ...->screenEffect = 0x209 and Func_020045dc(0), as
     * the other rooms write it, 2 of 147 instructions differ in tla-en. */
    register s32 skip __asm__("r0");
    s32 value = 0x209;

    skip = 0;
    *effect = value;
    Func_020045dc(skip);
  } else {
    if (!GameFlag_Test(0xa3d))
      GameFlag_SetBit(0x201);
    if (GameFlag_Test(0x201)) {
      Func_0200461c(1);
    } else {
      REG_BLDALPHA = 0xc08;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      Task_Add(Func_020018e0, 0x480);
      Func_02004414(0);
      Func_02004414(1);
      Func_0200440c(2);
      Func_0200440c(3);
      Func_0200440c(4);
      Func_0200440c(5);
      Func_0200440c(6);
    }
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov6a8Room2) {
      if (!GameFlag_Test(0x300))
        Actor_SetWideSprite(64, 1);
      Func_020027e4();
    } else if (scene == (s32)&SceneId_Ov6a8Room3) {
      Func_02002878();
    }
    if (GameFlag_Test(0x201)) {
      BattleEffect_ApplyColorToTargetBuffer(0x10004, 1);
      BattleEffect_StartBufferInterpolation(16);
      wait(16);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct Walker {
  u8 unk00[0x63];
  u8 arrived;
  u16 frames;
  u8 unk66[6];
  void (*update)(void);
};

void Func_020003bc(void);

void Func_0200279c(s32 actor, s32 x, s32 z, s32 frames) {
  struct Walker *object = ((struct Walker *)Actor_Get(actor));

  object->frames = frames;
  object->arrived = 0;
  object->update = Func_020003bc;
  Actor_SetPosition(actor, x << 16, z << 16);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200279c(s32 a, s32 b, s32 c, s32 d);
extern u8 gData_020047ac[];
extern u8 gData_02004848[];
extern u8 gData_020048e4[];

void Func_020027e4(void) {
  s32 t0;
  s32 t1;

  t0 = Event_GetFlag(0xa3d);
  if (t0 != 0) {
    t1 = Event_GetFlag(0x109);
    if (t1 == 0) {
      Func_0200279c(0x10, 0x218, 0x168, 0);
      Func_0200279c(0x11, 0x218, 0x1a8, 1);
      Func_0200279c(0x12, 0x244, 0x1a4, 2);
      System_Wait(1);
      Actor_SetScript(16, (s32)gData_020047ac);
      Actor_SetScript(17, (s32)gData_02004848);
      Actor_SetScript(18, (s32)gData_020048e4);
      System_Wait(1);
    }
  }
  FieldActor_SetSpritePriority(20, 1);
  Actor_SetSpriteColor(((s32)Actor_Get(20)), 0xf);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02002878(void) {
  s32 t9;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t11;
  s32 t16;
  s32 t20;

  FieldActor_SetSpritePriority(19, 1);
  Actor_SetSpriteColor(((s32)Actor_Get(19)), 0xf);
  t3 = Event_GetFlag(0xa3d);
  if (t3 == 0) {
    t4 = Event_GetFlag(0x9c0);
    if (t4 == 0) {
      Actor_SetPosition(8, 0, 0);
    }
    t6 = Event_GetFlag(0x200);
    if (t6 != 0) {
      MapGrid_CopyCellsTo(0x59, 0, 0x4d, 0x10, 1, 2);
    }
    t8 = Event_GetFlag(0x9c1);
    if (t8 != 0) {
      t9 = ((s32)Actor_Get(10));
      FIELD_AT(t9, u16, 6) = 0xb000;
      System_Wait(1);
    }
  }
  t11 = Event_GetFlag(0xa3d);
  if (t11 != 0) {
    MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
    MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
    MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
    Actor_SetPosition(20, 0, 0);
  } else {
    t16 = Event_GetFlag(0xa2d);
    if (t16 != 0) {
      MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
      MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
      MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
      t20 = Event_GetFlag(0x40);
      if (t20 == 0) {
        Actor_SetPosition(20, 0, 0);
      }
    }
  }
}
