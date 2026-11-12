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

void Func_02004464(void);
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
void Func_02004414(u32 layer);
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

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020029a4(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FieldActor_SetSpritePriority(
      16, ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020029c8(void) {
  s32 t2;

  Func_020045dc(0);
  Task_AddFrame((s32)Func_020029a4, 0x480);
  if (gMainRoutineWork.entrance == 0x14) {
    t2 = Event_GetFlag(0x9c5);
    if (t2 == 0) {
      Func_02001cf0();
    }
  }
}

#include "field_event.h"
#include "field_sprite.h"
#include "input.h"

void Func_02002a08(void) {
  s32 variant =
      ((struct FieldActor *)Actor_Get(gMainRoutineWork.currentOwner))
          ->sprite->priority;

  FieldActor_SetSpritePriority(8, variant);
  FieldActor_SetSpritePriority(10, variant);
}

// Unless flag 0x109 is set, calls Func_02001c24 with 0 for entrance 6 and with
// 1 for entrances 7 and 8, then schedules Func_02002a08. The entrance is read
// unsigned first and tested signed against 6, as the native reads it twice.

void Func_02001c24(s32 value);

void Func_02002a38(void) {
  s32 priority;

  if (Event_GetFlag(0x109) == 0) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if (gMainRoutineWork.entrance == 6)
      Func_02001c24(0);
    else if ((u32)((entrance - 7) << 16) <= 0x10000)
      Func_02001c24(1);
  }
  Func_020045dc(0);
  // The priority in a local is built before the task function's address (passed
  // directly, the address loads first).
  priority = 0x480;
  Task_AddFrame(Func_02002a08, priority);
}

s32 Func_020044dc();
extern u8 gData_02005ee8[];
extern u8 gData_02005f2c[];
extern u8 gData_02005f70[];
extern u8 gData_02005fac[];
extern u8 gData_02005fe8[];
extern u8 gData_02006038[];
extern u8 gData_02006088[];
extern u8 gData_020060cc[];
extern u8 gData_020060fc[];
extern u8 gData_0200617c[];
extern u8 gData_020061fc[];
extern u8 gData_0200622c[];
extern u8 gData_02006268[];
extern u8 MsgProx153[];

void Func_02002a8c(void) {
  u32 i;

  if (Event_GetFlag(0x9c5) != 0) {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(78);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    FieldEvent_SetMessageId((s32)MsgProx153);
    Event_ShowMessage(5, 0);
    FieldCamera_SetSpeed(0x33333, 0x6666);
    FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(228), 1);
    Func_0200457c();
    Prox_Jump(10, 0x108, 0);
    Event_ShowMessage(0x200a, 0);
    Event_PlaySound(37);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 136, 196);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Func_020044dc(0, gMainRoutineWork.currentOwner);
    Func_020044dc(0xb, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetScript(0, (s32)gData_02005ee8);
    Actor_RunScriptAndWait(11, (s32)gData_02005f2c);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTHWEST);
    Actor_ShowEmote(5, 0x103, 20);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(6, FACING_NORTHEAST, 0);
    Prox_Jump(6, 0x108, 20);
    Event_ShowMessage(6, 0);
    Actor_FaceDirectionAndWait(7, FACING_WEST);
    Actor_SetAnimationAndWait(7, 3);
    Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
    Event_ShowMessage(7, 0);
    Actor_ShowEmote(5, 0x107, 20);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(3, 4);
    Event_ShowMessage(3, 0);
    Actor_ShowEmote(10, 0x108, 0);
    Actor_FaceDirectionAndWait(10, 0xd000);
    Event_ShowMessage(10, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTH);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(5, 0);
    Prox_Jump(1, 0x109, 0);
    Actor_FaceDirectionAndWait(1, FACING_NORTHWEST);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(2, 0x105, 40);
    Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
    Event_ShowMessage(2, 0);
    Actor_ShowEmote(0, 0x109, 40);
    Actor_FaceDirectionAndWait(0, 0);
    FieldEvent_ShowMessageThenWait(0x8000, 0, 0xa);
    Actor_FaceDirection(8, 0x3000, 20);
    FieldEvent_ShowMessageThenWait(0x4008, 0, 0x14);
    if (Event_GetFlag(0x824) != 0) {
      Actor_FaceDirectionAndWait(0, FACING_SOUTH);
      Actor_SetAnimation(0, 4);
      Event_ShowMessage(0, 0);
      Actor_FaceDirectionAndWait(8, 0xd000);
      Event_ShowMessage(8, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId += 2;
      Actor_FaceDirectionAndWait(8, 0xd000);
      Event_ShowMessage(8, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    Actor_FaceDirection(11, FACING_WEST, 20);
    Actor_FaceDirectionAndWait(11, 0x5000);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirectionAndWait(10, FACING_WEST);
    Event_ShowMessage(0x600a, 0);
    Actor_FaceDirectionAndWait(9, 0);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(0x5009, 0);
    Actor_FaceDirectionAndWait(8, 0);
    Event_ShowMessage(0x5008, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirectionAndWait(0, 0);
    Actor_SetAnimation(0, 3);
    Prox_MessageOpen(0x8000, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(0, 3);
      Event_ShowMessage(0x8000, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(0, 4);
      Event_ShowMessage(0x8000, 0);
    }
    Actor_FaceDirectionAndWait(5, FACING_NORTHWEST);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(0x8005, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(6, 3);
    Actor_SetAnimation(7, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_SetSpeed(7, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    FieldActor_SetSpritePriority(0, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    FieldActor_SetSpritePriority(11, 2);
    FieldActor_SetSpritePriority(5, 2);
    FieldActor_SetSpritePriority(6, 2);
    FieldActor_SetSpritePriority(7, 2);
    FieldActor_SetSpritePriority(8, 2);
    FieldActor_SetSpritePriority(9, 2);
    FieldActor_SetSpritePriority(10, 2);
    FieldActor_SetSpritePriority(3, 2);
    FieldActor_SetSpritePriority(2, 2);
    FieldActor_SetSpritePriority(1, 2);
    FieldCamera_SetSpeed(0x6666, 0xccc);
    FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(246), 1);
    Actor_SetScript(9, (s32)gData_02005f70);
    Actor_SetScript(10, (s32)gData_02005fac);
    FieldEvent_Wait(10);
    Actor_SetScript(8, (s32)gData_02005fe8);
    Actor_SetScript(3, (s32)gData_02006038);
    FieldEvent_Wait(10);
    Actor_SetScript(6, gData_02005fe8);
    Actor_SetScript(7, (s32)gData_02005fe8);
    FieldEvent_Wait(10);
    Actor_SetScript(2, gData_02006038);
    Actor_SetScript(1, (s32)gData_02006088);
    FieldEvent_Wait(30);
    Actor_SetScript(5, (s32)gData_020060cc);
    FieldEvent_Wait(20);
    Actor_SetScript(0, (s32)gData_020060fc);
    Actor_SetScript(gMainRoutineWork.currentOwner,
                              (s32)gData_0200617c);
    FieldEvent_Wait(10);
    Actor_RunScriptAndWait(11, (s32)gData_020061fc);
    Actor_ShowEmote(11, 0x101, 20);
    Actor_FaceDirection(11, 0xb000, 0);
    FieldActor_RefreshSelectorById(5);
    Actor_FaceDirection(5, FACING_WEST, 0);
    Actor_FaceDirection(1, 0, 40);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_ShowEmote(11, 0x108, 40);
    Prox_MessageOpen(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
    }
    Actor_FaceDirection(0, FACING_NORTH, 20);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(0xa00b, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    Prox_Jump(1, 0x103, 0);
    FieldEvent_ShowMessageThenWait(1, 0, 0x14);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_ShowEmote(11, 0x103, 0);
    Prox_MessageOpen(0xa00b, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
    }
    Prox_Jump(1, 0x101, 20);
    Event_ShowMessage(1, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(11, 0);
    Actor_ShowEmote(5, 0x101, 0);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(11, 0x103, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(11, 0);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAnimationAndWait(1, 4);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(5, FACING_WEST);
    Actor_SetAnimation(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirectionAndWait(1, 0);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(1, 3);
    FieldEvent_Wait(10);
    Actor_SetSpeed(5, 0x19999, 0xcccc);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_SetScript(5, (s32)gData_0200622c);
    Actor_RunScriptAndWait(1, (s32)gData_02006268);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_WalkToAndWait(11, 156, 256);
    Actor_ShowEmote(11, 0x100, 0);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(0, 4);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0, 0);
    Actor_ShowEmote(11, 0x109, 40);
    Prox_MessageOpen(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Actor_SetAnimation(11, 4);
    FieldEvent_ShowMessageThenWait(0xa00b, 0, 0x14);
    Event_ShowMessage(0, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(0xa00b, 0);
    Prox_Jump(0, 0x101, 0);
    Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 40);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    FieldActor_Jump(11, 2, 10);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_ShowEmote(11, 0x108, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 0x21);
    Actor_SetAnimation(0, 0x28);
    Func_02004414(2);
    Func_02004414(3);
    Func_02004414(4);
    Func_02004414(5);
    Func_02004414(6);
    Event_PlaySound(78);
    FieldEvent_Wait(40);
    PaletteFade_ApplyTarget(0x10009, 0);
    PaletteFade_Interpolate(0x78);
    System_Wait(120);
    // Waits up to 120 frames, or until a key is held.
    i = 0;
    goto test_wait;
  next_wait:
    i++;
  test_wait:
    if (i < 120) {
      System_Wait(1);
      if (gInput.held == 0)
        goto next_wait;
    }
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(0x10);
    System_Wait(16);
    Event_SetFlag(0x9cc);
    FieldEvent_SetExitRequest(3);
  }
}

#include "actor.h"

void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Func_020043c4(struct Actor *object, s32 mode);
extern const s16 gData_02004740[];

void Func_0200385c(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_02004740[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Func_020043c4(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_020043c4(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004594(void);
void Func_020045e4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void FlagSet(s32 flag) { GameFlag_SetBit(flag); }

void Func_020038e8(void) {
  void *work = Ram_HeapSlots->eventWork;

  if (FIELD_AT(work, s16, 0xca2) != 0) {
    FlagSet(0x106);
    Func_02004594();
    Func_020045e4();
    GameFlag_ClearBit(0x106);
  }
}
