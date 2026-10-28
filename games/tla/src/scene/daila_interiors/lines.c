#include "types.h"
#include "field_event.h"
#include "sequence_parent_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila.h"
#include "scene_tables.h"
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
  Actor_SetAngleToward(actor, 4, 0);
  FieldEvent_Wait(10);
  Func_02000d94(actor, 0x103, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  Import_EventSetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Import_EventShowMessageAndWait(actor, 0);
  FieldEvent_Wait(10);
}

s32 DailaInteriors_TalkScamps(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsScamps);
  Import_EventShowMessageAndWait(19, 0);
  Import_EventEnd();
  return 0;
}

s32 DailaInteriors_TalkMazeTreasure(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsMazeTreasure);
  Import_EventShowMessageAndWait(19, 0);
  Import_EventEnd();
  return 0;
}

void DailaInteriors_StartLeaderEvent(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_ActorSetSpritePriority(gSequenceParentWork.currentOwner, 1);
}

extern const u8 gData_020016e4[];
extern const u8 gData_02001924[];
extern const u8 gData_02001990[];
extern const u8 gData_02001c18[];

const struct Trigger *Scene_GetEvents(void) {
  s32 flag = GameFlag_Test(0x873);
  struct SequenceParentWork *party = &gSequenceParentWork;
  s32 id = (s32)&SceneId_DailaInteriors2;

  if (flag) {
    if (party->scene == id)
      return (const struct Trigger *)gData_02001c18;
    return (const struct Trigger *)gData_02001990;
  }
  if (party->scene == id)
    return (const struct Trigger *)gData_02001924;
  return (const struct Trigger *)gData_020016e4;
}
