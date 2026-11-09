#include "local.h"
#include "event_inline.h"
#include "scene_ids.h"

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
  party = &gMainRoutineWork;
  /* FAKEMATCH: the empty asm holds the store below gMainRoutineWork's address load,
   * which sched2 otherwise puts above the ands (differ 2 with the plain
   * statement). */
  asm("");
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
