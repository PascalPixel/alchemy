/// @file
/// The scene code of overlay 0x683, in ROM order.

#include "shared/actors.h"
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
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "version.h"
#include "event_work.h"
#include "scene_ids.h"
#include "event_inline.h"
#include "module/scene/champa_interiors.h"
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Shop_Run(s32 a, s32 b);

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gData_020003a8;
  return gData_02000228;
}

void Func_0200008c(s32 value) { Shop_Run(25, value); }

void Func_02000098(s32 value) { Inn_CheckIn(10, value); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov683 &&
      (u16)(gMainRoutineWork.entrance - 8) <= 1)
    return gData_02000780;
  if (GameFlag_Test(0x97f))
    return gData_020005ac;
  return gData_020003d8;
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

struct ActorPlacement gData_02000228[] = {
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

struct ActorPlacement gData_020003a8[] = {
    {74, CONDITION_ALWAYS, 3, PIXELS(608), 0, PIXELS(612), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_020003d8[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgThereSNot)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgEveryoneInChampa)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgWhenMyFather)},
    {0x8400, 11, CONDITION_ALWAYS, MSGNO(MsgMyDadWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgTheseRaftsAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIfDadHadn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIHopeDad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFatherWorriesSo)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTheTidalWave)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgItLookedTo)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgMostOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgTheFishAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgSomethingMustHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgYouHaveTo)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgWeVeSeen)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAGroupOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgMoreVisitorsMeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgNowThatI)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTheOceanHas)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIVeHeardRumors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgWeChampaRely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAllTheSailors)},
    {0xc400, 19, CONDITION_ALWAYS, EVENT(Func_0200008c)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIHaveSome)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgIfYouGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgOurProductsCost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgTheStuffIn)},
    {0xc400, 21, CONDITION_ALWAYS, EVENT(Func_02000098)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgListenIfYou)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgWeUsedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgItSA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgWatchingOurCatches)},
    {TRIGGERS_END},
};

struct Trigger gData_020005ac[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgWeDonT)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgItSTrue)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFatherSActing)},
    {0x8400, 11, CONDITION_ALWAYS, MSGNO(MsgWhenBriggsReturned)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIDRather)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfFatherSits)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgQuittingTheirJobs)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIHearThat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgWhileIM)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBriggsReallySaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgTheNerveOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIShouldJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIRefuseTo)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIVeBeenWondering)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgWeVillageElders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgANewImage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgObservingBriggsRecently)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgItSeemsThe)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIForOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgSureBriggsSaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgSupportingAnEntire)},
    {0xc400, 19, CONDITION_ALWAYS, EVENT(Func_0200008c)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheJewelsBriggs)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgThanksToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgBriggsSMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNowSThe)},
    {0xc400, 21, CONDITION_ALWAYS, EVENT(Func_02000098)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeStillHaven)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgItSNice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgMostPeopleHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgTheOnlyWay)},
    {TRIGGERS_END},
};

struct Trigger gData_02000780[] = {
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgDidYouKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIMSoGrateful)},
    {TRIGGERS_END},
};
