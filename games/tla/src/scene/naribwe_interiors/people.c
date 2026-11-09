#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "event_inline.h"

extern const u8 gData_02001370[];
extern const u8 gData_020014c0[];

void Func_020010e0(void);
void Func_020010e8(void);
void Func_02001190(s32 slot, s32 actor);
void Func_02001198(s32 slot, s32 actor);

const u8 *Func_020000a4(void) {
  if (Event_GetFlag(0x8ff))
    return gData_020014c0;
  return gData_02001370;
}

void Func_020000c4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(14, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouAre);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}

void Func_02000124(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(14, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgWheneverIGet);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}

void Func_02000184(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(15, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgYouShouldVisit);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}

void Func_020001e4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(15, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouWish);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}

void Func_02000244(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001198(5, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgOurCookingIs);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}

void Func_020002a4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001198(5, actor);
  } else {
    Func_020010e0();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgISeeSo);
    Event_ShowMessage(actor, 0);
    Func_020010e8();
  }
}
