#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02002b40[];
extern const u8 gData_02002cf0[];
extern const u8 gData_02002d80[];
extern const u8 gData_02002f78[];
extern const u8 gData_02003164[];
extern const u8 gData_0200323c[];

void Func_020021c4(void);
void Func_020021cc(void);
void Func_02002304(s32 slot, s32 actor);
void Func_0200230c(s32 actor);
void Func_02002314(s32 slot, s32 actor);
void Func_0200061c(s32 mode);

static inline void LeaderAnimate(s32 animation, s32 frames) {
  /* FAKEMATCH(signature): Func_080d47b4 called through the inline as
   * value-returning, so the animation and frames are set before the owner is
   * loaded; as a void call, 4 instructions differ. */
  ((s32 (*)(s32, s32, s32))Func_080d47b4)(gMainRoutineWork.currentOwner,
                                          animation, frames);
}

const u8 *Func_020000b4(void) {
  if (GameFlag_Test(0x927))
    return gData_02002d80;
  if (gMainRoutineWork.entrance == 7)
    return gData_02002cf0;
  return gData_02002b40;
}

const u8 *Func_020000f0(void) {
  if (GameFlag_Test(0x927))
    return gData_0200323c;
  if (gMainRoutineWork.entrance == 7)
    return gData_02003164;
  return gData_02002f78;
}

void Func_0200012c(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_0200230c(actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927)) {
      Event_SetMessageId((s32)MsgNowThatPeace);
      Event_ShowMessageAndWait(12, 0);
    } else {
      Event_SetMessageId((s32)MsgWhyWouldOur);
      Event_ShowMessageAndWait(22, 0);
    }
    Func_020021cc();
  }
}

void Func_020001a8(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02002314(9, actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927))
      Event_SetMessageId((s32)MsgBriggsFromChampa);
    else
      Event_SetMessageId((s32)MsgOfCourseI);
    Event_ShowMessageAndWait(actor, 0);
    Func_020021cc();
  }
}

void Func_02000220(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02002304(24, actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927))
      Event_SetMessageId((s32)MsgYouMustSee);
    else
      Event_SetMessageId((s32)MsgIzumoSLeaders);
    Event_ShowMessageAndWait(actor, 0);
    Func_020021cc();
  }
}

void Func_02000298(void) {
  Func_020021c4();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x922)) {
    Event_SetMessageId((s32)MsgSusaSNot);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Func_080d47b4(8, 265, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Func_080d47b4(8, 261, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    GameFlag_SetBit(0x922);
  }
  Actor_SetModeById(8, 4);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgIfTheSerpent);
  Event_ShowMessageAndWait(8, 0);
  Func_020021cc();
}

void Func_02000358(void) {
  GameFlag_SetBit(0x923);
  Event_SetMessageId((s32)MsgIPrayThat);
  Event_ShowMessageAndWait(8, 0);
}

void Func_02000378(void) {
  void *work = Ram_HeapSlots->eventWork;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  if (GameFlag_Test(0x92c)) {
    Event_SetMessageId((s32)MsgWithThisDancing);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgIfYouHave);
    Event_ShowMessageAndWait(15, 0);
    if (PartyInventory_FindOwner(451) != -1)
      FIELD_AT(work, u16, 346) = 1;
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Func_020021cc();
}

void Func_02000400(void) {
  s32 msg;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgTheyVeBeen;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(16, 0);
  Func_020021cc();
}

void Func_02000458(void) {
  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  if (Event_GetFlag(0x92b)) {
    Event_SetMessageId((s32)MsgTryingAgainWhat);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgAndWhatWere);
    Event_ShowMessageAndWait(15, 0);
    if (GameFlag_Test(0x923)) {
      LeaderAnimate(256, 30);
      Func_080d47b4(15, 259, 30);
      Event_ShowMessageAndWait(15, 0);
      LeaderAnimate(258, 30);
      Actor_SetModeById(15, 4);
      Event_Wait(30);
      Event_ShowMessageAndWait(15, 0);
    }
    GameFlag_SetBit(0x92b);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Func_020021cc();
}
