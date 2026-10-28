#include "types.h"
#include "field_event.h"
#include "adventure.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila.h"
#include "field_scene.h"
#include "scene_ids.h"

void Func_020000ac(s32 unused, s32 actor) {
  Daila_Jump(actor, 0x100, 50);
  Daila_ObjLaunch(actor, 6, 15);
  Daila_ObjLaunch(actor, 6, 15);
  Daila_Wait(10);
  if (!Daila_FlagCheck(0x200)) {
    Daila_MessageSet((s32)MsgDailaInteriorsGah);
    Daila_MessagePut(actor, 0);
    Daila_ObjWalk(actor, 360, 72);
    Daila_ObjTurn(actor, 0x4000, 0);
    Daila_FlagSet(0x200);
  } else {
    Daila_MessageSet((s32)MsgDailaInteriorsYarg);
    Daila_MessagePut(actor, 0);
    Daila_ObjWalk(actor, 392, 104);
    Daila_ObjTurn(actor, 0x8000, 0);
    Daila_FlagClr(0x200);
  }
  Daila_Wait(20);
}

void Func_02000150(s32 unused, s32 actor) {
  ObjectMotion_SetAngleToward(actor, 4, 0);
  FieldEvent_WaitFrames(10);
  Func_02000d94(actor, 0x103, 50);
  ObjectMotion_Launch(actor, 6, 15);
  ObjectMotion_Launch(actor, 6, 15);
  FieldEvent_WaitFrames(10);
  Engine_EventSetMessage((s32)MsgDailaInteriorsWhatAreYouDoing);
  Engine_EventShowMessage(actor, 0);
  FieldEvent_WaitFrames(10);
}

s32 DailaInteriors_TalkScamps(void) {
  Engine_EventStart();
  Engine_EventResolvePendingActions(0);
  Engine_EventSetMessage((s32)MsgDailaInteriorsScamps);
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
  return 0;
}

s32 DailaInteriors_TalkMazeTreasure(void) {
  Engine_EventStart();
  Engine_EventResolvePendingActions(0);
  Engine_EventSetMessage((s32)MsgDailaInteriorsMazeTreasure);
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
  return 0;
}

void DailaInteriors_StartLeaderEvent(void) {
  Engine_EventStart();
  Engine_EventResolvePendingActions(0);
  Engine_ActorSetSpritePriority(gAdventure.currentOwner, 1);
}

extern const u8 gData_020016e4[];
extern const u8 gData_02001924[];
extern const u8 gData_02001990[];
extern const u8 gData_02001c18[];

const struct SceneEvent *Scene_GetEvents(void) {
  s32 flag = GameFlag_Test(0x873);
  struct Adventure *party = &gAdventure;
  s32 id = (s32)&SceneId_DailaInteriors2;

  if (flag) {
    if (party->scene == id)
      return (const struct SceneEvent *)gData_02001c18;
    return (const struct SceneEvent *)gData_02001990;
  }
  if (party->scene == id)
    return (const struct SceneEvent *)gData_02001924;
  return (const struct SceneEvent *)gData_020016e4;
}
