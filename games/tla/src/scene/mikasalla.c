/// @file
/// The scene code of overlay 0x65d, in ROM order.

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
#include "main_routine_work.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gData_02000794[];

s16 *Func_02000040(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_02000794;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s32 gData_02000fa0[];
extern u32 gData_020010c0[];

void *Func_02000070(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom1)
    return gData_02000fa0;
  return gData_020010c0;
}

#include "mikasalla/local.h"
#include "event_inline.h"

struct WarpEntry {
  s32 map;
  u16 x;
  u16 z;
};

extern struct WarpEntry gData_0200073c[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200009c(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 id;
  s16 *choice;
  s32 index;

  Func_02000648();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  choice = (s16 *)(work + 340);
  index = *choice;
  Sound_Play(158);
  index--;
  MapGrid_AnimateCells(gData_0200073c[index].map, gData_0200073c[index].x,
                 gData_0200073c[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  if (index != 4) {
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000650();
}

void Func_0200014c(void) { Func_080d7788(18, 2, 8); }

extern s32 gData_020012ac[];
extern s32 gData_020010d8[];

s32 *Func_0200015c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_020012ac;
  return gData_020010d8;
}

void Func_02000188(void) {
  s32 msg;

  Func_02000648();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Func_02000650();
}

void Func_020001e0(void) {
  Map_CopyCellPictureIndicesRect(5, 49, 5, 41, 1, 1);
  MapGrid_CopyCellAttributes(4, 48, 3, 2, 4, 40);
  GameFlag_SetBit(0x85b);
}

void Func_02000218(void) {
  if (Event_GetFlag(0x200) == 0) {
    Func_02000620(0);
    Func_02000620(1);
    Func_02000620(2);
    Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
    Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
    MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
    MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
    Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
    Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
    Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
    Map_CopyCellsRect(28, 92, 2, 2, 15, 92);
    Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    GameFlag_SetBit(0x200);
  }
}

s32 Func_020002f8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Func_0200030c(void) {
  struct MainRoutineWork *party;
  struct Actor *object;
  u8 flags;

  object = Actor_GetById(16);
  flags = object->priorityFlags & ~1;
  // Once-only: the once-only block holds the store below gMainRoutineWork's
  // address load, which sched2 otherwise puts above the ands (differ 2 with the
  // plain statement).
  do {
    party = &gMainRoutineWork;
  } while (0);
  object->priorityFlags = flags;
  Actor_GetById(party->currentOwner)->priorityFlags |= 0x20;
  if (party->scene == (s32)&SceneId_Ov65dRoom2) {
    if (GameFlag_Test(0x200)) {
      Func_02000620(0);
      Func_02000620(1);
      Func_02000620(2);
      Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
      Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
      Map_CopyCellsRect(28, 23, 16, 16, 15, 23);
      MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
      Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
      Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
      Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
      MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
      Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    }
  } else if (GameFlag_Test(0x85b)) {
    Func_020001e0();
  }
  return 0;
}

void Func_02000438(void) {
  s32 sound;
  s32 drop;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65dRoom1) {
    sound = 12;
    drop = 17;
  } else {
    sound = 3;
    drop = 9;
  }
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, drop);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(sound);
  Func_02000650();
}

void Func_020004ec(void) {
  s32 id;

  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_02000438();
}

void Func_02000514(void) {
  s32 id, x, z;

  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * distances and their negation; as no asm, 2 of 18 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_CommitPositionAndActivate(id, -x, -z);
  Func_02000438();
}

void Func_02000544(void) {
  s32 id, a;

  Func_02000648();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * angle byte and its shift; as no asm, 2 of 11 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Func_02000438();
}

void Func_02000564(void) {
  s32 x = ((s16 *)&Actor_GetById(4)->x)[1];

  Func_02000648();
  Event_ResolvePendingActions(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(4, -4, -4);
    Actor_WalkByAndWait(4, -4, -8);
  } else {
    Actor_WalkByAndWait(4, 4, -4);
    Actor_WalkByAndWait(4, 4, -8);
  }
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}

void Func_020005d4(void) {
  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -4);
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}
