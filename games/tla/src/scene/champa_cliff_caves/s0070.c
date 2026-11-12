/// @file
/// Scene code of overlay 0x685, in ROM order, from s0070's offset up to the next object the overlay links.

#include "scene.h"
#include "scene_ids.h"
#include "scene_tables.h"

extern u32 gData_0200375c[];
extern u32 gData_020037a4[];
extern u32 gData_0200381c[];
extern u32 gData_02003864[];
extern u32 gData_020038ac[];
extern u32 gData_0200390c[];
extern u32 gData_02003984[];
extern u32 gData_020039cc[];
extern u32 gData_02003aec[];
extern u32 gData_02003b94[];
extern u32 gData_02003c0c[];
extern u32 gData_02003c84[];

u32 *Func_02000070(void) {
  s32 scene;

  if (GameFlag_Test(0x97e)) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov685Room1)
      return gData_02003aec;
    if (scene == (s32)&SceneId_Ov685Room2)
      return gData_02003b94;
    if (scene == (s32)&SceneId_Ov685Room3)
      return gData_02003c0c;
    if (scene == (s32)&SceneId_Ov685Room4)
      return gData_02003c84;
  } else {
    if (GameFlag_Test(0x97f)) {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_020038ac;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_0200390c;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_02003984;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_020039cc;
    } else {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_0200375c;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_020037a4;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_0200381c;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_02003864;
    }
  }
  return gData_0200375c;
}

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200345c();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003464();
}

void Func_020001f4(s32 door) { Func_0200016c(&gData_02003d10, door, 0); }

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);

extern const struct WarpTarget gData_02003d18;

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

void Func_02000208(void) {
  const struct WarpTarget *warp;
  s32 msg;

  warp = &gData_02003d10;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 424);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(8)->motionFlags = 0;
  Func_020034d4_(8, 184 << 16, 204 << 17);
  Actor_SetSpeedParameters_(8, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(8, 0, 16);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  msg = (s32)MsgHasMyGrandson;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters_(8, 0xcccc, 0x6666);
  Actor_CommitPositionAndActivate_(8, 0, -14);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  warp = &gData_02003d18;
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Sound_Play(158);
}

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

extern s32 gData_02003d20[];
extern s32 gData_02003e7c[];
extern s32 gData_02003ffc[];
extern s32 gData_02004104[];

void Func_02000464(s32 a) { Func_02003474(a, 326, 0x978); }

s32 *Func_02000478(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov685Room1)
    return gData_02003d20;
  if (scene == (s32)&SceneId_Ov685Room2)
    return gData_02003e7c;
  if (scene == (s32)&SceneId_Ov685Room3)
    return gData_02003ffc;
  if (scene == (s32)&SceneId_Ov685Room4)
    return gData_02004104;
  return gData_02003d20;
}
