#include "local.h"
#include "scene_ids.h"

void Func_02000188(void) {
  s32 msg;

  Func_02000648();
  Func_020006d0(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Func_02000698(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020006e8(), 0) == 0) {
    FieldEvent_WaitFrames(10);
    Func_02000698(msg + 1);
  } else {
    FieldEvent_WaitFrames(20);
    Func_02000698(msg + 2);
  }
  Func_020006a8(9, 0);
  Func_02000650();
}

void Func_020001e0(void) {
  Func_02000618(5, 49, 5, 41, 1, 1);
  Func_02000630_(4, 48, 3, 2, 4, 40);
  GameFlag_SetBit(0x85b);
}

void Func_02000218(void) {
  if (GameFlag_Test_(0x200) == 0) {
    Func_02000620(0);
    Func_02000620(1);
    Func_02000620(2);
    Func_02000618(56, 46, 30, 46, 16, 16);
    Func_02000618(56, 174, 30, 174, 16, 16);
    Func_02000628_(28, 23, 16, 16, 15, 23);
    Func_02000628_(28, 90, 2, 2, 21, 88);
    Func_02000628(28, 90, 4, 4, 17, 89);
    Func_02000628(28, 90, 2, 1, 17, 91);
    Func_02000628(28, 90, 2, 2, 19, 91);
    Func_02000628(28, 92, 2, 2, 15, 92);
    Func_02000628(30, 92, 2, 2, 17, 92);
    GameFlag_SetBit_(0x200);
  }
}

s32 Func_020002f8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Func_0200030c(void) {
  struct Adventure *party;
  struct ObjectRuntime *object;
  u8 flags;

  object = Object_GetById(16);
  flags = object->priorityFlags & ~1;
  party = &gAdventure;
  /* FAKEMATCH: the empty asm holds the store below gAdventure's address load,
   * which sched2 otherwise puts above the ands (differ 2 with the plain
   * statement). */
  asm("");
  object->priorityFlags = flags;
  /* FAKEMATCH: the loop note keeps currentOwner's offset after the store;
   * without it the offset's movs and lsls move above the strb (differ 2). */
  do {
  } while (0);
  Object_GetById(party->currentOwner)->priorityFlags |= 0x20;
  if (party->scene == (s32)&SceneId_Ov65dRoom2) {
    if (GameFlag_Test(0x200)) {
      Func_02000620(0);
      Func_02000620(1);
      Func_02000620(2);
      Func_02000618(56, 46, 30, 46, 16, 16);
      Func_02000618(56, 174, 30, 174, 16, 16);
      Func_02000628_(28, 23, 16, 16, 15, 23);
      Func_02000628_(28, 90, 2, 2, 21, 88);
      Func_02000628(28, 90, 4, 4, 17, 89);
      Func_02000628_(28, 90, 2, 1, 17, 91);
      Func_02000628(28, 90, 2, 2, 19, 91);
      Func_02000628_(28, 92, 2, 2, 15, 92);
      Func_02000628_(30, 92, 2, 2, 17, 92);
    }
  } else if (GameFlag_Test(0x85b)) {
    Func_020001e0();
  }
  return 0;
}
