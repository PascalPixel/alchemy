/// @file
/// The scene code of overlay 0x66d, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gSceneExits[];

struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_End(void);
void Func_0200059c(s32 id);
void Func_020005cc(void);
void Func_020005d4(s32 a, s32 b, s32 c, s32 d);
void Shop_Run(s32 a, s32 b);
void Sanctum_Run(s32 a);
void Naribwe_HideStone(void);

extern const u8 gNaribwe_Placements[];
extern const u8 gData_020008c4[];
extern const u8 gData_02000954[];
extern const u8 gNaribwe_Triggers[];
extern const u8 gNaribwe_TriggersOnReturn[];
extern const struct WarpEntry gNaribwe_Doors[];

extern u8 MsgNaribwe02;
extern u8 MsgNaribwe03;
extern u8 MsgNaribwe07;
extern u8 MsgNaribwe08;
extern u8 MsgNaribwe09;
extern u8 MsgNaribwe21;
extern u8 MsgNaribwe43;
extern u8 MsgNaribwe23;
extern u8 MsgNaribwe45;
#include "field.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == 10)
    return gData_020008c4;
  if (Event_GetFlag(0x8ff))
    return gData_02000954;
  return gNaribwe_Placements;
}

#include "event_inline.h"

void Func_02000088(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u8 *flag;

  flag = work + 0xd38;
  *flag = 1;
  if (gMainRoutineWork.entrance != 10)
    *flag = 0;
}

const u8 *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8ff))
    return gNaribwe_TriggersOnReturn;
  return gNaribwe_Triggers;
}

/// "The Kibombo went on an expedition to the east, but they've already
/// returned." Asked once (flag 0x85d); afterwards the villager talks of
/// Akafubu.
void Naribwe_AskAboutKibomboExpedition(void) {
  s32 msg;

  if (Event_GetFlag(0x85d) == 0) {
    msg = (s32)&MsgNaribwe02;
    Event_SetMessageId(msg);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(14, 0);
  } else {
    Event_SetMessageId((s32)&MsgNaribwe03);
    Event_ShowMessage(14, 0);
  }
}

/// "Can you tell me where you have come from?" Asked once (flag 0x85e).
void Naribwe_AskWhereFrom(void) {
  s32 msg;

  if (Event_GetFlag(0x85e) == 0) {
    msg = (s32)&MsgNaribwe07;
    Event_SetMessageId(msg);
    Event_OpenMessage(18, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(18, 0);
  } else {
    Event_SetMessageId((s32)&MsgNaribwe08);
    Event_ShowMessage(14, 0);
  }
}

/// "You don't plan on going to Kibombo, do you, mister?"
void Naribwe_AskGoingToKibombo(void) {
  s32 msg = (s32)&MsgNaribwe09;

  Event_SetMessageId(msg);
  Event_OpenMessage(19, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(19, 0);
}

/// "No one can see the future. Absolutely no one. Except for our
/// fortune-teller."
void Naribwe_PraiseFortuneTeller(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Shop_Run(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe21);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Naribwe_PraiseFortuneTellerOnReturn(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Shop_Run(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe43);
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// "I am but a humble pilgrim, serving the greater good in the heart of
/// Gondowan."
void Naribwe_TalkToPilgrim(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe23);
  Event_ShowMessage(26, 0);
  Event_End();
}

void Naribwe_TalkToPilgrimOnReturn(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe45);
  Event_ShowMessage(26, 0);
  Event_End();
}

/// The party leader walks into one of Naribwe's doors, which opens with its
/// cell animation, and leaves by the exit of the door's number.
void Naribwe_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  u32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  work += 0x154;
  index = *(s16 *)work;
  Event_PlaySound(158);
  index -= 4;
  MapGrid_AnimateCells(gNaribwe_Doors[index].script, gNaribwe_Doors[index].x,
                       gNaribwe_Doors[index].z);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Whirlwind blows away the ivy in the back yard, uncovering the ladder (flag
/// 0x201).
void Naribwe_BlowAwayIvy(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(0x201);
  Event_End();
}

void Func_0200041c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  work[0xd31] = 0;
  work[0xd32] = 24;
  work[0xd33] = 25;
}

/// Reveal shows the stone that hides the Unicorn Ring.
void Naribwe_RevealStone(void) {
  Actor_SetPosition(27, 0, 0);
  TreasureSpot_SetPosition(65, 0x680000, 0x680000);
}

void Naribwe_HideStone(void) {
  ObjectEvent_PlaceOnGround(65, 0, 0);
  Actor_SetPosition(27, PIXELS(104), PIXELS(104));
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Actor_SetSingleChildFlags(Actor_GetById(23), 0);
  Actor_GetById(23)->scaleY = 0x19999;
  Actor_GetById(27)->scaleX = 0x8000;
  Actor_GetById(27)->scaleY = 0x8000;
  Naribwe_HideStone();
  if (Event_GetFlag(0x201))
    Actor_SetPosition(23, 0, 0);
  Func_020005cc();
  Func_020005d4(0, 0x202, 24, 25);
  Actor_SetSingleChildFlags(Actor_GetById(24), 0);
  Func_0200059c(25);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
