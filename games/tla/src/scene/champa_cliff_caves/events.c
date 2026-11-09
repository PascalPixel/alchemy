#include "scene_tables.h"
#include "scene.h"
#include "scene_ids.h"

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);

void Func_02000304(s32 actor) {
  s32 msg = (s32)MsgWhatYouWant;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_02000350(s32 actor) {
  struct FieldActor *object;

  Event_SetMessageId((s32)MsgAllRightI);
  Event_ShowMessageAndWait(actor, 0);
  object = Actor_GetById(actor);
  Func_02003624(object, Actor_GetById(gMainRoutineWork.currentOwner));
}

void Func_0200038c(s32 actor) {
  Event_SetMessageId((s32)MsgAwComeOn);
  Event_ShowMessageAndWait(actor, 0);
  Func_02003534_(actor, 0xd000);
  Actor_SetActionCallback((struct Actor *)Actor_GetById(actor), 8);
}

void Func_020003bc(void) {
  Func_0802b450(41, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(41, 33, 1, 4, 41, 13);
}

void Func_020003ec(void) {
  struct FieldActor *object;
  s32 x;
  s32 z;

  Func_0802b450(50, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(46, 33, 1, 4, 41, 13);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov685Room4 && x == 41 &&
      (u32)(z - 13) <= 4)
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                           PIXELS(664), PIXELS(280));
}
