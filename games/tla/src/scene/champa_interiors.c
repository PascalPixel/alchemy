/// @file
/// The houses of Champa, the village of Briggs's people on the cliffs of the
/// Eastern Sea, as SceneId_ChampaInteriors, with the inn and the artifact shop;
/// the same scene entered from the islets at entrances 8 and 9 is a house on an
/// islet.
///
/// Until Briggs sails from Alhafra in the boat he bought
/// (FLAG_BRIGGS_SAILED_FROM_ALHAFRA) the village is hungry, its fish gone
/// since the tidal wave; after, the people talk of the jewels he brought home.

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
#include "system/ram_buffer.h"
#include "version.h"
#include "field/field_sequence_event.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "flags.h"
#include "module/scene/scene.h"

extern struct ActorPlacement gChampaInteriors_ActorPlacements[];
extern struct ActorPlacement gChampaInteriors_IsletHouseActorPlacements[];
extern struct Trigger gChampaInteriors_Triggers[];
extern struct Trigger gChampaInteriors_TriggersOnceBriggsSailed[];
extern struct Trigger gChampaInteriors_IsletHouseTriggers[];

void Scene_FadeInAndClearFlag(void);

/// The houses' actors, numbering the rows of gChampaInteriors_ActorPlacements.
enum ChampaInteriorsActorId {
  ACTOR_RAFT_FISHERMAN = ACTOR_FIRST_PLACED,
  ACTOR_RAFT_DWELLER,
  ACTOR_FISHERMANS_CHILD,
  ACTOR_FISHERMANS_YOUNGER_CHILD,
  ACTOR_WAVE_SURVIVOR,
  ACTOR_SEA_OF_TIME_SAILOR,
  ACTOR_CAVE_SHUNNING_SAILOR,
  ACTOR_ELDER_SEEKING_A_CHIEF,
  ACTOR_ELDER,
  ACTOR_WARM_OCEAN_WATCHER,
  ACTOR_TIDAL_WAVE_RUMORMONGER,
  ACTOR_ARTIFACT_MERCHANT,
  ACTOR_ANKOHL_ADVISER,
  ACTOR_INNKEEPER,
  ACTOR_FISH_TRADER
};

/// The islet house's one actor.
enum ChampaIsletHouseActorId { ACTOR_INDRA_NEIGHBOR = ACTOR_FIRST_PLACED };

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Shop_Run(s32 a, s32 b);

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_ChampaInteriors &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gChampaInteriors_IsletHouseActorPlacements;
  return gChampaInteriors_ActorPlacements;
}

/// Talking to the artifact merchant across his counter opens his shop.
void ChampaInteriors_OpenArtifactShop(s32 actor) { Shop_Run(25, actor); }

/// Talking to the innkeeper across his counter opens the inn.
void ChampaInteriors_OpenInn(s32 actor) { Inn_CheckIn(10, actor); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_ChampaInteriors &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gChampaInteriors_IsletHouseTriggers;
  if (GameFlag_Get(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampaInteriors_TriggersOnceBriggsSailed;
  return gChampaInteriors_Triggers;
}

s32 Scene_Initialize(void) {
  s32 *effect = &gFieldWork->transition;
#if !TLA_PATCH_1
  s32 value = 0x201;
#else
  s32 value = 0x209;
#endif

  *effect = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_ChampaInteriors &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    Scene_FadeInAndClearFlag();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/scene_fade_in_and_clear_flag.h"

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 416, 0, 664, FACING_NORTH, 0, 288, 536, 528, 696, 0},
    {9, CONDITION_ALWAYS, 608, 0, 664, FACING_NORTH, 0, 488, 528, 728, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ChampaInteriors),
    SCENE_EXIT_UNLESS(1, Champa1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Champa1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Champa1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Champa1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Champa1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Champa1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT(8, Islets4, 11),
    SCENE_EXIT(9, Islets1, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gChampaInteriors_ActorPlacements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 72,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 88,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 144, 0, 136,
                    FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(138, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 104,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 72,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 112,
                    FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 88,
                    FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 152, 0, 360,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 336,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 344, 0, 360,
                    FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(111, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 592,
                    FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 584,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 80,
                    FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 91,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gChampaInteriors_IsletHouseActorPlacements[] = {
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 608, 0,
                    612, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gChampaInteriors_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_RAFT_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgThereSNot)},
    {TRIGGER_TALK, ACTOR_RAFT_DWELLER, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneInChampa)},
    {TRIGGER_TALK, ACTOR_FISHERMANS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgWhenMyFather)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_FISHERMANS_YOUNGER_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgMyDadWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAFT_FISHERMAN,
     CONDITION_ALWAYS, MSGNO(MsgTheseRaftsAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAFT_DWELLER, CONDITION_ALWAYS,
     MSGNO(MsgIfDadHadn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMANS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgIHopeDad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMANS_YOUNGER_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgFatherWorriesSo)},
    {TRIGGER_TALK, ACTOR_WAVE_SURVIVOR, CONDITION_ALWAYS,
     MSGNO(MsgTheTidalWave)},
    {TRIGGER_TALK, ACTOR_SEA_OF_TIME_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgItLookedTo)},
    {TRIGGER_TALK, ACTOR_CAVE_SHUNNING_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgMostOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAVE_SURVIVOR,
     CONDITION_ALWAYS, MSGNO(MsgTheFishAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OF_TIME_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgSomethingMustHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAVE_SHUNNING_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgYouHaveTo)},
    {TRIGGER_TALK, ACTOR_ELDER_SEEKING_A_CHIEF, CONDITION_ALWAYS,
     MSGNO(MsgWeVeSeen)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgAGroupOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER_SEEKING_A_CHIEF,
     CONDITION_ALWAYS, MSGNO(MsgMoreVisitorsMeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgNowThatI)},
    {TRIGGER_TALK, ACTOR_WARM_OCEAN_WATCHER, CONDITION_ALWAYS,
     MSGNO(MsgTheOceanHas)},
    {TRIGGER_TALK, ACTOR_TIDAL_WAVE_RUMORMONGER, CONDITION_ALWAYS,
     MSGNO(MsgIVeHeardRumors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARM_OCEAN_WATCHER,
     CONDITION_ALWAYS, MSGNO(MsgWeChampaRely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TIDAL_WAVE_RUMORMONGER,
     CONDITION_ALWAYS, MSGNO(MsgAllTheSailors)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, EVENT(ChampaInteriors_OpenArtifactShop)},
    {TRIGGER_TALK, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     MSGNO(MsgIHaveSome)},
    {TRIGGER_TALK, ACTOR_ANKOHL_ADVISER, CONDITION_ALWAYS, MSGNO(MsgIfYouGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgOurProductsCost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANKOHL_ADVISER,
     CONDITION_ALWAYS, MSGNO(MsgTheStuffIn)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(ChampaInteriors_OpenInn)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgListenIfYou)},
    {TRIGGER_TALK, ACTOR_FISH_TRADER, CONDITION_ALWAYS, MSGNO(MsgWeUsedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgItSA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISH_TRADER, CONDITION_ALWAYS,
     MSGNO(MsgWatchingOurCatches)},
    {TRIGGER_TABLE_END},
};

struct Trigger gChampaInteriors_TriggersOnceBriggsSailed[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_RAFT_FISHERMAN, CONDITION_ALWAYS, MSGNO(MsgWeDonT)},
    {TRIGGER_TALK, ACTOR_RAFT_DWELLER, CONDITION_ALWAYS, MSGNO(MsgItSTrue)},
    {TRIGGER_TALK, ACTOR_FISHERMANS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgFatherSActing)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_FISHERMANS_YOUNGER_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgWhenBriggsReturned)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAFT_FISHERMAN,
     CONDITION_ALWAYS, MSGNO(MsgIDRather)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAFT_DWELLER, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMANS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgIfFatherSits)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISHERMANS_YOUNGER_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgQuittingTheirJobs)},
    {TRIGGER_TALK, ACTOR_WAVE_SURVIVOR, CONDITION_ALWAYS, MSGNO(MsgIHearThat)},
    {TRIGGER_TALK, ACTOR_SEA_OF_TIME_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgWhileIM)},
    {TRIGGER_TALK, ACTOR_CAVE_SHUNNING_SAILOR, CONDITION_ALWAYS,
     MSGNO(MsgBriggsReallySaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAVE_SURVIVOR,
     CONDITION_ALWAYS, MSGNO(MsgTheNerveOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEA_OF_TIME_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgIShouldJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAVE_SHUNNING_SAILOR,
     CONDITION_ALWAYS, MSGNO(MsgIRefuseTo)},
    {TRIGGER_TALK, ACTOR_ELDER_SEEKING_A_CHIEF, CONDITION_ALWAYS,
     MSGNO(MsgIVeBeenWondering)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgWeVillageElders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER_SEEKING_A_CHIEF,
     CONDITION_ALWAYS, MSGNO(MsgANewImage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgObservingBriggsRecently)},
    {TRIGGER_TALK, ACTOR_WARM_OCEAN_WATCHER, CONDITION_ALWAYS,
     MSGNO(MsgItSeemsThe)},
    {TRIGGER_TALK, ACTOR_TIDAL_WAVE_RUMORMONGER, CONDITION_ALWAYS,
     MSGNO(MsgIForOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARM_OCEAN_WATCHER,
     CONDITION_ALWAYS, MSGNO(MsgSureBriggsSaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TIDAL_WAVE_RUMORMONGER,
     CONDITION_ALWAYS, MSGNO(MsgSupportingAnEntire)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, EVENT(ChampaInteriors_OpenArtifactShop)},
    {TRIGGER_TALK, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     MSGNO(MsgTheJewelsBriggs)},
    {TRIGGER_TALK, ACTOR_ANKOHL_ADVISER, CONDITION_ALWAYS,
     MSGNO(MsgThanksToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgBriggsSMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANKOHL_ADVISER,
     CONDITION_ALWAYS, MSGNO(MsgNowSThe)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(ChampaInteriors_OpenInn)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgWeStillHaven)},
    {TRIGGER_TALK, ACTOR_FISH_TRADER, CONDITION_ALWAYS, MSGNO(MsgItSNice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgMostPeopleHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FISH_TRADER, CONDITION_ALWAYS,
     MSGNO(MsgTheOnlyWay)},
    {TRIGGER_TABLE_END},
};

struct Trigger gChampaInteriors_IsletHouseTriggers[] = {
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, ACTOR_INDRA_NEIGHBOR, CONDITION_ALWAYS,
     MSGNO(MsgDidYouKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INDRA_NEIGHBOR,
     CONDITION_ALWAYS, MSGNO(MsgIMSoGrateful)},
    {TRIGGER_TABLE_END},
};
