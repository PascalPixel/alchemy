/// @file
/// The houses of Champa, the village of Briggs's people on the cliffs of the
/// Eastern Sea, as SceneId_Ov683, with the inn and the artifact shop; the
/// same scene entered from the islets at entrances 8 and 9 is a house on an
/// islet.
///
/// Until Briggs sails from Alhafra in the boat he bought
/// (FLAG_BRIGGS_SAILED_FROM_ALHAFRA) the village is hungry, its fish gone
/// since the tidal wave; after, the people talk of the jewels he brought home.

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
#include "system/ram_buffer.h"
#include "version.h"
#include "event_work.h"
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
  /// ★ Named from the game: "We don't need to live off fish anymore, so we
  /// don't need any fishermen, do we?" (MsgWeDonT)
  ACTOR_RAFT_FISHERMAN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Everyone in Champa is poor, but the poorest of
  /// us all live on rafts..." (MsgEveryoneInChampa)
  ACTOR_RAFT_DWELLER,
  /// ★ Named from the game: "When my father heads out to sea, he takes our
  /// home with him" (MsgWhenMyFather)
  ACTOR_FISHERMANS_CHILD,
  /// ★ Named from the game: "My dad went out to fish, but there were no fish
  /// to be caught" (MsgMyDadWent)
  ACTOR_FISHERMANS_YOUNGER_CHILD,
  /// ★ Named from the game: "The tidal wave was terrible... It... It nearly
  /// swallowed me whole!" (MsgTheTidalWave)
  ACTOR_WAVE_SURVIVOR,
  /// ★ Named from the game: "It looked to me like the wave came from the Sea
  /// of Time." (MsgItLookedTo)
  ACTOR_SEA_OF_TIME_SAILOR,
  /// ★ Named from the game: "Most of the sailors here live in the cliff
  /// dwellings, but I don't like those caves." (MsgMostOfThe)
  ACTOR_CAVE_SHUNNING_SAILOR,
  /// ★ Named from the game: "I've been wondering about finding a chief for
  /// Champa, since we don't have one." (MsgIVeBeenWondering)
  ACTOR_ELDER_SEEKING_A_CHIEF,
  /// ★ Named from the game: "We village elders have guided Champa for so many
  /// years..." (MsgWeVillageElders)
  ACTOR_ELDER,
  /// ★ Named from the game: "The ocean has been a lot warmer lately, and the
  /// fish population is decreasing." (MsgTheOceanHas)
  ACTOR_WARM_OCEAN_WATCHER,
  /// ★ Named from the game: "I've heard rumors of a terrible tidal wave
  /// striking the lands south of us." (MsgIVeHeardRumors)
  ACTOR_TIDAL_WAVE_RUMORMONGER,
  /// ★ Named from the game: "I have some lovely artifacts, but it's only
  /// because I've been to the Ankohl Ruins." (MsgIHaveSome)
  ACTOR_ARTIFACT_MERCHANT,
  /// ★ Named from the game: "If you go to the Ankohl Ruins, you might find
  /// some nice artifacts for yourself." (MsgIfYouGo)
  ACTOR_ANKOHL_ADVISER,
  /// ★ Named from the game: "Listen, if you came to Champa looking for good
  /// seafood, you came to the wrong place." (MsgListenIfYou)
  ACTOR_INNKEEPER,
  /// ★ Named from the game: "We used to do good trade with an island east of
  /// us." (MsgWeUsedTo)
  ACTOR_FISH_TRADER
};

/// The islet house's one actor.
/// ★ Named from the game: "Did you know Indra moved?" (MsgDidYouKnow)
enum ChampaIsletHouseActorId { ACTOR_INDRA_NEIGHBOR = ACTOR_FIRST_PLACED };

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Shop_Run(s32 a, s32 b);

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gChampaInteriors_IsletHouseActorPlacements;
  return gChampaInteriors_ActorPlacements;
}

/// Talking to the artifact merchant across his counter opens his shop.
void ChampaInteriors_OpenArtifactShop(s32 actor) { Shop_Run(25, actor); }

/// Talking to the innkeeper across his counter opens the inn.
void ChampaInteriors_OpenInn(s32 actor) { Inn_CheckIn(10, actor); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gChampaInteriors_IsletHouseTriggers;
  if (GameFlag_Get(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampaInteriors_TriggersOnceBriggsSailed;
  return gChampaInteriors_Triggers;
}

s32 Scene_Initialize(void) {
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
#if !TLA_PATCH_1
  s32 value = 0x201;
#else
  s32 value = 0x209;
#endif

  *effect = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    Scene_FadeInAndClearFlag();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

DEFINE_SCENE_FADE_IN_AND_CLEAR_FLAG(Scene_FadeInAndClearFlag)

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 416, 0, 664, FACING_NORTH, 0, 288, 536, 528, 696, 0},
    {9, CONDITION_ALWAYS, 608, 0, 664, FACING_NORTH, 0, 488, 528, 728, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(183),
    SCENE_EXIT_IF(1, 182, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 182, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 182, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 182, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 182, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 182, 6, CONDITION_ALWAYS),
    SCENE_EXIT(8, 134, 11),
    SCENE_EXIT(9, 131, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gChampaInteriors_ActorPlacements[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(72), FACING_SOUTH, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(88), FACING_EAST, 0, 0},
    {136, CONDITION_ALWAYS, 2, PIXELS(144), 0, PIXELS(136),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(104), FACING_EAST, 0, 0},
    {195, CONDITION_ALWAYS, 1, PIXELS(304), 0, PIXELS(72), FACING_EAST, 1, 0},
    {195, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(112),
     FACING_NORTH + FACING_STEP, 1, 0},
    {195, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(88), FACING_WEST, 0, 0},
    {112, CONDITION_ALWAYS, 2, PIXELS(152), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(336), FACING_WEST, 1, 0},
    {114, CONDITION_ALWAYS, 2, PIXELS(344), 0, PIXELS(360), FACING_NORTH, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(336), FACING_EAST, 1, 0},
    {111, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(592),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(584), FACING_WEST, 1, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(80),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {209, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(91), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gChampaInteriors_IsletHouseActorPlacements[] = {
    {74, CONDITION_ALWAYS, 3, PIXELS(608), 0, PIXELS(612), FACING_SOUTH, 0, 0},
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
    {0x8400, ACTOR_FISHERMANS_YOUNGER_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgMyDadWent)},
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
    {0xc400, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     EVENT(ChampaInteriors_OpenArtifactShop)},
    {TRIGGER_TALK, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     MSGNO(MsgIHaveSome)},
    {TRIGGER_TALK, ACTOR_ANKOHL_ADVISER, CONDITION_ALWAYS, MSGNO(MsgIfYouGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgOurProductsCost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANKOHL_ADVISER,
     CONDITION_ALWAYS, MSGNO(MsgTheStuffIn)},
    {0xc400, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(ChampaInteriors_OpenInn)},
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
    {0x8400, ACTOR_FISHERMANS_YOUNGER_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgWhenBriggsReturned)},
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
    {0xc400, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     EVENT(ChampaInteriors_OpenArtifactShop)},
    {TRIGGER_TALK, ACTOR_ARTIFACT_MERCHANT, CONDITION_ALWAYS,
     MSGNO(MsgTheJewelsBriggs)},
    {TRIGGER_TALK, ACTOR_ANKOHL_ADVISER, CONDITION_ALWAYS,
     MSGNO(MsgThanksToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARTIFACT_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgBriggsSMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANKOHL_ADVISER,
     CONDITION_ALWAYS, MSGNO(MsgNowSThe)},
    {0xc400, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(ChampaInteriors_OpenInn)},
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
