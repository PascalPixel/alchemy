/// @file
/// Izumo's houses, inn and Sanctum, with Susa's house when entered by entrance
/// 7, and other tables once the Great Serpent is slain (FLAG_SERPENT_SLAIN).

#include "shared/tables.h"
#include "types.h"
#include "flags.h"
#include "actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "objcmd.h"
#include "text/msg_ids.h"

extern struct ActorPlacement gIzumoInteriors_ActorPlacements[];
extern struct ActorPlacement gIzumoInteriors_SusasHouseActorPlacements[];
extern struct ActorPlacement
    gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain[];
extern struct Trigger gIzumoInteriors_Triggers[];
extern struct Trigger gIzumoInteriors_SusasHouseTriggers[];
extern struct Trigger gIzumoInteriors_TriggersOnceTheSerpentIsSlain[];

void Inn_CheckIn(s32 slot, s32 actor);

void IzumoInteriors_RecognizeTheHeroes(s32 mode);

void IzumoInteriors_ShowMove(void);
extern s32 gIzumoInteriors_FirstPacerScript[];
extern s32 gIzumoInteriors_SecondPacerScript[];
extern s32 gIzumoInteriors_RockingScript[];

/// Whether the other of the two men pacing in Susa's house is clear of the
/// spot by the doorway this one walks to; their scripts wait on it.
#define DEFINE_IZUMO_INTERIORS_IS_CLEAR(name, n1, n2)                          \
  s32 name(void) {                                                             \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    if ((u32)(object->x - n2) <= 0x80000) {                                    \
      s32 z = object->z;                                                       \
                                                                               \
      if (z >= 0x26c0000) {                                                    \
        if (z <= 0x2740000)                                                    \
          return 0;                                                            \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }

DEFINE_IZUMO_INTERIORS_IS_CLEAR(IzumoInteriors_IsFirstPacerClear, 10, 0x940000)

DEFINE_IZUMO_INTERIORS_IS_CLEAR(IzumoInteriors_IsSecondPacerClear, 9, 0x840000)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Shop_Run(s32 slot, s32 actor);

static inline void LeaderAnimate(s32 animation, s32 frames) {
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner, animation, frames);
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == 7)
    return gIzumoInteriors_SusasHouseActorPlacements;
  return gIzumoInteriors_ActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoInteriors_TriggersOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == 7)
    return gIzumoInteriors_SusasHouseTriggers;
  return gIzumoInteriors_Triggers;
}

void IzumoInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      Event_SetMessageId((s32)MsgNowThatPeace);
      Event_ShowMessageAndWait(12, 0);
    } else {
      Event_SetMessageId((s32)MsgWhyWouldOur);
      Event_ShowMessageAndWait(22, 0);
    }
    Event_End();
  }
}

void IzumoInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(9, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      Event_SetMessageId((s32)MsgBriggsFromChampa);
    else
      Event_SetMessageId((s32)MsgOfCourseI);
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

void IzumoInteriors_TalkToMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(24, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      Event_SetMessageId((s32)MsgYouMustSee);
    else
      Event_SetMessageId((s32)MsgIzumoSLeaders);
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

void IzumoInteriors_AskAfterSusa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x922)) {
    Event_SetMessageId((s32)MsgSusaSNot);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    GameFlag_Set(0x922);
  }
  Actor_SetModeById(8, 4);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgIfTheSerpent);
  Event_ShowMessageAndWait(8, 0);
  Event_End();
}

void IzumoInteriors_ReadPrayerForSusa(void) {
  GameFlag_Set(0x923);
  Event_SetMessageId((s32)MsgIPrayThat);
  Event_ShowMessageAndWait(8, 0);
}

void IzumoInteriors_TalkToUzume(void) {
  void *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  if (GameFlag_Get(FLAG_UZUME_HAS_THE_DOLL)) {
    Event_SetMessageId((s32)MsgWithThisDancing);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgIfYouHave);
    Event_ShowMessageAndWait(15, 0);
    if (PartyInventory_FindMember(451) != -1)
      FIELD_AT(work, u16, 346) = 1;
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Event_End();
}

void IzumoInteriors_AskAboutMtMikage(void) {
  s32 msg;

  Event_Start();
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
  Event_End();
}

void IzumoInteriors_ReadUzumesMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  if (Event_GetFlag(0x92b)) {
    Event_SetMessageId((s32)MsgTryingAgainWhat);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgAndWhatWere);
    Event_ShowMessageAndWait(15, 0);
    if (GameFlag_Get(0x923)) {
      LeaderAnimate(256, 30);
      Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      Event_ShowMessageAndWait(15, 0);
      LeaderAnimate(258, 30);
      Actor_SetModeById(15, 4);
      Event_Wait(30);
      Event_ShowMessageAndWait(15, 0);
    }
    GameFlag_Set(0x92b);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Event_End();
}

s32 IzumoInteriors_KushinadaPleads(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x925)) {
    Event_SetMessageId((s32)MsgSusaDonT);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(17, 1);
    Event_Wait(30);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_SetFlag(0x925);
  } else {
    Event_SetMessageId((s32)MsgSusaPleaseDon);
    Event_ShowMessageAndWait(17, 0);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_WalkByById(gMainRoutineWork.currentOwner, 0, 16);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Event_End();
  return 0;
}

void IzumoInteriors_HeroesComeFromTheEast(void) {
  IzumoInteriors_RecognizeTheHeroes(0);
}

void IzumoInteriors_HeroesComeFromTheWest(void) {
  IzumoInteriors_RecognizeTheHeroes(1);
}

/// The scene that opens with MsgWarriorsAppearedFrom: the warriors confront
/// the party in front of persons 20, 21 and 22 through three yes-or-no
/// answers, then persons 5, 6, 7 and 23 join the leader and flags
/// FLAG_IZUMO_RECOGNIZED_THE_HEROES and 0x350 are set. Mode 0 and mode 1
/// (IzumoInteriors_HeroesComeFromTheEast and
/// IzumoInteriors_HeroesComeFromTheWest) start the party at different spots.
void IzumoInteriors_RecognizeTheHeroes(s32 mode) {
  struct Actor *person, *leader;
  s32 party, owner, answer, message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(143, CAMERA_KEEP, 560, 1);
  party = (s32)&gMainRoutineWork;
  if (mode == 0) {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 184, 568);
  } else {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 104, 568);
  }
  FieldEvent_Wait(15);
  FieldEvent_SetMessageId((s32)MsgWarriorsAppearedFrom);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_SetAnimation(22, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(22, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_FaceDirection(20, FACING_EAST, 30);
  Actor_SetAnimation(20, 4);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_WEST, 30);
  Actor_SetAnimation(15, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(15);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(15, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(30);
  Actor_SetDestinationOffset(15, 0, 16);
  FieldActor_WaitForMoveById(15);
  FieldEvent_Wait(15);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  if (mode == 0) {
    Actor_WalkToAndWait(15, 168, 536);
  } else {
    Actor_WalkToAndWait(15, 120, 536);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_FaceActor(20, *(s32 *)owner, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldActor_Jump(22, 4, 0);
  Actor_SetAttachedEffect(22,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  FieldEvent_Wait(30);
  Actor_FaceDirection(20, FACING_SOUTH, 30);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(22, 20, 0);
  Actor_SetAttachedEffect(22,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Event_ShowMessage(22, 0);
  FieldEvent_Wait(30);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_WalkTo(15, 144, 520);
  FieldActor_WaitForMoveById(15);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_FaceDirection(20, FACING_SOUTH, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  if (mode == 0) {
    Actor_WalkTo(22, 104, 568);
  } else {
    Actor_WalkTo(22, 184, 568);
  }
  FieldActor_WaitForMoveById(22);
  if (mode == 0) {
    Actor_FaceDirection(22, FACING_EAST, 0);
  } else {
    Actor_FaceDirection(22, FACING_WEST, 0);
  }
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_WalkTo(*(s32 *)owner, 144, 560);
  FieldActor_WaitForMoveById(*(s32 *)owner);
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(5, 8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, 24, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(7, -8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(23, -24, 24, FACING_NORTH);
  FieldEvent_Wait(60);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, 7, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(5, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Actor_SetAnimation(6, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(5, 23, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(6, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_FaceEachOther(15, 20, 0);
  FieldEvent_Wait(60);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(20, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(*(s32 *)owner, 23, 0);
  Actor_FaceActor(5, 23, 0);
  Actor_FaceActor(6, 23, 0);
  Actor_FaceActor(7, 23, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, 15, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_FaceActor(*(s32 *)owner, 15, 0);
  Actor_FaceActor(5, 15, 0);
  Actor_FaceActor(6, 15, 0);
  Actor_FaceActor(7, 15, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(60);
  Actor_FaceDirection(15, FACING_EAST, 0);
  FieldEvent_Wait(30);
  IzumoInteriors_ShowMove();
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(5, 1);
  FieldEvent_Wait(15);
  Event_ShowMessage(5, 0);
  Actor_FaceActor(15, 20, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_Wait(30);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 1) {
    FieldEvent_SetMessageId((s32)MsgIThoughtSo);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
    Actor_SetAnimation(6, 3);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceActor(7, *(s32 *)owner, 0);
    FieldEvent_Wait(15);
    Actor_SetAnimation(7, 4);
    FieldEvent_Wait(30);
    Event_ShowMessage(7, 0);
  }
  FieldEvent_SetMessageId((s32)MsgSoYouCall);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(60);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  FieldEvent_Wait(90);
  Event_ShowMessage(21, 0);
  Actor_SetPosition(21, 144, 680);
  Actor_FaceActor(*(s32 *)owner, 21, 0);
  Actor_FaceActor(5, 21, 0);
  Actor_FaceActor(6, 21, 0);
  Actor_FaceActor(7, 21, 0);
  Actor_FaceActor(23, 21, 0);
  Actor_FaceActor(22, 21, 0);
  FieldEvent_Wait(120);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(30);
  EventCamera_FollowActor(21, 1);
  FieldEvent_Wait(120);
  Actor_SetSpeed(21, 0.6);
  Actor_SetSpeedParameters(21, 0x9999, 0x4ccc);
  Actor_GetById(21)->priorityFlags &= ~1;
  Actor_WalkTo(21, 144, 648);
  if (mode == 0) {
    Actor_WalkToAndWait(21, 88, 648);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 616);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 584);
    Actor_GetById(22)->actionFlags &= ~1;
    Actor_WalkToAndWait(22, 88, 568);
    Event_Wait(1);
    Actor_GetById(22)->actionFlags |= 1;
    Actor_ArmCallback(22, FACING_EAST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 104, 568);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetSpritePriority(23, 1);
    Actor_SetSpritePriority(21, 2);
    Actor_WalkToAndWaitById(21, 120, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(23, 3);
    Actor_WalkToAndWait(21, 120, 536);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetModeById(22, 3);
    Actor_WalkToAndWait(22, 56, 584);
    Actor_WalkToAndWait(22, 88, 648);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  } else {
    Actor_WalkToAndWait(21, 200, 648);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 616);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 584);
    Actor_GetById(22)->actionFlags &= ~1;
    Actor_WalkToAndWait(22, 200, 568);
    Event_Wait(1);
    Actor_GetById(22)->actionFlags |= 1;
    Actor_FaceDirection(22, FACING_WEST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 184, 568);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetSpritePriority(21, 2);
    Actor_SetSpritePriority(6, 1);
    Actor_WalkToAndWaitById(21, 168, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(6, 3);
    Actor_WalkToAndWait(21, 168, 536);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(5, 21, 0);
    Actor_FaceActorById(6, 21, 0);
    Actor_FaceActorById(7, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetModeById(22, 3);
    Actor_WalkToAndWait(22, 232, 584);
    Actor_WalkToAndWait(22, 200, 648);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  }
  person = Actor_GetById(21);
  {
    u8 *priority = &person->priorityFlags;
    s32 flags = *priority | 1;

    owner = (s32)&gMainRoutineWork;
    *priority = flags;
  }
  owner = owner + 532;
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_ArmCallback(23, FACING_NORTH, 0);
  Event_Wait(15);
  Event_ShowMessageAndWait(20, 0);
  Event_Wait(15);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(20, 0);
  Actor_FaceActorById(21, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  Event_Wait(60);
  Actor_FaceActorById(21, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(30);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(23, 21, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(20, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Event_ShowMessageAndWait(20, 0);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(7, 23, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceActorById(23, 7, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActorById(23, 15, 0);
  Actor_FaceActorById(7, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetModeById(5, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(90);
  Actor_SetModeById(15, 3);
  Event_OpenMessage(15, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId((s32)MsgRegardlessOfYour);
    Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgYouTookPity);
    Event_ShowMessageAndWait(15, 0);
  }
  message = (s32)MsgYouReNotImplying;
  Event_SetMessageId(message);
  Actor_StartRepeatedMotion(23, 1);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(15, 23, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(23, 21, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetModeById(7, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_ArmCallback(20, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_Wait(15);
  Actor_FaceActorById(15, 21, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageAndWait(21, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_FaceActorById(6, *(s32 *)owner, 0);
  Event_OpenMessage(6, 0);
  Actor_FaceActorById(5, *(s32 *)owner, 0);
  Actor_FaceActorById(6, *(s32 *)owner, 0);
  Actor_FaceActorById(7, *(s32 *)owner, 0);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  Event_Wait(30);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 10);
    Actor_SetModeById(21, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0);
  } else {
    Event_SetMessageId(message + 11);
    Actor_SetModeById(21, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0);
  }
  message = (s32)MsgIDisagree2;
  Event_SetMessageId(message);
  Actor_FaceActorById(23, 21, 0);
  Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(30);
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Event_Wait(30);
  Event_OpenMessage(23, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 4);
    Actor_SetModeById(5, 3);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(7, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(5, 0);
    Event_ShowMessageAndWait(6, 0);
    Event_ShowMessageAndWait(7, 0);
  } else {
    Event_SetMessageId(message + 7);
    Actor_SetModeById(5, 4);
    Actor_SetModeById(6, 4);
    Actor_SetModeById(7, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(5, 0);
    Event_ShowMessageAndWait(6, 0);
    Event_ShowMessageAndWait(7, 0);
  }
  Event_SetMessageId((s32)MsgAsYouCanSee);
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Actor_StartRepeatedMotion(21, 1);
  Event_Wait(15);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(*(s32 *)owner, 23, 0);
  Event_Wait(15);
  Actor_SetModeById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(5, 23, 0);
  Actor_FaceActorById(6, 23, 0);
  Actor_FaceActorById(7, 23, 0);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Actor_FaceActorById(*(s32 *)owner, 15, 0);
  Actor_FaceActorById(5, 15, 0);
  Actor_FaceActorById(6, 15, 0);
  Actor_FaceActorById(7, 15, 0);
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(5, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceActorById(6, 5, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_StartRepeatedMotion(7, 1);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 7, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_TurnToFaceEachOther(5, 6, 0);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(15);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 5, 0);
  Actor_TurnToFaceEachOther(7, 6, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Actor_FaceActorById(*(s32 *)owner, 15, 0);
  Actor_FaceActorById(5, 15, 0);
  Actor_FaceActorById(6, 15, 0);
  Actor_FaceActorById(7, 15, 0);
  Actor_FaceActorById(23, 15, 0);
  Actor_SetModeById(15, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(15);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Actor_SetModeById(21, 3);
  Event_Wait(60);
  Actor_SetModeById(5, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(5, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetModeById(6, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(6, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(7, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Actor_SetModeById(23, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(23, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(23);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  GameFlag_Set(FLAG_IZUMO_RECOGNIZED_THE_HEROES);
  GameFlag_Set(FLAG_AREA_TEMP_50);
  Event_End();
}

/// The Move cast below has nothing to move.
s32 IzumoInteriors_FindNoMoveTarget(void) { return -1; }

/// Actor 15 casts Move, with no target.
void IzumoInteriors_ShowMove(void) {
  BattleEffect_LoadActionEffectResources(140, 1);
  FieldPsynergy_SetupActorPair(15, -1);
  FIELD_AT(Ram_HeapSlots->effectWork, HandlerFn, 36) =
      IzumoInteriors_FindNoMoveTarget;
  FIELD_AT(Ram_HeapSlots->effectWork, u8, 32) = 1;
  EventObject_Initialize();
  Event_Wait(10);
  FieldPsynergy_RunHandler(0);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

void IzumoInteriors_GiveUzumeTheDoll(void) {
  s32 t21, t31;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAhCouldIt);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 144, 536);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Event_Wait(15);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_OpenMessage(15, 0);
  if (!Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0)) {
    Event_SetMessageId(((s32)&MsgAhCouldIt + 1));
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId(((s32)&MsgAhCouldIt + 2));
    Event_ShowMessageAndWait(15, 0);
  }
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
  Event_SetMessageId((s32)&MsgAgainYouHave);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Event_ShowMessageAndWait(15, 0);
  BattleEffect_StartArcEffect();
  t21 = ((s32)Actor_GetById(15));
  BattleEffect_SetObjectUpdateMode(t21, 2);
  Sound_Play(SE_ASCEND);
  Event_Wait(60);
  Actor_SetPosition(24, 144, 520);
  Actor_Jump(24, 6, 0);
  Actor_SetDestination(24, 160, 520);
  Actor_WaitForMoveById(24);
  Actor_ArmCallback(24, FACING_SOUTH, 0);
  Event_Wait(60);
  t31 = ((s32)Actor_GetById(15));
  BattleEffect_SetObjectUpdateMode(t31, 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Event_Wait(30);
  Event_Wait(30);
  Actor_FaceActorById(15, 24, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(30);
  Actor_SetModeById(15, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_TurnToFaceEachOther(15, 24, 0);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  Actor_Jump(24, 4, 20);
  Event_Wait(60);
  Actor_FaceActorById(15, gMainRoutineWork.currentOwner, 0);
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(24, 2, 20);
  Djinn_JoinParty(24, 2, 11);
  Event_End();
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 15, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(15, 2);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_UZUME_HAS_THE_DOLL);
  PartyInventory_Remove(0x1c3);
  Event_End();
}

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  if (gMainRoutineWork.entrance != 7)
    Event_ClearFlag(FLAG_AREA_TEMP_50);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      !GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == 7) {
    Event_PlaySound(MUS_007);
    Actor_SetPosition(22, 144, 568);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == 7 && GameFlag_Get(FLAG_AREA_TEMP_50)) {
    Event_PlaySound(MUS_007);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 5 && !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    Actor_SetAnimation(17, 5);
    Actor_SetAttachedEffect(17,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  }
  if (gMainRoutineWork.entrance == 7 && !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    struct Actor *object;

    Actor_SetAnimation(9, 5);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    object = Actor_GetById(11);
    object->scaleX = -Actor_GetById(11)->scaleX;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern s32 gIzumoInteriors_BounceScript[];

DEFINE_G_DATA_02002324(gIzumoInteriors_FirstPacerScript,
                       IzumoInteriors_IsFirstPacerClear, 0x700000, 0x880000,
                       0x8000, 0)

DEFINE_G_DATA_02002324(gIzumoInteriors_SecondPacerScript,
                       IzumoInteriors_IsSecondPacerClear, 0xb00000, 0x980000, 0,
                       0x8000)

// clang-format off
s32 gIzumoInteriors_RockingScript[] = {
    22, 30, 129, 39, 0, (s32)0x80010000,
    23, 9, -0x4000, 23, 10, 0x2000,
    0, 2, 23, 9, 0x2000, 23,
    10, -4096, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 40, 23, 9, -0x4000, 23,
    10, 0x2000, 0, 2, 23, 9,
    0x2000, 23, 10, -4096, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 5, 23, 9,
    -0x4000, 23, 10, 0x2000, 0, 2,
    23, 9, 0x2000, 23, 10, -4096,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 30,
    13, 0xffff, (s32)0xc0010000, 17, 22, 30,
    131, 22, 16, 0x1999, 22, 15,
    0x4ccc, 39, 2, (s32)0x80010000, 4, 0x1080000,
    0, 0x780000, 1, 0, 60, 4,
    0x1080000, 0, 0x600000, 1, 0, 60,
    13, 0xffff, (s32)0xc0010000, -1,
};

s32 gIzumoInteriors_BounceScript[] = {
    SC_SET(SW_COLLISION, 131),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(60),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(10),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_GRAVITY, 0x1999),
    SC_SET(SW_SPEED, 0x6666),
    SC_ARG(0),
    SC_LABEL(1),
    SC_SET(SW_Y, PIXELS(4)),
    SC_WAIT(30),
    SC_END,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(170),
    SCENE_EXIT_IF(1, 171, 2, -1),
    SCENE_EXIT_IF(2, 171, 3, -1),
    SCENE_EXIT_IF(3, 171, 4, -1),
    SCENE_EXIT_IF(4, 171, 5, -1),
    SCENE_EXIT_IF(5, 171, 6, -1),
    SCENE_EXIT_IF(6, 171, 7, -1),
    SCENE_EXIT_IF(7, 171, 8, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gIzumoInteriors_ActorPlacements[] = {
    {120, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(120), FACING_SOUTHEAST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(152), FACING_NORTHWEST, 1, 0},
    {120, CONDITION_ALWAYS, 2, PIXELS(88), 0, PIXELS(88), FACING_EAST, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(96), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(80), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(84), FACING_SOUTHEAST, 1, 0},
    {125, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(136), FACING_NORTHWEST, 1, 0},
    {125, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(120), FACING_SOUTHEAST, 1, 0},
    {32, CONDITION_ALWAYS, (s32)gIzumoInteriors_BounceScript, PIXELS(352), 0, PIXELS(328), FACING_SOUTH, 1, 0},
    {121, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(362), FACING_SOUTH, 1, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(108), 0, PIXELS(320), FACING_SOUTHEAST, 1, 0},
    {119, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {123, CONDITION_ALWAYS, 2, PIXELS(632), 0, PIXELS(88), FACING_EAST, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(328), FACING_SOUTH, 0, 0},
    {127, CONDITION_ALWAYS, (s32)gIzumoInteriors_RockingScript, PIXELS(504), 0, PIXELS(360), FACING_SOUTHWEST, 2, 0},
    {127, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(392), FACING_NORTHWEST, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoInteriors_SusasHouseActorPlacements[] = {
    {30, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(520), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gIzumoInteriors_FirstPacerScript, PIXELS(136), 0, PIXELS(624), FACING_EAST, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gIzumoInteriors_SecondPacerScript, PIXELS(152), 0, PIXELS(624), FACING_WEST, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(90), 0, PIXELS(622), FACING_WEST, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(198), 0, PIXELS(622), FACING_EAST, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain[] = {
    {121, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(362), FACING_SOUTH, 1, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(108), 0, PIXELS(320), FACING_SOUTHEAST, 1, 0},
    {119, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {123, CONDITION_ALWAYS, 2, PIXELS(632), 0, PIXELS(88), FACING_EAST, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(328), FACING_SOUTH, 0, 0},
    {127, CONDITION_ALWAYS, (s32)gIzumoInteriors_RockingScript, PIXELS(600), 0, PIXELS(360), FACING_WEST, 2, 0},
    {127, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(376), FACING_NORTHEAST, 1, 0},
    {30, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(520), FACING_SOUTH, 2, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(624), FACING_EAST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(624), FACING_WEST, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(616), FACING_NORTHEAST, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(616), FACING_NORTHWEST, 1, 0},
    {32, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(520), FACING_SOUTH, 1, 0},
    {29, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(552), FACING_NORTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {245, 0x63, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumoInteriors_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(IzumoInteriors_KushinadaPleads)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOkuniKushinadaAnd)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIfLadyUzume)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMSoRelieved)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhatIWant)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAWhileBack)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgThatDragonsbaneSeems)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgICanTFind)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIAmOld)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheSerpentCan)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToMerchant)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOneDayKushinada)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIfIWere)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToHealer)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgSniffHowCould)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgWhenIGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSusaSBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIOnlySuggested)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAccordingToLegend)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgOurGuardianSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgThereSNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgThePowerThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgItWasThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgThingsLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWhatDoesThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIfOnlyI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIDonTWantThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgNobodyWantsTo)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoInteriors_SusasHouseTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IzumoInteriors_AskAfterSusa)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgThereSNoPoint)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAttacked)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAte)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTheSerpentLives)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     EVENT(IzumoInteriors_ReadPrayerForSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgOkuniSSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgBeforeTheSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgLegendHasIt)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoInteriors_TriggersOnceTheSerpentIsSlain[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 22, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoInteriors_HeroesComeFromTheEast)},
    {TRIGGER_STEP, 23, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoInteriors_HeroesComeFromTheWest)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToMerchant)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyGoodsAre)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAsLadyUzume)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToHealer)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNowThatTheSerpent)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouReA)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToUzume)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     EVENT(IzumoInteriors_AskAboutMtMikage)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgSupposedlyTheDoll)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeHas)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeSNew)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSomedaySusaAnd)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIMSureYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIDLoveTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgThisMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfBriggsS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIUnderstandNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgIThankThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgMaybeNowKushinada)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgThatDragonWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1039, CONDITION_ALWAYS,
     EVENT(IzumoInteriors_ReadUzumesMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLadyUzume)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhatSort)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgIHeardThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIDonTKnowWhatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgIfSDescendants)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgIMNo)},
    {0x1c314, 15, FLAG_UZUME_HAS_THE_DOLL,
     EVENT(IzumoInteriors_GiveUzumeTheDoll)},
    {TRIGGER_TABLE_END},
};
