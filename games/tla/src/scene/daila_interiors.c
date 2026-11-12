/// @file
/// The scene code of overlay 0x652 that its scene tables belong to, in ROM order; the rest of the overlay is in daila_interiors/.

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
#include "scene_ids.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila_interiors/daila.h"
#include "ram_buffer.h"
#include "actor.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

extern const u8 gData_0200116c[];
extern const u8 gData_020011b8[];

const void *Scene_GetExits(void) {
  if (GameFlag_Test(0x873))
    return gData_020011b8;
  return gData_0200116c;
}

extern const u8 gData_02001204[];
extern const u8 gData_020013fc[];
extern const u8 gData_020014a4[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors2)
    return (const struct ActorPlacement *)gData_020013fc;
  if (GameFlag_Test(0x873))
    return (const struct ActorPlacement *)gData_020014a4;
  return (const struct ActorPlacement *)gData_02001204;
}

void DailaInteriors_EmptyHook(void) {}

void Func_020000ac(s32 unused, s32 actor) {
  Daila_Jump(actor, 0x100, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (!Daila_FlagCheck(0x200)) {
    Import_EventSetMessageId((s32)MsgDailaInteriorsGah);
    Import_EventShowMessageAndWait(actor, 0);
    Daila_ObjWalk(actor, 360, 72);
    Actor_ArmCallback(actor, FACING_SOUTH, 0);
    GameFlag_SetBit(0x200);
  } else {
    Import_EventSetMessageId((s32)MsgDailaInteriorsYarg);
    Import_EventShowMessageAndWait(actor, 0);
    Daila_ObjWalk(actor, 392, 104);
    Actor_ArmCallback(actor, FACING_WEST, 0);
    GameFlag_ClearBit(0x200);
  }
  FieldEvent_Wait(20);
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
  Import_ActorSetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

extern const u8 gData_020016e4[];
extern const u8 gData_02001924[];
extern const u8 gData_02001990[];
extern const u8 gData_02001c18[];

const struct Trigger *Scene_GetEvents(void) {
  s32 flag = GameFlag_Test(0x873);
  struct MainRoutineWork *party = &gMainRoutineWork;
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

struct DailaActor {
  u8 unk00[6];
  u16 facing;
};

struct DailaActor *Actor_GetById(s32 id);

void Func_02000ddc(s32 slot, s32 actor);
void Func_02000dec(s32 slot, s32 actor);
s32 Func_02000dd4(void);
void Func_02000de4(s32 actor);

void Func_02000270(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsSouthRoadDanger);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020002d0(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(2, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsNewTravelers);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000330(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
    Func_02000dec(0, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsDinnerChoice);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000390(s32 actor) {
  s32 msg;
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    if (GameFlag_Test(0x847) == 0) {
      Import_EventStart();
      Import_EventResolvePendingActions(0);
      msg = (s32)MsgDailaInteriorsTravelersFreeTime;
      Import_EventSetMessageId(msg);
      FieldMessage_ShowAtActor(actor, 0);
      if (Event_AskYesNoWithActorAnimation(Func_02000dd4(), 0) == 0) {
        FieldEvent_Wait(10);
        Import_EventSetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        Import_EventSetMessageId(msg + 2);
      }
      Import_EventShowMessageAndWait(actor, 0);
      Import_EventEnd();
      GameFlag_SetBit(0x847);
    } else {
      Func_02000de4(actor);
    }
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    msg = (s32)MsgDailaInteriorsTravelersFreeTime;
    Import_EventSetMessageId(msg);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000dd4(), 0) == 0) {
      FieldEvent_Wait(10);
      Import_EventSetMessageId(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Import_EventSetMessageId(msg + 2);
    }
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
    GameFlag_SetBit(0x847);
  }
}

void Func_02000498(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000de4(actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsLeaveForMadra);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020004f4(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsPirateTreasure);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000554(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(2, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsSpaciousVillage);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020005b4(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
    Func_02000dec(0, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsFishermenWaiting);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000614(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsGoingToMadra;
  Import_EventSetMessageId(msg);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02000dd4(), 0) == 0) {
    FieldEvent_Wait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(12, 0);
  Import_EventEnd();
}

void Func_0200066c(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsLookingForBoat;
  Import_EventSetMessageId(msg);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02000dd4(), 0) == 0) {
    FieldEvent_Wait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(14, 0);
  Import_EventEnd();
}

void Func_020006c4(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsWatchingPractice);
  Import_EventShowMessageAndWait(26, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(26, 3);
  FieldEvent_Wait(30);
  Import_EventShowMessageAndWait(26, 0);
  Import_EventEnd();
}

extern const u8 gData_02000e04[];

void Func_02000dcc(const u8 *script);

void Func_02000704(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 flags;

  Func_02000dcc(gData_02000e04);
  object = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= object->priorityFlags;
  flags &= 0xfd;
  object->priorityFlags = flags;
}
