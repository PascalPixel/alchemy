/// @file
/// Scene code of overlay 0x679, in ROM order, from events_d's offset up to the next object the overlay links.

#include "scene_tables.h"
#include "local.h"
#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "map.h"
#include "event_work.h"
#include "task.h"
#include "event_inline.h"

void Func_02000974(void) {
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(17)->update = (void (*)(void))Func_02000770;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(16, FACING_EAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_WEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  FieldActor_ShowEmote(16, 0x102, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(16, 2, 25);
  Event_ShowMessage(16, 0);
  Actor_RunRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Func_02001ccc();
  Event_ClearFlag(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

void Func_02000a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (Event_GetFlag(0xf9b) == 0) {
    Func_02001d04_(65, PIXELS(376), PIXELS(376));
    if (leader->x >> 20 == 23 && leader->z >> 20 == 23) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000af8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (Event_GetFlag(0xf9a) == 0) {
    Func_02001d04_(64, PIXELS(600), PIXELS(408));
    if (leader->x >> 20 == 37 && leader->z >> 20 == 25) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

void Func_02000ba8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(67, 38, 11, 6, 67, 8, 1);
  if (Event_GetFlag(0xf9e) == 0) {
    Func_02001d04_(64, PIXELS(200), PIXELS(152));
    if (leader->x >> 20 == 12 && leader->z >> 20 == 9) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000c38(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Event_PlaySound(123);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn_(gMainRoutineWork.currentOwner, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001ccc();
}

s32 Func_02000cb4(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    Event_ShowMessage(9, 0);
    Actor_ArmCallback_(9, FACING_SOUTHEAST, 0);
  }
  Func_02001ccc();
  return 0;
}

s32 Func_02000d20(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    Event_ShowMessage(9, 0);
  }
  Func_02001ccc();
  return 0;
}

void Func_02000d74(void) { Func_02001d2c(gMainRoutineWork.currentOwner, 1, 5); }

extern const u8 gData_020024b8[];
extern const u8 gData_020024d0[];
extern const u8 gData_02002698[];
extern const u8 gData_0200280c[];

const u8 *Func_02000d90(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov679Room1)
    return gData_020024d0;
  if (scene == (s32)&SceneId_Ov679Room3)
    return gData_02002698;
  if (scene == (s32)&SceneId_Ov679Room2)
    return gData_0200280c;
  return gData_020024b8;
}

void Func_02000de0(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e24(8, actor);
    return;
  }
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  Event_ShowMessage(actor, 0);
  Func_02001ccc();
}

void Func_02000e40(s32 actor) {
  s32 msg;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
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
  Func_02001ccc();
}

void Func_02000e98(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e1c(23, actor);
    return;
  }
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  Event_ShowMessage(actor, 0);
  Func_02001ccc();
}

void Func_02000ef8(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002a98) {
    value = gData_02002a9c;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002a9e;
    /* FAKEMATCH(dead): the once-only block orders this statement; as a plain
     * statement, 9 of 35 instructions differ in tla-en. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

void Func_02000f58(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  s16 y = layer->y >> 16;

  gData_02002a98 = 1448 - y;
  gData_02002a9c = layer->x >> 16;
  gData_02002a9e = (s16)(layer->x >> 16) - (gFrameCount >> 2);
}

void Func_02000f98(void) {
  s32 priority;

  Func_02001c0c(1, 0, Func_02000ef8);
  priority = 0xc80;
  FieldTask_AddFrame(Func_02000f58, priority);
}

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
  u8 high : 4;
};

/// The overlay's entry: sets the map mode and screen effect 0x202, then starts
/// each room's own setup.
s32 Func_02000fb8(void) {
  s32 *effect;
  s16 scene;
  struct MainRoutineWork *party;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  *effect = 0x202;
  party = &gMainRoutineWork;
  scene = party->scene;
  if (scene == (s32)&SceneId_Ov679Room2) {
    Func_02001054();
  } else if (scene == (s32)&SceneId_Ov679Room1) {
    FieldTask_AddFrame(Func_02000540, 0x480);
    if (Event_GetFlag(0x95e))
      Func_02000974();
  } else if (scene == (s32)&SceneId_Ov679Room3) {
    *effect = 0x100;
    if (party->entrance == 6)
      Func_020010b4();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001054(void) {
  Task_AddFrame(Func_020003e8, TASK_PASS_VBLANK);
  Func_02001dfc();
  Func_02001e04(0, 0x200, 8, 9);
  Func_02001dec(170);
  Func_02000f98();
  if (gMainRoutineWork.entrance == 3)
    Func_02001500();
  if (gMainRoutineWork.entrance == 4)
    Func_02001500();
  if (gMainRoutineWork.entrance == 5)
    Func_02001500();
}

void Func_020010b4(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_ClearFlag(0x12f);
}

void Func_020010d0(void) { Func_02001d9c(); }
