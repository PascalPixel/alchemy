/// @file
/// Mikasalla's houses: the inn, where Galahad waits in a linked game, and the
/// sanctum, as two scenes of one overlay.

#include "shared/tables.h"
#include "flags.h"
#include "shared/actors.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "field/event_inline.h"
#include "module/scene/scene.h"
#include "objcmd.h"
#include "text/msg_ids.h"

extern struct ActorPlacement gMikasallaInteriors_InnPlacements[];
extern struct ActorPlacement gMikasallaInteriors_SanctumPlacements[];
extern struct Trigger gMikasallaInteriors_InnTriggers[];
extern struct Trigger gMikasallaInteriors_SanctumTriggers[];

void Shop_Run();

void MikasallaInteriors_FadeInAndClearFlag(void);

extern u32 gMikasallaInteriors_ExplorersWidowPaces[];
extern u32 gMikasallaInteriors_ClimbersWifePaces[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MikasallaInteriors2)
    return gMikasallaInteriors_SanctumPlacements;
  return gMikasallaInteriors_InnPlacements;
}

void Event_Start();
void Event_End();

/// "I wonder if there are werewolves beyond those in Garoh..."
s32 MikasallaInteriors_MindReadWerewolfWonder(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors18);
  Event_ShowMessage(8, 0);
  Event_End();
  return 0;
}

/// "Whenever visitors come out, the islanders start to complain about their
/// lives."
s32 MikasallaInteriors_MindReadIslanderComplaints(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors32);
  Event_ShowMessage(9, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MikasallaInteriors2)
    return gMikasallaInteriors_SanctumTriggers;
  return gMikasallaInteriors_InnTriggers;
}

void Sanctum_Run();

/// "Hey, mister! Are you an explorer?"
void MikasallaInteriors_AskAreYouExplorer(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03 + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03 + 2);
  }
  Event_ShowMessage(10, 0);
  Event_End();
}

/// Mikasalla's merchant sells across the counter (stock row 13), and otherwise
/// says the shop now sells armor too.
void MikasallaInteriors_TalkToMerchant(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Shop_Run(13, actor);
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors11);
    Event_ShowMessage(actor, 0);
  }
}

/// Mikasalla's innkeeper takes the party in across the counter (inn 4), and
/// otherwise bids it rest.
/// "You've come a long way, friends. Stay a while, and rest your feet."
void MikasallaInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Inn_CheckIn(4, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors14);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// A sanctum's healer heals across the counter, and otherwise asks whether the
/// party has been to Garoh.
/// "Have you been to Garoh?"
void MikasallaInteriors_AskBeenToGaroh(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 2);
    }
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// A sanctum's healer heals across the counter, and otherwise talks of the
/// island.
void MikasallaInteriors_TalkAboutIslandLife(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors29);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// A sanctum's healer heals across the counter, and otherwise talks of the
/// far-off shores.
void MikasallaInteriors_TalkAboutFarShores(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_MikasallaInteriors1) {
    if (gMainRoutineWork.entrance == 5)
      MikasallaInteriors_FadeInAndClearFlag();
    else if (!Event_GetFlag(0x821) ||
             (Event_GetFlag(0xa39) && !Event_GetFlag(FLAG_AREA_TEMP_41)) ||
             Event_GetFlag(34))
      Actor_SetPosition(18, 0, 0);
    else
      FieldActor_SetSpriteColor(Actor_Get(18), 4);
    return 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_MikasallaInteriors2) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_Get(8)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(8, 2);
    } else if (entrance == 5) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
      Actor_Get(9)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(9, 2);
    } else if (entrance == 4) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Arriving by the inn's entrance 5, the screen fades in and flag 0x12f is
/// cleared.
DEFINE_SCENE_FADE_IN_AND_CLEAR_FLAG(MikasallaInteriors_FadeInAndClearFlag)

void MikasallaInteriors_MeetGalahad(void);

/// Galahad, in the inn in a linked game, tells of Isaac's enemies once he has
/// met the party.
void MikasallaInteriors_TalkToGalahad(void) {
  if (Event_GetFlag(0xa39)) {
    FieldEvent_SetMessageId(Event_GetFlag(FLAG_AREA_TEMP_40)
                                ? (s32)MsgMikasallaInteriors20
                                : (s32)MsgMikasallaInteriors21);
    Event_ShowMessage(18, 0);
  } else {
    MikasallaInteriors_MeetGalahad();
  }
}

void MikasallaInteriors_MindReadGalahad(void) {
  if (!Event_GetFlag(0xa39)) {
    MikasallaInteriors_MeetGalahad();
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors22);
    Event_ShowMessage(18, 0);
  }
}

/// Galahad introduces himself, asks whether the party knows Isaac, and warns
/// that the three gladiators Isaac beat are seeking revenge (flags 0xa39 and
/// 0x341; 0x340 if the party says it knows him).
void MikasallaInteriors_MeetGalahad(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors19);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetAnimation(18, 4);
  FieldEvent_Wait(15);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Event_OpenMessage(18, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Event_ShowMessage(18, 0);
    Event_SetFlag(FLAG_AREA_TEMP_40);
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_ShowMessage(18, 0);
  }
  Event_SetFlag(0xa39);
  Event_SetFlag(FLAG_AREA_TEMP_41);
  Event_End();
}

// clang-format off
/// Two women pace the inn: the explorer's widow, whose father-in-law and husband
/// explored, and the wife of the man who keeps trying to climb Air's Rock.
u32 gMikasallaInteriors_ExplorersWidowPaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_SET(SW_ACCEL, 0x1999),                                                
      SC_SET(SW_SPEED, 0x4ccc),                                                
      SC_LABEL(1),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(PIXELS(600), 0, PIXELS(88)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(PIXELS(616), 0, PIXELS(104)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

u32 gMikasallaInteriors_ClimbersWifePaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_SET(SW_ACCEL, 0x1999),                                                
      SC_SET(SW_SPEED, 0x4ccc),                                                
      SC_LABEL(1),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(PIXELS(360), 0, PIXELS(88)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(PIXELS(376), 0, PIXELS(104)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ARG(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MikasallaInteriors1),
    SCENE_EXIT_UNLESS(1, Mikasalla1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Mikasalla1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Mikasalla1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Mikasalla1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Islets2, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaInteriors2),
    SCENE_EXIT_UNLESS(1, Mikasalla1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ApojiiIslands1, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Yallam1, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gMikasallaInteriors_InnPlacements[] = {
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 152, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, (s32)gMikasallaInteriors_ExplorersWidowPaces, 616, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(77, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 136, FACING_NORTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 120, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, (s32)gMikasallaInteriors_ClimbersWifePaces, 360, 0, 88, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(130, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 408, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 664, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(239, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMikasallaInteriors_SanctumPlacements[] = {
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 147, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 466, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 520, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 536, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 472, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct Trigger gMikasallaInteriors_InnTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(MikasallaInteriors_AskAreYouExplorer)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors07)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors08)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToMerchant)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors13)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors23)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors24)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToGalahad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT(MikasallaInteriors_MindReadGalahad)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors35))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors36))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors37))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors38))},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMikasallaInteriors_SanctumTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_AskBeenToGaroh)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkAboutIslandLife)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors30)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors31)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkAboutFarShores)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_AskBeenToGaroh)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors28)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_MindReadWerewolfWonder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_MindReadIslanderComplaints)},
    {TRIGGER_TABLE_END},
};
