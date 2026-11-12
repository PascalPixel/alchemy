/// @file
/// Scene code of overlay 0x65f, in ROM order, from s0440's offset up to the next object the overlay links.

#include "garoh.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "scene_tables.h"

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;

extern u32 gData_020056dc[];
extern u32 gData_020056f4[];
extern s16 gData_02005874[];
extern u32 gData_020059dc[];
extern s32 gData_02005a54[];
extern u32 gData_02005b5c[];
extern s32 gData_02005c1c[];

struct GarohPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const struct GarohPicture gData_02005de0[];

void Event_FinishScreenTransition(void);

/// Return the table for the current room.
const void *Func_02000440(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65FRoom1) {
    if (Flag_IsSet(0x91b))
      return gData_02005874;
    return gData_020056f4;
  }
  if (scene == (s32)&SceneId_Ov65FRoom2)
    return gData_02005b5c;
  if (scene == (s32)&SceneId_Ov65FRoom3)
    return gData_020059dc;
  if (scene == (s32)&SceneId_Ov65FRoom4)
    return gData_02005a54;
  if (scene == (s32)&SceneId_Ov65FRoom5)
    return gData_02005c1c;
  return gData_020056dc;
}

static __inline__ void Func_0200517c_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): the call through this inline, so each call keeps its
   * own argument order; as a direct call to Map_PlayMapEdit, 12 of 72
   * instructions of Func_020004cc differ in tla-en. */
  Map_PlayMapEdit(data, a, b);
}

static __inline__ void OffsetPosition_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH(wrapper): the call through this inline, so the second argument is
   * set before the third's split constant is finished; as a direct call to
   * Actor_OffsetPositionAndResetMotion, 2 of 72 instructions of Func_020004cc
   * differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void CenterOnColumn_(s32 id, s32 column, s32 dz) {
  /* FAKEMATCH(wrapper): the call through this inline, for the same order as
   * OffsetPosition_; as a direct call to Actor_CenterOnColumn, 2 of 72
   * instructions of Func_020004cc differ in tla-en. */
  Actor_CenterOnColumn(id, column, dz);
}

/// 6A6's Func_02000e54 for this overlay: clears the motion of every object
/// from 8 to 63, shows the picture the event work's index names and moves the
/// party's current owner to it.
void Func_020004cc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  s32 i;

  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop: the bytes rebuild the zero on every pass, which a for
  // loop hoists (differ 12).
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  index = *(s16 *)(work + 0x154);
  Event_PlaySound(158);
  index--;
  Func_0200517c_(gData_02005de0[index].data, gData_02005de0[index].a,
                 gData_02005de0[index].b);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (index == 5)
      OffsetPosition_(owner, 0, -4);
    else
      CenterOnColumn_(owner, 2, -4);
  }
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020051b4();
}

extern u8 MsgGaroh02[];
extern u8 MsgGaroh04[];
extern u8 MsgGaroh06[];
extern u8 MsgGaroh17[];
extern u8 MsgGaroh18[];

void Func_02000580(s32 actor) {
  s32 msg = (s32)MsgGaroh02;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_020005cc(s32 actor) {
  s32 msg = (s32)MsgGaroh06;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000618(s32 actor) {
  s32 msg = (s32)MsgGaroh18;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000664(s32 actor) {
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaroh17);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}

void Func_02000698(s32 actor) {
  s32 flag;
  s32 msg;

  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  flag = Event_GetFlag(0x300);
  msg = (s32)MsgGaroh04;
  if (flag) {
    FieldEvent_SetMessageId(msg);
  } else {
    FieldEvent_SetMessageId(msg);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
  }
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Func_020051b4();
}

extern u8 MsgGaroh20[];
extern u8 MsgGaroh21[];

void Func_020006ec(void) {
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(13);
  Actor_SetAnimation(13, 1);
  FieldEvent_Wait(10);
  Actor_FaceActor(13, 4, 0);
  if (Event_GetFlag(0x301)) {
    FieldEvent_SetMessageId((s32)MsgGaroh21);
  } else {
    FieldEvent_SetMessageId((s32)MsgGaroh20);
    FieldEvent_ShowMessageThenWait(13, 0, 10);
    {
      s32 a = 0x101;
      s32 b = 15;

      FieldActor_ShowEmote(4, a, b);
    }
    {
      s32 a = 0x106;
      s32 b = 10;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
    {
      s32 a = 0x108;
      s32 b = 15;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
  }
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Event_SetFlag(0x301);
  Actor_SetScript(13, 2);
  Func_020051b4();
}

extern u8 MsgGaroh12[];

void Field_DispatchTypeHandler(s32 type);
void Func_020052b4(void);
void Func_02005304(s32 a, s32 b);
void Func_0200530c(s32 id, s32 a);
void Func_02005314(void);
void Func_0200531c(void);

void Func_02000808(void) {
  s32 flag;
  s32 speed;

  void Func_020007a0(s32 id) {
    Func_02005304(144, 1);
    Func_0200530c(id, 0);
    Field_DispatchTypeHandler(1);
    Func_02005314();
    Func_0200531c();
  }

  void Func_020007d4(s32 id) {
    Func_02005304(144, 1);
    Func_0200530c(id, 0);
    Field_DispatchTypeHandler(3);
    Func_02005314();
    Func_0200531c();
  }

  flag = Event_GetFlag(0x320);
  if (flag == 0) {
    Event_SetFlag(0x320);
    Func_020051ac();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSingleChildFlags(Actor_Get(22), 0);
    speed = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(360), PIXELS(199), speed);
    FieldCamera_MoveTo(PIXELS(359), -1, PIXELS(220), 1);
    Func_020052b4();
    Func_020007a0(22);
    FieldEvent_Wait(30);
    Actor_SetPositionAndFacing(21, PIXELS(360), PIXELS(199), speed);
    Actor_WalkByAndWait(21, 0, 16);
    Func_020007d4(22);
    FieldEvent_SetMessageId((s32)MsgGaroh12);
    Actor_FaceDirectionAndWait(21, 160 << 7);
    Actor_RunRepeatedMotion(21, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(4, 224 << 8, 0);
    FieldEvent_ShowMessageThenWait(21, 0, 10);
    FieldActor_ShowEmote(21, 0x102, 50);
    Actor_Get(21)->actionFlags &= ~1;
    Actor_FaceDirection(21, speed, 0);
    FieldEvent_Wait(30);
    Func_020007a0(22);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(21, 0, -16);
    Actor_Get(21)->motionFlags = flag;
    Actor_WalkByAndWait(21, 0, -16);
    Actor_SetPosition(21, 0, 0);
    FieldEvent_Wait(10);
    Func_020007d4(22);
    FieldEvent_Wait(60);
    Actor_SetPosition(22, 0, 0);
    Func_020051b4();
  }
}

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;

extern s32 gData_02005e10[];
extern s32 gData_02005e34[];
extern s32 gData_02005f78[];
extern s32 gData_020060d4[];
extern s32 gData_0200611c[];
extern s32 gData_020061d0[];
extern s32 gData_02006278[];

struct EventWork {
  u8 unk000[0x154];
  s16 unk154;
  u8 unk156[0x56];
  s32 unk1ac;
};

extern s32 gData_0200632c;

void Func_02002694(void);
void Func_02003878(void);

static inline void CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): the call through this inline, so each -1 is built on
   * its own; as a direct call to Camera_MoveWithinMapBounds, 6 of 30
   * instructions of Func_02000960 differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02000960(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  Event_PlaySound(123);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk1ac = 521;
  CamMove(-1, -1, -1, 0);
  Flag_Set(0x121);
  FieldEvent_SetExitRequest(event->unk154);
}

void Func_020009a4(void) {
  if (!Flag_IsSet(0x91b)) {
    if (!Flag_IsSet(0x91e))
      Func_02002694();
  } else {
    if (!Flag_IsSet(0x91f))
      Func_02003878();
  }
}

/// Return the table for the current room.
const void *Func_020009dc(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65FRoom1) {
    if (Flag_IsSet(0x91b))
      return gData_02005f78;
    return gData_02005e34;
  }
  if (scene == (s32)&SceneId_Ov65FRoom2)
    return gData_020060d4;
  if (scene == (s32)&SceneId_Ov65FRoom3)
    return gData_0200611c;
  if (scene == (s32)&SceneId_Ov65FRoom4)
    return gData_020061d0;
  if (scene == (s32)&SceneId_Ov65FRoom5)
    return gData_02006278;
  return gData_02005e10;
}

extern s32 gData_0200632c;

void Func_02000a68(void) {
  if (!Flag_IsSet(0x91b) && Flag_IsSet(0x310)) {
    gData_0200632c++;
    if ((gData_0200632c & 0x3ff) == 0)
      Event_PlaySound(0x20d);
  }
}
