#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_020032a8();
void GameFlag_SetBit();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_ResetAndSetPosition();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetPositionAndCommit();
void Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
void Actor_SetModeById();
void Func_02003148();

void Func_0200008c(void) {
  s32 v5;
  struct Actor *object0;
  s32 v6;
  struct Actor *object1;

  GameFlag_SetBit(SPLIT_CONSTANT(0x900, 1));
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_GetById(*(s32 *)((u8 *)v5 + 0));
  v6 = 192;
  v6 = (v6 << 11);
  *(s32 *)((u8 *)object0 + 40) = v6;
  Actor_SetSpeedParameters(*(s32 *)((u8 *)v5 + 0), 0x20000, 0x10000);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, *(s32 *)((u8 *)v5 + 0),
        136, 488);
  FieldEvent_Wait(10);
  Call3((void (*)())Actor_ResetAndSetPosition, 15, 120, 472);
  Actor_SetPositionAndCommit(14, 136, 475);
  object1 = Actor_GetById(15);
  *(s32 *)((u8 *)object1 + 40) = v6;
  Func_020032a8(SPLIT_CONSTANT(0x138, 255));
  Actor_WaitForMoveById(*(s32 *)((u8 *)v5 + 0));
  Actor_SetModeById(*(s32 *)((u8 *)v5 + 0), 1);
  Actor_ArmCallback(*(s32 *)((u8 *)v5 + 0), FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_WaitForMoveById(15);
  Func_020032a8(159);
  return;
}

void Func_02000128(s32 arg0) {
  Func_02003148(arg0, 0x1c9, SPLIT_CONSTANT(0x900, 2));
  return;
}

void FieldEvent_Wait();
void Field_StartPaletteTransition();
void Func_02003130();
void Func_02003138();
void Func_020031c8();
void Func_020031d8();
void Func_020031e0();
void Func_02003240();
void Func_02003268();
void Func_02003278();
void Func_02003280();
void Func_02003288();
void Func_02003290();
void Func_020032a8();
s32 GameFlag_Test();
void GameFlag_SetBit();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
struct Actor *Actor_GetById();

void Func_0200013c(s32 id) {
  struct Actor *object = Actor_GetById(12);
  u8 *flags;
  s32 i;

  Func_02003130();
  Func_02003240(0);
  Func_020031c8((s32)MsgGooGooGa);
  Func_020031e0(11, 0);
  Func_02003268(8, SPLIT_CONSTANT(0x1200, 16));
  Func_02003278();
  Func_02003290((s32)Actor_GetById(id), 2);
  Func_020032a8(201);
  FieldEvent_Wait(30);
  flags = &object->motionFlags;
  *flags = 4;
  for (i = 39; i >= 0; i--) {
    object->y += SPLIT_CONSTANT(0x3300, 51);
    FieldEvent_Wait(1);
  }
  FieldEvent_Wait(70);
  Func_02003290((s32)Actor_GetById(id), 0);
  Func_02003288();
  Func_02003280();
  Field_StartPaletteTransition(8);
  *flags = 3;
  object->gravity = 0x8000;
  FieldEvent_Wait(5);
  Func_020032a8(132);
  if (Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x800, 174)) == 0) {
    GameFlag_SetBit(SPLIT_CONSTANT(0x800, 174));
    FieldEvent_Wait(30);
    Actor_ArmCallback(10, 0x3000, 0);
    FieldEvent_Wait(30);
    Motion_SetVarCbAndRefresh(10, 2);
    FieldEvent_Wait(10);
    Func_020031d8(10, 0, 10);
    Call3((void (*)())Actor_ArmCallback, 10, SPLIT_CONSTANT(0xaf00, 255), 0);
  }
  Func_02003138();
}
