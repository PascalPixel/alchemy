#include "types.h"
#include "adventure.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct DailaActor {
  u8 unk00[6];
  u16 facing;
};

struct DailaActor *Actor_GetById(s32 id);
void Actor_SetModeById(s32 actor, s32 mode);
void Func_02000ddc(s32 slot, s32 actor);
void Func_02000dec(s32 slot, s32 actor);
s32 Func_02000dd4(void);
void Func_02000de4(s32 actor);

void Func_02000270(s32 actor) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    if (GameFlag_Test(0x847) == 0) {
      Import_EventStart();
      Import_EventResolvePendingActions(0);
      msg = (s32)MsgDailaInteriorsTravelersFreeTime;
      Import_EventSetMessageId(msg);
      FieldMessage_ShowAtActor(actor, 0);
      if (Event_AskYesNoWithActorAnimation(Func_02000dd4(), 0) == 0) {
        FieldEvent_WaitFrames(10);
        Import_EventSetMessageId(msg + 1);
      } else {
        FieldEvent_WaitFrames(20);
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
      FieldEvent_WaitFrames(10);
      Import_EventSetMessageId(msg + 1);
    } else {
      FieldEvent_WaitFrames(20);
      Import_EventSetMessageId(msg + 2);
    }
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
    GameFlag_SetBit(0x847);
  }
}

void Func_02000498(s32 actor) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
  u16 facing = Actor_GetById(gAdventure.currentOwner)->facing;

  /* FAKEMATCH: keeps the quadrant offset after the facing load. */
  __asm__("");

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
    FieldEvent_WaitFrames(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_WaitFrames(20);
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
    FieldEvent_WaitFrames(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_WaitFrames(20);
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
  FieldEvent_WaitFrames(30);
  Actor_SetModeById(26, 3);
  FieldEvent_WaitFrames(30);
  Import_EventShowMessageAndWait(26, 0);
  Import_EventEnd();
}
