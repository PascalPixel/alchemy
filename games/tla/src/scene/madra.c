/// @file
/// The scene code of overlay 0x658 that its scene tables belong to, in ROM order; the rest of the overlay is in madra/.

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
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(0x8ff))
    return gData_02002ff0;
  return gData_02002d38;
}

struct MadraEventWork {
  u8 unk000[0x154];
  s16 value154;
};

struct MadraPlacement {
  const void *data;
  u16 x;
  u16 y;
};

extern struct MadraPlacement gData_02003308[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002b28(void);
void Func_02002b30(void);

void Func_0200006c(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object != NULL) {
    object->motionFlags = 0;
  }
  id++;
  if (id < 64)
    goto next;
  trigger = &work->value154;
  index = *trigger - 1;
  Sound_Play(158);
  {
    s32 x = gData_02003308[index].x;
    s32 y = gData_02003308[index].y;

    Map_PlayMapEdit(gData_02003308[index].data, x, y);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002b30();
}

void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe;
extern u8 MsgTheArmorerAnd;
extern u8 MsgAfterTheChampa;
extern u8 MsgPeopleAreSaying;
extern u8 MsgIKnewThere;
extern u8 MsgOwOwOw;
extern u8 MsgMyBoyfriendShin;
extern u8 MsgAllMyInjuries;
extern u8 MsgMySisterWas;
extern u8 MsgIfYouDon;
extern u8 MsgHaveYouBy[];
extern u8 MsgUsingPsynergyOn;
extern u8 MsgWhereCouldMenardi;
extern u8 MsgEveryoneHereIn;
extern u8 MsgYeahWellWe;
extern u8 MsgYouCominThrough;
extern u8 MsgYeahUhWe;
extern u8 MsgOhUhYou;
extern u8 MsgWasIJust;
extern u8 MsgHeyMisterAre;
extern u8 MsgDidYouCome;
extern u8 MsgIsThereBy;

void Func_02000108(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(4, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgItLooksLikeThe);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgTheArmorerAnd);
  else
    Event_SetMessageId((s32)&MsgAfterTheChampa);
  Event_ShowMessageAndWait(29, 0);
}

void Func_0200018c(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(5, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgPeopleAreSaying);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgIKnewThere);
  else
    Event_SetMessageId((s32)&MsgOwOwOw);
  Event_ShowMessageAndWait(30, 0);
}

void Func_02000210(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(6, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgMyBoyfriendShin);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgAllMyInjuries);
  else
    Event_SetMessageId((s32)&MsgMySisterWas);
  Event_ShowMessageAndWait(arg, 0);
}

void Func_02000294(void) {
  Event_SetMessageId((s32)&MsgIfYouDon);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
}

void Func_020002bc(void) {
  s32 a, b;

  Event_SetMessageId(MsgHaveYouBy);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    a = 0x103;
    b = 30;
    /* FAKEMATCH(barrier): a and b held at an empty asm, so 0x103 and the frame
     * count are built before the id, and again for the second call (with no
     * asm the calls share 0x103 in r5); as no asm, 14 of 38 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+r"(a), "+r"(b));
    Func_080d47b4(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      Func_080d47b4(33, a, b);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}
