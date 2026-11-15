/// @file
/// Mikasalla's houses: the inn, where Galahad waits in a linked game, and, as a
/// second scene, the sanctums of Mikasalla, the Apojii Islands and Yallam.

#include "shared/tables.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/// Set once Galahad has met the party and warned it of Isaac's enemies.
#define FLAG_MIKASALLA_GALAHAD_MET 0xa39
/// Set if the party told Galahad it knows Isaac.
#define FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC FLAG_AREA_TEMP_40
/// Set as Galahad meets the party, for this visit: he stays in the inn until
/// the party leaves the area.
#define FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT FLAG_AREA_TEMP_41

/// Mikasalla's houses and inn (MikasallaInteriorsHouses), by the rows of
/// gMikasallaInteriors_InnPlacements.
enum MikasallaInteriorsActorId {
  /// The family of the late explorer: his son, his widow, and the grandson who
  /// asks whether the leader is an explorer too.
  ACTOR_EXPLORERS_SON = ACTOR_FIRST_PLACED,
  ACTOR_EXPLORERS_WIDOW,
  ACTOR_EXPLORERS_GRANDSON,
  /// The old man who has tried to climb Air's Rock, and his wife.
  ACTOR_AIRS_ROCK_CLIMBER,
  ACTOR_CLIMBERS_WIFE,
  ACTOR_MERCHANT,
  /// A man at the inn: "There aren't any towns between here and Garoh."
  ACTOR_MAN_ON_THE_ROAD_TO_GAROH,
  ACTOR_INNKEEPER,
  /// An old couple by the fire, who talk of Tundaria.
  ACTOR_OLD_MAN_ON_TUNDARIA,
  ACTOR_OLD_WOMAN_BY_THE_FIRE,
  /// Galahad, in the inn in a linked game.
  ACTOR_GALAHAD
};

/// The sanctums (MikasallaInteriorsSanctum): Mikasalla's, by entrance 1, and
/// those of the Apojii Islands, by entrance 5, and Yallam, by entrance 4, which
/// this scene also holds; by the rows of gMikasallaInteriors_SanctumPlacements.
enum MikasallaInteriorsSanctumActorId {
  ACTOR_MIKASALLA_HEALER = ACTOR_FIRST_PLACED,
  ACTOR_APOJII_HEALER,
  /// Two Apojii islanders tired of their island paradise.
  ACTOR_ISLANDER_SICK_OF_PARADISE,
  ACTOR_ISLANDER_LONGING_FOR_LAND,
  ACTOR_YALLAM_HEALER,
  /// Blank actors on the counters of Mikasalla's and the Apojii sanctums, so
  /// the healers answer from across them.
  ACTOR_MIKASALLA_COUNTER,
  ACTOR_APOJII_COUNTER
};

/// Mikasalla's shop and inn, as Shop_Run and Inn_CheckIn number them.
enum { SHOP_MIKASALLA = 13, MIKASALLA_INN = 4 };

extern struct ActorPlacement gMikasallaInteriors_InnPlacements[];
extern struct ActorPlacement gMikasallaInteriors_SanctumPlacements[];
extern struct Trigger gMikasallaInteriors_InnTriggers[];
extern struct Trigger gMikasallaInteriors_SanctumTriggers[];

void Shop_Run();

void Scene_FadeInAndClearFlag(void);

extern u32 gMikasallaInteriors_ExplorersWidowPaces[];
extern u32 gMikasallaInteriors_ClimbersWifePaces[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MIKASALLA_INTERIORS_SANCTUM)
    return gMikasallaInteriors_SanctumPlacements;
  return gMikasallaInteriors_InnPlacements;
}

void Event_Start();
void Event_End();

/// Mind Read on Mikasalla's sanctum counter: the healer wonders about
/// werewolves beyond Garoh.
s32 MikasallaInteriors_ReadHealersMind(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors18);
  // "I wonder if there are werewolves beyond those in Garoh... I also..."
  Event_ShowMessage(ACTOR_MIKASALLA_HEALER, 0);
  Event_End();
  return 0;
}

/// Mind Read on the Apojii sanctum's counter: the healer thinks of the
/// islanders' complaints.
s32 MikasallaInteriors_ReadApojiiHealersMind(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors32);
  // "Whenever visitors come out, the islanders start to complain about..."
  Event_ShowMessage(ACTOR_APOJII_HEALER, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MIKASALLA_INTERIORS_SANCTUM)
    return gMikasallaInteriors_SanctumTriggers;
  return gMikasallaInteriors_InnTriggers;
}

void Sanctum_Run();

/// "Hey, mister! Are you an explorer?"
void MikasallaInteriors_AskAreYouExplorer(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03);
  // "Hey, mister! Are you an explorer?"
  Event_OpenMessage(ACTOR_EXPLORERS_GRANDSON, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03 + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03 + 2);
  }
  Event_ShowMessage(ACTOR_EXPLORERS_GRANDSON, 0);
  Event_End();
}

/// Mikasalla's merchant sells across the counter (stock row 13), and otherwise
/// says the shop now sells armor too.
void MikasallaInteriors_TalkToMerchant(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Shop_Run(SHOP_MIKASALLA, actor);
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors11);
    // "We only have a small shop here. I used to specialize in items... But..."
    Event_ShowMessage(actor, 0);
  }
}

/// Mikasalla's innkeeper takes the party in across the counter (inn 4), and
/// otherwise bids it rest.
/// "You've come a long way, friends. Stay a while, and rest your feet."
void MikasallaInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Inn_CheckIn(MIKASALLA_INN, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors14);
    // "You've come a long way, friends. Stay a while, and rest your feet."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Mikasalla's healer heals across the counter, and otherwise asks whether the
/// party has been to Garoh.
/// "Have you been to Garoh?"
void MikasallaInteriors_TalkToHealer(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17);
    // "Have you been to Garoh?"
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

/// The Apojii Islands' healer heals across the counter, and otherwise talks of
/// the little island.
void MikasallaInteriors_TalkToApojiiHealer(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors29);
    // "This little island must seem quite boring to you sophisticated..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's healer heals across the counter, and otherwise talks of the
/// far-off shores.
void MikasallaInteriors_TalkToYallamHealer(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors27);
    // "Even out in these far-off shores, people need the services and hope..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x209;
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x20;
  if (gMainRoutineWork.next == SCENE_MIKASALLA_INTERIORS_HOUSES) {
    if (gMainRoutineWork.entrance == 5)
      Scene_FadeInAndClearFlag();
    else if (!Event_GetFlag(0x821) ||
             (Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET) &&
              !Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT)) ||
             Event_GetFlag(34))
      Actor_SetPosition(ACTOR_GALAHAD, 0, 0);
    else
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_GALAHAD), 4);
    return 0;
  }
  if (gMainRoutineWork.next == SCENE_MIKASALLA_INTERIORS_SANCTUM) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_Get(ACTOR_MIKASALLA_HEALER)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(ACTOR_MIKASALLA_HEALER, 2);
    } else if (entrance == 5) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
      Actor_Get(ACTOR_APOJII_HEALER)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(ACTOR_APOJII_HEALER, 2);
    } else if (entrance == 4) {
      Event_ClearFlag(FLAG_SHOW_AREA_NAME);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Arriving by the inn's entrance 5, the screen fades in and flag 0x12f is
/// cleared.
#include "shared/scene_fade_in_and_clear_flag.h"

void MikasallaInteriors_MeetGalahad(void);

/// Galahad, in the inn in a linked game, tells of Isaac's enemies once he has
/// met the party.
void MikasallaInteriors_TalkToGalahad(void) {
  if (Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET)) {
    FieldEvent_SetMessageId(Event_GetFlag(FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC)
                                ? (s32)MsgMikasallaInteriors20
                                : (s32)MsgMikasallaInteriors21);
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  } else {
    MikasallaInteriors_MeetGalahad();
  }
}

/// Mind Read on Galahad: before he has met the party he introduces himself;
/// after, he fears for Isaac.
void MikasallaInteriors_MindReadGalahad(void) {
  if (!Event_GetFlag(FLAG_MIKASALLA_GALAHAD_MET)) {
    MikasallaInteriors_MeetGalahad();
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors22);
    // "I fear what might happen if the three find Isaac before I can warn..."
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  }
}

/// Galahad introduces himself, asks whether the party knows Isaac, and warns
/// that the three gladiators Isaac beat are seeking revenge.
void MikasallaInteriors_MeetGalahad(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors19);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_GALAHAD, gMainRoutineWork.leaderActorId, 0);
  // "Forgive my manners..."
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetAnimation(ACTOR_GALAHAD, 4);
  FieldEvent_Wait(15);
  // "No, please... Do not fear me."
  Event_ShowMessage(ACTOR_GALAHAD, 0);
  Actor_ShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  // "I am the warrior Galahad. I am looking for someone, and I need your..."
  Event_OpenMessage(ACTOR_GALAHAD, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    // "You do? I must warn Sir Isaac that he is in danger most grievous......"
    Event_ShowMessage(ACTOR_GALAHAD, 0);
    Event_SetFlag(FLAG_MIKASALLA_TOLD_GALAHAD_OF_ISAAC);
  } else {
    gFieldWork->message += 1;
    // "A pity... The one I seek won a battle at Colosso, in Tolbi. He..."
    Event_ShowMessage(ACTOR_GALAHAD, 0);
  }
  Event_SetFlag(FLAG_MIKASALLA_GALAHAD_MET);
  Event_SetFlag(FLAG_MIKASALLA_GALAHAD_MET_THIS_VISIT);
  Event_End();
}

// clang-format off
/// Two women pace the inn: the explorer's widow, whose father-in-law and husband
/// explored, and the wife of the man who keeps trying to climb Air's Rock.
u32 gMikasallaInteriors_ExplorersWidowPaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.1),                                                
      SC_SPEED(0.3),                                                
      SC_LABEL(1),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(600, 0, 88),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(616, 0, 104),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

/// The climber's wife's script: she paces her house.
u32 gMikasallaInteriors_ClimbersWifePaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.1),                                                
      SC_SPEED(0.3),                                                
      SC_LABEL(1),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(360, 0, 88),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_WAIT(20),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(376, 0, 104),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_ANIMATION(1),                                                               
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
    SCENE_EXITS(MikasallaInteriorsHouses),
    SCENE_EXIT_UNLESS(1, Mikasalla1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Mikasalla1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Mikasalla1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Mikasalla1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Islets2, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaInteriorsSanctum),
    SCENE_EXIT_UNLESS(1, Mikasalla1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ApojiiIslands1, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Yallam1, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Mikasalla's houses and inn (enum MikasallaInteriorsActorId).
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

/// The three sanctums (enum MikasallaInteriorsSanctumActorId).
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
/// The houses' and inn's talk and Mind Read, Galahad, and what is cooking.
struct Trigger gMikasallaInteriors_InnTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_EXPLORERS_SON, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors01)},
    {TRIGGER_TALK, ACTOR_EXPLORERS_WIDOW, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors02)},
    {TRIGGER_TALK, ACTOR_EXPLORERS_GRANDSON, CONDITION_ALWAYS, EVENT(MikasallaInteriors_AskAreYouExplorer)},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_CLIMBER, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors07)},
    {TRIGGER_TALK, ACTOR_CLIMBERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors08)},
    {TRIGGER_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToMerchant)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_ROAD_TO_GAROH, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors13)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors23)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_BY_THE_FIRE, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors24)},
    {TRIGGER_TALK, ACTOR_GALAHAD, CONDITION_ALWAYS, EVENT(MikasallaInteriors_TalkToGalahad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_SON, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_WIDOW, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EXPLORERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_CLIMBER, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CLIMBERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_ROAD_TO_GAROH, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BY_THE_FIRE, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GALAHAD, CONDITION_ALWAYS, EVENT(MikasallaInteriors_MindReadGalahad)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors35))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors36))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors37))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors38))},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The three sanctums' healers, the islanders and the counters' Mind Read.
struct Trigger gMikasallaInteriors_SanctumTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_MIKASALLA_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_APOJII_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkToApojiiHealer)},
    {TRIGGER_TALK, ACTOR_ISLANDER_SICK_OF_PARADISE, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors30)},
    {TRIGGER_TALK, ACTOR_ISLANDER_LONGING_FOR_LAND, CONDITION_ALWAYS,
     MSGNO(MsgMikasallaInteriors31)},
    {TRIGGER_TALK, ACTOR_YALLAM_HEALER, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkToYallamHealer)},
    {TRIGGER_TALK, ACTOR_MIKASALLA_COUNTER, CONDITION_ALWAYS,
     EVENT(MikasallaInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIKASALLA_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APOJII_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER_SICK_OF_PARADISE,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER_LONGING_FOR_LAND,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YALLAM_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors28)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIKASALLA_COUNTER,
     CONDITION_ALWAYS, EVENT(MikasallaInteriors_ReadHealersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APOJII_COUNTER,
     CONDITION_ALWAYS, EVENT(MikasallaInteriors_ReadApojiiHealersMind)},
    {TRIGGER_TABLE_END},
};
